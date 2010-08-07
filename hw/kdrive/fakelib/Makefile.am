INCLUDES = 					\
	@KDRIVE_INCS@				\
	@KDRIVE_CFLAGS@

noinst_LTLIBRARIES = libfakelib.la

lib_LTLIBRARIES = libxfakelib.la

bin_PROGRAMS = Xfakelib

libfakelib_la_SOURCES =	\
	fakelib.c		\
	kbd.c		\
	os.c		\
	mouse.c		\
	fake.h

libxfakelib_la_SOURCES = \
	fakelibinit.c

libxfakelib_la_LIBADD = \
	libfakelib.la
	$(top_builddir)/dix/libmainlib.la
	@XFAKELIB_LOCAL_LIBS@

libxfakelib_la_LDFLAGS = $(LD_EXPORT_SYMBOLS_FLAG)

libxfakelib_la_DEPENDENCIES = \
	libfakelib.la
	@XFAKELIB_LOCAL_LIBS@

Xfakelib_SOURCES = \
	main.c

Xfakelib_LDFLAGS = -lxfakelib

relink:
	$(AM_V_at)rm -f $(bin_PROGRAMS) && $(MAKE) $(bin_PROGRAMS)
