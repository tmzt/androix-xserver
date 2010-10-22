/*
 * Copyright © 2004 Keith Packard
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Keith Packard not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Keith Packard makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _KDANDROID_H_
#define _KDANDROID_H_
#include <stdio.h>
#include <unistd.h>
#include "kdrive.h"

#ifdef RANDR
#include "randrstr.h"
#endif

#include "private.h"
#include "native.h"

extern KdCardFuncs  androidFuncs;

Bool
androidInitialize (KdCardInfo *card, AndroidPriv *priv);

Bool
androidCardInit (KdCardInfo *card);

Bool
androidScreenInit (KdScreenInfo *screen);

Bool
androidScreenInitialize (KdScreenInfo *screen, AndroidScrPriv *scrpriv);
    
Bool
androidInitScreen (ScreenPtr pScreen);

Bool
androidFinishInitScreen (ScreenPtr pScreen);

Bool
androidCreateResources (ScreenPtr pScreen);

void
androidPreserve (KdCardInfo *card);

Bool
androidEnable (ScreenPtr pScreen);

Bool
androidDPMS (ScreenPtr pScreen, int mode);

void
androidDisable (ScreenPtr pScreen);

void
androidRestore (KdCardInfo *card);

void
androidScreenFini (KdScreenInfo *screen);

void
androidCardFini (KdCardInfo *card);

void
androidGetColors (ScreenPtr pScreen, int n, xColorItem *pdefs);

void
androidPutColors (ScreenPtr pScreen, int n, xColorItem *pdefs);

Bool
androidMapFramebuffer (KdScreenInfo *screen);

void *
androidWindowLinear (ScreenPtr	pScreen,
		   CARD32	row,
		   CARD32	offset,
		   int		mode,
		   CARD32	*size,
		   void		*closure);

void
androidSetScreenSizes (ScreenPtr pScreen);

Bool
androidUnmapFramebuffer (KdScreenInfo *screen);

Bool
androidSetShadow (ScreenPtr pScreen);

Bool
androidCreateColormap (ColormapPtr pmap);
    
#ifdef RANDR
Bool
androidRandRGetInfo (ScreenPtr pScreen, Rotation *rotations);

Bool
androidRandRSetConfig (ScreenPtr		pScreen,
		     Rotation		randr,
		     int		rate,
		     RRScreenSizePtr	pSize);
Bool
androidRandRInit (ScreenPtr pScreen);

#endif

extern KdPointerDriver androidPointerDriver;

extern KdKeyboardDriver	androidKeyboardDriver;

extern KdOsFuncs   androidOsFuncs;

#endif /* _KDANDROID_H_ */
