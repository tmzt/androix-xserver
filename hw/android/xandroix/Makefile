#PLATFORM=/home/tmzt/src/android-ndk-4/android-ndk-r4b/build/platforms/android-8/arch-arm
PREBUILT=/home/tmzt/src/android-ndk-4/android-ndk-r4b/build/prebuilt/linux-x86/arm-eabi-4.4.0  
PLATFORM=/opt/androix 
CC=/home/tmzt/src/android-ndk-4/android-ndk-r4b/build/prebuilt/linux-x86/arm-eabi-4.4.0/bin/arm-eabi-gcc     
NM=/home/tmzt/src/android-ndk-4/android-ndk-r4b/build/prebuilt/linux-x86/arm-eabi-4.4.0/bin/arm-eabi-nm      
PKG_CONFIG_PATH=/opt/androix/usr/lib/pkgconfig    
CFLAGS = -W -Wall  -O   -Wstrict-prototypes -pipe -std=gnu99 -ffunction-sections -fno-short-enums -march=armv5te -mtune=xscale -msoft-float -fomit-frame-pointer  -Wno-pointer-sign -Wno-override-init -I/opt/androix/include -I/opt/androix/usr/include -nostdlib 
CFLAGS += -fPIC -DANDROID -I/opt/androix/usr/include -Dbionic -Dlinux -D__arm__
LDFLAGS=-Wl,-T,/home/tmzt/src/android-ndk-4/android-ndk-r4b/build/prebuilt/linux-x86/arm-eabi-4.4.0/arm-eabi/lib/ldscripts/armelf.x -Wl,-rpath-link=/opt/androix/usr/lib -L/opt/androix/usr/lib -nostdlib -lc -lm -lz -llog
LDFLAGS += -nostdlib -Wl,-dynamic-linker,/system/bin/linker -Wl,-z-nocopyreloc

LDFLAGS_BIN += -L../obj/local/armeabi -landroix
LDFLAGS_BIN += /opt/androix/usr/lib/crtbegin_dynamic.o

LDFLAGS_STATIC += ../obj/local/armeabi/libxandroid.a
LDFLAGS_STATIC += /opt/androix/usr/lib/crtbegin_dynamic.o

LOCAL_LDFLAGS += -L/opt/androix/usr/lib -Bstatic -lXdmcp -lpixman-1 -lXfont -lXau -lfontenc -lfreetype 
BUILD_LDFLAGS += -Bstatic -L../obj/local/armeabi -lxandroid

LDFLAGS_LIB += -shared ../obj/local/armeabi/libxandroid.a

xfakelibdynamic: andmain.c
	${CC} -o xfakelibdynamic andmain.c ${CFLAGS} ${LDFLAGS} ${LDFLAGS_BIN}

xandroixdynamic: xandroix.c stub.c
	${CC} -o xandroixdynamic xandroix.c ${CFLAGS} ${LDFLAGS} ${LDFLAGS_BIN}

xandroix: xandroix.c stub.c ../obj/local/armeabi/libxandroid.a
	${CC} -o xandroix xandroix.c stub.c ${CFLAGS} ${BUILD_LDFLAGS} ${LDFLAGS} ${LDFLAGS_STATIC} ${LOCAL_LDFLAGS}

libandroix.so: stub.c ../obj/local/armeabi/libxandroid.a
	$(CC) -o libandroix.so stub.c ${CFLAGS} ${LDFLAGS} ${LOCAL_LDFLAGS} ${LDFLAGS_LIB}


.PHONY: xfakelibdynamic xandroix libandroix.so
