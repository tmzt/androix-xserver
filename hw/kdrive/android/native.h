
#ifndef _KDANDROID_NATIVE_H_
#define _KDANDROID_NATIVE_H_
#include <stdio.h>
#include <unistd.h>
#include "kdrive.h"

int androidInitNative(AndroidPriv *priv);
int androidInitFramebuffer(AndroidPriv *priv, int width, int height, int depth);
void androidDraw(AndroidPriv *priv, int x, int y, int w, int h);

#endif /* _KDANDROID_NATIVE_H_ */
