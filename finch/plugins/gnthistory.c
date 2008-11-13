/**
 * @file gnthistory.c Show log from previous conversation
 *
 * Copyright (C) 2006 Sadrul Habib Chowdhury <sadrul@users.sourceforge.net>
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
 */

/* Ripped from gtk/plugins/history.c */

#include "internal.h"

#include "conversation.h"
#include "debug.h"
#include "log.h"
#include "request.h"
#include "prefs.h"
#include "signals.h"
#include "util.h"
#include "version.h"

#include "gntconv.h"
#include "gntplugin.h"
#include "gntrequest.h"
#include "gnttextview.h"

#define HISTORY_PLUGIN_ID "gnt-history"

#define HISTORY_SIZE (4 * 1024)

struct _historize_callback_data
{
	int counter;

	PurpleLog *log;

	guint flags;

	PurpleConversation *conv;
	PurpleAccount *account;

	const char *name;
	const char *alias;
};

static void historize_log_read_cb(char *history, PurpleLogReadFlags *flags, void *data)
{
	struct _historize_callback_data *callback_data = data;
	PurpleMessageFlags mflag = PURPLE_MESSAGE_NO_LOG | PURPLE_MESSAGE_SYSTEM | PURPLE_MESSAGE_DELAYED;

	/* XXX: maybe it's a hack, but it works */
	FinchConv * finch_conv = callback_data->conv->ui_data;
	GntTextView *view = GNT_TEXT_VIEW(finch_conv->tv);
	GString *buffer = view->string;
	char *text_backup = NULL;
	char *header = NULL;

	if (*buffer->str) {
		text_backup = strdup(buffer->str);
		gnt_text_view_clear(view);
	}

	header = g_strdup_printf(_("<b>Conversation with %s on %s:</b><br>"), callback_data->alias,
					 purple_date_format_full(localtime(&callback_data->log->time)));

	purple_conversation_write(callback_data->conv, "", header, mflag, time(NULL));
	g_free(header);

	if (*flags & PURPLE_LOG_READ_NO_NEWLINE)
		purple_str_strip_char(history, '\n');
	purple_conversation_write(callback_data->conv, "", history, mflag, time(NULL));

	purple_conversation_write(callback_data->conv, "", "<hr>", mflag, time(NULL));
	g_free(history);

	purple_conversation_write(callback_data->conv, "", text_backup, PURPLE_MESSAGE_NO_LOG, 
		time(NULL));
	g_free(text_backup);

	purple_log_free_nonblocking(callback_data->log, NULL, NULL);
	g_free(callback_data);
}

static PurpleLog *get_last_log(GList *list, PurpleLog *last_log) 
{
	GList *node;

	if (last_log == NULL) {
		last_log = list->data;
		node = g_list_next(list);
	} else 
		node = list;

	for(; node; node = g_list_next(node)) 
		if (purple_log_compare(last_log, node->data) > 0) {
			purple_log_free_nonblocking(last_log, NULL, NULL);
			last_log = node->data;
		} else
			purple_log_free_nonblocking(node->data, NULL, NULL);

	g_list_free(list);
	return last_log;
}

static void historize_log_list_cb(GList *list, void *data)
{
	struct _historize_callback_data *callback_data = data;

	if (list != NULL)
		callback_data->log = get_last_log(list, callback_data->log);
	else if (callback_data->log != NULL)
				purple_log_read_nonblocking(callback_data->log, &callback_data->flags, 
										historize_log_read_cb, callback_data);
	else
		g_free(callback_data);
}

static void historize_log_collector_cb(GList *list, void *data)
{
	struct _historize_callback_data *callback_data = data;

	if (list != NULL)
		callback_data->log = get_last_log(list, callback_data->log);
	else {
		callback_data->counter--;

		if (!callback_data->counter) {
			if (callback_data->log == NULL) {
				purple_debug_info("historize_log_collector_cb", "making purple_log_get_logs_nonblocking call");
				purple_log_get_logs_nonblocking(PURPLE_LOG_IM, callback_data->name, 
						callback_data->account, historize_log_list_cb, callback_data);
			} else
				historize_log_list_cb(NULL, callback_data);
		}
	}
}

static void historize(PurpleConversation *c)
{
	PurpleAccount *account = purple_conversation_get_account(c);
	const char *name = purple_conversation_get_name(c);
	PurpleConversationType convtype = purple_conversation_get_type(c);
	const char *alias = name;

	if (convtype == PURPLE_CONV_TYPE_IM)
	{
		GSList *buddies;
		GSList *cur;
		struct _historize_callback_data *callback_data = g_new0(struct _historize_callback_data, 1);

		/* If we're not logging, don't show anything.
		 * Otherwise, we might show a very old log. */
		if (!purple_prefs_get_bool("/purple/logging/log_ims"))
			return;

		/* Find buddies for this conversation. */
		buddies = purple_find_buddies(account, name);

		/* If we found at least one buddy, save the first buddy's alias. */
		if (buddies != NULL)
			alias = purple_buddy_get_contact_alias((PurpleBuddy *)buddies->data);

		callback_data->log = NULL;
		callback_data->conv = c;
		callback_data->name = name;
		callback_data->account = account;
		callback_data->alias = alias;

		for (cur = buddies; cur != NULL; cur = cur->next)
		{
			PurpleBlistNode *node = cur->data;
			if ((node != NULL) &&
					((purple_blist_node_get_sibling_prev(node) != NULL) ||
						(purple_blist_node_get_sibling_next(node) != NULL))) {
				PurpleBlistNode *node2;

				alias = purple_buddy_get_contact_alias((PurpleBuddy *)node);

				for (node2 = node->parent->child ; node2 != NULL ; node2 = node2->next)
					callback_data->counter++;

				/* We've found a buddy that matches this conversation.  It's part of a
				 * PurpleContact with more than one PurpleBuddy.  Loop through the PurpleBuddies
				 * in the contact and get all the logs. */
				for (node2 = purple_blist_node_get_first_child(purple_blist_node_get_parent(node));
					 node2 != NULL ; node2 = purple_blist_node_get_sibling_next(node2)) {
					purple_log_get_logs_nonblocking(PURPLE_LOG_IM,
							purple_buddy_get_name((PurpleBuddy *)node2),
							purple_buddy_get_account((PurpleBuddy *)node2), 
							historize_log_collector_cb, callback_data);
				}
				g_slist_free(buddies);
				return;
			}
		}
		g_slist_free(buddies);
		purple_log_get_logs_nonblocking(PURPLE_LOG_IM, callback_data->name, 
			callback_data->account, historize_log_list_cb, callback_data);
	}
	else if (convtype == PURPLE_CONV_TYPE_CHAT)
	{
		struct _historize_callback_data *callback_data;

		/* If we're not logging, don't show anything.
		 * Otherwise, we might show a very old log. */
		if (!purple_prefs_get_bool("/purple/logging/log_chats"))
			return;

		callback_data = g_new0(struct _historize_callback_data, 1);

		callback_data->conv = c;
		callback_data->alias = alias;
		purple_log_get_logs_nonblocking(PURPLE_LOG_CHAT, name, account, 
								historize_log_list_cb, callback_data);
	}
}

static void
history_prefs_check(PurplePlugin *plugin)
{
	if (!purple_prefs_get_bool("/purple/logging/log_ims") &&
	    !purple_prefs_get_bool("/purple/logging/log_chats"))
	{
		PurpleRequestFields *fields = purple_request_fields_new();
		PurpleRequestFieldGroup *group;
		PurpleRequestField *field;
		struct {
			const char *pref;
			const char *label;
		} prefs[] = {
			{"/purple/logging/log_ims", N_("Log IMs")},
			{"/purple/logging/log_chats", N_("Log chats")},
			{NULL, NULL}
		};
		int iter;
		GList *list = purple_log_logger_get_options();
		const char *system = purple_prefs_get_string("/purple/logging/format");

		group = purple_request_field_group_new(_("Logging"));

		field = purple_request_field_list_new("/purple/logging/format", _("Log format"));
		while (list) {
			const char *label = _(list->data);
			list = g_list_delete_link(list, list);
			purple_request_field_list_add(field, label, list->data);
			if (system && strcmp(system, list->data) == 0)
				purple_request_field_list_add_selected(field, label);
			list = g_list_delete_link(list, list);
		}
		purple_request_field_group_add_field(group, field);

		for (iter = 0; prefs[iter].pref; iter++) {
			field = purple_request_field_bool_new(prefs[iter].pref, _(prefs[iter].label),
						purple_prefs_get_bool(prefs[iter].pref));
			purple_request_field_group_add_field(group, field);
		}

		purple_request_fields_add_group(fields, group);

		purple_request_fields(plugin, NULL, _("History Plugin Requires Logging"),
				      _("Logging can be enabled from Tools -> Preferences -> Logging.\n\n"
				      "Enabling logs for instant messages and/or chats will activate "
				      "history for the same conversation type(s)."),
				      fields,
				      _("OK"), G_CALLBACK(finch_request_save_in_prefs),
				      _("Cancel"), NULL,
				      NULL, NULL, NULL, plugin);
	}
}

static void history_prefs_cb(const char *name, PurplePrefType type,
							 gconstpointer val, gpointer data)
{
	history_prefs_check((PurplePlugin *)data);
}

static gboolean
plugin_load(PurplePlugin *plugin)
{
	purple_signal_connect(purple_conversations_get_handle(),
						"conversation-created",
						plugin, PURPLE_CALLBACK(historize), NULL);

	purple_prefs_connect_callback(plugin, "/purple/logging/log_ims",
								history_prefs_cb, plugin);
	purple_prefs_connect_callback(plugin, "/purple/logging/log_chats",
								history_prefs_cb, plugin);

	history_prefs_check(plugin);

	return TRUE;
}

static PurplePluginInfo info =
{
	PURPLE_PLUGIN_MAGIC,
	PURPLE_MAJOR_VERSION,
	PURPLE_MINOR_VERSION,
	PURPLE_PLUGIN_STANDARD,
	NULL,
	0,
	NULL,
	PURPLE_PRIORITY_DEFAULT,
	HISTORY_PLUGIN_ID,
	N_("GntHistory"),
	DISPLAY_VERSION,
	N_("Shows recently logged conversations in new conversations."),
	N_("When a new conversation is opened this plugin will insert "
	   "the last conversation into the current conversation."),
	"Sean Egan <seanegan@gmail.com>\n"
	"Sadrul H Chowdhury <sadrul@users.sourceforge.net>",
	PURPLE_WEBSITE,
	plugin_load,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,

	/* padding */
	NULL,
	NULL,
	NULL,
	NULL
};

static void
init_plugin(PurplePlugin *plugin)
{
}

PURPLE_INIT_PLUGIN(gnthistory, init_plugin, info)

