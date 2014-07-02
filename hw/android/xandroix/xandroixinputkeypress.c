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

Except as contained in this notice, the name(s) of the above copyright
holders shall not be used in advertising or otherwise to promote the sale,
use or other dealings in this Software without prior written authorization.

includes code from quartzStartup.c, copyright 2001-2004 Torrey T. Lyons. All Rights Reserved

*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <pthread.h>
#include <math.h>

#include "../android.h"
#include "../androidevents.h"

extern int dix_main(int argc, char *argv[], char *envp[]);
static pthread_mutex_t events_lock = PTHREAD_MUTEX_INITIALIZER;
AndroidVars *Android;

int main_thread_id;
int server_thread_id;
int stress_thread_id;

struct arg {
    int argc;
    char **argv;
    char **envp;
};

static void server_thread (void *arg) {
    struct arg args = *((struct arg *)arg);
    int res;
    struct stat stats;
    int mode = 0666;
    free(arg);

    char *argv[] = {":1"};
    char *envp[] = {};

    LOG("fixing up /data/data/net.homeip.ofn.androix/usr/bin/xkbcomp");

    chmod("/data/data/net.homeip.ofn.androix/usr/bin/xkbcomp", 0775);

    LOG("done.");

    stat("/data/data/net.homeip.ofn.androix/usr/bin/xkbcomp", &stats);

    LOG("/data/data/net.homeip.ofn.androix/usr/bin/xkbcomp mode: %o" + stats.st_mode);

    LOG("creating event event socket pair");
    socketpair(AF_UNIX, SOCK_STREAM, 0, Android->eventFD);
    LOG("remote (Xorg) socket: %d", Android->eventFD[0]);
    LOG("local (android) socket: %d", Android->eventFD[1]);
    
    LOG("starting DIX");
	dix_main(1, argv, envp);
    LOG("returning from DIX (this shouldn't happen)");
}

void androidSendEvent(AndroidWireEvent *ev, size_t len)
{
    AndroidWireEvent wire;
    memcpy(&wire, ev, len);

    int res;
    LOG("writing event to eventFD");
    res = write(Android->eventFD[1], &wire, sizeof(AndroidWireEvent));
    LOG("wrote %d bytes", res);
}

void wakeupFD() {
    int res;
   	char nullbyte=0;
	//  <daniels> oh, i ... er ... christ.
    LOG("writing to eventFD");
	res = write(Android->eventFD[1], &nullbyte, sizeof(nullbyte));
//  res = write(Android->wakeupFD[1], "X", 1);
    LOG("wrote %d bytes", res);
}

static void stress_thread (void) {
    unsigned int k;
    AndroidWireKeyDownEvent down = { ANDROIDWIREKEYDOWNEVENT, 0, NULL, 0 };
    AndroidWireKeyUpEvent up = { ANDROIDWIREKEYUPEVENT, 0, NULL, 0 };

    fprintf(stderr, "[xfakelib stress_thread] starting input stress test\n");
    for(;;) {
            while (!keyboardPtr) sleep(1);
            k = floor(rand()%100);
            down.dev = (void *)keyboardPtr;
            down.keyCode = k;
            androidSendEvent((AndroidWireEvent *)&down, sizeof(AndroidWireKeyDownEvent));
            up.dev = (void *)keyboardPtr;
            up.keyCode = k;
            androidSendEvent((AndroidWireEvent *)&up, sizeof(AndroidWireKeyDownEvent));
            sleep(2);
    };

    fprintf(stderr, "[xfakelib stress_thread] stress test finished (should not happen)");
    exit(0);
}

static pthread_t create_thread (void *func, void *arg) {
    pthread_attr_t attr;
    pthread_t tid;

    fprintf(stderr, "[xfakelib create_thread] creating thread\n");

    pthread_attr_init (&attr);
    pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
    pthread_create (&tid, &attr, func, arg);
    pthread_attr_destroy (&attr);

    return tid;
}

int main(int argc, char *argv[], char *envp[])
{
    struct arg *args = (struct arg*)malloc(sizeof(struct arg));

    args->argc = argc;
    args->argv = argv;
    args->envp = envp;

    main_thread_id = pthread_self();

    Android = (AndroidVars *)calloc(sizeof(AndroidVars), 1);
    Android->events_lock = &events_lock;
    androidInitNative(NULL);

    server_thread_id = create_thread(server_thread, args);

    fprintf(stderr, "[xfakelib main] server_thread_id: %d\n", server_thread_id);

    if (!server_thread_id) {
        fprintf(stderr, "[xfakelib main] can't create server thread\n");
    }

    stress_thread_id = create_thread(stress_thread, NULL);

    if (!server_thread_id) {
        fprintf(stderr, "[xfakelib main] can't create stress thread\n");
    }

//    stress_thread();
    while(1) {};

    fprintf(stderr, "[xfakelib main] exited in main thread\n");
}

