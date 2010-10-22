
#include <jni.h>

#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif
#include "kdandroid.h"

extern JavaVM *xandroid_jvm;
extern JNIEnv *xandroid_jni_env;

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

//    priv->AndroiXService_class = (*(priv->jni_env))->FindClass(priv->jni_env, "net/homeip/ofn/androix/AndroiXService");
//    LogMessage(X_DEFAULT, "[native] androidInitNative: AndroiXService_class: %.8x", (unsigned int)priv->AndroiXService_class);

//    blitview_id = (*(priv->jni_env))->GetStaticFieldID(priv->jni_env, priv->AndroiXService_class, "blitView", "Lnet/homeip/ofn/androix/AndroiXBlitView");
//    priv->blitview = (*(priv->jni_env))->GetStaticObjectField(priv->jni_env, priv->AndroiXService_class, blitview_id);
//    priv->AndroiXBlitView_class = (*(priv->jni_env))->GetObjectClass(priv->jni_env, priv->blitview);
//    priv->init = (*(priv->jni_env))->GetMethodID(priv->jni_env, priv->AndroiXService_class, "initFramebuffer", "(III)I");
//  priv->draw = (*(priv->jni_env))->GetMethodID(priv->jni_env, priv->AndroiXService_class, "(III)I");

    return 0;
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
    init = (*jni_env)->GetMethodID(jni_env, AndroiXBlitView_class, "initFramebuffer", "(III)I");
    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer: init: %.8x", (unsigned int)blitview);
    jint res = (*jni_env)->CallIntMethod(jni_env, blitview, init, width, height, depth);
    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer: res: %.8x", (unsigned int)blitview);
    return res;
}

void androidDraw(AndroidPriv *priv, int x, int y, int w, int h) {
    //(*(priv->jni_env))->CallVoidMethod(priv->jni_env, priv->blitview, priv->draw, x, y, w, h);
}


