/*
 * Themes for Pidgin
 *
 * Pidgin is the legal property of its developers, whose names are too numerous
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 *
 */
#include "internal.h"
#include "pidgin.h"

#include "conversation.h"
#include "debug.h"
#include "prpl.h"
#include "util.h"

#include "gtkconv.h"
#include "gtkdialogs.h"
#include "gtkimhtml.h"
#include "gtksmiley.h"
#include "gtkthemes.h"

GSList *smiley_themes = NULL;
struct smiley_theme *current_smiley_theme;

static void pidgin_themes_destroy_smiley_theme_smileys(struct smiley_theme *theme);

gboolean pidgin_themes_smileys_disabled()
{
	if (!current_smiley_theme)
		return 1;

	return strcmp(current_smiley_theme->name, "none") == 0;
}

static void
pidgin_themes_destroy_smiley_theme(struct smiley_theme *theme)
{
	pidgin_themes_destroy_smiley_theme_smileys(theme);

	g_free(theme->name);
	g_free(theme->desc);
	g_free(theme->author);
	g_free(theme->icon);
	g_free(theme->path);
	g_free(theme);
}

static void pidgin_themes_remove_theme_dir(const char *theme_dir_name)
{
	GString *str = NULL;
	const char *file_name = NULL;
	GDir *theme_dir = NULL;

	if ((theme_dir = g_dir_open(theme_dir_name, 0, NULL)) != NULL) {
		if ((str = g_string_new(theme_dir_name)) != NULL) {
			while ((file_name = g_dir_read_name(theme_dir)) != NULL) {
				g_string_printf(str, "%s%s%s", theme_dir_name, G_DIR_SEPARATOR_S, file_name);
				g_unlink(str->str);
			}
			g_string_free(str, TRUE);
		}
		g_dir_close(theme_dir);
		g_rmdir(theme_dir_name);
	}
}

void pidgin_themes_remove_smiley_theme(const char *file)
{
	char *theme_dir = NULL, *last_slash = NULL;
	g_return_if_fail(NULL != file);
	
	if (!g_file_test(file, G_FILE_TEST_EXISTS)) return;
	if ((theme_dir = g_strdup(file)) == NULL) return ;

	if ((last_slash = g_strrstr(theme_dir, G_DIR_SEPARATOR_S)) != NULL) {
		GSList *iter = NULL;
		struct smiley_theme *theme = NULL, *new_theme = NULL;

		*last_slash = 0;

		/* Delete files on disk */
		pidgin_themes_remove_theme_dir(theme_dir);

		/* Find theme in themes list and remove it */
		for (iter = smiley_themes ; iter ; iter = iter->next) {
			theme = ((struct smiley_theme *)(iter->data));
			if (!strcmp(theme->path, file))
				break ;
		}
		if (iter) {
			if (theme == current_smiley_theme) {
				new_theme = ((struct smiley_theme *)(NULL == iter->next ? (smiley_themes == iter ? NULL : smiley_themes->data) : iter->next->data));
				if (new_theme)
					purple_prefs_set_string(PIDGIN_PREFS_ROOT "/smileys/theme", new_theme->name);
				else
					current_smiley_theme = NULL;
			}
			smiley_themes = g_slist_delete_link(smiley_themes, iter);

			/* Destroy theme structure */
			pidgin_themes_destroy_smiley_theme(theme);
		}
	}

	g_free(theme_dir);
}

static void _pidgin_themes_smiley_themeize(GtkWidget *imhtml, gboolean custom)
{
	struct smiley_list *list;
	if (!current_smiley_theme)
		return;

	gtk_imhtml_remove_smileys(GTK_IMHTML(imhtml));
	list = current_smiley_theme->list;
	while (list) {
		char *sml = !strcmp(list->sml, "default") ? NULL : list->sml;
		GSList *icons = list->smileys;
		while (icons) {
			gtk_imhtml_associate_smiley(GTK_IMHTML(imhtml), sml, icons->data);
			icons = icons->next;
		}

		if (custom == TRUE) {
			icons = pidgin_smileys_get_all();

			while (icons) {
				gtk_imhtml_associate_smiley(GTK_IMHTML(imhtml), sml, icons->data);
				icons = icons->next;
			}
		}

		list = list->next;
	}
}

void pidgin_themes_smiley_themeize(GtkWidget *imhtml)
{
	_pidgin_themes_smiley_themeize(imhtml, FALSE);
}

void pidgin_themes_smiley_themeize_custom(GtkWidget *imhtml)
{
	_pidgin_themes_smiley_themeize(imhtml, TRUE);
}

static void
pidgin_themes_destroy_smiley_theme_smileys(struct smiley_theme *theme)
{
	GHashTable *already_freed;
	struct smiley_list *wer;

	already_freed = g_hash_table_new(g_direct_hash, g_direct_equal);
	for (wer = theme->list; wer != NULL; wer = theme->list) {
		while (wer->smileys) {
			GtkIMHtmlSmiley *uio = wer->smileys->data;
			
			if (uio->imhtml) {
				g_signal_handlers_disconnect_matched(uio->imhtml, G_SIGNAL_MATCH_DATA,
					0, 0, NULL, NULL, uio);
			}
				
			if (uio->icon)
				g_object_unref(uio->icon);
			if (g_hash_table_lookup(already_freed, uio->file) == NULL) {
				g_free(uio->file);
				g_hash_table_insert(already_freed, uio->file, GINT_TO_POINTER(1));
			}
			g_free(uio->smile);
			g_free(uio);
			wer->smileys = g_slist_remove(wer->smileys, uio);
		}
		theme->list = wer->next;
		g_free(wer->sml);
		g_free(wer);
	}
	theme->list = NULL;

	g_hash_table_destroy(already_freed);
}

static void
pidgin_smiley_themes_remove_non_existing(void)
{
	static struct smiley_theme *theme = NULL;
	GSList *iter = NULL;

	if (!smiley_themes) return ;

	for (iter = smiley_themes ; iter ; iter = iter->next) {
		theme = ((struct smiley_theme *)(iter->data));
		if (!g_file_test(theme->path, G_FILE_TEST_EXISTS)) {
			if (theme == current_smiley_theme)
				current_smiley_theme = ((struct smiley_theme *)(NULL == iter->next ? NULL : iter->next->data));
			pidgin_themes_destroy_smiley_theme(theme);
			iter->data = NULL;
		}
	}
	/* Remove all elements whose data is NULL */
	smiley_themes = g_slist_remove_all(smiley_themes, NULL);

	if (!current_smiley_theme && smiley_themes) {
		struct smiley_theme *smile = g_slist_last(smiley_themes)->data;
		if (pidgin_themes_is_xml(smile->path)) {
			pidgin_themes_load_smiley_theme_xml(smile->path, TRUE);
		} else {
			pidgin_themes_load_smiley_theme(smile->path, TRUE);
		}
	}
}

static struct smiley_theme *
pidgin_themes_lookup_smiley_theme(const char *file)
{
	GSList *lst = smiley_themes;
	
	while (lst) {
		struct smiley_theme *thm = lst->data;
		if (!strcmp(thm->path, file)) {
			return thm;
		}
		lst = lst->next;
	}
	
	return NULL;
}

static void
pidgin_themes_reload_smiley_theme(struct smiley_theme *theme)
{
	/* reload smiley theme in GtkIMHtmls */
	GList *cnv;

	if (current_smiley_theme)
		pidgin_themes_destroy_smiley_theme_smileys(current_smiley_theme);
	current_smiley_theme = theme;

	for (cnv = purple_get_conversations(); cnv != NULL; cnv = cnv->next) {
		PurpleConversation *conv = cnv->data;

		if (PIDGIN_IS_PIDGIN_CONVERSATION(conv)) {
			/* We want to see our custom smileys on our entry if we write the shortcut */
			pidgin_themes_smiley_themeize(PIDGIN_CONVERSATION(conv)->imhtml);
			pidgin_themes_smiley_themeize_custom(PIDGIN_CONVERSATION(conv)->entry);
		}
	}
}

gboolean pidgin_themes_is_xml(const char *file)
{
	if (strcmp(g_path_get_basename(file), "icondef.xml") == 0) {
		return TRUE;
	} else if (strcmp(g_path_get_basename(file), "theme") == 0) {
		return FALSE;
	} else {
		purple_debug_error("gtkthemes", 
			"pidgin_themes_is_xml called with invalid path\n");
		return FALSE;
	}
}

void pidgin_themes_load_smiley_theme_xml(const char *file, gboolean load)
{
	xmlnode *icondef = 
		purple_util_read_xml_from_file_absolute(file, _("Smiley Themes"));
	struct smiley_theme *theme = pidgin_themes_lookup_smiley_theme(file);
	const gchar *dirname = g_path_get_dirname(file);
	gboolean new_theme = FALSE;
	
	if (icondef) {
		xmlnode *meta = NULL;
		
		if (strcmp(icondef->name, "icondef") != 0) {
			purple_debug_error("gtkthemes", 
				"icondefs.xml must contain a <icondef/> top level element\n");
			return;
		}
		
		meta = xmlnode_get_child(icondef, "meta");
		
		if (!theme) {
			 new_theme = TRUE;
			 theme = g_new0(struct smiley_theme, 1);
			 theme->path = g_strdup(file);
		} else if (theme == current_smiley_theme) {
			/* don't reload the current theme */
			return;
		}
		
		if (!meta) {
			purple_debug_error("gtkthemes", 
				"icondef.xml contains no meta data\n");
			if (theme) {
				g_free(theme);
			}
			return;
		} else {
			/* parse out meta data */
			xmlnode *name = xmlnode_get_child(meta, "name");
			xmlnode *description = xmlnode_get_child(meta, "description");
			xmlnode *author = xmlnode_get_child(meta, "author");
			
			if (name) {
				theme->name = xmlnode_get_data(name);
			} else {
				/* maybe should do something better here... */
				theme->name = g_strdup(file);
			}
			
			if (description) {
				theme->desc = xmlnode_get_data(description);
			}
			
			if (author) {
				theme->author = xmlnode_get_data(author);
			}
			
			/* skip other meta elements for now (version, etc.) */
			/* should also set the icon for the theme, from somewhere */
		}
		
		if (load) {
			/* create the "Default" list of smileys */
			struct smiley_list *child = g_new0(struct smiley_list, 1);
			xmlnode *icon = NULL;
			
			child->sml = g_strdup("default");
			
			for (icon = xmlnode_get_child(icondef, "icon") ; icon ;
				 icon = xmlnode_get_next_twin(icon)) {
				xmlnode *object = NULL;
				xmlnode *text = NULL;
				gboolean found_object = FALSE;
				gchar *path = NULL;
				gchar *filename = NULL;
					 
				for (object = xmlnode_get_child(icon, "object") ; object ;
					object = xmlnode_get_next_twin(object)) {
					const gchar *mime = xmlnode_get_attrib(object, "mime");
						
					if (mime && strncmp(mime, "image/", 6) == 0) {
						found_object = TRUE;
						break;
					}
				}
					 
				if (!found_object) {
					purple_debug_error("gtkthemes", 
						"icon in theme without an image object\n");
					continue;
				}
				
				filename = xmlnode_get_data_unescaped(object);
				path = g_build_filename(dirname, filename, NULL);
					 
				for (text = xmlnode_get_child(icon, "text") ; text ;
					text = xmlnode_get_next_twin(text)) {
					gchar *smile = xmlnode_get_data_unescaped(text);
					GtkIMHtmlSmiley *smiley =
						gtk_imhtml_smiley_create(path, smile, FALSE, 0);
						
					purple_debug_info("gtkthemes",
						"adding smiley with shortcut %s\n", smile);
						
					child->smileys = 
						g_slist_append(child->smileys, (gpointer) smiley);
					g_free(smile);
				}
					 
				g_free(filename);
				g_free(path);
			}
			
			theme->list = child;
		}
		
		if (new_theme) {
			smiley_themes = g_slist_append(smiley_themes, theme);
		}
		
		if (load) {
			pidgin_themes_reload_smiley_theme(theme);
		}
		
	} else {
		purple_debug_error("gtkthemes", "parse error in icondefs.xml");
	}
}

void pidgin_themes_load_smiley_theme(const char *file, gboolean load)
{
	FILE *f = g_fopen(file, "r");
	char buf[256];
	char *i;
	struct smiley_list *list = NULL;
	char *dirname;
	gboolean new_theme = FALSE;
	struct smiley_theme *theme = pidgin_themes_lookup_smiley_theme(file);
	
	purple_debug_info("gtkthemes", "lookup theme %p\n", theme);
	
	if (!f)
		return;
	
	if (!theme) {
		purple_debug_info("gtkthemes", "creating new theme structure\n");
		new_theme = TRUE;
		theme = g_new0(struct smiley_theme, 1);
		theme->path = g_strdup(file);
	} else if (theme == current_smiley_theme) {
		/* Don't reload the theme if it is already loaded */
		fclose(f);
		return;
	}

	dirname = g_path_get_dirname(file);

	while (!feof(f)) {
		if (!fgets(buf, sizeof(buf), f)) {
			break;
		}

		if (buf[0] == '#' || buf[0] == '\0')
			continue;

		i = buf;
		while (isspace(*i))
			i++;

		if (*i == '[' && strchr(i, ']') && load) {
			struct smiley_list *child = g_new0(struct smiley_list, 1);
			child->sml = g_strndup(i+1, strchr(i, ']') - i - 1);
			if (theme->list)
				list->next = child;
			else
				theme->list = child;
			/* Reverse the Smiley list since it was built in reverse order for efficiency reasons */
			if (list != NULL)
				list->smileys = g_slist_reverse(list->smileys);
			list = child;
		} else if (!g_ascii_strncasecmp(i, "Name=", strlen("Name="))) {
			int len;
			g_free(theme->name);
			theme->name = g_strdup(i + strlen("Name="));
			len = strlen(theme->name);
			theme->name[len-1] = 0;
			if(len > 2 && theme->name[len-2] == '\r')
				theme->name[len-2] = 0;
		} else if (!g_ascii_strncasecmp(i, "Description=", strlen("Description="))) {
			g_free(theme->desc);
			theme->desc = g_strdup(i + strlen("Description="));
			theme->desc[strlen(theme->desc)-1] = 0;
		} else if (!g_ascii_strncasecmp(i, "Icon=", strlen("Icon="))) {
			g_free(theme->icon);
			theme->icon = g_build_filename(dirname, i + strlen("Icon="), NULL);
			theme->icon[strlen(theme->icon)-1] = 0;
		} else if (!g_ascii_strncasecmp(i, "Author=", strlen("Author="))) {
			g_free(theme->author);
			theme->author = g_strdup(i + strlen("Author="));
			theme->author[strlen(theme->author)-1] = 0;
		} else if (load && list) {
			gboolean hidden = FALSE;
			char *sfile = NULL;

			if (*i == '!' && *(i + 1) == ' ') {
				hidden = TRUE;
				i = i + 2;
			}
			while  (*i) {
				char l[64];
				int li = 0;
				while (!isspace(*i) && li < sizeof(l) - 1) {
					if (*i == '\\' && *(i+1) != '\0' && *(i+1) != '\n' && *(i+1) != '\r')
						i++;
					l[li++] = *(i++);
				}
				l[li] = 0;
				if (!sfile) {
					sfile = g_build_filename(dirname, l, NULL);
				} else {
					GtkIMHtmlSmiley *smiley = gtk_imhtml_smiley_create(sfile, l, hidden, 0);
					list->smileys = g_slist_prepend(list->smileys, smiley);
				}
				while (isspace(*i))
					i++;

			}


			g_free(sfile);
		}
	}

	/* Reverse the Smiley list since it was built in reverse order for efficiency reasons */
	if (list != NULL)
		list->smileys = g_slist_reverse(list->smileys);

	g_free(dirname);
	fclose(f);

	if (!theme->name || !theme->desc || !theme->author) {
		purple_debug_error("gtkthemes", "Invalid file format, not loading smiley theme from '%s'\n", file);

		pidgin_themes_destroy_smiley_theme(theme);
		return;
	}

	if (new_theme) {
		purple_debug_info("gtkthemes", "adding theme %s to list of themes\n",
			theme->name);
		smiley_themes = g_slist_prepend(smiley_themes, theme);
	}

	if (load) {
		pidgin_themes_reload_smiley_theme(theme);
#if 0
		GList *cnv;

		if (current_smiley_theme)
			pidgin_themes_destroy_smiley_theme_smileys(current_smiley_theme);
		current_smiley_theme = theme;

		for (cnv = purple_get_conversations(); cnv != NULL; cnv = cnv->next) {
			PurpleConversation *conv = cnv->data;

			if (PIDGIN_IS_PIDGIN_CONVERSATION(conv)) {
				/* We want to see our custom smileys on our entry if we write the shortcut */
				pidgin_themes_smiley_themeize(PIDGIN_CONVERSATION(conv)->imhtml);
				pidgin_themes_smiley_themeize_custom(PIDGIN_CONVERSATION(conv)->entry);
			}
		}
#endif
	}
}

void pidgin_themes_smiley_theme_probe()
{
	GDir *dir;
	const gchar *file;
	gchar *path, *old_path, *test_path;
	int l;
	char* probedirs[3];

	pidgin_smiley_themes_remove_non_existing();

	probedirs[0] = g_build_filename(DATADIR, "pixmaps", "pidgin", "emotes", NULL);
	probedirs[1] = g_build_filename(purple_user_dir(), "smileys", NULL);
	probedirs[2] = 0;
	for (l=0; probedirs[l]; l++) {
		dir = g_dir_open(probedirs[l], 0, NULL);
		if (dir) {
			while ((file = g_dir_read_name(dir))) {
				test_path = g_build_filename(probedirs[l], file, NULL);
				if (g_file_test(test_path, G_FILE_TEST_IS_DIR)) {
					old_path = g_build_filename(probedirs[l], file, "theme", 
						NULL);
					path = g_build_filename(probedirs[l], file, "icondef.xml",
						NULL);
					
					/* Here we check to see that the theme has proper syntax.
					 * We set the second argument to FALSE so that it doesn't load
					 * the theme yet.
					 */
					purple_debug_info("gtkthemes",
						"trying to load %s or %s\n", path, old_path);
					if (g_file_test(path, G_FILE_TEST_IS_REGULAR)) {
						/* load as XML format theme XEP-0038 */
						purple_debug_info("gtkthemes",
							"loading theme using XML format from %s\n",
							path);
						pidgin_themes_load_smiley_theme_xml(path, FALSE);
					} else if (g_file_test(old_path, G_FILE_TEST_IS_REGULAR)) {
						purple_debug_info("gtkthemes",
							"loading theme using old format from %s\n",
							old_path);
						pidgin_themes_load_smiley_theme(old_path, FALSE);
					}
					g_free(path);
					g_free(old_path);
				}
				g_free(test_path);
			}
			g_dir_close(dir);
		} else if (l == 1) {
			g_mkdir(probedirs[l], S_IRUSR | S_IWUSR | S_IXUSR);
		}
		g_free(probedirs[l]);
	}

	if (!current_smiley_theme && smiley_themes) {
		struct smiley_theme *smile = smiley_themes->data;
		if (pidgin_themes_is_xml(smile->path)) {
			pidgin_themes_load_smiley_theme_xml(smile->path, TRUE);
		} else {
			pidgin_themes_load_smiley_theme(smile->path, TRUE);
		}
	}
}

GSList *pidgin_themes_get_proto_smileys(const char *id) {
	PurplePlugin *proto;
	struct smiley_list *list, *def;

	if ((current_smiley_theme == NULL) || (current_smiley_theme->list == NULL))
		return NULL;

	def = list = current_smiley_theme->list;

	if (id == NULL)
		return def->smileys;

	proto = purple_find_prpl(id);

	while (list) {
		if (!strcmp(list->sml, "default"))
			def = list;
		else if (proto && !strcmp(proto->info->name, list->sml))
			break;

		list = list->next;
	}

	return list ? list->smileys : def->smileys;
}

void pidgin_themes_init()
{
	GSList *l;
	const char *current_theme =
		purple_prefs_get_string(PIDGIN_PREFS_ROOT "/smileys/theme");

	pidgin_themes_smiley_theme_probe();

	for (l = smiley_themes; l; l = l->next) {
		struct smiley_theme *smile = l->data;
		if (smile->name && strcmp(current_theme, smile->name) == 0) {
			pidgin_themes_load_smiley_theme(smile->path, TRUE);
			break;
		}
	}

	/* If we still don't have a smiley theme, choose the first one */
	if (!current_smiley_theme && smiley_themes) {
		struct smiley_theme *smile = smiley_themes->data;
		pidgin_themes_load_smiley_theme(smile->path, TRUE);
	}
}
