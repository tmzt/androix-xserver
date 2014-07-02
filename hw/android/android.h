
#include <jni.h>
#include <pthread.h>

#include "android/log.h"

#define LOG_TAG "AndroiX"
//define LOG(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

typedef struct _androidVars {
    pthread_mutex_t *events_lock;
    JavaVM *jvm;

    /* Screen size */
    int screenWidth;
    int screenHeight;

    /* Event wakeup socket */
    int wakeupFD[2];

    /* Global references established in androidInitNative */
    jclass AndroiXService_class;
    jclass AndroiXBlitView_class;
    jobject blitview;
    
    jmethodID initNativeScreen;
    jmethodID initNativeKeyboard;
    jmethodID initNativeMouse;
    jmethodID initNativeTrackball;
    jmethodID initFramebuffer;
    jmethodID draw;

} AndroidVars;

AndroidVars *Android;

/* defined in android component */
int androidRequestInputLock(void);
int androidReleaseInputLock(void);

void androidInitNative(JavaVM *jvm);
void androidInitNativeScreen(void *screen);
int androidInitNativeKeyboard(void *kbd);
int androidInitNativeMouse(void *mouse);
int androidInitNativeFramebuffer(void *base, void **buf, int width, int height, int depth);
void androidDraw(int x, int y, int w, int h);

/* callbacks */
void androidCallbackKeyDown(void *kbd, int keyCode);
void androidCallbackKeyUp(void *kbd, int keyCode);
void androidCallbackTouchDown(void *mouse, int x, int y);
void androidCallbackTouchMove(void *mouse, int x, int y);
void androidCallbackTouchUp(void *mouse, int x, int y);
void androidCallbackTrackballNormalizedMotion(void *ballPtr, double fx, double fy);
void androidCallbackTrackballPress(void *ballPtr);
void androidCallbackTrackballRelease(void *ballPtr);



