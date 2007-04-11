grincludedir = $(includedir)/gaim

plugindir = $(libdir)/gaim

lib_LTLIBRARIES = libpurple-remote.la

grinclude_HEADERS = \
	remote-socket.h \
	remote.h

libpurple_remote_la_SOURCES = remote-socket.c
libpurple_remote_la_LIBADD = $(GLIB_LIBS)

purple_remote_la_LDFLAGS = -module -avoid-version

if PLUGINS

plugin_LTLIBRARIES = purple-remote.la

purple_remote_la_SOURCES = remote.c
purple_remote_la_LIBADD = libpurple-remote.la $(GTK_LIBS)

endif

AM_CPPFLAGS = \
	-DDATADIR=\"$(datadir)\" \
	-DVERSION=\"$(VERSION)\" \
	-I$(top_srcdir) \
	-I$(top_srcdir)/plugins \
	-I$(top_srcdir)/src \
	$(GTK_CFLAGS) \
	$(DEBUG_CFLAGS)
