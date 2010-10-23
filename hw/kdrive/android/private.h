
#ifndef _KDANDROID_PRIV_H_
#define _KDANDROID_PRIV_H_
#include <stdio.h>
#include <unistd.h>
#include "kdrive.h"

#ifdef RANDR
#include "randrstr.h"
#endif

#include <jni.h>

typedef struct _androidPriv {
    CARD8	*base;
    int		bytes_per_line;

    jobject buf;    // the java nio buffer corresponding to base

    JavaVM *jvm;

} AndroidPriv;

typedef struct _androidScreenPriv {
    Rotation	randr;
    Bool	shadow;
    DamagePtr   pDamage;
} AndroidScreenPriv;

typedef struct _androidKeyboardPriv {
    Bool    enabled;
} AndroidKeyboardPriv;

typedef struct _androidPointerPriv {
    Bool    enabled;
} AndroidPointerPriv;

#endif /* _KDANDROID_PRIV_H_ */
