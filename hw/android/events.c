/*

Copyright 1993, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from The Open Group.

*/

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <X11/X.h>
#include "mi.h"
#include <X11/Xproto.h>
#include "scrnintstr.h"
#include "inputstr.h"
#include <X11/Xos.h>
#include "mibstore.h"
#include "mipointer.h"
#include "xkbsrv.h"
#include <X11/keysym.h>
#include "xserver-properties.h"
#include "exevents.h"
#include "extinit.h"

#include <X11/extensions/XI.h>
#include <X11/extensions/XIproto.h>
#include "XIstubs.h" /* even though we don't use stubs.  cute, no? */
#include "exevents.h"
#include "extinit.h"
#include "exglobals.h"
#include "eventstr.h"
#include "xserver-properties.h"

#include "android.h"

Bool
LegalModifier(unsigned int key, DeviceIntPtr pDev)
{
    return TRUE;
}

void ProcessInputEvents(void) {
    char nullbyte;
	int x = sizeof(nullbyte);
    
//    TA_SERVER();

    LogMessage(X_INFO, "[events] before mieqProcessInputEvents();");
    mieqProcessInputEvents();
    LogMessage(X_INFO, "[events] after mieqProcessInputEvents();");

/*
    // Empty the signaling pipe
    while (x == sizeof(nullbyte)) {
      x = read(Android->wakeupFD[0], &nullbyte, sizeof(nullbyte));
      LogMessage(X_INFO, "[events] read %d bytes (nullbyte %c)", x, nullbyte);
    }
*/

    x = read(Android->wakeupFD[0], &nullbyte, 1);
    LogMessage(X_INFO, "[events] read %d bytes (nullbyte %c)", x, nullbyte);
}

void DDXRingBell(int volume, int pitch, int duration)
{
}

#define VFB_MIN_KEY 8
#define VFB_MAX_KEY 255

static int
androidKeybdProc(DeviceIntPtr pDevice, int onoff)
{
    DevicePtr pDev = (DevicePtr)pDevice;

    switch (onoff)
    {
    case DEVICE_INIT:
	InitKeyboardDeviceStruct(pDevice, NULL, NULL, NULL);
        break;
    case DEVICE_ON:
	pDev->on = TRUE;
    LogMessage(X_DEFAULT, "[events] initNativeKeyboard: pDevice: %p", pDevice);
    androidInitNativeKeyboard(pDevice);
	break;
    case DEVICE_OFF:
	pDev->on = FALSE;
	break;
    case DEVICE_CLOSE:
	break;
    }
    return Success;
}

static int
androidMouseProc(DeviceIntPtr pDevice, int onoff)
{
#define NBUTTONS 3
#define NAXES 2

    BYTE map[NBUTTONS + 1];
    DevicePtr pDev = (DevicePtr)pDevice;
    Atom btn_labels[NBUTTONS] = {0};
    Atom axes_labels[NAXES] = {0};

    switch (onoff)
    {
    case DEVICE_INIT:
	    map[1] = 1;
	    map[2] = 2;
	    map[3] = 3;

            btn_labels[0] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_LEFT);
            btn_labels[1] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_MIDDLE);
            btn_labels[2] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_RIGHT);

            axes_labels[0] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_X);
            axes_labels[1] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_Y);

	    InitPointerDeviceStruct(pDev, map, NBUTTONS, btn_labels,
		(PtrCtrlProcPtr)NoopDDA, GetMotionHistorySize(), NAXES, axes_labels);
	    break;

    case DEVICE_ON:
	pDev->on = TRUE;
    LogMessage(X_DEFAULT, "[events] initNativeMouse: pDevice: %p", pDevice);
    androidInitNativeMouse(pDev);
        break;

    case DEVICE_OFF:
	pDev->on = FALSE;
	break;

    case DEVICE_CLOSE:
	break;
    }
    return Success;

#undef NBUTTONS
#undef NAXES
}

static int
androidTrackballProc(DeviceIntPtr pDevice, int onoff)
{
#define NBUTTONS 3
#define NAXES 2

    BYTE map[NBUTTONS + 1];
    DevicePtr pDev = (DevicePtr)pDevice;
    Atom btn_labels[NBUTTONS] = {0};
    Atom axes_labels[NAXES] = {0};

    switch (onoff)
    {
    case DEVICE_INIT:
	    map[1] = 1;
	    map[2] = 2;
	    map[3] = 3;

            btn_labels[0] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_LEFT);
            btn_labels[1] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_MIDDLE);
            btn_labels[2] = XIGetKnownProperty(BTN_LABEL_PROP_BTN_RIGHT);

            axes_labels[0] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_X);
            axes_labels[1] = XIGetKnownProperty(AXIS_LABEL_PROP_REL_Y);

	    InitPointerDeviceStruct(pDev, map, NBUTTONS, btn_labels,
		(PtrCtrlProcPtr)NoopDDA, GetMotionHistorySize(), NAXES, axes_labels);
	    break;

    case DEVICE_ON:
	pDev->on = TRUE;
    LogMessage(X_DEFAULT, "[events] initNativeTrackball: pDevice: %p", pDevice);
    androidInitNativeTrackball(pDev);
        break;

    case DEVICE_OFF:
	pDev->on = FALSE;
	break;

    case DEVICE_CLOSE:
	break;
    }
    return Success;

#undef NBUTTONS
#undef NAXES
}

void
InitInput(int argc, char *argv[])
{
    DeviceIntPtr p, t, k;
    Atom xiclass;
    int rc;

    LogMessage(X_DEFAULT, "[events] InitInput: adding mouse proc %p (serverClient: %p)", androidMouseProc, serverClient);
    p = AddInputDevice(serverClient, androidMouseProc, TRUE);
    p->name = strdup("pointer");
    LogMessage(X_DEFAULT, "[events] InitInput: adding trackball proc %p (serverClient: %p)", androidTrackballProc, serverClient);
    t = AddInputDevice(serverClient, androidTrackballProc, TRUE);
    t->name = strdup("trackball");
    LogMessage(X_DEFAULT, "[events] InitInput: adding keybd proc %p (serverClient: %p)", androidKeybdProc, serverClient);
    k = AddInputDevice(serverClient, androidKeybdProc, TRUE);
    k->name = strdup("keyboard");

    LogMessage(X_DEFAULT, "[events] InitInput: registering mouse %p", p);
    /* RegisterPointerDevice(p); */
    xiclass = MakeAtom(XI_MOUSE, sizeof(XI_MOUSE) - 1, TRUE);
    AssignTypeAndName(p, xiclass, "Android mouse");
    LogMessage(X_DEFAULT, "[events] InitInput: registering trackball %p", t);
    /* RegisterPointerDevice(t); */
    xiclass = MakeAtom(XI_MOUSE, sizeof(XI_MOUSE) - 1, TRUE);
    AssignTypeAndName(t, xiclass, "Android trackball");
    LogMessage(X_DEFAULT, "[events] InitInput: registering keyboard %p", k);
    /* RegisterKeyboardDevice(k); */
    xiclass = MakeAtom(XI_KEYBOARD, sizeof(XI_KEYBOARD) - 1, TRUE);
    AssignTypeAndName(k, xiclass, "Android keyboard");
    LogMessage(X_DEFAULT, "[events] InitInput: AddEnabledDevice wakeupFD[0]", Android->wakeupFD[0]);
    AddEnabledDevice(Android->wakeupFD[0]);

    (void)mieqInit();
    InitEventList(GetMaximumEventsNum());;
}

void
CloseInput (void)
{
}

int
ChangeDeviceControl(register ClientPtr client, DeviceIntPtr pDev,
                        xDeviceCtl *control)
{
    switch (control->control) {
    case DEVICE_RESOLUTION:
        /* FIXME do something more intelligent here */
        return BadMatch;

    case DEVICE_ABS_CALIB:
    case DEVICE_ABS_AREA:
        return Success;

    case DEVICE_CORE:
        return BadMatch;
    case DEVICE_ENABLE:
        return Success;

    default:
        return BadMatch;
    }

    /* NOTREACHED */
    return BadImplementation;
}

void
OpenInputDevice(DeviceIntPtr pDev, ClientPtr client, int *status)
{
    if (!pDev)
        *status = BadDevice;
    else
        *status = Success;
}

void
CloseInputDevice(DeviceIntPtr pDev, ClientPtr client)
{
    return;
}

/* We initialise all input devices at startup. */
void
AddOtherInputDevices(void)
{
    return;
}

void
DeleteInputDeviceRequest(DeviceIntPtr dev)
{
    return;
}

/* At the moment, absolute/relative is up to the client. */
int
SetDeviceMode(register ClientPtr client, DeviceIntPtr pDev, int mode)
{
    return BadMatch;
}

int
SetDeviceValuators(register ClientPtr client, DeviceIntPtr pDev,
                   int *valuators, int first_valuator, int num_valuators)
{
    return BadMatch;
}



