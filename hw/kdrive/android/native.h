
#ifndef _KDANDROID_NATIVE_H_
#define _KDANDROID_NATIVE_H_
#include <stdio.h>
#include <unistd.h>
#include "kdrive.h"

#include <jni.h>

typedef struct _androidVars {
    JavaVM *jvm;

    /* Global references established in androidInitNative */
    jclass AndroiXService_class;
    jclass AndroiXBlitView_class;
    jobject blitview;
    
} AndroidVars;

typedef struct _androidNativeKeyboard {
    KdKeyboardInfo *kbd;
    void (*keyDown)(int keyCode);
    void (*keyUp)(int keyCode);
    /* expand */
} AndroidNativeKeyboard;

//extern Android *xdandroid_Android;

int androidInitNative(AndroidPriv *priv);
void androidInitNativeScreen(KdScreenInfo *pScreen);
int androidInitFramebuffer(AndroidPriv *priv, int width, int height, int depth);
void androidDraw(KdScreenInfo *screen, int x, int y, int w, int h);

#endif /* _KDANDROID_NATIVE_H_ */
