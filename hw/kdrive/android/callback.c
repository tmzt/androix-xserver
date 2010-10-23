
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
    lastEventTime = GetTimeInMillis();
    n = GetKeyboardEvents(androidEvents, kbd->dixdev, KeyPress, keyCode);
    for (i = 0; i < n; i++)
        mieqEnqueue(kbd->dixdev, (InternalEvent*)(androidEvents + i)->event);

    //KdEnqueueKeyboardEvent (kbd, keyCode, FALSE);
}

void androidCallbackKeyUp(KdKeyboardInfo *kbd, int keyCode) {
    int i, n;

    LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: kbd: %.8x keyCode: %d", (unsigned int)kbd, keyCode);
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: kbd->dixdev: %.8x", (unsigned int)(kbd->dixdev));
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: kbd->dixdev->kbdfeed: %.8x", (unsigned int)(kbd->dixdev->kbdfeed));
    LogMessage(X_DEFAULT, "[native] androidCallbackKeyUp: kbd->dixdev->key: %.8x", (unsigned int)(kbd->dixdev->key));

    GetEventList(&androidEvents);
    lastEventTime = GetTimeInMillis();
    n = GetKeyboardEvents(androidEvents, kbd->dixdev, KeyRelease, keyCode);
    for (i = 0; i < n; i++)
        mieqEnqueue(kbd->dixdev, (InternalEvent*)(androidEvents + i)->event);

    //KdEnqueueKeyboardEvent (kbd, keyCode, TRUE);
}


