
#include <jni.h>

#include "android/log.h"

#define LOG_TAG "AndroiX"
//define LOG(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

typedef struct _androidVars {
    JavaVM *jvm;

    /* Global references established in androidInitNative */
    jclass AndroiXService_class;
    jclass AndroiXBlitView_class;
    jobject blitview;
    
} AndroidVars;

/* defined in android component and shared with kdandroid */
JavaVM *xandroid_jvm;
JNIEnv *xandroid_jni_env;

/* defined in kdandroid and shared with android component */
extern AndroidVars *Android;

/* defined in android component */
int androidRequestInputLock(void);
int androidReleaseInputLock(void);

/* callbacks */
void androidCallbackKeyDown(void *kbd, int keyCode);
void androidCallbackKeyUp(void *kbd, int keyCode);
void androidCallbackTouchDown(void *mouse, int x, int y);
void androidCallbackTouchUp(void *mouse, int x, int y);
