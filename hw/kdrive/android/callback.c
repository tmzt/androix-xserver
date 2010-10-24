
#include <jni.h>

#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif
#include "kdandroid.h"
#include <inputstr.h>
#include <mi.h>

CARD32 lastEventTime = 0;
EventList *androidEvents;

void androidCallbackKeyDown(KdKeyboardInfo *kbd, int keyCode) {
    int i, n;

    LogMessage(X_DEFAULT, "[native] androidCallbackKeyDown: kbd: %.8x keyCode: %d", (unsigned int)kbd, keyCode);
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyDown: kbd->dixdev: %.8x", (unsigned int)(kbd->dixdev));
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyDown: kbd->dixdev->kbdfeed: %.8x", (unsigned int)(kbd->dixdev->kbdfeed));
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyDown: kbd->dixdev->key: %.8x", (unsigned int)(kbd->dixdev->key));

    GetEventList(&androidEvents);
    //lastEventTime = GetTimeInMillis();
    n = GetKeyboardEvents(androidEvents, kbd->dixdev, KeyPress, keyCode);
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyDown: n: %d", n);
    for (i = 0; i < n; i++) {
        mieqEnqueue(kbd->dixdev, (InternalEvent*)(androidEvents + i)->event);
        LogMessage(X_DEFAULT, "[native] androidCallbackKeyDown: enqueueing event KeyPress %d", keyCode);
    };

    //KdEnqueueKeyboardEvent (kbd, keyCode, FALSE);
}

void androidCallbackKeyUp(KdKeyboardInfo *kbd, int keyCode) {
    int i, n;

    LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: kbd: %p keyCode: %d", kbd, keyCode);
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: kbd->dixdev: %p", kbd->dixdev);
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: kbd->dixdev->kbdfeed: %p", kbd->dixdev->kbdfeed);
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: kbd->dixdev->key: %p", kbd->dixdev->key);

    GetEventList(&androidEvents);
    //lastEventTime = GetTimeInMillis();
    n = GetKeyboardEvents(androidEvents, kbd->dixdev, KeyRelease, keyCode);
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: n: %d", n);
    for (i = 0; i < n; i++)
        mieqEnqueue(kbd->dixdev, (InternalEvent*)(androidEvents + i)->event);
        LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: enqueueing event KeyRelease %d", keyCode);

    //KdEnqueueKeyboardEvent (kbd, keyCode, TRUE);
}

void androidCallbackTouchDown(KdPointerInfo *mouse, int x, int y) {
    int i, n;
    int v[3] = {x, y, 1};

    LogMessage(X_DEFAULT, "[native] androidCallbackTouchDown: mouse: %p x: %d y: %d", mouse, x, y);

    GetEventList(&androidEvents);
    n = GetPointerEvents(androidEvents, mouse->dixdev, MotionNotify, 1, POINTER_ABSOLUTE, 0, 1, v);
    LogMessage(X_DEFAULT, "[native] androidCallbackTouchDown: n: %d", n);
    for (i = 0; i < n; i++) {
        mieqEnqueue(mouse->dixdev, (InternalEvent*)(androidEvents + i)->event);
        LogMessage(X_DEFAULT, "[native] androidCallbackTouchDown: enqueueing event MotionNotify %d %d %d", x, y, 1);
    };
}

void androidCallbackTouchUp(KdPointerInfo *mouse, int x, int y) {
    int i, n;
    int v[3] = {x, y, 0};

    LogMessage(X_DEFAULT, "[native] androidCallbackTouchUp: mouse: %p x: %d y: %d", mouse, x, y);

    GetEventList(&androidEvents);
    n = GetPointerEvents(androidEvents, mouse->dixdev, MotionNotify, 1, POINTER_ABSOLUTE, 0, 1, v);
    LogMessage(X_DEFAULT, "[native] androidCallbackTouchUp: n: %d", n);
    for (i = 0; i < n; i++) {
        mieqEnqueue(mouse->dixdev, (InternalEvent*)(androidEvents + i)->event);
        LogMessage(X_DEFAULT, "[native] androidCallbackTouchUp: enqueueing event MotionNotify %d %d %d", x, y, 0);
    };
}

