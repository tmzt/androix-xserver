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

/* These event packets are used for the communication between the Android-called dispatch functions and the X server in hw/android/events.c, they are not X events and are not used within X. */


typedef enum _AndroidWireEventType {
    ANDROIDWIRESYNCEVENT = 0,
    ANDROIDWIREKEYDOWNEVENT,
    ANDROIDWIREKEYUPEVENT,
    ANDROIDWIRETOUCHDOWNEVENT,
    ANDROIDWIRETOUCHMOVEEVENT,
    ANDROIDWIRETOUCHUPEVENT,
    ANDROIDWIRETRACKBALLNORMALIZEDMOTIONEVENT,
    ANDROIDWIRETRACKBALLPRESSEVENT,
    ANDROIDWIRETRACKBALLRELEASEEVENT
} AndroidWireEventType;

typedef struct _AndroidWireEvent {
    AndroidWireEventType type;   
    long ts;
    void *dev;
    union {
        struct { int keyCode; };
        struct { int x, y; };
        struct { double x, y; };
    } u;
} AndroidWireEvent;

typedef struct _AndroidWireKeyDownEvent {
    AndroidWireEventType type;
    long ts;
    void *dev;
    int keyCode;
} AndroidWireKeyDownEvent;

typedef struct _AndroidWireKeyUpEvent {
    AndroidWireEventType type;
    long ts;
    void *dev;
    int keyCode;
} AndroidWireKeyUpEvent;

typedef struct _AndroidWireTouchDownEvent {
    AndroidWireEventType type;
    long ts;
    void *dev;
    double x;
    double y;
} AndroidWireTouchDownEvent;

typedef struct _AndroidWireTouchMoveEvent {
    AndroidWireEventType type;
    long ts;
    void *dev;
    double x;
    double y;
} AndroidWireTouchMoveEvent;

typedef struct _AndroidWireTouchUpEvent {
    AndroidWireEventType type;
    long ts;
    void *dev;
    double x;
    double y;
} AndroidWireTouchUpEvent;

typedef struct _AndroidWireTrackballNormalizedMotionEvent {
    AndroidWireEventType type;
    long ts;
    void *dev;
    double x;
    float y;
} AndroidWireTrackballNormalizedMotionEvent;

typedef struct _AndroidWireTrackballPressEvent {
    AndroidWireEventType type;
    long ts;
    void *dev;
} AndroidWireTrackballPressEvent;

typedef struct _AndroidWireTrackballReleaseEvent {
    AndroidWireEventType type;
    long ts;
    void *dev;
} AndroidWireTrackballReleaseEvent;

