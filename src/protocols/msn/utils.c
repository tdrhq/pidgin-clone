/**
 * @file utils.c Utility functions
 *
 * gaim
 *
 * Copyright (C) 2003-2004 Christian Hammond <chipx86@gnupdate.org>
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
#include "msn.h"

void
msn_parse_format(const char *mime, char **pre_ret, char **post_ret)
{
	char *cur;
	GString *pre  = g_string_new(NULL);
	GString *post = g_string_new(NULL);
	unsigned int colors[3];

	if (pre_ret  != NULL) *pre_ret  = NULL;
	if (post_ret != NULL) *post_ret = NULL;

	cur = strstr(mime, "FN=");

	if (cur && (*(cur = cur + 3) != ';'))
	{
		pre = g_string_append(pre, "<FONT FACE=\"");

		while (*cur && *cur != ';')
		{
			pre = g_string_append_c(pre, *cur);
			cur++;
		}

		pre = g_string_append(pre, "\">");
		post = g_string_prepend(post, "</FONT>");
	}

	cur = strstr(mime, "EF=");

	if (cur && (*(cur = cur + 3) != ';'))
	{
		while (*cur && *cur != ';')
		{
			pre = g_string_append_c(pre, '<');
			pre = g_string_append_c(pre, *cur);
			pre = g_string_append_c(pre, '>');
			cur++;
		}
	}

	cur = strstr(mime, "CO=");

	if (cur && (*(cur = cur + 3) != ';'))
	{
		int i;

		i = sscanf(cur, "%02x%02x%02x;", &colors[0], &colors[1], &colors[2]);

		if (i > 0)
		{
			char tag[64];

			if (i == 1)
			{
				colors[1] = 0;
				colors[2] = 0;
			}
			else if (i == 2)
			{
				unsigned int temp = colors[0];

				colors[0] = colors[1];
				colors[1] = temp;
				colors[2] = 0;
			}
			else if (i == 3)
			{
				unsigned int temp = colors[2];

				colors[2] = colors[0];
				colors[0] = temp;
			}

			g_snprintf(tag, sizeof(tag),
					   "<FONT COLOR=\"#%02hhx%02hhx%02hhx\">",
					   colors[0], colors[1], colors[2]);

			pre = g_string_append(pre, tag);
			post = g_string_prepend(post, "</FONT>");
		}
	}

	cur = g_strdup(gaim_url_decode(pre->str));
	g_string_free(pre, TRUE);

	if (pre_ret != NULL)
		*pre_ret = cur;
	else
		g_free(cur);

	cur = g_strdup(gaim_url_decode(post->str));
	g_string_free(post, TRUE);

	if (post_ret != NULL)
		*post_ret = cur;
	else
		g_free(cur);
}

/*
 * We need this because we're only supposed to encode spaces in the font
 * names. gaim_url_encode() isn't acceptable.
 */
const char *
encode_spaces(const char *str)
{
	static char buf[BUF_LEN];
	const char *c;
	char *d;

	g_return_val_if_fail(str != NULL, NULL);

	for (c = str, d = buf; *c != '\0'; c++)
	{
		if (*c == ' ')
		{
			*d++ = '%';
			*d++ = '2';
			*d++ = '0';
		}
		else
			*d++ = *c;
	}

	return buf;
}

/*
 * Taken from the zephyr plugin.
 * This parses HTML formatting (put out by one of the gtkimhtml widgets
 * and converts it to msn formatting. It doesn't deal with the tag closing,
 * but gtkimhtml widgets give valid html.
 * It currently deals properly with <b>, <u>, <i>, <font face=...>,
 * <font color=...>.
 * It ignores <font back=...> and <font size=...>
 */
void
msn_import_html(const char *html, char **attributes, char **message)
{
	int len, retcount = 0;
	const char *c;
	char *msg;
	char *fontface = NULL;
	char fonteffect[4];
	char fontcolor[7];

	g_return_if_fail(html       != NULL);
	g_return_if_fail(attributes != NULL);
	g_return_if_fail(message    != NULL);

	len = strlen(html);
	msg = g_malloc0(len + 1);

	memset(fontcolor, 0, sizeof(fontcolor));
	memset(fonteffect, 0, sizeof(fontcolor));

	for (c = html; *c != '\0';)
	{
		if (*c == '<')
		{
			if (!g_ascii_strncasecmp(c + 1, "i>", 2))
			{
				strcat(fonteffect, "I");
				c += 3;
			}
			else if (!g_ascii_strncasecmp(c + 1, "b>", 2))
			{
				strcat(fonteffect, "B");
				c += 3;
			}
			else if (!g_ascii_strncasecmp(c + 1, "u>", 2))
			{
				strcat(fonteffect, "U");
				c += 3;
			}
			else if (!g_ascii_strncasecmp(c + 1, "a href=\"", 8))
			{
				c += 9;

				while (g_ascii_strncasecmp(c, "\">", 2))
					msg[retcount++] = *c++;

				c += 2;

				/* ignore descriptive string */
				while (g_ascii_strncasecmp(c, "</a>", 4))
					c++;

				c += 4;
			}
			else if (!g_ascii_strncasecmp(c + 1, "font", 4))
			{
				c += 5;

				while (!g_ascii_strncasecmp(c, " ", 1))
					c++;

				if (!g_ascii_strncasecmp(c, "color=\"#", 7))
				{
					c += 8;

					fontcolor[0] = *(c + 4);
					fontcolor[1] = *(c + 5);
					fontcolor[2] = *(c + 2);
					fontcolor[3] = *(c + 3);
					fontcolor[4] = *c;
					fontcolor[5] = *(c + 1);

					c += 8;
				}
				else if (!g_ascii_strncasecmp(c, "face=\"", 6))
				{
					const char *end = NULL;
					unsigned int namelen = 0;

					c += 6;
					end = strchr(c, '\"');
					namelen = (unsigned int)(end - c);
					fontface = g_strndup(c, namelen);
					c = end + 2;
				}
				else
				{
					/* Drop all unrecognized/misparsed font tags */
					while (g_ascii_strncasecmp(c, "\">", 2))
						c++;

					c += 2;
				}
			}
			else
			{
				while (g_ascii_strncasecmp(c, ">", 1))
					c++;

				c++;
			}
		} else if (*c == '&') {
			if (!g_ascii_strncasecmp(c, "&lt;", 4)) {
				msg[retcount++] = '<';
				c += 4;
			} else if (!g_ascii_strncasecmp(c, "&gt;", 4)) {
				msg[retcount++] = '>';
				c += 4;
			} else if (!g_ascii_strncasecmp(c, "&quot;", 6)) {
				msg[retcount++] = '"';
				c += 6;
			} else if (!g_ascii_strncasecmp(c, "&amp;", 5)) {
				msg[retcount++] = '&';
				c += 5;
			}
		} else
			msg[retcount++] = *c++;
	}

	if (fontface == NULL)
		fontface = g_strdup("MS Sans Serif");

	*attributes = g_strdup_printf("FN=%s; EF=%s; CO=%s; PF=0",
								  encode_spaces(fontface),
								  fonteffect, fontcolor);
	*message = g_strdup(msg);

	g_free(fontface);
	g_free(msg);
}
