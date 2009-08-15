/**
 * @file gtkprivacy.c GTK+ Privacy UI
 * @ingroup pidgin
 */

/* pidgin
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
 */
#include "internal.h"
#include "pidgin.h"

#include "connection.h"
#include "debug.h"
#include "privacy.h"
#include "request.h"
#include "util.h"

#include "gtkblist.h"
#include "gtkprivacy.h"
#include "gtkutils.h"

typedef struct
{
	GtkWidget *win;
	GtkWidget *type_menu;
	GtkWidget *close_button;
	GtkWidget *vbox_pas;
	GtkWidget *sw_contacts;
	GtkWidget *add_contact_button;

	GtkListStore *model_presence;
	GtkListStore *model_contacts;

	gboolean show_sw_contacts;
	PurpleAccount *account;

} PidginPrivacyDialog;

enum
{
	PRESENCE_COLUMN_ICON,
	PRESENCE_COLUMN_USERNAME,
	PRESENCE_COLUMN_INV_NONBUD,
	PRESENCE_COLUMN_INV_ALL,
	PRESENCE_COLUMN_PROTOCOL,
	PRESENCE_COLUMN_DATA,
	PRESENCE_NUM_COLUMNS
};

enum
{
	CONTACTS_COLUMN_BUDDYNAME,
	CONTACTS_COLUMN_BLOCK_ALL,
	CONTACTS_COLUMN_INVISIBLE,
	CONTACTS_COLUMN_BLOCK_CHAT,
	CONTACTS_COLUMN_BLOCK_FT,
	CONTACTS_COLUMN_BLOCK_CONF,
	CONTACTS_COLUMN_DATA,
	CONTACTS_NUM_COLUMNS
};

static void show_contacts(PidginPrivacyDialog *dialog);

typedef struct
{
	PurpleAccount *account;
	char *name;
	gboolean block;

} PidginPrivacyRequestData;

static struct
{
	const char *text;
	int num;

} const menu_entries[] =
{
	{ N_("Allow all users to contact me"), PURPLE_PRIVACY_ALLOW_ALL },
	{ N_("Block messages from users not in buddly list"), PURPLE_PRIVACY_BLOCK_MSG_NONBUDDY },
	{ N_("Block all users not in buddy list"), PURPLE_PRIVACY_ALLOW_BUDDYLIST },
	{ N_("Per contact settings"), PURPLE_PRIVACY_CUSTOM }
};

static const size_t menu_entry_count = sizeof(menu_entries) / sizeof(*menu_entries);

static PidginPrivacyDialog *privacy_dialog = NULL;

static gint
destroy_cb(GtkWidget *w, GdkEvent *event, PidginPrivacyDialog *dialog)
{
	pidgin_privacy_dialog_hide();

	return 0;
}

static void
select_account_cb(GtkWidget *dropdown, PurpleAccount *account,
				  PidginPrivacyDialog *dialog)
{
	int i;

	dialog->account = account;
	gtk_option_menu_set_history(GTK_OPTION_MENU(dialog->type_menu), 0);

	for (i = 0; i < menu_entry_count; i++) {
		if( (menu_entries[i].num == purple_privacy_obtain_account_state(account)) ) {
			gtk_option_menu_set_history(GTK_OPTION_MENU(dialog->type_menu), i);
			break;
		}
	}
}

static void
type_changed_cb(GtkOptionMenu *optmenu, PidginPrivacyDialog *dialog)
{
	int new_type = menu_entries[gtk_option_menu_get_history(optmenu)].num;

	purple_privacy_set_account_state(dialog->account, new_type);

	if(new_type == PURPLE_PRIVACY_CUSTOM) {
		if(dialog->show_sw_contacts == FALSE)
			show_contacts(dialog);
	}
	else {
		if(dialog->show_sw_contacts == TRUE) {
			/* destroy model_contacts */
			gtk_widget_hide(dialog->sw_contacts);
			gtk_widget_hide(dialog->add_contact_button);
			dialog->show_sw_contacts = FALSE;
		}
	}
}

static void
close_cb(GtkWidget *button, PidginPrivacyDialog *dialog)
{
	gtk_widget_destroy(dialog->win);

	pidgin_privacy_dialog_hide();
}

static void toggle_button_cb(GtkWidget *widget, gpointer data)
{
	PidginPrivacyDialog *dialog = data;
	PurplePrivacySetting privacy_type = (PurplePrivacySetting) g_object_get_data(G_OBJECT(widget), "choice_id"); 
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
	{
		if(privacy_type == PURPLE_PRIVACY_CUSTOM)
			gtk_widget_show(dialog->vbox_pas);
	}
	else
	{
		if(privacy_type == PURPLE_PRIVACY_CUSTOM)
			gtk_widget_hide(dialog->vbox_pas);
	}
}

static void show_offline_nonbud_cb(GtkWidget *widget, gpointer data)
{
}

static void show_offline_all_cb(GtkCellRendererToggle *renderer, gchar *path_str, gpointer data)
{
	PidginPrivacyDialog *dialog = (PidginPrivacyDialog *)data;
	PurpleAccount *account;
	GtkTreeModel *model = GTK_TREE_MODEL(dialog->model_presence);
	GtkTreeIter iter;
	gboolean state;
	gboolean new_state = FALSE;

	gtk_tree_model_get_iter_from_string(model, &iter, path_str);
	gtk_tree_model_get(model, &iter,
					   PRESENCE_COLUMN_DATA, &account,
					   PRESENCE_COLUMN_INV_ALL, &state,
					   -1);

	state = !state;
	if(state) {
		if(purple_privacy_account_supports_invisible_status(account))
			new_state = purple_privacy_set_account_status_invisible(account);
	}
	else
		new_state = !( purple_privacy_set_account_status_visible(account) );

	gtk_list_store_set(dialog->model_presence, &iter,
						PRESENCE_COLUMN_INV_ALL, new_state,
						-1);
}

static void
add_columns_show_offline(GtkWidget *treeview, PidginPrivacyDialog *dialog)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	/* Username column */
	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, _("Username"));
	gtk_tree_view_column_set_resizable(column, TRUE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

	/* Username */
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer, "text", PRESENCE_COLUMN_USERNAME);

	/* Protocol column */
	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, _("Protocol"));
	gtk_tree_view_column_set_resizable(column, FALSE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

	/* Icon */
	renderer = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_column_pack_start(column, renderer, FALSE);
	gtk_tree_view_column_add_attribute(column, renderer, "pixbuf", PRESENCE_COLUMN_ICON);

	/* Protocol */
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer, "text", PRESENCE_COLUMN_PROTOCOL);

	/* Show Offline to non-buddies */
	renderer = gtk_cell_renderer_toggle_new();
	g_signal_connect(G_OBJECT(renderer), "toggled", G_CALLBACK(show_offline_nonbud_cb), dialog); /* privacy laters: connect signal */
	column = gtk_tree_view_column_new_with_attributes(_("Invisible to Non-Buddies"), renderer, "active", PRESENCE_COLUMN_INV_NONBUD, NULL);
	gtk_tree_view_column_set_resizable(column, FALSE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

	/* Show Offline to everyone */
	renderer = gtk_cell_renderer_toggle_new();
	g_signal_connect(G_OBJECT(renderer), "toggled", G_CALLBACK(show_offline_all_cb), dialog); /* privacy laters: connect signal */
	column = gtk_tree_view_column_new_with_attributes(_("Invisible to Everyone"), renderer, "active", PRESENCE_COLUMN_INV_ALL, NULL);
	gtk_tree_view_column_set_resizable(column, FALSE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
}

static void populate_accounts_list(PidginPrivacyDialog *dialog)
{
/* Privacy laters: this is trouble, it doesnt change the list when an account signs off or signs in */
	GList *l;
	GtkTreeIter iter;
	PurpleAccount *account;
	GdkPixbuf *pixbuf;

	gtk_list_store_clear(dialog->model_presence);

	for (l = purple_accounts_get_all_active(); l != NULL; l = g_list_delete_link(l,l)) {
		account = l->data;
		gtk_list_store_append(dialog->model_presence, &iter);
		pixbuf = pidgin_create_prpl_icon(account, PIDGIN_PRPL_ICON_MEDIUM);

		gtk_list_store_set(dialog->model_presence, &iter,
			PRESENCE_COLUMN_ICON, pixbuf,
			PRESENCE_COLUMN_USERNAME, purple_account_get_username(account),
			PRESENCE_COLUMN_INV_NONBUD, TRUE,	/* replace with invisiblity check */
			PRESENCE_COLUMN_INV_ALL, purple_privacy_account_status_invisible(account),
			PRESENCE_COLUMN_PROTOCOL, purple_account_get_protocol_name(account),
			PRESENCE_COLUMN_DATA, account,
			-1);

		if (pixbuf != NULL)
			g_object_unref(G_OBJECT(pixbuf));
	}
}

static void contacts_block_all_cb(GtkCellRendererToggle *renderer, gchar *path_str, gpointer data)
{
	PidginPrivacyDialog *dialog = (PidginPrivacyDialog *)data;
	PurpleAccount *account;
	GtkTreeModel *model = GTK_TREE_MODEL(dialog->model_contacts);
	GtkTreeIter iter;
	gboolean state;
	char *name;

	gtk_tree_model_get_iter_from_string(model, &iter, path_str);
	gtk_tree_model_get(model, &iter,
				CONTACTS_COLUMN_DATA, &account,
				CONTACTS_COLUMN_BLOCK_ALL, &state,
				CONTACTS_COLUMN_BUDDYNAME, &name,
				-1);

	state = !state;
	if(state)
		purple_privacy_set_block_all(account, name, TRUE, TRUE);
	else
		purple_privacy_unset_block_all(account, name, TRUE, TRUE);

	gtk_list_store_set(dialog->model_contacts, &iter,
				CONTACTS_COLUMN_BLOCK_ALL, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_ALL),
				CONTACTS_COLUMN_INVISIBLE, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_PRESENCE),
				CONTACTS_COLUMN_BLOCK_CHAT, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_MESSAGE),
				CONTACTS_COLUMN_BLOCK_FT, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_FT),
				CONTACTS_COLUMN_BLOCK_CONF, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_CONF),
				-1);
}

static void contacts_invisible_cb(GtkCellRendererToggle *renderer, gchar *path_str, gpointer data)
{
	PidginPrivacyDialog *dialog = (PidginPrivacyDialog *)data;
	PurpleAccount *account;
	GtkTreeModel *model = GTK_TREE_MODEL(dialog->model_contacts);
	GtkTreeIter iter;
	gboolean state;
	char *name;

	gtk_tree_model_get_iter_from_string(model, &iter, path_str);
	gtk_tree_model_get(model, &iter,
				CONTACTS_COLUMN_DATA, &account,
				CONTACTS_COLUMN_INVISIBLE, &state,
				CONTACTS_COLUMN_BUDDYNAME, &name,
				-1);

	state = !state;
	if(state)
		purple_privacy_set_block_presence(account, name, TRUE, TRUE);
	else
		purple_privacy_unset_block_presence(account, name, TRUE, TRUE);

	gtk_list_store_set(dialog->model_contacts, &iter,
				CONTACTS_COLUMN_BLOCK_ALL, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_ALL),
				CONTACTS_COLUMN_INVISIBLE, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_PRESENCE),
				CONTACTS_COLUMN_BLOCK_CHAT, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_MESSAGE),
				CONTACTS_COLUMN_BLOCK_FT, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_FT),
				CONTACTS_COLUMN_BLOCK_CONF, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_CONF),
				-1);
}

static void contacts_block_chat_cb(GtkCellRendererToggle *renderer, gchar *path_str, gpointer data)
{
	PidginPrivacyDialog *dialog = (PidginPrivacyDialog *)data;
	PurpleAccount *account;
	GtkTreeModel *model = GTK_TREE_MODEL(dialog->model_contacts);
	GtkTreeIter iter;
	gboolean state;
	char *name;

	gtk_tree_model_get_iter_from_string(model, &iter, path_str);
	gtk_tree_model_get(model, &iter,
				CONTACTS_COLUMN_DATA, &account,
				CONTACTS_COLUMN_BLOCK_CHAT, &state,
				CONTACTS_COLUMN_BUDDYNAME, &name,
				-1);

	state = !state;
	if(state)
		purple_privacy_set_block_message(account, name);
	else
		purple_privacy_unset_block_message(account, name);

	gtk_list_store_set(dialog->model_contacts, &iter,
				CONTACTS_COLUMN_BLOCK_ALL, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_ALL),
				CONTACTS_COLUMN_INVISIBLE, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_PRESENCE),
				CONTACTS_COLUMN_BLOCK_CHAT, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_MESSAGE),
				CONTACTS_COLUMN_BLOCK_FT, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_FT),
				CONTACTS_COLUMN_BLOCK_CONF, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_CONF),
				-1);
}

static void contacts_block_ft_cb(GtkCellRendererToggle *renderer, gchar *path_str, gpointer data)
{
	PidginPrivacyDialog *dialog = (PidginPrivacyDialog *)data;
	PurpleAccount *account;
	GtkTreeModel *model = GTK_TREE_MODEL(dialog->model_contacts);
	GtkTreeIter iter;
	gboolean state;
	char *name;

	gtk_tree_model_get_iter_from_string(model, &iter, path_str);
	gtk_tree_model_get(model, &iter,
				CONTACTS_COLUMN_DATA, &account,
				CONTACTS_COLUMN_BLOCK_FT, &state,
				CONTACTS_COLUMN_BUDDYNAME, &name,
				-1);

	state = !state;
	if(state)
		purple_privacy_set_block_ft(account, name);
	else
		purple_privacy_unset_block_ft(account, name);

	gtk_list_store_set(dialog->model_contacts, &iter,
				CONTACTS_COLUMN_BLOCK_ALL, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_ALL),
				CONTACTS_COLUMN_INVISIBLE, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_PRESENCE),
				CONTACTS_COLUMN_BLOCK_CHAT, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_MESSAGE),
				CONTACTS_COLUMN_BLOCK_FT, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_FT),
				CONTACTS_COLUMN_BLOCK_CONF, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_CONF),
				-1);
}

static void contacts_block_conf_cb(GtkCellRendererToggle *renderer, gchar *path_str, gpointer data)
{
	PidginPrivacyDialog *dialog = (PidginPrivacyDialog *)data;
	PurpleAccount *account;
	GtkTreeModel *model = GTK_TREE_MODEL(dialog->model_contacts);
	GtkTreeIter iter;
	gboolean state;
	char *name;

	gtk_tree_model_get_iter_from_string(model, &iter, path_str);
	gtk_tree_model_get(model, &iter,
				CONTACTS_COLUMN_DATA, &account,
				CONTACTS_COLUMN_BLOCK_CONF, &state,
				CONTACTS_COLUMN_BUDDYNAME, &name,
				-1);

	state = !state;
	if(state)
		purple_privacy_set_block_conf(account, name);
	else
		purple_privacy_unset_block_conf(account, name);

	gtk_list_store_set(dialog->model_contacts, &iter,
				CONTACTS_COLUMN_BLOCK_ALL, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_ALL),
				CONTACTS_COLUMN_INVISIBLE, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_PRESENCE),
				CONTACTS_COLUMN_BLOCK_CHAT, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_MESSAGE),
				CONTACTS_COLUMN_BLOCK_FT, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_FT),
				CONTACTS_COLUMN_BLOCK_CONF, purple_privacy_check(account, name, PURPLE_PRIVACY_BLOCK_CONF),
				-1);
}

static void populate_contacts_list(PidginPrivacyDialog *dialog)
{
	GSList *l;
	GtkTreeIter iter;
	PurpleAccount *account = dialog->account;
	PurpleBuddy *c;

	gtk_list_store_clear(dialog->model_contacts);

	/* add "All Others" meta contact */
	gtk_list_store_append(dialog->model_contacts, &iter);
	gtk_list_store_set(dialog->model_contacts, &iter,
				CONTACTS_COLUMN_BUDDYNAME, "All Others",
				CONTACTS_COLUMN_BLOCK_ALL, purple_privacy_check(account, "all-others", PURPLE_PRIVACY_BLOCK_ALL),
				CONTACTS_COLUMN_INVISIBLE, purple_privacy_check(account, "all-others", PURPLE_PRIVACY_BLOCK_PRESENCE),
				CONTACTS_COLUMN_BLOCK_CHAT, purple_privacy_check(account, "all-others", PURPLE_PRIVACY_BLOCK_MESSAGE),
				CONTACTS_COLUMN_BLOCK_FT, purple_privacy_check(account, "all-others", PURPLE_PRIVACY_BLOCK_FT),
				CONTACTS_COLUMN_BLOCK_CONF, purple_privacy_check(account, "all-others", PURPLE_PRIVACY_BLOCK_CONF),
				CONTACTS_COLUMN_DATA, account,
				-1);

	/* Add the contacts now */	
	for (l = purple_find_privacy_contacts(account, NULL); l != NULL; l = g_slist_delete_link(l,l))
	{
		c = l->data;
		if(purple_strequal(c->name, "all-others") == FALSE)
		{
			gtk_list_store_append(dialog->model_contacts, &iter);
			gtk_list_store_set(dialog->model_contacts, &iter,
				CONTACTS_COLUMN_BUDDYNAME, purple_buddy_get_name(c),
				CONTACTS_COLUMN_BLOCK_ALL, purple_privacy_check(account, purple_buddy_get_name(c), PURPLE_PRIVACY_BLOCK_ALL),
				CONTACTS_COLUMN_INVISIBLE, purple_privacy_check(account, purple_buddy_get_name(c), PURPLE_PRIVACY_BLOCK_PRESENCE),
				CONTACTS_COLUMN_BLOCK_CHAT, purple_privacy_check(account, purple_buddy_get_name(c), PURPLE_PRIVACY_BLOCK_MESSAGE),
				CONTACTS_COLUMN_BLOCK_FT, purple_privacy_check(account, purple_buddy_get_name(c), PURPLE_PRIVACY_BLOCK_FT),
				CONTACTS_COLUMN_BLOCK_CONF, purple_privacy_check(account, purple_buddy_get_name(c), PURPLE_PRIVACY_BLOCK_CONF),
				CONTACTS_COLUMN_DATA, account,
				-1);
		}
	}
}

static void
add_columns_contacts(GtkWidget *treeview, PidginPrivacyDialog *dialog)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	/* Buddyname column */
	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, _("Buddy"));
	gtk_tree_view_column_set_resizable(column, TRUE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

	/* Buddyname */
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer, "text", CONTACTS_COLUMN_BUDDYNAME);

	/* Block All */
	renderer = gtk_cell_renderer_toggle_new();
	g_signal_connect(G_OBJECT(renderer), "toggled", G_CALLBACK(contacts_block_all_cb), dialog); /* privacy laters: connect signal */
	column = gtk_tree_view_column_new_with_attributes(_("Block All"), renderer, "active", CONTACTS_COLUMN_BLOCK_ALL, NULL);
	gtk_tree_view_column_set_resizable(column, FALSE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

	/* Invisible */
	renderer = gtk_cell_renderer_toggle_new();
	g_signal_connect(G_OBJECT(renderer), "toggled", G_CALLBACK(contacts_invisible_cb), dialog); /* privacy laters: connect signal */
	column = gtk_tree_view_column_new_with_attributes(_("Invisible"), renderer, "active", CONTACTS_COLUMN_INVISIBLE, NULL);
	gtk_tree_view_column_set_resizable(column, FALSE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

	/* Block Chat */
	renderer = gtk_cell_renderer_toggle_new();
	g_signal_connect(G_OBJECT(renderer), "toggled", G_CALLBACK(contacts_block_chat_cb), dialog); /* privacy laters: connect signal */
	column = gtk_tree_view_column_new_with_attributes(_("Block Chat"), renderer, "active", CONTACTS_COLUMN_BLOCK_CHAT, NULL);
	gtk_tree_view_column_set_resizable(column, FALSE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

	/* Block FT */
	renderer = gtk_cell_renderer_toggle_new();
	g_signal_connect(G_OBJECT(renderer), "toggled", G_CALLBACK(contacts_block_ft_cb), dialog); /* privacy laters: connect signal */
	column = gtk_tree_view_column_new_with_attributes(_("Block File Transfer"), renderer, "active", CONTACTS_COLUMN_BLOCK_FT, NULL);
	gtk_tree_view_column_set_resizable(column, FALSE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

	/* Block CONF */
	renderer = gtk_cell_renderer_toggle_new();
	g_signal_connect(G_OBJECT(renderer), "toggled", G_CALLBACK(contacts_block_conf_cb), dialog); /* privacy laters: connect signal */
	column = gtk_tree_view_column_new_with_attributes(_("Block Conference"), renderer, "active", CONTACTS_COLUMN_BLOCK_CONF, NULL);
	gtk_tree_view_column_set_resizable(column, FALSE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
}

/* Callback to add a contact */
static void add_contact_cb(GtkWidget *button, PidginPrivacyDialog *dialog)
{
}

/* Create scroll box - Contacts and add contact button */
static void show_contacts(PidginPrivacyDialog *dialog)
{
	GtkWidget *treeview;
	GtkWidget *align;

	dialog->sw_contacts = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(dialog->sw_contacts), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(dialog->sw_contacts), GTK_SHADOW_IN);
	gtk_box_pack_start(GTK_BOX(dialog->vbox_pas), dialog->sw_contacts, TRUE, TRUE, 0);
	gtk_widget_show(dialog->sw_contacts);

	/* Add items to the scroll box - Contacts */
	dialog->model_contacts = gtk_list_store_new(CONTACTS_NUM_COLUMNS,
					G_TYPE_STRING,		/* CONTACTS_COLUMN_BUDDYNAME */
					G_TYPE_BOOLEAN,		/* CONTACTS_COLUMN_BLOCK_ALL */
					G_TYPE_BOOLEAN,		/* CONTACTS_COLUMN_INVISIBLE */
					G_TYPE_BOOLEAN,		/* CONTACTS_COLUMN_BLOCK_CHAT */
					G_TYPE_BOOLEAN,		/* CONTACTS_COLUMN_BLOCK_FT */
					G_TYPE_BOOLEAN,		/* CONTACTS_COLUMN_BLOCK_CONF */
					G_TYPE_POINTER		/* CONTACTS_COLUMN_DATA */
					);

	treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(dialog->model_contacts));
	gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(treeview), TRUE);
	gtk_widget_set_size_request(treeview, -1, 150);
	gtk_container_add(GTK_CONTAINER(dialog->sw_contacts), treeview);

	add_columns_contacts(treeview, dialog);
	gtk_tree_view_columns_autosize(GTK_TREE_VIEW(treeview));
	populate_contacts_list(dialog);

	gtk_widget_show_all(treeview);
	gtk_widget_show_all(treeview);

	/* Add contact button */
	align = gtk_alignment_new(1,1,0,0);
	gtk_box_pack_start(GTK_BOX(dialog->vbox_pas), align, FALSE, FALSE, 0);
	dialog->add_contact_button = gtk_button_new_from_stock(GTK_STOCK_ADD);
	g_signal_connect(G_OBJECT(dialog->add_contact_button), "clicked", G_CALLBACK(add_contact_cb), dialog);
	gtk_container_add(GTK_CONTAINER(align), dialog->add_contact_button);
	gtk_widget_show(dialog->add_contact_button);
	gtk_widget_show(align);

	dialog->show_sw_contacts = TRUE;
}

static PidginPrivacyDialog *
privacy_dialog_new(void)
{
	PidginPrivacyDialog *dialog;
	GtkWidget *vbox;
	GtkWidget *label;
	GtkWidget *radio = NULL;
	GtkWidget *vbox_radio;
	GtkWidget *presence_expander;
	GtkWidget *vbox_presence_expander;
	GtkWidget *sw_presence;
	GtkWidget *treeview_presence;
	GtkWidget *dropdown;
	GtkWidget *menu;
	int selected = 0;
	int i;
	PurplePrivacySetting state_global;

	GtkWidget *button;

	dialog = g_new0(PidginPrivacyDialog, 1);

	dialog->win = pidgin_create_dialog(_("Privacy"), PIDGIN_HIG_BORDER , "privacy", TRUE);

	g_signal_connect(G_OBJECT(dialog->win), "delete_event", G_CALLBACK(destroy_cb), dialog);

	/* Main vbox */
	vbox = pidgin_dialog_get_vbox_with_properties(GTK_DIALOG(dialog->win), FALSE, PIDGIN_HIG_BORDER);

	/* Description label */
	label = gtk_label_new(_("Changes to privacy settings take effect immediately."));

	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_widget_show(label);

	/* Radio buttons */
	vbox_radio = gtk_vbox_new(FALSE, PIDGIN_HIG_BOX_SPACE);
	gtk_box_pack_start(GTK_BOX(vbox), vbox_radio, FALSE, FALSE, 0);
	state_global = purple_privacy_obtain_global_state();

	radio = gtk_radio_button_new_with_label(NULL, _("Allow all users"));
	gtk_box_pack_start(GTK_BOX(vbox_radio), radio, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(radio), "toggled", G_CALLBACK(toggle_button_cb), (gpointer) dialog);
	g_object_set_data(G_OBJECT(radio), "choice_id", GINT_TO_POINTER(PURPLE_PRIVACY_ALLOW_ALL));
	if(state_global == PURPLE_PRIVACY_ALLOW_ALL)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio), TRUE);
	gtk_widget_show(radio);

	radio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio), _("Block messages from the users not in the buddy list"));
	gtk_box_pack_start(GTK_BOX(vbox_radio), radio, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(radio), "toggled", G_CALLBACK(toggle_button_cb), (gpointer) dialog);
	g_object_set_data(G_OBJECT(radio), "choice_id", GINT_TO_POINTER(PURPLE_PRIVACY_BLOCK_MSG_NONBUDDY));
	if(state_global == PURPLE_PRIVACY_BLOCK_MSG_NONBUDDY)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio), TRUE);
	gtk_widget_show(radio);

	radio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio), _("Block users not in the buddy list"));
	gtk_box_pack_start(GTK_BOX(vbox_radio), radio, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(radio), "toggled", G_CALLBACK(toggle_button_cb), (gpointer) dialog);
	g_object_set_data(G_OBJECT(radio), "choice_id", GINT_TO_POINTER(PURPLE_PRIVACY_ALLOW_BUDDYLIST));
	if(state_global == PURPLE_PRIVACY_ALLOW_BUDDYLIST)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio), TRUE);
	gtk_widget_show(radio);

	radio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(radio), _("Per account settings"));
	gtk_box_pack_start(GTK_BOX(vbox_radio), radio, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(radio), "toggled", G_CALLBACK(toggle_button_cb), (gpointer) dialog);
	g_object_set_data(G_OBJECT(radio), "choice_id", GINT_TO_POINTER(PURPLE_PRIVACY_CUSTOM));
	if(state_global == PURPLE_PRIVACY_CUSTOM)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio), TRUE);
	gtk_widget_show(radio);

	/* Prepare box for widgets under "Per account settings (pas) */
	dialog->vbox_pas = gtk_vbox_new(FALSE, PIDGIN_HIG_BOX_SPACE/2);
	gtk_box_pack_start(GTK_BOX(vbox_radio), dialog->vbox_pas, FALSE, FALSE, 0);

	gtk_widget_show(vbox_radio);

	/* Prepare widgets under PAS */
	/* Accounts drop-down */
	dropdown = pidgin_account_option_menu_new(NULL, FALSE, G_CALLBACK(select_account_cb), NULL, dialog);
	pidgin_add_widget_to_vbox(GTK_BOX(dialog->vbox_pas), _("Set privacy for:"), NULL, dropdown, TRUE, NULL);
	dialog->account = pidgin_account_option_menu_get_selected(dropdown);

	/* Privacy State drop down */
	dialog->type_menu = gtk_option_menu_new();
	pidgin_add_widget_to_vbox(GTK_BOX(dialog->vbox_pas), _("Privacy State: "), NULL, dialog->type_menu, TRUE, NULL);
	menu = gtk_menu_new();

	if(state_global == PURPLE_PRIVACY_CUSTOM)
		gtk_widget_show(dialog->vbox_pas);

	for (i = 0; i < menu_entry_count; i++) {
		pidgin_new_item(menu, _(menu_entries[i].text));
		if ( (menu_entries[i].num == purple_privacy_obtain_account_state(dialog->account)) )
			selected = i;
	}

	gtk_option_menu_set_menu(GTK_OPTION_MENU(dialog->type_menu), menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(dialog->type_menu), selected);
	g_signal_connect(G_OBJECT(dialog->type_menu), "changed", G_CALLBACK(type_changed_cb), dialog);

	/* If custom options selected: Construct and show contacts scroll window, add contact button */
	dialog->show_sw_contacts = FALSE;	/* Currently we are not showing contacts scroll window */
	if( menu_entries[selected].num == PURPLE_PRIVACY_CUSTOM )
		type_changed_cb(GTK_OPTION_MENU(dialog->type_menu), dialog);

	/* Expander for Presence */
	presence_expander = gtk_expander_new(_("Presence Settings"));
	gtk_box_pack_start(GTK_BOX(vbox), presence_expander, FALSE, FALSE, 0);
	gtk_widget_show(presence_expander);
	vbox_presence_expander = gtk_vbox_new(FALSE, 20);
	gtk_container_add(GTK_CONTAINER(presence_expander), vbox_presence_expander);
	gtk_widget_show(presence_expander);
	
	/* Create scroll box for Presence Settings */
	sw_presence = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw_presence), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw_presence), GTK_SHADOW_IN);
	gtk_box_pack_start(GTK_BOX(vbox_presence_expander), sw_presence, TRUE, TRUE, 0);
	gtk_widget_show(sw_presence);
	gtk_widget_show(vbox_presence_expander);

	/* Add items to the scroll box - (SOPNL)*/
	dialog->model_presence = gtk_list_store_new(PRESENCE_NUM_COLUMNS,
					GDK_TYPE_PIXBUF,   /* PRESENCE_COLUMN_ICON */
					G_TYPE_STRING,     /* PRESENCE_COLUMN_USERNAME */
					G_TYPE_BOOLEAN,    /* PRESENCE_COLUMN_INV_NONBUD */
					G_TYPE_BOOLEAN,    /* PRESENCE_COLUMN_INV_ALL */
					G_TYPE_STRING,     /* PRESENCE_COLUMN_PROTOCOL */
					G_TYPE_POINTER     /* PRESENCE_COLUMN_DATA */
					);
	treeview_presence = gtk_tree_view_new_with_model(GTK_TREE_MODEL(dialog->model_presence));
	gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(treeview_presence), TRUE);
	gtk_widget_set_size_request(treeview_presence, -1, 150);
	gtk_container_add(GTK_CONTAINER(sw_presence), treeview_presence);

	add_columns_show_offline(treeview_presence, dialog);
	gtk_tree_view_columns_autosize(GTK_TREE_VIEW(treeview_presence));
	populate_accounts_list(dialog);
	gtk_widget_show_all(treeview_presence);

	/* Close button */
	button = pidgin_dialog_add_button(GTK_DIALOG(dialog->win), GTK_STOCK_CLOSE, G_CALLBACK(close_cb), dialog);
	dialog->close_button = button;

	return dialog;
}

void
pidgin_privacy_dialog_show(void)
{
	g_return_if_fail(purple_connections_get_all() != NULL);

	if (privacy_dialog == NULL)
		privacy_dialog = privacy_dialog_new();

	gtk_widget_show(privacy_dialog->win);
	gdk_window_raise(privacy_dialog->win->window);
}

void
pidgin_privacy_dialog_hide(void)
{
	if (privacy_dialog == NULL)
		return;

	/* privacy laters: un ref stores, free data */
	g_free(privacy_dialog);
	privacy_dialog = NULL;
}

static void
destroy_request_data(PidginPrivacyRequestData *data)
{
	g_free(data->name);
	g_free(data);
}

/* privacy laters: remove laters */

static void confirm_permit_block_cb(PidginPrivacyRequestData *data, int option)
{
}

static void add_permit_block_cb(PidginPrivacyRequestData *data, const char *name)
{
}
/* privacy laters */

void
pidgin_request_add_permit(PurpleAccount *account, const char *name)
{
	PidginPrivacyRequestData *data;

	g_return_if_fail(account != NULL);

	data = g_new0(PidginPrivacyRequestData, 1);
	data->account = account;
	data->name    = g_strdup(name);
	data->block   = FALSE;

	if (name == NULL) {
		purple_request_input(account, _("Permit User"),
			_("Type a user you permit to contact you."),
			_("Please enter the name of the user you wish to be "
			  "able to contact you."),
			NULL, FALSE, FALSE, NULL,
			_("_Permit"), G_CALLBACK(add_permit_block_cb),
			_("Cancel"), G_CALLBACK(destroy_request_data),
			account, name, NULL,
			data);
	}
	else {
		char *primary = g_strdup_printf(_("Allow %s to contact you?"), name);
		char *secondary =
			g_strdup_printf(_("Are you sure you wish to allow "
							  "%s to contact you?"), name);


		purple_request_action(account, _("Permit User"), primary, secondary,
							0,
							account, name, NULL,
							data, 2,
							_("_Permit"), G_CALLBACK(confirm_permit_block_cb),
							_("Cancel"), G_CALLBACK(destroy_request_data));

		g_free(primary);
		g_free(secondary);
	}
}

void
pidgin_request_add_block(PurpleAccount *account, const char *name)
{
	PidginPrivacyRequestData *data;

	g_return_if_fail(account != NULL);

	data = g_new0(PidginPrivacyRequestData, 1);
	data->account = account;
	data->name    = g_strdup(name);
	data->block   = TRUE;

	if (name == NULL) {
		purple_request_input(account, _("Block User"),
			_("Type a user to block."),
			_("Please enter the name of the user you wish to block."),
			NULL, FALSE, FALSE, NULL,
			_("_Block"), G_CALLBACK(add_permit_block_cb),
			_("Cancel"), G_CALLBACK(destroy_request_data),
			account, name, NULL,
			data);
	}
	else {
		char *primary = g_strdup_printf(_("Block %s?"), name);
		char *secondary =
			g_strdup_printf(_("Are you sure you want to block %s?"), name);

		purple_request_action(account, _("Block User"), primary, secondary,
							0,
							account, name, NULL,
							data, 2,
							_("_Block"), G_CALLBACK(confirm_permit_block_cb),
							_("Cancel"), G_CALLBACK(destroy_request_data));

		g_free(primary);
		g_free(secondary);
	}
}

static void
pidgin_permit_added_removed(PurpleAccount *account, const char *name)
{
	/*if (privacy_dialog != NULL)
		rebuild_allow_list(privacy_dialog); */
}

static void
pidgin_deny_added_removed(PurpleAccount *account, const char *name)
{
	/*if (privacy_dialog != NULL)
		rebuild_block_list(privacy_dialog); */
}

static PurplePrivacyUiOps privacy_ops =
{
	pidgin_permit_added_removed,
	pidgin_permit_added_removed,
	pidgin_deny_added_removed,
	pidgin_deny_added_removed,
	NULL,
	NULL,
	NULL,
	NULL
};

PurplePrivacyUiOps *
pidgin_privacy_get_ui_ops(void)
{
	return &privacy_ops;
}

void
pidgin_privacy_init(void)
{
}
