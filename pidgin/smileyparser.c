
#include <gtk/gtk.h>
#include "smileyparser.h"
#include <smiley.h>
#include <string.h>
#include "gtkthemes.h"

static char* get_fullpath (const char* filename)
{
	if (g_path_is_absolute (filename)) return g_strdup (filename);
	else return g_build_path (g_get_current_dir (), filename, NULL);
}

static char*
parse_for_shortcut (const char* markup, const char* shortcut, const char* file) 
{
	GString* ret = g_string_new ("");
	const char *tmp = markup;

	for(;*tmp;) {
		const char *end = strstr (tmp, shortcut);
		char *path;

		if (end == NULL) {
			g_string_append (ret, tmp);
			break;
		}
		path = get_fullpath (file);
		g_string_append_len (ret, tmp, end-tmp);
		g_string_append_printf (ret,"<img alt='%s' src='%s' />",
					shortcut, path);
		g_free (path);

		g_assert (strlen (tmp) >= strlen (shortcut));
		tmp = end + strlen (shortcut);
	}

	return g_string_free (ret, FALSE);
}

static char*
parse_for_purple_smiley (const char* markup, PurpleSmiley *smiley)
{
	char *file = purple_smiley_get_full_path (smiley);
	char *ret = parse_for_shortcut (markup, purple_smiley_get_shortcut (smiley), file);
	g_free (file);
	return ret;
}

static char*
parse_for_smiley_list (const char* markup, GHashTable* smileys)
{
	GHashTableIter iter;
	char *key, *value;
	char *ret = g_strdup (markup);

	g_hash_table_iter_init (&iter, smileys);
	while (g_hash_table_iter_next (&iter, (gpointer*)&key, (gpointer*)&value))
	{
		char* temp = parse_for_shortcut (ret, key, value);
		g_free (ret);
		ret = temp;
	}
	return ret;
}

char*
smiley_parse_markup (const char* markup, const char *proto_id) 
{
	GList *smileys = purple_smileys_get_all ();
	char *temp = g_strdup (markup), *temp2;
	struct smiley_list *list;
	const char *proto_name = "default";

	if (proto_id != NULL) {
		PurplePlugin *proto;
		proto = purple_find_prpl (proto_id);
		proto_name = proto->info->name;
	}

	/* unnecessarily slow, but lets manage for now. */
	for (; smileys; smileys = g_list_next (smileys)) {
		temp2 = parse_for_purple_smiley (temp, PURPLE_SMILEY (smileys->data));
		g_free (temp);
		temp = temp2;
	}

	/* now for each theme smiley, observe that this does look nasty */
	
	if (!current_smiley_theme || !(current_smiley_theme->list)) {
		printf ("theme does not exist\n");
		return temp;
	}

	for (list = current_smiley_theme->list; list; list = list->next) {
		if (g_str_equal (list->sml, proto_name)) {
			temp2 = parse_for_smiley_list (temp, list->files);
			g_free (temp);
			temp = temp2;
		}
	}


	return temp;
}

