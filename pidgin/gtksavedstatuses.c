/**
 * @file gtksavedstatus.c GTK+ Saved Status Editor UI
 * @ingroup pidgin
 *
 * pidgin
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "account.h"
#include "internal.h"
#include "notify.h"
#include "request.h"
#include "savedstatuses.h"
#include "status.h"
#include "util.h"

#include "gtkblist.h"
#include "gtkexpander.h"
#include "pidgin.h"
#include "gtkimhtml.h"
#include "gtkimhtmltoolbar.h"
#include "gtksavedstatuses.h"
#include "pidginstock.h"
#include "gtkutils.h"

/*
 * TODO: Should attach to the account-deleted and account-added signals
 *       and update the GtkListStores in any StatusEditor windows that
 *       may be open.
 */

/**
 * These are used for the GtkTreeView when you're scrolling through
 * all your saved statuses.
 */
enum
{
	STATUS_WINDOW_COLUMN_TITLE,
	STATUS_WINDOW_COLUMN_TYPE,
	STATUS_WINDOW_COLUMN_MESSAGE,
	/** A hidden column containing a pointer to the editor for this saved status. */
	STATUS_WINDOW_COLUMN_WINDOW,
	STATUS_WINDOW_NUM_COLUMNS
};

/**
 * These is used for the GtkTreeView containing the list of accounts
 * at the bottom of the window when you're editing a particular
 * saved status.
 */
enum
{
	/** A hidden column containing a pointer to the PurpleAccount. */
	STATUS_EDITOR_COLUMN_ACCOUNT,
	/** A hidden column containing a pointer to the editor for this substatus. */
	STATUS_EDITOR_COLUMN_WINDOW,
	STATUS_EDITOR_COLUMN_ENABLE_SUBSTATUS,
	STATUS_EDITOR_COLUMN_ICON,
	STATUS_EDITOR_COLUMN_SCREENNAME,
	/** A hidden column containing the ID of this PurpleStatusType. */
	STATUS_EDITOR_COLUMN_STATUS_ID,
	STATUS_EDITOR_COLUMN_STATUS_NAME,
	STATUS_EDITOR_COLUMN_STATUS_MESSAGE,
	STATUS_EDITOR_NUM_COLUMNS
};

/**
 * These are used in the GtkComboBox to select the specific
 * PurpleStatusType when setting a substatus for a particular saved
 * status.
 */
enum
{
	SUBSTATUS_COLUMN_ICON,
	/** A hidden column containing the ID of this PurpleStatusType. */
	SUBSTATUS_COLUMN_STATUS_ID,
	SUBSTATUS_COLUMN_STATUS_NAME,
	SUBSTATUS_NUM_COLUMNS
};

typedef struct
{
	GtkWidget *window;
	GtkListStore *model;
	GtkWidget *treeview;
	GtkWidget *use_button;
	GtkWidget *modify_button;
	GtkWidget *delete_button;
} StatusWindow;

typedef struct
{
	GtkWidget *window;
	GtkListStore *model;
	GtkWidget *treeview;
	GtkButton *saveanduse_button;
	GtkButton *save_button;

	gchar *original_title;
	GtkEntry *title;
	GtkOptionMenu *type;
	GtkIMHtml *message;
} StatusEditor;

typedef struct
{
	StatusEditor *status_editor;
	PurpleAccount *account;

	GtkWidget *window;
	GtkListStore *model;
	GtkComboBox *box;
	GtkIMHtml *message;
	GtkIMHtmlToolbar *toolbar;
} SubStatusEditor;

static StatusWindow *status_window = NULL;


/**************************************************************************
* Status window
**************************************************************************/

static gboolean
status_window_find_savedstatus(GtkTreeIter *iter, const char *title)
{
	GtkTreeModel *model;
	char *cur;

	if ((status_window == NULL) || (title == NULL))
		return FALSE;

	model = GTK_TREE_MODEL(status_window->model);

	if (!gtk_tree_model_get_iter_first(model, iter))
		return FALSE;

	do {
		gtk_tree_model_get(model, iter, STATUS_WINDOW_COLUMN_TITLE, &cur, -1);
		if (!strcmp(title, cur))
		{
			g_free(cur);
			return TRUE;
		}
		g_free(cur);
	} while (gtk_tree_model_iter_next(model, iter));

	return FALSE;
}

static gboolean
status_window_destroy_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	StatusWindow *dialog = user_data;

	dialog->window = NULL;
	pidgin_status_window_hide();

	return FALSE;
}

#if !GTK_CHECK_VERSION(2,2,0)
static void
count_selected_helper(GtkTreeModel *model, GtkTreePath *path,
					GtkTreeIter *iter, gpointer user_data)
{
	(*(gint *)user_data)++;
}

static void
list_selected_helper(GtkTreeModel *model, GtkTreePath *path,
					GtkTreeIter *iter, gpointer user_data)
{
	GList **list = (GList **)user_data;
	*list = g_list_append(*list, gtk_tree_path_copy(path));
}
#endif

static void
status_window_use_cb(GtkButton *button, StatusWindow *dialog)
{
	GtkTreeSelection *selection;
	GtkTreeIter iter;
	GList *list = NULL;
	int num_selected = 0;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(dialog->treeview));

#if GTK_CHECK_VERSION(2,2,0)
	num_selected = gtk_tree_selection_count_selected_rows(selection);
#else
	gtk_tree_selection_selected_foreach(selection, count_selected_helper, &num_selected);
#endif
	if (num_selected != 1)
		/*
		 * This shouldn't happen because the "Use" button should have
		 * been grayed out.  Oh well.
		 */
		return;

#if GTK_CHECK_VERSION(2,2,0)
	list = gtk_tree_selection_get_selected_rows(selection, NULL);
#else
	gtk_tree_selection_selected_foreach(selection, list_selected_helper, &list);
#endif

	if (gtk_tree_model_get_iter(GTK_TREE_MODEL(dialog->model),
								&iter, list->data))
	{
		gchar *title;
		PurpleSavedStatus *saved_status;
		gtk_tree_model_get(GTK_TREE_MODEL(dialog->model), &iter,
						   STATUS_WINDOW_COLUMN_TITLE, &title,
						   -1);
		saved_status = purple_savedstatus_find(title);
		g_free(title);
		purple_savedstatus_activate(saved_status);
	}

	g_list_foreach(list, (GFunc)gtk_tree_path_free, NULL);
	g_list_free(list);
}

static void
status_window_add_cb(GtkButton *button, gpointer user_data)
{
	pidgin_status_editor_show(FALSE, NULL);
}

static void
status_window_modify_foreach(GtkTreeModel *model, GtkTreePath *path,
							 GtkTreeIter *iter, gpointer user_data)
{
	gchar *title;
	PurpleSavedStatus *saved_status;

	gtk_tree_model_get(model, iter, STATUS_WINDOW_COLUMN_TITLE, &title, -1);
	saved_status = purple_savedstatus_find(title);
	g_free(title);
	pidgin_status_editor_show(TRUE, saved_status);
}

static void
status_window_modify_cb(GtkButton *button, gpointer user_data)
{
	StatusWindow *dialog = user_data;
	GtkTreeSelection *selection;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(dialog->treeview));

	gtk_tree_selection_selected_foreach(selection, status_window_modify_foreach, user_data);
}

static void
status_window_delete_cancel_cb(gpointer data)
{
	GList *sel_titles = data;
	g_list_foreach(sel_titles, (GFunc) g_free, NULL);
	g_list_free(sel_titles);
}

static void
status_window_delete_confirm_cb(gpointer data)
{
	GtkTreeIter iter;
	GList *sel_titles = data, *l;
	char *title;

	for (l = sel_titles; l != NULL; l = l->next) {
		title = l->data;
		if (status_window_find_savedstatus(&iter, title))
			gtk_list_store_remove(status_window->model, &iter);
		purple_savedstatus_delete(title);
		g_free(title);
	}
	g_list_free(sel_titles);
}

static void
status_window_delete_cb(GtkButton *button, gpointer user_data)
{
	StatusWindow *dialog = user_data;
	GtkTreeIter iter;
	GtkTreeSelection *selection;
	GList *sel_paths, *l, *sel_titles = NULL;
	GtkTreeModel *model = GTK_TREE_MODEL(dialog->model);
	char *title;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(dialog->treeview));
#if GTK_CHECK_VERSION(2,2,0)
	sel_paths = gtk_tree_selection_get_selected_rows(selection, NULL);
#else
	gtk_tree_selection_selected_foreach(selection, list_selected_helper, &sel_paths);
#endif

	/* This is ugly because we're not allowed to modify the model from within
	 * gtk_tree_selection_selected_foreach() and the GtkTreePaths can become invalid
	 * when something is removed from the model.  The selection can also change while
	 * the request dialog is displayed, so we need to capture the selected rows at this time. */

	for (l = sel_paths; l != NULL; l = l->next) {
		if (gtk_tree_model_get_iter(model, &iter, l->data)) {
			gtk_tree_model_get(model, &iter, STATUS_WINDOW_COLUMN_TITLE, &title, -1);
			sel_titles = g_list_prepend(sel_titles, title);
		}
		gtk_tree_path_free(l->data);
	}
	g_list_free(sel_paths);

	if (g_list_length(sel_titles) == 1)
		title = g_strdup_printf(_("Are you sure you want to delete %s?"),
				(const gchar *)sel_titles->data);
	else
		title = g_strdup(_("Are you sure you want to delete the selected saved statuses?"));

	purple_request_action(dialog, NULL, title, NULL, 0,
		 NULL, NULL, NULL,
		 sel_titles, 2,
		_("Delete"), status_window_delete_confirm_cb,
		_("Cancel"), status_window_delete_cancel_cb);

	g_free(title);
}

static void
status_window_close_cb(GtkButton *button, gpointer user_data)
{
	pidgin_status_window_hide();
}

static void
status_selected_cb(GtkTreeSelection *sel, gpointer user_data)
{
	StatusWindow *dialog = user_data;
	int num_selected = 0;

#if GTK_CHECK_VERSION(2,2,0)
	num_selected = gtk_tree_selection_count_selected_rows(sel);
#else
	gtk_tree_selection_selected_foreach(sel, count_selected_helper, &num_selected);
#endif

	gtk_widget_set_sensitive(dialog->use_button, (num_selected == 1));
	gtk_widget_set_sensitive(dialog->modify_button, (num_selected > 0));
	gtk_widget_set_sensitive(dialog->delete_button, (num_selected > 0));
}

static void
add_status_to_saved_status_list(GtkListStore *model, PurpleSavedStatus *saved_status)
{
	GtkTreeIter iter;
	const char *title;
	const char *type;
	char *message;

	if (purple_savedstatus_is_transient(saved_status))
		return;

	title = purple_savedstatus_get_title(saved_status);
	type = purple_primitive_get_name_from_type(purple_savedstatus_get_type(saved_status));
	message = purple_markup_strip_html(purple_savedstatus_get_message(saved_status));

	gtk_list_store_append(model, &iter);
	gtk_list_store_set(model, &iter,
					   STATUS_WINDOW_COLUMN_TITLE, title,
					   STATUS_WINDOW_COLUMN_TYPE, type,
					   STATUS_WINDOW_COLUMN_MESSAGE, message,
					   -1);
	free(message);
}

static void
populate_saved_status_list(StatusWindow *dialog)
{
	const GList *saved_statuses;

	gtk_list_store_clear(dialog->model);

	for (saved_statuses = purple_savedstatuses_get_all(); saved_statuses != NULL;
			saved_statuses = g_list_next(saved_statuses))
	{
	  add_status_to_saved_status_list(dialog->model, saved_statuses->data);
	}
}

static gboolean
search_func(GtkTreeModel *model, gint column, const gchar *key, GtkTreeIter *iter, gpointer search_data)
{
	gboolean result;
	char *haystack;

	gtk_tree_model_get(model, iter, column, &haystack, -1);

	result = (purple_strcasestr(haystack, key) == NULL);

	g_free(haystack);

	return result;
}

static void
savedstatus_activated_cb(GtkTreeView *view, GtkTreePath *path, GtkTreeViewColumn *column, StatusWindow *dialog)
{
	status_window_modify_cb(NULL, dialog);
}

static GtkWidget *
create_saved_status_list(StatusWindow *dialog)
{
	GtkWidget *sw;
	GtkWidget *treeview;
	GtkTreeSelection *sel;
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;

	/* Create the scrolled window */
	sw = gtk_scrolled_window_new(0, 0);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
								   GTK_POLICY_AUTOMATIC,
								   GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw),
										GTK_SHADOW_IN);

	/* Create the list model */
	dialog->model = gtk_list_store_new(STATUS_WINDOW_NUM_COLUMNS,
									   G_TYPE_STRING,
									   G_TYPE_STRING,
									   G_TYPE_STRING,
									   G_TYPE_POINTER);

	/* Create the treeview */
	treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(dialog->model));
	dialog->treeview = treeview;
	gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(treeview), TRUE);
	g_signal_connect(G_OBJECT(treeview), "row-activated",
						G_CALLBACK(savedstatus_activated_cb), dialog);

	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	gtk_tree_selection_set_mode(sel, GTK_SELECTION_MULTIPLE);
	g_signal_connect(G_OBJECT(sel), "changed",
					 G_CALLBACK(status_selected_cb), dialog);

	gtk_container_add(GTK_CONTAINER(sw), treeview);

	/* Add columns */
	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, _("Title"));
	gtk_tree_view_column_set_resizable(column, TRUE);
	gtk_tree_view_column_set_min_width(column, 100);
	gtk_tree_view_column_set_sort_column_id(column,
											STATUS_WINDOW_COLUMN_TITLE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer, "text",
									   STATUS_WINDOW_COLUMN_TITLE);
#if GTK_CHECK_VERSION(2,6,0)
	g_object_set(renderer, "ellipsize", PANGO_ELLIPSIZE_END, NULL);
#endif

	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, _("Type"));
	gtk_tree_view_column_set_resizable(column, TRUE);
	gtk_tree_view_column_set_sort_column_id(column,
											STATUS_WINDOW_COLUMN_TYPE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer, "text",
									   STATUS_WINDOW_COLUMN_TYPE);

	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, _("Message"));
	gtk_tree_view_column_set_resizable(column, TRUE);
	gtk_tree_view_column_set_sort_column_id(column,
											STATUS_WINDOW_COLUMN_MESSAGE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer, "text",
									   STATUS_WINDOW_COLUMN_MESSAGE);
#if GTK_CHECK_VERSION(2,6,0)
	g_object_set(renderer, "ellipsize", PANGO_ELLIPSIZE_END, NULL);
#endif

	/* Enable CTRL+F searching */
	gtk_tree_view_set_search_column(GTK_TREE_VIEW(treeview), STATUS_WINDOW_COLUMN_TITLE);
	gtk_tree_view_set_search_equal_func(GTK_TREE_VIEW(treeview), search_func, NULL, NULL);

	/* Sort the title column by default */
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(dialog->model),
										 STATUS_WINDOW_COLUMN_TITLE,
										 GTK_SORT_ASCENDING);

	/* Populate list */
	populate_saved_status_list(dialog);

	gtk_widget_show_all(sw);

	return sw;
}

static gboolean
configure_cb(GtkWidget *widget, GdkEventConfigure *event, StatusWindow *dialog)
{
	if (GTK_WIDGET_VISIBLE(widget))
	{
		purple_prefs_set_int(PIDGIN_PREFS_ROOT "/status/dialog/width",  event->width);
		purple_prefs_set_int(PIDGIN_PREFS_ROOT "/status/dialog/height", event->height);
	}

	return FALSE;
}

void
pidgin_status_window_show(void)
{
	StatusWindow *dialog;
	GtkWidget *bbox;
	GtkWidget *button;
	GtkWidget *list;
	GtkWidget *vbox;
	GtkWidget *win;
	int width, height;

	if (status_window != NULL)
	{
		gtk_window_present(GTK_WINDOW(status_window->window));
		return;
	}

	status_window = dialog = g_new0(StatusWindow, 1);

	width  = purple_prefs_get_int(PIDGIN_PREFS_ROOT "/status/dialog/width");
	height = purple_prefs_get_int(PIDGIN_PREFS_ROOT "/status/dialog/height");

	dialog->window = win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(win), width, height);
	gtk_window_set_role(GTK_WINDOW(win), "statuses");
	gtk_window_set_title(GTK_WINDOW(win), _("Saved Statuses"));
	gtk_container_set_border_width(GTK_CONTAINER(win), PIDGIN_HIG_BORDER);

	g_signal_connect(G_OBJECT(win), "delete_event",
					 G_CALLBACK(status_window_destroy_cb), dialog);
	g_signal_connect(G_OBJECT(win), "configure_event",
					 G_CALLBACK(configure_cb), dialog);

	/* Setup the vbox */
	vbox = gtk_vbox_new(FALSE, PIDGIN_HIG_BORDER);
	gtk_container_add(GTK_CONTAINER(win), vbox);

	/* List of saved status states */
	list = create_saved_status_list(dialog);
	gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 0);

	/* Button box. */
	bbox = gtk_hbutton_box_new();
	gtk_box_set_spacing(GTK_BOX(bbox), PIDGIN_HIG_BOX_SPACE);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox), GTK_BUTTONBOX_END);
	gtk_box_pack_end(GTK_BOX(vbox), bbox, FALSE, TRUE, 0);

	/* Use button */
	button = pidgin_pixbuf_button_from_stock(_("_Use"), GTK_STOCK_EXECUTE,
										  PIDGIN_BUTTON_HORIZONTAL);
	dialog->use_button = button;
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	gtk_widget_set_sensitive(button, FALSE);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(status_window_use_cb), dialog);

	/* Add button */
	button = gtk_button_new_from_stock(GTK_STOCK_ADD);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(status_window_add_cb), dialog);

	/* Modify button */
	button = gtk_button_new_from_stock(PIDGIN_STOCK_MODIFY);
	dialog->modify_button = button;
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	gtk_widget_set_sensitive(button, FALSE);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(status_window_modify_cb), dialog);

	/* Delete button */
	button = gtk_button_new_from_stock(GTK_STOCK_DELETE);
	dialog->delete_button = button;
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	gtk_widget_set_sensitive(button, FALSE);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(status_window_delete_cb), dialog);

	/* Close button */
	button = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(status_window_close_cb), dialog);

	gtk_widget_show_all(win);
}

void
pidgin_status_window_hide(void)
{
	if (status_window == NULL)
		return;

	if (status_window->window != NULL)
		gtk_widget_destroy(status_window->window);

	purple_request_close_with_handle(status_window);
	purple_notify_close_with_handle(status_window);
	g_free(status_window);
	status_window = NULL;
}


/**************************************************************************
* Status editor
**************************************************************************/

static void substatus_editor_cancel_cb(GtkButton *button, gpointer user_data);

static void
status_editor_remove_dialog(StatusEditor *dialog)
{
	GtkTreeModel *model;
	GtkTreeIter iter;

	/* Remove the reference to this dialog from our parent's list store */
	if (status_window_find_savedstatus(&iter, dialog->original_title))
	{
		gtk_list_store_set(status_window->model, &iter,
							STATUS_WINDOW_COLUMN_WINDOW, NULL,
							-1);
	}

	/* Close any substatus editors that may be open */
	model = GTK_TREE_MODEL(dialog->model);
	if (gtk_tree_model_get_iter_first(model, &iter))
	{
		do {
			SubStatusEditor *substatus_dialog;

			gtk_tree_model_get(model, &iter,
							   STATUS_EDITOR_COLUMN_WINDOW, &substatus_dialog,
							   -1);
			if (substatus_dialog != NULL)
			{
				gtk_list_store_set(dialog->model, &iter,
								   STATUS_EDITOR_COLUMN_WINDOW, NULL,
								   -1);
				substatus_editor_cancel_cb(NULL, substatus_dialog);
			}
		} while (gtk_tree_model_iter_next(model, &iter));
	}
}


static gboolean
status_editor_destroy_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	StatusEditor *dialog = user_data;

	status_editor_remove_dialog(dialog);
	g_free(dialog->original_title);
	g_free(dialog);

	return FALSE;
}

static void
status_editor_cancel_cb(GtkButton *button, gpointer user_data)
{
	StatusEditor *dialog = user_data;

	status_editor_remove_dialog(dialog);
	gtk_widget_destroy(dialog->window);
	g_free(dialog->original_title);
	g_free(dialog);
}

static void
status_editor_ok_cb(GtkButton *button, gpointer user_data)
{
	StatusEditor *dialog = user_data;
	const char *title;
	PurpleStatusPrimitive type;
	char *message, *unformatted;
	PurpleSavedStatus *saved_status = NULL;
	GtkTreeModel *model;
	GtkTreeIter iter;

	title = gtk_entry_get_text(dialog->title);

	/*
	 * If we're saving this status, and the title is already taken
	 * then show an error dialog and don't do anything.
	 */
	if (((button == dialog->saveanduse_button) || (button == dialog->save_button)) &&
		(purple_savedstatus_find(title) != NULL) &&
		((dialog->original_title == NULL) || (strcmp(title, dialog->original_title))))
	{
		purple_notify_error(status_window, NULL, _("Title already in use.  You must "
						  "choose a unique title."), NULL);
		return;
	}

	type = gtk_option_menu_get_history(dialog->type) + (PURPLE_STATUS_UNSET + 1);
	message = gtk_imhtml_get_markup(dialog->message);
	unformatted = purple_markup_strip_html(message);

	/*
	 * If we're editing an old status, then lookup the old status.
	 * Note: It is possible that it has been deleted or renamed
	 *       or something, and no longer exists.
	 */
	if (dialog->original_title != NULL)
	{
		GtkTreeIter iter;

		saved_status = purple_savedstatus_find(dialog->original_title);

		if (status_window_find_savedstatus(&iter, dialog->original_title))
			gtk_list_store_remove(status_window->model, &iter);
	}

	if (saved_status == NULL)
	{
		/* This is a new status */
		if ((button == dialog->saveanduse_button)
				|| (button == dialog->save_button))
			saved_status = purple_savedstatus_new(title, type);
		else
			saved_status = purple_savedstatus_new(NULL, type);
	}
	else
	{
		/* Modify the old status */
		if (strcmp(title, dialog->original_title))
			purple_savedstatus_set_title(saved_status, title);
		purple_savedstatus_set_type(saved_status, type);
	}

	if (*unformatted == '\0')
		purple_savedstatus_set_message(saved_status, NULL);
	else
		purple_savedstatus_set_message(saved_status, message);

	/* Set any substatuses */
	model = GTK_TREE_MODEL(dialog->model);
	if (gtk_tree_model_get_iter_first(model, &iter))
	{
		do {
			PurpleAccount *account;
			gboolean enabled;
			char *id;
			char *message;
			PurpleStatusType *type;

			gtk_tree_model_get(model, &iter,
							   STATUS_EDITOR_COLUMN_ACCOUNT, &account,
							   STATUS_EDITOR_COLUMN_ENABLE_SUBSTATUS, &enabled,
							   STATUS_EDITOR_COLUMN_STATUS_ID, &id,
							   STATUS_EDITOR_COLUMN_STATUS_MESSAGE, &message,
							   -1);
			if (enabled)
			{
				type = purple_account_get_status_type(account, id);
				purple_savedstatus_set_substatus(saved_status, account, type, message);
			}
			else
			{
				purple_savedstatus_unset_substatus(saved_status, account);
			}
			g_free(id);
			g_free(message);
		} while (gtk_tree_model_iter_next(model, &iter));
	}

	g_free(message);
	g_free(unformatted);

	status_editor_remove_dialog(dialog);
	gtk_widget_destroy(dialog->window);
	g_free(dialog->original_title);

	if (status_window != NULL)
	  add_status_to_saved_status_list(status_window->model, saved_status);

	/* If they clicked on "Save & Use" or "Use," then activate the status */
	if (button != dialog->save_button)
		purple_savedstatus_activate(saved_status);

	g_free(dialog);
}

static void
editor_title_changed_cb(GtkWidget *widget, gpointer user_data)
{
	StatusEditor *dialog = user_data;
	const gchar *text;

	text = gtk_entry_get_text(dialog->title);

	gtk_widget_set_sensitive(GTK_WIDGET(dialog->saveanduse_button), (*text != '\0'));
	gtk_widget_set_sensitive(GTK_WIDGET(dialog->save_button), (*text != '\0'));
}

static GtkWidget *
create_status_type_menu(PurpleStatusPrimitive type)
{
	int i;
	GtkWidget *dropdown;
	GtkWidget *menu;
	GtkWidget *item;

	dropdown = gtk_option_menu_new();
	menu = gtk_menu_new();

	for (i = PURPLE_STATUS_UNSET + 1; i < PURPLE_STATUS_NUM_PRIMITIVES; i++)
	{
		item = gtk_menu_item_new_with_label(purple_primitive_get_name_from_type(i));
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	}

	gtk_menu_set_active(GTK_MENU(menu), type - (PURPLE_STATUS_UNSET + 1));
	gtk_option_menu_set_menu(GTK_OPTION_MENU(dropdown), menu);
	gtk_widget_show_all(menu);

	return dropdown;
}

static void edit_substatus(StatusEditor *status_editor, PurpleAccount *account);

static void
edit_substatus_cb(GtkTreeView *tv, GtkTreePath *path, GtkTreeViewColumn *col, gpointer user_data)
{
	StatusEditor *dialog = user_data;
	GtkTreeIter iter;
	PurpleAccount *account;

	gtk_tree_model_get_iter(GTK_TREE_MODEL(dialog->model), &iter, path);
	gtk_tree_model_get(GTK_TREE_MODEL(dialog->model), &iter,
					   STATUS_EDITOR_COLUMN_ACCOUNT, &account,
					   -1);

	edit_substatus(dialog, account);
}

static void
status_editor_substatus_cb(GtkCellRendererToggle *renderer, gchar *path_str, gpointer data)
{
	StatusEditor *dialog = (StatusEditor *)data;
	GtkTreeIter iter;
	gboolean enabled;
	PurpleAccount *account;

	gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(dialog->model), &iter, path_str);
	gtk_tree_model_get(GTK_TREE_MODEL(dialog->model), &iter,
					STATUS_EDITOR_COLUMN_ACCOUNT, &account,
					STATUS_EDITOR_COLUMN_ENABLE_SUBSTATUS, &enabled,
					-1);

	enabled = !enabled;

	if (enabled)
	{
		edit_substatus(dialog, account);
	}
	else
	{
		/* Remove the substatus */
		gtk_list_store_set(dialog->model, &iter,
						   STATUS_EDITOR_COLUMN_ENABLE_SUBSTATUS, enabled,
						   STATUS_EDITOR_COLUMN_STATUS_ID, NULL,
						   STATUS_EDITOR_COLUMN_STATUS_NAME, NULL,
						   STATUS_EDITOR_COLUMN_STATUS_MESSAGE, NULL,
						   -1);
	}
}

static void
status_editor_add_columns(StatusEditor *dialog)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	/* Enable Different status column */
	renderer = gtk_cell_renderer_toggle_new();
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(dialog->treeview),
						    -1, _("Different"),
						    renderer,
						    "active", STATUS_EDITOR_COLUMN_ENABLE_SUBSTATUS,
						    NULL);
	g_signal_connect(G_OBJECT(renderer), "toggled",
			 G_CALLBACK(status_editor_substatus_cb), dialog);

	/* Screen Name column */
	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_resizable(column, TRUE);
	gtk_tree_view_column_set_title(column, _("Screen Name"));
	gtk_tree_view_insert_column(GTK_TREE_VIEW(dialog->treeview), column, -1);
	gtk_tree_view_column_set_resizable(column, TRUE);

	/* Icon */
	renderer = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_column_pack_start(column, renderer, FALSE);
	gtk_tree_view_column_add_attribute(column, renderer, "pixbuf",
									   STATUS_EDITOR_COLUMN_ICON);

	/* Screen Name */
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer, "text",
									   STATUS_EDITOR_COLUMN_SCREENNAME);

	/* Status column */
	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_resizable(column, TRUE);
	gtk_tree_view_column_set_title(column, _("Status"));
	gtk_tree_view_insert_column(GTK_TREE_VIEW(dialog->treeview), column, -1);
	gtk_tree_view_column_set_resizable(column, TRUE);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer, "text",
									   STATUS_EDITOR_COLUMN_STATUS_NAME);

	/* Message column */
	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_resizable(column, TRUE);
	gtk_tree_view_column_set_title(column, _("Message"));
	gtk_tree_view_insert_column(GTK_TREE_VIEW(dialog->treeview), column, -1);
	gtk_tree_view_column_set_resizable(column, TRUE);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer, "text",
									   STATUS_EDITOR_COLUMN_STATUS_MESSAGE);

	g_signal_connect(G_OBJECT(dialog->treeview), "row-activated",
					G_CALLBACK(edit_substatus_cb), dialog);
}

static void
status_editor_set_account(GtkListStore *store, PurpleAccount *account,
						  GtkTreeIter *iter, PurpleSavedStatusSub *substatus)
{
	GdkPixbuf *pixbuf;
	const char *id = NULL, *name = NULL, *message = NULL;

	pixbuf = pidgin_create_prpl_icon(account, PIDGIN_PRPL_ICON_MEDIUM);
	if ((pixbuf != NULL) && !purple_account_is_connected(account))
	{
		gdk_pixbuf_saturate_and_pixelate(pixbuf, pixbuf, 0.0, FALSE);
	}

	if (substatus != NULL)
	{
		const PurpleStatusType *type;

		type = purple_savedstatus_substatus_get_type(substatus);
		id = purple_status_type_get_id(type);
		name = purple_status_type_get_name(type);
		if (purple_status_type_get_attr(type, "message"))
			message = purple_savedstatus_substatus_get_message(substatus);
	}

	gtk_list_store_set(store, iter,
			STATUS_EDITOR_COLUMN_ACCOUNT, account,
			STATUS_EDITOR_COLUMN_ENABLE_SUBSTATUS, (substatus != NULL),
			STATUS_EDITOR_COLUMN_ICON, pixbuf,
			STATUS_EDITOR_COLUMN_SCREENNAME, purple_account_get_username(account),
			STATUS_EDITOR_COLUMN_STATUS_ID, id,
			STATUS_EDITOR_COLUMN_STATUS_NAME, name,
			STATUS_EDITOR_COLUMN_STATUS_MESSAGE, message,
			-1);

	if (pixbuf != NULL)
		g_object_unref(G_OBJECT(pixbuf));
}

static void
status_editor_add_account(StatusEditor *dialog, PurpleAccount *account,
						  PurpleSavedStatusSub *substatus)
{
	GtkTreeIter iter;

	gtk_list_store_append(dialog->model, &iter);

	status_editor_set_account(dialog->model, account, &iter, substatus);
}

static void
status_editor_populate_list(StatusEditor *dialog, PurpleSavedStatus *saved_status)
{
	GList *iter;
	PurpleSavedStatusSub *substatus;

	gtk_list_store_clear(dialog->model);

	for (iter = purple_accounts_get_all(); iter != NULL; iter = iter->next)
	{
		PurpleAccount *account = (PurpleAccount *)iter->data;

		if (saved_status != NULL)
			substatus = purple_savedstatus_get_substatus(saved_status, account);
		else
			substatus = NULL;

		status_editor_add_account(dialog, account, substatus);
	}
}

void
pidgin_status_editor_show(gboolean edit, PurpleSavedStatus *saved_status)
{
	GtkTreeIter iter;
	StatusEditor *dialog;
	GtkSizeGroup *sg;
	GtkWidget *bbox;
	GtkWidget *button;
	GtkWidget *dbox;
	GtkWidget *expander;
	GtkWidget *dropdown;
	GtkWidget *entry;
	GtkWidget *frame;
	GtkWidget *hbox;
	GtkWidget *label;
	GtkWidget *sw;
	GtkWidget *text;
	GtkWidget *toolbar;
	GtkWidget *vbox;
	GtkWidget *win;
	GList *focus_chain = NULL;

	if (edit)
	{
		g_return_if_fail(saved_status != NULL);
		g_return_if_fail(!purple_savedstatus_is_transient(saved_status));
	}

	/* Find a possible window for this saved status and present it */
	if (edit && status_window_find_savedstatus(&iter, purple_savedstatus_get_title(saved_status)))
	{
		gtk_tree_model_get(GTK_TREE_MODEL(status_window->model), &iter,
							STATUS_WINDOW_COLUMN_WINDOW, &dialog,
							-1);
		if (dialog != NULL)
		{
			gtk_window_present(GTK_WINDOW(dialog->window));
			return;
		}
	}

	dialog = g_new0(StatusEditor, 1);
	if (edit && status_window_find_savedstatus(&iter, purple_savedstatus_get_title(saved_status)))
	{
		gtk_list_store_set(status_window->model, &iter,
							STATUS_WINDOW_COLUMN_WINDOW, dialog,
							-1);
	}

	if (edit)
		dialog->original_title = g_strdup(purple_savedstatus_get_title(saved_status));

	dialog->window = win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_role(GTK_WINDOW(win), "status");
	gtk_window_set_title(GTK_WINDOW(win), _("Status"));
	gtk_window_set_resizable(GTK_WINDOW(win), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(win), PIDGIN_HIG_BORDER);

	g_signal_connect(G_OBJECT(win), "delete_event",
					 G_CALLBACK(status_editor_destroy_cb), dialog);

	/* Setup the vbox */
	vbox = gtk_vbox_new(FALSE, PIDGIN_HIG_BORDER);
	gtk_container_add(GTK_CONTAINER(win), vbox);

	sg = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);

	/* Title */
	hbox = gtk_hbox_new(FALSE, PIDGIN_HIG_BOX_SPACE);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	label = gtk_label_new_with_mnemonic(_("_Title:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
	gtk_size_group_add_widget(sg, label);

	entry = gtk_entry_new();
	dialog->title = GTK_ENTRY(entry);
	if ((saved_status != NULL)
			&& !purple_savedstatus_is_transient(saved_status)
			&& (purple_savedstatus_get_title(saved_status) != NULL))
		gtk_entry_set_text(GTK_ENTRY(entry), purple_savedstatus_get_title(saved_status));
	gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 0);
	g_signal_connect(G_OBJECT(entry), "changed",
					 G_CALLBACK(editor_title_changed_cb), dialog);

	/* Status type */
	hbox = gtk_hbox_new(FALSE, PIDGIN_HIG_BOX_SPACE);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	label = gtk_label_new_with_mnemonic(_("_Status:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
	gtk_size_group_add_widget(sg, label);

	if (saved_status != NULL)
		dropdown = create_status_type_menu(purple_savedstatus_get_type(saved_status));
	else
		dropdown = create_status_type_menu(PURPLE_STATUS_AWAY);
	dialog->type = GTK_OPTION_MENU(dropdown);
	gtk_box_pack_start(GTK_BOX(hbox), dropdown, TRUE, TRUE, 0);

	/* Status message */
	hbox = gtk_hbox_new(FALSE, PIDGIN_HIG_BOX_SPACE);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	label = gtk_label_new_with_mnemonic(_("_Message:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
	gtk_size_group_add_widget(sg, label);

	frame = pidgin_create_imhtml(TRUE, &text, &toolbar, NULL);
	dialog->message = GTK_IMHTML(text);
	gtk_box_pack_start(GTK_BOX(hbox), frame, TRUE, TRUE, 0);
	focus_chain = g_list_prepend(focus_chain, dialog->message);
	gtk_container_set_focus_chain(GTK_CONTAINER(hbox), focus_chain);

	if ((saved_status != NULL) && (purple_savedstatus_get_message(saved_status) != NULL))
		gtk_imhtml_append_text(GTK_IMHTML(text),
							   purple_savedstatus_get_message(saved_status), 0);

	/* Different status message expander */
	expander = gtk_expander_new_with_mnemonic(_("Use a _different status for some accounts"));
	gtk_box_pack_start(GTK_BOX(vbox), expander, FALSE, FALSE, 0);

	/* Setup the box that the expander will cover */
	dbox = gtk_vbox_new(FALSE, PIDGIN_HIG_CAT_SPACE);
	gtk_container_add(GTK_CONTAINER(expander), dbox);

	/* Different status message treeview */
	sw = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
								   GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw),
										GTK_SHADOW_IN);
	gtk_box_pack_start(GTK_BOX(dbox), sw, TRUE, TRUE, 0);

	/* Create the list model */
	dialog->model = gtk_list_store_new(STATUS_EDITOR_NUM_COLUMNS,
									   G_TYPE_POINTER,
									   G_TYPE_POINTER,
									   G_TYPE_BOOLEAN,
									   GDK_TYPE_PIXBUF,
									   G_TYPE_STRING,
									   G_TYPE_STRING,
									   G_TYPE_STRING,
									   G_TYPE_STRING);

	/* Create the treeview */
	dialog->treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(dialog->model));
	gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(dialog->treeview), TRUE);
	gtk_widget_set_size_request(dialog->treeview, -1, 150);
	gtk_container_add(GTK_CONTAINER(sw), dialog->treeview);

	/* Add columns */
	status_editor_add_columns(dialog);

	/* Populate list */
	status_editor_populate_list(dialog, saved_status);

	/* Expand the treeview if we have substatuses */
	gtk_expander_set_expanded(GTK_EXPANDER(expander),
		(saved_status != NULL) && purple_savedstatus_has_substatuses(saved_status));

	/* Button box */
	bbox = gtk_hbutton_box_new();
	gtk_box_set_spacing(GTK_BOX(bbox), PIDGIN_HIG_BOX_SPACE);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox), GTK_BUTTONBOX_END);
	gtk_box_pack_end(GTK_BOX(vbox), bbox, FALSE, TRUE, 0);

	/* Cancel button */
	button = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(status_editor_cancel_cb), dialog);

	/* Use button */
	button = pidgin_pixbuf_button_from_stock(_("_Use"), GTK_STOCK_EXECUTE,
										   PIDGIN_BUTTON_HORIZONTAL);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(status_editor_ok_cb), dialog);

	/* Save & Use button */
	button = pidgin_pixbuf_button_from_stock(_("Sa_ve & Use"), GTK_STOCK_OK,
										   PIDGIN_BUTTON_HORIZONTAL);
	dialog->saveanduse_button = GTK_BUTTON(button);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	if (dialog->original_title == NULL)
		gtk_widget_set_sensitive(button, FALSE);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(status_editor_ok_cb), dialog);

	/* Save button */
	button = gtk_button_new_from_stock(GTK_STOCK_SAVE);
	dialog->save_button = GTK_BUTTON(button);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	if (dialog->original_title == NULL)
		gtk_widget_set_sensitive(button, FALSE);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(status_editor_ok_cb), dialog);

	gtk_widget_show_all(win);
}


/**************************************************************************
* SubStatus editor
**************************************************************************/

static void
substatus_selection_changed_cb(GtkComboBox *box, gpointer user_data)
{
	SubStatusEditor *select = user_data;
	GtkTreeIter iter;
	char *id;
	PurpleStatusType *type;

	if (!gtk_combo_box_get_active_iter(box, &iter))
		return;
	gtk_tree_model_get(GTK_TREE_MODEL(select->model), &iter,
					   SUBSTATUS_COLUMN_STATUS_ID, &id,
					   -1);
	type = purple_account_get_status_type(select->account, id);
	g_free(id);

	if (purple_status_type_get_attr(type, "message") == NULL)
	{
		gtk_widget_set_sensitive(GTK_WIDGET(select->message), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(select->toolbar), FALSE);
	}
	else
	{
		gtk_widget_set_sensitive(GTK_WIDGET(select->message), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(select->toolbar), TRUE);
	}
}

static gboolean
status_editor_find_account_in_treemodel(GtkTreeIter *iter,
										StatusEditor *status_editor,
										PurpleAccount *account)
{
	GtkTreeModel *model;
	PurpleAccount *cur;

	g_return_val_if_fail(status_editor != NULL, FALSE);
	g_return_val_if_fail(account       != NULL, FALSE);

	model = GTK_TREE_MODEL(status_editor->model);

	if (!gtk_tree_model_get_iter_first(model, iter))
		return FALSE;

	do {
		gtk_tree_model_get(model, iter, STATUS_EDITOR_COLUMN_ACCOUNT, &cur, -1);
		if (cur == account)
			return TRUE;
	} while (gtk_tree_model_iter_next(model, iter));

	return FALSE;
}

static void
substatus_editor_remove_dialog(SubStatusEditor *dialog)
{
	GtkTreeIter iter;

	if (status_editor_find_account_in_treemodel(&iter, dialog->status_editor, dialog->account))
	{
		/* Remove the reference to this dialog from our parent's list store */
		gtk_list_store_set(dialog->status_editor->model, &iter,
						   STATUS_EDITOR_COLUMN_WINDOW, NULL,
						   -1);
	}
}

static gboolean
substatus_editor_destroy_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	SubStatusEditor *dialog = user_data;

	substatus_editor_remove_dialog(dialog);
	g_free(dialog);

	return FALSE;
}

static void
substatus_editor_cancel_cb(GtkButton *button, gpointer user_data)
{
	SubStatusEditor *dialog = user_data;

	substatus_editor_remove_dialog(dialog);
	gtk_widget_destroy(dialog->window);
	g_free(dialog);
}


static void
substatus_editor_ok_cb(GtkButton *button, gpointer user_data)
{
	SubStatusEditor *dialog = user_data;
	StatusEditor *status_editor;
	GtkTreeIter iter;
	PurpleStatusType *type;
	char *id = NULL;
	char *message = NULL;
	const char *name = NULL;

	if (!gtk_combo_box_get_active_iter(dialog->box, &iter))
	{
		gtk_widget_destroy(dialog->window);
		g_free(dialog);
		return;
	}

	gtk_tree_model_get(GTK_TREE_MODEL(dialog->model), &iter,
					   SUBSTATUS_COLUMN_STATUS_ID, &id,
					   -1);
	type = purple_account_get_status_type(dialog->account, id);
	if (purple_status_type_get_attr(type, "message") != NULL)
		message = gtk_imhtml_get_text(GTK_IMHTML(dialog->message), NULL, NULL);
	name = purple_status_type_get_name(type);

	status_editor = dialog->status_editor;

	if (status_editor_find_account_in_treemodel(&iter, status_editor, dialog->account))
	{
		gtk_list_store_set(status_editor->model, &iter,
						   STATUS_EDITOR_COLUMN_ENABLE_SUBSTATUS, TRUE,
						   STATUS_EDITOR_COLUMN_STATUS_ID, id,
						   STATUS_EDITOR_COLUMN_STATUS_NAME, name,
						   STATUS_EDITOR_COLUMN_STATUS_MESSAGE, message,
						   STATUS_EDITOR_COLUMN_WINDOW, NULL,
						   -1);
	}

	gtk_widget_destroy(dialog->window);
	g_free(id);
	g_free(message);
	g_free(dialog);
}

static void
edit_substatus(StatusEditor *status_editor, PurpleAccount *account)
{
	char *tmp;
	SubStatusEditor *dialog;
	GtkSizeGroup *sg;
	GtkWidget *bbox;
	GtkWidget *button;
	GtkWidget *combo;
	GtkWidget *hbox;
	GtkWidget *frame;
	GtkWidget *label;
	GtkWidget *text;
	GtkWidget *toolbar;
	GtkWidget *vbox;
	GtkWidget *win;
	GtkTreeIter iter;
	GtkCellRenderer *rend;
	const char *status_id = NULL;
	const GList *list;
	gboolean select = FALSE;

	g_return_if_fail(status_editor != NULL);
	g_return_if_fail(account       != NULL);

	status_editor_find_account_in_treemodel(&iter, status_editor, account);
	gtk_tree_model_get(GTK_TREE_MODEL(status_editor->model), &iter,
						STATUS_EDITOR_COLUMN_WINDOW, &dialog,
						-1);
	if (dialog != NULL)
	{
		gtk_window_present(GTK_WINDOW(dialog->window));
		return;
	}

	dialog = g_new0(SubStatusEditor, 1);
	gtk_list_store_set(status_editor->model, &iter,
						STATUS_EDITOR_COLUMN_WINDOW, dialog,
						-1);
	dialog->status_editor = status_editor;
	dialog->account = account;

	dialog->window = win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_role(GTK_WINDOW(win), "substatus");
	tmp = g_strdup_printf(_("Status for %s"), purple_account_get_username(account));
	gtk_window_set_title(GTK_WINDOW(win), tmp);
	g_free(tmp);
	gtk_window_set_resizable(GTK_WINDOW(win), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(win), PIDGIN_HIG_BORDER);

	g_signal_connect(G_OBJECT(win), "delete_event",
					 G_CALLBACK(substatus_editor_destroy_cb), dialog);

	/* Setup the vbox */
	vbox = gtk_vbox_new(FALSE, PIDGIN_HIG_BORDER);
	gtk_container_add(GTK_CONTAINER(win), vbox);

	sg = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);

	/* Status type */
	hbox = gtk_hbox_new(FALSE, PIDGIN_HIG_BOX_SPACE);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	label = gtk_label_new_with_mnemonic(_("_Status:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
	gtk_size_group_add_widget(sg, label);

	dialog->model = gtk_list_store_new(SUBSTATUS_NUM_COLUMNS,
									   GDK_TYPE_PIXBUF,
									   G_TYPE_STRING,
									   G_TYPE_STRING);
	combo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(dialog->model));
	dialog->box = GTK_COMBO_BOX(combo);

	rend = GTK_CELL_RENDERER(gtk_cell_renderer_pixbuf_new());
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo), rend, FALSE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo), rend,
						"pixbuf", SUBSTATUS_COLUMN_ICON, NULL);

	rend = GTK_CELL_RENDERER(gtk_cell_renderer_text_new());
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo), rend, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo), rend,
						"text", SUBSTATUS_COLUMN_STATUS_NAME, NULL);

	g_signal_connect(G_OBJECT(combo), "changed",
					 G_CALLBACK(substatus_selection_changed_cb), dialog);

	gtk_box_pack_start(GTK_BOX(hbox), combo, FALSE, FALSE, 0);

	/* Status mesage */
	hbox = gtk_hbox_new(FALSE, PIDGIN_HIG_BOX_SPACE);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	label = gtk_label_new_with_mnemonic(_("_Message:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
	gtk_size_group_add_widget(sg, label);

	frame = pidgin_create_imhtml(TRUE, &text, &toolbar, NULL);
	dialog->message = GTK_IMHTML(text);
	dialog->toolbar = GTK_IMHTMLTOOLBAR(toolbar);
	gtk_box_pack_start(GTK_BOX(hbox), frame, TRUE, TRUE, 0);

	/* Button box */
	bbox = gtk_hbutton_box_new();
	gtk_box_set_spacing(GTK_BOX(bbox), PIDGIN_HIG_BOX_SPACE);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox), GTK_BUTTONBOX_END);
	gtk_box_pack_end(GTK_BOX(vbox), bbox, FALSE, TRUE, 0);

	/* Cancel button */
	button = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(substatus_editor_cancel_cb), dialog);

	/* OK button */
	button = gtk_button_new_from_stock(GTK_STOCK_OK);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(substatus_editor_ok_cb), dialog);

	/* Seed the input widgets with the current values */
	/* TODO: Get the current values from our parent's list store, not the saved_status! */
	if (status_editor->original_title != NULL)
	{
		PurpleSavedStatus *saved_status = NULL;
		PurpleSavedStatusSub *substatus = NULL;

		saved_status = purple_savedstatus_find(status_editor->original_title);
		if (saved_status != NULL)
			substatus = purple_savedstatus_get_substatus(saved_status, account);

		if (substatus != NULL)
		{
			gtk_imhtml_append_text(dialog->message,
								   purple_savedstatus_substatus_get_message(substatus),
								   0);
			status_id = purple_status_type_get_id(purple_savedstatus_substatus_get_type(substatus));
		}
		/* TODO: Else get the generic status type from our parent */
	}

	for (list = purple_account_get_status_types(account); list; list = list->next)
	{
		PurpleStatusType *status_type;
		GdkPixbuf *pixbuf;
		const char *id, *name;

		status_type = list->data;

		/* Only allow users to select statuses that are flagged as "user settable" */
		if (!purple_status_type_is_user_settable(status_type))
			continue;

		id = purple_status_type_get_id(status_type);
		pixbuf = pidgin_create_status_icon(purple_status_type_get_primitive(status_type), combo, PIDGIN_ICON_SIZE_TANGO_EXTRA_SMALL);
		name = purple_status_type_get_name(status_type);

		gtk_list_store_append(dialog->model, &iter);
		gtk_list_store_set(dialog->model, &iter,
						   SUBSTATUS_COLUMN_ICON, pixbuf,
						   SUBSTATUS_COLUMN_STATUS_ID, id,
						   SUBSTATUS_COLUMN_STATUS_NAME, name,
						   -1);
		if (pixbuf != NULL)
			g_object_unref(pixbuf);
		if ((status_id != NULL) && !strcmp(status_id, id))
		{
			gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo), &iter);
			select = TRUE;
		}
	}

	if (!select)
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);

	gtk_widget_show_all(win);
}


/**************************************************************************
 * Utilities                                                              *
 **************************************************************************/

enum {
	SS_MENU_ENTRY_TYPE_PRIMITIVE,
	SS_MENU_ENTRY_TYPE_SAVEDSTATUS
};

enum {
	/** _SSMenuEntryType */
	SS_MENU_TYPE_COLUMN,

	/**
	 * This is a GdkPixbuf (the other columns are strings).
	 * This column is visible.
	 */
	SS_MENU_ICON_COLUMN,

	/** The text displayed on the status box.  This column is visible. */
	SS_MENU_TEXT_COLUMN,

	/**
	 * This value depends on SS_MENU_TYPE_COLUMN.  For _SAVEDSTATUS types,
	 * this is the creation time.  For _PRIMITIVE types,
	 * this is the PurpleStatusPrimitive.
	 */
	SS_MENU_DATA_COLUMN,

	/**
	 * This is the emblem to use for this status
	 */
	SS_MENU_EMBLEM_COLUMN,

	/**
	 * And whether or not that emblem is visible
	 */
	SS_MENU_EMBLEM_VISIBLE_COLUMN,
	
	SS_MENU_NUM_COLUMNS
};

static void
status_menu_cb(GtkComboBox *widget, void(*callback)(PurpleSavedStatus*))
{
	GtkTreeIter iter;
	int type;
	gpointer data;
	PurpleSavedStatus *status = NULL;

	if (!gtk_combo_box_get_active_iter(widget, &iter))
		return;

	gtk_tree_model_get(gtk_combo_box_get_model(widget), &iter,
			   SS_MENU_TYPE_COLUMN, &type,
			   SS_MENU_DATA_COLUMN, &data,
			   -1);

	if (type == SS_MENU_ENTRY_TYPE_PRIMITIVE)
	{
		PurpleStatusPrimitive primitive = GPOINTER_TO_INT(data);
		status = purple_savedstatus_find_transient_by_type_and_message(primitive, NULL);
		if (status == NULL)
			status = purple_savedstatus_new(NULL, primitive);
	}
	else if (type == SS_MENU_ENTRY_TYPE_SAVEDSTATUS)
		status = purple_savedstatus_find_by_creation_time(GPOINTER_TO_INT(data));

	callback(status);
}

static gint
saved_status_sort_alphabetically_func(gconstpointer a, gconstpointer b)
{
	const PurpleSavedStatus *saved_status_a = a;
	const PurpleSavedStatus *saved_status_b = b;
	return g_utf8_collate(purple_savedstatus_get_title(saved_status_a),
				  purple_savedstatus_get_title(saved_status_b));
}

static gboolean pidgin_status_menu_add_primitive(GtkListStore *model, GtkWidget *w, PurpleStatusPrimitive primitive,
	PurpleSavedStatus *current_status)
{
	GtkTreeIter iter;
	gboolean currently_selected = FALSE;
	GdkPixbuf *pixbuf = pidgin_create_status_icon(primitive, w, PIDGIN_ICON_SIZE_TANGO_EXTRA_SMALL);

	gtk_list_store_append(model, &iter);
	gtk_list_store_set(model, &iter,
			   SS_MENU_TYPE_COLUMN, SS_MENU_ENTRY_TYPE_PRIMITIVE,
			   SS_MENU_ICON_COLUMN, pixbuf,
			   SS_MENU_TEXT_COLUMN, purple_primitive_get_name_from_type(primitive),
			   SS_MENU_DATA_COLUMN, GINT_TO_POINTER(primitive),
			   SS_MENU_EMBLEM_VISIBLE_COLUMN, FALSE,
			   -1);
	if (pixbuf != NULL)
		g_object_unref(pixbuf);

	if (purple_savedstatus_is_transient(current_status)
			&& !purple_savedstatus_has_substatuses(current_status)
			&& purple_savedstatus_get_type(current_status) == primitive)
		currently_selected = TRUE;

	return currently_selected;
}

GtkWidget *pidgin_status_menu(PurpleSavedStatus *current_status, GCallback callback)
{
	GtkWidget *combobox;
	GtkListStore *model;
	GList *sorted, *cur;
	int i = 0;
	int index = -1;
	GdkPixbuf *pixbuf;
	GtkTreeIter iter;
	GtkCellRenderer *text_rend;
	GtkCellRenderer *icon_rend;
	GtkCellRenderer *emblem_rend;

	model = gtk_list_store_new(SS_MENU_NUM_COLUMNS, G_TYPE_INT, GDK_TYPE_PIXBUF,
				   G_TYPE_STRING, G_TYPE_POINTER, G_TYPE_STRING, G_TYPE_BOOLEAN);

	combobox = gtk_combo_box_new();

	if (pidgin_status_menu_add_primitive(model, combobox, PURPLE_STATUS_AVAILABLE, current_status))
		index = i;
	i++;

	if (pidgin_status_menu_add_primitive(model, combobox, PURPLE_STATUS_AWAY, current_status))
		index = i;
	i++;

	if (pidgin_status_menu_add_primitive(model, combobox, PURPLE_STATUS_INVISIBLE, current_status))
		index = i;
	i++;

	if (pidgin_status_menu_add_primitive(model, combobox, PURPLE_STATUS_OFFLINE, current_status))
		index = i;
	i++;

	sorted = g_list_copy((GList *)purple_savedstatuses_get_all());
	sorted = g_list_sort(sorted, saved_status_sort_alphabetically_func);
	for (cur = sorted; cur; cur = cur->next)
	{
		PurpleSavedStatus *status = (PurpleSavedStatus *) cur->data;
		if (!purple_savedstatus_is_transient(status))
		{
			pixbuf = pidgin_create_status_icon(purple_savedstatus_get_type(status),
							combobox, PIDGIN_ICON_SIZE_TANGO_EXTRA_SMALL);
			gtk_list_store_append(model, &iter);
			gtk_list_store_set(model, &iter,
				SS_MENU_TYPE_COLUMN, SS_MENU_ENTRY_TYPE_SAVEDSTATUS,
				SS_MENU_ICON_COLUMN, pixbuf,
				SS_MENU_TEXT_COLUMN, purple_savedstatus_get_title(status),
				SS_MENU_DATA_COLUMN, GINT_TO_POINTER(purple_savedstatus_get_creation_time(status)),
				SS_MENU_EMBLEM_COLUMN, GTK_STOCK_SAVE,
				SS_MENU_EMBLEM_VISIBLE_COLUMN, TRUE,
				-1);
			g_object_unref(G_OBJECT(pixbuf));

			if (status == current_status)
				index = i;
			i++;
		}
	}
	g_list_free(sorted);

	gtk_combo_box_set_model(GTK_COMBO_BOX(combobox), GTK_TREE_MODEL(model));

	text_rend = gtk_cell_renderer_text_new();
	icon_rend = gtk_cell_renderer_pixbuf_new();
	emblem_rend = gtk_cell_renderer_pixbuf_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox), icon_rend, FALSE);
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox), text_rend, TRUE);
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox), emblem_rend, FALSE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combobox), icon_rend, "pixbuf", SS_MENU_ICON_COLUMN, NULL);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combobox), text_rend, "markup", SS_MENU_TEXT_COLUMN, NULL);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combobox), emblem_rend,
					"stock-id", SS_MENU_EMBLEM_COLUMN, "visible", SS_MENU_EMBLEM_VISIBLE_COLUMN, NULL);

	gtk_combo_box_set_active(GTK_COMBO_BOX(combobox), index);
	g_signal_connect(G_OBJECT(combobox), "changed", G_CALLBACK(status_menu_cb), callback);

	return combobox;
}


/**************************************************************************
* GTK+ saved status glue
**************************************************************************/

void *
pidgin_status_get_handle(void)
{
	static int handle;

	return &handle;
}

void
pidgin_status_init(void)
{
	purple_prefs_add_none(PIDGIN_PREFS_ROOT "/status");
	purple_prefs_add_none(PIDGIN_PREFS_ROOT "/status/dialog");
	purple_prefs_add_int(PIDGIN_PREFS_ROOT "/status/dialog/width",  550);
	purple_prefs_add_int(PIDGIN_PREFS_ROOT "/status/dialog/height", 250);
}

void
pidgin_status_uninit(void)
{
	pidgin_status_window_hide();
}