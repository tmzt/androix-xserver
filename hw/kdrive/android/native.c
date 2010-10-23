
#include <jni.h>

#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif
#include "kdandroid.h"

extern JavaVM *xandroid_jvm;
extern JNIEnv *xandroid_jni_env;

static AndroidVars Android;

int androidInitNative(AndroidPriv *priv)
{
//    jfieldID blitview_id;

    LogMessage(X_DEFAULT, "[native] xandroid_jvm: %.8x", (unsigned int)xandroid_jvm);

    if (!priv->jvm)
        priv->jvm = xandroid_jvm;

    LogMessage(X_DEFAULT, "[native] priv->jvm: %.8x", (unsigned int)xandroid_jvm);

    if (!priv->jvm) {
        ErrorF("missing JavaVM");
        return -1;
    }

    /* save for drivers */
    Android.jvm = priv->jvm;

    return 0;
}

void androidInitNativeScreen(KdScreenInfo *screen) {
    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen");
    JNIEnv *jni_env;

    AndroidPriv *priv;

    jclass AndroiXService_class;
    jfieldID blitview_id;
    jclass AndroiXBlitView_class;
    jobject blitview;
    jmethodID initNativeScreen;

    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen: screen: %.8x", (unsigned int)screen);
    priv = screen->card->driver;
    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen: priv: %.8x", (unsigned int)priv);

    (*(priv->jvm))->AttachCurrentThread(priv->jvm, (void**)&jni_env, NULL);
    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen: jni_env: %.8x", (unsigned int)jni_env);

    AndroiXService_class = (*jni_env)->FindClass(jni_env, "net/homeip/ofn/androix/AndroiXService");
    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen: AndroiXService_class: %.8x", (unsigned int)AndroiXService_class);

    blitview_id = (*jni_env)->GetStaticFieldID(jni_env, AndroiXService_class, "blitView", "Lnet/homeip/ofn/androix/AndroiXBlitView;");
    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen: blitview_id: %.8x", (unsigned int)blitview_id);

    blitview = (*jni_env)->GetStaticObjectField(jni_env, AndroiXService_class, blitview_id);
    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen: blitview: %.8x", (unsigned int)blitview);

    AndroiXBlitView_class = (*jni_env)->GetObjectClass(jni_env, blitview);
    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen: AndroiXBlitView_class: %.8x", (unsigned int)blitview);

    initNativeScreen = (*jni_env)->GetMethodID(jni_env, AndroiXBlitView_class, "initNativeScreen", "(I)I");
    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen: initNativeScreen: %.8x", (unsigned int)initNativeScreen);

    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen: screen: %.8x", (unsigned int)screen);
    jint res = (*jni_env)->CallIntMethod(jni_env, blitview, initNativeScreen, screen);
    LogMessage(X_DEFAULT, "[native] androidInitNativeScreen: res: %.8x", (unsigned int)res);

    (*jni_env)->DeleteLocalRef(jni_env, blitview);
    (*jni_env)->DeleteLocalRef(jni_env, AndroiXBlitView_class);
    (*jni_env)->DeleteLocalRef(jni_env, AndroiXService_class);
}

void androidInitNativeKeyboard(KdKeyboardInfo *kbd) {
    LogMessage(X_DEFAULT, "[native] androidInitNativeKeyboard");
    JavaVM *jvm;
    JNIEnv *jni_env;

    jclass AndroiXService_class;
    jfieldID blitview_id;
    jclass AndroiXBlitView_class;
    jobject blitview;
    jmethodID initNativeKeyboard;

    LogMessage(X_DEFAULT, "[native] androidInitNativeKeyboard: kbd: %.8x", (unsigned int)kbd);




    jvm = Android.jvm;
    LogMessage(X_DEFAULT, "[native] androidInitNativeKeyboard: jvm: %.8x", (unsigned int)jvm);

    (*jvm)->AttachCurrentThread(jvm, (void**)&jni_env, NULL);
    LogMessage(X_DEFAULT, "[native] androidInitNativeKeyboard: jni_env: %.8x", (unsigned int)jni_env);

    AndroiXService_class = (*jni_env)->FindClass(jni_env, "net/homeip/ofn/androix/AndroiXService");
    LogMessage(X_DEFAULT, "[native] androidInitNativeKeyboard: AndroiXService_class: %.8x", (unsigned int)AndroiXService_class);

    blitview_id = (*jni_env)->GetStaticFieldID(jni_env, AndroiXService_class, "blitView", "Lnet/homeip/ofn/androix/AndroiXBlitView;");
    LogMessage(X_DEFAULT, "[native] androidInitNativeKeyboard: blitview_id: %.8x", (unsigned int)blitview_id);

    blitview = (*jni_env)->GetStaticObjectField(jni_env, AndroiXService_class, blitview_id);
    LogMessage(X_DEFAULT, "[native] androidInitNativeKeyboard: blitview: %.8x", (unsigned int)blitview);

    AndroiXBlitView_class = (*jni_env)->GetObjectClass(jni_env, blitview);
    LogMessage(X_DEFAULT, "[native] androidInitNativeKeyboard: AndroiXBlitView_class: %.8x", (unsigned int)blitview);

    initNativeKeyboard = (*jni_env)->GetMethodID(jni_env, AndroiXBlitView_class, "initNativeKeyboard", "(I)I");
    LogMessage(X_DEFAULT, "[native] androidInitNativeKeyboard: initNativeKeyboard: %.8x", (unsigned int)initNativeKeyboard);

    LogMessage(X_DEFAULT, "[native] androidInitNativeKeyboard: kbd: %.8x", (unsigned int)kbd);
    jint res = (*jni_env)->CallIntMethod(jni_env, blitview, initNativeKeyboard, kbd);
    LogMessage(X_DEFAULT, "[native] androidInitNativeKeyboard: res: %.8x", (unsigned int)res);

    (*jni_env)->DeleteLocalRef(jni_env, blitview);
    (*jni_env)->DeleteLocalRef(jni_env, AndroiXBlitView_class);
    (*jni_env)->DeleteLocalRef(jni_env, AndroiXService_class);
}

int androidInitFramebuffer(AndroidPriv *priv, int width, int height, int depth)
{
    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer");
    JNIEnv *jni_env;

    jclass AndroiXService_class;
    jfieldID blitview_id;
    jclass AndroiXBlitView_class;
    jobject blitview;
    jmethodID init;

    (*(priv->jvm))->AttachCurrentThread(priv->jvm, (void**)&jni_env, NULL);
    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer: jni_env: %.8x", (unsigned int)jni_env);

    AndroiXService_class = (*jni_env)->FindClass(jni_env, "net/homeip/ofn/androix/AndroiXService");
    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer: AndroiXService_class: %.8x", (unsigned int)AndroiXService_class);

    blitview_id = (*jni_env)->GetStaticFieldID(jni_env, AndroiXService_class, "blitView", "Lnet/homeip/ofn/androix/AndroiXBlitView;");
    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer: blitview_id: %.8x", (unsigned int)blitview_id);

    blitview = (*jni_env)->GetStaticObjectField(jni_env, AndroiXService_class, blitview_id);
    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer: blitview: %.8x", (unsigned int)blitview);

    AndroiXBlitView_class = (*jni_env)->GetObjectClass(jni_env, blitview);
    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer: AndroiXBlitView_class: %.8x", (unsigned int)blitview);
    init = (*jni_env)->GetMethodID(jni_env, AndroiXBlitView_class, "initFramebuffer", "(IIILjava/nio/ByteBuffer;)I");
    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer: init: %.8x", (unsigned int)init);

    int bpp = depth/8;
    priv->buf = (*jni_env)->NewDirectByteBuffer(jni_env, priv->base, (width*height*bpp));
    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer: priv->buf: %.8x", (unsigned int)priv->buf);

    jint res = (*jni_env)->CallIntMethod(jni_env, blitview, init, width, height, depth, priv->buf);
    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer: res: %.8x", (unsigned int)blitview);

    (*jni_env)->DeleteLocalRef(jni_env, blitview);
    (*jni_env)->DeleteLocalRef(jni_env, AndroiXBlitView_class);
    (*jni_env)->DeleteLocalRef(jni_env, AndroiXService_class);

    return res;
}

void androidDraw(KdScreenInfo *screen, int x, int y, int w, int h) {
    LogMessage(X_DEFAULT, "[native] androidDraw");
    JNIEnv *jni_env;

    AndroidPriv *priv;

    jclass AndroiXService_class;
    jfieldID blitview_id;
    jclass AndroiXBlitView_class;
    jobject blitview;
    jmethodID draw;

    LogMessage(X_DEFAULT, "[native] androidDraw: screen: %.8x", (unsigned int)screen);
    priv = screen->card->driver;
    LogMessage(X_DEFAULT, "[native] androidDraw: priv: %.8x", (unsigned int)priv);

    (*(priv->jvm))->AttachCurrentThread(priv->jvm, (void**)&jni_env, NULL);
    LogMessage(X_DEFAULT, "[native] androidDraw: jni_env: %.8x", (unsigned int)jni_env);

    AndroiXService_class = (*jni_env)->FindClass(jni_env, "net/homeip/ofn/androix/AndroiXService");
    LogMessage(X_DEFAULT, "[native] androidDraw: AndroiXService_class: %.8x", (unsigned int)AndroiXService_class);

    blitview_id = (*jni_env)->GetStaticFieldID(jni_env, AndroiXService_class, "blitView", "Lnet/homeip/ofn/androix/AndroiXBlitView;");
    LogMessage(X_DEFAULT, "[native] androidDraw: blitview_id: %.8x", (unsigned int)blitview_id);

    blitview = (*jni_env)->GetStaticObjectField(jni_env, AndroiXService_class, blitview_id);
    LogMessage(X_DEFAULT, "[native] androidDraw: blitview: %.8x", (unsigned int)blitview);

    AndroiXBlitView_class = (*jni_env)->GetObjectClass(jni_env, blitview);
    LogMessage(X_DEFAULT, "[native] androidDraw: AndroiXBlitView_class: %.8x", (unsigned int)blitview);
    draw = (*jni_env)->GetMethodID(jni_env, AndroiXBlitView_class, "draw", "(IIII)V");
    LogMessage(X_DEFAULT, "[native] androidDraw: init: %.8x", (unsigned int)blitview);

    (*jni_env)->CallVoidMethod(jni_env, blitview, draw, x, y, w, h);
    LogMessage(X_DEFAULT, "[native] androidDraw: draw finished");

    (*jni_env)->DeleteLocalRef(jni_env, blitview);
    (*jni_env)->DeleteLocalRef(jni_env, AndroiXBlitView_class);
    (*jni_env)->DeleteLocalRef(jni_env, AndroiXService_class);
}


