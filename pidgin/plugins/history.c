/* Puts last 4k of log in new conversations a la Everybuddy (and then
 * stolen by Trillian "Pro") */

#include "internal.h"
#include "pidgin.h"

#include "conversation.h"
#include "debug.h"
#include "log.h"
#include "notify.h"
#include "prefs.h"
#include "signals.h"
#include "util.h"
#include "version.h"

#include "gtkconv.h"
#include "gtkimhtml.h"
#include "gtkplugin.h"

#define HISTORY_PLUGIN_ID "gtk-history"

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

static gboolean _scroll_imhtml_to_end(gpointer data)
{
	GtkIMHtml *imhtml = data;
	gtk_imhtml_scroll_to_end(GTK_IMHTML(imhtml), FALSE);
	g_object_unref(G_OBJECT(imhtml));
	return FALSE;
}

static void historize_log_read_cb(char *text, PurpleLog *log, PurpleLogReadFlags *flags, PurpleLogContext *context)
{
	struct _historize_callback_data *callback_data;
	GtkIMHtmlOptions options = GTK_IMHTML_NO_COLOURS;
	PidginConversation *gtkconv;
	const char *header_date;
	char *escaped_alias;
	char *header;
	char *protocol;
	char *text_backup = NULL;
	char *imhtml_text;

	g_return_if_fail(context != NULL);
	callback_data = purple_log_context_get_userdata(context);

	gtkconv = PIDGIN_CONVERSATION(callback_data->conv);

	imhtml_text = gtk_imhtml_get_markup((GtkIMHtml *)gtkconv->imhtml);

	if (imhtml_text != NULL && *imhtml_text) {
		text_backup = strdup(imhtml_text);
		gtk_imhtml_clear((GtkIMHtml *)gtkconv->imhtml);
	}

	if (*flags & PURPLE_LOG_READ_NO_NEWLINE)
		options |= GTK_IMHTML_NO_NEWLINE;

	protocol = g_strdup(gtk_imhtml_get_protocol_name(GTK_IMHTML(gtkconv->imhtml)));
	gtk_imhtml_set_protocol_name(GTK_IMHTML(gtkconv->imhtml),
								purple_account_get_protocol_name(callback_data->log->account));

	if (gtk_text_buffer_get_char_count(gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtkconv->imhtml))))
		gtk_imhtml_append_text(GTK_IMHTML(gtkconv->imhtml), "<BR>", options);

	if (log->tm)
		header_date = purple_date_format_full(log->tm);
	else
		header_date = purple_date_format_full(localtime(&log->time));

	escaped_alias = g_markup_escape_text(callback_data->alias, -1);

	header = g_strdup_printf(_("<b>Conversation with %s on %s:</b><br>"), escaped_alias, header_date);

	gtk_imhtml_append_text(GTK_IMHTML(gtkconv->imhtml), header, options);
	g_free(header);
	g_free(escaped_alias);

	g_strchomp(text);
	gtk_imhtml_append_text(GTK_IMHTML(gtkconv->imhtml), text, options);
	g_free(text);

	gtk_imhtml_append_text(GTK_IMHTML(gtkconv->imhtml), "<hr>", options);

	gtk_imhtml_set_protocol_name(GTK_IMHTML(gtkconv->imhtml), protocol);
	g_free(protocol);

	if (text_backup)
		gtk_imhtml_append_text(GTK_IMHTML(gtkconv->imhtml), text_backup, GTK_IMHTML_NO_NEWLINE );
	g_free(text_backup);

	g_object_ref(G_OBJECT(gtkconv->imhtml));
	g_idle_add(_scroll_imhtml_to_end, gtkconv->imhtml);
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

static void historize_log_free_context_cb(void *data)
{
	struct _historize_callback_data *callback_data = data;

	purple_log_free_nonblocking(callback_data->log, NULL, NULL);
	g_free(callback_data);
}

static void historize_log_list_cb(GList *list, PurpleLogType type, const char *name, 
									PurpleAccount *account, PurpleLogContext *context)
{
	struct _historize_callback_data *callback_data;

	g_return_if_fail(context != NULL);

	callback_data = purple_log_context_get_userdata(context);

	if (list != NULL)
		callback_data->log = get_last_log(list, callback_data->log);
	else if (callback_data->log != NULL) {
		PurpleLogContext *read_context = purple_log_context_new(historize_log_free_context_cb);
		purple_log_context_set_userdata(read_context, callback_data);
		purple_log_read_nonblocking(callback_data->log, &callback_data->flags, 
								historize_log_read_cb, read_context);
		purple_log_context_close(read_context);
	}
}

static void historize_log_collector_cb(GList *list, PurpleLogType type, const char *name, 
									PurpleAccount *account, PurpleLogContext *context)
{
	struct _historize_callback_data *callback_data;

	g_return_if_fail(context != NULL);

	callback_data = purple_log_context_get_userdata(context);

	if (list != NULL)
		callback_data->log = get_last_log(list, callback_data->log);
}

static void historize_log_collector_finish_cb(void *data)
{
	struct _historize_callback_data *callback_data = data;

	PurpleLogContext *list_context = purple_log_context_new(NULL);
	purple_log_context_set_userdata(list_context, callback_data);
	if (callback_data->log == NULL)
		purple_log_get_logs_nonblocking(PURPLE_LOG_IM, callback_data->name, 
				callback_data->account, historize_log_list_cb, list_context);
	else
		historize_log_list_cb(NULL, PURPLE_LOG_IM, callback_data->name, 
				callback_data->account, list_context);
	purple_log_context_close(list_context);

}

static void historize(PurpleConversation *c)
{
	PurpleAccount *account = purple_conversation_get_account(c);
	const char *name = purple_conversation_get_name(c);
	PurpleConversationType convtype = purple_conversation_get_type(c);
	const char *alias = name;
	PidginConversation *gtkconv =  PIDGIN_CONVERSATION(c);
	gtkconv = PIDGIN_CONVERSATION(c);

	g_return_if_fail(gtkconv != NULL);

	if (convtype == PURPLE_CONV_TYPE_IM && g_list_length(gtkconv->convs) < 2)
	{
		GSList *buddies;
		GSList *cur;
		struct _historize_callback_data *callback_data = g_new0(struct _historize_callback_data, 1);
		PurpleLogContext *context = purple_log_context_new(historize_log_collector_finish_cb);

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

		purple_log_context_set_userdata(context, callback_data);

		for (cur = buddies; cur != NULL; cur = cur->next)
		{
			PurpleBlistNode *node = cur->data;
			if ((node != NULL) && ((node->prev != NULL) || (node->next != NULL)))
			{
				PurpleBlistNode *node2;
				alias = purple_buddy_get_contact_alias((PurpleBuddy *)node);

				/* We've found a buddy that matches this conversation.  It's part of a
				 * PurpleContact with more than one PurpleBuddy.  Loop through the PurpleBuddies
				 * in the contact and get all the logs. */
				for (node2 = node->parent->child ; node2 != NULL ; node2 = node2->next)
					purple_log_get_logs_nonblocking(PURPLE_LOG_IM,
							purple_buddy_get_name((PurpleBuddy *)node2),
							purple_buddy_get_account((PurpleBuddy *)node2), 
							historize_log_collector_cb, context);
			}
		}
		/* free buddies list */
		g_slist_free(buddies);
		purple_log_context_close(context);
	}
	else if (convtype == PURPLE_CONV_TYPE_CHAT){
		struct _historize_callback_data *callback_data;
		PurpleLogContext *context = purple_log_context_new(NULL);

		/* If we're not logging, don't show anything.
		 * Otherwise, we might show a very old log. */
		if (!purple_prefs_get_bool("/purple/logging/log_chats"))
			return;

		callback_data = g_new0(struct _historize_callback_data, 1);

		callback_data->conv = c;
		callback_data->alias = alias;
		purple_log_context_set_userdata(context, callback_data);
		purple_log_get_logs_nonblocking(PURPLE_LOG_CHAT, name, account, 
								historize_log_list_cb, context);
		purple_log_context_close(context);
	}
}

static void
history_prefs_check(PurplePlugin *plugin)
{
	if (!purple_prefs_get_bool("/purple/logging/log_ims") &&
	    !purple_prefs_get_bool("/purple/logging/log_chats"))
	{
		purple_notify_warning(plugin, NULL, _("History Plugin Requires Logging"),
							_("Logging can be enabled from Tools -> Preferences -> Logging.\n\n"
							  "Enabling logs for instant messages and/or chats will activate "
							  "history for the same conversation type(s)."));
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
	/* XXX: Do we want to listen to pidgin's "conversation-displayed" signal? */

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
	PIDGIN_PLUGIN_TYPE,
	0,
	NULL,
	PURPLE_PRIORITY_DEFAULT,
	HISTORY_PLUGIN_ID,
	N_("History"),
	DISPLAY_VERSION,
	N_("Shows recently logged conversations in new conversations."),
	N_("When a new conversation is opened this plugin will insert "
	   "the last conversation into the current conversation."),
	"Sean Egan <seanegan@gmail.com>",
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

PURPLE_INIT_PLUGIN(history, init_plugin, info)
