/*
 * Contact priority settings plugin.
 *
 * Copyright (C) 2003 Etan Reisner, <deryni9@users.sourceforge.net>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "internal.h"
#include "gtkgaim.h"
#include "gtkplugin.h"
#include "gtkutils.h"
#include "prefs.h"
#include "version.h"

#define CONTACT_PRIORITY_PLUGIN_ID "gtk-contact-priority"

static void
select_account(GtkWidget *widget, GaimAccount *account, gpointer data)
{
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(data),
														(gdouble)gaim_account_get_int(account, "score", 0));
}

static void
account_update(GtkWidget *widget, GtkOptionMenu *optmenu)
{
	GaimAccount *account = NULL;

	account = g_object_get_data(G_OBJECT(gtk_menu_get_active(GTK_MENU(gtk_option_menu_get_menu(optmenu)))), "account");
	gaim_account_set_int(account, "score", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widget)));
}

static void
pref_update(GtkWidget *widget, gpointer data)
{
	gchar pref[256];

	g_snprintf(pref, sizeof(pref), "/core/contact/%s", (gchar *)data);

	if (gaim_prefs_get_type(pref) == GAIM_PREF_INT)
		gaim_prefs_set_int(pref, gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widget)));
	if (gaim_prefs_get_type(pref) == GAIM_PREF_BOOLEAN)
		gaim_prefs_set_bool(pref, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)));
}

static GtkWidget *
get_config_frame(GaimPlugin *plugin)
{
	GtkWidget *ret = NULL, *hbox = NULL, *frame = NULL, *vbox = NULL;
	GtkWidget *label = NULL, *spin = NULL, *check = NULL;
	GtkWidget *optmenu = NULL;
	GtkObject *adj = NULL;
	GtkSizeGroup *sg = NULL;
	GaimAccount *account = NULL;
/*
	GList *accounts = NULL;
 */
	int offline = gaim_prefs_get_int("/core/contact/offline_score");
	int away = gaim_prefs_get_int("/core/contact/away_score");
	int idle = gaim_prefs_get_int("/core/contact/idle_score");
	/*
	int score;
	*/
	gboolean last_match = gaim_prefs_get_bool("/core/contact/last_match");

	sg = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);

	ret = gtk_vbox_new(FALSE, 18);
	gtk_container_set_border_width(GTK_CONTAINER(ret), 12);

	frame = gaim_gtk_make_frame(ret, _("Point values to use when..."));

	vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(frame), vbox);

	/* Offline */
	hbox = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	label = gtk_label_new_with_mnemonic(_("Buddy is offline:"));
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
	gtk_size_group_add_widget(sg, label);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0);

	adj = gtk_adjustment_new(offline, -20, 20, 1, 1, 1);
	spin = gtk_spin_button_new((GtkAdjustment *)adj, 1, 0);
	g_signal_connect(G_OBJECT(spin), "value-changed", G_CALLBACK(pref_update), "offline_score");
	gtk_box_pack_start(GTK_BOX(hbox), spin, FALSE, FALSE, 0);

	/* Away */
	hbox = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	label = gtk_label_new_with_mnemonic(_("Buddy is away:"));
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
	gtk_size_group_add_widget(sg, label);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0);

	adj = gtk_adjustment_new(away, -20, 20, 1, 1, 1);
	spin = gtk_spin_button_new((GtkAdjustment *)adj, 1, 0);
	g_signal_connect(G_OBJECT(spin), "value-changed", G_CALLBACK(pref_update), "away_score");
	gtk_box_pack_start(GTK_BOX(hbox), spin, FALSE, FALSE, 0);

	/* Idle */
	hbox = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	label = gtk_label_new_with_mnemonic(_("Buddy is idle:"));
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
	gtk_size_group_add_widget(sg, label);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0);

	adj = gtk_adjustment_new(idle, -20, 20, 1, 1, 1);
	spin = gtk_spin_button_new((GtkAdjustment *)adj, 1, 0);
	g_signal_connect(G_OBJECT(spin), "value-changed", G_CALLBACK(pref_update), "idle_score");
	gtk_box_pack_start(GTK_BOX(hbox), spin, FALSE, FALSE, 0);

	/* Last match */
	hbox = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	check = gtk_check_button_new_with_label(_("Use last matching buddy"));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(check), last_match);
	g_signal_connect(G_OBJECT(check), "toggled", G_CALLBACK(pref_update), "last_match");
	gtk_box_pack_start(GTK_BOX(hbox), check, FALSE, FALSE, 0);

	/* Explanation */
	label = gtk_label_new(_("The buddy with the lowest score is the buddy who will have priority in the contact.\nThe default values (offline = 4,away = 2, and idle = 1)\nwill use what used to be the built-in order active->idle->away->away+idle->offline."));
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

	frame = gaim_gtk_make_frame(ret, _("Point values to use for Account..."));

	vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(frame), vbox);

	/* Account */
	hbox = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

	/* make this here so I can use it in the option menu callback, we'll
	 * actually set it up later */
	adj = gtk_adjustment_new(0, -20, 20, 1, 1, 1);
	spin = gtk_spin_button_new((GtkAdjustment *)adj, 1, 0);

	optmenu = gaim_gtk_account_option_menu_new(NULL, TRUE,
																						 G_CALLBACK(select_account),
																						 NULL, spin);
	gtk_box_pack_start(GTK_BOX(hbox), optmenu, FALSE, FALSE, 0);

	/* this is where we set up the spin button we made above */
	account = g_object_get_data(G_OBJECT(gtk_menu_get_active(GTK_MENU(gtk_option_menu_get_menu(GTK_OPTION_MENU(optmenu))))),
															"account");
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin),
														(gdouble)gaim_account_get_int(account, "score", 0));
	gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(spin), GTK_ADJUSTMENT(adj));
	g_signal_connect(G_OBJECT(spin), "value-changed",
									 G_CALLBACK(account_update), optmenu);
	gtk_box_pack_start(GTK_BOX(hbox), spin, FALSE, FALSE, 0);

	gtk_widget_show_all(ret);

	return ret;
}

static GaimGtkPluginUiInfo ui_info =
{
	get_config_frame
};

static GaimPluginInfo info =
{
	GAIM_PLUGIN_MAGIC,
	GAIM_MAJOR_VERSION,
	GAIM_MINOR_VERSION,
	GAIM_PLUGIN_STANDARD,                             /**< type           */
	GAIM_GTK_PLUGIN_TYPE,                             /**< ui_requirement */
	0,                                                /**< flags          */
	NULL,                                             /**< dependencies   */
	GAIM_PRIORITY_DEFAULT,                            /**< priority       */

	CONTACT_PRIORITY_PLUGIN_ID,                       /**< id             */
	N_("Contact Priority"),                           /**< name           */
	VERSION,                                          /**< version        */
                                                    /**< summary        */
	N_("Allows for controlling the values associated with different buddy states."),
                                                    /**< description    */
	N_("Allows for changing the point values of idle/away/offline states for buddies in contact priority computations."),
	"Etan Reisner <deryni@eden.rutgers.edu>",         /**< author         */
	GAIM_WEBSITE,                                     /**< homepage       */

	NULL,                                             /**< load           */
	NULL,                                             /**< unload         */
	NULL,                                             /**< destroy        */
	&ui_info,                                         /**< ui_info        */
	NULL                                              /**< extra_info     */
};

static void
init_plugin(GaimPlugin *plugin)
{
}

GAIM_INIT_PLUGIN(contactpriority, init_plugin, info)