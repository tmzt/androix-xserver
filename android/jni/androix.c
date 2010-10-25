
#include <android.h>

#include <sys/stat.h>

jint
JNI_OnLoad(JavaVM *jvm, void *reserved) {
    xandroid_jvm = jvm; // pass to kdandroid driver
    LOG("xandroid_jvm: %.8x", xandroid_jvm);

    Android = (AndroidVars *)calloc(sizeof(AndroidVars), 1);
//    androidInitNative(jvm);

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
    LOG("xandroid_jni_env: %p", xandroid_jni_env);

    LOG("fixing up /data/data/net.homeip.ofn.androix/usr/bin/xkbcomp");

    chmod("/data/data/net.homeip.ofn.androix/usr/bin/xkbcomp", 0775);

    LOG("done.");

    stat("/data/data/net.homeip.ofn.androix/usr/bin/xkbcomp", &stats);

    LOG("/data/data/net.homeip.ofn.androix/usr/bin/xkbcomp mode: %o" + stats.st_mode);

    LOG("starting DIX");
	dix_main(1, argv, envp);
    LOG("returning from DIX (this shouldn't happen)");
}

void
Java_net_homeip_ofn_androix_AndroiXLib_keyDown( JNIEnv* env, jobject thiz, jint kbd, jint keyCode )
{
    LOG("keyDown: kbd: %p keyCode: %d", kbd, keyCode);
    androidRequestInputLock();
    androidCallbackKeyDown((void *)kbd, keyCode);
    androidReleaseInputLock();
}

void
Java_net_homeip_ofn_androix_AndroiXLib_keyUp( JNIEnv* env, jobject thiz, jint kbd, jint keyCode )
{
    LOG("keyUp: kbd: %p keyCode: %d", (unsigned int)kbd, keyCode);
    androidRequestInputLock();
    androidCallbackKeyUp((void *)kbd, keyCode);
    androidReleaseInputLock();
}

void
Java_net_homeip_ofn_androix_AndroiXLib_touchDown( JNIEnv* env, jobject thiz, jint mouse, jint x, jint y )
{
    LOG("touchDown: mouse: %p x: %d y: %d", mouse, x, y);
    androidRequestInputLock();
    androidCallbackTouchDown((void *)mouse, x, y);
    androidReleaseInputLock();
}

void
Java_net_homeip_ofn_androix_AndroiXLib_touchUp( JNIEnv* env, jobject thiz, jint mouse, jint x, jint y )
{
    LOG("touchUp: mouse: %p x: %d y: %d", mouse, x, y);
    androidRequestInputLock();
    androidCallbackTouchUp((void *)mouse, x, y);
    androidReleaseInputLock();
}

