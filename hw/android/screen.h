#include <X11/X.h>
#include <X11/Xproto.h>
#include "shadow.h"

extern DevPrivateKeyRec androidScreenKeyRec;
#define androidScreenKey (&androidScreenKeyRec)

Bool AndroidScreenInit(int index, ScreenPtr pScreen, int argc, char **argv);
Bool AndroidFinishScreenInit (int index, ScreenPtr pScreen, int argc, char **argv);
static Bool AndroidInitVisuals (ScreenPtr pScreen);

/* damage.c */
Bool androidSetShadow (ScreenPtr pScreen);
void androidShadowUpdate (ScreenPtr pScreen, shadowBufPtr pBuf);

Bool androidSetInternalDamage (ScreenPtr pScreen);
void androidUnsetInternalDamage (ScreenPtr pScreen);

