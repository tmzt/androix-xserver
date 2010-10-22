#INCLUDES = 					\
#	@KDRIVE_INCS@				\
#	@KDRIVE_CFLAGS@

noinst_LTLIBRARIES = libkdandroid.la

lib_LTLIBRARIES = libxandroid.la

libkdandroid_la_CFLAGS = 	\
	@KDRIVE_INCS@		\
	@KDRIVE_CFLAGS@	

libkdandroid_la_SOURCES =	\
	android.c		\
	init.c			\
	kbd.c		\
	os.c		\
	mouse.c		\
	kdandroid.h

libxandroid_la_CFLAGS =        \
        @KDRIVE_INCS@           \
	@XANDROID_CFLAGS@

libxandroid_la_SOURCES =

libxandroid_la_LIBADD = \
	libfakelib.la	\
	@XANDROID_LIBS@

libxandroid_la_LDFLAGS = $(LD_EXPORT_SYMBOLS_FLAG) -Wl,--shared \
    -Wl,-z,nocopyreloc -llog

libxandroid_la_DEPENDENCIES = \
	libkdandroid.la		\
	@XANDROID_LOCAL_LIBS@

relink:
	$(AM_V_at)rm -f $(bin_PROGRAMS) && $(MAKE) $(bin_PROGRAMS)
