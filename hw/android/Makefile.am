
noinst_LTLIBRARIES = libddxandroid.la

lib_LTLIBRARIES = libxandroid.la

libddxandroid_la_CFLAGS = 	\
	${DIX_CFLAGS}		\
	-I/opt/androix/usr/include

libddxandroid_la_SOURCES =	\
	startup.c		\
	native.c		\
	callback.c		\
	events.c		

libxandroid_la_SOURCES =

libxandroid_la_LIBADD = \
	libddxandroid.la	\
	@DDXANDROID_LIBS@

libxandroid_la_LDFLAGS = $(LD_EXPORT_SYMBOLS_FLAG) -Wl,--shared \
    -Wl,-z,nocopyreloc -llog

libxandroid_la_DEPENDENCIES = \
	libddxandroid.la		\
	${DDXANDROID_LIBS}

relink:
	$(AM_V_at)rm -f $(bin_PROGRAMS) && $(MAKE) $(bin_PROGRAMS)
