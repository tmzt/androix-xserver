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

#include <jni.h>
#include <pthread.h>

#include "android/log.h"

#define LOG_TAG "AndroiX"
//define LOG(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOG(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

typedef struct _androidVars {
    pthread_mutex_t *miEventQueueMutex;
    pthread_mutex_t *events_lock;
    JavaVM *jvm;

    /* Event wakeup socket */
//    int wakeupFD[2];
    int eventFD[2];

    /* Global references established in androidInitNative */
    jclass AndroiXService_class;
    jclass AndroiXBlitView_class;
    jobject blitview;
    
    jmethodID initNativeScreen;
    jmethodID initNativeKeyboard;
    jmethodID initNativeMouse;
    jmethodID initNativeTrackball;
    jmethodID initFramebuffer;
    jmethodID draw;

} AndroidVars;

AndroidVars *Android;

/* defined in android component */
int androidRequestInputLock(void);
int androidReleaseInputLock(void);

void androidInitNative(JavaVM *jvm);
void androidInitNativeScreen(void *screen);
int androidInitNativeKeyboard(void *kbd);
int androidInitNativeMouse(void *mouse);
int androidInitNativeFramebuffer(void *base, void **buf, int width, int height, int depth);
void androidDraw(int x, int y, int w, int h);

/* callbacks */
void androidCallbackKeyDown(void *kbd, int keyCode);
void androidCallbackKeyUp(void *kbd, int keyCode);
void androidCallbackTouchDown(void *mouse, int x, int y);
void androidCallbackTouchUp(void *mouse, int x, int y);
void androidCallbackTrackballNormalizedMotion(void *ballPtr, double fx, double fy);
void androidCallbackTrackballPress(void *ballPtr);
void androidCallbackTrackballRelease(void *ballPtr);



