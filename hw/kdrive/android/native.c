
#include <jni.h>

#ifdef HAVE_CONFIG_H
#include <kdrive-config.h>
#endif
#include "kdandroid.h"

extern JNIEnv *xandroid_jni_env;

int androidInitNative(AndroidPriv *priv)
{
    jfieldid blitview_id;

    if (!priv->jni_env) {
        ErrorF("missing JNIEnvironment");
        return -1;
    }
      priv->AndroiXService_class = priv->jni_env->FindClass(priv->jni_env, "net/homeip/ofn/androix/AndroiXService");

      blitview_id = priv->jni_env->GetStaticFieldId(priv->jni_env, "blitView", "Lnet/homeip/ofn/androix/AndroiXBlitView");
      priv->blitview = priv->jni_env->GetStaticObjectField(priv->jni_env, priv->AndroiXService_class, blitview_id);
      priv->AndroiXBlitView_class = priv->jni_env->GetObjectClass(priv->jni_env, priv->blitview);
      priv->init = priv->jni_env->GetMethodID(priv->jni_env, priv->AndroiXService_class, "initFramebuffer", "(III)I");
//    priv->draw = priv->jni_env->GetMethodID(priv->jni_env, priv->AndroiXService_class, "(III)S");
}

int androidInitFramebuffer(AndroidPriv *priv, int width, int height, int depth)
{
    jint res = priv->jni_env->CallIntMethod(priv->jni_env, priv->blitview, priv->draw, x, y, height);
    return res;
}

void androidDraw(AndroidPriv *priv, int x, int y, int w, int h) {
    //priv->jni_env->CallVoidMethod(priv->jni_env, priv->blitview, priv->draw, x, y, w, h);
}


