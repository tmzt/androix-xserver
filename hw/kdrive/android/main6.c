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

#include <pthread.h>

extern int dix_main(int argc, char *argv[], char *envp[]);

int main_thread_id;
int server_thread_id;

struct arg {
    int argc;
    char **argv;
    char **envp;
};

static void server_thread (void *arg) {
    struct arg args = *((struct arg *)arg);
    int res;
    free(arg);

    fprintf(stderr, "[xfakelib server_thread] starting dix_main\n");
    res = dix_main(args.argc, args.argv, args.envp);
    fprintf(stderr, "[xfakelib server_thread] dix_main exit: %d\n", res);
    exit(res);
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
    server_thread_id = create_thread(server_thread, args);

    fprintf(stderr, "[xfakelib main] server_thread_id: %d\n", server_thread_id);

    if (!server_thread_id) {
        fprintf(stderr, "[xfakelib main] can't create secondary thread\n");
    }

    while(1) {};

    fprintf(stderr, "[xfakelib main] exited in main thread\n");
}

