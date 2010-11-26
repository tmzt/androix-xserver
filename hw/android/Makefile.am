
noinst_LTLIBRARIES = libddxandroid.la libfbcmap.la

lib_LTLIBRARIES = libxandroid.la

libddxandroid_la_CFLAGS = 	\
	${DIX_CFLAGS}
#	${DIX_CFLAGS}		\
#	#-I/opt/androix/usr/include

libddxandroid_la_SOURCES =	\
	startup.c		\
	screen.c		\
	damage.c		\
	callback.c		\
	events.c		\
	$(top_srcdir)/mi/miinitext.c

libfbcmap_la_SOURCES = $(top_srcdir)/fb/fbcmap_mi.c
libfbcmap_la_CFLAGS = $(DIX_CFLAGS)

libxandroid_la_SOURCES =

libxandroid_la_LIBADD = \
    libfbcmap.la        \
	libddxandroid.la	\
	@DDXANDROID_LIBS@

libxandroid_la_LDFLAGS = -nostdlib $(LD_EXPORT_SYMBOLS_FLAG) $(LDFLAGS) \
	/opt/androix/usr/lib/crtbegin_dynamic.o

libxandroid_la_DEPENDENCIES = \
    libfbcmap.la            \
	libddxandroid.la		\
	/opt/androix/usr/lib/crtbegin_dynamic.o	\
	@DDXANDROID_LIBS@

relink:
	$(AM_V_at)rm -f $(bin_PROGRAMS) && $(MAKE) $(bin_PROGRAMS)
