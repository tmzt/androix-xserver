
#include <jni.h>

#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif
#include "kdandroid.h"
#include <inputstr.h>

void androidCallbackKeyDown(KdKeyboardInfo *kbd, int keyCode) {
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyDown: kbd: %.8x keyCode: %d", (unsigned int)kbd, keyCode);
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyDown: kbd->dixdev: %.8x", (unsigned int)(kbd->dixdev));
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyDown: kbd->dixdev->kbdfeed: %.8x", (unsigned int)(kbd->dixdev->kbdfeed));
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyDown: kbd->dixdev->key: %.8x", (unsigned int)(kbd->dixdev->key));

    KdEnqueueKeyboardEvent (kbd, keyCode, FALSE);
}

void androidCallbackKeyUp(KdKeyboardInfo *kbd, int keyCode) {
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: kbd: %.8x keyCode: %d", (unsigned int)kbd, keyCode);
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: kbd->dixdev: %.8x", (unsigned int)(kbd->dixdev));
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: kbd->dixdev->kbdfeed: %.8x", (unsigned int)(kbd->dixdev->kbdfeed));
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: kbd->dixdev->key: %.8x", (unsigned int)(kbd->dixdev->key));

    KdEnqueueKeyboardEvent (kbd, keyCode, TRUE);
}


