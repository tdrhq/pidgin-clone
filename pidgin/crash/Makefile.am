if ENABLE_CRASHREPORTER

bin_PROGRAMS = crashreporter

crashreporter_SOURCES = \
	crashreporter.c \
	minidump_stackwalk.cc

crashreporter_LDADD = $(top_srcdir)/thirdparty/google-breakpad/src/libbreakpad.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/thirdparty/google-breakpad/src

endif
