EXTRA_DIST = \
	Makefile.mingw

pkgdir = $(libdir)/purple-$(PURPLE_MAJOR_VERSION)

TELEPATHYSOURCES = \
	telepathy.c \
	telepathy_account.c \
	telepathy_avatar.c \
	telepathy_channel.c \
	telepathy_channel_list.c \
	telepathy_channel_text.c \
	telepathy_client.c \
	telepathy_connection.c \
	telepathy_contact.c \
	telepathy_utils.c

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
