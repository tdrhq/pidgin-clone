dnl Added by setup-gettext. Do not remove this
dnl unless you know what you are doing.
AC_DEFUN([AM_GNU_GETTEXT_VERSION], [])

dnl ac_var_timeszone_externals.m4

# Define 'timezone', 'altzone' and 'daylight'
# http://www.gnu.org/software/ac-archive/Miscellaneous/ac_var_timezone_externals.html
# Use instead of 'AC_STRUCT_TIMEZONE' to determine whether the
# the external timezone variables 'timezone', 'altzone' and 'daylight' exist,
# defining 'HAVE_TIMEZONE', 'HAVE_ALTZONE' and 'HAVE_DAYLIGHT' respectively
# (as well as gaining the macros defined by 'AC_STRUCT_TIMEZONE').
# Mark R.Bannister <markb@freedomware.co.uk>
AC_DEFUN([AC_VAR_TIMEZONE_EXTERNALS],
[  AC_REQUIRE([AC_STRUCT_TIMEZONE])dnl
   AC_CACHE_CHECK(for timezone external, mb_cv_var_timezone,
   [  AC_TRY_LINK([#include <time.h>], [return (int)timezone;],
         mb_cv_var_timezone=yes,
         mb_cv_var_timezone=no)
   ])
   AC_CACHE_CHECK(for altzone external, mb_cv_var_altzone,
   [  AC_TRY_LINK([#include <time.h>], [return (int)altzone;],
         mb_cv_var_altzone=yes,
         mb_cv_var_altzone=no)
   ])
   AC_CACHE_CHECK(for daylight external, mb_cv_var_daylight,
   [  AC_TRY_LINK([#include <time.h>], [return (int)daylight;],
         mb_cv_var_daylight=yes,
         mb_cv_var_daylight=no)
   ])
   if test $mb_cv_var_timezone = yes; then
      AC_DEFINE([HAVE_TIMEZONE], 1,
              [Define if you have the external 'timezone' variable.])
   fi
   if test $mb_cv_var_altzone = yes; then
      AC_DEFINE([HAVE_ALTZONE], 1,
              [Define if you have the external 'altzone' variable.])
   fi
   if test $mb_cv_var_daylight = yes; then
      AC_DEFINE([HAVE_DAYLIGHT], 1,
              [Define if you have the external 'daylight' variable.])
   fi
])

dnl Available from the GNU Autoconf Macro Archive at:
dnl http://www.gnu.org/software/ac-archive/htmldoc/ac_prog_perl_modules.html
dnl
AC_DEFUN([AC_PROG_PERL_MODULES],[dnl
ac_perl_modules="$1"
# Make sure we have perl
if test -z "$PERL"; then
AC_CHECK_PROG(PERL,perl,perl)
fi

if test "x$PERL" != x; then
  ac_perl_modules_failed=0
  for ac_perl_module in $ac_perl_modules; do
    AC_MSG_CHECKING(for perl module $ac_perl_module)

    # Would be nice to log result here, but can't rely on autoconf internals
    $PERL "-M$ac_perl_module" -e exit > /dev/null 2>&1
    if test $? -ne 0; then
      AC_MSG_RESULT(no);
      ac_perl_modules_failed=1
   else
      AC_MSG_RESULT(ok);
    fi
  done

  # Run optional shell commands
  if test "$ac_perl_modules_failed" = 0; then
    :
    $2
  else
    :
    $3
  fi
else
  AC_MSG_WARN(could not find perl)
fi])dnl


dnl AM_GCONF_SOURCE_2
dnl Defines GCONF_SCHEMA_CONFIG_SOURCE which is where you should install schemas
dnl  (i.e. pass to gconftool-2
dnl Defines GCONF_SCHEMA_FILE_DIR which is a filesystem directory where
dnl  you should install foo.schemas files
dnl

AC_DEFUN([AM_GCONF_SOURCE_2],
[
  if test "x$GCONF_SCHEMA_INSTALL_SOURCE" = "x"; then
    GCONF_SCHEMA_CONFIG_SOURCE=`gconftool-2 --get-default-source`
  else
    GCONF_SCHEMA_CONFIG_SOURCE=$GCONF_SCHEMA_INSTALL_SOURCE
  fi

  AC_ARG_WITH(gconf-source, 
  [  --with-gconf-source=sourceaddress      Config database for installing schema files.],GCONF_SCHEMA_CONFIG_SOURCE="$withval",)

  AC_SUBST(GCONF_SCHEMA_CONFIG_SOURCE)
  AC_MSG_RESULT([Using config source $GCONF_SCHEMA_CONFIG_SOURCE for schema installation])

  if test "x$GCONF_SCHEMA_FILE_DIR" = "x"; then
    GCONF_SCHEMA_FILE_DIR='$(sysconfdir)/gconf/schemas'
  fi

  AC_ARG_WITH(gconf-schema-file-dir, 
  [  --with-gconf-schema-file-dir=dir        Directory for installing schema files.],GCONF_SCHEMA_FILE_DIR="$withval",)

  AC_SUBST(GCONF_SCHEMA_FILE_DIR)
  AC_MSG_RESULT([Using $GCONF_SCHEMA_FILE_DIR as install directory for schema files])

  AC_ARG_ENABLE(schemas-install,
     [  --disable-schemas-install	Disable the schemas installation],
     [case ${enableval} in
       yes|no) ;;
       *) AC_MSG_ERROR(bad value ${enableval} for --enable-schemas-install) ;;
      esac])
  AM_CONDITIONAL([GCONF_SCHEMAS_INSTALL], [test "$enable_schemas_install" != no])
])
