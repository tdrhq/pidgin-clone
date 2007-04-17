bin_PROGRAMS = psyduck

psyduck_SOURCES = defines.h psyduck.c psyduck-connection-manager.c
psyduck_DEPENDENCIES =
psyduck_LDFLAGS = -export-dynamic
psyduck_LDADD = \
	$(DBUS_LIBS) \
	$(INTLLIBS) \
	$(GLIB_LIBS) \
	$(LIBXML_LIBS) \
	$(top_builddir)/libpurple/libpurple.la \
	$(TP_GLIB_LIBS)

AM_CPPFLAGS = \
	-DSTANDALONE \
	-DBR_PTHREADS=0 \
	-DDATADIR=\"$(datadir)\" \
	-DLIBDIR=\"$(libdir)/libpurple/\" \
	-DLOCALEDIR=\"$(datadir)/locale\" \
	-DSYSCONFDIR=\"$(sysconfdir)\" \
	-I$(top_srcdir)/libpurple/ \
	-I$(top_srcdir) \
	$(DEBUG_CFLAGS) \
	$(GLIB_CFLAGS) \
	$(DBUS_CFLAGS) \
	$(LIBXML_CFLAGS) \
	$(TP_GLIB_CFLAGS)
