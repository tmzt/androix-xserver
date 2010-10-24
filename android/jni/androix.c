
//include <string.h>
#include <jni.h>

#include <sys/stat.h>

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

/*
jstring
Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
    return (*env)->NewStringUTF(env, "Hello from JNI !");
}
*/

JavaVM *xandroid_jvm;
JNIEnv *xandroid_jni_env;
extern AndroidVars *Android;

jint
JNI_OnLoad(JavaVM *jvm, void *reserved) {
    xandroid_jvm = jvm; // pass to kdandroid driver
    LOG("xandroid_jvm: %.8x", xandroid_jvm);

    return JNI_VERSION_1_4;
}

void
Java_net_homeip_ofn_androix_AndroiXLib_init( JNIEnv* env, jobject thiz )
{
    struct stat stats;
    int mode = 0666;
    
    char *argv[] = {":1"};
    char *envp[] = {};

    xandroid_jni_env = env; // pass to kdandroid driver
    LOG("xandroid_jni_env: %.8x", xandroid_jni_env);

    LOG("fixing up /data/data/net.homeip.ofn.androix/usr/bin/xkbcomp");

    chmod("/data/data/net.homeip.ofn.androix/usr/bin/xkbcomp", 0775);

    LOG("done.");

    stat("/data/data/net.homeip.ofn.androix/usr/bin/xkbcomp", &stats);

    LOG("/data/data/net.homeip.ofn.androix/usr/bin/xkbcomp mode: " + stats.st_mode);

    LOG("starting DIX");
	dix_main(1, argv, envp);
    LOG("returning from DIX (this shouldn't happen)");
}

void
Java_net_homeip_ofn_androix_AndroiXLib_keyDown( JNIEnv* env, jobject thiz, jint kbd, jint keyCode )
{
    LOG("keyDown: kbd: %.8x keyCode: %d", (unsigned int)kbd, keyCode);
    requestInputLock(Android);
    androidCallbackKeyDown(kbd, keyCode);
    releaseInputLock(Android);
}

void
Java_net_homeip_ofn_androix_AndroiXLib_keyUp( JNIEnv* env, jobject thiz, jint kbd, jint keyCode )
{
    LOG("keyUp: kbd: %.8x keyCode: %d", (unsigned int)kbd, keyCode);
    requestInputLock(Android);
    androidCallbackKeyUp(kbd, keyCode);
    releaseInputLock(Android);
}

void
Java_net_homeip_ofn_androix_AndroiXLib_touchDown( JNIEnv* env, jobject thiz, jint mouse, jint x, jint y )
{
    LOG("touchDown: mouse: %p x: %d y: %d", mouse, x, y);
    requestInputLock(Android);
    androidCallbackTouchDown(mouse, x, y);
    releaseInputLock(Android);
}

void
Java_net_homeip_ofn_androix_AndroiXLib_touchUp( JNIEnv* env, jobject thiz, jint mouse, jint x, jint y )
{
    LOG("touchUp: mouse: %p x: %d y: %d", mouse, x, y);
    requestInputLock(Android);
    androidCallbackKeyUp(mouse, x, y);
    releaseInputLock(Android);
}






