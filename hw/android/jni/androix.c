
#include "../android.h"

#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>

static pthread_mutex_t events_lock = PTHREAD_MUTEX_INITIALIZER;

jint
JNI_OnLoad(JavaVM *jvm, void *reserved) {
    Android = (AndroidVars *)calloc(sizeof(AndroidVars), 1);
    Android->events_lock = &events_lock;
    androidInitNative(jvm);
    return JNI_VERSION_1_4;
}

void
Java_net_homeip_ofn_androix_AndroiXLib_init( JNIEnv* env, jobject thiz, int screenWidth, int screenHeight )
{
    struct stat stats;
    int mode = 0666;
    
    char argv[] = {":1"};
    char envp[] = {};

    LOG("[androix] init(%d, %d)", screenWidth, screenHeight);
    Android->screenWidth = screenWidth;
    Android->screenHeight = screenHeight;

    LOG("fixing up /data/data/net.homeip.ofn.androix/usr/bin/xkbcomp");
    chmod("/data/data/net.homeip.ofn.androix/usr/bin/xkbcomp", 0775);
    LOG("done.");

    LOG("creating event wakeup socket pair");
    socketpair(AF_UNIX, SOCK_STREAM, 0, Android->wakeupFD);
    LOG("remote (Xorg) fd: %d", Android->wakeupFD[0]);
    LOG("local (android) fd: %d", Android->wakeupFD[1]);
    
    LOG("starting DIX");
    #if 1 
	dix_main(1, &argv, &envp);
    #else
    	main(1, &argv, &envp);
    #endif
    LOG("returning from DIX (this shouldn't happen)");
}

void wakeupFD() {
    int res;
   	char nullbyte=0;
	//  <daniels> oh, i ... er ... christ.
    LOG("writing to wakeupFD");
	res = write(Android->wakeupFD[1], &nullbyte, sizeof(nullbyte));
    //res = write(Android->wakeupFD[1], "X", 1);
    LOG("wrote %d bytes", res);
}

void
Java_net_homeip_ofn_androix_AndroiXLib_keyDown( JNIEnv* env, jobject thiz, jint kbd, jint keyCode )
{
    LOG("keyDown: kbd: %p keyCode: %d", kbd, keyCode);
    LOG("keyDown TAKING LOCK");
    pthread_mutex_lock(Android->events_lock);
    androidCallbackKeyDown((void *)kbd, keyCode);
    LOG("keyDown RELEASIN LOCK");
    pthread_mutex_unlock(Android->events_lock);
    wakeupFD();
}

void
Java_net_homeip_ofn_androix_AndroiXLib_keyUp( JNIEnv* env, jobject thiz, jint kbd, jint keyCode )
{
    LOG("keyUp: kbd: %p keyCode: %d", (unsigned int)kbd, keyCode);
    LOG("keyUp TAKING LOCK");
    pthread_mutex_lock(Android->events_lock);
    androidCallbackKeyUp((void *)kbd, keyCode);
    LOG("keyUp RELEASING LOCK");
    pthread_mutex_unlock(Android->events_lock);
    wakeupFD();
}

void
Java_net_homeip_ofn_androix_AndroiXLib_touchDown( JNIEnv* env, jobject thiz, jint mouse, jint x, jint y )
{
    LOG("touchDown: mouse: %p x: %d y: %d", mouse, x, y);
    pthread_mutex_lock(Android->events_lock);
    androidCallbackTouchDown((void *)mouse, x, y);
    pthread_mutex_unlock(Android->events_lock);
    wakeupFD();
}

void
Java_net_homeip_ofn_androix_AndroiXLib_touchMove( JNIEnv* env, jobject thiz, jint mouse, jint x, jint y )
{
    LOG("touchMove: mouse: %p x: %d y: %d", mouse, x, y);
    pthread_mutex_lock(Android->events_lock);
    androidCallbackTouchMove((void *)mouse, x, y);
    pthread_mutex_unlock(Android->events_lock);
    wakeupFD();
}

void
Java_net_homeip_ofn_androix_AndroiXLib_touchUp( JNIEnv* env, jobject thiz, jint mouse, jint x, jint y )
{
    LOG("touchUp: mouse: %p x: %d y: %d", mouse, x, y);
    pthread_mutex_lock(Android->events_lock);
    androidCallbackTouchUp((void *)mouse, x, y);
    pthread_mutex_unlock(Android->events_lock);
    wakeupFD();
}

void
Java_net_homeip_ofn_androix_AndroiXLib_trackballNormalizedMotion( JNIEnv* env, jobject thiz, jint ball, jfloat x, jfloat y )
{
    LOG("trackballNormalizedMotion: ball: %p x: %d y: %d", ball, x, y);
    pthread_mutex_lock(Android->events_lock);
    androidCallbackTrackballNormalizedMotion((void *)ball, x, y);
    pthread_mutex_unlock(Android->events_lock);
    wakeupFD();
}

void
Java_net_homeip_ofn_androix_AndroiXLib_trackballPress( JNIEnv* env, jobject thiz, jint ball )
{
    LOG("trackballPress: ball: %p", ball);
    pthread_mutex_lock(Android->events_lock);
    androidCallbackTrackballPress((void *)ball);
    pthread_mutex_unlock(Android->events_lock);
    wakeupFD();
}

void
Java_net_homeip_ofn_androix_AndroiXLib_trackballRelease( JNIEnv* env, jobject thiz, jint ball )
{
    LOG("trackballPress: ball: %p", ball);
    pthread_mutex_lock(Android->events_lock);
    androidCallbackTrackballRelease((void *)ball);
    pthread_mutex_unlock(Android->events_lock);
    wakeupFD();
}

