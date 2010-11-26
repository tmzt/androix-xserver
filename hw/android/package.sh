make -C ../../dix
make -C ../../os
make -C ../../fb
make -C ../../mi
#make
#make install

#cp /opt/androix/usr/lib/libxandroid.a obj/local/armeabi/
#cp .libs/libxandroid.a obj/local/armeabi
/home/tmzt/src/android-ndk-4/android-ndk-r4b/ndk-build
cp usrdata.zip libs/armeabi/libusrdata.so
ant compile
ant debug
