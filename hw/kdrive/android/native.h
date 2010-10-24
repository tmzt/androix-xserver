
#ifndef _KDANDROID_NATIVE_H_
#define _KDANDROID_NATIVE_H_
#include <stdio.h>
#include <unistd.h>
#include "kdrive.h"

#if 1
int androidInitNative(AndroidPriv *priv);
void androidInitNativeScreen(void *screen);
int androidInitNativeKeyboard(void *kbd);
int androidInitNativeMouse(void *mouse);
int androidInitNativeFramebuffer(void *base, void *buf, int width, int height, int depth);
void androidDraw(void *screen, int x, int y, int w, int h);

/* callbacks */
void androidCallbackKeyDown(void *kbd, int keyCode);
void androidCallbackKeyUp(void *kbd, int keyCode);
void androidCallbackTouchDown(void *mouse, int x, int y);
void androidCallbackTouchUp(void *mouse, int x, int y);
#else
#include "../../../android/jni/android.h"

int androidInitNative(AndroidPriv *priv);
void androidInitNativeScreen(KdScreenInfo *pScreen);
int androidInitNativeKeyboard(KdKeyboardInfo *kbd);
int androidInitNativeMouse(KdPointerInfo *mouse);
int androidInitNativeFramebuffer(KdScreenInfo *pScreen, int width, int height, int depth);
void androidDraw(KdScreenInfo *screen, int x, int y, int w, int h);
#endif

#endif /* _KDANDROID_NATIVE_H_ */
