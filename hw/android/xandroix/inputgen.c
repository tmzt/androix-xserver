
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <string.h>
#include <math.h>

#include "../android.h"
#include "../androidevents.h"

extern int dix_main(int argc, char *argv[], char *envp[]);

int fds[2];

void androidSendEvent(AndroidWireEvent *ev, size_t len)
{
    AndroidWireEvent wire;
    memcpy(&wire, ev, len);

    int res;
//    LOG("[inputgen] writing event to eventFD");
    res = write(fds[1], &wire, sizeof(AndroidWireEvent));
//    LOG("[inputgen] wrote %d bytes", res);
}

int main() {
    int serverpid = -1;

    unsigned int k;
    AndroidWireKeyDownEvent down = { ANDROIDWIREKEYDOWNEVENT, 0, NULL, 0 };
    AndroidWireKeyUpEvent up = { ANDROIDWIREKEYUPEVENT, 0, NULL, 0 };

    pipe(fds);

    LOG("[inputgen] about to fork");
    if (!(serverpid = fork())) {

        LOG("[inputgen] [child] serverpid: %d", serverpid);
        LOG("[inputgen] [child] input fd: %d", fds[0]);

        /* in child */
        dup2(fds[0], 1); /* stdin */
        close(fds[0]);
        execl("xandroixinputgen", "xandroixinputgen", (char *)NULL);

    } else {

        LOG("[inputgen] [parent] serverpid: %d", serverpid);

        /* in parent */

        /* requires the server's hw/android/events.c defaults to Android->keyboardPtr when dev is NULL */
        
        for(;;) {
                k = floor(rand()%100);
                down.keyCode = k;
                androidSendEvent((AndroidWireEvent *)&down, sizeof(AndroidWireKeyDownEvent));
                up.keyCode = k;
                androidSendEvent((AndroidWireEvent *)&up, sizeof(AndroidWireKeyDownEvent));
                sleep(2);
        };

    };

}

