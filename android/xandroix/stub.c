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

#include "../jni/android.h"

#include <unistd.h>
#include <stdio.h>

void androidInitNative(JavaVM *jvm) {
    LOG("[stub] androidInitNative");
};

void androidInitNativeScreen(void *screen) {
    LOG("[stub] androidInitNativeScreen: screen: %p", screen);
};

int androidInitNativeKeyboard(void *keyboard) {
    LOG("[stub] androidInitNativeKeyboard: keyboard: %p", keyboard);
    return 0;
};

int androidInitNativeMouse(void *mouse) {
    LOG("[stub] androidInitNativeMouse: mouse: %p", mouse);
    return 0;
};

int androidInitNativeFramebuffer(void *base, void **bufPtr, int width, int height, int depth) {
    void *buf = (void*)bufPtr;
    int bpp = depth/8;
    LOG("[stub] androidInitNativeFramebuffer: base: %p buf: %p width: %d height: %d bpp: %d", base, buf, width, height, bpp);
    return 0;
};

void androidDraw(int x, int y, int w, int h) {
    LOG("[stub] androidInitNativeFramebuffer: %d,%d,%d,%d", x, y, w, h);
};
