/*
 * gaim
 *
 * Copyright (C) 1998-1999, Mark Spencer <markster@marko.net>
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
 *
 */

#include "internal.h"

#include "conversation.h"
#include "debug.h"
#include "prpl.h"
#include "prefs.h"

static char home_dir[MAXPATHLEN];

char *full_date()
{
	char *date;
	time_t tme;

	time(&tme);
	date = ctime(&tme);
	date[strlen(date) - 1] = '\0';
	return date;
}

G_GNUC_CONST static gint badchar(char c)
{
	switch (c) {
	case ' ':
	case ',':
	case '(':
	case ')':
	case '\0':
	case '\n':
	case '<':
	case '>':
	case '"':
	case '\'':
		return 1;
	default:
		return 0;
	}
}


gchar *sec_to_text(guint sec)
{
	guint daze, hrs, min;
	char *ret = NULL;

	daze = sec / (60 * 60 * 24);
	hrs = (sec % (60 * 60 * 24)) / (60 * 60);
	min = (sec % (60 * 60)) / 60;
	sec = min % 60;

	if (daze) {
		if (hrs || min) {
			if (hrs) {
				if (min) {
					ret = g_strdup_printf(
						   "%d %s, %d %s, %d %s.",
						   daze, ngettext("day","days",daze),
						   hrs, ngettext("hour","hours",hrs), min, ngettext("minute","minutes",min));
				} else {
					ret = g_strdup_printf(
						   "%d %s, %d %s.",
						   daze, ngettext("day","days",daze), hrs, ngettext("hour","hours",hrs));
				}
			} else {
				ret = g_strdup_printf(
					   "%d %s, %d %s.",
					   daze, ngettext("day","days",daze), min, ngettext("minute","minutes",min));
			}
		} else
			ret = g_strdup_printf("%d %s.", daze, ngettext("day","days",daze));
	} else {
		if (hrs) {
			if (min) {
				ret = g_strdup_printf(
					   "%d %s, %d %s.",
					   hrs, ngettext("hour","hours",hrs), min, ngettext("minute","minutes",min));
			} else {
				ret = g_strdup_printf("%d %s.", hrs, ngettext("hour","hours",hrs));
			}
		} else {
			ret = g_strdup_printf("%d %s.", min, ngettext("minute","minutes",min));
		}
	}

	return ret;
}

char *linkify_text(const char *text)
{
	const char *c, *t, *q = NULL;
	char *tmp;
	char url_buf[BUF_LEN * 4];
	GString *ret = g_string_new("");
	/* Assumes you have a buffer able to cary at least BUF_LEN * 2 bytes */

	c = text;
	while (*c) {
		if(!q && (*c == '\"' || *c == '\'')) {
			q = c;
		} else if(q) {
			if(*c == *q)
				q = NULL;
		} else if (!g_ascii_strncasecmp(c, "<A", 2)) {
			while (1) {
				if (!g_ascii_strncasecmp(c, "/A>", 3)) {
					break;
				}
				ret = g_string_append_c(ret, *c);
				c++;
				if (!(*c))
					break;
			}
		} else if ((*c=='h') && (!g_ascii_strncasecmp(c, "http://", 7) ||
					(!g_ascii_strncasecmp(c, "https://", 8)))) {
			t = c;
			while (1) {
				if (badchar(*t)) {

					if (*(t) == ',' && (*(t + 1) != ' ')) {
						t++;
						continue;
					}

					if (*(t - 1) == '.')
						t--;
					strncpy(url_buf, c, t - c);
					url_buf[t - c] = 0;
					g_string_append_printf(ret, "<A HREF=\"%s\">%s</A>",
							url_buf, url_buf);
					c = t;
					break;
				}
				if (!t)
					break;
				t++;

			}
		} else if (!g_ascii_strncasecmp(c, "www.", 4)) {
			if (c[4] != '.') {
				t = c;
				while (1) {
					if (badchar(*t)) {
						if (t - c == 4) {
							break;
						}

						if (*(t) == ',' && (*(t + 1) != ' ')) {
							t++;
							continue;
						}

						if (*(t - 1) == '.')
							t--;
						strncpy(url_buf, c, t - c);
						url_buf[t - c] = 0;
						g_string_append_printf(ret,
								"<A HREF=\"http://%s\">%s</A>", url_buf,
								url_buf);
						c = t;
						break;
					}
					if (!t)
						break;
					t++;
				}
			}
		} else if (!g_ascii_strncasecmp(c, "ftp://", 6)) {
			t = c;
			while (1) {
				if (badchar(*t)) {
					if (*(t - 1) == '.')
						t--;
					strncpy(url_buf, c, t - c);
					url_buf[t - c] = 0;
					g_string_append_printf(ret, "<A HREF=\"%s\">%s</A>",
							url_buf, url_buf);
					c = t;
					break;
				}
				if (!t)
					break;
				t++;

			}
		} else if (!g_ascii_strncasecmp(c, "ftp.", 4)) {
			if (c[4] != '.') {
				t = c;
				while (1) {
					if (badchar(*t)) {
						if (t - c == 4) {
							break;
						}
						if (*(t - 1) == '.')
							t--;
						strncpy(url_buf, c, t - c);
						url_buf[t - c] = 0;
						g_string_append_printf(ret,
								"<A HREF=\"ftp://%s\">%s</A>", url_buf,
								url_buf);
						c = t;
						break;
					}
					if (!t)
						break;
					t++;
				}
			}
		} else if (!g_ascii_strncasecmp(c, "mailto:", 7)) {
			t = c;
			while (1) {
				if (badchar(*t)) {
					if (*(t - 1) == '.')
						t--;
					strncpy(url_buf, c, t - c);
					url_buf[t - c] = 0;
					g_string_append_printf(ret, "<A HREF=\"%s\">%s</A>",
							  url_buf, url_buf);
					c = t;
					break;
				}
				if (!t)
					break;
				t++;

			}
		} else if (c != text && (*c == '@')) {
			char *tmp;
			int flag;
			int len = 0;
			const char illegal_chars[] = "!@#$%^&*()[]{}/|\\<>\":;\r\n \0";
			url_buf[0] = 0;

			if (strchr(illegal_chars,*(c - 1)) || strchr(illegal_chars, *(c + 1)))
				flag = 0;
			else
				flag = 1;

			t = c;
			while (flag) {
				if (badchar(*t)) {
					ret = g_string_truncate(ret, ret->len - (len - 1));
					break;
				} else {
					len++;
					tmp = g_malloc(len + 1);
					tmp[len] = 0;
					tmp[0] = *t;
					strncpy(tmp + 1, url_buf, len - 1);
					strcpy(url_buf, tmp);
					url_buf[len] = 0;
					g_free(tmp);
					t--;
					if (t < text) {
						ret = g_string_assign(ret, "");
						break;
					}
				}
			}

			t = c + 1;

			while (flag) {
				if (badchar(*t)) {
					char *d;

					for (d = url_buf + strlen(url_buf) - 1; *d == '.'; d--, t--)
						*d = '\0';

					g_string_append_printf(ret, "<A HREF=\"mailto:%s\">%s</A>",
							url_buf, url_buf);
					c = t;

					break;
				} else {
					strncat(url_buf, t, 1);
					len++;
					url_buf[len] = 0;
				}

				t++;
			}
		}

		if (*c == 0)
			break;

		ret = g_string_append_c(ret, *c);
		c++;

	}
	tmp = ret->str;
	g_string_free(ret, FALSE);
	return tmp;
}


static const char alphabet[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

/* This was borrowed from the Kame source, and then tweaked to our needs */
char *tobase64(const unsigned char *buf, size_t len)
{
	char *s = NULL, *rv = NULL;
	unsigned long tmp;

	s = g_malloc((4 * (len + 1)) / 3 + 1);

	rv = s;
	while (len >= 3) {
		tmp = buf[0] << 16 | buf[1] << 8 | buf[2];
		s[0] = alphabet[tmp >> 18];
		s[1] = alphabet[(tmp >> 12) & 077];
		s[2] = alphabet[(tmp >> 6) & 077];
		s[3] = alphabet[tmp & 077];
		len -= 3;
		buf += 3;
		s += 4;
	}

	/* RFC 1521 enumerates these three possibilities... */
	switch(len) {
		case 2:
			tmp = buf[0] << 16 | buf[1] << 8;
			s[0] = alphabet[(tmp >> 18) & 077];
			s[1] = alphabet[(tmp >> 12) & 077];
			s[2] = alphabet[(tmp >> 6) & 077];
			s[3] = '=';
			s[4] = '\0';
			break;
		case 1:
			tmp = buf[0] << 16;
			s[0] = alphabet[(tmp >> 18) & 077];
			s[1] = alphabet[(tmp >> 12) & 077];
			s[2] = s[3] = '=';
			s[4] = '\0';
			break;
		case 0:
			s[0] = '\0';
			break;
	}

	return rv;
}


void frombase64(const char *text, char **data, int *size)
{
	char *out = NULL;
	char tmp = 0;
	const char *c;
	gint32 tmp2 = 0;
	int len = 0, n = 0;

	if (!text || !data)
		return;

	c = text;

	while (*c) {
		if (*c >= 'A' && *c <= 'Z') {
			tmp = *c - 'A';
		} else if (*c >= 'a' && *c <= 'z') {
			tmp = 26 + (*c - 'a');
		} else if (*c >= '0' && *c <= 57) {
			tmp = 52 + (*c - '0');
		} else if (*c == '+') {
			tmp = 62;
		} else if (*c == '/') {
			tmp = 63;
		} else if (*c == '\r' || *c == '\n') {
			c++;
			continue;
		} else if (*c == '=') {
			if (n == 3) {
				out = g_realloc(out, len + 2);
				out[len] = (char)(tmp2 >> 10) & 0xff;
				len++;
				out[len] = (char)(tmp2 >> 2) & 0xff;
				len++;
			} else if (n == 2) {
				out = g_realloc(out, len + 1);
				out[len] = (char)(tmp2 >> 4) & 0xff;
				len++;
			}
			break;
		}
		tmp2 = ((tmp2 << 6) | (tmp & 0xff));
		n++;
		if (n == 4) {
			out = g_realloc(out, len + 3);
			out[len] = (char)((tmp2 >> 16) & 0xff);
			len++;
			out[len] = (char)((tmp2 >> 8) & 0xff);
			len++;
			out[len] = (char)(tmp2 & 0xff);
			len++;
			tmp2 = 0;
			n = 0;
		}
		c++;
	}

	out = g_realloc(out, len + 1);
	out[len] = 0;

	*data = out;
	if (size)
		*size = len;
}

/*
 * Converts raw data to a pretty, null-terminated base16 string.
 */
unsigned char *tobase16(const unsigned char *data, int length)
{
	int i;
	unsigned char *ascii = NULL;

	if (!data || !length)
		return NULL;

	ascii = g_malloc(length*2 + 1);

	for (i=0; i<length; i++)
		snprintf(&ascii[i*2], 3, "%02hhx", data[i]);

	return ascii;
}

/*
 * Converts a null-terminated string of hexidecimal to raw data.
 */
int frombase16(const char *ascii, unsigned char **raw)
{
	int len, i, accumulator=0;
	unsigned char *data;

	if (!ascii || !(len = strlen(ascii)) || (len % 2))
		return 0;

	data = g_malloc(len/2);
	for (i=0; i<len; i++) {
		if (!(i % 2))
			accumulator = 0;
		else
			accumulator = accumulator << 4;
		if (isdigit(ascii[i]))
			accumulator |= ascii[i]-48;
		else switch(ascii[i]) {
			case 'a':  case 'A':  accumulator|=10;  break;
			case 'b':  case 'B':  accumulator|=11;  break;
			case 'c':  case 'C':  accumulator|=12;  break;
			case 'd':  case 'D':  accumulator|=13;  break;
			case 'e':  case 'E':  accumulator|=14;  break;
			case 'f':  case 'F':  accumulator|=15;  break;
		}
		if (i % 2)
			data[(i-1)/2] = accumulator;
	}

	*raw = data;
	return len/2;
}

char *normalize(const char *s)
{
	static char buf[BUF_LEN];
	char *tmp;
	int i, j;

	g_return_val_if_fail((s != NULL), NULL);

	strncpy(buf, s, BUF_LEN);
	for (i=0, j=0; buf[j]; i++, j++) {
		while (buf[j] == ' ')
			j++;
		buf[i] = buf[j];
	}
	buf[i] = '\0';

	tmp = g_utf8_strdown(buf, -1);
	g_snprintf(buf, sizeof(buf), "%s", tmp);
	g_free(tmp);
	tmp = g_utf8_normalize(buf, -1, G_NORMALIZE_DEFAULT);
	g_snprintf(buf, sizeof(buf), "%s", tmp);
	g_free(tmp);

	return buf;
}

char *date()
{
	static char date[80];
	time_t tme;
	time(&tme);
	strftime(date, sizeof(date), "%H:%M:%S", localtime(&tme));
	return date;
}

void clean_pid(void)
{
#ifndef _WIN32
	int status;
	pid_t pid;

	do {
		pid = waitpid(-1, &status, WNOHANG);
	} while (pid != 0 && pid != (pid_t)-1);
	if(pid == (pid_t)-1 && errno != ECHILD) {
		char errmsg[BUFSIZ];
		snprintf(errmsg, BUFSIZ, "Warning: waitpid() returned %d", pid);
		perror(errmsg);
	}
#endif
}


/* Look for %n, %d, or %t in msg, and replace with the sender's name, date,
   or time */
char *away_subs(const char *msg, const char *name)
{
	char *c;
	static char cpy[BUF_LONG];
	int cnt = 0;
	time_t t = time(0);
	struct tm *tme = localtime(&t);
	char tmp[20];

	cpy[0] = '\0';
	c = (char *)msg;
	while (*c) {
		switch (*c) {
		case '%':
			if (*(c + 1)) {
				switch (*(c + 1)) {
				case 'n':
					/* append name */
					strcpy(cpy + cnt, name);
					cnt += strlen(name);
					c++;
					break;
				case 'd':
					/* append date */
					strftime(tmp, 20, "%m/%d/%Y", tme);
					strcpy(cpy + cnt, tmp);
					cnt += strlen(tmp);
					c++;
					break;
				case 't':
					/* append time */
					strftime(tmp, 20, "%r", tme);
					strcpy(cpy + cnt, tmp);
					cnt += strlen(tmp);
					c++;
					break;
				default:
					cpy[cnt++] = *c;
				}
			}
			break;
		default:
			cpy[cnt++] = *c;
		}
		c++;
	}
	cpy[cnt] = '\0';
	return (cpy);
}

GSList *message_split(char *message, int limit)
{
	static GSList *ret = NULL;
	int lastgood = 0, curgood = 0, curpos = 0, len = strlen(message);
	gboolean intag = FALSE;

	if (ret) {
		GSList *tmp = ret;
		while (tmp) {
			g_free(tmp->data);
			tmp = g_slist_remove(tmp, tmp->data);
		}
		ret = NULL;
	}

	while (TRUE) {
		if (lastgood >= len)
			return ret;

		if (len - lastgood < limit) {
			ret = g_slist_append(ret, g_strdup(&message[lastgood]));
			return ret;
		}

		curgood = curpos = 0;
		intag = FALSE;
		while (curpos <= limit) {
			if (isspace(message[curpos + lastgood]) && !intag)
				curgood = curpos;
			if (message[curpos + lastgood] == '<')
				intag = TRUE;
			if (message[curpos + lastgood] == '>')
				intag = FALSE;
			curpos++;
		}

		if (curgood) {
			ret = g_slist_append(ret, g_strndup(&message[lastgood], curgood));
			if (isspace(message[curgood + lastgood]))
				lastgood += curgood + 1;
			else
				lastgood += curgood;
		} else {
			/* whoops, guess we have to fudge it here */
			ret = g_slist_append(ret, g_strndup(&message[lastgood], limit));
			lastgood += limit;
		}
	}
}

const gchar *gaim_home_dir()
{
#ifndef _WIN32
	if(g_get_home_dir())
		return g_get_home_dir();
	else
		return NULL;
#else
        return wgaim_data_dir();
#endif
}

/* returns a string of the form ~/.gaim, where ~ is replaced by the user's home
 * dir. Note that there is no trailing slash after .gaim. */
gchar *gaim_user_dir()
{
	const gchar *hd = gaim_home_dir();
        if(hd) {
		strcpy( (char*)&home_dir, hd );
		strcat( (char*)&home_dir, G_DIR_SEPARATOR_S ".gaim" );
		return (gchar*)&home_dir;
	}
	else {
   	        return NULL;
	}
}

/*
 * rcg10312000 This could be more robust, but it works for my current
 *  goal: to remove those annoying <BR> tags.  :)
 * dtf12162000 made the loop more readable. i am a neat freak. ;) */
void strncpy_nohtml(gchar *dest, const gchar *src, size_t destsize)
{
	gchar *ptr;
	g_snprintf(dest, destsize, "%s", src);

	while ((ptr = strstr(dest, "<BR>")) != NULL) {
		/* replace <BR> with a newline. */
		*ptr = '\n';
		memmove(ptr + 1, ptr + 4, strlen(ptr + 4) + 1);
	}
}

void strncpy_withhtml(gchar *dest, const gchar *src, size_t destsize)
{
	gchar *end = dest + destsize;

	while (dest < end) {
		if (*src == '\n' && dest < end - 5) {
			strcpy(dest, "<BR>");
			src++;
			dest += 4;
		} else if(*src == '\r') {
			src++;
		} else {
			*dest++ = *src;
			if (*src == '\0')
				return;
			else
				src++;
		}
	}
}


/*
 * Like strncpy_withhtml (above), but malloc()'s the necessary space
 *
 * The caller is responsible for freeing the space pointed to by the
 * return value.
 */

gchar *strdup_withhtml(const gchar *src)
{
	gchar *sp, *dest;
	gulong destsize;

	if(!src)
		return NULL;

	/*
	 * All we need do is multiply the number of newlines by 3 (the
	 * additional length of "<BR>" over "\n"), account for the
	 * terminator, malloc the space and call strncpy_withhtml.
	 */
	for(destsize = 0, sp = (gchar *)src; (sp = strchr(sp, '\n')) != NULL; ++sp, ++destsize)
		;
	destsize *= 3;
	destsize += strlen(src) + 1;
	dest = g_malloc(destsize);
	strncpy_withhtml(dest, src, destsize);

	return(dest);
}

void strip_linefeed(gchar *text)
{
	int i, j;
	gchar *text2 = g_malloc(strlen(text) + 1);

	for (i = 0, j = 0; text[i]; i++)
		if (text[i] != '\r')
			text2[j++] = text[i];
	text2[j] = '\0';

	strcpy(text, text2);
	g_free(text2);
}

char *add_cr(const char *text)
{
	char *ret = NULL;
	int count = 0, i, j;

	if (text[0] == '\n')
		count++;
	for (i = 1; i < strlen(text); i++)
		if (text[i] == '\n' && text[i - 1] != '\r')
			count++;

	if (count == 0)
		return g_strdup(text);

	ret = g_malloc0(strlen(text) + count + 1);

	i = 0; j = 0;
	if (text[i] == '\n')
		ret[j++] = '\r';
	ret[j++] = text[i++];
	for (; i < strlen(text); i++) {
		if (text[i] == '\n' && text[i - 1] != '\r')
			ret[j++] = '\r';
		ret[j++] = text[i];
	}

	gaim_debug(GAIM_DEBUG_INFO, "add_cr", "got: %s, leaving with %s\n",
			   text, ret);

	return ret;
}

time_t get_time(int year, int month, int day, int hour, int min, int sec)
{
	struct tm tm;

	tm.tm_year = year - 1900;
	tm.tm_mon = month - 1;
	tm.tm_mday = day;
	tm.tm_hour = hour;
	tm.tm_min = min;
	tm.tm_sec = sec >= 0 ? sec : time(NULL) % 60;
	return mktime(&tm);
}

/*
 * Like mkstemp() but returns a file pointer, uses a pre-set template,
 * uses the semantics of tempnam() for the directory to use and allocates
 * the space for the filepath.
 *
 * Caller is responsible for closing the file and removing it when done,
 * as well as freeing the space pointed-to by "path" with g_free().
 *
 * Returns NULL on failure and cleans up after itself if so.
 */
static const char *gaim_mkstemp_templ = {"gaimXXXXXX"};

FILE *gaim_mkstemp(gchar **fpath)
{
	const gchar *tmpdir;
#ifndef _WIN32
	int fd;
#endif
	FILE *fp = NULL;

	if((tmpdir = (gchar*)g_get_tmp_dir()) != NULL) {
		if((*fpath = g_strdup_printf("%s" G_DIR_SEPARATOR_S "%s", tmpdir, gaim_mkstemp_templ)) != NULL) {
#ifdef _WIN32
			char* result = _mktemp( *fpath );
			if( result == NULL )
				gaim_debug(GAIM_DEBUG_ERROR, "gaim_mkstemp",
						   "Problem creating the template\n");
			else
			{
				if( (fp = fopen( result, "w+" )) == NULL ) {
					gaim_debug(GAIM_DEBUG_ERROR, "gaim_mkstemp",
							   "Couldn't fopen() %s\n", result);
				}
			}
#else
			if((fd = mkstemp(*fpath)) == -1) {
				gaim_debug(GAIM_DEBUG_ERROR, "gaim_mkstemp",
						   "Couldn't make \"%s\", error: %d\n",
						   *fpath, errno);
			} else {
				if((fp = fdopen(fd, "r+")) == NULL) {
					close(fd);
					gaim_debug(GAIM_DEBUG_ERROR, "gaim_mkstemp",
							   "Couldn't fdopen(), error: %d\n", errno);
				}
			}
#endif
			if(!fp) {
				g_free(*fpath);
				*fpath = NULL;
			}
		}
	} else {
		gaim_debug(GAIM_DEBUG_ERROR, "gaim_mkstemp",
				   "g_get_tmp_dir() failed!");
	}

	return fp;
}

gboolean program_is_valid(const char *program) 
{
	GError *error = NULL;
	char **argv; 
	gchar *progname;
	gboolean is_valid = FALSE;

	if (program == NULL || *program == '\0') {
		return FALSE;
	}

	if (!g_shell_parse_argv(program, NULL, &argv, &error)) {
		gaim_debug(GAIM_DEBUG_ERROR, "program_is_valid",
				   "Could not parse program '%s': %s\n",
				   program, error->message);
		g_error_free(error);
		return FALSE;
	}

	if (argv == NULL) {
		return FALSE;
	}

	progname = g_find_program_in_path(argv[0]);
	is_valid = (progname != NULL);

	g_strfreev(argv);
	g_free(progname);
	
	return is_valid;
}

char *gaim_try_conv_to_utf8(const char *str)
{
	gsize converted;
	char *utf8;

	if (str == NULL) {
		return NULL;
	}

	if (g_utf8_validate(str, -1, NULL)) {
		return g_strdup(str);
	}

	utf8 = g_locale_to_utf8(str, -1, &converted, NULL, NULL);
	if (utf8) 
		return(utf8);
	
	g_free(utf8);
	
	utf8 = g_convert(str, -1, "UTF-8", "ISO-8859-15", &converted, NULL, NULL);
	if (utf8 && converted == strlen (str)) {
		return(utf8);
	} else if (utf8) {
		g_free(utf8);
	}

	return(NULL);
}

char *gaim_getip_from_fd(int fd)
{
	struct sockaddr addr;
	socklen_t namelen = sizeof(addr);

	if (getsockname(fd, &addr, &namelen))
		return NULL;

	return g_strdup(inet_ntoa(((struct sockaddr_in *)&addr)->sin_addr));
}

gint gaim_utf8_strcasecmp(const gchar *a, const gchar *b) {
	gchar *a_norm=NULL;
	gchar *b_norm=NULL;
	gint ret=-1;

	if(!g_utf8_validate(a, -1, NULL) || !g_utf8_validate(b, -1, NULL)) {
		gaim_debug(GAIM_DEBUG_ERROR, "gaim_utf8_strcasecmp", "One or both parameters are invalid UTF8\n");
		return ret;
	}

	a_norm = g_utf8_casefold(a, -1);
	b_norm = g_utf8_casefold(b, -1);
	ret = g_utf8_collate(a_norm, b_norm);
	g_free(a_norm);
	g_free(b_norm);
	return ret;
}

gchar *gaim_strreplace(const gchar *string, const gchar *delimiter, const gchar *replacement) {
	gchar **split;
	gchar *ret;

	split = g_strsplit(string, delimiter, 0);
	ret = g_strjoinv(replacement, split);
	g_strfreev(split);

	return ret;
}

char *gaim_get_size_string(size_t size)
{
	static const char *size_str[4] = { "bytes", "KB", "MB", "GB" };
	float size_mag;
	int size_index = 0;

	if (size == -1) {
		return g_strdup(_("Calculating..."));
	}
	else if (size == 0) {
		return g_strdup(_("Unknown."));
	}
	else {
		size_mag = (float)size;

		while ((size_index < 4) && (size_mag > 1024)) {
			size_mag /= 1024;
			size_index++;
		}

		return g_strdup_printf("%.2f %s", size_mag, size_str[size_index]);
	}
}
