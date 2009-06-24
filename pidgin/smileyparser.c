
#include <gtk/gtk.h>
#include "smileyparser.h"
#include <smiley.h>
#include <string.h>
#include "gtkthemes.h"

static char*
smiley_parse_markup_for_smiley (const char* markup, PurpleSmiley *smiley) 
{
	const char* shortcut = purple_smiley_get_shortcut (smiley);
	GString* ret = g_string_new ("");
	const char *tmp = markup;

	for(;*tmp;) {
		const char *end = strstr (tmp, shortcut);
		char *path;

		if (end == NULL) {
			g_string_append (ret, tmp);
			break;
		}
		path = purple_smiley_get_full_path (smiley);
		g_string_append_len (ret, tmp, end-tmp);
		g_string_append_printf (ret,"<img alt='%s' src='%s' />",
					shortcut, path);
		g_free (path);

		g_assert (strlen (tmp) >= strlen (shortcut));
		tmp = end + strlen (shortcut);
	}

	return g_string_free (ret, FALSE);
}

char*
smiley_parse_markup (const char* markup) 
{
	GList *smileys = purple_smileys_get_all ();
	char *temp = g_strdup (markup);

	/* unnecessarily slow, but lets manage for now. */
	for (; smileys; smileys = g_list_next (smileys)) {
		char* replaced = smiley_parse_markup_for_smiley (temp, PURPLE_SMILEY (smileys->data));
		g_free (temp);
		temp = replaced;
	}
	return temp;
}

