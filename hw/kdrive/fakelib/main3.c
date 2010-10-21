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

#include <unistd.h>
#include <stdio.h>
#include <dlfcn.h>

#include <pthread.h>

int (*dix_main)(int argc, char *argv[], char *envp[]);

int main_thread_id;
int server_thread_id;

struct arg {
    int argc;
    char **argv;
    char **envp;
};

static void server_thread (void *arg) {
    struct arg args = *((struct arg *)arg);
    free(arg);
    exit (dix_main(args.argc, args.argv, args.envp));
}

static pthread_t create_thread (void *func, void *arg) {
    pthread_attr_t attr;
    pthread_t tid;

    pthread_attr_init (&attr);
    pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
    pthread_create (&tid, &attr, func, arg);
    pthread_attr_destroy (&attr);

    return tid;
}

int xfakelib_init(int argc, char *argv[], char *envp[]) {
//    struct arg *args = (struct arg*)malloc(sizeof(struct arg));
    struct arg args;
//    if(!args) {
//        fprintf(stderr, "Could not allocate memory.\n");
//    };

    args.argc = argc;
    args.argv = argv;
    args.envp = envp;

    main_thread_id = pthread_self();
    server_thread_id = create_thread(server_thread, &args);

    if (!server_thread_id) {
        fprintf(stderr, "[xfakelib main] can't create secondary thread\n");
    }
}

int main(int argc, char *argv[], char *envp[])
{
	void *xfakelib;

	fprintf(stderr, "[xfakelib2 main] opening the library\n");
	xfakelib = (void *)dlopen("libxfakelib.so", RTLD_NOW);
	*(void **) (&dix_main) = dlsym(xfakelib, "dix_main");
	fprintf(stderr, "[xfakelib2 main] dix_main: %llu\n", (unsigned long long)dix_main);

	fprintf(stderr, "[xfakelib main] creating thread\n");
//	dix_main(argc, argv, envp);
	xfakelib_init(argc, argv, envp);
	fprintf(stderr, "[xfakelib main] dix_main exited in main thread\n");
}

