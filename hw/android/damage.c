
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

#include "shadow.h"

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

#if 0
Bool
androidScreenInitialize (KdScreenInfo *screen, AndroidScreenPriv *scrpriv)
{
    if (!screen->width || !screen->height)
    {
	screen->width = 800;    //1024;
	screen->height = 480;   //768;
	screen->rate = 72;
    }

    if (screen->width <= 0)
	screen->width = 1;
    if (screen->height <= 0)
	screen->height = 1;
    
    if (!screen->fb.depth)
	screen->fb.depth = 16;

    if (screen->fb.depth <= 8)
    {
	screen->fb.visuals = ((1 << StaticGray) |
			      (1 << GrayScale) |
			      (1 << StaticColor) |
			      (1 << PseudoColor) |
			      (1 << TrueColor) |
			      (1 << DirectColor));
    }
    else 
    {
	screen->fb.visuals = (1 << TrueColor);
#define Mask(o,l)   (((1 << l) - 1) << o)
	if (screen->fb.depth <= 15)
	{
	    screen->fb.depth = 15;
	    screen->fb.bitsPerPixel = 16;
	    screen->fb.redMask = Mask (10, 5);
	    screen->fb.greenMask = Mask (5, 5);
	    screen->fb.blueMask = Mask (0, 5);
	}
	else if (screen->fb.depth <= 16)
	{
	    screen->fb.depth = 16;
	    screen->fb.bitsPerPixel = 16;
	    screen->fb.redMask = Mask (11, 5);
	    screen->fb.greenMask = Mask (5, 6);
	    screen->fb.blueMask = Mask (0, 5);
	}
	else
	{
	    screen->fb.depth = 24;
	    screen->fb.bitsPerPixel = 32;
	    screen->fb.redMask = Mask (16, 8);
	    screen->fb.greenMask = Mask (8, 8);
	    screen->fb.blueMask = Mask (0, 8);
	}
    }

    scrpriv->randr = screen->randr;

    return androidMapFramebuffer (screen);
}

Bool
androidScreenInit (KdScreenInfo *screen)
{
    AndroidScreenPriv *scrpriv;

    scrpriv = calloc(1, sizeof (AndroidScreenPriv));
    if (!scrpriv)
	return FALSE;
    screen->driver = scrpriv;
    if (!androidScreenInitialize (screen, scrpriv))
    {
	screen->driver = 0;
	free(scrpriv);
	return FALSE;
    }
    return TRUE;
}
#endif
    
void *
androidWindowLinear (ScreenPtr	pScreen,
		   CARD32	row,
		   CARD32	offset,
		   int		mode,
		   CARD32	*size,
		   void		*closure)
{
//    KdScreenPriv(pScreen);
//    AndroidPriv	    *priv = pScreenPriv->card->driver;
    AndroidScreenPriv *priv = dixLookupPrivate(&(pScreen->devPrivates), androidScreenKey);

//    if (!pScreenPriv->enabled)
//	return 0;
    *size = priv->bytes_per_line;
    return priv->base + row * priv->bytes_per_line;
}

#if 0
Bool
androidMapFramebuffer (KdScreenInfo *screen)
{
    AndroidScreenPriv	*scrpriv = screen->driver;
    KdPointerMatrix	m;
    AndroidPriv		*priv = screen->card->driver;

#if 1
    if (scrpriv->randr != RR_Rotate_0)
	scrpriv->shadow = TRUE;
    else
	scrpriv->shadow = FALSE;
#else
    scrpriv->shadow = TRUE;
#endif

    LogMessage(X_DEFAULT, "[native] androidMapFramebuffer: shadow: %d", (int)scrpriv->shadow);
    
    KdComputePointerMatrix (&m, scrpriv->randr, screen->width, screen->height);
    
    KdSetPointerMatrix (&m);
    
    priv->bytes_per_line = ((screen->width * screen->fb.bitsPerPixel + 31) >> 5) << 2;
    free(priv->base);
    priv->base = malloc (priv->bytes_per_line * screen->height);
    
    if (scrpriv->shadow)
    {
	if (!KdShadowFbAlloc (screen, scrpriv->randr & (RR_Rotate_90|RR_Rotate_270)))
	    return FALSE;
    }
    else
    {
        screen->fb.byteStride = priv->bytes_per_line;
        screen->fb.pixelStride = (priv->bytes_per_line * 8/
				     screen->fb.bitsPerPixel);
        screen->fb.frameBuffer = (CARD8 *) (priv->base);
    }

    /* initFramebuffer here for now */
//    androidInitNativeFramebuffer(screen, screen->width, screen->height, screen->fb.depth);
    LogMessage(X_DEFAULT, "[android] androidMapFramebuffer: initFramebuffer base: %p buf: %p width: %d height: %d depth: %d", priv->base, priv->buf, screen->width, screen->height, screen->fb.depth);
    androidInitNativeFramebuffer(priv->base, &(priv->buf), screen->width, screen->height, screen->fb.depth);
    
    return TRUE;
}

void
androidSetScreenSizes (ScreenPtr pScreen)
{
    KdScreenPriv(pScreen);
    KdScreenInfo	*screen = pScreenPriv->screen;
    AndroidScreenPriv	*scrpriv = screen->driver;

    if (scrpriv->randr & (RR_Rotate_0|RR_Rotate_180))
    {
	pScreen->width = screen->width;
	pScreen->height = screen->height;
	pScreen->mmWidth = screen->width_mm;
	pScreen->mmHeight = screen->height_mm;
    }
    else
    {
	pScreen->width = screen->width;
	pScreen->height = screen->height;
	pScreen->mmWidth = screen->height_mm;
	pScreen->mmHeight = screen->width_mm;
    }
}

Bool
androidUnmapFramebuffer (KdScreenInfo *screen)
{
    AndroidPriv		*priv = screen->card->driver;
    KdShadowFbFree (screen);
    if (priv->base)
    {
	free (priv->base);
	priv->base = 0;
    }
    return TRUE;
}
#endif

Bool
androidSetShadow (ScreenPtr pScreen)
{
//    KdScreenPriv(pScreen);
//    KdScreenInfo	*screen = pScreenPriv->screen;
//    AndroidScreenPriv	*scrpriv = screen->driver;
    AndroidScreenPriv *priv = dixLookupPrivate(&(pScreen->devPrivates), androidScreenKey);
    ShadowUpdateProc	update;
    ShadowWindowProc	window;

    window = androidWindowLinear;
    update = 0;
    if (priv->randr)
	update = shadowUpdateRotatePacked;
    else
	update = shadowUpdatePacked;
//    return KdShadowSet (pScreen, scrpriv->randr, update, window);

    if(!shadowAdd(pScreen, pScreen->devPrivate, update, window, 0, 0)) {
        LogMessage(X_ERROR, "[screen] AndroidCreateScreenResources: shadowAdd failed");
    }

}

#if defined(RANDR) && 0
Bool
androidRandRGetInfo (ScreenPtr pScreen, Rotation *rotations)
{
//    KdScreenPriv(pScreen);
//    KdScreenInfo	    *screen = pScreenPriv->screen;
//    AndroidScreenPriv	    *scrpriv = screen->driver;
    AndroidScreenPriv *priv = dixLookupPrivate(&(pScreen->devPrivates), androidScreenKey);

    RRScreenSizePtr	    pSize;
    Rotation		    randr;
    int			    n;
    
    *rotations = RR_Rotate_All|RR_Reflect_All;
    
    for (n = 0; n < pScreen->numDepths; n++)
	if (pScreen->allowedDepths[n].numVids)
	    break;
    if (n == pScreen->numDepths)
	return FALSE;
    
    pSize = RRRegisterSize (pScreen,
			    screen->width,
			    screen->height,
			    screen->width_mm,
			    screen->height_mm);
    
    randr = KdSubRotation (scrpriv->randr, screen->randr);
    
    RRSetCurrentConfig (pScreen, randr, 0, pSize);
    
    return TRUE;
}

Bool
androidRandRSetConfig (ScreenPtr		pScreen,
		     Rotation		randr,
		     int		rate,
		     RRScreenSizePtr	pSize)
{
    KdScreenPriv(pScreen);
    KdScreenInfo	*screen = pScreenPriv->screen;
    AndroidScreenPriv	*scrpriv = screen->driver;
    Bool		wasEnabled = pScreenPriv->enabled;
    AndroidScreenPriv	oldscr;
    int			oldwidth;
    int			oldheight;
    int			oldmmwidth;
    int			oldmmheight;
    int			newwidth, newheight;

    if (screen->randr & (RR_Rotate_0|RR_Rotate_180))
    {
	newwidth = pSize->width;
	newheight = pSize->height;
    }
    else
    {
	newwidth = pSize->height;
	newheight = pSize->width;
    }

    if (wasEnabled)
	KdDisableScreen (pScreen);

    oldscr = *scrpriv;
    
    oldwidth = screen->width;
    oldheight = screen->height;
    oldmmwidth = pScreen->mmWidth;
    oldmmheight = pScreen->mmHeight;
    
    /*
     * Set new configuration
     */
    
    scrpriv->randr = KdAddRotation (screen->randr, randr);

    androidUnmapFramebuffer (screen);
    
    if (!androidMapFramebuffer (screen))
	goto bail4;

    KdShadowUnset (screen->pScreen);

    if (!androidSetShadow (screen->pScreen))
	goto bail4;

    androidSetScreenSizes (screen->pScreen);

    /*
     * Set frame buffer mapping
     */
    (*pScreen->ModifyPixmapHeader) (fbGetScreenPixmap (pScreen),
				    pScreen->width,
				    pScreen->height,
				    screen->fb.depth,
				    screen->fb.bitsPerPixel,
				    screen->fb.byteStride,
				    screen->fb.frameBuffer);
    
    /* set the subpixel order */
    
    KdSetSubpixelOrder (pScreen, scrpriv->randr);
    if (wasEnabled)
	KdEnableScreen (pScreen);

    return TRUE;

bail4:
    androidUnmapFramebuffer (screen);
    *scrpriv = oldscr;
    (void) androidMapFramebuffer (screen);
    pScreen->width = oldwidth;
    pScreen->height = oldheight;
    pScreen->mmWidth = oldmmwidth;
    pScreen->mmHeight = oldmmheight;
    
    if (wasEnabled)
	KdEnableScreen (pScreen);
    return FALSE;
}

Bool
androidRandRInit (ScreenPtr pScreen)
{
    rrScrPrivPtr    pScrPriv;
    
    if (!RRScreenInit (pScreen))
	return FALSE;

    pScrPriv = rrGetScrPriv(pScreen);
    pScrPriv->rrGetInfo = androidRandRGetInfo;
    pScrPriv->rrSetConfig = androidRandRSetConfig;
    return TRUE;
}
#endif

#if 0
Bool
androidCreateColormap (ColormapPtr pmap)
{
    return fbInitializeColormap (pmap);
}

Bool
androidInitScreen (ScreenPtr pScreen)
{
    KdScreenPriv(pScreen);
    KdScreenInfo	*screen = pScreenPriv->screen;

#ifdef TOUCHSCREEN
    KdTsPhyScreen = pScreen->myNum;
#endif

    androidInitNativeScreen(screen);

    pScreen->CreateColormap = androidCreateColormap;
    return TRUE;
}

Bool
androidFinishInitScreen (ScreenPtr pScreen)
{
    if (!shadowSetup (pScreen))
	return FALSE;

#ifdef RANDR
    if (!androidRandRInit (pScreen))
	return FALSE;
#endif
    
    return TRUE;
}


Bool
androidCreateResources (ScreenPtr pScreen)
{
    KdScreenPriv(pScreen);
    KdScreenInfo	*screen    = pScreenPriv->screen;
    AndroidScreenPriv *scrpriv  = screen->driver;

    if (scrpriv->shadow)
        return KdShadowSet (pScreen,
            scrpriv->randr,
            androidShadowUpdate,
            androidWindowLinear
        );
    else
        return androidSetInternalDamage(pScreen);

    //return androidSetShadow (pScreen);
}
#endif

#if 0
void
androidGetColors (ScreenPtr pScreen, int n, xColorItem *pdefs)
{
    while (n--)
    {
	pdefs->red = 0;
	pdefs->green = 0;
	pdefs->blue = 0;
	pdefs++;
    }
}

void
androidPutColors (ScreenPtr pScreen, int n, xColorItem *pdefs)
{
}
#endif

/* track damage and update the process */
/* based on Xephyr */

void 
androidShadowUpdate (ScreenPtr pScreen, shadowBufPtr pBuf)
{
//  KdScreenPriv(pScreen);
//  KdScreenInfo *screen = pScreenPriv->screen;
    AndroidScreenPriv *priv = dixLookupPrivate(&(pScreen->devPrivates), androidScreenKey);
  
//  Android_LOG("slow paint");
  
  /* FIXME: Slow Rotated/Reflected updates could be much
   * much faster efficiently updating via tranforming 
   * pBuf->pDamage  regions     
  */
//  shadowUpdateRotatePacked(pScreen, pBuf);


  //hostx_paint_rect(screen, 0,0,0,0, screen->width, screen->height);
  //androidDraw(screen,0, 0, /*?*/ screen->width, screen->height);
  androidDraw(0, 0, /*?*/ pScreen->width, pScreen->height);
}

static void
androidInternalDamageRedisplay (ScreenPtr pScreen)
{
//  KdScreenPriv(pScreen);
//  KdScreenInfo	*screen = pScreenPriv->screen;
//  AndroidScreenPriv	*scrpriv = screen->driver;
  AndroidScreenPriv *priv = dixLookupPrivate(&(pScreen->devPrivates), androidScreenKey);

  RegionPtr	 pRegion;

  if (!priv || !priv->pDamage)
    return;

  pRegion = DamageRegion (priv->pDamage);

  if (RegionNotEmpty(pRegion))
    {
      int           nbox;
      BoxPtr        pbox;

      nbox = RegionNumRects (pRegion);
      pbox = RegionRects (pRegion);

      while (nbox--)
        {
/*
          hostx_paint_rect(screen,
                           pbox->x1, pbox->y1,
                           pbox->x1, pbox->y1,
                           pbox->x2 - pbox->x1,
                           pbox->y2 - pbox->y1);
*/
          androidDraw(pbox->x1, pbox->y1,
                      /* ? */
                      pbox->x2 - pbox->x1,
                      pbox->y2 - pbox->y1);
          pbox++;
        }
      DamageEmpty (priv->pDamage);
    }
}

static void
androidInternalDamageBlockHandler (pointer   data,
				 OSTimePtr pTimeout,
				 pointer   pRead)
{
  ScreenPtr pScreen = (ScreenPtr) data;
  
  androidInternalDamageRedisplay (pScreen);
}

static void
androidInternalDamageWakeupHandler (pointer data, int i, pointer LastSelectMask)
{
  /* FIXME: Not needed ? */
}

Bool
androidSetInternalDamage (ScreenPtr pScreen)
{
//  KdScreenPriv(pScreen);
//  KdScreenInfo	*screen = pScreenPriv->screen;
//  AndroidScreenPriv	*scrpriv = screen->driver;
    AndroidScreenPriv *priv = dixLookupPrivate(&(pScreen->devPrivates), androidScreenKey);

  PixmapPtr      pPixmap = NULL;
  
  priv->pDamage = DamageCreate ((DamageReportFunc) 0,
				   (DamageDestroyFunc) 0,
				   DamageReportNone,
				   TRUE,
				   pScreen,
				   pScreen);
  
  if (!RegisterBlockAndWakeupHandlers (androidInternalDamageBlockHandler,
				       androidInternalDamageWakeupHandler,
				       (pointer) pScreen))
    return FALSE;
  
  pPixmap = (*pScreen->GetScreenPixmap) (pScreen);
  
  DamageRegister (&pPixmap->drawable, priv->pDamage);
      
  return TRUE;
}

void
androidUnsetInternalDamage (ScreenPtr pScreen)
{
//  KdScreenPriv(pScreen);
//  KdScreenInfo	*screen = pScreenPriv->screen;
//  AndroidScreenPriv	*scrpriv = screen->driver;
    AndroidScreenPriv *priv = dixLookupPrivate(&(pScreen->devPrivates), androidScreenKey);

  PixmapPtr      pPixmap = NULL;
  
  pPixmap = (*pScreen->GetScreenPixmap) (pScreen);
  DamageUnregister (&pPixmap->drawable, priv->pDamage);
  DamageDestroy (priv->pDamage);
  
  RemoveBlockAndWakeupHandlers (androidInternalDamageBlockHandler,
				androidInternalDamageWakeupHandler,
				(pointer) pScreen);
}

