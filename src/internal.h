 /**
 * @file internal.h Internal definitions and includes
 * @ingroup core
 *
 * gaim
 *
 * Gaim is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef _GAIM_INTERNAL_H_
#define _GAIM_INTERNAL_H_

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef ENABLE_NLS
#  include <libintl.h>
#  define _(x) gettext(x)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  include <locale.h>
#  define N_(String) (String)
#  define _(x) (x)
#  define ngettext(Singular, Plural, Number) ((Number == 1) ? (Singular) : (Plural))
#endif
/*
 * The Singular/Plural/Number ngettext define above was taken from 
 * an email to the texinfo mailing list by Manuel Guerrero.  Thank 
 * you Manuel, and thank you Alex's good friend Google.
 */

#ifdef HAVE_ENDIAN_H
# include <endian.h>
#endif

#define MSG_LEN 2048
/* The above should normally be the same as BUF_LEN,
 * but just so we're explicitly asking for the max message
 * length. */
#define BUF_LEN MSG_LEN
#define BUF_LONG BUF_LEN * 2

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#ifndef _WIN32
#include <sys/wait.h>
#endif
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef HAVE_ICONV
#include <iconv.h>
#endif

#ifdef HAVE_LANGINFO_CODESET
#include <langinfo.h>
#endif

#ifdef GAIM_PLUGINS
# include <gmodule.h>
# ifndef _WIN32
#  include <dlfcn.h>
# endif
#endif

#ifndef _WIN32
# include <netinet/in.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <sys/un.h>
# include <sys/utsname.h>
# include <netdb.h>
# include <signal.h>
# include <unistd.h>
#endif

#ifndef MAXPATHLEN
# define MAXPATHLEN 1024
#endif

#ifndef HOST_NAME_MAX
# define HOST_NAME_MAX 255
#endif

#define PATHSIZE 1024

#include <glib.h>

#ifdef _WIN32
#include "win32dep.h"
#endif


#define GAIM_WEBSITE "http://gaim.sourceforge.net/"

#endif /* _GAIM_INTERNAL_H_ */
