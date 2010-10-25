
#PLATFORM=/home/tmzt/src/android-ndk-4/android-ndk-r4b/build/platforms/android-8/arch-arm
PREBUILT="/home/tmzt/src/android-ndk-4/android-ndk-r4b/build/prebuilt/linux-x86/arm-eabi-4.4.0"  \
PLATFORM="/opt/androix" \
CC="$PREBUILT/bin/arm-eabi-gcc"     \
NM="$PREBUILT/bin/arm-eabi-nm"      \
PKG_CONFIG_PATH="/opt/androix/usr/lib/pkgconfig"    \
CFLAGS="-W -Wall  -O   -Wstrict-prototypes -pipe -std=gnu99 -ffunction-sections -fno-short-enums -march=armv5te -mtune=xscale -msoft-float -fomit-frame-pointer  -Wno-pointer-sign -Wno-override-init -I/opt/androix/include -I/opt/androix/usr/include -nostdlib"                      \
CFLAGS="$CFLAGS -fPIC -DANDROID -I$PLATFORM/usr/include -Dbionic -Dlinux -D__arm__"     \
LDFLAGS="-Wl,-T,$PREBUILT/arm-eabi/lib/ldscripts/armelf.x -Wl,-rpath-link=$PLATFORM/usr/lib -L$PLATFORM/usr/lib -nostdlib -lc -lm -ldl"   \
LDFLAGS="$LDFLAGS    -nostdlib -Wl,-dynamic-linker,/system/bin/linker -Wl,-z-nocopyreloc"			\
./configure                \
--host arm-eabi		\
--prefix=/opt/androix/usr   \
--disable-glx           \
--disable-xvfb          \
--disable-xinerama      \
--disable-xnest         \
--enable-kdrive         \
--enable-xfakelib       \
--disable-xfbdev        \
--disable-xephyr        \
