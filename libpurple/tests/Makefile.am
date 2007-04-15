if HAVE_CHECK

if USE_MONO
MONO_TEST=check_mono
MONO_TEST_PROGRAM=check_mono
endif

TESTS=check_libpurple $(MONO_TEST)

check_PROGRAMS=check_libpurple $(MONO_TEST_PROGRAM)

check_libpurple_SOURCES=\
        check_libpurple.c \
	    tests.h \
		test_cipher.c \
		test_jabber_jutil.c \
		test_util.c \
		$(top_builddir)/libpurple/util.h

check_libpurple_CFLAGS=\
        @CHECK_CFLAGS@ \
		$(GLIB_CFLAGS) \
		$(DEBUG_CFLAGS) \
		-I.. \
		-DBUILDDIR=\"$(top_builddir)\"

check_libpurple_LDADD=\
        @CHECK_LIBS@ \
		$(GLIB_LIBS) \
		$(top_builddir)/libpurple/protocols/jabber/libjabber.la \
		$(top_builddir)/libpurple/libpurple.la
	
if USE_MONO

check_mono_SOURCES=\
		check_mono.c
		
check_mono_CFLAGS=\
		@CHECK_CFLAGS@ \
		$(GLIB_CFLAGS) \
		$(DEBUG_CFLAGS) \
		-I.. \
		-DBUILDDIR=\"$(top_builddir)\"
		
check_mono_LDADD=\
		@CHECK_LIBS@ \
		$(GLIB_LIBS) \
		$(top_builddir)/libpurple/libpurple.la

endif

endif
