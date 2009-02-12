if ENABLE_CRASHREPORTER

bin_PROGRAMS = pidgincrashreporter

pidgincrashreporter_SOURCES = \
	crashreporter.c \
	minidump_stackwalk.cc

pidgincrashreporter_LDADD = $(top_srcdir)/thirdparty/google-breakpad/src/libbreakpad.la

AM_CPPFLAGS = \
	-I$(top_srcdir)/thirdparty/google-breakpad/src

endif
