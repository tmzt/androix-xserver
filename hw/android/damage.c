
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
    
void *
androidWindowLinear (ScreenPtr	pScreen,
		   CARD32	row,
		   CARD32	offset,
		   int		mode,
		   CARD32	*size,
		   void		*closure)
{
    AndroidScreenPriv *priv = dixLookupPrivate(&(pScreen->devPrivates), androidScreenKey);

//    if (!pScreenPriv->enabled)
//	return 0;
    *size = priv->bytes_per_line;
    return priv->base + row * priv->bytes_per_line;
}


Bool
androidSetShadow (ScreenPtr pScreen)
{
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

/* track damage and update the process */
/* based on Xephyr */

/* TODO convert to shadow if possible or integrate better with upstream */

void 
AndroidShadowUpdate (ScreenPtr pScreen, shadowBufPtr pBuf)
{
    AndroidScreenPriv *priv = dixLookupPrivate(&(pScreen->devPrivates), androidScreenKey);
  
    //LogMessage(X_INFO, "[ddxandroid damage] slow paint");
  
  /* FIXME: Slow Rotated/Reflected updates could be much
   * much faster efficiently updating via tranforming 
   * pBuf->pDamage  regions     
  */
  //shadowUpdateRotatePacked(pScreen, pBuf);

  //hostx_paint_rect(screen, 0,0,0,0, screen->width, screen->height);
  androidDraw(0, 0, /*?*/ pScreen->width, pScreen->height);
}

static void
AndroidInternalDamageRedisplay (ScreenPtr pScreen)
{
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
AndroidInternalDamageBlockHandler (pointer   data,
				 OSTimePtr pTimeout,
				 pointer   pRead)
{
  ScreenPtr pScreen = (ScreenPtr) data;
  
  AndroidInternalDamageRedisplay (pScreen);
}

static void
AndroidInternalDamageWakeupHandler (pointer data, int i, pointer LastSelectMask)
{
  /* FIXME: Not needed ? */
}

Bool
AndroidSetInternalDamage (ScreenPtr pScreen)
{
    AndroidScreenPriv *priv = dixLookupPrivate(&(pScreen->devPrivates), androidScreenKey);

  PixmapPtr      pPixmap = NULL;
  
  priv->pDamage = DamageCreate ((DamageReportFunc) 0,
				   (DamageDestroyFunc) 0,
				   DamageReportNone,
				   TRUE,
				   pScreen,
				   pScreen);
  
  if (!RegisterBlockAndWakeupHandlers (AndroidInternalDamageBlockHandler,
				       AndroidInternalDamageWakeupHandler,
				       (pointer) pScreen))
    return FALSE;
  
  pPixmap = (*pScreen->GetScreenPixmap) (pScreen);
  
  DamageRegister (&pPixmap->drawable, priv->pDamage);
      
  return TRUE;
}

void
AndroidUnsetInternalDamage (ScreenPtr pScreen)
{
    AndroidScreenPriv *priv = dixLookupPrivate(&(pScreen->devPrivates), androidScreenKey);

  PixmapPtr      pPixmap = NULL;
  
  pPixmap = (*pScreen->GetScreenPixmap) (pScreen);
  DamageUnregister (&pPixmap->drawable, priv->pDamage);
  DamageDestroy (priv->pDamage);
  
  RemoveBlockAndWakeupHandlers (AndroidInternalDamageBlockHandler,
				AndroidInternalDamageWakeupHandler,
				(pointer) pScreen);
}

