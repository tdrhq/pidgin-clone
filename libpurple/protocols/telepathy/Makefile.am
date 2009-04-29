EXTRA_DIST = \
	Makefile.mingw

pkgdir = $(libdir)/purple-$(PURPLE_MAJOR_VERSION)

TELEPATHYSOURCES = telepathy.c

AM_CFLAGS = $(st)

libtelepathy_la_LDFLAGS = -module -avoid-version `pkg-config --libs telepathy-glib`

st =
pkg_LTLIBRARIES    = libtelepathy.la
libtelepathy_la_SOURCES = $(TELEPATHYSOURCES)
libtelepathy_la_LIBADD  = $(GLIB_LIBS)

AM_CPPFLAGS = \
	-I$(top_srcdir)/libpurple \
	-I$(top_builddir)/libpurple \
	$(GLIB_CFLAGS) \
        `pkg-config --cflags telepathy-glib` \
	$(DEBUG_CFLAGS)
