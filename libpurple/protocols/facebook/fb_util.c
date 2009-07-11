/*
 * libfacebook
 *
 * libfacebook is the property of its developers.  See the COPYRIGHT file
 * for more details.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fb_util.h"

/*****************************************************************************
 * UTILITY CODE                                                              *
 *****************************************************************************/

/* Converts *text* into <b>text</b>  and _text_ into <i>text</i> */
gchar *fb_replace_styled_text(const gchar *text)
{
	if (glib_check_version(2, 14, 0))
	{
		return g_strdup(text);
	} else {
		static GRegex *underline_regex = NULL;
		static GRegex *bold_regex = NULL;
		gchar *dup_text;
		gchar *midway_string;
		gchar *output_string;
		
		if (underline_regex == NULL)
		{
			underline_regex = g_regex_new(
				"\\b_([^_\\*]+)_\\b", G_REGEX_OPTIMIZE,
				0, NULL);
		}
		if (bold_regex == NULL)
		{
			bold_regex = g_regex_new(
				"(\\s|^)\\*([^_\\*]+)\\*(?=$|\\s)",
				G_REGEX_OPTIMIZE, 0, NULL);
		}
		
		dup_text = g_strdup(text);
		midway_string = g_regex_replace(underline_regex, dup_text,
			-1, 0, "<u>\\1</u>", 0, NULL);
		g_free(dup_text);
		output_string = g_regex_replace(bold_regex, midway_string,
			-1, 0, "\\1<b>\\2</b>", 0, NULL);
		g_free(midway_string);
		
		return output_string;
	}
}

