INCLUDES = 					\
	@KDRIVE_INCS@				\
	@KDRIVE_CFLAGS@

noinst_LTLIBRARIES = libfakelib.la

bin_PROGRAMS = Xfakelib

libfakelib_la_SOURCES =	\
	fakelib.c		\
	kbd.c		\
	os.c		\
	mouse.c		\
	fake.h

Xfakelib_SOURCES = \
	fakeinit.c

Xfakelib_LDADD = 						\
	libfakelib.la					\
	@KDRIVE_LIBS@

Xfakelib_LDFLAGS = $(LD_EXPORT_SYMBOLS_FLAG)

Xfakelib_DEPENDENCIES =	\
	libfakelib.la					\
	@XFAKELIB_LOCAL_LIBS@

relink:
	$(AM_V_at)rm -f $(bin_PROGRAMS) && $(MAKE) $(bin_PROGRAMS)
