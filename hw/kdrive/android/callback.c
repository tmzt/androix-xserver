
#include <jni.h>

#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif
#include "kdandroid.h"

void androidCallbackKeyDown(KdKeyboardInfo *kbd, int keyCode) {
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyDown: kbd: %.8x keyCode: %d", (unsigned int)kbd, keyCode);
    KdEnqueueKeyboardEvent (kbd, keyCode, FALSE);
}

void androidCallbackKeyUp(KdKeyboardInfo *kbd, int keyCode) {
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: kbd: %.8x keyCode: %d", (unsigned int)kbd, keyCode);
    KdEnqueueKeyboardEvent (kbd, keyCode, TRUE);
}


