
#include <jni.h>
#include <pthread.h>

#include "android/log.h"

#define LOG_TAG "AndroiX"
//define LOG(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

typedef struct _androidVars {
    pthread_mutex_t *miEventQueueMutex;
    JavaVM *jvm;

    /* Event wakeup socket */
    int wakeupFD[2];

    /* Global references established in androidInitNative */
    jclass AndroiXService_class;
    jclass AndroiXBlitView_class;
    jobject blitview;
    
    jmethodID initNativeScreen;
    jmethodID initNativeKeyboard;
    jmethodID initNativeMouse;
    jmethodID initFramebuffer;
    jmethodID draw;

} AndroidVars;

/* defined in android component and shared with kdandroid */
JavaVM *xandroid_jvm;
JNIEnv *xandroid_jni_env;

/* defined in kdandroid and shared with android component */

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
void androidCallbackTouchUp(void *mouse, int x, int y);


