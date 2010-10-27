
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
#include "screen.h"
#include "private.h"

DevPrivateKeyRec androidScreenKeyRec;


static Bool AndroidSaveScreen(ScreenPtr screen, int on) {
    /* tell Android to shutoff the display (based on dpms settings?) */
    return TRUE;
}

Bool DPMSSupported() { return TRUE; }

int DPMSSet(ClientPtr client, int level) {
    /* tell Android to shutoff the display */
    return TRUE;
}

static Bool AndroidCreateScreenResources(ScreenPtr pScreen) {
    Bool res;
    AndroidScreenPriv *priv = dixLookupPrivate(&(pScreen->devPrivates), androidScreenKey);

//    miCreateScreenResources(pScreen);
    res = priv->wrappedCreateScreenResources(pScreen);

/*
    if(!shadowAdd(pScreen, pScreen->devPrivate, AndroidShadowUpdate, NULL, 0, 0)) {
        LogMessage(X_ERROR, "[screen] AndroidCreateScreenResources: shadowAdd failed");
    }
*/


/*
    LogMessage(X_INFO, "[startup] AndroidCreateScreenResources: before androidSetShadow: pScreen->devPrivate: %p", pScreen->devPrivate);
    androidSetShadow(pScreen);
    LogMessage(X_INFO, "[startup] AndroidCreateScreenResources: after androidSetShadow: pScreen->devPrivate: %p", pScreen->devPrivate);
*/

    LogMessage(X_INFO, "[startup] AndroidCreateScreenResources: before androidSetInternalDamage: pScreen->devPrivate: %p", pScreen->devPrivate);

/*
    if(!shadowAdd(pScreen, pScreen->devPrivate, androidShadowUpdate, NULL, 0, 0)) {
        LogMessage(X_ERROR, "[screen] AndroidCreateScreenResources: shadowAdd failed");
    }
*/

//    androidSetShadow(pScreen);
    androidSetInternalDamage(pScreen);
    LogMessage(X_INFO, "[startup] AndroidCreateScreenResources: after androidSetInternalDamage: pScreen->devPrivate: %p", pScreen->devPrivate);


//    return res;
    return TRUE;
}

Bool AndroidScreenInit(int index, ScreenPtr pScreen, int argc, char **argv) {
    int dpi = 96; /* set from android */
    AndroidScreenPriv *priv;

    LogMessage(X_INFO, "[startup] AndroidScreenInit called: index: %d pScreen: %p", index, pScreen);

    if(!dixRegisterPrivateKey(androidScreenKey, PRIVATE_SCREEN, 0)) {
        LogMessage(X_ERROR, "[startup] AndroidScreenInit: failed to register screen key");
    };
    LogMessage(X_INFO, "[startup] AndroidScreenInit: registered key: %p", androidScreenKey);

    priv = (AndroidScreenPriv *)malloc(sizeof(AndroidScreenPriv));

    LogMessage(X_INFO, "[startup] AndroidScreenInit: setting private: key: %p priv: %p", androidScreenKey, priv);
    dixSetPrivate(&(pScreen->devPrivates), androidScreenKey, priv);

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

//    priv->bytes_per_line = ((pScreen->width * priv->bitsPerPixel + 31) >> 5) << 2;
    priv->bytes_per_line = 1600;
    priv->base = NULL;
    free(priv->base);
    priv->base = malloc (priv->bytes_per_line * pScreen->height);
    LogMessage(X_INFO, "[startup] alloc framebuffer (%dx%d): %p", priv->bytes_per_line, pScreen->height, priv->base);

    priv->byteStride = priv->bytes_per_line;
    priv->pixelStride = (priv->bytes_per_line * 8/priv->bitsPerPixel);

    LogMessage(X_INFO, "[startup] AddScreen: priv->base: %p", priv->base);

/*
    LogMessage(X_INFO, "[startup] AndroidScreenInit: miSetVisualTypes");
	miSetVisualTypesAndMasks (16, ((1 << TrueColor) | (1 << DirectColor)),
        8, TrueColor, priv->redMask, priv->greenMask, priv->blueMask);
*/

/*
    LogMessage(X_INFO, "[startup] AndroidScreenInit: miSetPixmapsDepths");
    miSetPixmapDepths();
*/

/*
    LogMessage(X_INFO, "[startup] AndroidScreenInit: fbScreenInit: pScreen: %p base: %p width: %d height: %d dpi: %d stride?: %d bpp: %d", pScreen, priv->base, pScreen->width, pScreen->height, dpi, priv->pitch/(priv->bitsPerPixel/8), priv->bitsPerPixel);
*/

/* moved fbScreenInit */

    pScreen->SaveScreen = AndroidSaveScreen;
    pScreen->CreateScreenResources = AndroidCreateScreenResources;

    LogMessage(X_INFO, "[startup] AndroidScreenInit: pScreen->CreateScreenResources: %p", pScreen->CreateScreenResources);

    miClearVisualTypes();

/* moved fbPictureInit */

    pScreen->x = 0;
    pScreen->y = 0;

/* moved   fbCreateDefColormap(pScreen); */

    AndroidFinishScreenInit(index, pScreen, argc, argv);

    LogMessage(X_INFO, "[startup] initNativeScreen: pScreen: %p", pScreen);
    androidInitNativeScreen(pScreen);
    LogMessage(X_INFO, "[startup] initNativeFramebuffer: base: %p buf: %p width: %d height: %d depth: %d", priv->base, &(priv->buf), pScreen->width, pScreen->height, priv->depth);
    androidInitNativeFramebuffer(priv->base, &(priv->buf), pScreen->width, pScreen->height, priv->depth);

    return TRUE;
}

Bool AndroidFinishScreenInit (int index, ScreenPtr pScreen, int argc, char **argv) {
    AndroidScreenPriv *priv = dixLookupPrivate(&(pScreen->devPrivates), androidScreenKey);
    char *pbits = NULL;

    /* allocate */

    if (!AndroidInitVisuals(pScreen)) {
        LogMessage(X_ERROR, "[screen] AndroidFinishScreenInitFB: AndroidInitVisuals failed");
        return FALSE;
    }

    int dpi = 100; /* get this from Android */
    if (! fbSetupScreen(pScreen,
                priv->base,                 // pointer to screen bitmap
                pScreen->width, pScreen->height,          // screen size in pixels
                dpi, dpi,                         // dots per inch
/*              priv->pitch/(priv->bitsPerPixel/8), // pixel width of framebuffer   */
                pScreen->width,
                priv->bitsPerPixel))               // bits per pixel for screen
    {
        LogMessage(X_ERROR, "[startup] AndroidScreenInit: fbScreenInit failed");
        return FALSE;
    }

    pbits = priv->base;
    miSetPixmapDepths();

    pScreen->SaveScreen = AndroidSaveScreen;

    if (!fbFinishScreenInit(pScreen,
                priv->base,                 // pointer to screen bitmap
                pScreen->width, pScreen->height,          // screen size in pixels
                dpi, dpi,                         // dots per inch
/*              priv->pitch/(priv->bitsPerPixel/8), // pixel width of framebuffer   */
                pScreen->width,
                priv->bitsPerPixel))               // bits per pixel for screen
    {
        LogMessage(X_ERROR, "[startup] AndroidScreenInit: fbFinishScreenInit failed");
        return FALSE;
    }

    fbCreateDefColormap(pScreen);
    
//    pScreen->BlockHandler = androidBlockHandler;
//    pScreen->WakeupHandler = androidWakeupHandler;
//    pScreen->blockData = pScreen;
//    pScreen->wakeupData = pScreen;

    if (! fbPictureInit(pScreen, 0, 0)) {
        LogMessage(X_ERROR, "[screen] AndroidFinishScreenInitFB: fbPictureInit failed");
        return FALSE;
    }

    /* randr */

    /* is this used?? */
    miInitializeBackingStore (pScreen);

    miDCInitialize(pScreen, &androidPointerCursorFuncs);

    if (!shadowSetup(pScreen))
	{
	  ErrorF ("[screen] AndroidFinishScreenInit: shadowSetup failed\n");
	  return FALSE;
	}

    priv->wrappedCreateScreenResources = pScreen->CreateScreenResources;
    pScreen->CreateScreenResources = AndroidCreateScreenResources;

    return TRUE;

}

/* from kdrive/epyhr */
/* BlockHandler/WakeHandler */

static Bool AndroidInitVisuals (ScreenPtr pScreen) {
    AndroidScreenPriv *priv = dixLookupPrivate(&(pScreen->devPrivates), androidScreenKey);

    if (!miSetVisualTypesAndMasks (priv->depth,
                     TrueColorMask,         
                     8,
                     -1,
                     priv->redMask,
                     priv->greenMask,
                     priv->blueMask))
	{
        LogMessage(X_ERROR, "[screen] AndroidInitVisuals: miSetVisualTypesAndMasks failed");
        return FALSE;
	}

    return TRUE;
}

static Bool androidCursorOffScreen(ScreenPtr *ppScreen, int *x, int *y)
{
    return FALSE;
}

static void androidCrossScreen(ScreenPtr pScreen, Bool entering)
{
}

static miPointerScreenFuncRec androidPointerCursorFuncs =
{
    androidCursorOffScreen,
    androidCrossScreen,
    miPointerWarpCursor
};


