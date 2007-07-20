/**
* @file gntlog.c GNT Log API
* @ingroup finch
*
* finch
*
* Finch is the legal property of its developers, whose names are too numerous
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

#include "finch.h"

static GHashTable *log_viewers = NULL;

#include "gntlog.h"
typedef struct {
	PurpleLogType type;
	char *screenname;
	PurpleAccount *account;
	PurpleContact *contact;
} LogViewerHashT;

typedef struct {
	FinchLogViewer *log_viewer;

	PurpleLogVoidCallback done_cb;
	int counter;

	gboolean need_continue;
	gulong destroy_handler_id;
} FinchLogData
;
static guint log_viewer_hash(gconstpointer data)
{
	const LogViewerHashT *viewer = data;

	if (viewer->contact != NULL)
		return g_direct_hash(viewer->contact);

	return g_str_hash(viewer->screenname) +
		g_str_hash(purple_account_get_username(viewer->account));
}

static gboolean log_viewer_equal(gconstpointer y, gconstpointer z)
{
	const LogViewerHashT *a, *b;
	int ret;
	char *normal;

	a = y;
	b = z;

	if (a->contact != NULL) {
		if (b->contact != NULL)
			return (a->contact == b->contact);
		else
			return FALSE;
	} else {
		if (b->contact != NULL)
			return FALSE;
	}

	normal = g_strdup(purple_normalize(a->account, a->screenname));
	ret = (a->account == b->account) &&
		!strcmp(normal, purple_normalize(b->account, b->screenname));
	g_free(normal);

	return ret;
}

static void
finch_log_window_destroy_cb(GntWidget *w, gpointer data)
{
	FinchLogData *callback_data = data;

	/* mark that log window has destroyed*/
	callback_data->need_continue = FALSE;

}

static void set_log_viewer_log_size(FinchLogViewer *log_viewer, int log_size) 
{
	char *sz_txt = purple_str_size_to_units(log_size);
	/* const char *field_name = _("Total log size:"); Useful later, throws a warning for now.*/

	g_free(sz_txt);
}

static void
finch_log_size_cb(int size, void *data)
{
	FinchLogData * finch_log_data = data;

	if (finch_log_data->need_continue == TRUE)
		set_log_viewer_log_size(finch_log_data->log_viewer, size);

	finch_log_data->done_cb(finch_log_data);
}

static void
populate_log_tree(FinchLogViewer *lw)
{

}

static void
select_first_log(FinchLogViewer *lv)
{

}

static void append_log_viewer_logs(FinchLogViewer *log_viewer, GList *logs) 
{
	// TODO: Instead of doing this, we should find a way to avoid
	// TODO: rebuilding the entire UI tree and just insert the new logs.
	log_viewer->logs = g_list_concat(logs, log_viewer->logs);
	log_viewer->logs = g_list_sort(log_viewer->logs, purple_log_compare);
	populate_log_tree(log_viewer);
	select_first_log(log_viewer);
}

static void
finch_log_list_cb(GList * list, void * data)
{
	FinchLogData *finch_log_data = data;

	if (list != NULL) {
		if (finch_log_data->need_continue == TRUE) {
			append_log_viewer_logs(finch_log_data->log_viewer, list);
		} } else {
		finch_log_data->done_cb(finch_log_data);
	}

}

static void
finch_log_done_cb(void *data)
{
	FinchLogData *finch_log_data = data;
	FinchLogViewer *log_viewer = finch_log_data->log_viewer;

	finch_log_data->counter--;

	if (!finch_log_data->counter) {

		if (finch_log_data->need_continue == TRUE) {
			g_signal_handler_disconnect(log_viewer->window, finch_log_data->destroy_handler_id);

			// TODO: We should only select the first log
			// TODO: if one is not already selected.
			select_first_log(log_viewer);
		} 

		g_free(finch_log_data);
	}
}

static FinchLogViewer *
display_log_viewer(LogViewerHashT *ht, const gchar * title, gboolean need_log_size)
{
	return NULL;
}

void
finch_log_show(PurpleLogType type, gchar *screenname, PurpleAccount *account)
{
	LogViewerHashT *ht;
	FinchLogViewer *lv = NULL;
	gchar * name = screenname;
	char *title;
	FinchLogData * finch_log_data;
	
	g_return_if_fail(account != NULL);
	g_return_if_fail(screenname != NULL);

	ht = g_new0(LogViewerHashT, 1);

	ht->type = type;
	ht->screenname = g_strdup(screenname);
	ht->account = account;

	if (log_viewers == NULL) {
		log_viewers = g_hash_table_new(log_viewer_hash, log_viewer_equal);
	} else if ((lv = g_hash_table_lookup(log_viewers, ht))) {
		gnt_window_present(lv->window);
		g_free(ht->screenname);
		g_free(ht);
		return;
	}

	if (type == PURPLE_LOG_CHAT) {
		PurpleChat *chat;

		chat = purple_blist_find_chat(account, screenname);
		if (chat != NULL)
			name = (gchar *)purple_chat_get_name(chat);

		title = g_strdup_printf(_("Conversations in %s"), name);
	} else {
		PurpleBuddy *buddy;

		buddy = purple_find_buddy(account, screenname);
		if (buddy != NULL)
			name = (gchar *)purple_buddy_get_contact_alias(buddy);

		title = g_strdup_printf(_("Conversations with %s"), name);
	}

	finch_log_data = g_new0(FinchLogData, 1);
	finch_log_data->done_cb = finch_log_done_cb;

	/* we should set count of nonblocking  calls
	    when counter will be zero
	    we free all data and make neccessary operations
	    we have 2 nonblocking calls: 
	    purple_log_get_logs_nonblocking andpurple_log_get_total_size_nonblocking  */

	finch_log_data->counter = 2;
	finch_log_data->log_viewer = display_log_viewer(ht, title, TRUE);
	g_free(title);
	finch_log_data->destroy_handler_id =
					g_signal_connect( G_OBJECT(finch_log_data->log_viewer->window), 
													  "destroy",
														G_CALLBACK(finch_log_window_destroy_cb), 
														finch_log_data);

	finch_log_data->need_continue = TRUE;

	purple_log_get_logs_nonblocking(type, screenname, account, finch_log_list_cb, finch_log_data);
	purple_log_get_total_size_nonblocking(type, screenname, account, finch_log_size_cb, finch_log_data);

}

void
finch_log_show_contact(PurpleContact *contact)
{
}

void
finch_syslog_show()
{
}

void
finch_log_init()
{
	
}

void *
finch_log_get_handle()
{
		static int handle;
		return &handle;
}

void 
finch_log_uninit()
{
}
