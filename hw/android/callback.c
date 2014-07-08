
#include <jni.h>
#include <inputstr.h>
#include <inpututils.h>
#include <mi.h>
#include "private.h"

#define ANDROIDCALLBACKABI 2    /* added trackball events */

int androidCallbackGetVersion(void) {
    return ANDROIDCALLBACKABI;
}

void androidCallbackKeyDown(void *kbdPtr, int keyCode) {
  LogMessage(X_DEFAULT, "[native] androidCallbackKeyDown: kbd: %.8x keyCode: %d", (unsigned int)kbdPtr, keyCode);
  QueueKeyboardEvents(kbdPtr, KeyPress, keyCode, NULL);
}

void androidCallbackKeyUp(void *kbdPtr, int keyCode) {
  LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: kbd: %p keyCode: %d", kbdPtr, keyCode);
  QueueKeyboardEvents(kbdPtr, KeyRelease, keyCode, NULL);
}

void androidCallbackTouchDown(void *mousePtr, int x, int y) {
  int valuators[2];
  ValuatorMask mask;

  LogMessage(X_DEFAULT, "[native] androidCallbackTouchDown: mouse: %p x: %d y: %d", mousePtr, x, y);
  /* Press */
  valuators[0] = x;
  valuators[1] = y;
  valuator_mask_set_range(&mask, 0, 2, valuators);
  QueuePointerEvents(mousePtr, ButtonPress, 1, POINTER_ABSOLUTE, &mask);
}

void androidCallbackTouchMove(void *mousePtr, int x, int y) {
  int valuators[2];
  ValuatorMask mask;

  LogMessage(X_DEFAULT, "[native] androidCallbackTouchMove: mouse: %p x: %d y: %d", mousePtr, x, y);
  /* Motion notify */
  valuators[0] = x;
  valuators[1] = y;
  valuator_mask_set_range(&mask, 0, 2, valuators);
  QueuePointerEvents(mousePtr, MotionNotify, 0, POINTER_ABSOLUTE, &mask);
}

void androidCallbackTouchUp(void *mousePtr, int x, int y) {
  int valuators[2];
  ValuatorMask mask;

  LogMessage(X_DEFAULT, "[native] androidCallbackTouchUp: mouse: %p x: %d y: %d", mousePtr, x, y);
  /* Press */
  /* Press */
  valuators[0] = x;
  valuators[1] = y;
  valuator_mask_set_range(&mask, 0, 2, valuators);
  QueuePointerEvents(mousePtr, ButtonRelease, 1, POINTER_ABSOLUTE, &mask);
}

void androidCallbackTrackballNormalizedMotion(void *ballPtr, double fx, double fy) {
#ifdef NEVER
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
#endif
}

void androidCallbackTrackballPress(void *ballPtr) {
#ifdef NEVER
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
#endif
};

void androidCallbackTrackballRelease(void *ballPtr) {
#ifdef NEVER
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
#endif
}

