#INCLUDES = 					\
#	@KDRIVE_INCS@				\
#	@KDRIVE_CFLAGS@

noinst_LTLIBRARIES = libfakelib.la

lib_LTLIBRARIES = libxfakelib.la

bin_PROGRAMS = Xfakelib Xfakelibthreaded andtest

libfakelib_la_CFLAGS = 	\
	@KDRIVE_INCS@		\
	@KDRIVE_CFLAGS@	

libfakelib_la_SOURCES =	\
	fakelibinit.c	\
	fakelib.c	\
	kbd.c		\
	os.c		\
	mouse.c		\
	fake.h

libxfakelib_la_CFLAGS =        \
        @KDRIVE_INCS@           \
	@XFAKELIB_CFLAGS@

libxfakelib_la_SOURCES =

libxfakelib_la_LIBADD = \
	libfakelib.la	\
	@XFAKELIB_LIBS@

libxfakelib_la_LDFLAGS = $(LD_EXPORT_SYMBOLS_FLAG) -Wl,--shared \
    -Wl,-z,nocopyreloc -llog

libxfakelib_la_DEPENDENCIES = \
	libfakelib.la		\
	@XFAKELIB_LOCAL_LIBS@

Xfakelib_SOURCES = \
	andmain.c

Xfakelib_LDFLAGS = -lc -lz -ldl -llog -lxfakelib \
    /opt/androix/usr/lib/crtbegin_dynamic.o

Xfakelibthreaded_SOURCES = \
        main6.c

Xfakelibthreaded_LDFLAGS = -lc -lz -ldl -llog -lxfakelib	\
    -nostdlib -Bdynamic \
    -Wl,-dynamic-linker,/system/bin/linker \
    -Wl,-z,nocopyreloc \
    /opt/androix/usr/lib/crtbegin_dynamic.o

andtest_SOURCES = \
	andtest.c

andtest_LDFLAGS = -lc -llog \
    -nostdlib -Bdynamic \
    -Wl,-dynamic-linker,/system/bin/linker \
    -Wl,-z,nocopyreloc \
    /opt/androix/usr/lib/crtbegin_dynamic.o

relink:
	$(AM_V_at)rm -f $(bin_PROGRAMS) && $(MAKE) $(bin_PROGRAMS)
