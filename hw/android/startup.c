
#include <X11/X.h>
#include <X11/Xproto.h>
#include "os.h"
#include "servermd.h"
#include "inputstr.h"
#include "scrnintstr.h"
#include "mibstore.h"		// mi backing store implementation
#include "mipointer.h"		// mi software cursor
#include "micmap.h"		// mi colormap code
#include "fb.h"			// fb framebuffer code
#include "site.h"
#include "globals.h"
#include "dix.h"
#include "xkbsrv.h"

#include <X11/extensions/XI.h>
#include <X11/extensions/XIproto.h>
#include "exevents.h"
#include "extinit.h"

//include "xserver-properties.h"

#include <sys/types.h>
#include <sys/time.h>
#include <sys/syslimits.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "android.h"
#include "screen.h"
#include "private.h"

//static DevScreenPrivateKeyRec androidScreenKeyRec;
//define androidScreenKey (&androidScreenKeyRec)

// Common pixmap formats
static PixmapFormatRec formats[] = {
        { 1,    1,      BITMAP_SCANLINE_PAD },
        { 4,    8,      BITMAP_SCANLINE_PAD },
        { 8,    8,      BITMAP_SCANLINE_PAD },
        { 15,   16,     BITMAP_SCANLINE_PAD },
        { 16,   16,     BITMAP_SCANLINE_PAD },
        { 24,   32,     BITMAP_SCANLINE_PAD },
        { 32,   32,     BITMAP_SCANLINE_PAD }
};
const int NUMFORMATS = sizeof(formats)/sizeof(formats[0]);

void AbortDDX(void) { exit(-1); }
void OsVendorInit(void) {}
void OsVendorFatalError(void) {}

void InitOutput( ScreenInfo *pScreenInfo, int argc, char **argv ) {
    int i;
    int numFormats = 0;

    pScreenInfo->imageByteOrder = IMAGE_BYTE_ORDER;
    pScreenInfo->bitmapScanlineUnit = BITMAP_SCANLINE_UNIT;
    pScreenInfo->bitmapScanlinePad = BITMAP_SCANLINE_PAD;
    pScreenInfo->bitmapBitOrder = BITMAP_BIT_ORDER;

    for(i = 0; i < NUMFORMATS; i++) {
        pScreenInfo->formats[i].depth = formats[i].depth;
        pScreenInfo->formats[i].bitsPerPixel = formats[i].bitsPerPixel;
        pScreenInfo->formats[i].scanlinePad = BITMAP_SCANLINE_PAD;
        numFormats++;
    }

    pScreenInfo->numPixmapFormats = numFormats;

    AddScreen(AndroidScreenInit, argc, argv);
}

int ddxProcessArgument(int argc, char *argv[], int i) { return 0; }

void ddxUseMsg(void) {
    /* we're started from jni, not the command line */
}

void ddxGiveUp(void) {
    LogMessage(X_ERROR, "[startup] AndroiX DIX Exiting");
}

