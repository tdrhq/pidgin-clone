EXTRA_DIST = \
	Makefile.mingw

pkgdir = $(libdir)/purple-$(PURPLE_MAJOR_VERSION)

FACEBOOK_SOURCES = \
	libfacebook.h \
	libfacebook.c \
	fb_blist.h \
	fb_blist.c \
	fb_connection.h \
	fb_connection.c \
	fb_conversation.h \
	fb_conversation.c \
	fb_friendlist.h \
	fb_friendlist.c \
	fb_info.h \
	fb_info.c \
	fb_managefriends.h \
	fb_managefriends.c \
	fb_messages.h \
	fb_messages.c \
	fb_notifications.h \
	fb_notifications.c \
	fb_search.h \
	fb_search.c \
	fb_util.h \
	fb_util.c

AM_CFLAGS = $(st)

libfacebook_la_LDFLAGS = -module -avoid-version

if STATIC_FACEBOOK

st = -DPURPLE_STATIC_PRPL
noinst_LTLIBRARIES    = libfacebook.la
libfacebook_la_SOURCES = $(FACEBOOK_SOURCES)
libfacebook_la_CFLAGS  = $(AM_CFLAGS)

else

st =
pkg_LTLIBRARIES       = libfacebook.la
libfacebook_la_SOURCES = $(FACEBOOK_SOURCES)
libfacebook_la_LIBADD  = $(GLIB_LIBS)

endif

AM_CPPFLAGS = \
	-I$(top_srcdir)/libpurple \
	-I$(top_builddir)/libpurple \
	$(GLIB_CFLAGS) \
	$(DEBUG_CFLAGS)
