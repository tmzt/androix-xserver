/*

Copyright 2010 Timothy Meade

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include "../android.h"

#define ANDROIDNATIVEABI 2    /* added trackball events */

int androidGetNativeVersion(void) {
    return ANDROIDNATIVEABI;
}

void androidInitNative(JavaVM *jvm) {
    JNIEnv *jni_env;
    jclass AndroiXService_class;
    jfieldID blitview_id;
    jclass AndroiXBlitView_class;
    jobject blitview;
    
    Android->jvm = jvm;
    
    (*(Android->jvm))->AttachCurrentThread(Android->jvm, &jni_env, NULL);                    
    AndroiXService_class = (*jni_env)->FindClass(jni_env, "net/homeip/ofn/androix/AndroiXService");
    LOG("[native] init: local AndroiXService_class: %p", AndroiXService_class);

    blitview_id = (*jni_env)->GetStaticFieldID(jni_env, AndroiXService_class, "blitView", "Lnet/homeip/ofn/androix/AndroiXBlitView;");
    blitview = (*jni_env)->GetStaticObjectField(jni_env, AndroiXService_class, blitview_id);
    LOG("[native] init: local blitview: %p", blitview);

    AndroiXBlitView_class = (*jni_env)->GetObjectClass(jni_env, blitview);
    LOG("[native] init: local AndroiXBlitView_class: %p", AndroiXBlitView_class);

    /* setup global references and cache methodIDs */
    Android->AndroiXService_class = (*jni_env)->NewGlobalRef(jni_env, AndroiXService_class);

    Android->AndroiXBlitView_class = (*jni_env)->NewGlobalRef(jni_env, AndroiXBlitView_class);
    Android->blitview = (*jni_env)->NewGlobalRef(jni_env, blitview);

    Android->initNativeScreen = (*jni_env)->GetMethodID(jni_env, Android->AndroiXBlitView_class, "initNativeScreen", "(I)I");
    Android->initNativeKeyboard = (*jni_env)->GetMethodID(jni_env, Android->AndroiXBlitView_class, "initNativeKeyboard", "(I)I");
    Android->initNativeMouse = (*jni_env)->GetMethodID(jni_env, Android->AndroiXBlitView_class, "initNativeMouse", "(I)I");
    Android->initNativeTrackball = (*jni_env)->GetMethodID(jni_env, Android->AndroiXBlitView_class, "initNativeTrackball", "(I)I");
    Android->initFramebuffer = (*jni_env)->GetMethodID(jni_env, Android->AndroiXBlitView_class, "initFramebuffer", "(IIILjava/nio/ByteBuffer;)I");
    Android->draw = (*jni_env)->GetMethodID(jni_env, Android->AndroiXBlitView_class, "draw", "(IIII)V");

    (*jni_env)->DeleteLocalRef(jni_env, AndroiXService_class);
    (*jni_env)->DeleteLocalRef(jni_env, AndroiXBlitView_class);
    (*jni_env)->DeleteLocalRef(jni_env, blitview);
};

void androidInitNativeScreen(void *screen) {
    JNIEnv *jni_env;
    (*(Android->jvm))->AttachCurrentThread(Android->jvm, &jni_env, NULL);
    (*jni_env)->CallIntMethod(jni_env, Android->blitview, Android->initNativeScreen, screen);
};

int androidInitNativeKeyboard(void *keyboard) {
    JNIEnv *jni_env;
    (*(Android->jvm))->AttachCurrentThread(Android->jvm, &jni_env, NULL);
    LOG("[native] androidInitNativeKeyboard: keyboard: %p", keyboard);
    return (*jni_env)->CallIntMethod(jni_env, Android->blitview, Android->initNativeKeyboard, keyboard);
};

int androidInitNativeMouse(void *mouse) {
    JNIEnv *jni_env;
    (*(Android->jvm))->AttachCurrentThread(Android->jvm, &jni_env, NULL);
    LOG("[native] androidInitNativeMouse: mouse: %p", mouse);
    return (*jni_env)->CallIntMethod(jni_env, Android->blitview, Android->initNativeMouse, mouse);
};

int androidInitNativeTrackball(void *ball) {
    JNIEnv *jni_env;
    (*(Android->jvm))->AttachCurrentThread(Android->jvm, &jni_env, NULL);
    LOG("[native] androidInitNativeTrackball: ball: %p", ball);
    return (*jni_env)->CallIntMethod(jni_env, Android->blitview, Android->initNativeTrackball, ball);
};

int androidInitNativeFramebuffer(void *base, void **bufPtr, int width, int height, int depth) {
    JNIEnv *jni_env;
    jobject *buf = (jobject *)bufPtr;
    (*(Android->jvm))->AttachCurrentThread(Android->jvm, &jni_env, NULL);

    int bpp = depth/8;
    LOG("[native] androidInitNativeFramebuffer: base: %p buf: %p width: %d height: %d bpp: %d", base, buf, width, height, bpp);

    buf = (*jni_env)->NewDirectByteBuffer(jni_env, base, (width*height*bpp));

    jint res = (*jni_env)->CallIntMethod(jni_env, Android->blitview, Android->initFramebuffer, width, height, depth, buf);
    return res;
};

void androidDraw(int x, int y, int w, int h) {
    JNIEnv *jni_env;
    jmethodID initNativeMouse;
    LOG("[native] androidDraw: %d,%d %d x %d", x, y, w, h);
    (*(Android->jvm))->AttachCurrentThread(Android->jvm, &jni_env, NULL);
    (*jni_env)->CallVoidMethod(jni_env, Android->blitview, Android->draw, x, y, w, h);
};

