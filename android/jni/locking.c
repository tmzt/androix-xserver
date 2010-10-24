
#include "android.h"

#include <unistd.h>

int androidRequestInputLock() {
    /* Android is only used read only, if this doesn't work add pthread locking to it */
    JNIEnv *jni_env;   

    (*(Android->jvm))->AttachCurrentThread(Android->jvm, &jni_env, NULL);
    return ((*jni_env)->MonitorEnter(jni_env, Android->blitview) == JNI_OK);
};

int androidReleaseInputLock() {
    /* Android is only used read only, if this doesn't work add pthread locking to it */
    JNIEnv *jni_env;   

    (*(Android->jvm))->AttachCurrentThread(Android->jvm, &jni_env, NULL);
    return ((*jni_env)->MonitorExit(jni_env, Android->blitview) == JNI_OK);
};

