
#include <jni.h>

#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif
#include "kdandroid.h"

extern JavaVM *xandroid_jvm;
extern JNIEnv *xandroid_jni_env;

int androidInitNative(AndroidPriv *priv)
{
    jfieldID blitview_id;

    LogMessage(X_DEFAULT, "[native] xandroid_jvm: %.8x", (unsigned int)xandroid_jvm);
    LogMessage(X_DEFAULT, "[native] XXxandroid_jni_env: %.8x", (unsigned int)xandroid_jni_env);

    if (!priv->jvm)
        priv->jvm = xandroid_jvm;

    LogMessage(X_DEFAULT, "[native] priv->jvm: %.8x", (unsigned int)xandroid_jvm);

    if (!priv->jvm) {
        ErrorF("missing JavaVM");
        return -1;
    }

/*
    if (!priv->jni_env)
        priv->jni_env = xandroid_jni_env;

    if (!priv->jni_env) {
        ErrorF("missing JNIEnvironment");
        return -1;
    }
*/

    LogMessage(X_DEFAULT, "[native] priv->jni_env: %.8x", (unsigned int)xandroid_jni_env);


    priv->AndroiXService_class = (*(priv->jni_env))->FindClass(priv->jni_env, "net/homeip/ofn/androix/AndroiXService");
    LogMessage(X_DEFAULT, "[native] androidInitNative: AndroiXService_class: %.8x", (unsigned int)priv->AndroiXService_class);

//    blitview_id = (*(priv->jni_env))->GetStaticFieldID(priv->jni_env, priv->AndroiXService_class, "blitView", "Lnet/homeip/ofn/androix/AndroiXBlitView");
//    priv->blitview = (*(priv->jni_env))->GetStaticObjectField(priv->jni_env, priv->AndroiXService_class, blitview_id);
//    priv->AndroiXBlitView_class = (*(priv->jni_env))->GetObjectClass(priv->jni_env, priv->blitview);
//    priv->init = (*(priv->jni_env))->GetMethodID(priv->jni_env, priv->AndroiXService_class, "initFramebuffer", "(III)I");
//  priv->draw = (*(priv->jni_env))->GetMethodID(priv->jni_env, priv->AndroiXService_class, "(III)S");
}

int androidInitFramebuffer(AndroidPriv *priv, int width, int height, int depth)
{
    LogMessage(X_DEFAULT, "[native] androidInitFramebuffer");
    jfieldID blitview_id;

    blitview_id = (*(priv->jni_env))->GetStaticFieldID(priv->jni_env, priv->AndroiXService_class, "blitView", "Lnet/homeip/ofn/androix/AndroiXBlitView");
    priv->blitview = (*(priv->jni_env))->GetStaticObjectField(priv->jni_env, priv->AndroiXService_class, blitview_id);
    priv->AndroiXBlitView_class = (*(priv->jni_env))->GetObjectClass(priv->jni_env, priv->blitview);
    priv->init = (*(priv->jni_env))->GetMethodID(priv->jni_env, priv->AndroiXService_class, "initFramebuffer", "(III)");

    jint res = (*(priv->jni_env))->CallIntMethod(priv->jni_env, priv->blitview, priv->init, width, height, depth);
    return res;
}

void androidDraw(AndroidPriv *priv, int x, int y, int w, int h) {
    //(*(priv->jni_env))->CallVoidMethod(priv->jni_env, priv->blitview, priv->draw, x, y, w, h);
}


