/*
 * gaim
 *
 * Copyright (C) 2003, Nathan Walp <faceprint@faceprint.com>
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <glib.h>
#include "internal.h"
#include "prefs.h"
#include "debug.h"
#include "util.h"

#ifdef _WIN32
#include "win32dep.h"
#endif

struct pref_cb {
	GaimPrefCallback func;
	gpointer data;
	guint id;
};

struct gaim_pref {
	GaimPrefType type;
	char *name;
	union {
		gpointer generic;
		gboolean boolean;
		int integer;
		char *string;
		GList *stringlist;
	} value;
	GSList *callbacks;
	struct gaim_pref *parent;
	struct gaim_pref *sibling;
	struct gaim_pref *first_child;
};

static GHashTable *prefs_hash = NULL;

static struct gaim_pref prefs = { GAIM_PREF_NONE, NULL, {NULL}, NULL,
	NULL, NULL, NULL };

static guint prefs_save_timer = 0;
static gboolean prefs_is_loaded = FALSE;


static gboolean prefs_save_callback(gpointer who_cares) {
	gaim_prefs_sync();
	prefs_save_timer = 0;
	return FALSE;
}

static void schedule_prefs_save() {
	if(!prefs_save_timer)
		prefs_save_timer = g_timeout_add(5000, prefs_save_callback, NULL);
}

static void prefs_save_cb(const char *name, GaimPrefType type, gpointer val,
		gpointer user_data) {

	if(!prefs_is_loaded)
		return;

	gaim_debug(GAIM_DEBUG_MISC, "prefs", "%s changed, scheduling save.\n", name);

	schedule_prefs_save();
}

void gaim_prefs_init() {
	prefs_hash = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

	gaim_prefs_connect_callback("/", prefs_save_cb, NULL);

	gaim_prefs_add_none("/core");
	gaim_prefs_add_none("/plugins");
	gaim_prefs_add_none("/plugins/core");
	gaim_prefs_add_none("/plugins/lopl");
	gaim_prefs_add_none("/plugins/prpl");

	/* Away */
	gaim_prefs_add_none("/core/away");
	gaim_prefs_add_bool("/core/away/away_when_idle", TRUE);
	gaim_prefs_add_int("/core/away/mins_before_away", 1);
	/* XXX: internationalized string in prefs...evil */
	gaim_prefs_add_string("/core/away/default_message",
			_("Slightly less boring default"));

	/* Away -> Auto Response */
	gaim_prefs_add_none("/core/away/auto_response");
	gaim_prefs_add_bool("/core/away/auto_response/enabled", TRUE);
	gaim_prefs_add_bool("/core/away/auto_response/in_active_conv", TRUE);
	gaim_prefs_add_bool("/core/away/auto_response/idle_only", FALSE);
	gaim_prefs_add_int("/core/away/auto_response/sec_before_resend", 60);

	/* Buddies */
	gaim_prefs_add_none("/core/buddies");
	gaim_prefs_add_bool("/core/buddies/use_server_alias", TRUE);

	/* Contact Priority Settings */
	gaim_prefs_add_none("/core/contact");
	gaim_prefs_add_bool("/core/contact/last_match", FALSE);
	gaim_prefs_add_int("/core/contact/offline_score", 4);
	gaim_prefs_add_int("/core/contact/away_score", 2);
	gaim_prefs_add_int("/core/contact/idle_score", 1);
}

static char *
get_path_dirname(const char *name)
{
	char *c, *str;

	str = g_strdup(name);

	if ((c = strrchr(str, '/')) != NULL) {
		*c = '\0';

		if (*str == '\0') {
			g_free(str);

			str = g_strdup("/");
		}
	}
	else {
		g_free(str);

		str = g_strdup(".");
	}

	return str;
}

static char *
get_path_basename(const char *name)
{
	const char *c;

	if ((c = strrchr(name, '/')) != NULL)
		return g_strdup(c + 1);

	return g_strdup(name);
}

static char *pref_full_name(struct gaim_pref *pref) {
	GString *name;
	struct gaim_pref *parent;
	char *ret;

	if(!pref)
		return NULL;

	if(pref == &prefs)
		return g_strdup("/");

	name = g_string_new(pref->name);
	parent = pref->parent;

	for(parent = pref->parent; parent && parent->name; parent = parent->parent) {
		name = g_string_prepend_c(name, '/');
		name = g_string_prepend(name, parent->name);
	}
	ret = name->str;
	g_string_free(name, FALSE);
	return ret;
}

static struct gaim_pref *find_pref(const char *name)
{
	if(!name || name[0] != '/') {
		return NULL;
	} else if(name[1] == '\0') {
		return &prefs;
	} else {
		return g_hash_table_lookup(prefs_hash, name);
	}
}

static struct gaim_pref *find_pref_parent(const char *name)
{
	char *parent_name = get_path_dirname(name);
	struct gaim_pref *ret = &prefs;

	if(strcmp(parent_name, "/")) {
		ret = find_pref(parent_name);
	}

	g_free(parent_name);
	return ret;
}

static void free_pref_value(struct gaim_pref *pref) {
	switch(pref->type) {
		case GAIM_PREF_BOOLEAN:
			pref->value.boolean = FALSE;
			break;
		case GAIM_PREF_INT:
			pref->value.integer = 0;
			break;
		case GAIM_PREF_STRING:
			g_free(pref->value.string);
			pref->value.string = NULL;
			break;
		case GAIM_PREF_STRING_LIST:
			{
				GList *tmp;
				for(tmp = pref->value.stringlist; tmp; tmp = tmp->next)
					g_free(tmp->data);

				g_list_free(pref->value.stringlist);
			} break;
		case GAIM_PREF_NONE:
			break;
	}
}

static struct gaim_pref *add_pref(GaimPrefType type, const char *name) {
	struct gaim_pref *parent;
	struct gaim_pref *me;
	struct gaim_pref *sibling;
	char *my_name;

	parent = find_pref_parent(name);

	if(!parent)
		return NULL;

	my_name = get_path_basename(name);

	for(sibling = parent->first_child; sibling; sibling = sibling->sibling) {
		if(!strcmp(sibling->name, my_name)) {
			g_free(my_name);
			return NULL;
		}
	}

	me = g_new0(struct gaim_pref, 1);
	me->type = type;
	me->name = my_name;

	me->parent = parent;
	if(parent->first_child) {
		/* blatant abuse of a for loop */
		for(sibling = parent->first_child; sibling->sibling;
				sibling = sibling->sibling);
		sibling->sibling = me;
	} else {
		parent->first_child = me;
	}

	g_hash_table_insert(prefs_hash, g_strdup(name), (gpointer)me);

	return me;
}

void gaim_prefs_add_none(const char *name) {
	add_pref(GAIM_PREF_NONE, name);
}

void gaim_prefs_add_bool(const char *name, gboolean value) {
	struct gaim_pref *pref = add_pref(GAIM_PREF_BOOLEAN, name);

	if(!pref)
		return;

	pref->value.boolean = value;
}

void gaim_prefs_add_int(const char *name, int value) {
	struct gaim_pref *pref = add_pref(GAIM_PREF_INT, name);

	if(!pref)
		return;

	pref->value.integer = value;
}

void gaim_prefs_add_string(const char *name, const char *value) {
	struct gaim_pref *pref = add_pref(GAIM_PREF_STRING, name);

	if(!pref)
		return;

	pref->value.string = g_strdup(value);
}

void gaim_prefs_add_string_list(const char *name, GList *value) {
	struct gaim_pref *pref = add_pref(GAIM_PREF_STRING_LIST, name);
	GList *tmp;

	if(!pref)
		return;

	for(tmp = value; tmp; tmp = tmp->next)
		pref->value.stringlist = g_list_append(pref->value.stringlist,
				g_strdup(tmp->data));
}

void remove_pref(struct gaim_pref *pref) {
	char *name;

	if(!pref || pref == &prefs)
		return;

	while(pref->first_child)
		remove_pref(pref->first_child);

	if(pref->parent->first_child == pref) {
		pref->parent->first_child = pref->sibling;
	} else {
		struct gaim_pref *sib = pref->parent->first_child;
		while(sib->sibling != pref)
			sib = sib->sibling;
		sib->sibling = pref->sibling;
	}

	name = pref_full_name(pref);

	gaim_debug(GAIM_DEBUG_INFO, "prefs", "removing pref %s\n", name);

	g_hash_table_remove(prefs_hash, name);
	g_free(name);

	free_pref_value(pref);

	g_slist_free(pref->callbacks);
	g_free(pref->name);
	g_free(pref);
}

void gaim_prefs_remove(const char *name) {
	struct gaim_pref *pref = find_pref(name);

	if(!pref)
		return;

	remove_pref(pref);
}

void gaim_prefs_destroy() {
	gaim_prefs_remove("/");
}

static void do_callbacks(const char* name, struct gaim_pref *pref) {
	GSList *cbs;
	struct gaim_pref *cb_pref;
	for(cb_pref = pref; cb_pref; cb_pref = cb_pref->parent) {
		for(cbs = cb_pref->callbacks; cbs; cbs = cbs->next) {
			struct pref_cb *cb = cbs->data;
			cb->func(name, pref->type, pref->value.generic, cb->data);
		}
	}
}

void gaim_prefs_trigger_callback(const char *name) {
	struct gaim_pref *pref = find_pref(name);

	if(!pref) {
		gaim_debug(GAIM_DEBUG_ERROR, "prefs",
				"gaim_prefs_trigger_callback: Unknown pref %s\n", name);
		return;
	}

	do_callbacks(name, pref);
}

void gaim_prefs_set_generic(const char *name, gpointer value) {
	struct gaim_pref *pref = find_pref(name);

	if(!pref) {
		gaim_debug(GAIM_DEBUG_ERROR, "prefs",
				"gaim_prefs_set_generic: Unknown pref %s\n", name);
		return;
	}

	pref->value.generic = value;
	do_callbacks(name, pref);
}

void gaim_prefs_set_bool(const char *name, gboolean value) {
	struct gaim_pref *pref = find_pref(name);

	if(pref) {
		if(pref->type != GAIM_PREF_BOOLEAN) {
			gaim_debug(GAIM_DEBUG_ERROR, "prefs",
					"gaim_prefs_set_bool: %s not a boolean pref\n", name);
			return;
		}

		if(pref->value.boolean != value) {
			pref->value.boolean = value;
			do_callbacks(name, pref);
		}
	} else {
		gaim_prefs_add_bool(name, value);
	}
}

void gaim_prefs_set_int(const char *name, int value) {
	struct gaim_pref *pref = find_pref(name);

	if(pref) {
		if(pref->type != GAIM_PREF_INT) {
			gaim_debug(GAIM_DEBUG_ERROR, "prefs",
					"gaim_prefs_set_int: %s not an integer pref\n", name);
			return;
		}

		if(pref->value.integer != value) {
			pref->value.integer = value;
			do_callbacks(name, pref);
		}
	} else {
		gaim_prefs_add_int(name, value);
	}
}

void gaim_prefs_set_string(const char *name, const char *value) {
	struct gaim_pref *pref = find_pref(name);

	if(pref) {
		if(pref->type != GAIM_PREF_STRING) {
			gaim_debug(GAIM_DEBUG_ERROR, "prefs",
					"gaim_prefs_set_string: %s not a string pref\n", name);
			return;
		}

		if((value && !pref->value.string) ||
				(!value && pref->value.string) ||
				strcmp(pref->value.string, value)) {
			g_free(pref->value.string);
			pref->value.string = g_strdup(value);
			do_callbacks(name, pref);
		}
	} else {
		gaim_prefs_add_string(name, value);
	}
}

void gaim_prefs_set_string_list(const char *name, GList *value) {
	struct gaim_pref *pref = find_pref(name);
	if(pref) {
		GList *tmp;

		if(pref->type != GAIM_PREF_STRING_LIST) {
			gaim_debug(GAIM_DEBUG_ERROR, "prefs",
					"gaim_prefs_set_string_list: %s not a string list pref\n",
					name);
			return;
		}

		for(tmp = pref->value.stringlist; tmp; tmp = tmp->next)
			g_free(tmp->data);

		g_list_free(pref->value.stringlist);
		pref->value.stringlist = NULL;

		for(tmp = value; tmp; tmp = tmp->next)
			pref->value.stringlist = g_list_append(pref->value.stringlist,
					g_strdup(tmp->data));

		do_callbacks(name, pref);

	} else {
		gaim_prefs_add_string_list(name, value);
	}
}

gpointer gaim_prefs_get_generic(const char *name) {
	struct gaim_pref *pref = find_pref(name);

	if(!pref) {
		gaim_debug(GAIM_DEBUG_ERROR, "prefs",
				"gaim_prefs_get_generic: Unknown pref %s\n", name);
		return NULL;
	}

	return pref->value.generic;
}

GaimPrefType gaim_prefs_get_type(const char *name) {
	struct gaim_pref *pref = find_pref(name);

	if (pref == NULL)
		return GAIM_PREF_NONE;

	return (pref->type);
}

gboolean gaim_prefs_get_bool(const char *name) {
	struct gaim_pref *pref = find_pref(name);

	if(!pref) {
		gaim_debug(GAIM_DEBUG_ERROR, "prefs",
				"gaim_prefs_get_bool: Unknown pref %s\n", name);
		return FALSE;
	} else if(pref->type != GAIM_PREF_BOOLEAN) {
		gaim_debug(GAIM_DEBUG_ERROR, "prefs",
				"gaim_prefs_get_bool: %s not a boolean pref\n", name);
		return FALSE;
	}

	return pref->value.boolean;
}

int gaim_prefs_get_int(const char *name) {
	struct gaim_pref *pref = find_pref(name);

	if(!pref) {
		gaim_debug(GAIM_DEBUG_ERROR, "prefs",
				"gaim_prefs_get_int: Unknown pref %s\n", name);
		return 0;
	} else if(pref->type != GAIM_PREF_INT) {
		gaim_debug(GAIM_DEBUG_ERROR, "prefs",
				"gaim_prefs_get_int: %s not an integer pref\n", name);
		return 0;
	}

	return pref->value.integer;
}

const char *gaim_prefs_get_string(const char *name) {
	struct gaim_pref *pref = find_pref(name);

	if(!pref) {
		gaim_debug(GAIM_DEBUG_ERROR, "prefs",
				"gaim_prefs_get_string: Unknown pref %s\n", name);
		return NULL;
	} else if(pref->type != GAIM_PREF_STRING) {
		gaim_debug(GAIM_DEBUG_ERROR, "prefs",
				"gaim_prefs_get_string: %s not a string pref\n", name);
		return NULL;
	}

	return pref->value.string;
}

GList *gaim_prefs_get_string_list(const char *name) {
	struct gaim_pref *pref = find_pref(name);
	GList *ret = NULL, *tmp;

	if(!pref) {
		gaim_debug(GAIM_DEBUG_ERROR, "prefs",
				"gaim_prefs_get_string_list: Unknown pref %s\n", name);
		return NULL;
	} else if(pref->type != GAIM_PREF_STRING_LIST) {
		gaim_debug(GAIM_DEBUG_ERROR, "prefs",
				"gaim_prefs_get_string_list: %s not a string list pref\n", name);
		return NULL;
	}

	for(tmp = pref->value.stringlist; tmp; tmp = tmp->next)
		ret = g_list_append(ret, g_strdup(tmp->data));

	return ret;
}

void gaim_prefs_rename(const char *oldname, const char *newname) {
	struct gaim_pref *oldpref, *newpref;

	oldpref = find_pref(oldname);
	newpref = find_pref(newname);

	/* it's already been renamed, call off the dogs */
	if(!oldpref)
		return;

	g_return_if_fail(newpref != NULL); /* the new one needs to be created */
	g_return_if_fail(oldpref->type == newpref->type);
	g_return_if_fail(oldpref->first_child == NULL); /* can't rename parents */

	switch(oldpref->type) {
		case GAIM_PREF_NONE:
			break;
		case GAIM_PREF_BOOLEAN:
			gaim_prefs_set_bool(newname, oldpref->value.boolean);
			break;
		case GAIM_PREF_INT:
			gaim_prefs_set_int(newname, oldpref->value.integer);
			break;
		case GAIM_PREF_STRING:
			gaim_prefs_set_string(newname, oldpref->value.string);
			break;
		case GAIM_PREF_STRING_LIST:
			gaim_prefs_set_string_list(newname, oldpref->value.stringlist);
			break;
	}

	remove_pref(oldpref);
}

void gaim_prefs_rename_old() {
	/* This doesn't actually do anything yet, but it will */
}

guint gaim_prefs_connect_callback(const char *name, GaimPrefCallback func, gpointer data)
{
	struct gaim_pref *pref = find_pref(name);
	struct pref_cb *cb;
	static guint cb_id = 0;

	if(!pref)
		return 0;

	cb = g_new0(struct pref_cb, 1);

	cb->func = func;
	cb->data = data;
	cb->id = ++cb_id;

	pref->callbacks = g_slist_append(pref->callbacks, cb);

	return cb->id;
}

gboolean disco_callback_helper(struct gaim_pref *pref, guint callback_id) {
	GSList *cbs;
	struct gaim_pref *child;

	if(!pref)
		return FALSE;

	for(cbs = pref->callbacks; cbs; cbs = cbs->next) {
		struct pref_cb *cb = cbs->data;
		if(cb->id == callback_id) {
			pref->callbacks = g_slist_remove(pref->callbacks, cb);
			g_free(cb);
			return TRUE;
		}
	}

	for(child = pref->first_child; child; child = child->sibling) {
		if(disco_callback_helper(child, callback_id))
			return TRUE;
	}

	return FALSE;
}

void gaim_prefs_disconnect_callback(guint callback_id) {
	disco_callback_helper(&prefs, callback_id);
}

static void gaim_prefs_write(FILE *f, struct gaim_pref *pref, int depth) {
	struct gaim_pref *tmp;
	char *esc;
	int i;

	if(!pref) {
		pref = &prefs;

		fprintf(f, "<?xml version='1.0' encoding='UTF-8' ?>\n");
		fprintf(f, "<pref name='/'");
	} else {
		for(i=0; i<depth; i++)
			fprintf(f, "\t");
		esc = g_markup_escape_text(pref->name, -1);
		fprintf(f, "<pref name='%s'", esc);
		g_free(esc);
	}

	switch(pref->type) {
		case GAIM_PREF_NONE:
			break;
		case GAIM_PREF_BOOLEAN:
			fprintf(f, " type='bool' value='%d'", pref->value.boolean);
			break;
		case GAIM_PREF_INT:
			fprintf(f, " type='int' value='%d'", pref->value.integer);
			break;
		case GAIM_PREF_STRING:
			esc = g_markup_escape_text(pref->value.string, -1);
			fprintf(f, " type='string' value='%s'", esc);
			g_free(esc);
			break;
		case GAIM_PREF_STRING_LIST:
			fprintf(f, " type='stringlist'");
			break;
	}

	if(pref->first_child || pref->type == GAIM_PREF_STRING_LIST) {
		fprintf(f, ">\n");

		for(tmp = pref->first_child; tmp; tmp = tmp->sibling)
			gaim_prefs_write(f, tmp, depth+1);

		if(pref->type == GAIM_PREF_STRING_LIST) {
			GList *tmp2;
			for(tmp2 = pref->value.stringlist; tmp2; tmp2 = tmp2->next) {
				for(i=0; i<depth+1; i++)
					fprintf(f, "\t");
				esc = g_markup_escape_text(tmp2->data, -1);
				fprintf(f, "<item value='%s' />\n", esc);
				g_free(esc);
			}
		}

		for(i=0; i<depth; i++)
			fprintf(f, "\t");
		fprintf(f, "</pref>\n");
	} else {
		fprintf(f, " />\n");
	}
}

void gaim_prefs_sync() {
	FILE *file;
	const char *user_dir = gaim_user_dir();
	char *filename;
	char *filename_real;

	if(!prefs_is_loaded) {
		gaim_debug(GAIM_DEBUG_WARNING, "prefs", "prefs saved before loading!  scheduling save.\n");
		schedule_prefs_save(); /* schedule a save for after we read in */
		return;
	}

	if(!user_dir)
		return;

	gaim_debug(GAIM_DEBUG_INFO, "prefs", "writing prefs out to disk.\n");

	file = fopen(user_dir, "r");
	if(!file)
		mkdir(user_dir, S_IRUSR | S_IWUSR | S_IXUSR);
	else
		fclose(file);

	filename = g_build_filename(user_dir, "prefs.xml.save", NULL);

	if((file = fopen(filename, "w"))) {
		gaim_prefs_write(file, NULL, 0);
		fclose(file);
		chmod(filename, S_IRUSR | S_IWUSR);
	} else {
		gaim_debug(GAIM_DEBUG_ERROR, "prefs", "Unable to write %s\n",
				filename);
	}

	filename_real = g_build_filename(user_dir, "prefs.xml", NULL);
	if(rename(filename, filename_real) < 0)
		gaim_debug(GAIM_DEBUG_ERROR, "prefs", "Error renaming %s to %s\n",
				filename, filename_real);

	g_free(filename);
	g_free(filename_real);
}

static GList *prefs_stack = NULL;

static void prefs_start_element_handler (GMarkupParseContext *context,
		const gchar *element_name,
		const gchar **attribute_names,
		const gchar **attribute_values,
		gpointer user_data,
		GError **error) {
	GaimPrefType pref_type = GAIM_PREF_NONE;
	int i;
	const char *pref_name = NULL, *pref_value = NULL;
	GString *pref_name_full;
	GList *tmp;

	if(strcmp(element_name, "pref") && strcmp(element_name, "item"))
		return;

	for(i = 0; attribute_names[i]; i++) {
		if(!strcmp(attribute_names[i], "name")) {
			pref_name = attribute_values[i];
		} else if(!strcmp(attribute_names[i], "type")) {
			if(!strcmp(attribute_values[i], "bool"))
				pref_type = GAIM_PREF_BOOLEAN;
			else if(!strcmp(attribute_values[i], "int"))
				pref_type = GAIM_PREF_INT;
			else if(!strcmp(attribute_values[i], "string"))
				pref_type = GAIM_PREF_STRING;
			else if(!strcmp(attribute_values[i], "stringlist"))
				pref_type = GAIM_PREF_STRING_LIST;
			else
				return;
		} else if(!strcmp(attribute_names[i], "value")) {
			pref_value = attribute_values[i];
		}
	}

	if(!strcmp(element_name, "item")) {
		struct gaim_pref *pref;

		pref_name_full = g_string_new("");

		for(tmp = prefs_stack; tmp; tmp = tmp->next) {
			pref_name_full = g_string_prepend(pref_name_full, tmp->data);
			pref_name_full = g_string_prepend_c(pref_name_full, '/');
		}

		pref = find_pref(pref_name_full->str);

		if(pref) {
			pref->value.stringlist = g_list_append(pref->value.stringlist,
					g_strdup(pref_value));
		}
	} else {
		if(!pref_name || !strcmp(pref_name, "/"))
			return;

		pref_name_full = g_string_new(pref_name);

		for(tmp = prefs_stack; tmp; tmp = tmp->next) {
			pref_name_full = g_string_prepend_c(pref_name_full, '/');
			pref_name_full = g_string_prepend(pref_name_full, tmp->data);
		}

		pref_name_full = g_string_prepend_c(pref_name_full, '/');

		switch(pref_type) {
			case GAIM_PREF_NONE:
				break;
			case GAIM_PREF_BOOLEAN:
				gaim_prefs_set_bool(pref_name_full->str, atoi(pref_value));
				break;
			case GAIM_PREF_INT:
				gaim_prefs_set_int(pref_name_full->str, atoi(pref_value));
				break;
			case GAIM_PREF_STRING:
				gaim_prefs_set_string(pref_name_full->str, pref_value);
				break;
			case GAIM_PREF_STRING_LIST:
				gaim_prefs_set_string_list(pref_name_full->str, NULL);
				break;
		}
		prefs_stack = g_list_prepend(prefs_stack, g_strdup(pref_name));
		g_string_free(pref_name_full, TRUE);
	}
}

static void prefs_end_element_handler(GMarkupParseContext *context,
		const gchar *element_name, gpointer user_data, GError **error) {
	if(prefs_stack && !strcmp(element_name, "pref")) {
		g_free(prefs_stack->data);
		prefs_stack = g_list_delete_link(prefs_stack, prefs_stack);
	}
}

static GMarkupParser prefs_parser = {
	prefs_start_element_handler,
	prefs_end_element_handler,
	NULL,
	NULL,
	NULL
};

gboolean gaim_prefs_load() {
	gchar *filename = g_build_filename(gaim_user_dir(), "prefs.xml", NULL);
	gchar *contents = NULL;
	gsize length;
	GMarkupParseContext *context;
	GError *error = NULL;


	if(!filename) {
		prefs_is_loaded = TRUE;
		return FALSE;
	}

	gaim_debug(GAIM_DEBUG_INFO, "prefs", "Reading %s\n", filename);

	if(!g_file_get_contents(filename, &contents, &length, &error)) {
		gaim_debug(GAIM_DEBUG_ERROR, "prefs", "Error reading prefs: %s\n",
				error->message);
		g_error_free(error);
		g_free(filename);
		prefs_is_loaded = TRUE;

		return FALSE;
	}

	context = g_markup_parse_context_new(&prefs_parser, 0, NULL, NULL);

	if(!g_markup_parse_context_parse(context, contents, length, NULL)) {
		g_markup_parse_context_free(context);
		g_free(contents);
		g_free(filename);
		prefs_is_loaded = TRUE;

		return FALSE;
	}

	if(!g_markup_parse_context_end_parse(context, NULL)) {
		gaim_debug(GAIM_DEBUG_ERROR, "prefs", "Error parsing %s\n", filename);
		g_markup_parse_context_free(context);
		g_free(contents);
		g_free(filename);
		prefs_is_loaded = TRUE;

		return FALSE;
	}

	gaim_debug(GAIM_DEBUG_INFO, "prefs", "Finished reading %s\n", filename);
	g_markup_parse_context_free(context);
	g_free(contents);
	g_free(filename);
	prefs_is_loaded = TRUE;

	return TRUE;
}

