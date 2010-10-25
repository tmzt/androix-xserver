
#include <android.h>

#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>

static pthread_mutex_t miEventQueueMutex = PTHREAD_MUTEX_INITIALIZER;

jint
JNI_OnLoad(JavaVM *jvm, void *reserved) {
//    xandroid_jvm = jvm; // pass to kdandroid driver
//    LOG("xandroid_jvm: %.8x", xandroid_jvm);

    Android = (AndroidVars *)calloc(sizeof(AndroidVars), 1);
    Android->miEventQueueMutex = &miEventQueueMutex;
    androidInitNative(jvm);

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

    LOG("creating event wakeup socket pair");
    socketpair(AF_UNIX, SOCK_STREAM, 0, &(Android->wakeupFD));
    LOG("remote (Xorg) socket: %d", Android->wakeupFD[0]);
    LOG("local (android) socket: %d", Android->wakeupFD[1]);
    
    LOG("starting DIX");
	dix_main(1, argv, envp);
    LOG("returning from DIX (this shouldn't happen)");
}

void wakeupFD() {
    int res;
   	char nullbyte=0;
	//  <daniels> oh, i ... er ... christ.
    LOG("writing to wakeupFD");
//	res = write(Android->wakeupFD[1], &nullbyte, sizeof(nullbyte));
    write(Android->wakeupFD[1], "X", 1);
    LOG("wrote %d bytes", res);
}

void
Java_net_homeip_ofn_androix_AndroiXLib_keyDown( JNIEnv* env, jobject thiz, jint kbd, jint keyCode )
{
    LOG("keyDown: kbd: %p keyCode: %d", kbd, keyCode);
    wakeupFD();

    //androidRequestInputLock();
//    LOG("keyDown: taking miEventQueueMutex");
//    pthread_mutex_lock(Android->miEventQueueMutex);
    androidCallbackKeyDown((void *)kbd, keyCode);
    wakeupFD();
    //androidReleaseInputLock();
//    LOG("keyDown: releasing miEventQueueMutex");
//    pthread_mutex_unlock(Android->miEventQueueMutex);
}

void
Java_net_homeip_ofn_androix_AndroiXLib_keyUp( JNIEnv* env, jobject thiz, jint kbd, jint keyCode )
{
    LOG("keyUp: kbd: %p keyCode: %d", (unsigned int)kbd, keyCode);
    //androidRequestInputLock();
//    LOG("keyUp: taking miEventQueueMutex");
//    pthread_mutex_lock(Android->miEventQueueMutex);
    androidCallbackKeyUp((void *)kbd, keyCode);
    wakeupFD();
    //androidReleaseInputLock();
//    LOG("keyUp: releasing miEventQueueMutex");
//    pthread_mutex_unlock(Android->miEventQueueMutex);
}

void
Java_net_homeip_ofn_androix_AndroiXLib_touchDown( JNIEnv* env, jobject thiz, jint mouse, jint x, jint y )
{
    LOG("touchDown: mouse: %p x: %d y: %d", mouse, x, y);
    //androidRequestInputLock();
//    LOG("touchDown: taking miEventQueueMutex");
//    pthread_mutex_lock(Android->miEventQueueMutex);
    androidCallbackTouchDown((void *)mouse, x, y);
    wakeupFD();
    //androidReleaseInputLock();
//    LOG("touchDown: releasing miEventQueueMutex");
//    pthread_mutex_unlock(Android->miEventQueueMutex);
}

void
Java_net_homeip_ofn_androix_AndroiXLib_touchUp( JNIEnv* env, jobject thiz, jint mouse, jint x, jint y )
{
    LOG("touchUp: mouse: %p x: %d y: %d", mouse, x, y);
    //androidRequestInputLock();
//    LOG("touchUp: taking miEventQueueMutex");
//    pthread_mutex_lock(Android->miEventQueueMutex);
    androidCallbackTouchUp((void *)mouse, x, y);
    wakeupFD();
    //androidReleaseInputLock();
//    LOG("touchUp: releasing miEventQueueMutex");
//    pthread_mutex_unlock(Android->miEventQueueMutex);
}

