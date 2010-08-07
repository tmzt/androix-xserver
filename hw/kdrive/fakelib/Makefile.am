#INCLUDES = 					\
#	@KDRIVE_INCS@				\
#	@KDRIVE_CFLAGS@

noinst_LTLIBRARIES = libfakelib.la

lib_LTLIBRARIES = libxfakelib.la

bin_PROGRAMS = Xfakelib

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

libxfakelib_la_LDFLAGS = $(LD_EXPORT_SYMBOLS_FLAG)

libxfakelib_la_DEPENDENCIES = \
	libfakelib.la		\
	@XFAKELIB_LOCAL_LIBS@

Xfakelib_SOURCES = \
	main.c

Xfakelib_LDFLAGS = -lxfakelib

relink:
	$(AM_V_at)rm -f $(bin_PROGRAMS) && $(MAKE) $(bin_PROGRAMS)
