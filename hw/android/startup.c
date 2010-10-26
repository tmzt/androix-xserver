
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

DevPrivateKeyRec    androidScreenKeyRec;

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

    if(!dixRegisterPrivateKey(&androidScreenKeyRec, PRIVATE_SCREEN, 0)) return FALSE;

    priv = (AndroidScreenPriv *)malloc(sizeof(AndroidScreenPriv));
    dixSetPrivate(pScreen->devPrivates, &androidScreenKeyRec, priv);

    priv->visuals = (1 << TrueColor);
#define Mask(o,l)   (((1 << l) - 1) << o)
    priv->depth = 16;
    priv->bitsPerPixel = 16;
    priv->redMask = Mask (11, 5);
    priv->greenMask = Mask (5, 6);
    priv->blueMask = Mask (0, 5);

	miSetVisualTypesAndMasks (16, (1 << TrueColor),
        8, TrueColor, priv->redMask, priv->greenMask, priv->blueMask);

    miSetPixmapDepths();

    if (! fbScreenInit(pScreen,
                priv->base,                 // pointer to screen bitmap
                priv->width, priv->height,          // screen size in pixels
                dpi, dpi,                         // dots per inch
                priv->pitch/(priv->bitsPerPixel/8), // pixel width of framebuffer
                priv->bitsPerPixel))               // bits per pixel for screen
    {
        return FALSE;
    }

    pScreen->SaveScreen = AndroidSaveScreen;

    if (! fbPictureInit(pScreen, 0, 0)) {
        return FALSE;
    }

    pScreen->x = 0;
    pScreen->y = 0;

    androidInitNativeScreen(pScreen);
    androidInitNativeFramebuffer(priv->base, &(priv->buf), priv->width, priv->height, priv->depth);

    return TRUE;
}

void AbortDDX(void) { exit(-1); }
void OsVendorInit(void) {}
void OsVendorFatalError(void) {}

void InitOutput( ScreenInfo *pScreenInfo, int argc, char **argv ) {
    AddScreen(AndroidScreenInit, argc, argv);
}

int ddxProcessArgument(int argc, char *argv[], int i) { return 0; }

void ddxUseMsg(void) {
    /* we're started from jni, not the command line */
}

void ddxGiveUp(void) {
    LogMessage(X_ERROR, "[startup] AndroiX DIX Exiting");
}





