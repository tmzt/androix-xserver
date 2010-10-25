
#ifndef _KDANDROID_NATIVE_H_
#define _KDANDROID_NATIVE_H_
#include <stdio.h>
#include <unistd.h>
#include "kdrive.h"

#include "../../../android/jni/android.h"

/* callbacks */
void androidCallbackKeyDown(void *kbd, int keyCode);
void androidCallbackKeyUp(void *kbd, int keyCode);
void androidCallbackTouchDown(void *mouse, int x, int y);
void androidCallbackTouchUp(void *mouse, int x, int y);

#endif /* _KDANDROID_NATIVE_H_ */
