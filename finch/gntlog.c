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

/* Finch */
#include "finch.h"
#include "gntlog.h"

/* libpurple */
#include "debug.h"

/* libgnt */
#include "gntwindow.h"
#include "gnttree.h"
#include "gntentry.h"
#include "gntbox.h"
#include "gntcombobox.h"
#include "gnttextview.h"
#include "gntbutton.h"

typedef struct {
	FinchLogViewer *log_viewer;

	PurpleLogVoidCallback done_cb;
	int counter;

} FinchLogData;

GList *log_viewers = NULL;

static FinchLogViewer * create_log_viewer(PurpleAccount *account, const char * screenname, const gchar * title, gboolean need_log_size);

static void set_log_viewer_log_size(FinchLogViewer *log_viewer, int log_size) 
{
	char *size = log_size ? purple_str_size_to_units(log_size) : NULL;

	GntTextView *tv = GNT_TEXT_VIEW(log_viewer->info);
	gnt_text_view_tag_change(tv, "log-size", size ? size : _("No logs available."), TRUE);
	g_free(size);
	gnt_widget_draw(log_viewer->window);
}

static void
finch_log_size_cb(int size, void *data)
{
	FinchLogData * finch_log_data = data;
	if (g_list_find(log_viewers, finch_log_data->log_viewer))
		set_log_viewer_log_size(finch_log_data->log_viewer, size);

	finch_log_data->done_cb(finch_log_data);
}

static const char *
log_get_date(PurpleLog *log)
{
	if (log->tm)
		return purple_date_format_full(log->tm);
	else
		return purple_date_format_full(localtime(&log->time));
}

static void
add_logs(FinchLogViewer *lv, GList *logs)
{
	const char *month;
	PurpleLog *log;
	char * current_month;
	GList *tmp;

	if(!logs)
		return;

	lv->logs = g_list_concat(logs, lv->logs);
	lv->logs = g_list_sort(lv->logs, purple_log_compare);

	while(logs != NULL) {
		log = logs->data;

		if(gnt_tree_get_row_text_list(GNT_TREE(lv->tree),log)){
			logs = logs->next;
			continue;
		}

		month = purple_utf8_strftime(_("%B %Y"),
															 log->tm ? log->tm : localtime(&log->time));
		tmp = g_list_find_custom(lv->months, month, (GCompareFunc)g_ascii_strcasecmp);
		if(!tmp) {
			current_month = g_strdup(month);

			/* Add it so we can clear it later */
			lv->months = g_list_append(lv->months, current_month);

			gnt_tree_add_row_last(GNT_TREE(lv->tree), 
											current_month, 
											gnt_tree_create_row(GNT_TREE(lv->tree), month), 
											NULL);			
		}
		else {
			current_month = tmp->data;
		}

		gnt_tree_add_row_last(GNT_TREE(lv->tree), log,
				gnt_tree_create_row(GNT_TREE(lv->tree),	log_get_date(log)),
				current_month);

		logs = logs->next;
	}
}

static void append_log_viewer_logs(FinchLogViewer *log_viewer, GList *logs) 
{
	add_logs(log_viewer, logs);
}

static void
free_months (FinchLogViewer *lv)
{
	if(lv->months){
		g_list_foreach(lv->months, (GFunc)g_free, NULL);
		g_list_free(lv->months);
		lv->months = NULL;
	}
}


static void
finch_log_list_cb(GList * list, void * data)
{
	FinchLogData *finch_log_data = data;
	
	if (list != NULL) {
		if (g_list_find(log_viewers, finch_log_data->log_viewer)){
			append_log_viewer_logs(finch_log_data->log_viewer, list);
		} 
	} else {
		gnt_text_view_clear(GNT_TEXT_VIEW(finch_log_data->log_viewer->tv));
		gnt_text_view_append_text_with_flags(GNT_TEXT_VIEW(finch_log_data->log_viewer->tv),
				_("Select a log and press enter."), GNT_TEXT_FLAG_BOLD);
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
		g_free(finch_log_data);
	}
	
	if (g_list_find(log_viewers, finch_log_data->log_viewer))
		gnt_widget_draw(log_viewer->window);
}

static void
show_log(char *text, PurpleLogReadFlags *flags, gpointer *data)
{
	FinchLogViewer *lv = (FinchLogViewer *)data;
	
	gnt_text_view_clear(GNT_TEXT_VIEW(lv->tv));

	if(*flags & PURPLE_LOG_READ_NO_NEWLINE) {
		gnt_text_view_append_html(GNT_TEXT_VIEW(lv->tv), text ? text : _("<html><body>Empty Log</body></html>"));
	}
	else {
		gnt_text_view_append_text_with_flags(GNT_TEXT_VIEW(lv->tv), text ? text : _("Empty Log"), GNT_TEXT_FLAG_NORMAL);
	}
	
	gnt_widget_draw(lv->tv);
}

static void
update_log_view(GntTree *tree, gpointer data)
{
	FinchLogViewer *lv = data;
	PurpleLog *log;
	gpointer key = gnt_tree_get_selection_data(GNT_TREE(lv->tree));

	if(!g_list_find(lv->months, key)) { /* Check to see if it's a month header */
		log = (PurpleLog *)key;
		lv->flags = 0;
		purple_log_read_nonblocking(log,&(lv->flags),(PurpleLogReadCallback)show_log, lv);
	}

}

static void
clean_up_screenname(FinchLogViewer *viewer)
{
	GSList *itr;
	GntEntry *entry = GNT_ENTRY(viewer->screenname);

	gnt_entry_set_text(entry, "");

	for(itr = viewer->suggest;itr;itr = itr->next){
		PurpleBuddy *buddy = itr->data;
		gnt_entry_remove_suggest(entry, buddy->name);
	}

	g_slist_free(viewer->suggest);
	viewer->suggest = NULL;
	
}

static void
setup_suggest(GntEntry *entry, FinchLogViewer *viewer, PurpleAccount *account)
{
	GSList *itrs;
	viewer->suggest = purple_find_buddies(account, NULL);
	for(itrs = viewer->suggest;itrs;itrs = itrs->next) {
		PurpleBuddy *buddy = itrs->data;
		gnt_entry_add_suggest(GNT_ENTRY(viewer->screenname), buddy->name);
	}
}

static char *
get_title(PurpleLogType type, PurpleAccount *account, const gchar *screenname)
{
	char *title;
	const char *name = screenname;

	if (type == PURPLE_LOG_CHAT) {
		PurpleChat *chat;
	
		chat = purple_blist_find_chat(account, screenname);
		if (chat != NULL)
			name = (gchar *)purple_chat_get_name(chat);
	
		title = g_strdup_printf(_("Conversations in %s"), name);
	} 
	else {
		PurpleBuddy *buddy;
	
		buddy = purple_find_buddy(account, screenname);
		if (buddy != NULL)
			name = (gchar *)purple_buddy_get_contact_alias(buddy);
		title = g_strdup_printf(_("Conversations with %s"), name);
	}

	return title;

}

static void
cleanup_viewer(FinchLogViewer *viewer)
{
	free_months(viewer);
}

static void
clear_viewer(FinchLogViewer *viewer)
{
	gnt_box_set_title(GNT_BOX(viewer->window),"");
	
	gnt_text_view_tag_change(GNT_TEXT_VIEW(viewer->info), "who", " ", TRUE);
	gnt_text_view_tag_change(GNT_TEXT_VIEW(viewer->info), "log-size", " ", TRUE);

	gnt_tree_remove_all(GNT_TREE(viewer->tree));
	gnt_text_view_clear(GNT_TEXT_VIEW(viewer->tv));
	gnt_entry_clear(GNT_ENTRY(viewer->entry));

	cleanup_viewer(viewer);

	gnt_widget_draw(viewer->window);

}

static void
setup_viewer(FinchLogViewer *viewer, PurpleAccount *account, const gchar *screenname, const char *title)
{
	gnt_box_set_title(GNT_BOX(viewer->window), title);
	gnt_text_view_tag_change(GNT_TEXT_VIEW(viewer->info), "who",  screenname, TRUE);
}

static void
load_logs(PurpleLogType type, PurpleAccount *account, const gchar *screenname, FinchLogViewer *viewer)
{
	FinchLogData * finch_log_data;
	char *title = get_title(type, account, screenname);

	setup_viewer(viewer, account, screenname, title);

	finch_log_data = g_new0(FinchLogData, 1);
	finch_log_data->done_cb = finch_log_done_cb;

	/* we should set count of nonblocking  calls
			when counter will be zero
			we free all data and make neccessary operations
			we have 2 nonblocking calls: 
			purple_log_get_logs_nonblocking andpurple_log_get_total_size_nonblocking  */

	
	finch_log_data->log_viewer = viewer;

	finch_log_data->counter = 2;

	purple_log_get_logs_nonblocking(type, screenname, account, finch_log_list_cb, finch_log_data);
	purple_log_get_total_size_nonblocking(type, screenname, account, finch_log_size_cb, finch_log_data);

}


static void
load_user(GntEntry *entry, FinchLogViewer *viewer)
{
	const char * screenname = gnt_entry_get_text(entry);
	PurpleAccount *account = gnt_combo_box_get_selected_data(GNT_COMBO_BOX(viewer->account));
	PurpleBuddy *buddy = purple_find_buddy(account, screenname);
	PurpleBlistNode *node = (PurpleBlistNode *)buddy;
	PurpleLogType type = PURPLE_LOG_IM;


	if(PURPLE_BLIST_NODE_IS_BUDDY(node)) {
		type = PURPLE_LOG_IM;
	}
	else if(PURPLE_BLIST_NODE_IS_CHAT(node)) {
		type = PURPLE_LOG_CHAT;
	}
	clear_viewer(viewer);
	load_logs(type, account, screenname, viewer);
	
}

static gboolean
screenname_key_pressed(GntEntry *entry, const char * key, FinchLogViewer *viewer)
{
	if(key[0] == '\r' && key[1] == 0) {
		load_user(entry, viewer);
		return TRUE;
	}
	return FALSE;
}

static void
change_account(GntComboBox *combox, gpointer oldkey, gpointer newkey, FinchLogViewer *viewer)
{
	PurpleAccount *account = newkey;

	clean_up_screenname(viewer);
	clear_viewer(viewer);

	gnt_box_give_focus_to_child(GNT_BOX(viewer->window), viewer->screenname);
	setup_suggest(GNT_ENTRY(viewer->screenname), viewer, account);
	
}

static void
destroy_viewer(GntWindow *win, FinchLogViewer *viewer)
{
	cleanup_viewer(viewer);
	
	log_viewers = g_list_remove(log_viewers, viewer);
}

static struct {
	int occurences;
	int current_position;
	int in_progress;
} search_data = {0,0,0};

static void
do_search(FinchLogViewer *viewer)
{

	if(search_data.in_progress){
		search_data.current_position++;
		if(search_data.current_position == search_data.occurences)
			search_data.current_position = 0;
	}
	else {
		search_data.occurences = gnt_text_view_search(GNT_TEXT_VIEW(viewer->tv), gnt_entry_get_text(GNT_ENTRY(viewer->entry)));
		search_data.current_position = 0;
		if(!search_data.occurences)
			return;
		search_data.in_progress = 1;
	}
	gnt_text_view_search_jump(GNT_TEXT_VIEW(viewer->tv), search_data.current_position);
}

static FinchLogViewer *
create_log_viewer(PurpleAccount *account, const char *screenname, const gchar * title, gboolean need_log_size)
{
	GntWidget *win;
	GntWidget *tv;
	GntWidget *tree;
	GntWidget *entry;
	GntWidget *button;
	GntWidget *combox;
	GntWidget *selectbox;
	GntWidget *splitbox;
	GntWidget *rightbox;
	GntWidget *searchbox;
	GntWidget *aligned_box;
	

	GList *itr;
	int h, w;

	FinchLogViewer *viewer;

	viewer = g_new0(FinchLogViewer, 1);
	viewer->suggest = NULL;

	viewer->window = win = gnt_window_box_new(FALSE, TRUE);

	gnt_box_set_title(GNT_BOX(win), title);
	gnt_box_set_pad(GNT_BOX(win), 0);
	gnt_box_set_toplevel(GNT_BOX(win), TRUE);
	gnt_box_set_fill(GNT_BOX(win), TRUE);
	gnt_box_set_alignment(GNT_BOX(win), GNT_ALIGN_MID);

	selectbox = gnt_box_new(TRUE, FALSE);	
	viewer->screenname = entry = gnt_entry_new("");
	gnt_entry_set_text(GNT_ENTRY(viewer->screenname), screenname);

	viewer->account = combox = gnt_combo_box_new();
	gnt_widget_get_size(combox, &w, &h);
	gnt_widget_set_size(combox, 35, h);
	
	for(itr = purple_accounts_get_all();itr;itr = itr->next) {
		PurpleAccount * account = itr->data;
		
		gnt_combo_box_add_data(GNT_COMBO_BOX(viewer->account), account, account->username);
			
	}
	gnt_combo_box_set_selected(GNT_COMBO_BOX(viewer->account), account);
	setup_suggest(GNT_ENTRY(entry), viewer, account);

	g_signal_connect_swapped(G_OBJECT(entry), "destroy", G_CALLBACK(clean_up_screenname), viewer);
	g_signal_connect(G_OBJECT(entry), "key_pressed", G_CALLBACK(screenname_key_pressed), viewer);
	g_signal_connect(G_OBJECT(combox), "selection-changed", G_CALLBACK(change_account), viewer);

	gnt_box_add_widget(GNT_BOX(selectbox), entry);
	gnt_box_add_widget(GNT_BOX(selectbox), combox);
	gnt_box_readjust(GNT_BOX(selectbox));
	gnt_box_add_widget(GNT_BOX(win), selectbox);
	

	viewer->info = tv = gnt_text_view_new();
	gnt_widget_get_size(tv, &w, &h);
	gnt_widget_set_size(tv, w, 3);
	gnt_text_view_append_text_with_flags(GNT_TEXT_VIEW(tv), _("Conversation with "), GNT_TEXT_FLAG_NORMAL);
	gnt_text_view_append_text_with_tag(GNT_TEXT_VIEW(tv), screenname, GNT_TEXT_FLAG_BOLD, "who");
	if(need_log_size) {
		gnt_text_view_append_text_with_flags(GNT_TEXT_VIEW(tv), _("\nTotal Log Size: "), GNT_TEXT_FLAG_NORMAL);
		gnt_text_view_append_text_with_tag(GNT_TEXT_VIEW(tv), _("(Computing...)"), GNT_TEXT_FLAG_BOLD, "log-size");
	}
	gnt_text_view_scroll(GNT_TEXT_VIEW(tv), 0);
	gnt_text_view_set_flag(GNT_TEXT_VIEW(tv), GNT_TEXT_VIEW_NO_SCROLL);  /* Let's not show the scrollbars here */
	GNT_WIDGET_UNSET_FLAGS(tv, GNT_WIDGET_GROW_Y);
	gnt_box_add_widget(GNT_BOX(win), tv);

	splitbox = gnt_hbox_new(FALSE);
	gnt_box_set_pad(GNT_BOX(splitbox), 1);
	gnt_box_set_alignment(GNT_BOX(splitbox), GNT_ALIGN_TOP);

	viewer->tree = tree = gnt_tree_new();
	gnt_widget_get_size(tree, &w, &h);
	gnt_widget_set_size(tree, 30, h);
	gnt_box_add_widget(GNT_BOX(splitbox), tree);
	g_signal_connect(G_OBJECT(tree),"activate",G_CALLBACK(update_log_view),viewer);

	rightbox = gnt_vbox_new(FALSE);
	gnt_widget_get_size(tv, &w, &h);
	gnt_widget_set_size(tv, 45, h);

	viewer->tv = tv = gnt_text_view_new();
	gnt_text_view_set_flag(GNT_TEXT_VIEW(tv), GNT_TEXT_VIEW_TOP_ALIGN);
	gnt_text_view_append_text_with_flags(GNT_TEXT_VIEW(tv), _("Fetching logs ..."), GNT_TEXT_FLAG_BOLD);
	gnt_box_add_widget(GNT_BOX(rightbox), tv);
	gnt_text_view_attach_pager_widget(GNT_TEXT_VIEW(tv), win);

	searchbox = gnt_hbox_new(FALSE);
	
	viewer->entry = entry = gnt_entry_new("");
	gnt_box_add_widget(GNT_BOX(searchbox), entry);
	gnt_text_view_attach_scroll_widget(GNT_TEXT_VIEW(tv), entry);

	button = gnt_button_new(_("Search"));
	gnt_box_add_widget(GNT_BOX(searchbox), button);
	g_signal_connect_swapped(G_OBJECT(button), "activate", G_CALLBACK(do_search), viewer);
	gnt_text_view_attach_scroll_widget(GNT_TEXT_VIEW(tv), button);

	gnt_box_add_widget(GNT_BOX(rightbox), searchbox);
	gnt_box_add_widget(GNT_BOX(splitbox), rightbox);
	gnt_box_add_widget(GNT_BOX(win), splitbox);

	aligned_box = gnt_hbox_new(FALSE);
	gnt_box_set_alignment(GNT_BOX(aligned_box), GNT_ALIGN_RIGHT);
	button = gnt_button_new(_("Close"));
	gnt_box_add_widget(GNT_BOX(aligned_box), button);
	gnt_box_add_widget(GNT_BOX(win), aligned_box);

	g_signal_connect(G_OBJECT(win),"destroy",G_CALLBACK(destroy_viewer),viewer);

	gnt_widget_show(win);

	return viewer;
}

void
finch_log_show(PurpleLogType type, gchar *screenname, PurpleAccount *account)
{
	const gchar *title = get_title(type, account, screenname);
	FinchLogViewer *viewer;

	g_return_if_fail(account != NULL);
	g_return_if_fail(screenname != NULL);

	viewer = create_log_viewer(account, screenname, title, TRUE);

	log_viewers = g_list_append(log_viewers, viewer);

	load_logs(type, account, screenname, viewer);
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
