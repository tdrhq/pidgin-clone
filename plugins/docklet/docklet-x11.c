/*
 * System tray icon (aka docklet) plugin for Gaim
 *
 * Copyright (C) 2002-3 Robert McQueen <robot101@debian.org>
 * Copyright (C) 2003 Herman Bloggs <hermanator12002@yahoo.com>
 * Inspired by a similar plugin by:
 *  John (J5) Palmieri <johnp@martianrock.com>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#include "internal.h"
#include "gtkinternal.h"
#include "debug.h"
#include "stock.h"

#include "gaim.h"
#include "ui.h"

#include "eggtrayicon.h"
#include "docklet.h"

/* globals */
static EggTrayIcon *docklet = NULL;
static GtkWidget *image = NULL;
static GdkPixbuf *blank_icon = NULL;

/* protos */
static void docklet_x11_create();

static gboolean
docklet_x11_create_cb()
{
	docklet_x11_create();

	return FALSE; /* for when we're called by the glib idle handler */
}

static void
docklet_x11_embedded_cb(GtkWidget *widget, void *data)
{
	gaim_debug(GAIM_DEBUG_INFO, "tray icon", "embedded\n");

	docklet_embedded();
}

static void
docklet_x11_destroyed_cb(GtkWidget *widget, void *data)
{
	gaim_debug(GAIM_DEBUG_INFO, "tray icon", "destroyed\n");

	docklet_remove(TRUE);

	g_object_unref(G_OBJECT(docklet));
	docklet = NULL;

	g_idle_add(docklet_x11_create_cb, &handle);
}

static void
docklet_x11_clicked_cb(GtkWidget *button, GdkEventButton *event, void *data)
{
	if (event->type != GDK_BUTTON_PRESS)
		return;

	docklet_clicked(event->button);
}

static void
docklet_x11_update_icon(enum docklet_status icon)
{
	const gchar *icon_name = NULL;

	switch (icon) {
		case offline:
			icon_name = GAIM_STOCK_ICON_OFFLINE;
			break;
		case offline_connecting:
		case online_connecting:
			icon_name = GAIM_STOCK_ICON_CONNECT;
			break;
		case online:
			icon_name = GAIM_STOCK_ICON_ONLINE;
			break;
		case online_pending:
			icon_name = GAIM_STOCK_ICON_ONLINE_MSG;
			break;
		case away:
			icon_name = GAIM_STOCK_ICON_AWAY;
			break;
		case away_pending:
			icon_name = GAIM_STOCK_ICON_AWAY_MSG;
			break;
	}

	gtk_image_set_from_stock(GTK_IMAGE(image), icon_name, GTK_ICON_SIZE_LARGE_TOOLBAR);
}

static void
docklet_x11_blank_icon()
{
	if (!blank_icon) {
		gint width, height;

		gtk_icon_size_lookup(GTK_ICON_SIZE_LARGE_TOOLBAR, &width, &height);
		blank_icon = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
		gdk_pixbuf_fill(blank_icon, 0);
	}

	gtk_image_set_from_pixbuf(GTK_IMAGE(image), blank_icon);
}

static void
docklet_x11_destroy()
{
	docklet_remove(GTK_WIDGET_VISIBLE(docklet));

	g_signal_handlers_disconnect_by_func(G_OBJECT(docklet), G_CALLBACK(docklet_x11_destroyed_cb), NULL);
	gtk_widget_destroy(GTK_WIDGET(docklet));

	g_object_unref(G_OBJECT(docklet));
	docklet = NULL;

	if (blank_icon)
		g_object_unref(G_OBJECT(blank_icon));
	blank_icon = NULL;

	gaim_debug(GAIM_DEBUG_INFO, "tray icon", "destroyed\n");
}

static void
docklet_x11_create()
{
	GtkWidget *box;

	if (docklet) {
		/* if this is being called when a tray icon exists, it's because
		   something messed up. try destroying it before we proceed,
		   although docklet_refcount may be all hosed. hopefully won't happen. */
		gaim_debug(GAIM_DEBUG_WARNING, "tray icon", "trying to create icon but it already exists?\n");
		docklet_x11_destroy();
	}

	docklet = egg_tray_icon_new("Gaim");
	box = gtk_event_box_new();
	image = gtk_image_new();

	g_signal_connect(G_OBJECT(docklet), "embedded", G_CALLBACK(docklet_x11_embedded_cb), NULL);
	g_signal_connect(G_OBJECT(docklet), "destroy", G_CALLBACK(docklet_x11_destroyed_cb), NULL);
	g_signal_connect(G_OBJECT(box), "button-press-event", G_CALLBACK(docklet_x11_clicked_cb), NULL);

	gtk_container_add(GTK_CONTAINER(box), image);
	gtk_container_add(GTK_CONTAINER(docklet), box);

	if(!gtk_check_version(2,4,0))
		g_object_set(G_OBJECT(box), "visible-window", FALSE, NULL);

	gtk_widget_show_all(GTK_WIDGET(docklet));

	/* ref the docklet before we bandy it about the place */
	g_object_ref(G_OBJECT(docklet));

	gaim_debug(GAIM_DEBUG_INFO, "tray icon", "created\n");
}

static struct docklet_ui_ops ui_ops =
{
	docklet_x11_create,
	docklet_x11_destroy,
	docklet_x11_update_icon,
	docklet_x11_blank_icon
};

void
docklet_ui_init()
{
	docklet_set_ui_ops(&ui_ops);
}
