
#include <X11/X.h>
#include <X11/Xproto.h>
#include "os.h"
#include "servermd.h"
#include "inputstr.h"
#include "scrnintstr.h"
#include "mibstore.h"		// mi backing store implementation
#include "mipointer.h"		// mi software cursor
#include "micmap.h"		// mi colormap code
#include "fb.h"			// fb framebuffer code
#include "site.h"
#include "globals.h"
#include "dix.h"
#include "xkbsrv.h"

#include <X11/extensions/XI.h>
#include <X11/extensions/XIproto.h>
#include "exevents.h"
#include "extinit.h"

//include "xserver-properties.h"

#include <sys/types.h>
#include <sys/time.h>
#include <sys/syslimits.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "android.h"
#include "private.h"

//static DevScreenPrivateKeyRec androidScreenKeyRec;
//define androidScreenKey (&androidScreenKeyRec)

static DevPrivateKeyRec androidScreenKeyRec;
#define androidScreenKey (&androidScreenKeyRec)

// Common pixmap formats
static PixmapFormatRec formats[] = {
        { 1,    1,      BITMAP_SCANLINE_PAD },
        { 4,    8,      BITMAP_SCANLINE_PAD },
        { 8,    8,      BITMAP_SCANLINE_PAD },
        { 15,   16,     BITMAP_SCANLINE_PAD },
        { 16,   16,     BITMAP_SCANLINE_PAD },
        { 24,   32,     BITMAP_SCANLINE_PAD },
        { 32,   32,     BITMAP_SCANLINE_PAD }
};
const int NUMFORMATS = sizeof(formats)/sizeof(formats[0]);


static Bool AndroidSaveScreen(ScreenPtr screen, int on) {
    /* tell Android to shutoff the display (based on dpms settings?) */
    return TRUE;
}

Bool DPMSSupported() { return TRUE; }

int DPMSSet(ClientPtr client, int level) {
    /* tell Android to shutoff the display */
    return TRUE;
}

static Bool AndroidScreenInit(int index, ScreenPtr pScreen, int argc, char **argv) {
    int dpi = 96; /* set from android */
    AndroidScreenPriv *priv;

    LogMessage(X_INFO, "[startup] AndroidScreenInit called: index: %d pScreen: %p", index, pScreen);

//    if(!dixRegisterPrivateKey(androidScreenKey, pScreen, PRIVATE_SCREEN, 0)) return FALSE;
    if(!dixRegisterPrivateKey(androidScreenKey, PRIVATE_SCREEN, 0)) {
        LogMessage(X_ERROR, "[startup] AndroidScreenInit: failed to register screen key");
    };
    LogMessage(X_INFO, "[startup] AndroidScreenInit: registered key: %p", androidScreenKey);

    priv = (AndroidScreenPriv *)malloc(sizeof(AndroidScreenPriv));

    LogMessage(X_INFO, "[startup] AndroidScreenInit: setting private: key: %p priv: %p", androidScreenKey, priv);
    dixSetPrivate(&(pScreen->devPrivates), androidScreenKey, priv);
//    dixSetScreenPrivate(pScreen->devPrivates, androidScreenKey, pScreen, priv);
//    dixSetScreenPrivate(PrivatePtr *privates, const DevScreenPrivateKey key, ScreenPtr pScreen, pointer val)

    LogMessage(X_INFO, "[startup] AndroidScreenInit: private set: %p (devPrivates: %p)", androidScreenKey, pScreen->devPrivates );

    pScreen->width = 800;
    pScreen->height = 480;

    priv->visuals = (1 << TrueColor);
#define Mask(o,l)   (((1 << l) - 1) << o)
    priv->depth = 16;
    priv->bitsPerPixel = 16;
    priv->redMask = Mask (11, 5);
    priv->greenMask = Mask (5, 6);
    priv->blueMask = Mask (0, 5);

    priv->bytes_per_line = ((pScreen->width * priv->bitsPerPixel + 31) >> 5) << 2;
    priv->base = NULL;
    free(priv->base);
    priv->base = malloc (priv->bytes_per_line * pScreen->height);
    LogMessage(X_INFO, "[startup] alloc framebuffer (%dx%d): %p", priv->bytes_per_line, pScreen->height, priv->base);

    priv->byteStride = priv->bytes_per_line;
    priv->pixelStride = (priv->bytes_per_line * 8/priv->bitsPerPixel);

    LogMessage(X_INFO, "[startup] AddScreen: priv->base: %p", priv->base);

    LogMessage(X_INFO, "[startup] AndroidScreenInit: miSetVisualTypes");
	miSetVisualTypesAndMasks (16, ((1 << TrueColor) | (1 << DirectColor)),
        8, TrueColor, priv->redMask, priv->greenMask, priv->blueMask);

    LogMessage(X_INFO, "[startup] AndroidScreenInit: miSetPixmapsDepths");
    miSetPixmapDepths();

    LogMessage(X_INFO, "[startup] AndroidScreenInit: fbScreenInit: pScreen: %p base: %p width: %d height: %d dpi: %d stride?: %d bpp: %d", pScreen, priv->base, pScreen->width, pScreen->height, dpi, priv->pitch/(priv->bitsPerPixel/8), priv->bitsPerPixel);
    if (! fbScreenInit(pScreen,
                priv->base,                 // pointer to screen bitmap
                pScreen->width, pScreen->height,          // screen size in pixels
                dpi, dpi,                         // dots per inch
                priv->pitch/(priv->bitsPerPixel/8), // pixel width of framebuffer
                priv->bitsPerPixel))               // bits per pixel for screen
    {
        LogMessage(X_ERROR, "[startup] AndroidScreenInit: fbScreenInit failed");
        return FALSE;
    }

    pScreen->SaveScreen = AndroidSaveScreen;

/*
    LogMessage(X_INFO, "[startup] fbFinishScreenInit");
    if (!fbFinishScreenInit (pScreen,
			     priv->base,
			     pScreen->width, pScreen->height,
			     monitorResolution, monitorResolution,
                 dpi, dpi,
			     priv->pixelStride,
			     priv->bitsPerPixel))
    {
	return FALSE;
    }
*/

    LogMessage(X_INFO, "[startup] AndroidScreenInit: pScreen->CreateScreenResources: %p", pScreen->CreateScreenResources);


    if (! fbPictureInit(pScreen, 0, 0)) {
        return FALSE;
    }

    pScreen->x = 0;
    pScreen->y = 0;

    fbCreateDefColormap(pScreen);

    LogMessage(X_INFO, "[startup] initNativeScreen: pScreen: %p", pScreen);
    androidInitNativeScreen(pScreen);
    LogMessage(X_INFO, "[startup] initNativeFramebuffer: base: %p buf: %p width: %d height: %d depth: %d", priv->base, &(priv->buf), pScreen->width, pScreen->height, priv->depth);
    androidInitNativeFramebuffer(priv->base, &(priv->buf), pScreen->width, pScreen->height, priv->depth);

    return TRUE;
}

void AbortDDX(void) { exit(-1); }
void OsVendorInit(void) {}
void OsVendorFatalError(void) {}

void InitOutput( ScreenInfo *pScreenInfo, int argc, char **argv ) {
    int i;
    int numFormats = 0;

    LogMessage(X_INFO, "[startup] InitOutput called");

    pScreenInfo->imageByteOrder = IMAGE_BYTE_ORDER;
    pScreenInfo->bitmapScanlineUnit = BITMAP_SCANLINE_UNIT;
    pScreenInfo->bitmapScanlinePad = BITMAP_SCANLINE_PAD;
    pScreenInfo->bitmapBitOrder = BITMAP_BIT_ORDER;

    for(i = 0; i < NUMFORMATS; i++) {
        pScreenInfo->formats[i].depth = formats[i].depth;
        pScreenInfo->formats[i].bitsPerPixel = formats[i].bitsPerPixel;
        pScreenInfo->formats[i].scanlinePad = BITMAP_SCANLINE_PAD;
        numFormats++;
    }

    pScreenInfo->numPixmapFormats = numFormats;

    AddScreen(AndroidScreenInit, argc, argv);
}

int ddxProcessArgument(int argc, char *argv[], int i) { return 0; }

void ddxUseMsg(void) {
    /* we're started from jni, not the command line */
}

void ddxGiveUp(void) {
    LogMessage(X_ERROR, "[startup] AndroiX DIX Exiting");
}

