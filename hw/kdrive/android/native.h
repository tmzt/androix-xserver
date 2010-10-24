
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

AndroidVars *Android;

int androidInitNative(AndroidPriv *priv);
void androidInitNativeScreen(KdScreenInfo *pScreen);
int androidInitNativeKeyboard(KdKeyboardInfo *kbd);
int androidInitNativeFramebuffer(KdScreenInfo *pScreen, int width, int height, int depth);
void androidDraw(KdScreenInfo *screen, int x, int y, int w, int h);

/* callbacks */
void androidCallbackKeyDown(KdKeyboardInfo *kbd, int keyCode);
void androidCallbackKeyUp(KdKeyboardInfo *kbd, int keyCode);
void androidCallbackTouchDown(KdPointerInfo *mouse, int x, int y);
void androidCallbackTouchUp(KdPointerInfo *mouse, int x, int y);

#endif /* _KDANDROID_NATIVE_H_ */
