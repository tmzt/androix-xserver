
#include <inputstr.h>
#include <mi.h>
#include "private.h"

CARD32 lastEventTime = 0;
EventList *androidEvents;

#define ANDROIDCALLBACKABI 2    /* added trackball events */

int androidCallbackGetVersion(void) {
    return ANDROIDCALLBACKABI;
}

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
        LogMessage(X_DEFAULT, "[native] androidCallbackKeyDown: enqueueing event KeyPress %d", keyCode);
    };
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
        LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: enqueueing event KeyRelease %d", keyCode);
    };
}

void androidCallbackTouchDown(void *mousePtr, int x, int y) {
    int i, n;
    int v[3] = {x, y, 1};
    DeviceIntPtr mouse = mousePtr;

    LogMessage(X_DEFAULT, "[native] androidCallbackTouchDown: mouse: %p x: %d y: %d", mouse, x, y);

    GetEventList(&androidEvents);
    n = GetPointerEvents(androidEvents, mouse, MotionNotify | ButtonPress, 1, POINTER_ABSOLUTE, 0, 2, v);
    LogMessage(X_DEFAULT, "[native] andriodCallbackTouchDown mouse->enabled: %d", mouse->enabled);
    LogMessage(X_DEFAULT, "[native] androidCallbackTouchDown: n: %d", n);
    for (i = 0; i < n; i++) {
        mieqEnqueue(mouse, (InternalEvent*)(androidEvents + i)->event);
        LogMessage(X_DEFAULT, "[native] androidCallbackTouchDown: enqueueing event MotionNotify %d %d %d", x, y, 1);
    };
    n = GetPointerEvents(androidEvents, mouse, ButtonPress, 1, POINTER_ABSOLUTE, 0, 2, v);
    for (i = 0; i < n; i++) {
        mieqEnqueue(mouse, (InternalEvent*)(androidEvents + i)->event);
        LogMessage(X_DEFAULT, "[native] androidCallbackTouchDown: enqueueing event ButtonPress buttons: %d", 1);
    };

}

void androidCallbackTouchUp(void *mousePtr, int x, int y) {
    int i, n;
    int v[3] = {x, y, 0};
    DeviceIntPtr mouse = mousePtr;

    LogMessage(X_DEFAULT, "[native] androidCallbackTouchUp: mouse: %p x: %d y: %d", mouse, x, y);

    GetEventList(&androidEvents);
    n = GetPointerEvents(androidEvents, mouse, MotionNotify | ButtonRelease, 1, POINTER_ABSOLUTE, 0, 2, v);
    LogMessage(X_DEFAULT, "[native] androidCallbackTouchUp mouse->enable: %d", mouse->enabled);
    LogMessage(X_DEFAULT, "[native] androidCallbackTouchUp: n: %d", n);
    for (i = 0; i < n; i++) {
        mieqEnqueue(mouse, (InternalEvent*)(androidEvents + i)->event);
        LogMessage(X_DEFAULT, "[native] androidCallbackTouchUp: enqueueing event MotionNotify %d %d %d", x, y, 0);
    };
    n = GetPointerEvents(androidEvents, mouse, ButtonRelease, 1, POINTER_ABSOLUTE, 0, 2, v);
    for (i = 0; i < n; i++) {
        mieqEnqueue(mouse, (InternalEvent*)(androidEvents + i)->event);
        LogMessage(X_DEFAULT, "[native] androidCallbackTouchDown: enqueueing event ButtonPress buttons: %d", 1);
    };

}

void androidCallbackTrackballNormalizedMotion(void *ballPtr, double fx, double fy) {
    int i, n;
    int x, y;

//    x = (int)floor(fx * 800) % 800;
//    y = (int)floor(fy * 480) % 480;  
  
//    x = (int)(pow((abs(fx) * 6.01), 2.5);
//    y = (int)pow((abs(fy) * 6.01), 2.5);
  
    x = (int)((fx/2.5) * 250);
    y = (int)((fy/2.5) * 200);

    int v[3] = {x, y, 0};
    DeviceIntPtr ball = ballPtr;

    LogMessage(X_DEFAULT, "[native] androidCallbackTrackballNormalizedMotion: ball: %p x: %d y: %d", ball, x, y);

    GetEventList(&androidEvents);
    n = GetPointerEvents(androidEvents, ball, MotionNotify, 1, POINTER_RELATIVE, 0, 2, v);
    LogMessage(X_DEFAULT, "[native] androidCallbackTrackballNormalizedMotion ball->enable: %d", ball->enabled);
    LogMessage(X_DEFAULT, "[native] androidCallbackTrackballNormalizedMotion: n: %d", n);
    for (i = 0; i < n; i++) {
        mieqEnqueue(ball, (InternalEvent*)(androidEvents + i)->event);
        LogMessage(X_DEFAULT, "[native] androidCallbackTrackballNormalizedMotion: enqueueing event MotionNotify REL %d %d %d", x, y, 0);
    };
}

void androidCallbackTrackballPress(void *ballPtr) {
    int i, n;

    int v[1] = {1};
    DeviceIntPtr ball = ballPtr;

    LogMessage(X_DEFAULT, "[native] androidCallbackTrackballPress: ball: %p", ball);

    GetEventList(&androidEvents);
    n = GetPointerEvents(androidEvents, ball, ButtonPress, 1, POINTER_RELATIVE, 0, 1, v);
    LogMessage(X_DEFAULT, "[native] androidCallbackTrackballPress ball->enable: %d", ball->enabled);
    LogMessage(X_DEFAULT, "[native] androidCallbackTrackballPress: n: %d", n);
    for (i = 0; i < n; i++) {
        mieqEnqueue(ball, (InternalEvent*)(androidEvents + i)->event);
        LogMessage(X_DEFAULT, "[native] androidCallbackTrackballPress: enqueueing event ButtonPress");
    };
};

void androidCallbackTrackballRelease(void *ballPtr) {
    int i, n;

    int v[1] = {0};
    DeviceIntPtr ball = ballPtr;

    LogMessage(X_DEFAULT, "[native] androidCallbackTrackballRelease: ball: %p", ball);

    GetEventList(&androidEvents);
    n = GetPointerEvents(androidEvents, ball, ButtonRelease, 1, POINTER_RELATIVE, 0, 1, v);
    LogMessage(X_DEFAULT, "[native] androidCallbackTrackballPress ball->enable: %d", ball->enabled);
    LogMessage(X_DEFAULT, "[native] androidCallbackTrackballRelease: n: %d", n);
    for (i = 0; i < n; i++) {
        mieqEnqueue(ball, (InternalEvent*)(androidEvents + i)->event);
        LogMessage(X_DEFAULT, "[native] androidCallbackTrackballRelease: enqueueing event ButtonRelease");
    };
}

