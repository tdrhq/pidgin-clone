/**
 * @file gtkaccount.c GTK+ Account Editor UI
 * @ingroup gtkui
 *
 * gaim
 *
 * Gaim is the legal property of its developers, whose names are too numerous
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

#include "internal.h"
#include "gtkgaim.h"

#include "account.h"
#include "accountopt.h"
#include "core.h"
#include "debug.h"
#include "notify.h"
#include "plugin.h"
#include "prefs.h"
#include "prpl.h"
#include "request.h"
#include "signals.h"
#include "util.h"

#include "gtkgaim-disclosure.h"
#include "gtkaccount.h"
#include "gtkblist.h"
#include "gtkdialogs.h"
#include "gtkutils.h"
#include "gtkstock.h"

enum
{
	COLUMN_ICON,
	COLUMN_SCREENNAME,
	COLUMN_ENABLED,
	COLUMN_PROTOCOL,
	COLUMN_DATA,
	COLUMN_PULSE_DATA,
	NUM_COLUMNS
};

typedef struct
{
	GaimAccount *account;
	char *username;
	char *alias;

} GaimGtkAccountAddUserData;

typedef struct
{
	GtkWidget *window;
	GtkWidget *treeview;

	GtkWidget *modify_button;
	GtkWidget *delete_button;

	GtkListStore *model;
	GtkTreeIter drag_iter;

	GtkTreeViewColumn *screenname_col;

	GHashTable *account_pref_wins;

} AccountsWindow;

typedef struct
{
	GaimGtkAccountDialogType type;

	GaimAccount *account;
	char *protocol_id;
	GaimPlugin *plugin;
	GaimPluginProtocolInfo *prpl_info;

	GaimProxyType new_proxy_type;

	GList *user_split_entries;
	GList *protocol_opt_entries;

	GtkSizeGroup *sg;
	GtkWidget *window;

	GtkWidget *top_vbox;
	GtkWidget *bottom_vbox;
	GtkWidget *ok_button;
	GtkWidget *register_button;

	/* Login Options */
	GtkWidget *login_frame;
	GtkWidget *protocol_menu;
	GtkWidget *password_box;
	GtkWidget *screenname_entry;
	GtkWidget *password_entry;
	GtkWidget *alias_entry;
	GtkWidget *remember_pass_check;

	/* User Options */
	GtkWidget *user_frame;
	GtkWidget *new_mail_check;
	GtkWidget *icon_hbox;
	GtkWidget *icon_entry;
	char *icon_path;
	GtkWidget *icon_filesel;
	GtkWidget *icon_preview;
	GtkWidget *icon_text;

	/* Protocol Options */
	GtkWidget *protocol_frame;

	/* Proxy Options */
	GtkWidget *proxy_frame;
	GtkWidget *proxy_vbox;
	GtkWidget *proxy_dropdown;
#if !GTK_CHECK_VERSION(2,4,0)
	GtkWidget *proxy_menu;
#endif
	GtkWidget *proxy_host_entry;
	GtkWidget *proxy_port_entry;
	GtkWidget *proxy_user_entry;
	GtkWidget *proxy_pass_entry;

} AccountPrefsDialog;

typedef struct
{
	GdkPixbuf *online_pixbuf;
	gboolean pulse_to_grey;
	float pulse_value;
	int timeout;
	GaimAccount *account;
	GtkTreeModel *model;

} GaimGtkPulseData;


static AccountsWindow *accounts_window = NULL;

static void add_account(AccountsWindow *dialog, GaimAccount *account);
static void set_account(GtkListStore *store, GtkTreeIter *iter,
						  GaimAccount *account);
static char*
convert_buddy_icon(GaimPlugin *plugin, const char *path);

/**************************************************************************
 * Add/Modify Account dialog
 **************************************************************************/
static void add_login_options(AccountPrefsDialog *dialog, GtkWidget *parent);
static void add_user_options(AccountPrefsDialog *dialog, GtkWidget *parent);
static void add_protocol_options(AccountPrefsDialog *dialog,
								 GtkWidget *parent);
static void add_proxy_options(AccountPrefsDialog *dialog, GtkWidget *parent);

static GtkWidget *
add_pref_box(AccountPrefsDialog *dialog, GtkWidget *parent,
			 const char *text, GtkWidget *widget)
{
	GtkWidget *hbox;
	GtkWidget *label;

	hbox = gtk_hbox_new(FALSE, 6);
	gtk_box_pack_start(GTK_BOX(parent), hbox, FALSE, FALSE, 0);
	gtk_widget_show(hbox);

	label = gtk_label_new_with_mnemonic(text);
	gtk_size_group_add_widget(dialog->sg, label);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
	gtk_widget_show(label);

	gtk_box_pack_start(GTK_BOX(hbox), widget, TRUE, TRUE, 0);
	gtk_widget_show(widget);
	gaim_set_accessible_label (widget, label);

	return hbox;
}

static void
set_account_protocol_cb(GtkWidget *item, const char *id,
						AccountPrefsDialog *dialog)
{
	GaimPlugin *new_plugin;

	new_plugin = gaim_find_prpl(id);

	if (new_plugin == dialog->plugin)
		return;

	dialog->plugin = new_plugin;

	if (dialog->plugin != NULL)
	{
		dialog->prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(dialog->plugin);

		if (dialog->protocol_id != NULL)
			g_free(dialog->protocol_id);

		dialog->protocol_id = g_strdup(dialog->plugin->info->id);
	}

	if (dialog->account != NULL)
		gaim_account_clear_settings(dialog->account);

	add_login_options(dialog,    dialog->top_vbox);
	add_user_options(dialog,     dialog->top_vbox);
	add_protocol_options(dialog, dialog->bottom_vbox);

	if (!dialog->prpl_info || !dialog->prpl_info->register_user)
		gtk_widget_hide(dialog->register_button);
	else
		gtk_widget_show(dialog->register_button);
}

static void
screenname_changed_cb(GtkEntry *entry, AccountPrefsDialog *dialog)
{
	if (dialog->ok_button)
		gtk_widget_set_sensitive(dialog->ok_button,
				*gtk_entry_get_text(entry) != '\0');
	if (dialog->register_button)
		gtk_widget_set_sensitive(dialog->register_button,
				*gtk_entry_get_text(entry) != '\0');

}

#if GTK_CHECK_VERSION(2,4,0) /* FILECHOOSER */
static void
icon_filesel_choose_cb(GtkWidget *widget, gint response, AccountPrefsDialog *dialog)
{
	const char *filename;

	if (response != GTK_RESPONSE_ACCEPT) {
		if (response == GTK_RESPONSE_CANCEL)
			gtk_widget_destroy(dialog->icon_filesel);
		dialog->icon_filesel = NULL;
		return;
	}

	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog->icon_filesel));
#else /* FILECHOOSER */
static void
icon_filesel_choose_cb(GtkWidget *w, AccountPrefsDialog *dialog)
{
	const char *filename;

	filename = gtk_file_selection_get_filename(
			GTK_FILE_SELECTION(dialog->icon_filesel));

	/* If they typed in a directory, change there */
	if (gaim_gtk_check_if_dir(filename,
			GTK_FILE_SELECTION(dialog->icon_filesel)))
	{
		return;
	}
#endif /* FILECHOOSER */

	if (dialog->icon_path)
		g_free(dialog->icon_path);
	dialog->icon_path = convert_buddy_icon(dialog->plugin, filename);
	gtk_image_set_from_file(GTK_IMAGE(dialog->icon_entry), dialog->icon_path);
	gtk_widget_show(dialog->icon_entry);

	gtk_widget_destroy(dialog->icon_filesel);
	dialog->icon_filesel = NULL;
 }

static void
#if GTK_CHECK_VERSION(2,4,0) /* FILECHOOSER */
icon_preview_change_cb(GtkFileChooser *widget, AccountPrefsDialog *dialog)
#else /* FILECHOOSER */
icon_preview_change_cb(GtkTreeSelection *sel, AccountPrefsDialog *dialog)
#endif /* FILECHOOSER */
{
	GdkPixbuf *pixbuf, *scale;
	int height, width;
	char *basename, *markup, *size;
	struct stat st;
	const char *filename;

#if GTK_CHECK_VERSION(2,4,0) /* FILECHOOSER */
	filename = gtk_file_chooser_get_preview_filename(
					GTK_FILE_CHOOSER(dialog->icon_filesel));
#else /* FILECHOOSER */
	filename = gtk_file_selection_get_filename(
		GTK_FILE_SELECTION(dialog->icon_filesel));
#endif /* FILECHOOSER */

	if (!filename || stat(filename, &st))
		return;

	pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
	if (!pixbuf) {
		gtk_image_set_from_pixbuf(GTK_IMAGE(dialog->icon_preview), NULL);
		gtk_label_set_markup(GTK_LABEL(dialog->icon_text), "");
#if GTK_CHECK_VERSION(2,4,0) /* FILECHOOSER */
		gtk_file_chooser_set_preview_widget_active(
					GTK_FILE_CHOOSER(dialog->icon_filesel), FALSE);
#endif /* FILECHOOSER */
		return;
	}

	width = gdk_pixbuf_get_width(pixbuf);
	height = gdk_pixbuf_get_height(pixbuf);
	basename = g_path_get_basename(filename);
	size = gaim_str_size_to_units(st.st_size);
	markup = g_strdup_printf(_("<b>File:</b> %s\n"
							   "<b>File size:</b> %s\n"
							   "<b>Image size:</b> %dx%d"),
							 basename, size, width, height);

	scale = gdk_pixbuf_scale_simple(pixbuf, width * 50 / height,
									50, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(dialog->icon_preview), scale);
#if GTK_CHECK_VERSION(2,4,0) /* FILECHOOSER */
	gtk_file_chooser_set_preview_widget_active(
					GTK_FILE_CHOOSER(dialog->icon_filesel), TRUE);
#endif /* FILECHOOSER */
	gtk_label_set_markup(GTK_LABEL(dialog->icon_text), markup);

	g_object_unref(G_OBJECT(pixbuf));
	g_object_unref(G_OBJECT(scale));
	g_free(basename);
	g_free(size);
	g_free(markup);
}

#if !GTK_CHECK_VERSION(2,4,0) /* FILECHOOSER */
static void
icon_filesel_delete_cb(GtkWidget *w, AccountPrefsDialog *dialog)
{
	if (dialog->icon_filesel != NULL)
		gtk_widget_destroy(dialog->icon_filesel);

	dialog->icon_filesel = NULL;
}
#endif /* FILECHOOSER */

static void
icon_select_cb(GtkWidget *button, AccountPrefsDialog *dialog)
{
#if !GTK_CHECK_VERSION(2,4,0) /* FILECHOOSER */
	GtkWidget *hbox;
	GtkWidget *tv;
	GtkTreeSelection *sel;
#endif /* FILECHOOSER */

	if (dialog->icon_filesel != NULL) {
		gtk_window_present(GTK_WINDOW(dialog->icon_filesel));
		return;
	}

#if GTK_CHECK_VERSION(2,4,0) /* FILECHOOSER */
	dialog->icon_filesel = gtk_file_chooser_dialog_new(_("Buddy Icon"),
									GTK_WINDOW(dialog->window),
									GTK_FILE_CHOOSER_ACTION_OPEN,
									GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
									GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
									NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(dialog->icon_filesel), GTK_RESPONSE_ACCEPT);


	/*  I removed code to set the current path to the current icon in the old file selector so I figure
	 * it shouldn't be here either.  The reason is because the icon will potentially converted and won't
	 * be anything near what the user selected last time (which is the advantage to doing it this way. 
	 * The solution is to create a new pref to specify the last chosen buddy icon.  This would also have the
	 * advantage (?) of not being account-specific. 
	if (dialog->icon_path != NULL)
		gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog->icon_filesel),
			dialog->icon_path);
	*/

	dialog->icon_preview = gtk_image_new();
	dialog->icon_text = gtk_label_new(NULL);
	gtk_widget_set_size_request(GTK_WIDGET(dialog->icon_preview), -1, 50);
	gtk_file_chooser_set_preview_widget(GTK_FILE_CHOOSER(dialog->icon_filesel), 
					    GTK_WIDGET(dialog->icon_preview));
	g_signal_connect(G_OBJECT(dialog->icon_filesel), "update-preview",
					 G_CALLBACK(icon_preview_change_cb), dialog);
	g_signal_connect(G_OBJECT(dialog->icon_filesel), "response",
					 G_CALLBACK(icon_filesel_choose_cb), dialog);
	icon_preview_change_cb(NULL, dialog);
#else /* FILECHOOSER */
	dialog->icon_filesel = gtk_file_selection_new(_("Buddy Icon"));
	dialog->icon_preview = gtk_image_new();
	dialog->icon_text = gtk_label_new(NULL);

	gtk_widget_set_size_request(GTK_WIDGET(dialog->icon_preview), -1, 50);
	hbox = gtk_hbox_new(FALSE, 6);
	gtk_box_pack_start(
		GTK_BOX(GTK_FILE_SELECTION(dialog->icon_filesel)->main_vbox),
		hbox, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(hbox), dialog->icon_preview,
					 FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(hbox), dialog->icon_text, FALSE, FALSE, 0);

	tv = GTK_FILE_SELECTION(dialog->icon_filesel)->file_list;
	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tv));

	g_signal_connect(G_OBJECT(sel), "changed",
					 G_CALLBACK(icon_preview_change_cb), dialog);
	g_signal_connect(
		G_OBJECT(GTK_FILE_SELECTION(dialog->icon_filesel)->ok_button),
		"clicked",
		G_CALLBACK(icon_filesel_choose_cb), dialog);
	g_signal_connect(
		G_OBJECT(GTK_FILE_SELECTION(dialog->icon_filesel)->cancel_button),
		"clicked",
		G_CALLBACK(icon_filesel_delete_cb), dialog);
	g_signal_connect(G_OBJECT(dialog->icon_filesel), "destroy",
					 G_CALLBACK(icon_filesel_delete_cb), dialog);
#endif /* FILECHOOSER */

	gtk_widget_show_all(GTK_WIDGET(dialog->icon_filesel));
}

static void
icon_reset_cb(GtkWidget *button, AccountPrefsDialog *dialog)
{
	if (dialog->icon_path)
		g_free(dialog->icon_path);
	dialog->icon_path = NULL;

	gtk_widget_hide(dialog->icon_entry);
}


static void
account_dnd_recv(GtkWidget *widget, GdkDragContext *dc, gint x, gint y,
		 GtkSelectionData *sd, guint info, guint t, AccountPrefsDialog *dialog)
{
	gchar *name = sd->data;
	
	if ((sd->length >= 0) && (sd->format == 8)) {
		/* Well, it looks like the drag event was cool. 
		 * Let's do something with it */
		if (!g_ascii_strncasecmp(name, "file://", 7)) {
			GError *converr = NULL;
			gchar *tmp, *rtmp;
			/* It looks like we're dealing with a local file. Let's 
			 * just untar it in the right place */
			if(!(tmp = g_filename_from_uri(name, NULL, &converr))) {
				gaim_debug(GAIM_DEBUG_ERROR, "buddyicon", "%s\n",
					   (converr ? converr->message :
					    "g_filename_from_uri error"));
				return;
			}
			if ((rtmp = strchr(tmp, '\r')) || (rtmp = strchr(tmp, '\n')))
				*rtmp = '\0';
			if (dialog->icon_path)
				g_free(dialog->icon_path);
			dialog->icon_path = convert_buddy_icon(dialog->plugin, tmp);
			gtk_image_set_from_file(GTK_IMAGE(dialog->icon_entry), dialog->icon_path);
			gtk_widget_show(dialog->icon_entry);
			g_free(tmp);
		} 
		gtk_drag_finish(dc, TRUE, FALSE, t);
	}
	gtk_drag_finish(dc, FALSE, FALSE, t);
}


#if GTK_CHECK_VERSION(2,2,0)
static gboolean
str_array_match(char **a, char **b)
{
	int i, j;

	if (!a || !b)
		return FALSE;
	for (i = 0; a[i] != NULL; i++)
		for (j = 0; b[j] != NULL; j++)
			if (!g_ascii_strcasecmp(a[i], b[j]))
				return TRUE;
	return FALSE;
}
#endif

static char*
convert_buddy_icon(GaimPlugin *plugin, const char *path)
{
#if GTK_CHECK_VERSION(2,2,0)
	int width, height;
	char **pixbuf_formats = NULL;
	GdkPixbufFormat *format;
	GdkPixbuf *pixbuf;
	GaimPluginProtocolInfo *prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(plugin);
	char **prpl_formats =  g_strsplit (prpl_info->icon_spec.format,",",0);
#if !GTK_CHECK_VERSION(2,4,0)
	GdkPixbufLoader *loader;
	FILE *file;
	struct stat st;
	void *data = NULL;
#endif

#if GTK_CHECK_VERSION(2,4,0)
	format = gdk_pixbuf_get_file_info (path, &width, &height);
#else
	loader = gdk_pixbuf_loader_new();
	if (!stat(path, &st) && (file = fopen(path, "rb")) != NULL) {
		data = g_malloc(st.st_size);
		fread(data, 1, st.st_size, file);
		fclose(file);
		gdk_pixbuf_loader_write(loader, data, st.st_size, NULL);
		g_free(data);
	}
	pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);
	width = gdk_pixbuf_get_width(pixbuf);
	height = gdk_pixbuf_get_height(pixbuf);
	format = gdk_pixbuf_loader_get_format(loader);
	gdk_pixbuf_loader_close(loader, NULL);
	g_object_unref(G_OBJECT(loader));
#endif
	pixbuf_formats =  gdk_pixbuf_format_get_extensions(format);

	if (str_array_match(pixbuf_formats, prpl_formats) &&                                 /* This is an acceptable format AND */
		 (!(prpl_info->icon_spec.scale_rules & GAIM_ICON_SCALE_SEND) ||                   /* The prpl doesn't scale before it sends OR */
		  (prpl_info->icon_spec.min_width <= width &&
		   prpl_info->icon_spec.max_width >= width &&
		   prpl_info->icon_spec.min_height <= height &&
		   prpl_info->icon_spec.max_height >= height))) {                                  /* The icon is the correct size */
#endif
		return g_strdup(path);
#if GTK_CHECK_VERSION(2,2,0)
	} else {
		int i;
		GError *error = NULL;
		GdkPixbuf *scale;
		char *random   = g_strdup_printf("%x", g_random_int());
		const char *dirname = gaim_buddy_icons_get_cache_dir();
		char *filename = g_build_filename(dirname, random, NULL);
		pixbuf = gdk_pixbuf_new_from_file(path, &error);
		if (!error && (prpl_info->icon_spec.scale_rules & GAIM_ICON_SCALE_SEND) &&
			(width < prpl_info->icon_spec.min_width ||
			 width > prpl_info->icon_spec.max_width ||
			 height < prpl_info->icon_spec.min_height ||
			 height > prpl_info->icon_spec.max_height))
		{
			int new_width = gdk_pixbuf_get_width(pixbuf);
			int new_height = gdk_pixbuf_get_height(pixbuf);

			if(new_width > prpl_info->icon_spec.max_width)
				new_width = prpl_info->icon_spec.max_width;
			else if(new_width < prpl_info->icon_spec.min_width)
				new_width = prpl_info->icon_spec.min_width;
			if(new_height > prpl_info->icon_spec.max_height)
				new_height = prpl_info->icon_spec.max_height;
			else if(new_height < prpl_info->icon_spec.min_height)
				new_height = prpl_info->icon_spec.min_height;

			scale = gdk_pixbuf_scale_simple (pixbuf, new_width, new_height,
					GDK_INTERP_HYPER);
			g_object_unref(G_OBJECT(pixbuf));
			pixbuf = scale;
		}
		if (error) {
			g_free(filename);
			g_free(random);
			gaim_debug_error("buddyicon", "Could not open icon for conversion: %s\n", error->message);
			g_error_free(error);
			return NULL;
		}

		if (!g_file_test(dirname, G_FILE_TEST_IS_DIR)) {
			gaim_debug_info("buddyicon", "Creating icon cache directory.\n");

			if (mkdir(dirname, S_IRUSR | S_IWUSR | S_IXUSR) < 0) {
				gaim_debug_error("buddyicon",
								 "Unable to create directory %s: %s\n",
								 dirname, strerror(errno));
				return NULL;
			}
		}

		for (i = 0; prpl_formats[i]; i++) {
			gaim_debug_info("buddyicon", "Converting buddy icon to %s as %s\n", prpl_formats[i], filename);
			/* The gdk-pixbuf documentation is wrong. gdk_pixbuf_save returns TRUE if it was successful,
			 * FALSE if an error was set. */
			if (gdk_pixbuf_save (pixbuf, filename, prpl_formats[i], &error, NULL) == TRUE)
					break;
		}
		if (!error) {
			g_free(random);
			g_object_unref(G_OBJECT(pixbuf));
			return filename;
		} else {
			gaim_debug_error("buddyicon", "Could not convert icon to usable format: %s\n", error->message);
			g_error_free(error);
		}
		g_free(filename);
		g_free(random);
		g_object_unref(G_OBJECT(pixbuf));
	}
	return NULL;
#endif
}

static void
add_login_options(AccountPrefsDialog *dialog, GtkWidget *parent)
{
	GtkWidget *frame;
	GtkWidget *vbox;
	GtkWidget *entry;
	GList *user_splits;
	GList *l, *l2;
	char *username = NULL;

	if (dialog->login_frame != NULL)
		gtk_widget_destroy(dialog->login_frame);

	/* Build the login options frame. */
	frame = gaim_gtk_make_frame(parent, _("Login Options"));

	/* cringe */
	dialog->login_frame = gtk_widget_get_parent(gtk_widget_get_parent(frame));

	gtk_box_reorder_child(GTK_BOX(parent), dialog->login_frame, 0);
	gtk_widget_show(dialog->login_frame);

	/* Main vbox */
	vbox = gtk_vbox_new(FALSE, 6);
	gtk_container_add(GTK_CONTAINER(frame), vbox);
	gtk_widget_show(vbox);

	/* Protocol */
	dialog->protocol_menu = gaim_gtk_protocol_option_menu_new(
			dialog->protocol_id, G_CALLBACK(set_account_protocol_cb), dialog);

	add_pref_box(dialog, vbox, _("Protocol:"), dialog->protocol_menu);

	/* Screen Name */
	dialog->screenname_entry = gtk_entry_new();

	add_pref_box(dialog, vbox, _("Screen Name:"), dialog->screenname_entry);

	g_signal_connect(G_OBJECT(dialog->screenname_entry), "changed",
			 G_CALLBACK(screenname_changed_cb), dialog);

	/* Do the user split thang */
	if (dialog->plugin == NULL) /* Yeah right. */
		user_splits = NULL;
	else
		user_splits = dialog->prpl_info->user_splits;

	if (dialog->account != NULL)
		username = g_strdup(gaim_account_get_username(dialog->account));

	if (dialog->user_split_entries != NULL) {
		g_list_free(dialog->user_split_entries);
		dialog->user_split_entries = NULL;
	}

	for (l = user_splits; l != NULL; l = l->next) {
		GaimAccountUserSplit *split = l->data;
		char *buf;

		buf = g_strdup_printf("%s:", gaim_account_user_split_get_text(split));

		entry = gtk_entry_new();

		add_pref_box(dialog, vbox, buf, entry);

		g_free(buf);

		dialog->user_split_entries =
			g_list_append(dialog->user_split_entries, entry);
	}

	for (l = g_list_last(dialog->user_split_entries),
		 l2 = g_list_last(user_splits);
		 l != NULL && l2 != NULL;
		 l = l->prev, l2 = l2->prev) {

		GtkWidget *entry = l->data;
		GaimAccountUserSplit *split = l2->data;
		const char *value = NULL;
		char *c;

		if (dialog->account != NULL) {
			c = strrchr(username,
						gaim_account_user_split_get_separator(split));

			if (c != NULL) {
				*c = '\0';
				c++;

				value = c;
			}
		}

		if (value == NULL)
			value = gaim_account_user_split_get_default_value(split);

		if (value != NULL)
			gtk_entry_set_text(GTK_ENTRY(entry), value);
	}

	if (username != NULL)
		gtk_entry_set_text(GTK_ENTRY(dialog->screenname_entry), username);

	g_free(username);


	/* Password */
	dialog->password_entry = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(dialog->password_entry), FALSE);
	dialog->password_box = add_pref_box(dialog, vbox, _("Password:"),
										  dialog->password_entry);

	/* Alias */
	dialog->alias_entry = gtk_entry_new();
	add_pref_box(dialog, vbox, _("Alias:"), dialog->alias_entry);

	/* Remember Password */
	dialog->remember_pass_check =
		gtk_check_button_new_with_label(_("Remember password"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(dialog->remember_pass_check),
								 FALSE);
	gtk_box_pack_start(GTK_BOX(vbox), dialog->remember_pass_check,
					   FALSE, FALSE, 0);
	gtk_widget_show(dialog->remember_pass_check);

	/* Set the fields. */
	if (dialog->account != NULL) {
		if (gaim_account_get_password(dialog->account))
			gtk_entry_set_text(GTK_ENTRY(dialog->password_entry),
							   gaim_account_get_password(dialog->account));

		if (gaim_account_get_alias(dialog->account))
			gtk_entry_set_text(GTK_ENTRY(dialog->alias_entry),
							   gaim_account_get_alias(dialog->account));

		gtk_toggle_button_set_active(
				GTK_TOGGLE_BUTTON(dialog->remember_pass_check),
				gaim_account_get_remember_password(dialog->account));
	}

	if (dialog->prpl_info != NULL &&
		(dialog->prpl_info->options & OPT_PROTO_NO_PASSWORD)) {

		gtk_widget_hide(dialog->password_box);
		gtk_widget_hide(dialog->remember_pass_check);
	}
}

static void
add_user_options(AccountPrefsDialog *dialog, GtkWidget *parent)
{
	GtkWidget *frame;
	GtkWidget *vbox;
	GtkWidget *vbox2;
	GtkWidget *hbox;
	GtkWidget *hbox2;
	GtkWidget *button;
	GtkWidget *label;

	if (dialog->user_frame != NULL)
		gtk_widget_destroy(dialog->user_frame);

	/* Build the user options frame. */
	frame = gaim_gtk_make_frame(parent, _("User Options"));
	dialog->user_frame = gtk_widget_get_parent(gtk_widget_get_parent(frame));

	gtk_box_reorder_child(GTK_BOX(parent), dialog->user_frame, 1);
	gtk_widget_show(dialog->user_frame);

	/* Main vbox */
	vbox = gtk_vbox_new(FALSE, 6);
	gtk_container_add(GTK_CONTAINER(frame), vbox);
	gtk_widget_show(vbox);

	/* New mail notifications */
	dialog->new_mail_check =
		gtk_check_button_new_with_label(_("New mail notifications"));
	gtk_box_pack_start(GTK_BOX(vbox), dialog->new_mail_check, FALSE, FALSE, 0);
	gtk_widget_show(dialog->new_mail_check);

	/* Buddy icon */
	dialog->icon_hbox = hbox = gtk_hbox_new(FALSE, 6);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
	gtk_widget_show(hbox);

	label = gtk_label_new(_("Buddy icon:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.0);
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
	gtk_widget_show(label);

	dialog->icon_entry = gtk_image_new();
	gtk_box_pack_start(GTK_BOX(hbox), dialog->icon_entry,
					   FALSE, FALSE, 0);
	gtk_widget_show(dialog->icon_entry);
	gaim_set_accessible_label (dialog->icon_entry, label);
	dialog->icon_path = NULL;

	vbox2 = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), vbox2, TRUE, TRUE, 0);
	gtk_widget_show(vbox2);

	hbox2 = gtk_hbox_new(FALSE, 6);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox2, FALSE, FALSE, 0);
	gtk_widget_show(hbox2);

	button = gtk_button_new_from_stock(GTK_STOCK_OPEN);
	gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(icon_select_cb), dialog);
	gtk_widget_show(button);

	button = gtk_button_new_from_stock(GTK_STOCK_REMOVE);
	g_signal_connect(G_OBJECT(button), "clicked",
			 G_CALLBACK(icon_reset_cb), dialog);
	gtk_box_pack_start(GTK_BOX(hbox2), button, FALSE, FALSE, 0);
	gtk_widget_show(button);

	if (dialog->prpl_info != NULL) {
		if (!(dialog->prpl_info->options & OPT_PROTO_MAIL_CHECK))
			gtk_widget_hide(dialog->new_mail_check);

		if (!(dialog->prpl_info->icon_spec.format != NULL))
			gtk_widget_hide(dialog->icon_hbox);
	}

	if (dialog->account != NULL) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(dialog->new_mail_check),
				gaim_account_get_check_mail(dialog->account));

		if (gaim_account_get_buddy_icon(dialog->account) != NULL) {
			dialog->icon_path = g_strdup(gaim_account_get_buddy_icon(dialog->account));
			gtk_image_set_from_file(GTK_IMAGE(dialog->icon_entry),dialog->icon_path);
		}
	}

	if (!dialog->prpl_info ||
			(!(dialog->prpl_info->options & OPT_PROTO_MAIL_CHECK) &&
			 (dialog->prpl_info->icon_spec.format ==  NULL))) {

		/* Nothing to see :( aww. */
		gtk_widget_hide(dialog->user_frame);
	}
}

static void
add_protocol_options(AccountPrefsDialog *dialog, GtkWidget *parent)
{
	GaimAccountOption *option;
	GaimAccount *account;
	GtkWidget *frame;
	GtkWidget *vbox;
	GtkWidget *check;
	GtkWidget *entry;
	GList *l;
	char buf[1024];
	char *title;
	const char *str_value;
	gboolean bool_value;
	int int_value;

	if (dialog->protocol_frame != NULL) {
		gtk_widget_destroy(dialog->protocol_frame);
		dialog->protocol_frame = NULL;
	}

	if (dialog->prpl_info == NULL ||
		dialog->prpl_info->protocol_options == NULL) {

		return;
	}

	account = dialog->account;

	/* Build the protocol options frame. */
	g_snprintf(buf, sizeof(buf), _("%s Options"), dialog->plugin->info->name);

	frame = gaim_gtk_make_frame(parent, buf);
	dialog->protocol_frame =
		gtk_widget_get_parent(gtk_widget_get_parent(frame));

	gtk_box_reorder_child(GTK_BOX(parent), dialog->protocol_frame, 0);
	gtk_widget_show(dialog->protocol_frame);

	/* Main vbox */
	vbox = gtk_vbox_new(FALSE, 6);
	gtk_container_add(GTK_CONTAINER(frame), vbox);
	gtk_widget_show(vbox);

	if (dialog->protocol_opt_entries != NULL) {
		g_list_free(dialog->protocol_opt_entries);
		dialog->protocol_opt_entries = NULL;
	}

	for (l = dialog->prpl_info->protocol_options; l != NULL; l = l->next)
	{
		option = (GaimAccountOption *)l->data;

		switch (gaim_account_option_get_type(option))
		{
			case GAIM_PREF_BOOLEAN:
				if (account == NULL ||
					strcmp(gaim_account_get_protocol_id(account),
						   dialog->protocol_id))
				{
					bool_value = gaim_account_option_get_default_bool(option);
				}
				else
				{
					bool_value = gaim_account_get_bool(account,
						gaim_account_option_get_setting(option),
						gaim_account_option_get_default_bool(option));
				}

				check = gtk_check_button_new_with_label(
					gaim_account_option_get_text(option));

				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check),
											 bool_value);

				gtk_box_pack_start(GTK_BOX(vbox), check, FALSE, FALSE, 0);
				gtk_widget_show(check);

				dialog->protocol_opt_entries =
					g_list_append(dialog->protocol_opt_entries, check);

				break;

			case GAIM_PREF_INT:
				if (account == NULL ||
					strcmp(gaim_account_get_protocol_id(account),
						   dialog->protocol_id))
				{
					int_value = gaim_account_option_get_default_int(option);
				}
				else
				{
					int_value = gaim_account_get_int(account,
						gaim_account_option_get_setting(option),
						gaim_account_option_get_default_int(option));
				}

				g_snprintf(buf, sizeof(buf), "%d", int_value);

				entry = gtk_entry_new();
				gtk_entry_set_text(GTK_ENTRY(entry), buf);

				title = g_strdup_printf("%s:",
						gaim_account_option_get_text(option));

				add_pref_box(dialog, vbox, title, entry);

				g_free(title);

				dialog->protocol_opt_entries =
					g_list_append(dialog->protocol_opt_entries, entry);

				break;

			case GAIM_PREF_STRING:
				if (account == NULL ||
					strcmp(gaim_account_get_protocol_id(account),
						   dialog->protocol_id))
				{
					str_value = gaim_account_option_get_default_string(option);
				}
				else
				{
					str_value = gaim_account_get_string(account,
						gaim_account_option_get_setting(option),
						gaim_account_option_get_default_string(option));
				}

				entry = gtk_entry_new();

				if (str_value != NULL)
					gtk_entry_set_text(GTK_ENTRY(entry), str_value);

				title = g_strdup_printf("%s:",
						gaim_account_option_get_text(option));

				add_pref_box(dialog, vbox, title, entry);

				g_free(title);

				dialog->protocol_opt_entries =
					g_list_append(dialog->protocol_opt_entries, entry);

				break;

			default:
				break;
		}
	}
}

static GtkWidget *
make_proxy_dropdown(void)
{
	GtkWidget *dropdown;

#if GTK_CHECK_VERSION(2,4,0)
	GtkListStore *model;
	GtkTreeIter iter;
	GtkCellRenderer *renderer;

	model = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
	dropdown = gtk_combo_box_new_with_model(GTK_TREE_MODEL(model));

	gtk_list_store_append(model, &iter);
	gtk_list_store_set(model, &iter,
			0, _("Use Global Proxy Settings"),
			1, GAIM_PROXY_USE_GLOBAL,
			-1);

	gtk_list_store_append(model, &iter);
	gtk_list_store_set(model, &iter,
			0, _("No Proxy"),
			1, GAIM_PROXY_NONE,
			-1);

	gtk_list_store_append(model, &iter);
	gtk_list_store_set(model, &iter,
			0, _("HTTP"),
			1, GAIM_PROXY_HTTP,
			-1);

	gtk_list_store_append(model, &iter);
	gtk_list_store_set(model, &iter,
			0, _("SOCKS 4"),
			1, GAIM_PROXY_SOCKS4,
			-1);

	gtk_list_store_append(model, &iter);
	gtk_list_store_set(model, &iter,
			0, _("SOCKS 5"),
			1, GAIM_PROXY_SOCKS5,
			-1);

	gtk_list_store_append(model, &iter);
	gtk_list_store_set(model, &iter,
			0, _("Use Environmental Settings"),
			1, GAIM_PROXY_USE_ENVVAR,
			-1);

	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(dropdown), renderer, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(dropdown), renderer,
			"text", 0, NULL);

#else
	GtkWidget *menu;
	GtkWidget *item;

	dropdown = gtk_option_menu_new();
	menu = gtk_menu_new();

	/* Use Global Proxy Settings */
	item = gtk_menu_item_new_with_label(_("Use Global Proxy Settings"));
	g_object_set_data(G_OBJECT(item), "proxytype",
					  GINT_TO_POINTER(GAIM_PROXY_USE_GLOBAL));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_widget_show(item);

	/* No Proxy */
	item = gtk_menu_item_new_with_label(_("No Proxy"));
	g_object_set_data(G_OBJECT(item), "proxytype",
					  GINT_TO_POINTER(GAIM_PROXY_NONE));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_widget_show(item);

	/* HTTP */
	item = gtk_menu_item_new_with_label(_("HTTP"));
	g_object_set_data(G_OBJECT(item), "proxytype",
					  GINT_TO_POINTER(GAIM_PROXY_HTTP));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_widget_show(item);

	/* SOCKS 4 */
	item = gtk_menu_item_new_with_label(_("SOCKS 4"));
	g_object_set_data(G_OBJECT(item), "proxytype",
					  GINT_TO_POINTER(GAIM_PROXY_SOCKS4));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_widget_show(item);

	/* SOCKS 5 */
	item = gtk_menu_item_new_with_label(_("SOCKS 5"));
	g_object_set_data(G_OBJECT(item), "proxytype",
					  GINT_TO_POINTER(GAIM_PROXY_SOCKS5));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_widget_show(item);

	/* Use Environmental Settings */
	item = gtk_menu_item_new_with_label(_("Use Environmental Settings"));
	g_object_set_data(G_OBJECT(item), "proxytype",
					  GINT_TO_POINTER(GAIM_PROXY_USE_ENVVAR));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_widget_show(item);

	gtk_option_menu_set_menu(GTK_OPTION_MENU(dropdown), menu);
#endif

	return dropdown;
}

static void
proxy_type_changed_cb(GtkWidget *menu, AccountPrefsDialog *dialog)
{
#if GTK_CHECK_VERSION(2,4,0)
	dialog->new_proxy_type =
		gtk_combo_box_get_active(GTK_COMBO_BOX(menu)) - 1;
#else
	dialog->new_proxy_type =
		gtk_option_menu_get_history(GTK_OPTION_MENU(menu)) - 1;
#endif

	if (dialog->new_proxy_type == GAIM_PROXY_USE_GLOBAL ||
		dialog->new_proxy_type == GAIM_PROXY_NONE ||
		dialog->new_proxy_type == GAIM_PROXY_USE_ENVVAR) {

		gtk_widget_hide_all(dialog->proxy_vbox);
	}
	else
		gtk_widget_show_all(dialog->proxy_vbox);
}

static void
port_popup_cb(GtkWidget *w, GtkMenu *menu, gpointer data)
{
	GtkWidget *item;

	item = gtk_menu_item_new_with_label(
			_("you can see the butterflies mating"));
	gtk_widget_show(item);
	gtk_menu_shell_prepend(GTK_MENU_SHELL(menu), item);

	item = gtk_menu_item_new_with_label(_("If you look real closely"));
	gtk_widget_show(item);
	gtk_menu_shell_prepend(GTK_MENU_SHELL(menu), item);
}

static void
add_proxy_options(AccountPrefsDialog *dialog, GtkWidget *parent)
{
	GaimProxyInfo *proxy_info;
	GtkWidget *frame;
	GtkWidget *vbox;
	GtkWidget *vbox2;

	if (dialog->proxy_frame != NULL)
		gtk_widget_destroy(dialog->proxy_frame);

	frame = gaim_gtk_make_frame(parent, _("Proxy Options"));
	dialog->proxy_frame = gtk_widget_get_parent(gtk_widget_get_parent(frame));

	gtk_box_reorder_child(GTK_BOX(parent), dialog->proxy_frame, 1);
	gtk_widget_show(dialog->proxy_frame);

	/* Main vbox */
	vbox = gtk_vbox_new(FALSE, 6);
	gtk_container_add(GTK_CONTAINER(frame), vbox);
	gtk_widget_show(vbox);

	/* Proxy Type drop-down. */
	dialog->proxy_dropdown = make_proxy_dropdown();
#if !GTK_CHECK_VERSION(2,4,0)
	dialog->proxy_menu =
		gtk_option_menu_get_menu(GTK_OPTION_MENU(dialog->proxy_dropdown));
#endif

	add_pref_box(dialog, vbox, _("Proxy _type:"), dialog->proxy_dropdown);

	/* Setup the second vbox, which may be hidden at times. */
	dialog->proxy_vbox = vbox2 = gtk_vbox_new(FALSE, 6);
	gtk_box_pack_start(GTK_BOX(vbox), vbox2, FALSE, FALSE, 0);
	gtk_widget_show(vbox2);

	/* Host */
	dialog->proxy_host_entry = gtk_entry_new();
	add_pref_box(dialog, vbox2, _("_Host:"), dialog->proxy_host_entry);

	/* Port */
	dialog->proxy_port_entry = gtk_entry_new();
	add_pref_box(dialog, vbox2, _("_Port:"), dialog->proxy_port_entry);

	g_signal_connect(G_OBJECT(dialog->proxy_port_entry), "populate-popup",
					 G_CALLBACK(port_popup_cb), NULL);

	/* User */
	dialog->proxy_user_entry = gtk_entry_new();

	add_pref_box(dialog, vbox2, _("_Username:"), dialog->proxy_user_entry);

	/* Password */
	dialog->proxy_pass_entry = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(dialog->proxy_pass_entry), FALSE);
	add_pref_box(dialog, vbox2, _("Pa_ssword:"), dialog->proxy_pass_entry);

	if (dialog->account != NULL &&
		(proxy_info = gaim_account_get_proxy_info(dialog->account)) != NULL) {

		GaimProxyType type = gaim_proxy_info_get_type(proxy_info);

		/* Hah! */
		/* I dunno what you're laughing about, fuzz ball. */
		dialog->new_proxy_type = type;
#if GTK_CHECK_VERSION(2,4,0)
		gtk_combo_box_set_active(GTK_COMBO_BOX(dialog->proxy_dropdown),
				type + 1);
#else
		gtk_option_menu_set_history(GTK_OPTION_MENU(dialog->proxy_dropdown),
				(int)type + 1);
#endif

		if (type == GAIM_PROXY_USE_GLOBAL || type == GAIM_PROXY_NONE ||
			type == GAIM_PROXY_USE_ENVVAR) {
			gtk_widget_hide_all(vbox2);
		}
		else {
			const char *value;
			int int_val;

			if ((value = gaim_proxy_info_get_host(proxy_info)) != NULL)
				gtk_entry_set_text(GTK_ENTRY(dialog->proxy_host_entry), value);

			if ((int_val = gaim_proxy_info_get_port(proxy_info)) != 0) {
				char buf[32];

				g_snprintf(buf, sizeof(buf), "%d", int_val);

				gtk_entry_set_text(GTK_ENTRY(dialog->proxy_port_entry), buf);
			}

			if ((value = gaim_proxy_info_get_username(proxy_info)) != NULL)
				gtk_entry_set_text(GTK_ENTRY(dialog->proxy_user_entry), value);

			if ((value = gaim_proxy_info_get_password(proxy_info)) != NULL)
				gtk_entry_set_text(GTK_ENTRY(dialog->proxy_pass_entry), value);
		}
	}
	else {
		dialog->new_proxy_type = GAIM_PROXY_USE_GLOBAL;
#if GTK_CHECK_VERSION(2,4,0)
		gtk_combo_box_set_active(GTK_COMBO_BOX(dialog->proxy_dropdown),
				dialog->new_proxy_type + 1);
#else
		gtk_option_menu_set_history(GTK_OPTION_MENU(dialog->proxy_dropdown),
									dialog->new_proxy_type + 1);
#endif
		gtk_widget_hide_all(vbox2);
	}

	/* Connect signals. */
	g_signal_connect(G_OBJECT(dialog->proxy_dropdown), "changed",
					 G_CALLBACK(proxy_type_changed_cb), dialog);
}

static void
account_win_destroy_cb(GtkWidget *w, GdkEvent *event,
					   AccountPrefsDialog *dialog)
{
	if (accounts_window != NULL)
		g_hash_table_remove(accounts_window->account_pref_wins, dialog->account);

	gtk_widget_destroy(dialog->window);

	if (dialog->user_split_entries != NULL)
		g_list_free(dialog->user_split_entries);

	if (dialog->protocol_opt_entries != NULL)
		g_list_free(dialog->protocol_opt_entries);

	if (dialog->protocol_id != NULL)
		g_free(dialog->protocol_id);

	if (dialog->icon_filesel)
		gtk_widget_destroy(dialog->icon_filesel);

	g_free(dialog);
}

static void
cancel_account_prefs_cb(GtkWidget *w, AccountPrefsDialog *dialog)
{
	account_win_destroy_cb(NULL, NULL, dialog);
}

static GaimAccount*
ok_account_prefs_cb(GtkWidget *w, AccountPrefsDialog *dialog)
{
	GaimProxyInfo *proxy_info = NULL;
	GList *l, *l2;
	const char *value;
	char *username;
	char *tmp;
	size_t index;
	GtkTreeIter iter;
	GaimAccount *ret;

	if (dialog->account == NULL)
	{
		const char *screenname;

		screenname = gtk_entry_get_text(GTK_ENTRY(dialog->screenname_entry));

		dialog->account = gaim_account_new(screenname, dialog->protocol_id);
	}
	else
	{
		/* Protocol */
		gaim_account_set_protocol_id(dialog->account, dialog->protocol_id);
	}

	/* Alias */
	value = gtk_entry_get_text(GTK_ENTRY(dialog->alias_entry));

	if (*value != '\0')
		gaim_account_set_alias(dialog->account, value);
	else
		gaim_account_set_alias(dialog->account, NULL);

	/* Buddy Icon */
	value = dialog->icon_path;
	gaim_account_set_buddy_icon(dialog->account, value);
	
	/* Remember Password */
	gaim_account_set_remember_password(dialog->account,
			gtk_toggle_button_get_active(
					GTK_TOGGLE_BUTTON(dialog->remember_pass_check)));

	/* Check Mail */
	if (dialog->prpl_info && dialog->prpl_info->options & OPT_PROTO_MAIL_CHECK)
		gaim_account_set_check_mail(dialog->account,
			gtk_toggle_button_get_active(
					GTK_TOGGLE_BUTTON(dialog->new_mail_check)));

	/* Password */
	value = gtk_entry_get_text(GTK_ENTRY(dialog->password_entry));

	if (gaim_account_get_remember_password(dialog->account) && *value != '\0')
		gaim_account_set_password(dialog->account, value);
	else
		gaim_account_set_password(dialog->account, NULL);

	/* Build the username string. */
	username =
		g_strdup(gtk_entry_get_text(GTK_ENTRY(dialog->screenname_entry)));

	if (dialog->prpl_info != NULL)
	{
		for (l = dialog->prpl_info->user_splits,
			 l2 = dialog->user_split_entries;
			 l != NULL && l2 != NULL;
			 l = l->next, l2 = l2->next)
		{
			GaimAccountUserSplit *split = l->data;
			GtkEntry *entry = l2->data;
			char sep[2] = " ";

			value = gtk_entry_get_text(entry);

			*sep = gaim_account_user_split_get_separator(split);

			tmp = g_strconcat(username, sep,
					(*value ? value :
					 gaim_account_user_split_get_default_value(split)),
					NULL);

			g_free(username);
			username = tmp;
		}
	}

	gaim_account_set_username(dialog->account, username);
	g_free(username);

	/* Add the protocol settings */

	if(dialog->prpl_info) {
		for (l = dialog->prpl_info->protocol_options,
				l2 = dialog->protocol_opt_entries;
				l != NULL && l2 != NULL;
				l = l->next, l2 = l2->next) {

			GaimPrefType type;
			GaimAccountOption *option = l->data;
			GtkWidget *widget = l2->data;
			const char *setting;
			int int_value;
			gboolean bool_value;

			type = gaim_account_option_get_type(option);

			setting = gaim_account_option_get_setting(option);

			switch (type) {
				case GAIM_PREF_STRING:
					value = gtk_entry_get_text(GTK_ENTRY(widget));
					gaim_account_set_string(dialog->account, setting, value);
					break;

				case GAIM_PREF_INT:
					int_value = atoi(gtk_entry_get_text(GTK_ENTRY(widget)));
					gaim_account_set_int(dialog->account, setting, int_value);
					break;

				case GAIM_PREF_BOOLEAN:
					bool_value =
						gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
					gaim_account_set_bool(dialog->account, setting, bool_value);
					break;

				default:
					break;
			}
		}
	}

	/* Set the proxy stuff. */
	if (dialog->new_proxy_type == GAIM_PROXY_USE_GLOBAL) {
		gaim_account_set_proxy_info(dialog->account, NULL);
	}
	else {
		proxy_info = gaim_account_get_proxy_info(dialog->account);

		/* Create the proxy info if it doesn't exist. */
		if (proxy_info == NULL) {
			proxy_info = gaim_proxy_info_new();
			gaim_account_set_proxy_info(dialog->account, proxy_info);
		}

		/* Set the proxy info type. */
		gaim_proxy_info_set_type(proxy_info, dialog->new_proxy_type);

		/* Host */
		value = gtk_entry_get_text(GTK_ENTRY(dialog->proxy_host_entry));

		if (*value != '\0')
			gaim_proxy_info_set_host(proxy_info, value);
		else
			gaim_proxy_info_set_host(proxy_info, NULL);

		/* Port */
		value = gtk_entry_get_text(GTK_ENTRY(dialog->proxy_port_entry));

		if (*value != '\0')
			gaim_proxy_info_set_port(proxy_info, atoi(value));
		else
			gaim_proxy_info_set_port(proxy_info, 0);

		/* Username */
		value = gtk_entry_get_text(GTK_ENTRY(dialog->proxy_user_entry));

		if (*value != '\0')
			gaim_proxy_info_set_username(proxy_info, value);
		else
			gaim_proxy_info_set_username(proxy_info, NULL);

		/* Password */
		value = gtk_entry_get_text(GTK_ENTRY(dialog->proxy_pass_entry));

		if (*value != '\0')
			gaim_proxy_info_set_password(proxy_info, value);
		else
			gaim_proxy_info_set_password(proxy_info, NULL);
	}

	/* Adds the account to the list, or modify the existing entry. */
	if (accounts_window != NULL) {
		index = g_list_index(gaim_accounts_get_all(), dialog->account);

		if (index != -1 &&
			(gtk_tree_model_iter_nth_child(
					GTK_TREE_MODEL(accounts_window->model), &iter,
					NULL, index))) {

			set_account(accounts_window->model, &iter,
						dialog->account);
		}
		else {
			add_account(accounts_window, dialog->account);
			gaim_accounts_add(dialog->account);
		}
	}

	ret = dialog->account;

	account_win_destroy_cb(NULL, NULL, dialog);

	gaim_signal_emit(gaim_gtk_account_get_handle(), "account-modified", ret);

	return ret;
}

static void
register_account_prefs_cb(GtkWidget *w, AccountPrefsDialog *dialog)
{
	GaimAccount *account = ok_account_prefs_cb(NULL, dialog);

	gaim_account_register(account);
}


static const GtkTargetEntry dnd_targets[] = {
       {"text/plain", 0, 0},
	{"text/uri-list", 0, 1},
	{"STRING", 0, 2}
};

void
gaim_gtk_account_dialog_show(GaimGtkAccountDialogType type,
							 GaimAccount *account)
{
	AccountPrefsDialog *dialog;
	GtkWidget *win;
	GtkWidget *main_vbox;
	GtkWidget *vbox;
	GtkWidget *bbox;
	GtkWidget *dbox;
	GtkWidget *disclosure;
	GtkWidget *sep;
	GtkWidget *button;

	if (accounts_window != NULL && account != NULL &&
		(dialog = g_hash_table_lookup(accounts_window->account_pref_wins,
									  account)) != NULL)
	{
		gtk_window_present(GTK_WINDOW(dialog->window));
		return;
	}

	dialog = g_new0(AccountPrefsDialog, 1);

	if (accounts_window != NULL && account != NULL)
	{
		g_hash_table_insert(accounts_window->account_pref_wins,
							account, dialog);
	}

	dialog->account = account;
	dialog->type    = type;
	dialog->sg      = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);

	if (dialog->account == NULL) {
		/* Select the first prpl in the list*/
		GList *prpl_list = gaim_plugins_get_protocols();
		if (prpl_list != NULL)
			dialog->protocol_id = g_strdup(((GaimPlugin *) prpl_list->data)->info->id);
	}
	else
	{
		dialog->protocol_id =
			g_strdup(gaim_account_get_protocol_id(dialog->account));
	}

	if ((dialog->plugin = gaim_find_prpl(dialog->protocol_id)) != NULL)
		dialog->prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(dialog->plugin);


	dialog->window = win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_role(GTK_WINDOW(win), "account");

	if (type == GAIM_GTK_ADD_ACCOUNT_DIALOG)
		gtk_window_set_title(GTK_WINDOW(win), _("Add Account"));
	else
		gtk_window_set_title(GTK_WINDOW(win), _("Modify Account"));

	gtk_window_set_resizable(GTK_WINDOW(win), FALSE);

	gtk_container_set_border_width(GTK_CONTAINER(win), 12);

	g_signal_connect(G_OBJECT(win), "delete_event",
					 G_CALLBACK(account_win_destroy_cb), dialog);

	/* Setup the vbox */
	main_vbox = gtk_vbox_new(FALSE, 12);
	gtk_container_add(GTK_CONTAINER(win), main_vbox);
	gtk_widget_show(main_vbox);

	/* Setup the inner vbox */
	dialog->top_vbox = vbox = gtk_vbox_new(FALSE, 18);
	gtk_box_pack_start(GTK_BOX(main_vbox), vbox, FALSE, FALSE, 0);
	gtk_widget_show(vbox);

	/* Setup the top frames. */
	add_login_options(dialog, vbox);
	add_user_options(dialog, vbox);

	/* Add the disclosure */
	disclosure = gaim_disclosure_new(_("Show more options"),
									 _("Show fewer options"));
	gtk_box_pack_start(GTK_BOX(vbox), disclosure, FALSE, FALSE, 0);
	gtk_widget_show(disclosure);

	/* Setup the box that the disclosure will cover. */
	dialog->bottom_vbox = dbox = gtk_vbox_new(FALSE, 18);
	gtk_box_pack_start(GTK_BOX(vbox), dbox, FALSE, FALSE, 0);

	gaim_disclosure_set_container(GAIM_DISCLOSURE(disclosure), dbox);

	/** Setup the bottom frames. */
	add_protocol_options(dialog, dbox);
	add_proxy_options(dialog, dbox);

	/* Separator... */
	sep = gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX(main_vbox), sep, FALSE, FALSE, 0);
	gtk_widget_show(sep);

	/* Setup the button box */
	bbox = gtk_hbutton_box_new();
	gtk_box_set_spacing(GTK_BOX(bbox), 6);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox), GTK_BUTTONBOX_END);
	gtk_box_pack_end(GTK_BOX(main_vbox), bbox, FALSE, TRUE, 0);
	gtk_widget_show(bbox);

	/* Register button */
	button = gtk_button_new_with_label(_("Register"));
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);

	g_signal_connect(G_OBJECT(button), "clicked",
			G_CALLBACK(register_account_prefs_cb), dialog);

	dialog->register_button = button;

	if (dialog->account == NULL)
		gtk_widget_set_sensitive(button, FALSE);

	if (!dialog->prpl_info || !dialog->prpl_info->register_user)
		gtk_widget_hide(button);

	/* Cancel button */
	button = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(cancel_account_prefs_cb), dialog);

	/* Save button */
	button = gtk_button_new_from_stock(GTK_STOCK_SAVE);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);

	if (dialog->account == NULL)
		gtk_widget_set_sensitive(button, FALSE);

	gtk_widget_show(button);

	dialog->ok_button = button;

	/* Set up DND */
	gtk_drag_dest_set(dialog->window,
			  GTK_DEST_DEFAULT_MOTION |
			  GTK_DEST_DEFAULT_DROP,
			  dnd_targets,
			  sizeof(dnd_targets) / sizeof(GtkTargetEntry),
			  GDK_ACTION_COPY);

	g_signal_connect(G_OBJECT(dialog->window), "drag_data_received",
			 G_CALLBACK(account_dnd_recv), dialog);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(ok_account_prefs_cb), dialog);

	/* Show the window. */
	gtk_widget_show(win);
}

/**************************************************************************
 * Accounts Dialog
 **************************************************************************/
static void
signed_on_off_cb(GaimConnection *gc, AccountsWindow *dialog)
{
	GaimAccount *account = gaim_connection_get_account(gc);
	GaimGtkPulseData *pulse_data;
	GtkTreeModel *model = GTK_TREE_MODEL(dialog->model);
	GtkTreeIter iter;
	GdkPixbuf *pixbuf, *scale = NULL;
	size_t index = g_list_index(gaim_accounts_get_all(), account);

	if (gtk_tree_model_iter_nth_child(model, &iter, NULL, index))
	{
		gtk_tree_model_get(GTK_TREE_MODEL(dialog->model), &iter,
						   COLUMN_PULSE_DATA, &pulse_data, -1);

		if (pulse_data != NULL)
		{
			if (pulse_data->timeout > 0)
				g_source_remove(pulse_data->timeout);

			g_object_unref(G_OBJECT(pulse_data->online_pixbuf));

			g_free(pulse_data);
		}

		pixbuf = create_prpl_icon(account);

		if (pixbuf != NULL)
		{
			scale = gdk_pixbuf_scale_simple(pixbuf, 16, 16,
											GDK_INTERP_BILINEAR);

			if (!gaim_account_is_connected(account))
				gdk_pixbuf_saturate_and_pixelate(scale, scale, 0.0, FALSE);
		}
		gtk_list_store_set(dialog->model, &iter,
				   COLUMN_ICON, scale,
				   COLUMN_PULSE_DATA, NULL,
				   -1);


		if (pixbuf != NULL) g_object_unref(G_OBJECT(pixbuf));
		if (scale  != NULL) g_object_unref(G_OBJECT(scale));
	}
}

static void
drag_data_get_cb(GtkWidget *widget, GdkDragContext *ctx,
				 GtkSelectionData *data, guint info, guint time,
				 AccountsWindow *dialog)
{
	if (data->target == gdk_atom_intern("GAIM_ACCOUNT", FALSE)) {
		GtkTreeRowReference *ref;
		GtkTreePath *source_row;
		GtkTreeIter iter;
		GaimAccount *account = NULL;
		GValue val = {0};

		ref = g_object_get_data(G_OBJECT(ctx), "gtk-tree-view-source-row");
		source_row = gtk_tree_row_reference_get_path(ref);

		if (source_row == NULL)
			return;

		gtk_tree_model_get_iter(GTK_TREE_MODEL(dialog->model), &iter,
								source_row);
		gtk_tree_model_get_value(GTK_TREE_MODEL(dialog->model), &iter,
								 COLUMN_DATA, &val);

		dialog->drag_iter = iter;

		account = g_value_get_pointer(&val);

		gtk_selection_data_set(data, gdk_atom_intern("GAIM_ACCOUNT", FALSE),
							   8, (void *)&account, sizeof(account));

		gtk_tree_path_free(source_row);
	}
}

static void
move_account_after(GtkListStore *store, GtkTreeIter *iter,
				   GtkTreeIter *position)
{
	GtkTreeIter new_iter;
	GaimAccount *account;

	gtk_tree_model_get(GTK_TREE_MODEL(store), iter,
					   COLUMN_DATA, &account,
					   -1);

	gtk_list_store_insert_after(store, &new_iter, position);

	set_account(store, &new_iter, account);

	gtk_list_store_remove(store, iter);
}

static void
move_account_before(GtkListStore *store, GtkTreeIter *iter,
					GtkTreeIter *position)
{
	GtkTreeIter new_iter;
	GaimAccount *account;

	gtk_tree_model_get(GTK_TREE_MODEL(store), iter,
					   COLUMN_DATA, &account,
					   -1);

	gtk_list_store_insert_before(store, &new_iter, position);

	set_account(store, &new_iter, account);

	gtk_list_store_remove(store, iter);
}

static void
drag_data_received_cb(GtkWidget *widget, GdkDragContext *ctx,
					  guint x, guint y, GtkSelectionData *sd,
					  guint info, guint t, AccountsWindow *dialog)
{
	if (sd->target == gdk_atom_intern("GAIM_ACCOUNT", FALSE) && sd->data) {
		size_t dest_index;
		GaimAccount *a = NULL;
		GtkTreePath *path = NULL;
		GtkTreeViewDropPosition position;

		memcpy(&a, sd->data, sizeof(a));

		if (gtk_tree_view_get_dest_row_at_pos(GTK_TREE_VIEW(widget), x, y,
											  &path, &position)) {

			GtkTreeIter iter;
			GaimAccount *account;
			GValue val = {0};

			gtk_tree_model_get_iter(GTK_TREE_MODEL(dialog->model), &iter, path);
			gtk_tree_model_get_value(GTK_TREE_MODEL(dialog->model), &iter,
									 COLUMN_DATA, &val);

			account = g_value_get_pointer(&val);

			switch (position) {
				case GTK_TREE_VIEW_DROP_AFTER:
				case GTK_TREE_VIEW_DROP_INTO_OR_AFTER:
					move_account_after(dialog->model, &dialog->drag_iter,
									   &iter);
					dest_index = g_list_index(gaim_accounts_get_all(),
											  account) + 1;
					break;

				case GTK_TREE_VIEW_DROP_BEFORE:
				case GTK_TREE_VIEW_DROP_INTO_OR_BEFORE:
					dest_index = g_list_index(gaim_accounts_get_all(),
											  account);

					move_account_before(dialog->model, &dialog->drag_iter,
										&iter);
					break;

				default:
					return;
			}

			gaim_accounts_reorder(a, dest_index);
		}
	}
}

static gint
accedit_win_destroy_cb(GtkWidget *w, GdkEvent *event, AccountsWindow *dialog)
{
	gaim_gtk_accounts_window_hide();

	return 0;
}

static gboolean
configure_cb(GtkWidget *w, GdkEventConfigure *event, AccountsWindow *dialog)
{
	if (GTK_WIDGET_VISIBLE(w)) {
		int old_width = gaim_prefs_get_int("/gaim/gtk/accounts/dialog/width");
		int col_width;
		int difference;

		gaim_prefs_set_int("/gaim/gtk/accounts/dialog/width",  event->width);
		gaim_prefs_set_int("/gaim/gtk/accounts/dialog/height", event->height);

		col_width = gtk_tree_view_column_get_width(dialog->screenname_col);

		if (col_width == 0)
			return FALSE;

		difference = (MAX(old_width, event->width) -
					  MIN(old_width, event->width));

		if (difference == 0)
			return FALSE;

		if (old_width < event->width)
			gtk_tree_view_column_set_min_width(dialog->screenname_col,
					col_width + difference);
		else
			gtk_tree_view_column_set_max_width(dialog->screenname_col,
					col_width - difference);
	}

	return FALSE;
}

static void
add_account_cb(GtkWidget *w, AccountsWindow *dialog)
{
	gaim_gtk_account_dialog_show(GAIM_GTK_ADD_ACCOUNT_DIALOG, NULL);
}

static void
modify_account_sel(GtkTreeModel *model, GtkTreePath *path,
				   GtkTreeIter *iter, gpointer data)
{
	GaimAccount *account;

	gtk_tree_model_get(model, iter, COLUMN_DATA, &account, -1);

	if (account != NULL)
		gaim_gtk_account_dialog_show(GAIM_GTK_MODIFY_ACCOUNT_DIALOG, account);
}

static void
modify_account_cb(GtkWidget *w, AccountsWindow *dialog)
{
	GtkTreeSelection *selection;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(dialog->treeview));

	gtk_tree_selection_selected_foreach(selection, modify_account_sel, dialog);
}

static void
delete_account_cb(GaimAccount *account)
{
	size_t index;
	GtkTreeIter iter;

	index = g_list_index(gaim_accounts_get_all(), account);

	if (accounts_window != NULL)
	{
		AccountPrefsDialog *dialog;

		if (gtk_tree_model_iter_nth_child(
				GTK_TREE_MODEL(accounts_window->model), &iter, NULL, index))
		{
			gtk_list_store_remove(accounts_window->model, &iter);
		}

		if ((dialog = g_hash_table_lookup(accounts_window->account_pref_wins,
										  account)) != NULL)
		{
			account_win_destroy_cb(NULL, NULL, dialog);
		}
	}

	gaim_accounts_delete(account);
}

static void
ask_delete_account_sel(GtkTreeModel *model, GtkTreePath *path,
					   GtkTreeIter *iter, gpointer data)
{
	GaimAccount *account;

	gtk_tree_model_get(model, iter, COLUMN_DATA, &account, -1);

	if (account != NULL) {
		char *buf;

		buf = g_strdup_printf(_("Are you sure you want to delete %s?"),
							  gaim_account_get_username(account));

		gaim_request_close_with_handle(account);
		gaim_request_action(account, NULL, buf, NULL, 0, account, 2,
							_("Delete"), delete_account_cb,
							_("Cancel"), NULL);
		g_free(buf);
	}
}

static void
ask_delete_account_cb(GtkWidget *w, AccountsWindow *dialog)
{
	GtkTreeSelection *selection;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(dialog->treeview));

	gtk_tree_selection_selected_foreach(selection, ask_delete_account_sel,
										dialog);
}

static void
close_accounts_cb(GtkWidget *w, AccountsWindow *dialog)
{
	gtk_widget_destroy(dialog->window);

	gaim_gtk_accounts_window_hide();
}


static void
enabled_cb(GtkCellRendererToggle *renderer, gchar *path_str,
			   gpointer data)
{
	AccountsWindow *dialog = (AccountsWindow *)data;
	GaimAccount *account;
	GtkTreeModel *model = GTK_TREE_MODEL(dialog->model);
	GtkTreeIter iter;
	gboolean enabled;

	gtk_tree_model_get_iter_from_string(model, &iter, path_str);
	gtk_tree_model_get(model, &iter,
					   COLUMN_DATA, &account,
					   COLUMN_ENABLED, &enabled,
					   -1);

	gaim_account_set_enabled(account, GAIM_GTK_UI, !enabled);

	gtk_list_store_set(dialog->model, &iter,
					   COLUMN_ENABLED, !enabled,
					   -1);
}

static void
add_columns(GtkWidget *treeview, AccountsWindow *dialog)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	/* Screen Name column */
	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, _("Screen Name"));
	gtk_tree_view_insert_column(GTK_TREE_VIEW(treeview), column, -1);
	gtk_tree_view_column_set_resizable(column, TRUE);

	/* Icon */
	renderer = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_column_pack_start(column, renderer, FALSE);
	gtk_tree_view_column_add_attribute(column, renderer,
					   "pixbuf", COLUMN_ICON);

	/* Screen Name */
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer,
					   "text", COLUMN_SCREENNAME);
	dialog->screenname_col = column;

	/* Enabled */
	renderer = gtk_cell_renderer_toggle_new();

	g_signal_connect(G_OBJECT(renderer), "toggled",
					 G_CALLBACK(enabled_cb), dialog);

	column = gtk_tree_view_column_new_with_attributes(_("Enabled"),
			renderer, "active", COLUMN_ENABLED, NULL);

	gtk_tree_view_insert_column(GTK_TREE_VIEW(treeview), column, -1);
	gtk_tree_view_column_set_resizable(column, TRUE);

	/* Protocol name */
	column = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(column, _("Protocol"));
	gtk_tree_view_insert_column(GTK_TREE_VIEW(treeview), column, -1);
	gtk_tree_view_column_set_resizable(column, TRUE);

	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(column, renderer, TRUE);
	gtk_tree_view_column_add_attribute(column, renderer,
					   "text", COLUMN_PROTOCOL);
}

static void
set_account(GtkListStore *store, GtkTreeIter *iter, GaimAccount *account)
{
	GdkPixbuf *pixbuf;
	GdkPixbuf *scale;

	scale = NULL;

	pixbuf = create_prpl_icon(account);

	if (pixbuf != NULL)
	{
		scale = gdk_pixbuf_scale_simple(pixbuf, 16, 16, GDK_INTERP_BILINEAR);

		if (!gaim_account_is_connected(account))
			gdk_pixbuf_saturate_and_pixelate(scale, scale, 0.0, FALSE);
	}

	gtk_list_store_set(store, iter,
			COLUMN_ICON, scale,
			COLUMN_SCREENNAME, gaim_account_get_username(account),
			COLUMN_ENABLED, gaim_account_get_enabled(account, GAIM_GTK_UI),
			COLUMN_PROTOCOL, gaim_account_get_protocol_name(account),
			COLUMN_DATA, account,
			-1);

	if (pixbuf != NULL) g_object_unref(G_OBJECT(pixbuf));
	if (scale  != NULL) g_object_unref(G_OBJECT(scale));
}

static void
add_account(AccountsWindow *dialog, GaimAccount *account)
{
	GtkTreeIter iter;

	gtk_list_store_append(dialog->model, &iter);

	set_account(dialog->model, &iter, account);
}

static void
populate_accounts_list(AccountsWindow *dialog)
{
	GList *l;

	gtk_list_store_clear(dialog->model);

	for (l = gaim_accounts_get_all(); l != NULL; l = l->next)
		add_account(dialog, (GaimAccount *)l->data);
}

#if !GTK_CHECK_VERSION(2,2,0)
static void
get_selected_helper(GtkTreeModel *model, GtkTreePath *path,
					GtkTreeIter *iter, gpointer user_data)
{
	*((gboolean *)user_data) = TRUE;
}
#endif

static void
account_selected_cb(GtkTreeSelection *sel, AccountsWindow *dialog)
{
	gboolean selected = FALSE;

#if GTK_CHECK_VERSION(2,2,0)
	selected = (gtk_tree_selection_count_selected_rows(sel) > 0);
#else
	gtk_tree_selection_selected_foreach(sel, get_selected_helper, &selected);
#endif

	gtk_widget_set_sensitive(dialog->modify_button, selected);
	gtk_widget_set_sensitive(dialog->delete_button, selected);
}

static GtkWidget *
create_accounts_list(AccountsWindow *dialog)
{
	GtkWidget *sw;
	GtkWidget *treeview;
	GtkTreeSelection *sel;
	GtkTargetEntry gte[] = {{"GAIM_ACCOUNT", GTK_TARGET_SAME_APP, 0}};

	/* Create the scrolled window. */
	sw = gtk_scrolled_window_new(0, 0);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
								   GTK_POLICY_AUTOMATIC,
								   GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw),
										GTK_SHADOW_IN);
	gtk_widget_show(sw);

	/* Create the list model. */
	dialog->model = gtk_list_store_new(NUM_COLUMNS,
									   GDK_TYPE_PIXBUF, G_TYPE_STRING,
									   G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_POINTER,
									   G_TYPE_POINTER);

	/* And now the actual treeview */
	treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(dialog->model));
	dialog->treeview = treeview;
	gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(treeview), TRUE);

	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	gtk_tree_selection_set_mode(sel, GTK_SELECTION_MULTIPLE);
	g_signal_connect(G_OBJECT(sel), "changed",
					 G_CALLBACK(account_selected_cb), dialog);

	gtk_container_add(GTK_CONTAINER(sw), treeview);
	gtk_widget_show(treeview);

	add_columns(treeview, dialog);

	populate_accounts_list(dialog);

	/* Setup DND. I wanna be an orc! */
	gtk_tree_view_enable_model_drag_source(
			GTK_TREE_VIEW(treeview), GDK_BUTTON1_MASK, gte,
			1, GDK_ACTION_COPY);
	gtk_tree_view_enable_model_drag_dest(
			GTK_TREE_VIEW(treeview), gte, 1,
			GDK_ACTION_COPY | GDK_ACTION_MOVE);

	g_signal_connect(G_OBJECT(treeview), "drag-data-received",
					 G_CALLBACK(drag_data_received_cb), dialog);
	g_signal_connect(G_OBJECT(treeview), "drag-data-get",
					 G_CALLBACK(drag_data_get_cb), dialog);

	return sw;
}

void
gaim_gtk_accounts_window_show(void)
{
	AccountsWindow *dialog;
	GtkWidget *win;
	GtkWidget *vbox;
	GtkWidget *bbox;
	GtkWidget *sw;
	GtkWidget *sep;
	GtkWidget *button;
	int width, height;

	if (accounts_window != NULL) {
		gtk_window_present(GTK_WINDOW(accounts_window->window));
		return;
	}

	accounts_window = dialog = g_new0(AccountsWindow, 1);

	accounts_window->account_pref_wins =
		g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);

	width  = gaim_prefs_get_int("/gaim/gtk/accounts/dialog/width");
	height = gaim_prefs_get_int("/gaim/gtk/accounts/dialog/height");

	dialog->window = win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(win), width, height);
	gtk_window_set_role(GTK_WINDOW(win), "accounts");
	gtk_window_set_title(GTK_WINDOW(win), _("Accounts"));
	gtk_container_set_border_width(GTK_CONTAINER(win), 12);

	g_signal_connect(G_OBJECT(win), "delete_event",
					 G_CALLBACK(accedit_win_destroy_cb), accounts_window);
	g_signal_connect(G_OBJECT(win), "configure_event",
					 G_CALLBACK(configure_cb), accounts_window);

	/* Setup the vbox */
	vbox = gtk_vbox_new(FALSE, 12);
	gtk_container_add(GTK_CONTAINER(win), vbox);
	gtk_widget_show(vbox);

	/* Setup the scrolled window that will contain the list of accounts. */
	sw = create_accounts_list(dialog);
	gtk_box_pack_start(GTK_BOX(vbox), sw, TRUE, TRUE, 0);
	gtk_widget_show(sw);

	/* Separator... */
	sep = gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX(vbox), sep, FALSE, FALSE, 0);
	gtk_widget_show(sep);

	/* Button box. */
	bbox = gtk_hbutton_box_new();
	gtk_box_set_spacing(GTK_BOX(bbox), 6);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox), GTK_BUTTONBOX_END);
	gtk_box_pack_end(GTK_BOX(vbox), bbox, FALSE, TRUE, 0);
	gtk_widget_show(bbox);

	/* Add button */
	button = gtk_button_new_from_stock(GTK_STOCK_ADD);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(add_account_cb), dialog);

	/* Modify button */
	button = gtk_button_new_from_stock(GAIM_STOCK_MODIFY);
	dialog->modify_button = button;
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	gtk_widget_set_sensitive(button, FALSE);
	gtk_widget_show(button);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(modify_account_cb), dialog);

	/* Delete button */
	button = gtk_button_new_from_stock(GTK_STOCK_DELETE);
	dialog->delete_button = button;
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	gtk_widget_set_sensitive(button, FALSE);
	gtk_widget_show(button);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(ask_delete_account_cb), dialog);

	/* Close button */
	button = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(close_accounts_cb), dialog);

	/* Setup some gaim signal handlers. */
	gaim_signal_connect(gaim_connections_get_handle(), "signed-on",
						dialog, GAIM_CALLBACK(signed_on_off_cb), dialog);
	gaim_signal_connect(gaim_connections_get_handle(), "signed-off",
						dialog, GAIM_CALLBACK(signed_on_off_cb), dialog);

	gtk_widget_show(win);
}

void
gaim_gtk_accounts_window_hide(void)
{
	if (accounts_window == NULL)
		return;

	gaim_signals_disconnect_by_handle(accounts_window);

	g_hash_table_destroy(accounts_window->account_pref_wins);

	g_free(accounts_window);
	accounts_window = NULL;

	/* See if we're the main window here. */
	if (GAIM_GTK_BLIST(gaim_get_blist())->window == NULL &&
		gaim_connections_get_all() == NULL) {

		gaim_core_quit();
	}
}

static void
free_add_user_data(GaimGtkAccountAddUserData *data)
{
	g_free(data->username);

	if (data->alias != NULL)
		g_free(data->alias);

	g_free(data);
}

static void
add_user_cb(GaimGtkAccountAddUserData *data)
{
	GaimConnection *gc = gaim_account_get_connection(data->account);

	if (g_list_find(gaim_connections_get_all(), gc))
	{
		gaim_blist_request_add_buddy(data->account, data->username,
									 NULL, data->alias);
	}

	free_add_user_data(data);
}

static void
gaim_gtk_accounts_notify_added(GaimAccount *account, const char *remote_user,
							   const char *id, const char *alias,
							   const char *msg)
{
	char *buffer;
	GaimConnection *gc;
	GaimGtkAccountAddUserData *data;
	GaimBuddy *buddy;

	gc = gaim_account_get_connection(account);

	buddy = gaim_find_buddy(account, remote_user);

	data = g_new0(GaimGtkAccountAddUserData, 1);
	data->account  = account;
	data->username = g_strdup(remote_user);
	data->alias    = (alias != NULL ? g_strdup(alias) : NULL);

	buffer = g_strdup_printf(_("%s%s%s%s has made %s his or her buddy%s%s%s"),
		remote_user,
		(alias != NULL ? " ("  : ""),
		(alias != NULL ? alias : ""),
		(alias != NULL ? ")"   : ""),
		(id != NULL
		 ? id
		 : (gaim_connection_get_display_name(gc) != NULL
			? gaim_connection_get_display_name(gc)
			: gaim_account_get_username(account))),
		(msg != NULL ? ": " : "."),
		(msg != NULL ? msg  : ""),
		(buddy != NULL
		 ? ""
		 : _("\n\nDo you wish to add him or her to your buddy list?")));

	if (buddy != NULL)
	{
		gaim_notify_info(NULL, NULL, _("Information"), buffer);
	}
	else
	{
		gaim_request_action(NULL, NULL, _("Add buddy to your list?"),
							buffer, GAIM_DEFAULT_ACTION_NONE, data, 2,
							_("Add"),    G_CALLBACK(add_user_cb),
							_("Cancel"), G_CALLBACK(free_add_user_data));
	}

	g_free(buffer);
}

static GaimAccountUiOps ui_ops =
{
	gaim_gtk_accounts_notify_added,
	NULL
};

GaimAccountUiOps *
gaim_gtk_accounts_get_ui_ops(void)
{
	return &ui_ops;
}

void *
gaim_gtk_account_get_handle() {
	static int handle;

	return &handle;
}

void
gaim_gtk_account_init(void)
{
	gaim_prefs_add_none("/gaim/gtk/accounts");
	gaim_prefs_add_none("/gaim/gtk/accounts/dialog");
	gaim_prefs_add_int("/gaim/gtk/accounts/dialog/width",  550);
	gaim_prefs_add_int("/gaim/gtk/accounts/dialog/height", 250);

	gaim_signal_register(gaim_gtk_account_get_handle(), "account-modified",
						 gaim_marshal_VOID__POINTER, NULL, 1,
						 gaim_value_new(GAIM_TYPE_SUBTYPE,
										GAIM_SUBTYPE_ACCOUNT));
}

void
gaim_gtk_account_uninit(void)
{
	gaim_signals_unregister_by_instance(gaim_gtk_account_get_handle());
}
