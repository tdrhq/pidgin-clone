/*
 * @file util.h Utility Functions
 * @ingroup core
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
#include "internal.h"

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "conversation.h"
#include "debug.h"
#include "prpl.h"
#include "prefs.h"
#include "util.h"

typedef struct
{
	void (*callback)(void *, const char *, size_t);
	void *user_data;

	struct
	{
		char *user;
		char *passwd;
		char *address;
		int port;
		char *page;

	} website;

	char *url;
	gboolean full;
	char *user_agent;
	gboolean http11;

	int inpa;

	gboolean sentreq;
	gboolean newline;
	gboolean startsaving;
	gboolean has_explicit_data_len;
	char *webdata;
	unsigned long len;
	unsigned long data_len;

} GaimFetchUrlData;

static char custom_home_dir[MAXPATHLEN];
static char home_dir[MAXPATHLEN];

/**************************************************************************
 * Base16 Functions
 **************************************************************************/
unsigned char *
gaim_base16_encode(const unsigned char *data, int length)
{
	int i;
	unsigned char *ascii = NULL;

	g_return_val_if_fail(data != NULL, NULL);
	g_return_val_if_fail(length > 0,   NULL);

	ascii = g_malloc(length * 2 + 1);

	for (i = 0; i < length; i++)
		snprintf(&ascii[i * 2], 3, "%02hhx", data[i]);

	return ascii;
}

int
gaim_base16_decode(const char *ascii, unsigned char **raw)
{
	int len, i, accumulator = 0;
	unsigned char *data;

	g_return_val_if_fail(ascii != NULL, 0);

	len = strlen(ascii);

	g_return_val_if_fail(strlen(ascii) > 0, 0);
	g_return_val_if_fail(len % 2 > 0,       0);

	data = g_malloc(len / 2);

	for (i = 0; i < len; i++)
	{
		if ((i % 2) == 0)
			accumulator = 0;
		else
			accumulator <<= 4;

		if (isdigit(ascii[i]))
			accumulator |= ascii[i] - 48;
		else
		{
			switch(ascii[i])
			{
				case 'a':  case 'A':  accumulator |= 10;  break;
				case 'b':  case 'B':  accumulator |= 11;  break;
				case 'c':  case 'C':  accumulator |= 12;  break;
				case 'd':  case 'D':  accumulator |= 13;  break;
				case 'e':  case 'E':  accumulator |= 14;  break;
				case 'f':  case 'F':  accumulator |= 15;  break;
			}
		}

		if (i % 2)
			data[(i - 1) / 2] = accumulator;
	}

	*raw = data;

	return (len / 2);
}

/**************************************************************************
 * Base64 Functions
 **************************************************************************/
static const char alphabet[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

static const char xdigits[] =
	"0123456789abcdef";

unsigned char *
gaim_base64_encode(const unsigned char *in, size_t inlen)
{
	char *out, *rv;

	g_return_val_if_fail(in != NULL, NULL);
	g_return_val_if_fail(inlen > 0,  NULL);

	rv = out = g_malloc(((inlen/3)+1)*4 + 1);

	for (; inlen >= 3; inlen -= 3)
	{
		*out++ = alphabet[in[0] >> 2];
		*out++ = alphabet[((in[0] << 4) & 0x30) | (in[1] >> 4)];
		*out++ = alphabet[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
		*out++ = alphabet[in[2] & 0x3f];
		in += 3;
	}

	if (inlen > 0)
	{
		unsigned char fragment;

		*out++ = alphabet[in[0] >> 2];
		fragment = (in[0] << 4) & 0x30;

		if (inlen > 1)
			fragment |= in[1] >> 4;

		*out++ = alphabet[fragment];
		*out++ = (inlen < 2) ? '=' : alphabet[(in[1] << 2) & 0x3c];
		*out++ = '=';
	}

	*out = '\0';

	return rv;
}

void
gaim_base64_decode(const char *text, char **data, int *size)
{
	char *out = NULL;
	char tmp = 0;
	const char *c;
	gint32 tmp2 = 0;
	int len = 0, n = 0;

	g_return_if_fail(text != NULL);
	g_return_if_fail(data != NULL);

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

/**************************************************************************
 * Quoted Printable Functions (see RFC 1341)
 **************************************************************************/
void
gaim_quotedp_decode(const char *str, char **ret_str, int *ret_len)
{
	char *n, *new;
	const char *end, *p;

	n = new = g_malloc(strlen (str) + 1);
	end = str + strlen(str);

	for (p = str; p < end; p++, n++) {
		if (*p == '=') {
			if (p[1] == '\r' && p[2] == '\n') { /* 5.1 #5 */
				n -= 1;
				p += 2;
			} else if (p[1] == '\n') { /* fuzzy case for 5.1 #5 */
				n -= 1;
				p += 1;
			} else if (p[1] && p[2]) {
				char *nibble1 = strchr(xdigits, tolower(p[1]));
				char *nibble2 = strchr(xdigits, tolower(p[2]));
				if (nibble1 && nibble2) { /* 5.1 #1 */
					*n = ((nibble1 - xdigits) << 4) | (nibble2 - xdigits);
					p += 2;
				} else { /* This should never happen */
					*n = *p;
				}
			} else { /* This should never happen */
				*n = *p;
			}
		}
		else if (*p == '_')
			*n = ' ';
		else
			*n = *p;
	}

	*n = '\0';

	if (ret_len)
		*ret_len = n - new;

	/* Resize to take less space */
	/* new = realloc(new, n - new); */

	*ret_str = new;
}

/**************************************************************************
 * MIME Functions
 **************************************************************************/
char *
gaim_mime_decode_field(const char *str)
{
	/*
	 * This is wing's version, partially based on revo/shx's version
	 * See RFC2047 [which apparently obsoletes RFC1342]
	 */
	typedef enum {
		state_start, state_equal1, state_question1,
		state_charset, state_question2,
		state_encoding, state_question3,
		state_encoded_text, state_question4, state_equal2 = state_start
	} encoded_word_state_t;
	encoded_word_state_t state = state_start;
	const char *cur, *mark;
	const char *charset0 = NULL, *encoding0 = NULL, *encoded_text0 = NULL;
	char *n, *new;

	/* token can be any CHAR (supposedly ISO8859-1/ISO2022), not just ASCII */
	#define token_char_p(c) \
		(c != ' ' && !iscntrl(c) && !strchr("()<>@,;:\"/[]?.=", c))

	/* But encoded-text must be ASCII; alas, isascii() may not exist */
	#define encoded_text_char_p(c) \
		((c & 0x80) == 0 && c != '?' && c != ' ' && isgraph(c))

	#define RECOVER_MARKED_TEXT strncpy(n, mark, cur - mark + 1); \
		n += cur - mark + 1

	g_return_val_if_fail(str != NULL, NULL);

	/* NOTE: Assuming that we need just strlen(str)+1 *may* be wrong.
	 * It would be wrong if one byte (in some unknown encoding) could
	 * expand to >=4 bytes of UTF-8; I don't know if there are such things.
	 */
	n = new = g_malloc(strlen(str) + 1);

	/* Here we will be looking for encoded words and if they seem to be
	 * valid then decode them.
	 * They are of this form: =?charset?encoding?text?=
	 */

	for (cur = str, mark = NULL; *cur; cur += 1) {
		switch (state) {
		case state_equal1:
			if (*cur == '?') {
				state = state_question1;
			} else {
				RECOVER_MARKED_TEXT;
				state = state_start;
			}
			break;
		case state_question1:
			if (token_char_p(*cur)) {
				charset0 = cur;
				state = state_charset;
			} else { /* This should never happen */
				RECOVER_MARKED_TEXT;
				state = state_start;
			}
			break;
		case state_charset:
			if (*cur == '?') {
				state = state_question2;
			} else if (!token_char_p(*cur)) { /* This should never happen */
				RECOVER_MARKED_TEXT;
				state = state_start;
			}
			break;
		case state_question2:
			if (token_char_p(*cur)) {
				encoding0 = cur;
				state = state_encoding;
			} else { /* This should never happen */
				RECOVER_MARKED_TEXT;
				state = state_start;
			}
			break;
		case state_encoding:
			if (*cur == '?') {
				state = state_question3;
			} else if (!token_char_p(*cur)) { /* This should never happen */
				RECOVER_MARKED_TEXT;
				state = state_start;
			}
			break;
		case state_question3:
			if (encoded_text_char_p(*cur)) {
				encoded_text0 = cur;
				state = state_encoded_text;
			} else if (*cur == '?') { /* empty string */
				encoded_text0 = cur;
				state = state_question4;
			} else { /* This should never happen */
				RECOVER_MARKED_TEXT;
				state = state_start;
			}
			break;
		case state_encoded_text:
			if (*cur == '?') {
				state = state_question4;
			} else if (!encoded_text_char_p(*cur)) {
				RECOVER_MARKED_TEXT;
				state = state_start;
			}
			break;
		case state_question4:
			if (*cur == '=') { /* Got the whole encoded-word */
				char *charset = g_strndup(charset0, encoding0 - charset0 - 1);
				char *encoding = g_strndup(encoding0, encoded_text0 - encoding0 - 1);
				char *encoded_text = g_strndup(encoded_text0, cur - encoded_text0 - 1);
				char *decoded = NULL;
				int dec_len;
				if (g_ascii_strcasecmp(encoding, "Q") == 0)
					gaim_quotedp_decode(encoded_text, &decoded, &dec_len);
				else if (g_ascii_strcasecmp(encoding, "B") == 0)
					gaim_base64_decode(encoded_text, &decoded, &dec_len);
				else
					decoded = NULL;
				if (decoded) {
					gsize len;
					char *converted = g_convert(decoded, dec_len, "utf-8", charset, NULL, &len, NULL);

					if (converted) {
						n = strncpy(n, converted, len) + len;
						g_free(converted);
					}
					g_free(decoded);
				}
				g_free(charset);
				g_free(encoding);
				g_free(encoded_text);
				state = state_equal2; /* Restart the FSM */
			} else { /* This should never happen */
				RECOVER_MARKED_TEXT;
				state = state_start;
			}
			break;
		default:
			if (*cur == '=') {
				mark = cur;
				state = state_equal1;
			} else {
				/* Some unencoded text. */
				*n = *cur;
				n += 1;
			}
			break;
		} /* switch */
	} /* for */

	if (state != state_start) {
		RECOVER_MARKED_TEXT;
	}
	*n = '\0';

	return new;
}


/**************************************************************************
 * Date/Time Functions
 **************************************************************************/
const char *
gaim_date(void)
{
	static char date[80];
	time_t tme;

	time(&tme);
	strftime(date, sizeof(date), "%H:%M:%S", localtime(&tme));

	return date;
}

const char *
gaim_date_full(void)
{
	char *date;
	time_t tme;

	time(&tme);
	date = ctime(&tme);
	date[strlen(date) - 1] = '\0';

	return date;
}

time_t
gaim_time_build(int year, int month, int day, int hour, int min, int sec)
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

time_t
gaim_str_to_time(const char *timestamp, gboolean utc)
{
    struct tm t;
    time_t retval = 0;
    char buf[32];
    char *c;
    int tzoff = 0;

    time(&retval);
    localtime_r(&retval, &t);

    snprintf(buf, sizeof(buf), "%s", timestamp);
    c = buf;

    /* 4 digit year */
    if(!sscanf(c, "%04d", &t.tm_year)) return 0;
    c+=4;
    if(*c == '-')
        c++;

	t.tm_year -= 1900;

	/* 2 digit month */
	if(!sscanf(c, "%02d", &t.tm_mon)) return 0;
	c+=2;
	if(*c == '-')
		c++;

    t.tm_mon -= 1;


    /* 2 digit day */
    if(!sscanf(c, "%02d", &t.tm_mday)) return 0;
    c+=2;
    if(*c == 'T' || *c == '.') { /* we have more than a date, keep going */
        c++; /* skip the "T" */

        /* 2 digit hour */
        if(sscanf(c, "%02d:%02d:%02d", &t.tm_hour, &t.tm_min, &t.tm_sec) == 3 ||
				sscanf(c, "%02d%02d%02d", &t.tm_hour, &t.tm_min, &t.tm_sec) == 3) {
            int tzhrs, tzmins;
            c+=8;
            if(*c == '.') /* dealing with precision we don't care about */
                c += 4;

            if((*c == '+' || *c == '-') &&
                    sscanf(c+1, "%02d:%02d", &tzhrs, &tzmins)) {
                tzoff = tzhrs*60*60 + tzmins*60;
                if(*c == '+')
                    tzoff *= -1;
            }

			if(tzoff || utc) {

#ifdef HAVE_TM_GMTOFF
                tzoff += t.tm_gmtoff;
#else
#   ifdef HAVE_TIMEZONE
                tzset();    /* making sure */
                tzoff -= timezone;
#   endif
#endif
			}
        }
    }

	t.tm_isdst = -1;

	retval = mktime(&t);

    retval += tzoff;

    return retval;
}



/**************************************************************************
 * Markup Functions
 **************************************************************************/
gboolean
gaim_markup_find_tag(const char *needle, const char *haystack,
					 const char **start, const char **end, GData **attributes)
{
	GData *attribs;
	const char *cur = haystack;
	char *name = NULL;
	gboolean found = FALSE;
	gboolean in_tag = FALSE;
	gboolean in_attr = FALSE;
	const char *in_quotes = NULL;
	size_t needlelen;

	g_return_val_if_fail(    needle != NULL, FALSE);
	g_return_val_if_fail(   *needle != '\0', FALSE);
	g_return_val_if_fail(  haystack != NULL, FALSE);
	g_return_val_if_fail( *haystack != '\0', FALSE);
	g_return_val_if_fail(     start != NULL, FALSE);
	g_return_val_if_fail(       end != NULL, FALSE);
	g_return_val_if_fail(attributes != NULL, FALSE);

	needlelen = strlen(needle);
	g_datalist_init(&attribs);

	while (*cur && !found) {
		if (in_tag) {
			if (in_quotes) {
				const char *close = cur;

				while (*close && *close != *in_quotes)
					close++;

				/* if we got the close quote, store the value and carry on from    *
				 * after it. if we ran to the end of the string, point to the NULL *
				 * and we're outta here */
				if (*close) {
					/* only store a value if we have an attribute name */
					if (name) {
						size_t len = close - cur;
						char *val = g_strndup(cur, len);

						g_datalist_set_data_full(&attribs, name, val, g_free);
						g_free(name);
						name = NULL;
					}

					in_quotes = NULL;
					cur = close + 1;
				} else {
					cur = close;
				}
			} else if (in_attr) {
				const char *close = cur;

				while (*close && *close != '>' && *close != '"' &&
						*close != '\'' && *close != ' ' && *close != '=')
					close++;

				/* if we got the equals, store the name of the attribute. if we got
				 * the quote, save the attribute and go straight to quote mode.
				 * otherwise the tag closed or we reached the end of the string,
				 * so we can get outta here */
				switch (*close) {
				case '"':
				case '\'':
					in_quotes = close;
				case '=':
					{
						size_t len = close - cur;

						/* don't store a blank attribute name */
						if (len) {
							if (name)
								g_free(name);
							name = g_ascii_strdown(cur, len);
						}

						in_attr = FALSE;
						cur = close + 1;
						break;
					}
				case ' ':
				case '>':
					in_attr = FALSE;
				default:
					cur = close;
					break;
				}
			} else {
				switch (*cur) {
				case ' ':
					/* swallow extra spaces inside tag */
					while (*cur && *cur == ' ') cur++;
					in_attr = TRUE;
					break;
				case '>':
					found = TRUE;
					*end = cur;
					break;
				case '"':
				case '\'':
					in_quotes = cur;
				default:
					cur++;
					break;
				}
			}
		} else {
			/* if we hit a < followed by the name of our tag... */
			if (*cur == '<' && !g_ascii_strncasecmp(cur + 1, needle, needlelen)) {
				*start = cur;
				cur = cur + needlelen + 1;

				/* if we're pointing at a space or a >, we found the right tag. if *
				 * we're not, we've found a longer tag, so we need to skip to the  *
				 * >, but not being distracted by >s inside quotes.                */
				if (*cur == ' ' || *cur == '>') {
					in_tag = TRUE;
				} else {
					while (*cur && *cur != '"' && *cur != '\'' && *cur != '>') {
						if (*cur == '"') {
							cur++;
							while (*cur && *cur != '"')
								cur++;
						} else if (*cur == '\'') {
							cur++;
							while (*cur && *cur != '\'')
								cur++;
						} else {
							cur++;
						}
					}
				}
			} else {
				cur++;
			}
		}
	}

	/* clean up any attribute name from a premature termination */
	if (name)
		g_free(name);

	if (found) {
		*attributes = attribs;
	} else {
		*start = NULL;
		*end = NULL;
		*attributes = NULL;
	}

	return found;
}

gboolean
gaim_markup_extract_info_field(const char *str, int len, GString *dest,
							   const char *start_token, int skip,
							   const char *end_token, char check_value,
							   const char *no_value_token,
							   const char *display_name, gboolean is_link,
							   const char *link_prefix)
{
	const char *p, *q;

	g_return_val_if_fail(str          != NULL, FALSE);
	g_return_val_if_fail(dest  != NULL, FALSE);
	g_return_val_if_fail(start_token  != NULL, FALSE);
	g_return_val_if_fail(end_token    != NULL, FALSE);
	g_return_val_if_fail(display_name != NULL, FALSE);

	p = strstr(str, start_token);

	if (p == NULL)
		return FALSE;

	p += strlen(start_token) + skip;

	if (p >= str + len)
		return FALSE;

	if (check_value != '\0' && *p == check_value)
		return FALSE;

	q = strstr(p, end_token);

	/* Trim leading blanks */
	while (*p != '\n' && g_ascii_isspace(*p)) {
		p += 1;
	}

	/* Trim trailing blanks */
	while (q > p && g_ascii_isspace(*(q - 1))) {
		q -= 1;
	}

	/* Don't bother with null strings */
	if (p == q)
		return FALSE;

	if (q != NULL && (!no_value_token ||
					  (no_value_token && strncmp(p, no_value_token,
												 strlen(no_value_token)))))
	{
		g_string_append_printf(dest, _("<b>%s:</b> "), display_name);

		if (is_link)
		{
			g_string_append(dest, "<br><a href=\"");

			if (link_prefix)
				g_string_append(dest, link_prefix);

			g_string_append_len(dest, p, q - p);
			g_string_append(dest, "\">");

			if (link_prefix)
				g_string_append(dest, link_prefix);

			g_string_append_len(dest, p, q - p);
			g_string_append(dest, "</a>");
		}
		else
		{
			g_string_append_len(dest, p, q - p);
		}

		g_string_append(dest, "<br>\n");

		return TRUE;
	}

	return FALSE;
}

struct gaim_parse_tag {
	char *src_tag;
	char *dest_tag;
	gboolean ignore;
};

#define ALLOW_TAG_ALT(x, y) if(!g_ascii_strncasecmp(c, "<" x " ", strlen("<" x " "))) { \
						const char *o = c + strlen("<" x); \
						const char *p = NULL, *q = NULL, *r = NULL; \
						GString *innards = g_string_new(""); \
						while(o && *o) { \
							if(!q && (*o == '\"' || *o == '\'') ) { \
								q = o; \
							} else if(q) { \
								if(*o == *q) { \
									char *unescaped = g_strndup(q+1, o-q-1); \
									char *escaped = g_markup_escape_text(unescaped, -1); \
									g_string_append_printf(innards, "%c%s%c", *q, escaped, *q); \
									g_free(unescaped); \
									g_free(escaped); \
									q = NULL; \
								} else if(*c == '\\') { \
									o++; \
								} \
							} else if(*o == '<') { \
								r = o; \
							} else if(*o == '>') { \
								p = o; \
								break; \
							} else { \
								innards = g_string_append_c(innards, *o); \
							} \
							o++; \
						} \
						if(p && !r) { \
							if(*(p-1) != '/') { \
								struct gaim_parse_tag *pt = g_new0(struct gaim_parse_tag, 1); \
								pt->src_tag = x; \
								pt->dest_tag = y; \
								tags = g_list_prepend(tags, pt); \
							} \
							xhtml = g_string_append(xhtml, "<" y); \
							c += strlen("<" x ); \
							xhtml = g_string_append(xhtml, innards->str); \
							xhtml = g_string_append_c(xhtml, '>'); \
							c = p + 1; \
						} else { \
							xhtml = g_string_append(xhtml, "&lt;"); \
							plain = g_string_append_c(plain, '<'); \
							c++; \
						} \
						g_string_free(innards, TRUE); \
						continue; \
					} \
						if(!g_ascii_strncasecmp(c, "<" x, strlen("<" x)) && \
								(*(c+strlen("<" x)) == '>' || \
								 !g_ascii_strncasecmp(c+strlen("<" x), "/>", 2))) { \
							xhtml = g_string_append(xhtml, "<" y); \
							c += strlen("<" x); \
							if(*c != '/') { \
								struct gaim_parse_tag *pt = g_new0(struct gaim_parse_tag, 1); \
								pt->src_tag = x; \
								pt->dest_tag = y; \
								tags = g_list_prepend(tags, pt); \
								xhtml = g_string_append_c(xhtml, '>'); \
							} else { \
								xhtml = g_string_append(xhtml, "/>");\
							} \
							c = strchr(c, '>') + 1; \
							continue; \
						}
#define ALLOW_TAG(x) ALLOW_TAG_ALT(x, x)
void
gaim_markup_html_to_xhtml(const char *html, char **xhtml_out,
						  char **plain_out)
{
	GString *xhtml = g_string_new("");
	GString *plain = g_string_new("");
	GList *tags = NULL, *tag;
	const char *c = html;

	while(c && *c) {
		if(*c == '<') {
			if(*(c+1) == '/') { /* closing tag */
				tag = tags;
				while(tag) {
					struct gaim_parse_tag *pt = tag->data;
					if(!g_ascii_strncasecmp((c+2), pt->src_tag, strlen(pt->src_tag)) && *(c+strlen(pt->src_tag)+2) == '>') {
						c += strlen(pt->src_tag) + 3;
						break;
					}
					tag = tag->next;
				}
				if(tag) {
					while(tags) {
						struct gaim_parse_tag *pt = tags->data;
						g_string_append_printf(xhtml, "</%s>", pt->dest_tag);
						if(tags == tag)
							break;
						tags = g_list_remove(tags, pt);
						g_free(pt);
					}
					g_free(tag->data);
					tags = g_list_remove(tags, tag->data);
				} else {
					/* a closing tag we weren't expecting...
					 * we'll let it slide, if it's really a tag...if it's
					 * just a </ we'll escape it properly */
					const char *end = c+2;
					while(*end && g_ascii_isalpha(*end))
						end++;
					if(*end == '>') {
						c = end+1;
					} else {
						xhtml = g_string_append(xhtml, "&lt;");
						plain = g_string_append_c(plain, '<');
						c++;
					}
				}
			} else { /* opening tag */
				ALLOW_TAG("a");
				ALLOW_TAG_ALT("b", "strong");
				ALLOW_TAG("blockquote");
				ALLOW_TAG_ALT("bold", "strong");
				ALLOW_TAG("cite");
				ALLOW_TAG("div");
				ALLOW_TAG("em");
				ALLOW_TAG("h1");
				ALLOW_TAG("h2");
				ALLOW_TAG("h3");
				ALLOW_TAG("h4");
				ALLOW_TAG("h5");
				ALLOW_TAG("h6");
				/* we only allow html to start the message */
				if(c == html)
					ALLOW_TAG("html");
				ALLOW_TAG_ALT("i", "em");
				ALLOW_TAG_ALT("italic", "em");
				ALLOW_TAG("li");
				ALLOW_TAG("ol");
				ALLOW_TAG("p");
				ALLOW_TAG("pre");
				ALLOW_TAG("q");
				ALLOW_TAG("span");
				ALLOW_TAG("strong");
				ALLOW_TAG("ul");

				/* we skip <HR> because it's not legal in XHTML-IM.  However,
				 * we still want to send something sensible, so we put a
				 * linebreak in its place. <BR> also needs special handling
				 * because putting a </BR> to close it would just be dumb. */
				if((!g_ascii_strncasecmp(c, "<br", 3)
							|| !g_ascii_strncasecmp(c, "<hr", 3))
						&& (*(c+3) == '>' ||
							!g_ascii_strncasecmp(c+3, "/>", 2) ||
							!g_ascii_strncasecmp(c+3, " />", 3))) {
					c = strchr(c, '>') + 1;
					xhtml = g_string_append(xhtml, "<br/>");
					if(*c != '\n')
						plain = g_string_append_c(plain, '\n');
					continue;
				}
				if(!g_ascii_strncasecmp(c, "<u>", 3) || !g_ascii_strncasecmp(c, "<underline>", strlen("<underline>"))) {
					struct gaim_parse_tag *pt = g_new0(struct gaim_parse_tag, 1);
					pt->src_tag = *(c+2) == '>' ? "u" : "underline";
					pt->dest_tag = "span";
					tags = g_list_prepend(tags, pt);
					c = strchr(c, '>') + 1;
					xhtml = g_string_append(xhtml, "<span style='text-decoration: underline;'>");
					continue;
				}
				if(!g_ascii_strncasecmp(c, "<s>", 3) || !g_ascii_strncasecmp(c, "<strike>", strlen("<strike>"))) {
					struct gaim_parse_tag *pt = g_new0(struct gaim_parse_tag, 1);
					pt->src_tag = *(c+2) == '>' ? "s" : "strike";
					pt->dest_tag = "span";
					tags = g_list_prepend(tags, pt);
					c = strchr(c, '>') + 1;
					xhtml = g_string_append(xhtml, "<span style='text-decoration: line-through;'>");
					continue;
				}
				if(!g_ascii_strncasecmp(c, "<sub>", 5)) {
					struct gaim_parse_tag *pt = g_new0(struct gaim_parse_tag, 1);
					pt->src_tag = "sub";
					pt->dest_tag = "span";
					tags = g_list_prepend(tags, pt);
					c = strchr(c, '>') + 1;
					xhtml = g_string_append(xhtml, "<span style='vertical-align:sub;'>");
					continue;
				}
				if(!g_ascii_strncasecmp(c, "<sup>", 5)) {
					struct gaim_parse_tag *pt = g_new0(struct gaim_parse_tag, 1);
					pt->src_tag = "sup";
					pt->dest_tag = "span";
					tags = g_list_prepend(tags, pt);
					c = strchr(c, '>') + 1;
					xhtml = g_string_append(xhtml, "<span style='vertical-align:super;'>");
					continue;
				}
				if(!g_ascii_strncasecmp(c, "<font", 5) && (*(c+5) == '>' || *(c+5) == ' ')) {
					const char *p = c;
					GString *style = g_string_new("");
					struct gaim_parse_tag *pt;
					while(*p && *p != '>') {
						if(!g_ascii_strncasecmp(p, "back=", strlen("back="))) {
							const char *q = p + strlen("back=");
							GString *color = g_string_new("");
							if(*q == '\'' || *q == '\"')
								q++;
							while(*q && *q != '\"' && *q != '\'' && *q != ' ') {
								color = g_string_append_c(color, *q);
								q++;
							}
							g_string_append_printf(style, "background: %s; ", color->str);
							g_string_free(color, TRUE);
							p = q;
						} else if(!g_ascii_strncasecmp(p, "color=", strlen("color="))) {
							const char *q = p + strlen("color=");
							GString *color = g_string_new("");
							if(*q == '\'' || *q == '\"')
								q++;
							while(*q && *q != '\"' && *q != '\'' && *q != ' ') {
								color = g_string_append_c(color, *q);
								q++;
							}
							g_string_append_printf(style, "color: %s; ", color->str);
							g_string_free(color, TRUE);
							p = q;
						} else if(!g_ascii_strncasecmp(p, "face=", strlen("face="))) {
							const char *q = p + strlen("face=");
							gboolean space_allowed = FALSE;
							GString *face = g_string_new("");
							if(*q == '\'' || *q == '\"') {
								space_allowed = TRUE;
								q++;
							}
							while(*q && *q != '\"' && *q != '\'' && (space_allowed || *q != ' ')) {
								face = g_string_append_c(face, *q);
								q++;
							}
							g_string_append_printf(style, "font-family: %s; ", face->str);
							g_string_free(face, TRUE);
							p = q;
						} else if(!g_ascii_strncasecmp(p, "size=", strlen("size="))) {
							const char *q = p + strlen("size=");
							int sz;
							const char *size = "medium";
							if(*q == '\'' || *q == '\"')
								q++;
							sz = atoi(q);
							switch (sz)
							{
							case 1:
							  size = "xx-small";
							  break;
							case 2:
							  size = "x-small";
							  break;
							case 3:
							  size = "small";
							  break;
							case 4:
							  size = "medium";
							  break;
							case 5:
							  size = "large";
							  break;
							case 6:
							  size = "x-large";
							  break;
							case 7:
							  size = "xx-large";
							  break;
							default:
							  break;
							}
							g_string_append_printf(style, "font-size: %s; ", size);
							p = q;
						}
						p++;
					}
					c = strchr(c, '>') + 1;
					pt = g_new0(struct gaim_parse_tag, 1);
					pt->src_tag = "font";
					pt->dest_tag = "span";
					tags = g_list_prepend(tags, pt);
					if(style->len)
						g_string_append_printf(xhtml, "<span style='%s'>", style->str);
					else
						pt->ignore = TRUE;
					g_string_free(style, TRUE);
					continue;
				}
				if(!g_ascii_strncasecmp(c, "<body ", 6)) {
					const char *p = c;
					gboolean did_something = FALSE;
					while(*p && *p != '>') {
						if(!g_ascii_strncasecmp(p, "bgcolor=", strlen("bgcolor="))) {
							const char *q = p + strlen("bgcolor=");
							struct gaim_parse_tag *pt = g_new0(struct gaim_parse_tag, 1);
							GString *color = g_string_new("");
							if(*q == '\'' || *q == '\"')
								q++;
							while(*q && *q != '\"' && *q != '\'' && *q != ' ') {
								color = g_string_append_c(color, *q);
								q++;
							}
							g_string_append_printf(xhtml, "<span style='background: %s;'>", color->str);
							g_string_free(color, TRUE);
							c = strchr(c, '>') + 1;
							pt->src_tag = "body";
							pt->dest_tag = "span";
							tags = g_list_prepend(tags, pt);
							did_something = TRUE;
							break;
						}
						p++;
					}
					if(did_something) continue;
				}
				/* this has to come after the special case for bgcolor */
				ALLOW_TAG("body");
				if(!g_ascii_strncasecmp(c, "<!--", strlen("<!--"))) {
					char *p = strstr(c + strlen("<!--"), "-->");
					if(p) {
						xhtml = g_string_append(xhtml, "<!--");
						c += strlen("<!--");
						continue;
					}
				}

				xhtml = g_string_append(xhtml, "&lt;");
				plain = g_string_append_c(plain, '<');
				c++;
			}
		} else if(*c == '&') {
			char buf[7];
			char *pln;
			int len = 1;
			guint pound;
			if(!g_ascii_strncasecmp(c, "&amp;", 5)) {
				pln = "&";
				len = 5;
			} else if(!g_ascii_strncasecmp(c, "&lt;", 4)) {
				pln = "<";
				len = 4;
			} else if(!g_ascii_strncasecmp(c, "&gt;", 4)) {
				pln = ">";
				len = 4;
			} else if(!g_ascii_strncasecmp(c, "&nbsp;", 6)) {
				pln = " ";
				len = 6;
			} else if(!g_ascii_strncasecmp(c, "&copy;", 6)) {
				pln = "©";
				len = 6;
			} else if(!g_ascii_strncasecmp(c, "&quot;", 6)) {
				pln = "\"";
				len = 6;
			} else if(!g_ascii_strncasecmp(c, "&reg;", 5)) {
				pln = "®";
				len = 5;
			} else if(!g_ascii_strncasecmp(c, "&apos;", 6)) {
				pln = "\'";
				len = 6;
			} else if(*(c+1) == '#' && (sscanf(c, "&#%u;", &pound) == 1) &&
					pound != 0 && *(c+3+(gint)log10(pound)) == ';') {
				int buflen = g_unichar_to_utf8((gunichar)pound, buf);
				buf[buflen] = '\0';
				pln = buf;


				len = 2;
				while(isdigit((gint) c [len])) len++;
				if(c [len] == ';') len++;
			} else {
				len = 1;
				g_snprintf(buf, sizeof(buf), "%c", *c);
				pln = buf;
			}
			xhtml = g_string_append_len(xhtml, c, len);
			plain = g_string_append(plain, pln);
			c += len;
		} else {
			xhtml = g_string_append_c(xhtml, *c);
			plain = g_string_append_c(plain, *c);
			c++;
		}
	}
	tag = tags;
	while(tag) {
		struct gaim_parse_tag *pt = tag->data;
		if(!pt->ignore)
			g_string_append_printf(xhtml, "</%s>", pt->dest_tag);
		tag = tag->next;
	}
	g_list_free(tags);
	if(xhtml_out)
		*xhtml_out = g_strdup(xhtml->str);
	if(plain_out)
		*plain_out = g_strdup(plain->str);
	g_string_free(xhtml, TRUE);
	g_string_free(plain, TRUE);
}

/* The following are probably reasonable changes:
 * - \n should be converted to a normal space
 * - in addition to <br>, <p> and <div> etc. should also be converted into \n
 * - We want to turn </td>#whitespace<td> sequences into a single tab
 * - We want to turn <td> into a single tab (for msn profile "parsing")
 * - We want to turn </tr>#whitespace<tr> sequences into a single \n
 * - <script>...</script> and <style>...</style> should be completely removed
 */

char *
gaim_markup_strip_html(const char *str)
{
	int i, j, k;
	gboolean visible = TRUE;
	gboolean closing_td_p = FALSE;
	gchar *str2;
	const gchar *cdata_close_tag = NULL;

	if(!str)
		return NULL;

	str2 = g_strdup(str);

	for (i = 0, j = 0; str2[i]; i++)
	{
		if (str2[i] == '<')
		{
			if (cdata_close_tag)
			{
				/* Note: Don't even assume any other tag is a tag in CDATA */
				if (strncasecmp(str2 + i, cdata_close_tag,
						strlen(cdata_close_tag)) == 0)
				{
					i += strlen(cdata_close_tag) - 1;
					cdata_close_tag = NULL;
				}
				continue;
			}
			else if (strncasecmp(str2 + i, "<td", 3) == 0 && closing_td_p)
			{
				str2[j++] = '\t';
				visible = TRUE;
			}
			else if (strncasecmp(str2 + i, "</td>", 5) == 0)
			{
				closing_td_p = TRUE;
				visible = FALSE;
			}
			else
			{
				closing_td_p = FALSE;
				visible = TRUE;
			}

			k = i + 1;

			if(g_ascii_isspace(str2[k]))
				visible = TRUE;
			else if (str2[k])
			{
				/* Scan until we end the tag either implicitly (closed start
				 * tag) or explicitly, using a sloppy method (i.e., < or >
				 * inside quoted attributes will screw us up)
				 */
				while (str2[k] && str2[k] != '<' && str2[k] != '>')
				{
					k++;
				}

				/* Check for tags which should be mapped to newline */
				if (strncasecmp(str2 + i, "<p>", 3) == 0
				 || strncasecmp(str2 + i, "<tr", 3) == 0
				 || strncasecmp(str2 + i, "<br", 3) == 0
				 || strncasecmp(str2 + i, "<li", 3) == 0
				 || strncasecmp(str2 + i, "<div", 4) == 0
				 || strncasecmp(str2 + i, "</table>", 8) == 0)
				{
					str2[j++] = '\n';
				}
				/* Check for tags which begin CDATA and need to be closed */
#if 0 /* FIXME.. option is end tag optional, we can't handle this right now */
				else if (strncasecmp(str2 + i, "<option", 7) == 0)
				{
					/* FIXME: We should not do this if the OPTION is SELECT'd */
					cdata_close_tag = "</option>";
				}
#endif
				else if (strncasecmp(str2 + i, "<script", 7) == 0)
				{
					cdata_close_tag = "</script>";
				}
				else if (strncasecmp(str2 + i, "<style", 6) == 0)
				{
					cdata_close_tag = "</style>";
				}
				/* Update the index and continue checking after the tag */
				i = (str2[k] == '<')? k - 1: k;
				continue;
			}
		}
		else if (cdata_close_tag)
		{
			continue;
		}
		else if (!g_ascii_isspace(str2[i]))
		{
			visible = TRUE;
		}

		/* XXX: This sucks.  We need to be un-escaping all entities, which
		 * includes these, as well as the &#num; ones */

		if (str2[i] == '&' && strncasecmp(str2 + i, "&quot;", 6) == 0)
		{
		    str2[j++] = '\"';
		    i = i + 5;
		    continue;
		}

		if (str2[i] == '&' && strncasecmp(str2 + i, "&amp;", 5) == 0)
		{
			str2[j++] = '&';
			i = i + 4;
			continue;
		}

		if (str2[i] == '&' && strncasecmp(str2 + i, "&lt;", 4) == 0)
		{
			str2[j++] = '<';
			i = i + 3;
			continue;
		}

		if (str2[i] == '&' && strncasecmp(str2 + i, "&gt;", 4) == 0)
		{
			str2[j++] = '>';
			i = i + 3;
			continue;
		}

		if (str2[i] == '&' && strncasecmp(str2 + i, "&apos;", 6) == 0)
		{
			str2[j++] = '\'';
			i = i + 5;
			continue;
		}

		if (visible)
			str2[j++] = g_ascii_isspace(str2[i])? ' ': str2[i];
	}

	str2[j] = '\0';

	return str2;
}

static gint
badchar(char c)
{
	switch (c) {
	case ' ':
	case ',':
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

char *
gaim_markup_linkify(const char *text)
{
	const char *c, *t, *q = NULL;
	char *tmp, *tmpurlbuf;
	char url_buf[BUF_LEN * 4];
	gunichar g;
	gboolean inside_html = FALSE;
	int inside_paren = 0;
	GString *ret = g_string_new("");
	/* Assumes you have a buffer able to cary at least BUF_LEN * 2 bytes */

	c = text;
	while (*c) {

		if(*c == '(' && !inside_html) {
			inside_paren++;
			ret = g_string_append_c(ret, *c);
			c++;
		}

		if(inside_html) {
			if(*c == '>') {
				inside_html = FALSE;
			} else if(!q && (*c == '\"' || *c == '\'')) {
				q = c;
			} else if(q) {
				if(*c == *q)
					q = NULL;
			}
		} else if(*c == '<') {
			inside_html = TRUE;
			if (!g_ascii_strncasecmp(c, "<A", 2)) {
				while (1) {
					if (!g_ascii_strncasecmp(c, "/A>", 3)) {
						inside_html = FALSE;
						break;
					}
					ret = g_string_append_c(ret, *c);
					c++;
					if (!(*c))
						break;
				}
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
					if ((*(t - 1) == ')' && (inside_paren > 0))) {
						t--;
					}
					strncpy(url_buf, c, t - c);
					url_buf[t - c] = 0;
					tmpurlbuf = gaim_unescape_html(url_buf);
					g_string_append_printf(ret, "<A HREF=\"%s\">%s</A>",
							tmpurlbuf, url_buf);
					g_free(tmpurlbuf);
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
						if ((*(t - 1) == ')' && (inside_paren > 0))) {
							t--;
						}
						strncpy(url_buf, c, t - c);
						url_buf[t - c] = 0;
						tmpurlbuf = gaim_unescape_html(url_buf);
						g_string_append_printf(ret,
								"<A HREF=\"http://%s\">%s</A>", tmpurlbuf,
								url_buf);
						g_free(tmpurlbuf);
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
					if ((*(t - 1) == ')' && (inside_paren > 0))) {
						t--;
					}
					strncpy(url_buf, c, t - c);
					url_buf[t - c] = 0;
					tmpurlbuf = gaim_unescape_html(url_buf);
					g_string_append_printf(ret, "<A HREF=\"%s\">%s</A>",
							tmpurlbuf, url_buf);
					g_free(tmpurlbuf);
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
						if ((*(t - 1) == ')' && (inside_paren > 0))) {
							t--;
						}
						strncpy(url_buf, c, t - c);
						url_buf[t - c] = 0;
						tmpurlbuf = gaim_unescape_html(url_buf);
						g_string_append_printf(ret,
								"<A HREF=\"ftp://%s\">%s</A>", tmpurlbuf,
								url_buf);
						g_free(tmpurlbuf);
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
					tmpurlbuf = gaim_unescape_html(url_buf);
					g_string_append_printf(ret, "<A HREF=\"%s\">%s</A>",
							  tmpurlbuf, url_buf);
					g_free(tmpurlbuf);
					c = t;
					break;
				}
				if (!t)
					break;
				t++;

			}
		} else if (c != text && (*c == '@')) {
			int flag;
			GString *gurl_buf;
			const char illegal_chars[] = "!@#$%^&*()[]{}/|\\<>\":;\r\n \0";
			url_buf[0] = 0;

			if (strchr(illegal_chars,*(c - 1)) || strchr(illegal_chars, *(c + 1)))
				flag = 0;
			else
				flag = 1;

			t = c;
			gurl_buf = g_string_new("");
			while (flag) {
				/* iterate backwards grabbing the local part of an email address */
				g = g_utf8_get_char(t);
				if (badchar(*t) || (g >= 127) || (*t == '(') ||
						((*t == ';') && (t > (text+2)) && !g_ascii_strncasecmp(t - 3, "&lt;", 4))) {
					/* local part will already be part of ret, strip it out */
					ret = g_string_truncate(ret, ret->len - (c - t));
					ret = g_string_append_unichar(ret, g);
					break;
				} else {
					g_string_prepend_unichar(gurl_buf, g);
					t = g_utf8_find_prev_char(text, t);
					if (t < text) {
						ret = g_string_assign(ret, "");
						break;
					}
				}
			}

			t = g_utf8_find_next_char(c, NULL);

			while (flag) {
				/* iterate forwards grabbing the domain part of an email address */
				g = g_utf8_get_char(t);
				if (badchar(*t) || (g >= 127) || (*t == ')') ||
						((*t == '&') && !g_ascii_strncasecmp(t, "&gt;", 4))) {
					char *d;

					strcpy(url_buf, gurl_buf->str);

					/* strip off trailing periods */
					for (d = url_buf + strlen(url_buf) - 1; *d == '.'; d--, t--)
						*d = '\0';

					tmpurlbuf = gaim_unescape_html(url_buf);
					if (gaim_email_is_valid(tmpurlbuf)) {
						g_string_append_printf(ret, "<A HREF=\"mailto:%s\">%s</A>",
								tmpurlbuf, url_buf);
					} else {
						g_string_append(ret, url_buf);
					}
					g_free(tmpurlbuf);
					c = t;

					break;
				} else {
					g_string_append_unichar(gurl_buf, g);
					t = g_utf8_find_next_char(t, NULL);
				}
			}
			g_string_free(gurl_buf, TRUE);
		}

		if(*c == ')' && !inside_html) {
			inside_paren--;
			ret = g_string_append_c(ret, *c);
			c++;
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

char *
gaim_escape_html(const char *html) {
	char *escaped = NULL;

	if (html != NULL) {
		const char *c = html;
		GString *ret = g_string_new("");
		while (*c) {
			switch(*c) {
				case '&':
					ret = g_string_append(ret, "&amp;");
					break;
				case '<':
					ret = g_string_append(ret, "&lt;");
					break;
				case '>':
					ret = g_string_append(ret, "&gt;");
					break;
				case '"':
					ret = g_string_append(ret, "&quot;");
					break;
				default:
					ret = g_string_append_c(ret, *c);
			}
			c++;
		}

		escaped = ret->str;
		g_string_free(ret, FALSE);
	}
	return escaped;

}

char *
gaim_unescape_html(const char *html) {
	char *unescaped = NULL;

	if (html != NULL) {
		const char *c = html;
		GString *ret = g_string_new("");
		while (*c) {
			if (!strncmp(c, "&amp;", 5)) {
				ret = g_string_append_c(ret, '&');
				c += 5;
			} else if (!strncmp(c, "&lt;", 4)) {
				ret = g_string_append_c(ret, '<');
				c += 4;
			} else if (!strncmp(c, "&gt;", 4)) {
				ret = g_string_append_c(ret, '>');
				c += 4;
			} else if (!strncmp(c, "&quot;", 6)) {
				ret = g_string_append_c(ret, '"');
				c += 6;
			} else if (!strncmp(c, "&apos;", 6)) {
				ret = g_string_append_c(ret, '\'');
				c += 6;
			} else if (!strncmp(c, "<br>", 4)) {
				ret = g_string_append_c(ret, '\n');
				c += 4;
			} else {
				ret = g_string_append_c(ret, *c);
				c++;
			}
		}

		unescaped = ret->str;
		g_string_free(ret, FALSE);
	}
	return unescaped;

}

char *
gaim_markup_slice(const char *str, guint x, guint y)
{
	GString *ret;
	GQueue *q;
	guint z = 0;
	gboolean appended = FALSE;
	gunichar c;
	char *tag;

	g_return_val_if_fail(x <= y, NULL);

	if (x == y)
		return g_strdup("");

	ret = g_string_new("");
	q = g_queue_new();

	while (*str && (z < y)) {
		c = g_utf8_get_char(str);

		if (c == '<') {
			char *end = strchr(str, '>');

			if (!end) {
				g_string_free(ret, TRUE);
				while ((tag = g_queue_pop_head(q)))
					g_free(tag);
				g_queue_free(q);
				return NULL;
			}

			if (!g_ascii_strncasecmp(str, "<img ", 5)) {
				z += strlen("[Image]");
			} else if (!g_ascii_strncasecmp(str, "<br", 3)) {
				z += 1;
			} else if (!g_ascii_strncasecmp(str, "<hr>", 4)) {
				z += strlen("\n---\n");
			} else if (!g_ascii_strncasecmp(str, "</", 2)) {
				/* pop stack */
				char *tmp;

				tmp = g_queue_pop_head(q);
				if (tmp)
					g_free(tmp);
				/* z += 0; */
			} else {
				/* push it unto the stack */
				char *tmp;

				tmp = g_strndup(str, end - str + 1);
				g_queue_push_head(q, tmp);
				/* z += 0; */
			}

			if (z == x && !appended) {
				GList *l = q->tail;

				while (l) {
					tag = l->data;
					g_string_append(ret, tag);
					l = l->prev;
				}
				appended = TRUE;
			} else if (z >= x) {
				g_string_append_len(ret, str, end - str + 1);
			}

			str = end;
		} else if (c == '&') {
			char *end = strchr(str, ';');
			if (!end) {
				g_string_free(ret, TRUE);
				while ((tag = g_queue_pop_head(q)))
					g_free(tag);
				g_queue_free(q);

				return NULL;
			}

			if (z >= x)
				g_string_append_len(ret, str, end - str + 1);

			z++;
			str = end;
		} else {
			if (z >= x)
				g_string_append_unichar(ret, c);
			z++;
		}

		str = g_utf8_next_char(str);
	}

	while ((tag = g_queue_pop_head(q))) {
		char *name;

		name = gaim_markup_get_tag_name(tag);
		g_string_append_printf(ret, "</%s>", name);
		g_free(name);
		g_free(tag);
	}

	g_queue_free(q);
	return g_string_free(ret, FALSE);
}

char *
gaim_markup_get_tag_name(const char *tag)
{
	int i;
	g_return_val_if_fail(tag != NULL, NULL);
	g_return_val_if_fail(*tag == '<', NULL);

	for (i = 1; tag[i]; i++)
		if (tag[i] == '>' || tag[i] == ' ' || tag[i] == '/')
			break;

	return g_strndup(tag, i);
}

/**************************************************************************
 * Path/Filename Functions
 **************************************************************************/
const char *
gaim_home_dir(void)
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
char *
gaim_user_dir(void)
{
	if (custom_home_dir != NULL && strlen(custom_home_dir) > 0) {
		strcpy ((char*) &home_dir, (char*) &custom_home_dir);
	} else {
		const gchar *hd = gaim_home_dir();

		if (hd) {
			strcpy((char*) &home_dir, hd);
			strcat((char*) &home_dir, G_DIR_SEPARATOR_S ".gaim");
		}
	}

	return home_dir;
}

void set_gaim_user_dir(const char *dir)
{
	if (dir != NULL && strlen(dir) > 0) {
		strcpy((char*) &custom_home_dir, dir);
	}
}

int gaim_build_dir (const char *path, int mode)
{
	char *dir, **components, delim[] = { G_DIR_SEPARATOR, '\0' };
	int cur, len;

	g_return_val_if_fail(path != NULL, -1);

	dir = g_new0(char, strlen(path) + 1);
	components = g_strsplit(path, delim, -1);
	len = 0;
	for (cur = 0; components[cur] != NULL; cur++) {
		/* If you don't know what you're doing on both
		 * win32 and *NIX, stay the hell away from this code */
		if(cur > 1)
			dir[len++] = G_DIR_SEPARATOR;
		strcpy(dir + len, components[cur]);
		len += strlen(components[cur]);
		if(cur == 0)
			dir[len++] = G_DIR_SEPARATOR;

		if(g_file_test(dir, G_FILE_TEST_IS_DIR)) {
			continue;
		} else if(g_file_test(dir, G_FILE_TEST_EXISTS)) {
			gaim_debug(GAIM_DEBUG_WARNING, "build_dir", "bad path: %s\n", path);
			g_strfreev(components);
			g_free(dir);
			return -1;
		}

		if (mkdir(dir, mode) < 0) {
			gaim_debug(GAIM_DEBUG_WARNING, "build_dir", "mkdir: %s\n", strerror(errno));
			g_strfreev(components);
			g_free(dir);
			return -1;
		}
	}

	g_strfreev(components);
	g_free(dir);
	return 0;
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

FILE *
gaim_mkstemp(char **fpath)
{
	const gchar *tmpdir;
#ifndef _WIN32
	int fd;
#endif
	FILE *fp = NULL;

	g_return_val_if_fail(fpath != NULL, NULL);

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

gboolean
gaim_program_is_valid(const char *program)
{
	GError *error = NULL;
	char **argv;
	gchar *progname;
	gboolean is_valid = FALSE;

	g_return_val_if_fail(program != NULL,  FALSE);
	g_return_val_if_fail(*program != '\0', FALSE);

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

char *
gaim_fd_get_ip(int fd)
{
	struct sockaddr addr;
	socklen_t namelen = sizeof(addr);

	g_return_val_if_fail(fd != 0, NULL);

	if (getsockname(fd, &addr, &namelen))
		return NULL;

	return g_strdup(inet_ntoa(((struct sockaddr_in *)&addr)->sin_addr));
}


/**************************************************************************
 * String Functions
 **************************************************************************/
const char *
gaim_normalize(const GaimAccount *account, const char *s)
{
	GaimPlugin *prpl = NULL;
	GaimPluginProtocolInfo *prpl_info = NULL;
	const char *ret = NULL;

	if(account)
		prpl = gaim_find_prpl(gaim_account_get_protocol_id(account));

	if(prpl)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(prpl);

	if(prpl_info && prpl_info->normalize)
		ret = prpl_info->normalize(account, s);

	if(!ret) {
		static char buf[BUF_LEN];
		char *tmp;
		int i, j;

		g_return_val_if_fail(s != NULL, NULL);

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

		ret = buf;
	}
	return ret;
}

gchar *
gaim_str_sub_away_formatters(const char *str, const char *name)
{
	char *c;
	gchar *cpy;
	int cnt = 0;
	time_t t;
	struct tm *tme;
	char tmp[20];

	g_return_val_if_fail(str  != NULL, NULL);
	g_return_val_if_fail(name != NULL, NULL);

	cpy = g_malloc(BUF_LONG);

	t = time(NULL);
	tme = localtime(&t);

	cpy[0] = '\0';
	c = (char *)str;
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
					strftime(tmp, 20, "%I:%M:%S %p", tme);
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

	return cpy;
}

gchar *
gaim_strdup_withhtml(const gchar *src)
{
	gulong destsize, i, j;
	gchar *dest;

	g_return_val_if_fail(src != NULL, NULL);

	/* New length is (length of src) + (number of \n's * 3) + 1 */
	for (i = 0, j = 0; src[i] != '\0'; i++)
		if (src[i] == '\n')
			j++;

	destsize = i + (j * 3) + 1;
	dest = g_malloc(destsize);

	/* Copy stuff, ignoring \r's, because they are dumb */
	for (i = 0, j = 0; src[i] != '\0'; i++) {
		if (src[i] == '\n') {
			strcpy(&dest[j], "<BR>");
			j += 4;
		} else if (src[i] != '\r')
			dest[j++] = src[i];
	}

	dest[destsize-1] = '\0';

	return dest;
}

gboolean
gaim_str_has_prefix(const char *s, const char *p)
{
	if (!strncmp(s, p, strlen(p)))
		return TRUE;

	return FALSE;
}

gboolean
gaim_str_has_suffix(const char *s, const char *x)
{
	int off = strlen(s) - strlen(x);

	if (off >= 0 && !strcmp(s + off, x))
		return TRUE;

	return FALSE;
}

char *
gaim_str_add_cr(const char *text)
{
	char *ret = NULL;
	int count = 0, j;
	guint i;

	g_return_val_if_fail(text != NULL, NULL);

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

	gaim_debug_misc("gaim_str_add_cr", "got: %s, leaving with %s\n",
					text, ret);

	return ret;
}

void
gaim_str_strip_cr(char *text)
{
	int i, j;
	char *text2;

	g_return_if_fail(text != NULL);

	text2 = g_malloc(strlen(text) + 1);

	for (i = 0, j = 0; text[i]; i++)
		if (text[i] != '\r')
			text2[j++] = text[i];
	text2[j] = '\0';

	strcpy(text, text2);
	g_free(text2);
}

gchar *
gaim_strreplace(const char *string, const char *delimiter,
				const char *replacement)
{
	gchar **split;
	gchar *ret;

	g_return_val_if_fail(string      != NULL, NULL);
	g_return_val_if_fail(delimiter   != NULL, NULL);
	g_return_val_if_fail(replacement != NULL, NULL);

	split = g_strsplit(string, delimiter, 0);
	ret = g_strjoinv(replacement, split);
	g_strfreev(split);

	return ret;
}

gchar *
gaim_strcasereplace(const char *string, const char *delimiter,
					const char *replacement)
{
	gchar *ret;
	int length_del, length_rep, i, j;

	g_return_val_if_fail(string      != NULL, NULL);
	g_return_val_if_fail(delimiter   != NULL, NULL);
	g_return_val_if_fail(replacement != NULL, NULL);

	length_del = strlen(delimiter);
	length_rep = strlen(replacement);

	/* Count how many times the delimiter appears */
	i = 0; /* position in the source string */
	j = 0; /* number of occurrences of "delimiter" */
	while (string[i] != '\0') {
		if (!strncasecmp(&string[i], delimiter, length_del)) {
			i += length_del;
			j += length_rep;
		} else {
			i++;
			j++;
		}
	}

	ret = g_malloc(j+1);

	i = 0; /* position in the source string */
	j = 0; /* position in the destination string */
	while (string[i] != '\0') {
		if (!strncasecmp(&string[i], delimiter, length_del)) {
			strncpy(&ret[j], replacement, length_rep);
			i += length_del;
			j += length_rep;
		} else {
			ret[j] = string[i];
			i++;
			j++;
		}
	}

	ret[j] = '\0';

	return ret;
}

const char *
gaim_strcasestr(const char *haystack, const char *needle)
{
	size_t hlen, nlen;
	const char *tmp, *ret;

	g_return_val_if_fail(haystack != NULL, NULL);
	g_return_val_if_fail(needle != NULL, NULL);

	hlen = strlen(haystack);
	nlen = strlen(needle);
	tmp = haystack,
	ret = NULL;

	g_return_val_if_fail(hlen > 0, NULL);
	g_return_val_if_fail(nlen > 0, NULL);

	while (*tmp && !ret) {
		if (!g_ascii_strncasecmp(needle, tmp, nlen))
			ret = tmp;
		else
			tmp++;
	}

	return ret;
}

char *
gaim_str_size_to_units(size_t size)
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

char *
gaim_str_seconds_to_string(guint secs)
{
	GString *gstr;
	const char *prefix = "";
	guint days, hrs, mins;

	gstr = g_string_new("");

	if (secs < 60)
	{
		g_string_append_printf(gstr, "%d %s", secs,
							   ngettext("second", "seconds", secs));
		return g_string_free(gstr, FALSE);
	}

	days = secs / (60 * 60 * 24);
	secs = secs % (60 * 60 * 24);
	hrs  = secs / (60 * 60);
	secs = secs % (60 * 60);
	mins = secs / 60;
	secs = secs % 60;

	if (days > 0)
	{
		g_string_append_printf(gstr, "%d %s", days,
							   ngettext("day", "days", days));

		prefix = ", ";
	}

	if (hrs > 0)
	{
		g_string_append_printf(gstr, "%s%d %s", prefix, hrs,
							   ngettext("hour", "hours", hrs));

		prefix = ", ";
	}

	if (mins > 0)
	{
		g_string_append_printf(gstr, "%s%d %s", prefix, mins,
							   ngettext("minute", "minutes", mins));
	}

	return g_string_free(gstr, FALSE);
}


char *
gaim_str_binary_to_ascii(const unsigned char *binary, guint len)
{
	GString *ret;
	guint i;

	g_return_val_if_fail(len > 0, NULL);

	ret = g_string_sized_new(len);

	for (i = 0; i < len; i++)
		if (binary[i] < 32 || binary[i] > 126)
			g_string_append_printf(ret, "\\x%02hhx", binary[i]);
		else if (binary[i] == '\\')
			g_string_append(ret, "\\\\");
		else
			g_string_append_c(ret, binary[i]);

	return g_string_free(ret, FALSE);
}

/**************************************************************************
 * URI/URL Functions
 **************************************************************************/
gboolean
gaim_url_parse(const char *url, char **ret_host, int *ret_port,
			   char **ret_path, char **ret_user, char **ret_passwd)
{
	char scan_info[255];
	char port_str[6];
	int f;
	const char *at, *slash;
	const char *turl;
	char host[256], path[256], user[256], passwd[256];
	int port = 0;
	/* hyphen at end includes it in control set */
	static char addr_ctrl[] = "A-Za-z0-9.-";
	static char port_ctrl[] = "0-9";
	static char page_ctrl[] = "A-Za-z0-9.~_/:*!@&%%?=+^-";
	static char user_ctrl[] = "A-Za-z0-9.~_/*!&%%?=+^-";
	static char passwd_ctrl[] = "A-Za-z0-9.~_/*!&%%?=+^-";

	g_return_val_if_fail(url != NULL, FALSE);

	if ((turl = strstr(url, "http://")) != NULL ||
		(turl = strstr(url, "HTTP://")) != NULL)
	{
		turl += 7;
		url = turl;
	}

	/* parse out authentication information if supplied */
	/* Only care about @ char BEFORE the first / */
	at = strchr(url, '@');
	slash = strchr(url, '/');
	if ((at != NULL) &&
			(((slash != NULL) && (strlen(at) > strlen(slash))) ||
			(slash == NULL))) {
		g_snprintf(scan_info, sizeof(scan_info),
					"%%255[%s]:%%255[%s]^@", user_ctrl, passwd_ctrl);
		f = sscanf(url, scan_info, user, passwd);

		if (f ==1 ) {
			/* No passwd, possibly just username supplied */
			g_snprintf(scan_info, sizeof(scan_info),
						"%%255[%s]^@", user_ctrl);
			f = sscanf(url, scan_info, user);
			*passwd = '\0';
		}

		url = at+1; /* move pointer after the @ char */
	} else {
		*user = '\0';
		*passwd = '\0';
	}

	g_snprintf(scan_info, sizeof(scan_info),
			   "%%255[%s]:%%5[%s]/%%255[%s]", addr_ctrl, port_ctrl, page_ctrl);

	f = sscanf(url, scan_info, host, port_str, path);

	if (f == 1)
	{
		g_snprintf(scan_info, sizeof(scan_info),
				   "%%255[%s]/%%255[%s]",
				   addr_ctrl, page_ctrl);
		f = sscanf(url, scan_info, host, path);
		g_snprintf(port_str, sizeof(port_str), "80");
	}

	if (f == 1)
		*path = '\0';

	sscanf(port_str, "%d", &port);

	if (ret_host != NULL) *ret_host = g_strdup(host);
	if (ret_port != NULL) *ret_port = port;
	if (ret_path != NULL) *ret_path = g_strdup(path);
	if (ret_user != NULL) *ret_user = g_strdup(user);
	if (ret_passwd != NULL) *ret_passwd = g_strdup(passwd);

	return TRUE;
}

static void
destroy_fetch_url_data(GaimFetchUrlData *gfud)
{
	if (gfud->webdata         != NULL) g_free(gfud->webdata);
	if (gfud->url             != NULL) g_free(gfud->url);
	if (gfud->user_agent      != NULL) g_free(gfud->user_agent);
	if (gfud->website.address != NULL) g_free(gfud->website.address);
	if (gfud->website.page    != NULL) g_free(gfud->website.page);
	if (gfud->website.user    != NULL) g_free(gfud->website.user);
	if (gfud->website.passwd    != NULL) g_free(gfud->website.passwd);

	g_free(gfud);
}

static gboolean
parse_redirect(const char *data, size_t data_len, gint sock,
			   GaimFetchUrlData *gfud)
{
	gchar *s;

	if ((s = g_strstr_len(data, data_len, "Location: ")) != NULL)
	{
		gchar *new_url, *temp_url, *end;
		gboolean full;
		int len;

		s += strlen("Location: ");
		end = strchr(s, '\r');

		/* Just in case :) */
		if (end == NULL)
			end = strchr(s, '\n');

		len = end - s;

		new_url = g_malloc(len + 1);
		strncpy(new_url, s, len);
		new_url[len] = '\0';

		full = gfud->full;

		if (*new_url == '/' || g_strstr_len(new_url, len, "://") == NULL)
		{
			temp_url = new_url;

			new_url = g_strdup_printf("%s:%d%s", gfud->website.address,
									  gfud->website.port, temp_url);

			g_free(temp_url);

			full = FALSE;
		}

		/* Close the existing stuff. */
		gaim_input_remove(gfud->inpa);
		close(sock);

		gaim_debug_info("gaim_url_fetch", "Redirecting to %s\n", new_url);

		/* Try again, with this new location. */
		gaim_url_fetch(new_url, full, gfud->user_agent, gfud->http11,
					   gfud->callback, gfud->user_data);

		/* Free up. */
		g_free(new_url);
		destroy_fetch_url_data(gfud);

		return TRUE;
	}

	return FALSE;
}

static size_t
parse_content_len(const char *data, size_t data_len)
{
	size_t content_len = 0;
	const char *p = NULL;

	/* This is still technically wrong, since headers are case-insensitive
	 * [RFC 2616, section 4.2], though this ought to catch the normal case.
	 * Note: data is _not_ nul-terminated.
	 */
	if (data_len > 16) {
		p = strncmp(data, "Content-Length: ", 16) == 0? data: NULL;
		if (!p) {
			p = g_strstr_len(data, data_len, "\nContent-Length: ");
			if (p)
				p += 1;
		}
	}

	/* If we can find a Content-Length header at all, try to sscanf it.
	 * Response headers should end with at least \r\n, so sscanf is safe,
	 * if we make sure that there is indeed a \n in our header.
	 */
	if (p && g_strstr_len(p, data_len - (p - data), "\n")) {
		sscanf(p, "Content-Length: %d", (int *)&content_len);
		gaim_debug_misc("parse_content_len", "parsed %d\n", content_len);
	}

	return content_len;
}

static void
url_fetched_cb(gpointer url_data, gint sock, GaimInputCondition cond)
{
	GaimFetchUrlData *gfud = url_data;
	char data;
	gboolean got_eof = FALSE;

	if (sock == -1)
	{
		gfud->callback(gfud->user_data, NULL, 0);

		destroy_fetch_url_data(gfud);

		return;
	}

	if (!gfud->sentreq)
	{
		char buf[1024];

		if (gfud->user_agent)
		{
			/* Host header is not forbidden in HTTP/1.0 requests, and HTTP/1.1
			 * clients must know how to handle the "chunked" transfer encoding.
			 * Gaim doesn't know how to handle "chunked", so should always send
			 * the Host header regardless, to get around some observed problems
			 */
			g_snprintf(buf, sizeof(buf),
					   "GET %s%s HTTP/%s\r\n"
					   "User-Agent: %s\r\n"
					   "Host: %s\r\n\r\n",
					   (gfud->full ? "" : "/"),
					   (gfud->full ? gfud->url : gfud->website.page),
					   (gfud->http11 ? "1.1" : "1.0"),
					   gfud->user_agent, gfud->website.address);
		}
		else
		{
			g_snprintf(buf, sizeof(buf),
					   "GET %s%s HTTP/%s\r\n"
					   "Host: %s\r\n\r\n",
					   (gfud->full ? "" : "/"),
					   (gfud->full ? gfud->url : gfud->website.page),
					   (gfud->http11 ? "1.1" : "1.0"),
					   gfud->website.address);
		}

		gaim_debug_misc("gaim_url_fetch", "Request: %s\n", buf);

		write(sock, buf, strlen(buf));
		fcntl(sock, F_SETFL, O_NONBLOCK);
		gfud->sentreq = TRUE;
		gfud->inpa = gaim_input_add(sock, GAIM_INPUT_READ,
									url_fetched_cb, url_data);
		gfud->data_len = 4096;
		gfud->webdata = g_malloc(gfud->data_len);

		return;
	}

	if (read(sock, &data, 1) > 0 || errno == EWOULDBLOCK)
	{
		if (errno == EWOULDBLOCK)
		{
			errno = 0;

			return;
		}

		gfud->len++;

		if (gfud->len == gfud->data_len + 1)
		{
			gfud->data_len += (gfud->data_len) / 2;

			gfud->webdata = g_realloc(gfud->webdata, gfud->data_len);
		}

		gfud->webdata[gfud->len - 1] = data;

		if (!gfud->startsaving)
		{
			if (data == '\r')
				return;

			if (data == '\n')
			{
				if (gfud->newline)
				{
					size_t content_len;
					gfud->startsaving = TRUE;

					gaim_debug_misc("gaim_url_fetch", "Response headers: '%*.*s'\n", gfud->len, gfud->len, gfud->webdata);

					/* See if we can find a redirect. */
					if (parse_redirect(gfud->webdata, gfud->len, sock, gfud))
						return;

					/* No redirect. See if we can find a content length. */
					content_len = parse_content_len(gfud->webdata, gfud->len);

					if (content_len == 0)
					{
						/* We'll stick with an initial 8192 */
						content_len = 8192;
					}
					else
					{
						gfud->has_explicit_data_len = TRUE;
					}

					/* Out with the old... */
					gfud->len = 0;
					g_free(gfud->webdata);
					gfud->webdata = NULL;

					/* In with the new. */
					gfud->data_len = content_len;
					gfud->webdata = g_malloc(gfud->data_len);
				}
				else
					gfud->newline = TRUE;

				return;
			}

			gfud->newline = FALSE;
		}
		else if (gfud->has_explicit_data_len && gfud->len == gfud->data_len)
		{
			got_eof = TRUE;
		}
	}
	else if (errno != ETIMEDOUT)
	{
		got_eof = TRUE;
	}
	else
	{
		gaim_input_remove(gfud->inpa);
		close(sock);

		gfud->callback(gfud->user_data, NULL, 0);

		destroy_fetch_url_data(gfud);
	}

	if (got_eof) {
		gfud->webdata = g_realloc(gfud->webdata, gfud->len + 1);
		gfud->webdata[gfud->len] = 0;

		/* gaim_debug_misc("gaim_url_fetch", "Received: '%s'\n", gfud->webdata); */

		gaim_input_remove(gfud->inpa);
		close(sock);
		gfud->callback(gfud->user_data, gfud->webdata, gfud->len);

		destroy_fetch_url_data(gfud);
	}
}

void
gaim_url_fetch(const char *url, gboolean full,
			   const char *user_agent, gboolean http11,
			   void (*cb)(gpointer, const char *, size_t),
			   void *user_data)
{
	int sock;
	GaimFetchUrlData *gfud;

	g_return_if_fail(url != NULL);
	g_return_if_fail(cb  != NULL);

	gaim_debug_info("gaim_url_fetch",
			 "requested to fetch (%s), full=%d, user_agent=(%s), http11=%d\n",
			 url, full, user_agent?user_agent:"(null)", http11);

	gfud = g_new0(GaimFetchUrlData, 1);

	gfud->callback   = cb;
	gfud->user_data  = user_data;
	gfud->url        = g_strdup(url);
	gfud->user_agent = (user_agent != NULL ? g_strdup(user_agent) : NULL);
	gfud->http11     = http11;
	gfud->full       = full;

	gaim_url_parse(url, &gfud->website.address, &gfud->website.port,
				   &gfud->website.page, &gfud->website.user, &gfud->website.passwd);

	if ((sock = gaim_proxy_connect(NULL, gfud->website.address,
								   gfud->website.port, url_fetched_cb,
								   gfud)) < 0)
	{
		destroy_fetch_url_data(gfud);

		cb(user_data, g_strdup(_("g003: Error opening connection.\n")), 0);
	}
}

const char *
gaim_url_decode(const char *str)
{
	static char buf[BUF_LEN];
	guint i, j = 0;
	char *bum;

	g_return_val_if_fail(str != NULL, NULL);

	for (i = 0; i < strlen(str); i++) {
		char hex[3];

		if (str[i] != '%')
			buf[j++] = str[i];
		else {
			strncpy(hex, str + ++i, 2);
			hex[2] = '\0';

			/* i is pointing to the start of the number */
			i++;

			/*
			 * Now it's at the end and at the start of the for loop
			 * will be at the next character.
			 */
			buf[j++] = strtol(hex, NULL, 16);
		}
	}

	buf[j] = '\0';

	if (!g_utf8_validate(buf, -1, (const char **)&bum))
		*bum = '\0';

	return buf;
}

const char *
gaim_url_encode(const char *str)
{
	static char buf[BUF_LEN];
	guint i, j = 0;

	g_return_val_if_fail(str != NULL, NULL);

	for (i = 0; i < strlen(str); i++) {
		if (isalnum(str[i]))
			buf[j++] = str[i];
		else {
			sprintf(buf + j, "%%%02x", (unsigned char)str[i]);
			j += 3;
		}
	}

	buf[j] = '\0';

	return buf;
}

/* Originally lifted from
 * http://www.oreillynet.com/pub/a/network/excerpt/spcookbook_chap03/index3.html
 * ... and slightly modified to be a bit more rfc822 compliant
 * ... and modified a bit more to make domain checking rfc1035 compliant
 *     with the exception permitted in rfc1101 for domains to start with digit
 *     but not completely checking to avoid conflicts with IP addresses
 */
gboolean
gaim_email_is_valid(const char *address)
{
	const char *c, *domain;
	static char *rfc822_specials = "()<>@,;:\\\"[]";

	/* first we validate the name portion (name@domain) (rfc822)*/
	for (c = address;  *c;  c++) {
		if (*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) == '\"')) {
			while (*++c) {
				if (*c == '\\') {
					if (*c++ && *c < 127 && *c != '\n' && *c != '\r') continue;
					else return FALSE;
				}
				if (*c == '\"') break;
				if (*c < ' ' || *c >= 127) return FALSE;
			}
			if (!*c++) return FALSE;
			if (*c == '@') break;
			if (*c != '.') return FALSE;
			continue;
		}
		if (*c == '@') break;
		if (*c <= ' ' || *c >= 127) return FALSE;
		if (strchr(rfc822_specials, *c)) return FALSE;
	}
	/* strictly we should return false if (*(c - 1) == '.') too, but I think
	 * we should permit user.@domain type addresses - they do work :) */
	if (c == address) return FALSE;

	/* next we validate the domain portion (name@domain) (rfc1035 & rfc1011) */
	if (!*(domain = ++c)) return FALSE;
	do {
		if (*c == '.' && (c == domain || *(c - 1) == '.' || *(c - 1) == '-'))
			return FALSE;
		if (*c == '-' && *(c - 1) == '.') return FALSE;
		if ((*c < '0' && *c != '-' && *c != '.') || (*c > '9' && *c < 'A') ||
			(*c > 'Z' && *c < 'a') || (*c > 'z')) return FALSE;
	} while (*++c);

	if (*(c - 1) == '-') return FALSE;

	return ((c - domain) > 3 ? TRUE : FALSE);
}

/** Stolen from gnome_uri_list_extract_uris **/
GList* gaim_uri_list_extract_uris (const gchar* uri_list) {
	const gchar *p, *q;
	gchar *retval;
	GList *result = NULL;

	g_return_val_if_fail (uri_list != NULL, NULL);

	p = uri_list;

	/* We don't actually try to validate the URI according to RFC
	* 2396, or even check for allowed characters - we just ignore
	* comments and trim whitespace off the ends.  We also
	* allow LF delimination as well as the specified CRLF.
	*/
	while (p) {
		if (*p != '#') {
			while (isspace(*p))
				p++;

			q = p;
			while (*q && (*q != '\n') && (*q != '\r'))
				q++;

			if (q > p) {
				q--;
				while (q > p && isspace(*q))
					q--;

				retval = (gchar*)g_malloc (q - p + 2);
				strncpy (retval, p, q - p + 1);
				retval[q - p + 1] = '\0';

				result = g_list_prepend (result, retval);
			}
		}
		p = strchr (p, '\n');
		if (p)
			p++;
	}

	return g_list_reverse (result);
}


/** Stolen from gaim_uri_list_extract_filenames **/
GList* gaim_uri_list_extract_filenames (const gchar* uri_list) {
	GList *tmp_list, *node, *result;

	g_return_val_if_fail (uri_list != NULL, NULL);

	result = gaim_uri_list_extract_uris (uri_list);

	tmp_list = result;
	while (tmp_list) {
		gchar *s = (gchar*)tmp_list->data;

		node = tmp_list;
		tmp_list = tmp_list->next;

		if (!strncmp (s, "file:", 5)) {
			node->data = g_filename_from_uri (s, NULL, NULL);
			/* not sure if this fallback is useful at all */
			if (!node->data) node->data = g_strdup (s+5);
		} else {
			result = g_list_remove_link(result, node);
			g_list_free_1 (node);
		}
		g_free (s);
	}
	return result;
}

/**************************************************************************
 * UTF8 String Functions
 **************************************************************************/
gchar *
gaim_utf8_try_convert(const char *str)
{
	gsize converted;
	gchar *utf8;

	g_return_val_if_fail(str != NULL, NULL);

	if (g_utf8_validate(str, -1, NULL)) {
		return g_strdup(str);
	}

	utf8 = g_locale_to_utf8(str, -1, &converted, NULL, NULL);
	if (utf8 != NULL)
		return utf8;

	utf8 = g_convert(str, -1, "UTF-8", "ISO-8859-15", &converted, NULL, NULL);
	if ((utf8 != NULL) && (converted == strlen(str)))
		return utf8;

	if (utf8 != NULL)
		g_free(utf8);

	return NULL;
}

char *
gaim_utf8_ncr_decode(const char *in)
{
	GString *out = g_string_new("");
	int i;

	g_return_val_if_fail(in != NULL, NULL);
	g_return_val_if_fail(g_utf8_validate(in, -1, NULL), NULL);

	for (i = 0; in[i]; i += 1) {
		gboolean ncr_found_p = FALSE;
		if (in[i] == '&' && in[i + 1] == '#' && isdigit(in[i + 2])) {
			gunichar wc;
			int j;
			for (wc = 0, j = i + 2; isdigit(in[j]); j += 1) {
				wc *= 10;
				wc += in[j] - '0';
			}
			if (in[j] == ';') { /* Technically not completely correct */
				g_string_append_unichar(out, wc);
				i = j;
				ncr_found_p = TRUE;
			}
		}
		if (!ncr_found_p) {
			g_string_append_c(out, in[i]);
		}
	}
	return g_string_free(out, FALSE);
}

int
gaim_utf8_strcasecmp(const char *a, const char *b)
{
	char *a_norm = NULL;
	char *b_norm = NULL;
	int ret = -1;

	if(!a && b)
		return -1;
	else if(!b && a)
		return 1;
	else if(!a && !b)
		return 0;

	if(!g_utf8_validate(a, -1, NULL) || !g_utf8_validate(b, -1, NULL))
	{
		gaim_debug_error("gaim_utf8_strcasecmp",
						 "One or both parameters are invalid UTF8\n");
		return ret;
	}

	a_norm = g_utf8_casefold(a, -1);
	b_norm = g_utf8_casefold(b, -1);
	ret = g_utf8_collate(a_norm, b_norm);
	g_free(a_norm);
	g_free(b_norm);

	return ret;
}

gboolean gaim_message_meify(char *message, size_t len)
{
	char *c;
	gboolean inside_html = FALSE;

	g_return_val_if_fail(message != NULL, FALSE);

	if(len == -1)
		len = strlen(message);

	for (c = message; *c; c++, len--) {
		if(inside_html) {
			if(*c == '>')
				inside_html = FALSE;
		} else {
			if(*c == '<')
				inside_html = TRUE;
			else
				break;
		}
	}

	if(*c && !g_ascii_strncasecmp(c, "/me ", 4)) {
		memmove(c, c+4, len-3);
		return TRUE;
	}

	return FALSE;
}

char *gaim_text_strip_mnemonic(const char *in)
{
	char *out;
	char *a;
	char *a0;
	const char *b;

	g_return_val_if_fail(in != NULL, NULL);

	out = g_malloc(strlen(in)+1);
	a = out;
	b = in;

	a0 = a; /* The last non-space char seen so far, or the first char */

	while(*b) {
		if(*b == '_') {
			if(a > out && b > in && *(b-1) == '(' && *(b+1) && !(*(b+1) & 0x80) && *(b+2) == ')') {
				/* Detected CJK style shortcut (Bug 875311) */
				a = a0;	/* undo the left parenthesis */
				b += 3;	/* and skip the whole mess */
			} else if(*(b+1) == '_') {
				*(a++) = '_';
				b += 2;
				a0 = a;
			} else {
				b++;
			}
		/* We don't want to corrupt the middle of UTF-8 characters */
		} else if (!(*b & 0x80)) {	/* other 1-byte char */
			if (*b != ' ')
				a0 = a;
			*(a++) = *(b++);
		} else {
			/* Multibyte utf8 char, don't look for _ inside these */
			int n = 0;
			int i;
			if ((*b & 0xe0) == 0xc0) {
				n = 2;
			} else if ((*b & 0xf0) == 0xe0) {
				n = 3;
			} else if ((*b & 0xf8) == 0xf0) {
				n = 4;
			} else if ((*b & 0xfc) == 0xf8) {
				n = 5;
			} else if ((*b & 0xfe) == 0xfc) {
				n = 6;
			} else {		/* Illegal utf8 */
				n = 1;
			}
			a0 = a; /* unless we want to delete CJK spaces too */
			for (i = 0; i < n && *b; i += 1) {
				*(a++) = *(b++);
			}
		}
	}
	*a = '\0';

	return out;
}

