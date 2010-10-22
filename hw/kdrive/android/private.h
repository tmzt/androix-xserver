
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

    JNIEnv *jni_env;

    jobject AndroiXService_instance;
    jclass AndroiXService_class;

    jobject blitview;
    jclass AndroixBlitView_class;

    jmethodID init;
    jmethodID draw;

} AndroidPriv;

typedef struct _androidScrPriv {
    Rotation	randr;
    Bool	shadow;
} AndroidScrPriv;

#endif /* _KDANDROID_PRIV_H_ */
