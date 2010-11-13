
#include "../android.h"
#include "../androidevents.h"

#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>

static pthread_mutex_t miEventQueueMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t events_lock = PTHREAD_MUTEX_INITIALIZER;

jint
JNI_OnLoad(JavaVM *jvm, void *reserved) {
    Android = (AndroidVars *)calloc(sizeof(AndroidVars), 1);
    Android->miEventQueueMutex = &miEventQueueMutex;
    Android->events_lock = &events_lock;
    androidInitNative(jvm);
    return JNI_VERSION_1_4;
}

void
Java_net_homeip_ofn_androix_AndroiXLib_init( JNIEnv* env, jobject thiz )
{
    struct stat stats;
    int mode = 0666;
    
    char argv[] = {":1"};
    char envp[] = {};

    LOG("fixing up /data/data/net.homeip.ofn.androix/usr/bin/xkbcomp");
    chmod("/data/data/net.homeip.ofn.androix/usr/bin/xkbcomp", 0775);
    LOG("done.");

    LOG("creating event wakeup socket pair");
    socketpair(AF_UNIX, SOCK_STREAM, 0, Android->eventFD);
//    pipe(Android->eventFD);
    LOG("remote (Xorg) fd: %d", Android->eventFD[0]);
    LOG("local (android) fd: %d", Android->eventFD[1]);
    
    LOG("starting DIX");
    #if 1 
	dix_main(1, &argv, &envp);
    #else
    	main(1, &argv, &envp);
    #endif
    LOG("returning from DIX (this shouldn't happen)");
}

void androidSendEvent(AndroidWireEvent *ev, size_t len)
{
    AndroidWireEvent wire;
    memcpy(&wire, ev, len);

    int res;
    LOG("writing event to eventFD");
    res = write(Android->eventFD[1], &wire, sizeof(AndroidWireEvent));
    LOG("wrote %d bytes", res);
}

void wakeupFD() {
    int res;
   	char nullbyte=0;
	//  <daniels> oh, i ... er ... christ.
    LOG("writing to eventFD");
	res = write(Android->eventFD[1], &nullbyte, sizeof(nullbyte));
    //res = write(Android->wakeupFD[1], "X", 1);
    LOG("wrote %d bytes", res);
}

void
Java_net_homeip_ofn_androix_AndroiXLib_keyDown( JNIEnv* env, jobject thiz, jint kbd, jint keyCode )
{
    AndroidWireKeyDownEvent ev = { ANDROIDWIREKEYDOWNEVENT, 0, (void *)kbd, keyCode };

    LOG("keyDown: kbd: %p keyCode: %d", kbd, keyCode);
    //LOG("keyDown TAKING LOCK");
    //pthread_mutex_lock(Android->events_lock);
    //androidCallbackKeyDown((void *)kbd, keyCode);
//    write(Android->eventFD[1], &ev, sizeof(AndroidWireEvent));
    androidSendEvent((AndroidWireEvent *)&ev, sizeof(AndroidWireKeyDownEvent));
    //LOG("keyDown RELEASIN LOCK");
    //pthread_mutex_unlock(Android->events_lock);
    wakeupFD();
}

void
Java_net_homeip_ofn_androix_AndroiXLib_keyUp( JNIEnv* env, jobject thiz, jint kbd, jint keyCode )
{
    AndroidWireKeyUpEvent ev = { ANDROIDWIREKEYUPEVENT, 0, (void *)kbd, keyCode };

    LOG("keyUp: kbd: %p keyCode: %d", (unsigned int)kbd, keyCode);
    //LOG("keyUp TAKING LOCK");
    //pthread_mutex_lock(Android->events_lock);
    //androidCallbackKeyUp((void *)kbd, keyCode);
//    write(Android->eventFD[1], &ev, sizeof(AndroidWireEvent));
    androidSendEvent((AndroidWireEvent *)&ev, sizeof(AndroidWireKeyUpEvent));
    //LOG("keyUp RELEASING LOCK");
    //pthread_mutex_unlock(Android->events_lock);
    wakeupFD();
}

void
Java_net_homeip_ofn_androix_AndroiXLib_touchDown( JNIEnv* env, jobject thiz, jint mouse, jint x, jint y )
{
    AndroidWireTouchDownEvent ev = { ANDROIDWIRETOUCHDOWNEVENT, 0, (void *)mouse, x, y };

    LOG("touchDown: mouse: %p x: %d y: %d", mouse, x, y);
    //pthread_mutex_lock(Android->events_lock);
    //androidCallbackTouchDown((void *)mouse, x, y);
//    write(Android->eventFD[1], &ev, sizeof(AndroidWireEvent));
    //pthread_mutex_unlock(Android->events_lock);
    wakeupFD();
}

void
Java_net_homeip_ofn_androix_AndroiXLib_touchUp( JNIEnv* env, jobject thiz, jint mouse, jint x, jint y )
{
    AndroidWireTouchDownEvent ev = { ANDROIDWIRETOUCHDOWNEVENT, 0, (void *)mouse, x, y };

    LOG("touchUp: mouse: %p x: %d y: %d", mouse, x, y);
    //pthread_mutex_lock(Android->events_lock);
    //androidCallbackTouchUp((void *)mouse, x, y);
//    write(Android->eventFD[1], &ev, sizeof(AndroidWireEvent));
    //pthread_mutex_unlock(Android->events_lock);
    wakeupFD();
}

void
Java_net_homeip_ofn_androix_AndroiXLib_trackballNormalizedMotion( JNIEnv* env, jobject thiz, jint ball, jfloat x, jfloat y )
{
    AndroidWireTrackballNormalizedMotionEvent ev = { ANDROIDWIRETRACKBALLNORMALIZEDMOTIONEVENT, 0, (void *)ball, (double)x, (double)y };

    LOG("trackballNormalizedMotion: ball: %p x: %d y: %d", ball, x, y);
    //pthread_mutex_lock(Android->events_lock);
    //androidCallbackTrackballNormalizedMotion((void *)ball, x, y);
//    write(Android->eventFD[1], &ev, sizeof(AndroidWireEvent));
    //pthread_mutex_unlock(Android->events_lock);
    wakeupFD();
}

void
Java_net_homeip_ofn_androix_AndroiXLib_trackballPress( JNIEnv* env, jobject thiz, jint ball )
{
    AndroidWireTrackballPressEvent ev = { ANDROIDWIRETRACKBALLPRESSEVENT, 0, (void *)ball };

    LOG("trackballPress: ball: %p", ball);
    //pthread_mutex_lock(Android->events_lock);
    //androidCallbackTrackballPress((void *)ball);
//    write(Android->eventFD[1], &ev, sizeof(AndroidWireEvent));
    //pthread_mutex_unlock(Android->events_lock);
    wakeupFD();
}

void
Java_net_homeip_ofn_androix_AndroiXLib_trackballRelease( JNIEnv* env, jobject thiz, jint ball )
{
    AndroidWireTrackballReleaseEvent ev = { ANDROIDWIRETRACKBALLRELEASEEVENT, 0, (void *)ball };

    LOG("trackballPress: ball: %p", ball);
    //pthread_mutex_lock(Android->events_lock);
    //androidCallbackTrackballRelease((void *)ball);
//    write(Android->eventFD[1], &ev, sizeof(AndroidWireEvent));
    //pthread_mutex_unlock(Android->events_lock);
    wakeupFD();
}

