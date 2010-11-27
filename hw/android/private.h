
#ifndef _DDXANDROID_PRIV_H_
#define _DDXANDROID_PRIV_H_
#include <stdio.h>
#include <unistd.h>

#ifdef RANDR
#include "randrstr.h"
#endif

#include <jni.h>

#include <inputstr.h>
#include <damage.h>

typedef struct _androidPriv {

    DeviceIntPtr keyboard;
    DeviceIntPtr mouse;

} AndroidPriv;

typedef struct _androidScreenPriv {
    Rotation	randr;
    DamagePtr       pDamage;
    ScreenInfo   *pScreenInfo;
    Bool(*wrappedCreateScreenResources)(ScreenPtr);

    CARD8	*base;
    jobject buf;
    int		bytes_per_line;
    int dpi;
    int pitch;
    int depth;
    int bitsPerPixel;
    int pixelStride;
    int byteStride;
    Bool shadow;
    unsigned long visuals;
    Pixel redMask, greenMask, blueMask;

    int width;
    int height;
} AndroidScreenPriv;

typedef struct _androidKeyboardPriv {
    Bool    enabled;
} AndroidKeyboardPriv;

typedef struct _androidPointerPriv {
    Bool    enabled;
} AndroidPointerPriv;

#endif /* _DDXANDROID_PRIV_H_ */
