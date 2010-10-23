
#ifndef _KDANDROID_NATIVE_H_
#define _KDANDROID_NATIVE_H_
#include <stdio.h>
#include <unistd.h>
#include "kdrive.h"

int androidInitNative(AndroidPriv *priv);
void androidInitNativeScreen(KdScreenInfo *pScreen);
int androidInitFramebuffer(AndroidPriv *priv, int width, int height, int depth);
void androidDraw(KdScreenInfo *screen, int x, int y, int w, int h);

#endif /* _KDANDROID_NATIVE_H_ */
