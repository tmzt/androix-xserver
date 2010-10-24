#if 0
#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif
#include "kdandroid.h"

extern JavaVM *xandroid_jvm;
extern JNIEnv *xandroid_jni_env;

int androidInitNative(AndroidPriv *priv)
{
    JNIEnv *jni_env;
    jclass AndroiXService_class;
    jfieldID blitview_id;
    jclass AndroiXBlitView_class;
    jobject blitview;

    Android = (AndroidVars *)calloc(sizeof(AndroidVars), 1);

    LogMessage(X_DEFAULT, "[native] xandroid_jvm: %p", xandroid_jvm);
    priv->jvm = xandroid_jvm;
    LogMessage(X_DEFAULT, "[native] priv->jvm: %p", priv->jvm);

    /* save for drivers */
    LogMessage(X_DEFAULT, "[native] init: Android: %p", Android);
    Android->jvm = priv->jvm;
    LogMessage(X_DEFAULT, "[native] init: Android->jvm: %p", Android->jvm);

    (*(Android->jvm))->AttachCurrentThread(Android->jvm, &jni_env, NULL);
    LogMessage(X_DEFAULT, "[native] init: jni_env: %p", jni_env);
    AndroiXService_class = (*jni_env)->FindClass(jni_env, "net/homeip/ofn/androix/AndroiXService");
    LogMessage(X_DEFAULT, "[native] init: local AndroiXService_class: %p", AndroiXService_class);

    blitview_id = (*jni_env)->GetStaticFieldID(jni_env, AndroiXService_class, "blitView", "Lnet/homeip/ofn/androix/AndroiXBlitView;");
    blitview = (*jni_env)->GetStaticObjectField(jni_env, AndroiXService_class, blitview_id);
    LogMessage(X_DEFAULT, "[native] init: local blitview: %p", blitview);

    AndroiXBlitView_class = (*jni_env)->GetObjectClass(jni_env, blitview);
    LogMessage(X_DEFAULT, "[native] init: local AndroiXBlitView_class: %p", AndroiXBlitView_class);

    Android->AndroiXService_class = (*jni_env)->NewGlobalRef(jni_env, AndroiXService_class);
    Android->AndroiXBlitView_class = (*jni_env)->NewGlobalRef(jni_env, AndroiXBlitView_class);
    Android->blitview = (*jni_env)->NewGlobalRef(jni_env, blitview);

    return 0;
}

void androidInitNativeScreen(KdScreenInfo *screen) {
    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen");
    JNIEnv *jni_env;
    jmethodID initNativeScreen;

    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen: Android: %p", Android);
    (*(Android->jvm))->AttachCurrentThread(Android->jvm, &jni_env, NULL);
    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen: jni_env: %p", jni_env);

    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen: Android->AndroiXBlitView: %p", Android->AndroiXBlitView_class);
    initNativeScreen = (*jni_env)->GetMethodID(jni_env, Android->AndroiXBlitView_class, "initNativeScreen", "(I)I");

    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen: screen: %.8x", (unsigned int)screen);
    jint res = (*jni_env)->CallIntMethod(jni_env, Android->blitview, initNativeScreen, screen);
    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen: res: %.8x", (unsigned int)res);

}

int androidInitNativeKeyboard(KdKeyboardInfo *kbd) {
    LogMessage(X_DEFAULT, "[native] androidInitNativeKeyboard");
    JNIEnv *jni_env;
    jmethodID initNativeKeyboard;

    LogMessage(X_DEFAULT, "[native] androidInitNativeKeyboard: Android: %p", Android);
    (*(Android->jvm))->AttachCurrentThread(Android->jvm, &jni_env, NULL); 
    LogMessage(X_DEFAULT, "[native] androidInitNativeKeyboard: jni_env: %p", jni_env);

    initNativeKeyboard = (*jni_env)->GetMethodID(jni_env, Android->AndroiXBlitView_class, "initNativeKeyboard", "(I)I");

    LogMessage(X_DEFAULT, "[native] androidInitNativeKeyboard: kbd: %p", kbd);
    jint res = (*jni_env)->CallIntMethod(jni_env, Android->blitview, initNativeKeyboard, kbd);
    LogMessage(X_DEFAULT, "[native] androidInitNativeKeyboard: res: %d", res);
    return res;
}

int androidInitNativeMouse(KdPointerInfo *mouse) {
    LogMessage(X_DEFAULT, "[native] androidInitNativeMouse");
    JNIEnv *jni_env;
    jmethodID initNativeMouse;

    LogMessage(X_DEFAULT, "[native] androidInitNativeMouse: Android: %p", Android);
    (*(Android->jvm))->AttachCurrentThread(Android->jvm, &jni_env, NULL); 
    LogMessage(X_DEFAULT, "[native] androidInitNativeMouse: jni_env: %p", jni_env);

    initNativeMouse = (*jni_env)->GetMethodID(jni_env, Android->AndroiXBlitView_class, "initNativeMouse", "(I)I");

    LogMessage(X_DEFAULT, "[native] androidInitNativeMouse: mouse: %p", mouse);
    jint res = (*jni_env)->CallIntMethod(jni_env, Android->blitview, initNativeMouse, mouse);
    LogMessage(X_DEFAULT, "[native] androidInitNativeMouse: res: %d", res);
    return res;
}

int androidInitNativeFramebuffer(KdScreenInfo *screen, int width, int height, int depth)
{
    LogMessage(X_DEFAULT, "[native] androidInitNativeFramebuffer");
    JNIEnv *jni_env;
    AndroidPriv *android = screen->card->driver;
    AndroidScreenPriv *priv = screen->driver;

    priv->jvm = Android->jvm;
    priv->AndroiXBlitView_class = Android->AndroiXBlitView_class;
    priv->blitview = Android->blitview;
    jmethodID initFramebuffer;

    (*(Android->jvm))->AttachCurrentThread(priv->jvm, &jni_env, NULL);
    LogMessage(X_DEFAULT, "[native] androidInitNativeFramebuffer: jni_env: %.8x", (unsigned int)jni_env);

    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer: priv->AndroiXBlitView_class: %d", priv->AndroiXBlitView_class);
    priv->initFramebuffer = (*jni_env)->GetMethodID(jni_env, priv->AndroiXBlitView_class, "initFramebuffer", "(IIILjava/nio/ByteBuffer;)I");
    priv->draw = (*jni_env)->GetMethodID(jni_env, priv->AndroiXBlitView_class, "draw", "(IIII)V");

    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer: priv->initFramebuffer: %d", priv->initFramebuffer);
    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer: priv->draw: %d", priv->draw);

    int bpp = depth/8;
    android->buf = (*jni_env)->NewDirectByteBuffer(jni_env, android->base, (width*height*bpp));
    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer: android->buf: %p", android->buf);

    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer: priv->blitview: %p", priv->blitview);
    jint res = (*jni_env)->CallIntMethod(jni_env, priv->blitview, priv->initFramebuffer, width, height, depth, android->buf);
    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer: res: %d", res);
    return res;
}

void androidDraw(KdScreenInfo *screen, int x, int y, int w, int h) {
    LogMessage(X_DEFAULT, "[native] androidDraw");
    JNIEnv *jni_env;
    AndroidScreenPriv *priv = screen->driver;

    (*(priv->jvm))->AttachCurrentThread(priv->jvm, &jni_env, NULL);
    LogMessage(X_DEFAULT, "[native] androidDraw: jni_env: %.8x", (unsigned int)jni_env);

    //draw = (*jni_env)->GetMethodID(jni_env, priv->AndroiXBlitView_class, "draw", "(IIII)V");
    LogMessage(X_DEFAULT, "[native] androidDraw: blitview: %p", priv->blitview);
    LogMessage(X_DEFAULT, "[native] androidDraw: draw: %d", priv->draw);
    (*jni_env)->CallVoidMethod(jni_env, priv->blitview, priv->draw, x, y, w, h);
    LogMessage(X_DEFAULT, "[native] androidDraw: draw finished");
}
#endif
