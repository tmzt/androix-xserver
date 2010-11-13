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


#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <signal.h>
#include <stdio.h>
#include <sys/file.h>
#include <fcntl.h>

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
#include "androidevents.h"

Bool
LegalModifier(unsigned int key, DeviceIntPtr pDev)
{
    return TRUE;
}

void ProcessInputEvents(void) {
    LogMessage(X_INFO, "[events] before mieqProcessInputEvents();");
    mieqProcessInputEvents();
    LogMessage(X_INFO, "[events] after mieqProcessInputEvents();");
}

static void AndroidWireSigio(void) {
    char buf[256];
    char nullbyte;
	int x = 0;

    AndroidWireEvent *ev = NULL;
    
//    TA_SERVER();

    LogMessage(X_INFO, "[events] AndroidWireSigio reading event");

    x = read(Android->eventFD[0], buf, sizeof(AndroidWireEvent));
    if (x == sizeof(AndroidWireEvent)) {
      //x = read(Android->eventFD[0], buf, sizeof(AndroidWireEvent));
      LogMessage(X_INFO, "[events] read %d bytes (AndroidWireEvent %d)", x, sizeof(AndroidWireEvent));

      ev = (AndroidWireEvent *)buf;

      switch (ev->type) {
            case ANDROIDWIREKEYDOWNEVENT:
                if (ev->dev == NULL) ev->dev = Android->keyboardPtr;
                androidCallbackKeyDown(ev->dev, ((AndroidWireKeyDownEvent *)ev)->keyCode);
                break;
            case ANDROIDWIREKEYUPEVENT:
                if (ev->dev == NULL) ev->dev = Android->keyboardPtr;
                androidCallbackKeyUp(ev->dev, ((AndroidWireKeyDownEvent *)ev)->keyCode);
                break;
            case ANDROIDWIRETOUCHDOWNEVENT:
                if (ev->dev == NULL) ev->dev = Android->mousePtr;
                androidCallbackTouchDown(ev->dev, ((AndroidWireTouchUpEvent *)ev)->x, ((AndroidWireTouchUpEvent *)ev)->y);
                break;
            case ANDROIDWIRETOUCHUPEVENT:
                if (ev->dev == NULL) ev->dev = Android->mousePtr;
                androidCallbackTouchUp(ev->dev, ((AndroidWireTouchUpEvent *)ev)->x, ((AndroidWireTouchUpEvent *)ev)->y);

            case ANDROIDWIRETRACKBALLNORMALIZEDMOTIONEVENT:
                if (ev->dev == NULL) ev->dev = Android->trackballPtr;
                androidCallbackTrackballNormalizedMotion(ev->dev, ((AndroidWireTrackballNormalizedMotionEvent *)ev)->x, ((AndroidWireTrackballNormalizedMotionEvent *)ev)->y);
                break;
            case ANDROIDWIRETRACKBALLPRESSEVENT:
                if (ev->dev == NULL) ev->dev = Android->trackballPtr;
                androidCallbackTrackballPress(ev->dev);
                break;
            case ANDROIDWIRETRACKBALLRELEASEEVENT:
                if (ev->dev == NULL) ev->dev = Android->trackballPtr;
                androidCallbackTrackballRelease(ev->dev);
                break;
            case ANDROIDWIRESYNCEVENT:
                break;
            default:
                LogMessage(X_INFO, "[events] unhandled android wire event");
                break;
        };
    }
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
    struct sigaction act;
    sigset_t set;

    LogMessage(X_DEFAULT, "[events] in InitInput");

    LogMessage(X_DEFAULT, "[events] InitInput: adding mouse proc %p (serverClient: %p)", androidMouseProc, serverClient);
    p = AddInputDevice(serverClient, androidMouseProc, TRUE);
    LogMessage(X_DEFAULT, "[events] InitInput: adding trackball proc %p (serverClient: %p)", androidTrackballProc, serverClient);
    t = AddInputDevice(serverClient, androidTrackballProc, TRUE);
    LogMessage(X_DEFAULT, "[events] InitInput: adding keybd proc %p (serverClient: %p)", androidKeybdProc, serverClient);
    k = AddInputDevice(serverClient, androidKeybdProc, TRUE);

    LogMessage(X_DEFAULT, "[events] InitInput: registering mouse %p", p);
    RegisterPointerDevice(p);
    xiclass = MakeAtom(XI_MOUSE, sizeof(XI_MOUSE) - 1, TRUE);
    AssignTypeAndName(p, xiclass, "Android mouse");
    LogMessage(X_DEFAULT, "[events] InitInput: registering trackball %p", t);
    RegisterPointerDevice(t);
    xiclass = MakeAtom(XI_MOUSE, sizeof(XI_MOUSE) - 1, TRUE);
    AssignTypeAndName(t, xiclass, "Android trackball");
    LogMessage(X_DEFAULT, "[events] InitInput: registering keyboard %p", k);
    RegisterKeyboardDevice(k);
    xiclass = MakeAtom(XI_KEYBOARD, sizeof(XI_KEYBOARD) - 1, TRUE);
    AssignTypeAndName(k, xiclass, "Android keyboard");


    /* enable wakeup on eventFD */
    fcntl (Android->eventFD[0], F_SETOWN, getpid());
    fcntl (Android->eventFD[0], F_SETFL, (fcntl(Android->eventFD[0], F_GETFL) | (O_ASYNC|O_NONBLOCK)));

    memset (&act, 0, sizeof(act));
    act.sa_handler = AndroidWireSigio;
    sigemptyset (&act.sa_mask);
    sigaddset (&act.sa_mask, SIGIO);
    sigaddset (&act.sa_mask, SIGALRM);
    sigaddset (&act.sa_mask, SIGVTALRM);
    sigaction (SIGIO, &act, 0);
    sigemptyset (&set);
    sigprocmask (SIG_SETMASK, &set, 0);

    LogMessage(X_DEFAULT, "[events] InitInput: AddEnabledDevice eventFD[0]", Android->eventFD[0]);
    AddEnabledDevice(Android->eventFD[0]);
    (void)mieqInit();
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



