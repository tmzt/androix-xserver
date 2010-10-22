if BUILD_KDRIVEFBDEVLIB
FBDEV_SUBDIRS = fbdev
endif

if XFAKESERVER
XFAKE_SUBDIRS = fake
endif

if XFAKELIB
XFAKELIB_SUBDIRS = fakelib
endif

if XANDROID
XANDROID_SUBDIRS = android 
endif

if XEPHYR
XEPHYR_SUBDIRS = ephyr
endif

if KDRIVELINUX
LINUX_SUBDIRS = linux
endif

SERVER_SUBDIRS = 		\
	$(FBDEV_SUBDIRS)	\
	$(XEPHYR_SUBDIRS)       \
	$(XFAKE_SUBDIRS)	\
	$(XFAKELIB_SUBDIRS)	\
	$(XANDROID_SUBDIRS)

SUBDIRS =			\
	src			\
	$(LINUX_SUBDIRS)	\
	$(SERVER_SUBDIRS)

DIST_SUBDIRS = fbdev ephyr src linux fake fakelib android

relink:
	$(AM_V_at)for i in $(SERVER_SUBDIRS) ; do $(MAKE) -C $$i relink || exit 1 ; done
