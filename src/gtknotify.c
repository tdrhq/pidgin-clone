/**
 * @file gtknotify.c GTK+ Notification API
 * @ingroup gtkui
 *
 * gaim
 *
 * Copyright (C) 2003 Christian Hammond <chipx86@gnupdate.org>
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
#include "gtkinternal.h"

#include "connection.h"
#include "debug.h"
#include "prefs.h"
#include "stock.h"

#include "gtkimhtml.h"
#include "gtknotify.h"
#include "gtkutils.h"

#include "ui.h"

typedef struct
{
	GaimConnection *gc;
	char *url;
	GtkWidget *dialog;
	GtkWidget *label;

} GaimNotifyMailData;

static void *gaim_gtk_notify_emails(size_t count, gboolean detailed,
									const char **subjects,
									const char **froms, const char **tos,
									const char **urls, GCallback cb,
									void *user_data);

static void
message_response_cb(GtkDialog *dialog, gint id, GtkWidget *widget)
{
	gaim_notify_close(GAIM_NOTIFY_MESSAGE, widget);
}

static void
email_response_cb(GtkDialog *dialog, gint id, GaimNotifyMailData *data)
{
	if (id == 0)
		open_url(NULL, data->url);

	gaim_notify_close(GAIM_NOTIFY_EMAILS, data);
}

static void *
gaim_gtk_notify_message(GaimNotifyMsgType type, const char *title,
						const char *primary, const char *secondary,
						GCallback cb, void *user_data)
{
	GtkWidget *dialog;
	GtkWidget *hbox;
	GtkWidget *label;
	GtkWidget *img = NULL;
	char label_text[2048];
	const char *icon_name = NULL;

	switch (type) {
		case GAIM_NOTIFY_MSG_ERROR:
			icon_name = GAIM_STOCK_DIALOG_ERROR;
			break;

		case GAIM_NOTIFY_MSG_WARNING:
			icon_name = GAIM_STOCK_DIALOG_WARNING;
			break;

		case GAIM_NOTIFY_MSG_INFO:
			icon_name = GAIM_STOCK_DIALOG_INFO;
			break;

		default:
			icon_name = NULL;
			break;
	}

	if (icon_name != NULL) {
		img = gtk_image_new_from_stock(icon_name, GTK_ICON_SIZE_DIALOG);
		gtk_misc_set_alignment(GTK_MISC(img), 0, 0);
	}

	dialog = gtk_dialog_new_with_buttons(title, NULL, 0,
										 GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
										 NULL);
	g_signal_connect(G_OBJECT(dialog), "response",
					 G_CALLBACK(message_response_cb), dialog);

	gtk_container_set_border_width(GTK_CONTAINER(dialog), 6);
	gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
	gtk_dialog_set_has_separator(GTK_DIALOG(dialog), FALSE);
	gtk_box_set_spacing(GTK_BOX(GTK_DIALOG(dialog)->vbox), 12);
	gtk_container_set_border_width(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), 6);

	hbox = gtk_hbox_new(FALSE, 12);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), hbox);

	if (img != NULL)
		gtk_box_pack_start(GTK_BOX(hbox), img, FALSE, FALSE, 0);

	g_snprintf(label_text, sizeof(label_text),
			   "<span weight=\"bold\" size=\"larger\">%s</span>\n\n%s",
			   primary, (secondary ? secondary : ""));

	label = gtk_label_new(NULL);

	gtk_label_set_markup(GTK_LABEL(label), label_text);
	gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0);
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);

	gtk_widget_show_all(dialog);

	return dialog;
}

static void *
gaim_gtk_notify_email(const char *subject, const char *from,
					  const char *to, const char *url,
					  GCallback cb, void *user_data)
{
	return gaim_gtk_notify_emails(1, TRUE,
								  (subject == NULL ? NULL : &subject),
								  (from    == NULL ? NULL : &from),
								  (to      == NULL ? NULL : &to),
								  (url     == NULL ? NULL : &url),
								  cb, user_data);
}

static void *
gaim_gtk_notify_emails(size_t count, gboolean detailed,
					   const char **subjects, const char **froms,
					   const char **tos, const char **urls,
					   GCallback cb, void *user_data)
{
	GaimNotifyMailData *data;
	GtkWidget *dialog;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *label;
	GtkWidget *img;
	char *detail_text;
	char *label_text;

	data = g_new0(GaimNotifyMailData, 1);

	data->url = g_strdup(urls[0]);

	/* Create the dialog. */
	data->dialog = dialog = gtk_dialog_new();

	gtk_dialog_add_button(GTK_DIALOG(dialog), GTK_STOCK_CLOSE, 1);

	if (urls != NULL)
		gtk_dialog_add_button(GTK_DIALOG(dialog), GAIM_STOCK_OPEN_MAIL, 0);

	g_signal_connect(G_OBJECT(dialog), "response",
					 G_CALLBACK(email_response_cb), data);

	/* Setup the dialog */
	gtk_container_set_border_width(GTK_CONTAINER(dialog), 6);
	gtk_container_set_border_width(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), 6);
	gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
	gtk_dialog_set_has_separator(GTK_DIALOG(dialog), FALSE);
	gtk_box_set_spacing(GTK_BOX(GTK_DIALOG(dialog)->vbox), 12);

	/* Setup the main horizontal box */
	hbox = gtk_hbox_new(FALSE, 12);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), hbox);

	/* Dialog icon. */
	img = gtk_image_new_from_stock(GAIM_STOCK_DIALOG_INFO,
								   GTK_ICON_SIZE_DIALOG);
	gtk_misc_set_alignment(GTK_MISC(img), 0, 0);
	gtk_box_pack_start(GTK_BOX(hbox), img, FALSE, FALSE, 0);

	/* Vertical box */
	vbox = gtk_vbox_new(FALSE, 12);

	gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);

	/* Descriptive label */
	detail_text = g_strdup_printf(
		ngettext("%s has %d new message.", "%s has %d new messages.", count),
		*tos, count);

	if (count == 1) {
		char *from_text = NULL, *subject_text = NULL;

		if (froms != NULL) {
			from_text = g_strdup_printf(
				_("<span weight=\"bold\">From:</span> %s\n"), *froms);
		}

		if (subjects != NULL) {
			subject_text = g_strdup_printf(
				_("<span weight=\"bold\">Subject:</span> %s\n"), *subjects);
		}

		label_text = g_strdup_printf(
			_("<span weight=\"bold\" size=\"larger\">You have mail!</span>"
			"\n\n%s%s%s%s"),
			detail_text,
			(from_text == NULL && subject_text == NULL ? "" : "\n\n"),
			(from_text == NULL ? "" : from_text),
			(subject_text == NULL ? "" : subject_text));

		if (from_text != NULL)
			g_free(from_text);

		if (subject_text != NULL)
			g_free(subject_text);
	}
	else {
		label_text = g_strdup_printf(
			_("<span weight=\"bold\" size=\"larger\">You have mail!</span>"
			"\n\n%s"), detail_text);
	}

	g_free(detail_text);

	label = gtk_label_new(NULL);

	gtk_label_set_markup(GTK_LABEL(label), label_text);
	gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

	g_free(label_text);

	/* Show everything. */
	gtk_widget_show_all(dialog);

	return data;
}

static void *
gaim_gtk_notify_formatted(const char *title, const char *primary,
						  const char *secondary, const char *text,
						  GCallback cb, void *user_data)
{
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *label;
	GtkWidget *button;
	GtkWidget *imhtml;
	GtkWidget *sw;
	int options = 0;
	char label_text[2048];

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_container_set_border_width(GTK_CONTAINER(window), 12);

	/* Setup the main vbox */
	vbox = gtk_vbox_new(FALSE, 12);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show(vbox);

	/* Setup the descriptive label */
	g_snprintf(label_text, sizeof(label_text),
		   "<span weight=\"bold\" size=\"larger\">%s</span>\n\n%s",
		   primary, (secondary ? secondary : ""));

	label = gtk_label_new(NULL);

	gtk_label_set_markup(GTK_LABEL(label), label_text);
	gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
	gtk_widget_show(label);

	/* Setup the scrolled window that we're putting the gtkimhtml in. */
	sw = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
								   GTK_POLICY_NEVER,
								   GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw),
										GTK_SHADOW_IN);
	gtk_widget_set_size_request(sw, 300, 250);
	gtk_box_pack_start(GTK_BOX(vbox), sw, TRUE, TRUE, 0);
	gtk_widget_show(sw);

	/* Now build that gtkimhtml */
	imhtml = gtk_imhtml_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(sw), imhtml);
	gtk_widget_show(imhtml);
	gaim_setup_imhtml(imhtml);

	/* Add the Close button. */
	button = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	gtk_widget_show(button);

	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(gtk_widget_destroy), window);

	/* Add the text to the gtkimhtml */
	if (gaim_prefs_get_bool("/gaim/gtk/conversations/ignore_colors"))
		options ^= GTK_IMHTML_NO_COLOURS;

	if (gaim_prefs_get_bool("/gaim/gtk/conversations/ignore_fonts"))
		options ^= GTK_IMHTML_NO_FONTS;

	if (gaim_prefs_get_bool("/gaim/gtk/conversations/ignore_font_sizes"))
		options ^= GTK_IMHTML_NO_SIZES;

	options ^= GTK_IMHTML_NO_COMMENTS;
	options ^= GTK_IMHTML_NO_TITLE;
	options ^= GTK_IMHTML_NO_NEWLINE;
	options ^= GTK_IMHTML_NO_SCROLL;

	gtk_imhtml_append_text(GTK_IMHTML(imhtml), text, -1, options);

	/* Show the window */
	gtk_widget_show(window);

	return window;
}

static void
gaim_gtk_close_notify(GaimNotifyType type, void *ui_handle)
{
	if (type == GAIM_NOTIFY_EMAIL || type == GAIM_NOTIFY_EMAILS) {
		GaimNotifyMailData *data = (GaimNotifyMailData *)ui_handle;

		gtk_widget_destroy(data->dialog);

		g_free(data->url);
		g_free(data);
	}
	else
		gtk_widget_destroy(GTK_WIDGET(ui_handle));
}

static GaimNotifyUiOps ops =
{
	gaim_gtk_notify_message,
	gaim_gtk_notify_email,
	gaim_gtk_notify_emails,
	gaim_gtk_notify_formatted,
	gaim_gtk_close_notify
};

GaimNotifyUiOps *
gaim_get_gtk_notify_ui_ops(void)
{
	return &ops;
}
