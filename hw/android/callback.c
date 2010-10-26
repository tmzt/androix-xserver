
#include <jni.h>

#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif
#include "private.h"
#include <inputstr.h>
#include <mi.h>

CARD32 lastEventTime = 0;
EventList *androidEvents;

void androidCallbackKeyDown(void *kbdPtr, int keyCode) {
    int i, n;
    DeviceIntPtr kbd = kbdPtr;

    LogMessage(X_DEFAULT, "[native] androidCallbackKeyDown: kbd: %.8x keyCode: %d", (unsigned int)kbd, keyCode);

    GetEventList(&androidEvents);
    //lastEventTime = GetTimeInMillis();
    n = GetKeyboardEvents(androidEvents, kbd, KeyPress, keyCode);
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyDown: n: %d", n);
    for (i = 0; i < n; i++) {
        mieqEnqueue(kbd, (InternalEvent*)(androidEvents + i)->event);
//        mieqProcessDeviceEvent(kbd, (InternalEvent*)(androidEvents + i)->event, NULL);
        LogMessage(X_DEFAULT, "[native] androidCallbackKeyDown: enqueueing event KeyPress %d", keyCode);
    };

    //KdEnqueueKeyboardEvent (kbd, keyCode, FALSE);
}

void androidCallbackKeyUp(void *kbdPtr, int keyCode) {
    int i, n;
    DeviceIntPtr kbd = kbdPtr;

    LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: kbd: %p keyCode: %d", kbd, keyCode);

    GetEventList(&androidEvents);
    //lastEventTime = GetTimeInMillis();
    n = GetKeyboardEvents(androidEvents, kbd, KeyRelease, keyCode);
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: n: %d", n);
    for (i = 0; i < n; i++) {
        mieqEnqueue(kbd, (InternalEvent*)(androidEvents + i)->event);
//        mieqProcessDeviceEvent(kbd, (InternalEvent*)(androidEvents + i)->event, NULL);
        LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: enqueueing event KeyRelease %d", keyCode);
    };

    //KdEnqueueKeyboardEvent (kbd, keyCode, TRUE);
}

void androidCallbackTouchDown(void *mousePtr, int x, int y) {
    int i, n;
    int v[3] = {x, y, 1};
    DeviceIntPtr mouse = mousePtr;

    LogMessage(X_DEFAULT, "[native] androidCallbackTouchDown: mouse: %p x: %d y: %d", mouse, x, y);

    GetEventList(&androidEvents);
    n = GetPointerEvents(androidEvents, mouse, MotionNotify, 1, POINTER_ABSOLUTE, 0, 2, v);
    LogMessage(X_DEFAULT, "[native] andriodCallbackTouchDown mouse->enabled: %d", mouse->enabled);
    LogMessage(X_DEFAULT, "[native] androidCallbackTouchDown: n: %d", n);
    for (i = 0; i < n; i++) {
        mieqEnqueue(mouse, (InternalEvent*)(androidEvents + i)->event);
//        mieqProcessDeviceEvent(mouse, (InternalEvent*)(androidEvents + i)->event, NULL);
        LogMessage(X_DEFAULT, "[native] androidCallbackTouchDown: enqueueing event MotionNotify %d %d %d", x, y, 1);
    };
}

void androidCallbackTouchUp(void *mousePtr, int x, int y) {
    int i, n;
    int v[3] = {x, y, 0};
    DeviceIntPtr mouse = mousePtr;

    LogMessage(X_DEFAULT, "[native] androidCallbackTouchUp: mouse: %p x: %d y: %d", mouse, x, y);

    GetEventList(&androidEvents);
    n = GetPointerEvents(androidEvents, mouse, MotionNotify, 1, POINTER_ABSOLUTE, 0, 2, v);
    LogMessage(X_DEFAULT, "[native] androidCallbackTouchUp mouse->enable: %d", mouse->enabled);
    LogMessage(X_DEFAULT, "[native] androidCallbackTouchUp: n: %d", n);
    for (i = 0; i < n; i++) {
        mieqEnqueue(mouse, (InternalEvent*)(androidEvents + i)->event);
//        mieqProcessDeviceEvent(mouse, (InternalEvent*)(androidEvents + i)->event, NULL);
        LogMessage(X_DEFAULT, "[native] androidCallbackTouchUp: enqueueing event MotionNotify %d %d %d", x, y, 0);
    };
}

