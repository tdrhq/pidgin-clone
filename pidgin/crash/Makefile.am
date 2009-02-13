if ENABLE_CRASHREPORTER

bin_PROGRAMS = pidgincrashreporter

pidgincrashreporter_SOURCES = \
	crashreporter.c \
	minidump_stackwalk.cc

pidgincrashreporter_LDADD = \
	$(top_srcdir)/thirdparty/google-breakpad/src/libbreakpad.la \
	$(GLIB_LIBS) \
	$(GTK_LIBS) \
	$(top_builddir)/libpurple/libpurple.la

AM_CPPFLAGS = \
	-DDATADIR=\"$(datadir)\" \
	-DLIBDIR=\"$(libdir)/pidgin/\" \
	-DLOCALEDIR=\"$(datadir)/locale\" \
	-DSYSCONFDIR=\"$(sysconfdir)\" \
	-I$(top_builddir)/libpurple \
	-I$(top_srcdir)/libpurple/ \
	-I$(top_srcdir)/thirdparty/google-breakpad/src \
	-I$(top_builddir) \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS) \
	$(GTK_CFLAGS) 


endif
