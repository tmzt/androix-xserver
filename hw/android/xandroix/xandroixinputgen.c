
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <string.h>

#include "../android.h"
#include "../androidevents.h"

extern int dix_main(int argc, char *argv[], char *envp[]);
AndroidVars *Android;

int main(/*int argc, char *argv[], char *envp[]*/) {
    int res;
    struct stat stats;
    int mode = 0666;

    char *name = "X";
    char *arg1 = ":1";
    char *argv[] = {name, arg1};
    char *envp = NULL;

    Android = (AndroidVars *)calloc(sizeof(AndroidVars), 1);
    Android->keyboardPtr = NULL;
    Android->mousePtr = NULL;
    Android->trackballPtr = NULL;
    androidInitNative(NULL);

    LOG("fixing up /data/data/net.homeip.ofn.androix/usr/bin/xkbcomp");

    chmod("/data/data/net.homeip.ofn.androix/usr/bin/xkbcomp", 0775);

    LOG("done.");

    stat("/data/data/net.homeip.ofn.androix/usr/bin/xkbcomp", &stats);

//    LOG("/data/data/net.homeip.ofn.androix/usr/bin/xkbcomp mode: %o" + stats.st_mode);

    LOG("local (android) socket (STDIN): %d", 1);
    Android->eventFD[0] = 1;   

    LOG("starting DIX");
	dix_main(2, (char**)argv, (char**)&envp);
    LOG("returning from DIX (this shouldn't happen)");
}

