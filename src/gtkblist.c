/*
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
 *
 */
#include "internal.h"
#include "gtkgaim.h"

#include "account.h"
#include "connection.h"
#include "core.h"
#include "debug.h"
#include "notify.h"
#include "prpl.h"
#include "prefs.h"
#include "plugin.h"
#include "request.h"
#include "signals.h"
#include "sound.h"
#include "stock.h"
#include "util.h"

#include "away.h"
#include "gtkaccount.h"
#include "gtkblist.h"
#include "gtkconv.h"
#include "gtkdebug.h"
#include "gtkdialogs.h"
#include "gtkft.h"
#include "gtklog.h"
#include "gtkpounce.h"
#include "gtkprefs.h"
#include "gtkprivacy.h"
#include "gtkroomlist.h"
#include "gtkutils.h"

#include "gaim.h"

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#if (GTK_CHECK_VERSION(2,2,0) && !(defined(__APPLE__) && defined(__MACH__)))
#define WANT_DROP_SHADOW
#endif

typedef struct
{
	GaimAccount *account;

	GtkWidget *window;
	GtkWidget *combo;
	GtkWidget *entry;
	GtkWidget *entry_for_alias;
	GtkWidget *account_box;

} GaimGtkAddBuddyData;

typedef struct
{
	GaimAccount *account;
	gchar *default_chat_name;

	GtkWidget *window;
	GtkWidget *account_menu;
	GtkWidget *alias_entry;
	GtkWidget *group_combo;
	GtkWidget *entries_box;
	GtkSizeGroup *sg;

	GList *entries;

} GaimGtkAddChatData;

typedef struct
{
	GaimAccount *account;

	GtkWidget *window;
	GtkWidget *account_menu;
	GtkWidget *entries_box;
	GtkSizeGroup *sg;

	GList *entries;
} GaimGtkJoinChatData;


static GtkWidget *protomenu = NULL, *pluginmenu = NULL;

GSList *gaim_gtk_blist_sort_methods = NULL;
static struct gaim_gtk_blist_sort_method *current_sort_method = NULL;
static GtkTreeIter sort_method_none(GaimBlistNode *node, GaimBuddyList *blist, GtkTreeIter groupiter, GtkTreeIter *cur);

/* The functions we use for sorting aren't available in gtk 2.0.x, and
 * segfault in 2.2.0.  2.2.1 is known to work, so I'll require that */
#if GTK_CHECK_VERSION(2,2,1)
static GtkTreeIter sort_method_alphabetical(GaimBlistNode *node, GaimBuddyList *blist, GtkTreeIter groupiter, GtkTreeIter *cur);
static GtkTreeIter sort_method_status(GaimBlistNode *node, GaimBuddyList *blist, GtkTreeIter groupiter, GtkTreeIter *cur);
static GtkTreeIter sort_method_log(GaimBlistNode *node, GaimBuddyList *blist, GtkTreeIter groupiter, GtkTreeIter *cur);
#endif
static GaimGtkBuddyList *gtkblist = NULL;

/* part of the best damn Docklet code this side of Tahiti */
static gboolean gaim_gtk_blist_obscured = FALSE;

static void gaim_gtk_blist_selection_changed(GtkTreeSelection *selection, gpointer data);
static void gaim_gtk_blist_update(GaimBuddyList *list, GaimBlistNode *node);
static char *gaim_get_tooltip_text(GaimBlistNode *node);
static char *item_factory_translate_func (const char *path, gpointer func_data);
static gboolean get_iter_from_node(GaimBlistNode *node, GtkTreeIter *iter);
static void redo_buddy_list(GaimBuddyList *list, gboolean remove);
static void gaim_gtk_blist_collapse_contact_cb(GtkWidget *w, GaimBlistNode *node);

static void show_rename_group(GtkWidget *unused, GaimGroup *g);

struct _gaim_gtk_blist_node {
	GtkTreeRowReference *row;
	gboolean contact_expanded;
};

#ifdef WANT_DROP_SHADOW
/**************************** Weird drop shadow stuff *******************/
/* This is based on a patch for drop shadows in GTK menus available at http://www.xfce.org/gtkmenu-shadow/ */

enum side {
  EAST_SIDE,
  SOUTH_SIDE
};

const double shadow_strip_l[5] = {
  .937, .831, .670, .478, .180
};

const double bottom_left_corner[25] = {
  1.00, .682, .423, .333, .258,
  1.00, .898, .800, .682, .584,
  1.00, .937, .874, .800, .737,
  1.00, .968, .937, .898, .866,
  1.00, .988, .976, .960, .945
};

const double bottom_right_corner[25] = {
  .258, .584, .737, .866, .945,
  .584, .682, .800, .898, .960,
  .737, .800, .874, .937, .976,
  .866, .898, .937, .968, .988,
  .945, .960, .976, .988, .996
};

const double top_right_corner[25] = {
  1.00, 1.00, 1.00, 1.00, 1.00,
  .686, .898, .937, .968, .988,
  .423, .803, .874, .937, .976,
  .333, .686, .800, .898, .960,
  .258, .584, .737, .866, .945
};

const double top_left_corner[25] = {
  .988, .968, .937, .898, .498,
  .976, .937, .874, .803, .423,
  .960, .898, .800, .686, .333,
  .945, .866, .737, .584, .258,
  .941, .847, .698, .521, .215
};


static GdkPixbuf *
get_pixbuf(GtkWidget *menu, int x, int y, int width, int height)
{
	GdkPixbuf *dest, *src;
	GdkScreen *screen = gtk_widget_get_screen (GTK_WIDGET(menu));
	GdkWindow *root = gdk_screen_get_root_window (screen);
	gint screen_height = gdk_screen_get_height (screen);
	gint screen_width = gdk_screen_get_width (screen);
	gint original_width = width;
	gint original_height = height;

#ifdef _WIN32
#if !GTK_CHECK_VERSION(2,4,8)
	/* XXX: Kill this entire block someday.
	 *
	 * 2004-08-22: This bug fix should land in GTK+ version 2.4.8:
	 *   http://bugzilla.gnome.org/show_bug.cgi?id=149013
	 *
	 * In Win32, GDK gets the workarea that isn't occupied by toolbars
	 * (including the taskbar) and uses that region as the screen size.
	 * GTK returns positions based on a screen size that ignores these
	 * toolbars.  Since we want a pixmap with real X,Y coordinates, we
	 * need to find out the offset from GTK's screen to GDK's screen,
	 * and adjust the pixmaps we grab accordingly.  GDK will not deal
	 * with toolbar position updates, so we're stuck restarting Gaim
	 * if that happens. - SimGuy
	 */
	RECT *workarea = g_malloc(sizeof(RECT));
	SystemParametersInfo(SPI_GETWORKAREA, 0, (void *)workarea, 0);
	x += (workarea->left);
	y += (workarea->top);
	g_free(workarea);
#endif
#endif

	if (x < 0) {
		width += x;
		x = 0;
	}

	if (y < 0) {
		height += y;
		y = 0;
	}

	if (x + width > screen_width) {
		width = screen_width - x;
	}

	if (y + height > screen_height) {
		height = screen_height - y;
	}

	if (width <= 0 || height <= 0)
		return NULL;

	dest = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8,
						  original_width, original_height);
	src = gdk_pixbuf_get_from_drawable(NULL, root, NULL, x, y, 0, 0,
									   width, height);
	gdk_pixbuf_copy_area (src, 0, 0, width, height, dest, 0, 0);

	g_object_unref (G_OBJECT (src));

	return dest;
}

static void
shadow_paint(GaimGtkBuddyList *blist, GdkRectangle *area, enum side shadow)
{
	gint width, height;
	GdkGC *gc = gtkblist->tipwindow->style->black_gc;

	switch (shadow) {
		case EAST_SIDE:
		if (gtkblist->east != NULL) {
			if (area)
				gdk_gc_set_clip_rectangle (gc, area);

			width = gdk_pixbuf_get_width (gtkblist->east);
			height = gdk_pixbuf_get_height (gtkblist->east);

			gdk_draw_pixbuf(GDK_DRAWABLE(gtkblist->east_shadow), gc,
				gtkblist->east, 0, 0, 0, 0, width, height,
				GDK_RGB_DITHER_NONE, 0, 0);

			if (area)
				gdk_gc_set_clip_rectangle (gc, NULL);
		}
		break;

		case SOUTH_SIDE:
		if (blist->south != NULL) {
			if (area)
				gdk_gc_set_clip_rectangle (gc, area);

			width = gdk_pixbuf_get_width (gtkblist->south);
			height = gdk_pixbuf_get_height (gtkblist->south);

			gdk_draw_pixbuf(GDK_DRAWABLE(gtkblist->south_shadow), gc,
				gtkblist->south, 0, 0, 0, 0, width, height,
				GDK_RGB_DITHER_NONE, 0, 0);

			if (area)
			gdk_gc_set_clip_rectangle (gc, NULL);
		}
		break;
	}
}

static void
pixbuf_add_shadow (GdkPixbuf *pb, enum side shadow)
{
	gint width, rowstride, height;
	gint i;
	guchar *pixels, *p;

	width = gdk_pixbuf_get_width (pb);
	height = gdk_pixbuf_get_height (pb);
	rowstride = gdk_pixbuf_get_rowstride (pb);
	pixels = gdk_pixbuf_get_pixels (pb);

	switch (shadow) {
		case EAST_SIDE:
		if (height > 5) {
			for (i = 0; i < width; i++) {
				gint j, k;

				p = pixels + (i * rowstride);
				for (j = 0, k = 0; j < 3 * width; j += 3, k++) {
					p[j] = (guchar) (p[j] * top_right_corner [i * width + k]);
					p[j + 1] = (guchar) (p[j + 1] * top_right_corner [i * width + k]);
					p[j + 2] = (guchar) (p[j + 2] * top_right_corner [i * width + k]);
				}
			}

			i = 5;
		} else {
			i = 0;
		}

		for (; i < height; i++) {
			gint j, k;

			p = pixels + (i * rowstride);
			for (j = 0, k = 0; j < 3 * width; j += 3, k++) {
				p[j] = (guchar) (p[j] * shadow_strip_l[width - 1 - k]);
				p[j + 1] = (guchar) (p[j + 1] * shadow_strip_l[width - 1 - k]);
				p[j + 2] = (guchar) (p[j + 2] * shadow_strip_l[width - 1 - k]);
			}
		}
		break;

		case SOUTH_SIDE:
			for (i = 0; i < height; i++) {
				gint j, k;

				p = pixels + (i * rowstride);
				for (j = 0, k = 0; j < 3 * height; j += 3, k++) {
					p[j] = (guchar) (p[j] * bottom_left_corner[i * height + k]);
					p[j + 1] = (guchar) (p[j + 1] * bottom_left_corner[i * height + k]);
					p[j + 2] = (guchar) (p[j + 2] * bottom_left_corner[i * height + k]);
				}

				p = pixels + (i * rowstride) + 3 * height;
				for (j = 0, k = 0; j < (width * 3) - (6 * height); j += 3, k++) {
					p[j] = (guchar) (p[j] * bottom_right_corner [i * height]);
					p[j + 1] = (guchar) (p[j + 1] * bottom_right_corner [i * height]);
					p[j + 2] = (guchar) (p[j + 2] * bottom_right_corner [i * height]);
				}

				p = pixels + (i * rowstride) + ((width * 3) - (3 * height));
				for (j = 0, k = 0; j < 3 * height; j += 3, k++) {
					p[j] = (guchar) (p[j] * bottom_right_corner[i * height + k]);
					p[j + 1] = (guchar) (p[j + 1] * bottom_right_corner[i * height + k]);
					p[j + 2] = (guchar) (p[j + 2] * bottom_right_corner[i * height + k]);
				}
			}
		break;
	}
}

static gboolean
map_shadow_windows (gpointer data)
{
	GaimGtkBuddyList *blist = (GaimGtkBuddyList*)data;
	GtkWidget *widget = blist->tipwindow;
	GdkPixbuf *pixbuf;
	int x, y;

	gtk_window_get_position(GTK_WINDOW(widget), &x, &y);
	pixbuf = get_pixbuf(widget,
						x + widget->allocation.width, y,
						5, widget->allocation.height + 5);
	if (pixbuf != NULL)
	{
		pixbuf_add_shadow (pixbuf, EAST_SIDE);
		if (blist->east != NULL)
		{
			g_object_unref (G_OBJECT (blist->east));
		}
		blist->east = pixbuf;
	}

	pixbuf = get_pixbuf (widget,
			x, y + widget->allocation.height,
			widget->allocation.width + 5, 5);
	if (pixbuf != NULL)
	{
		pixbuf_add_shadow (pixbuf, SOUTH_SIDE);
		if (blist->south != NULL)
		{
			g_object_unref (G_OBJECT (blist->south));
		}
		blist->south = pixbuf;
	}

	gdk_window_move_resize (blist->east_shadow,
				x + widget->allocation.width, MAX(0,y),
				5, MIN(widget->allocation.height, gdk_screen_height()));
	
	gdk_window_move_resize (blist->south_shadow,
				MAX(0,x), y + widget->allocation.height,
				MIN(widget->allocation.width + 5, gdk_screen_width()), 5);

	gdk_window_show (blist->east_shadow);
	gdk_window_show (blist->south_shadow);
	shadow_paint(blist, NULL, EAST_SIDE);
	shadow_paint(blist, NULL, SOUTH_SIDE);

	return FALSE;
}

/**************** END WEIRD DROP SHADOW STUFF ***********************************/
#endif

static GSList *blist_prefs_callbacks = NULL;

/***************************************************
 *              Callbacks                          *
 ***************************************************/

static gboolean gtk_blist_delete_cb(GtkWidget *w, GdkEventAny *event, gpointer data)
{
	if (docklet_count)
		gaim_blist_set_visible(FALSE);
	else
		gaim_core_quit();

	/* we handle everything, event should not propogate further */
	return TRUE;
}

static gboolean gtk_blist_configure_cb(GtkWidget *w, GdkEventConfigure *event, gpointer data)
{
	/* unfortunately GdkEventConfigure ignores the window gravity, but  *
	 * the only way we have of setting the position doesn't. we have to *
	 * call get_position because it does pay attention to the gravity.  *
	 * this is inefficient and I agree it sucks, but it's more likely   *
	 * to work correctly.                                    - Robot101 */
	gint x, y;

	/* check for visibility because when we aren't visible, this will   *
	 * give us bogus (0,0) coordinates.                      - xOr      */
	if (GTK_WIDGET_VISIBLE(w))
		gtk_window_get_position(GTK_WINDOW(w), &x, &y);
	else
		return FALSE; /* carry on normally */

	/* don't save if nothing changed */
	if (x == gaim_prefs_get_int("/gaim/gtk/blist/x") &&
		y == gaim_prefs_get_int("/gaim/gtk/blist/y") &&
		event->width  == gaim_prefs_get_int("/gaim/gtk/blist/width") &&
		event->height == gaim_prefs_get_int("/gaim/gtk/blist/height")) {

		return FALSE; /* carry on normally */
	}

	/* don't save off-screen positioning */
	if (x + event->width < 0 ||
		y + event->height < 0 ||
		x > gdk_screen_width() ||
		y > gdk_screen_height()) {

		return FALSE; /* carry on normally */
	}

	/* store the position */
	gaim_prefs_set_int("/gaim/gtk/blist/x",      x);
	gaim_prefs_set_int("/gaim/gtk/blist/y",      y);
	gaim_prefs_set_int("/gaim/gtk/blist/width",  event->width);
	gaim_prefs_set_int("/gaim/gtk/blist/height", event->height);

	/* continue to handle event normally */
	return FALSE;
}

static gboolean gtk_blist_visibility_cb(GtkWidget *w, GdkEventVisibility *event, gpointer data)
{
	if (event->state == GDK_VISIBILITY_FULLY_OBSCURED)
		gaim_gtk_blist_obscured = TRUE;
	else
		gaim_gtk_blist_obscured = FALSE;

	/* continue to handle event normally */
	return FALSE;
}

static void gtk_blist_menu_info_cb(GtkWidget *w, GaimBuddy *b)
{
	serv_get_info(b->account->gc, b->name);
}

static void gtk_blist_menu_im_cb(GtkWidget *w, GaimBuddy *b)
{
	gaim_gtkdialogs_im_with_user(b->account, b->name);
}

static void gtk_blist_menu_send_file_cb(GtkWidget *w, GaimBuddy *b)
{
	serv_send_file(b->account->gc, b->name, NULL);
}

static void gtk_blist_menu_autojoin_cb(GtkWidget *w, GaimChat *chat)
{
	gaim_blist_node_set_bool((GaimBlistNode*)chat, "gtk-autojoin",
			gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(w)));
}

static void gtk_blist_menu_join_cb(GtkWidget *w, GaimChat *chat)
{
	serv_join_chat(chat->account->gc, chat->components);
}

static void gtk_blist_menu_alias_cb(GtkWidget *w, GaimBlistNode *node)
{
	if(GAIM_BLIST_NODE_IS_BUDDY(node))
		gaim_gtkdialogs_alias_buddy((GaimBuddy*)node);
	else if(GAIM_BLIST_NODE_IS_CONTACT(node))
		gaim_gtkdialogs_alias_contact((GaimContact*)node);
	else if(GAIM_BLIST_NODE_IS_CHAT(node))
		gaim_gtkdialogs_alias_chat((GaimChat*)node);
}

static void gtk_blist_menu_bp_cb(GtkWidget *w, GaimBuddy *b)
{
	gaim_gtkpounce_dialog_show(b->account, b->name, NULL);
}

static void gtk_blist_menu_showlog_cb(GtkWidget *w, GaimBuddy *b)
{
	gaim_gtk_log_show(b->name, b->account);
}

static void gtk_blist_show_systemlog_cb()
{
	gaim_gtk_syslog_show();
}

static void gtk_blist_show_onlinehelp_cb()
{
	gaim_notify_uri(NULL, GAIM_WEBSITE "documentation.php");
}

static void gtk_blist_button_im_cb(GtkWidget *w, GtkTreeView *tv)
{
	GtkTreeIter iter;
	GtkTreeModel *model = gtk_tree_view_get_model(tv);
	GtkTreeSelection *sel = gtk_tree_view_get_selection(tv);

	if(gtk_tree_selection_get_selected(sel, &model, &iter)){
		GaimBlistNode *node;

		gtk_tree_model_get(GTK_TREE_MODEL(gtkblist->treemodel), &iter, NODE_COLUMN, &node, -1);
		if (GAIM_BLIST_NODE_IS_BUDDY(node)) {
			gaim_gtkdialogs_im_with_user(((GaimBuddy*)node)->account, ((GaimBuddy*)node)->name);
			return;
		} else if(GAIM_BLIST_NODE_IS_CONTACT(node)) {
			GaimBuddy *buddy =
				gaim_contact_get_priority_buddy((GaimContact*)node);
			gaim_gtkdialogs_im_with_user(buddy->account, buddy->name);
			return;
		}
	}
	gaim_gtkdialogs_im();
}

static void gtk_blist_button_info_cb(GtkWidget *w, GtkTreeView *tv)
{
	GtkTreeIter iter;
	GtkTreeModel *model = gtk_tree_view_get_model(tv);
	GtkTreeSelection *sel = gtk_tree_view_get_selection(tv);

	if(gtk_tree_selection_get_selected(sel, &model, &iter)){
		GaimBlistNode *node;

		gtk_tree_model_get(GTK_TREE_MODEL(gtkblist->treemodel), &iter, NODE_COLUMN, &node, -1);
		if (GAIM_BLIST_NODE_IS_BUDDY(node)) {
			serv_get_info(((GaimBuddy*)node)->account->gc, ((GaimBuddy*)node)->name);
			return;
		} else if(GAIM_BLIST_NODE_IS_CONTACT(node)) {
			GaimBuddy *buddy = gaim_contact_get_priority_buddy((GaimContact*)node);
			serv_get_info(buddy->account->gc, buddy->name);
			return;
		}
	}
	gaim_gtkdialogs_info();
}

static void
do_join_chat(GaimGtkJoinChatData *data)
{
	if (data)
	{
		GHashTable *components =
			g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
		GList *tmp;

		for (tmp = data->entries; tmp != NULL; tmp = tmp->next)
		{
			if (g_object_get_data(tmp->data, "is_spin"))
			{
				g_hash_table_replace(components,
					g_strdup(g_object_get_data(tmp->data, "identifier")),
					g_strdup_printf("%d",
							gtk_spin_button_get_value_as_int(tmp->data)));
			}
			else
			{
				g_hash_table_replace(components,
					g_strdup(g_object_get_data(tmp->data, "identifier")),
					g_strdup(gtk_entry_get_text(tmp->data)));
			}
		}

		serv_join_chat(gaim_account_get_connection(data->account), components);

		g_hash_table_destroy(components);
	}
}

static void
do_joinchat(GtkWidget *dialog, int id, GaimGtkJoinChatData *info)
{
	switch(id)
	{
		case GTK_RESPONSE_OK:
			do_join_chat(info);

		break;
	}

	gtk_widget_destroy(GTK_WIDGET(dialog));
	g_list_free(info->entries);
	g_free(info);
}

static void
rebuild_joinchat_entries(GaimGtkJoinChatData *data)
{
	GaimConnection *gc;
	GList *list, *tmp;
	GHashTable *defaults = NULL;
	struct proto_chat_entry *pce;
	gboolean focus = TRUE;

	gc = gaim_account_get_connection(data->account);

	while (GTK_BOX(data->entries_box)->children)
	{
		gtk_container_remove(GTK_CONTAINER(data->entries_box),
			((GtkBoxChild *)GTK_BOX(data->entries_box)->children->data)->widget);
	}

	if (data->entries != NULL)
		g_list_free(data->entries);

	data->entries = NULL;

	list = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl)->chat_info(gc);

	if (GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl)->chat_info_defaults != NULL)
		defaults = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl)->chat_info_defaults(gc, NULL);

	for (tmp = list; tmp; tmp = tmp->next)
	{
		GtkWidget *label;
		GtkWidget *rowbox;

		pce = tmp->data;

		rowbox = gtk_hbox_new(FALSE, 12);
		gtk_box_pack_start(GTK_BOX(data->entries_box), rowbox, FALSE, FALSE, 0);

		label = gtk_label_new_with_mnemonic(pce->label);
		gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
		gtk_size_group_add_widget(data->sg, label);
		gtk_box_pack_start(GTK_BOX(rowbox), label, FALSE, FALSE, 0);

		if (pce->is_int)
		{
			GtkObject *adjust;
			GtkWidget *spin;
			adjust = gtk_adjustment_new(pce->min, pce->min, pce->max,
										1, 10, 10);
			spin = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1, 0);
			g_object_set_data(G_OBJECT(spin), "is_spin",
							  GINT_TO_POINTER(TRUE));
			g_object_set_data(G_OBJECT(spin), "identifier", pce->identifier);
			data->entries = g_list_append(data->entries, spin);
			gtk_widget_set_size_request(spin, 50, -1);
			gtk_box_pack_end(GTK_BOX(rowbox), spin, FALSE, FALSE, 0);
			gtk_label_set_mnemonic_widget(GTK_LABEL(label), GTK_WIDGET(spin));
			gaim_set_accessible_label (spin, label);
		}
		else
		{
			GtkWidget *entry = gtk_entry_new();
			char *value;

			gtk_entry_set_activates_default(GTK_ENTRY(entry), TRUE);
			g_object_set_data(G_OBJECT(entry), "identifier", pce->identifier);
			data->entries = g_list_append(data->entries, entry);

			value = g_hash_table_lookup(defaults, pce->identifier);
			if (value != NULL)
				gtk_entry_set_text(GTK_ENTRY(entry), value);

			if (focus)
			{
				gtk_widget_grab_focus(entry);
				focus = FALSE;
			}

			if (pce->secret)
				gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);

			gtk_box_pack_end(GTK_BOX(rowbox), entry, TRUE, TRUE, 0);
			gtk_label_set_mnemonic_widget(GTK_LABEL(label), GTK_WIDGET(entry));
			gaim_set_accessible_label (entry, label);
		}

		g_free(pce);
	}

	g_list_free(list);

	gtk_widget_show_all(data->entries_box);
}

static void
joinchat_select_account_cb(GObject *w, GaimAccount *account,
							GaimGtkJoinChatData *data)
{
	if (strcmp(gaim_account_get_protocol_id(data->account),
				gaim_account_get_protocol_id(account)) == 0)
	{
		data->account = account;
	}
	else
	{
		data->account = account;
		rebuild_joinchat_entries(data);
	}
}

static gboolean
joinchat_account_filter_func(GaimAccount *account)
{
	GaimConnection *gc = gaim_account_get_connection(account);
	GaimPluginProtocolInfo *prpl_info = NULL;

	prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	return (prpl_info->chat_info != NULL);
}

gboolean
gaim_gtk_blist_joinchat_is_showable()
{
	GList *c;
	GaimConnection *gc;

	for (c = gaim_connections_get_all(); c != NULL; c = c->next) {
		gc = c->data;

		if (joinchat_account_filter_func(gaim_connection_get_account(gc)))
			return TRUE;
	}

	return FALSE;
}

void
gaim_gtk_blist_joinchat_show(void)
{
	GtkWidget *hbox, *vbox;
	GtkWidget *rowbox;
	GtkWidget *label;
	GaimGtkBuddyList *gtkblist;
	GtkWidget *img = NULL;
	GaimGtkJoinChatData *data = NULL;

	gtkblist = GAIM_GTK_BLIST(gaim_get_blist());
	img = gtk_image_new_from_stock(GAIM_STOCK_DIALOG_QUESTION,
								   GTK_ICON_SIZE_DIALOG);
	data = g_new0(GaimGtkJoinChatData, 1);

	data->window = gtk_dialog_new_with_buttons(_("Join a Chat"),
		NULL, GTK_DIALOG_NO_SEPARATOR,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GAIM_STOCK_CHAT, GTK_RESPONSE_OK, NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(data->window), GTK_RESPONSE_OK);
	gtk_container_set_border_width(GTK_CONTAINER(data->window), 6);
	gtk_window_set_resizable(GTK_WINDOW(data->window), FALSE);
	gtk_box_set_spacing(GTK_BOX(GTK_DIALOG(data->window)->vbox), 12);
	gtk_container_set_border_width(
		GTK_CONTAINER(GTK_DIALOG(data->window)->vbox), 6);
	gtk_window_set_role(GTK_WINDOW(data->window), "join_chat");

	hbox = gtk_hbox_new(FALSE, 12);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(data->window)->vbox), hbox);
	gtk_box_pack_start(GTK_BOX(hbox), img, FALSE, FALSE, 0);
	gtk_misc_set_alignment(GTK_MISC(img), 0, 0);

	vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
	gtk_container_add(GTK_CONTAINER(hbox), vbox);

	label = gtk_label_new(_("Please enter the appropriate information "
							"about the chat you would like to join.\n"));
	gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

	rowbox = gtk_hbox_new(FALSE, 12);
	gtk_box_pack_start(GTK_BOX(vbox), rowbox, TRUE, TRUE, 0);

	data->sg = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);

	label = gtk_label_new_with_mnemonic(_("_Account:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_box_pack_start(GTK_BOX(rowbox), label, FALSE, FALSE, 0);
	gtk_size_group_add_widget(data->sg, label);

	data->account_menu = gaim_gtk_account_option_menu_new(NULL, FALSE,
			G_CALLBACK(joinchat_select_account_cb),
			joinchat_account_filter_func, data);
	gtk_box_pack_start(GTK_BOX(rowbox), data->account_menu, TRUE, TRUE, 0);
	gtk_label_set_mnemonic_widget(GTK_LABEL(label),
								  GTK_WIDGET(data->account_menu));
	gaim_set_accessible_label (data->account_menu, label);

	data->entries_box = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(vbox), data->entries_box);
	gtk_container_set_border_width(GTK_CONTAINER(data->entries_box), 0);

	data->account =	gaim_gtk_account_option_menu_get_selected(data->account_menu);

	rebuild_joinchat_entries(data);

	g_signal_connect(G_OBJECT(data->window), "response",
					 G_CALLBACK(do_joinchat), data);

	g_object_unref(data->sg);

	gtk_widget_show_all(data->window);
}

static void gtk_blist_button_chat_cb(GtkWidget *w, GtkTreeView *tv)
{
	GtkTreeIter iter;
	GtkTreeModel *model = gtk_tree_view_get_model(tv);
	GtkTreeSelection *sel = gtk_tree_view_get_selection(tv);

	if(gtk_tree_selection_get_selected(sel, &model, &iter)){
		GaimBlistNode *node;

		gtk_tree_model_get(GTK_TREE_MODEL(gtkblist->treemodel), &iter, NODE_COLUMN, &node, -1);
		if (GAIM_BLIST_NODE_IS_CHAT(node)) {
			serv_join_chat(((GaimChat *)node)->account->gc, ((GaimChat *)node)->components);
			return;
		}
	}
	gaim_gtk_blist_joinchat_show();
}

static void gtk_blist_button_away_cb(GtkWidget *w, gpointer data)
{
	gtk_menu_popup(GTK_MENU(awaymenu), NULL, NULL, NULL, NULL, 1, GDK_CURRENT_TIME);
}

static void gtk_blist_row_expanded_cb(GtkTreeView *tv, GtkTreeIter *iter, GtkTreePath *path, gpointer user_data) {
	GaimBlistNode *node;
	GValue val = {0,};

	gtk_tree_model_get_value(GTK_TREE_MODEL(gtkblist->treemodel), iter, NODE_COLUMN, &val);

	node = g_value_get_pointer(&val);

	if (GAIM_BLIST_NODE_IS_GROUP(node)) {
		gaim_blist_node_set_bool(node, "collapsed", FALSE);
	}
}

static void gtk_blist_row_collapsed_cb(GtkTreeView *tv, GtkTreeIter *iter, GtkTreePath *path, gpointer user_data) {
	GaimBlistNode *node;
	GValue val = {0,};

	gtk_tree_model_get_value(GTK_TREE_MODEL(gtkblist->treemodel), iter, NODE_COLUMN, &val);

	node = g_value_get_pointer(&val);

	if (GAIM_BLIST_NODE_IS_GROUP(node)) {
		gaim_blist_node_set_bool(node, "collapsed", TRUE);
	} else if(GAIM_BLIST_NODE_IS_CONTACT(node)) {
		gaim_gtk_blist_collapse_contact_cb(NULL, node);
	}
}

static void gtk_blist_row_activated_cb(GtkTreeView *tv, GtkTreePath *path, GtkTreeViewColumn *col, gpointer data) {
	GaimBlistNode *node;
	GtkTreeIter iter;
	GValue val = { 0, };

	gtk_tree_model_get_iter(GTK_TREE_MODEL(gtkblist->treemodel), &iter, path);

	gtk_tree_model_get_value (GTK_TREE_MODEL(gtkblist->treemodel), &iter, NODE_COLUMN, &val);
	node = g_value_get_pointer(&val);

	if(GAIM_BLIST_NODE_IS_CONTACT(node) || GAIM_BLIST_NODE_IS_BUDDY(node)) {
		GaimBuddy *buddy;

		if(GAIM_BLIST_NODE_IS_CONTACT(node))
			buddy = gaim_contact_get_priority_buddy((GaimContact*)node);
		else
			buddy = (GaimBuddy*)node;

		gaim_gtkdialogs_im_with_user(buddy->account, buddy->name);
	} else if (GAIM_BLIST_NODE_IS_CHAT(node)) {
		serv_join_chat(((GaimChat *)node)->account->gc, ((GaimChat *)node)->components);
	} else if (GAIM_BLIST_NODE_IS_GROUP(node)) {
		if (gtk_tree_view_row_expanded(tv, path))
			gtk_tree_view_collapse_row(tv, path);
		else
			gtk_tree_view_expand_row(tv,path,FALSE);
	}
}

static void gaim_gtk_blist_add_chat_cb()
{
	GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(gtkblist->treeview));
	GtkTreeIter iter;
	GaimBlistNode *node;

	if(gtk_tree_selection_get_selected(sel, NULL, &iter)){
		gtk_tree_model_get(GTK_TREE_MODEL(gtkblist->treemodel), &iter, NODE_COLUMN, &node, -1);
		if (GAIM_BLIST_NODE_IS_BUDDY(node))
			gaim_blist_request_add_chat(NULL, (GaimGroup*)node->parent->parent, NULL, NULL);
		if (GAIM_BLIST_NODE_IS_CONTACT(node) || GAIM_BLIST_NODE_IS_CHAT(node))
			gaim_blist_request_add_chat(NULL, (GaimGroup*)node->parent, NULL, NULL);
		else if (GAIM_BLIST_NODE_IS_GROUP(node))
			gaim_blist_request_add_chat(NULL, (GaimGroup*)node, NULL, NULL);
	}
	else {
		gaim_blist_request_add_chat(NULL, NULL, NULL, NULL);
	}
}

static void gaim_gtk_blist_add_buddy_cb()
{
	GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(gtkblist->treeview));
	GtkTreeIter iter;
	GaimBlistNode *node;

	if(gtk_tree_selection_get_selected(sel, NULL, &iter)){
		gtk_tree_model_get(GTK_TREE_MODEL(gtkblist->treemodel), &iter, NODE_COLUMN, &node, -1);
		if (GAIM_BLIST_NODE_IS_BUDDY(node)) {
			gaim_blist_request_add_buddy(NULL, NULL, ((GaimGroup*)node->parent->parent)->name,
					NULL);
		} else if (GAIM_BLIST_NODE_IS_CONTACT(node)
				|| GAIM_BLIST_NODE_IS_CHAT(node)) {
			gaim_blist_request_add_buddy(NULL, NULL, ((GaimGroup*)node->parent)->name, NULL);
		} else if (GAIM_BLIST_NODE_IS_GROUP(node)) {
			gaim_blist_request_add_buddy(NULL, NULL, ((GaimGroup*)node)->name, NULL);
		}
	}
	else {
		gaim_blist_request_add_buddy(NULL, NULL, NULL, NULL);
	}
}

static void
gaim_gtk_blist_remove_cb (GtkWidget *w, GaimBlistNode *node)
{
	if (GAIM_BLIST_NODE_IS_BUDDY(node)) {
		gaim_gtkdialogs_remove_buddy((GaimBuddy*)node);
	} else if (GAIM_BLIST_NODE_IS_CHAT(node)) {
		gaim_gtkdialogs_remove_chat((GaimChat*)node);
	} else if (GAIM_BLIST_NODE_IS_GROUP(node)) {
		gaim_gtkdialogs_remove_group((GaimGroup*)node);
	} else if (GAIM_BLIST_NODE_IS_CONTACT(node)) {
		gaim_gtkdialogs_remove_contact((GaimContact*)node);
	}
}

static void
gaim_gtk_blist_expand_contact_cb(GtkWidget *w, GaimBlistNode *node)
{
	struct _gaim_gtk_blist_node *gtknode;
	GtkTreeIter iter, parent;
	GaimBlistNode *bnode;
	GtkTreePath *path;		

	if(!GAIM_BLIST_NODE_IS_CONTACT(node))
		return;

	gtknode = (struct _gaim_gtk_blist_node *)node->ui_data;

	gtknode->contact_expanded = TRUE;

	for(bnode = node->child; bnode; bnode = bnode->next) {
		gaim_gtk_blist_update(NULL, bnode);
	}
	
	/* This ensures that the bottom buddy is visible, i.e. not scrolled off the alignment */
	get_iter_from_node(node, &parent);
	gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(gtkblist->treemodel), &iter, &parent, 
			  gtk_tree_model_iter_n_children(GTK_TREE_MODEL(gtkblist->treemodel), &parent) -1);
	path = gtk_tree_model_get_path(GTK_TREE_MODEL(gtkblist->treemodel), &iter);
	/* Let the treeview draw so it knows where to scroll */
	while (gtk_events_pending())
		gtk_main_iteration();
	gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW(gtkblist->treeview), path, NULL, FALSE, 0, 0); 
	

	gaim_gtk_blist_update(NULL, node);
	gtk_tree_path_free(path);
}

static void
gaim_gtk_blist_collapse_contact_cb(GtkWidget *w, GaimBlistNode *node)
{
	GaimBlistNode *bnode;
	struct _gaim_gtk_blist_node *gtknode;

	if(!GAIM_BLIST_NODE_IS_CONTACT(node))
		return;

	gtknode = (struct _gaim_gtk_blist_node *)node->ui_data;

	gtknode->contact_expanded = FALSE;

	for(bnode = node->child; bnode; bnode = bnode->next) {
		gaim_gtk_blist_update(NULL, bnode);
	}
}


static void
blist_node_menu_cb(GtkMenuItem *item, GaimBlistNode *node)
{
	GaimBlistNodeAction *act;
	act = (GaimBlistNodeAction *) g_object_get_data(G_OBJECT(item),
			"gaimcallback");
	if(act->callback)
		act->callback(node, act->data);
}


static void
append_blist_node_action (GtkWidget *menu, GaimBlistNodeAction *act,
		GaimBlistNode *node, gboolean *dup_separator)
{
	if(act == NULL) {
		if(! *dup_separator) {
			gaim_separator(menu);
			*dup_separator = TRUE;
		}
	} else {
		GtkWidget *menuitem;

		*dup_separator = FALSE;

		menuitem = gtk_menu_item_new_with_mnemonic(act->label);
		g_object_set_data(G_OBJECT(menuitem), "gaimcallback", act);
		g_signal_connect(G_OBJECT(menuitem), "activate",
				G_CALLBACK(blist_node_menu_cb), node);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	}
}


void
gaim_gtk_append_blist_node_proto_menu (GtkWidget *menu, GaimConnection *gc, GaimBlistNode *node)
{
	GList *l, *ll;
	gboolean dup_separator = FALSE;
	GaimPluginProtocolInfo *prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	if(!prpl_info || !prpl_info->blist_node_menu)
		return;

	for(l = ll = prpl_info->blist_node_menu(node); l; l = l->next) {
		GaimBlistNodeAction *act = (GaimBlistNodeAction *) l->data;
		append_blist_node_action(menu, act, node, &dup_separator);
	}
	g_list_free(ll);
}


void
gaim_gtk_append_blist_node_extended_menu (GtkWidget *menu, GaimBlistNode *node)
{
	GList *l, *ll;
	gboolean dup_separator = FALSE;

	for(l = ll = gaim_blist_node_get_extended_menu(node); l; l = l->next) {
		GaimBlistNodeAction *act = (GaimBlistNodeAction *) l->data;
		append_blist_node_action(menu, act, node, &dup_separator);
	}
	g_list_free(ll);
}


void
gaim_gtk_blist_make_buddy_menu(GtkWidget *menu, GaimBuddy *buddy) {
	GaimPluginProtocolInfo *prpl_info;

	g_return_if_fail(menu);
	g_return_if_fail(buddy);

	prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(buddy->account->gc->prpl);

	if (prpl_info && prpl_info->get_info) {
		gaim_new_item_from_stock(menu, _("Get _Info"), GAIM_STOCK_INFO,
				G_CALLBACK(gtk_blist_menu_info_cb), buddy, 0, 0, NULL);
	}
	gaim_new_item_from_stock(menu, _("I_M"), GAIM_STOCK_IM,
			G_CALLBACK(gtk_blist_menu_im_cb), buddy, 0, 0, NULL);
	if (prpl_info && prpl_info->send_file) {
		if (!prpl_info->can_receive_file ||
			prpl_info->can_receive_file(buddy->account->gc, buddy->name))
		{
			gaim_new_item_from_stock(menu, _("_Send File"),
									 GAIM_STOCK_FILE_TRANSFER,
									 G_CALLBACK(gtk_blist_menu_send_file_cb),
									 buddy, 0, 0, NULL);
		}
	}
	gaim_new_item_from_stock(menu, _("Add Buddy _Pounce"), NULL,
			G_CALLBACK(gtk_blist_menu_bp_cb), buddy, 0, 0, NULL);
	gaim_new_item_from_stock(menu, _("View _Log"), NULL,
			G_CALLBACK(gtk_blist_menu_showlog_cb), buddy, 0, 0, NULL);

	gaim_gtk_append_blist_node_proto_menu(menu, buddy->account->gc,
										  (GaimBlistNode *)buddy);
	gaim_gtk_append_blist_node_extended_menu(menu, (GaimBlistNode *)buddy);

	gaim_separator(menu);

	gaim_new_item_from_stock(menu, _("_Alias..."), GAIM_STOCK_ALIAS,
			G_CALLBACK(gtk_blist_menu_alias_cb), buddy, 0, 0, NULL);
	gaim_new_item_from_stock(menu, _("_Remove"), GTK_STOCK_REMOVE,
			G_CALLBACK(gaim_gtk_blist_remove_cb), buddy,
			0, 0, NULL);
}

static gboolean
gtk_blist_key_press_cb(GtkWidget *tv, GdkEventKey *event, gpointer data) {
	GaimBlistNode *node;
	GValue val = { 0, };
	GtkTreeIter iter;
	GtkTreeSelection *sel;

	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tv));
	if(!gtk_tree_selection_get_selected(sel, NULL, &iter))
		return FALSE;

	gtk_tree_model_get_value(GTK_TREE_MODEL(gtkblist->treemodel), &iter,
			NODE_COLUMN, &val);
	node = g_value_get_pointer(&val);

	if(event->state & GDK_CONTROL_MASK &&
			(event->keyval == 'o' || event->keyval == 'O')) {
		GaimBuddy *buddy;

		if(GAIM_BLIST_NODE_IS_CONTACT(node)) {
			buddy = gaim_contact_get_priority_buddy((GaimContact*)node);
		} else if(GAIM_BLIST_NODE_IS_BUDDY(node)) {
			buddy = (GaimBuddy*)node;
		} else {
			return FALSE;
		}
		if(buddy)
			serv_get_info(buddy->account->gc, buddy->name);
	}

	return FALSE;
}


static GtkWidget *
create_group_menu (GaimBlistNode *node, GaimGroup *g)
{
	GtkWidget *menu;

	menu = gtk_menu_new();
	gaim_new_item_from_stock(menu, _("Add a _Buddy"), GTK_STOCK_ADD,
				 G_CALLBACK(gaim_gtk_blist_add_buddy_cb), node, 0, 0, NULL);
	gaim_new_item_from_stock(menu, _("Add a C_hat"), GTK_STOCK_ADD,
				 G_CALLBACK(gaim_gtk_blist_add_chat_cb), node, 0, 0, NULL);
	gaim_new_item_from_stock(menu, _("_Delete Group"), GTK_STOCK_REMOVE,
				 G_CALLBACK(gaim_gtk_blist_remove_cb), node, 0, 0, NULL);
	gaim_new_item_from_stock(menu, _("_Rename"), NULL,
				 G_CALLBACK(show_rename_group), node, 0, 0, NULL);

	gaim_gtk_append_blist_node_extended_menu(menu, node);

	return menu;
}


static GtkWidget *
create_chat_menu(GaimBlistNode *node, GaimChat *c) {
	GtkWidget *menu;
	gboolean autojoin;

	menu = gtk_menu_new();
	autojoin = (gaim_blist_node_get_bool(node, "gtk-autojoin") ||
			(gaim_blist_node_get_string(node, "gtk-autojoin") != NULL));

	gaim_new_item_from_stock(menu, _("_Join"), GAIM_STOCK_CHAT,
			G_CALLBACK(gtk_blist_menu_join_cb), node, 0, 0, NULL);
	gaim_new_check_item(menu, _("Auto-Join"),
			G_CALLBACK(gtk_blist_menu_autojoin_cb), node, autojoin);

	gaim_gtk_append_blist_node_proto_menu(menu, c->account->gc, node);
	gaim_gtk_append_blist_node_extended_menu(menu, node);

	gaim_separator(menu);

	gaim_new_item_from_stock(menu, _("_Alias..."), GAIM_STOCK_ALIAS,
				 G_CALLBACK(gtk_blist_menu_alias_cb), node, 0, 0, NULL);
	gaim_new_item_from_stock(menu, _("_Remove"), GTK_STOCK_REMOVE,
				 G_CALLBACK(gaim_gtk_blist_remove_cb), node, 0, 0, NULL);

	return menu;
}

static GtkWidget *
create_contact_menu (GaimBlistNode *node)
{
	GtkWidget *menu;

	menu = gtk_menu_new();
	gaim_new_item_from_stock(menu, _("_Alias..."), GAIM_STOCK_ALIAS,
				 G_CALLBACK(gtk_blist_menu_alias_cb), node, 0, 0, NULL);
	gaim_new_item_from_stock(menu, _("_Collapse"), GTK_STOCK_ZOOM_OUT,
				 G_CALLBACK(gaim_gtk_blist_collapse_contact_cb),
				 node, 0, 0, NULL);
	gaim_new_item_from_stock(menu, _("_Remove"), GTK_STOCK_REMOVE,
				 G_CALLBACK(gaim_gtk_blist_remove_cb), node, 0, 0, NULL);

	gaim_gtk_append_blist_node_extended_menu(menu, node);

	return menu;
}

static GtkWidget *
create_buddy_menu(GaimBlistNode *node, GaimBuddy *b) {
	struct _gaim_gtk_blist_node *gtknode = (struct _gaim_gtk_blist_node *)node->ui_data;
	GtkWidget *menu;
	GtkWidget *menuitem;
	gboolean show_offline = gaim_prefs_get_bool("/gaim/gtk/blist/show_offline_buddies");

	menu = gtk_menu_new();
	gaim_gtk_blist_make_buddy_menu(menu, b);

	if(GAIM_BLIST_NODE_IS_CONTACT(node)) {
		gaim_separator(menu);

		if(gtknode->contact_expanded) {
			gaim_new_item_from_stock(menu, _("_Collapse"),
						 GTK_STOCK_ZOOM_OUT,
						 G_CALLBACK(gaim_gtk_blist_collapse_contact_cb),
						 node, 0, 0, NULL);
		} else {
			gaim_new_item_from_stock(menu, _("_Expand"),
						 GTK_STOCK_ZOOM_IN,
						 G_CALLBACK(gaim_gtk_blist_expand_contact_cb), node,
						 0, 0, NULL);
		}
		if(node->child->next) {
			GaimBlistNode *bnode;

			for(bnode = node->child; bnode; bnode = bnode->next) {
				GaimBuddy *buddy = (GaimBuddy*)bnode;
				GdkPixbuf *buf;
				GtkWidget *submenu;
				GtkWidget *image;

				if(buddy == b)
					continue;
				if(!buddy->account->gc)
					continue;
				if(!show_offline && !GAIM_BUDDY_IS_ONLINE(buddy))
					continue;

				menuitem = gtk_image_menu_item_new_with_label(buddy->name);
				buf = gaim_gtk_blist_get_status_icon(bnode,
										GAIM_STATUS_ICON_SMALL);
				image = gtk_image_new_from_pixbuf(buf);
				g_object_unref(G_OBJECT(buf));
				gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menuitem),
											  image);
				gtk_widget_show(image);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
				gtk_widget_show(menuitem);

				submenu = gtk_menu_new();
				gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), submenu);
				gtk_widget_show(submenu);

				gaim_gtk_blist_make_buddy_menu(submenu, buddy);
			}
		}
	}
	return menu;
}

static gboolean
gaim_gtk_blist_show_context_menu(GaimBlistNode *node,
								 GtkMenuPositionFunc func,
								 GtkWidget *tv,
								 guint button,
								 guint32 time)
{
	struct _gaim_gtk_blist_node *gtknode;
	GtkWidget *menu = NULL;
	gboolean handled = FALSE;

	gtknode = (struct _gaim_gtk_blist_node *)node->ui_data;

	/* Create a menu based on the thing we right-clicked on */
	if (GAIM_BLIST_NODE_IS_GROUP(node)) {
		GaimGroup *g = (GaimGroup *)node;

		menu = create_group_menu(node, g);
	} else if (GAIM_BLIST_NODE_IS_CHAT(node)) {
		GaimChat *c = (GaimChat *)node;

		menu = create_chat_menu(node, c);
	} else if ((GAIM_BLIST_NODE_IS_CONTACT(node)) && (gtknode->contact_expanded)) {
		menu = create_contact_menu(node);
	} else if (GAIM_BLIST_NODE_IS_CONTACT(node) || GAIM_BLIST_NODE_IS_BUDDY(node)) {
		GaimBuddy *b;

		if (GAIM_BLIST_NODE_IS_CONTACT(node))
			b = gaim_contact_get_priority_buddy((GaimContact*)node);
		else
			b = (GaimBuddy *)node;

		menu = create_buddy_menu(node, b);
	}

	/* Now display the menu */
	if (menu != NULL) {
		gtk_widget_show_all(menu);
		gtk_menu_popup(GTK_MENU(menu), NULL, NULL, func, tv, button, time);
		handled = TRUE;
	}

	return handled;
}

static gboolean gtk_blist_button_press_cb(GtkWidget *tv, GdkEventButton *event, gpointer user_data)
{
	GtkTreePath *path;
	GaimBlistNode *node;
	GValue val = { 0, };
	GtkTreeIter iter;
	GtkTreeSelection *sel;
	GaimPlugin *prpl = NULL;
	GaimPluginProtocolInfo *prpl_info = NULL;
	struct _gaim_gtk_blist_node *gtknode;
	gboolean handled = FALSE;

	/* Here we figure out which node was clicked */
	if (!gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(tv), event->x, event->y, &path, NULL, NULL, NULL))
		return FALSE;
	gtk_tree_model_get_iter(GTK_TREE_MODEL(gtkblist->treemodel), &iter, path);
	gtk_tree_model_get_value(GTK_TREE_MODEL(gtkblist->treemodel), &iter, NODE_COLUMN, &val);
	node = g_value_get_pointer(&val);
	gtknode = (struct _gaim_gtk_blist_node *)node->ui_data;

	/* Right click draws a context menu */
	if ((event->button == 3) && (event->type == GDK_BUTTON_PRESS)) {
		handled = gaim_gtk_blist_show_context_menu(node, NULL, tv, 3, event->time);

	/* CTRL+middle click expands or collapse a contact */
	} else if ((event->button == 2) && (event->type == GDK_BUTTON_PRESS) &&
			   (event->state & GDK_CONTROL_MASK) && (GAIM_BLIST_NODE_IS_CONTACT(node))) {
		if (gtknode->contact_expanded)
			gaim_gtk_blist_collapse_contact_cb(NULL, node);
		else
			gaim_gtk_blist_expand_contact_cb(NULL, node);
		handled = TRUE;

	/* Double middle click gets info */
	} else if ((event->button == 2) && (event->type == GDK_2BUTTON_PRESS) &&
			   ((GAIM_BLIST_NODE_IS_CONTACT(node)) || (GAIM_BLIST_NODE_IS_BUDDY(node)))) {
		GaimBuddy *b;
		if(GAIM_BLIST_NODE_IS_CONTACT(node))
			b = gaim_contact_get_priority_buddy((GaimContact*)node);
		else
			b = (GaimBuddy *)node;

		prpl = gaim_find_prpl(gaim_account_get_protocol_id(b->account));
		if (prpl != NULL)
			prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(prpl);

		if (prpl && prpl_info->get_info)
			serv_get_info(b->account->gc, b->name);
		handled = TRUE;
	}

#if (1)
	/*
	 * This code only exists because GTK doesn't work.  If we return
	 * FALSE here, as would be normal the event propoagates down and
	 * somehow gets interpreted as the start of a drag event.
	 *
	 * Um, isn't it _normal_ to return TRUE here?  Since the event
	 * was handled?  --Mark
	 */
	if(handled) {
		sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tv));
		gtk_tree_selection_select_path(sel, path);
		gtk_tree_path_free(path);
		return TRUE;
	}
#endif
	gtk_tree_path_free(path);

	return FALSE;
}

static gboolean
gaim_gtk_blist_popup_menu_cb(GtkWidget *tv, void *user_data)
{
	GaimBlistNode *node;
	GValue val = { 0, };
	GtkTreeIter iter;
	GtkTreeSelection *sel;
	gboolean handled = FALSE;

	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tv));
	if (!gtk_tree_selection_get_selected(sel, NULL, &iter))
		return FALSE;

	gtk_tree_model_get_value(GTK_TREE_MODEL(gtkblist->treemodel),
							 &iter, NODE_COLUMN, &val);
	node = g_value_get_pointer(&val);

	/* Shift+F10 draws a context menu */
	handled = gaim_gtk_blist_show_context_menu(node, gaim_gtk_treeview_popup_menu_position_func, tv, 0, GDK_CURRENT_TIME);

	return handled;
}

static void gaim_gtk_blist_show_empty_groups_cb(gpointer data, guint action, GtkWidget *item)
{
	gaim_prefs_set_bool("/gaim/gtk/blist/show_empty_groups",
			gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(item)));
}

static void gaim_gtk_blist_edit_mode_cb(gpointer callback_data, guint callback_action,
		GtkWidget *checkitem) {
	if(gtkblist->window->window) {
		GdkCursor *cursor = gdk_cursor_new(GDK_WATCH);
		gdk_window_set_cursor(gtkblist->window->window, cursor);
		while (gtk_events_pending())
			gtk_main_iteration();
		gdk_cursor_unref(cursor);
	}

	gaim_prefs_set_bool("/gaim/gtk/blist/show_offline_buddies",
			gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(checkitem)));

	if(gtkblist->window->window) {
		GdkCursor *cursor = gdk_cursor_new(GDK_LEFT_PTR);
		gdk_window_set_cursor(gtkblist->window->window, cursor);
		gdk_cursor_unref(cursor);
	}
}

static void
add_buddies_from_vcard(const char *prpl_id, GaimGroup *group, GList *list,
					   const char *alias)
{
	GList *l;
	GaimAccount *account = NULL;
	GaimConnection *gc;

	if (list == NULL)
		return;

	for (l = gaim_connections_get_all(); l != NULL; l = l->next)
	{
		gc = (GaimConnection *)l->data;
		account = gaim_connection_get_account(gc);

		if (!strcmp(gaim_account_get_protocol_id(account), prpl_id))
			break;

		account = NULL;
	}

	if (account != NULL)
	{
		for (l = list; l != NULL; l = l->next)
		{
			gaim_blist_request_add_buddy(account, l->data,
										 (group ? group->name : NULL),
										 alias);
		}
	}

	g_list_foreach(list, (GFunc)g_free, NULL);
	g_list_free(list);
}

static gboolean
parse_vcard(const char *vcard, GaimGroup *group)
{
	char *temp_vcard;
	char *s, *c;
	char *alias    = NULL;
	GList *aims    = NULL;
	GList *icqs    = NULL;
	GList *yahoos  = NULL;
	GList *msns    = NULL;
	GList *jabbers = NULL;

	s = temp_vcard = g_strdup(vcard);

	while (*s != '\0' && strncmp(s, "END:vCard", strlen("END:vCard")))
	{
		char *field, *value;

		field = s;

		/* Grab the field */
		while (*s != '\r' && *s != '\n' && *s != '\0' && *s != ':')
			s++;

		if (*s == '\r') s++;
		if (*s == '\n')
		{
			s++;
			continue;
		}

		if (*s != '\0') *s++ = '\0';

		if ((c = strchr(field, ';')) != NULL)
			*c = '\0';

		/* Proceed to the end of the line */
		value = s;

		while (*s != '\r' && *s != '\n' && *s != '\0')
			s++;

		if (*s == '\r') *s++ = '\0';
		if (*s == '\n') *s++ = '\0';

		/* We only want to worry about a few fields here. */
		if (!strcmp(field, "FN"))
			alias = g_strdup(value);
		else if (!strcmp(field, "X-AIM") || !strcmp(field, "X-ICQ") ||
				 !strcmp(field, "X-YAHOO") || !strcmp(field, "X-MSN") ||
				 !strcmp(field, "X-JABBER"))
		{
			char **values = g_strsplit(value, ":", 0);
			char **im;

			for (im = values; *im != NULL; im++)
			{
				if (!strcmp(field, "X-AIM"))
					aims = g_list_append(aims, g_strdup(*im));
				else if (!strcmp(field, "X-ICQ"))
					icqs = g_list_append(icqs, g_strdup(*im));
				else if (!strcmp(field, "X-YAHOO"))
					yahoos = g_list_append(yahoos, g_strdup(*im));
				else if (!strcmp(field, "X-MSN"))
					msns = g_list_append(msns, g_strdup(*im));
				else if (!strcmp(field, "X-JABBER"))
					jabbers = g_list_append(jabbers, g_strdup(*im));
			}

			g_strfreev(values);
		}
	}

	g_free(temp_vcard);

	if (aims == NULL && icqs == NULL && yahoos == NULL &&
		msns == NULL && jabbers == NULL)
	{
		if (alias != NULL)
			g_free(alias);

		return FALSE;
	}

	add_buddies_from_vcard("prpl-oscar",  group, aims,    alias);
	add_buddies_from_vcard("prpl-oscar",  group, icqs,    alias);
	add_buddies_from_vcard("prpl-yahoo",  group, yahoos,  alias);
	add_buddies_from_vcard("prpl-msn",    group, msns,    alias);
	add_buddies_from_vcard("prpl-jabber", group, jabbers, alias);

	if (alias != NULL)
		g_free(alias);

	return TRUE;
}

static void gaim_gtk_blist_drag_data_get_cb(GtkWidget *widget,
											GdkDragContext *dc,
											GtkSelectionData *data,
											guint info,
											guint time,
											gpointer *null)
{
	if (data->target == gdk_atom_intern("GAIM_BLIST_NODE", FALSE))
	{
		GtkTreeRowReference *ref = g_object_get_data(G_OBJECT(dc), "gtk-tree-view-source-row");
		GtkTreePath *sourcerow = gtk_tree_row_reference_get_path(ref);
		GtkTreeIter iter;
		GaimBlistNode *node = NULL;
		GValue val = {0};
		if(!sourcerow)
			return;
		gtk_tree_model_get_iter(GTK_TREE_MODEL(gtkblist->treemodel), &iter, sourcerow);
		gtk_tree_model_get_value (GTK_TREE_MODEL(gtkblist->treemodel), &iter, NODE_COLUMN, &val);
		node = g_value_get_pointer(&val);
		gtk_selection_data_set (data,
					gdk_atom_intern ("GAIM_BLIST_NODE", FALSE),
					8, /* bits */
					(void*)&node,
					sizeof (node));

		gtk_tree_path_free(sourcerow);
	}
	else if (data->target == gdk_atom_intern("application/x-im-contact", FALSE))
	{
		GtkTreeRowReference *ref;
		GtkTreePath *sourcerow;
		GtkTreeIter iter;
		GaimBlistNode *node = NULL;
		GaimBuddy *buddy;
		GaimConnection *gc;
		GValue val = {0};
		GString *str;
		const char *protocol;
		char *mime_str;

		ref = g_object_get_data(G_OBJECT(dc), "gtk-tree-view-source-row");
		sourcerow = gtk_tree_row_reference_get_path(ref);

		if (!sourcerow)
			return;

		gtk_tree_model_get_iter(GTK_TREE_MODEL(gtkblist->treemodel), &iter,
								sourcerow);
		gtk_tree_model_get_value(GTK_TREE_MODEL(gtkblist->treemodel), &iter,
								 NODE_COLUMN, &val);

		node = g_value_get_pointer(&val);

		if (GAIM_BLIST_NODE_IS_CONTACT(node))
		{
			buddy = gaim_contact_get_priority_buddy((GaimContact *)node);
		}
		else if (!GAIM_BLIST_NODE_IS_BUDDY(node))
		{
			gtk_tree_path_free(sourcerow);
			return;
		}
		else
		{
			buddy = (GaimBuddy *)node;
		}

		gc = gaim_account_get_connection(buddy->account);

		if (gc == NULL)
		{
			gtk_tree_path_free(sourcerow);
			return;
		}

		protocol =
			GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl)->list_icon(buddy->account,
														   buddy);

		str = g_string_new(NULL);
		g_string_printf(str,
			"MIME-Version: 1.0\r\n"
			"Content-Type: application/x-im-contact\r\n"
			"X-IM-Protocol: %s\r\n"
			"X-IM-Username: %s\r\n",
			protocol,
			buddy->name);

		if (buddy->alias != NULL)
		{
			g_string_append_printf(str,
				"X-IM-Alias: %s\r\n",
				buddy->alias);
		}

		str = g_string_append(str, "\r\n");

		mime_str = g_string_free(str, FALSE);

		gtk_selection_data_set(data,
					gdk_atom_intern("application/x-im-contact", FALSE),
					8, /* bits */
					mime_str,
					strlen(mime_str) + 1);

		g_free(mime_str);
		gtk_tree_path_free(sourcerow);
	}
}

static void gaim_gtk_blist_drag_data_rcv_cb(GtkWidget *widget, GdkDragContext *dc, guint x, guint y,
			  GtkSelectionData *sd, guint info, guint t)
{
	if (sd->target == gdk_atom_intern("GAIM_BLIST_NODE", FALSE) && sd->data) {
		GaimBlistNode *n = NULL;
		GtkTreePath *path = NULL;
		GtkTreeViewDropPosition position;
		memcpy(&n, sd->data, sizeof(n));
		if(gtk_tree_view_get_dest_row_at_pos(GTK_TREE_VIEW(widget), x, y, &path, &position)) {
			/* if we're here, I think it means the drop is ok */
			GtkTreeIter iter;
			GaimBlistNode *node;
			GValue val = {0};
			struct _gaim_gtk_blist_node *gtknode;

			gtk_tree_model_get_iter(GTK_TREE_MODEL(gtkblist->treemodel),
					&iter, path);
			gtk_tree_model_get_value (GTK_TREE_MODEL(gtkblist->treemodel),
					&iter, NODE_COLUMN, &val);
			node = g_value_get_pointer(&val);
			gtknode = node->ui_data;

			if (GAIM_BLIST_NODE_IS_CONTACT(n)) {
				GaimContact *c = (GaimContact*)n;
				if (GAIM_BLIST_NODE_IS_CONTACT(node) && gtknode->contact_expanded) {
					gaim_blist_merge_contact(c, node);
				} else if (GAIM_BLIST_NODE_IS_CONTACT(node) ||
						GAIM_BLIST_NODE_IS_CHAT(node)) {
					switch(position) {
						case GTK_TREE_VIEW_DROP_AFTER:
						case GTK_TREE_VIEW_DROP_INTO_OR_AFTER:
							gaim_blist_add_contact(c, (GaimGroup*)node->parent,
									node);
							break;
						case GTK_TREE_VIEW_DROP_BEFORE:
						case GTK_TREE_VIEW_DROP_INTO_OR_BEFORE:
							gaim_blist_add_contact(c, (GaimGroup*)node->parent,
									node->prev);
							break;
					}
				} else if(GAIM_BLIST_NODE_IS_GROUP(node)) {
					gaim_blist_add_contact(c, (GaimGroup*)node, NULL);
				} else if(GAIM_BLIST_NODE_IS_BUDDY(node)) {
					gaim_blist_merge_contact(c, node);
				}
			} else if (GAIM_BLIST_NODE_IS_BUDDY(n)) {
				GaimBuddy *b = (GaimBuddy*)n;
				if (GAIM_BLIST_NODE_IS_BUDDY(node)) {
					switch(position) {
						case GTK_TREE_VIEW_DROP_AFTER:
						case GTK_TREE_VIEW_DROP_INTO_OR_AFTER:
							gaim_blist_add_buddy(b, (GaimContact*)node->parent,
									(GaimGroup*)node->parent->parent, node);
							break;
						case GTK_TREE_VIEW_DROP_BEFORE:
						case GTK_TREE_VIEW_DROP_INTO_OR_BEFORE:
							gaim_blist_add_buddy(b, (GaimContact*)node->parent,
									(GaimGroup*)node->parent->parent,
									node->prev);
							break;
					}
				} else if(GAIM_BLIST_NODE_IS_CHAT(node)) {
					gaim_blist_add_buddy(b, NULL, (GaimGroup*)node->parent,
							NULL);
				} else if (GAIM_BLIST_NODE_IS_GROUP(node)) {
					gaim_blist_add_buddy(b, NULL, (GaimGroup*)node, NULL);
				} else if (GAIM_BLIST_NODE_IS_CONTACT(node)) {
					if(gtknode->contact_expanded) {
						switch(position) {
							case GTK_TREE_VIEW_DROP_INTO_OR_AFTER:
							case GTK_TREE_VIEW_DROP_AFTER:
							case GTK_TREE_VIEW_DROP_INTO_OR_BEFORE:
								gaim_blist_add_buddy(b, (GaimContact*)node,
										(GaimGroup*)node->parent, NULL);
								break;
							case GTK_TREE_VIEW_DROP_BEFORE:
								gaim_blist_add_buddy(b, NULL,
										(GaimGroup*)node->parent, node->prev);
								break;
						}
					} else {
						switch(position) {
							case GTK_TREE_VIEW_DROP_INTO_OR_AFTER:
							case GTK_TREE_VIEW_DROP_AFTER:
								gaim_blist_add_buddy(b, NULL,
										(GaimGroup*)node->parent, NULL);
								break;
							case GTK_TREE_VIEW_DROP_INTO_OR_BEFORE:
							case GTK_TREE_VIEW_DROP_BEFORE:
								gaim_blist_add_buddy(b, NULL,
										(GaimGroup*)node->parent, node->prev);
								break;
						}
					}
				}
			} else if (GAIM_BLIST_NODE_IS_CHAT(n)) {
				GaimChat *chat = (GaimChat *)n;
				if (GAIM_BLIST_NODE_IS_BUDDY(node)) {
					switch(position) {
						case GTK_TREE_VIEW_DROP_AFTER:
						case GTK_TREE_VIEW_DROP_INTO_OR_AFTER:
							gaim_blist_add_chat(chat,
									(GaimGroup*)node->parent->parent, node);
							break;
						case GTK_TREE_VIEW_DROP_BEFORE:
						case GTK_TREE_VIEW_DROP_INTO_OR_BEFORE:
							gaim_blist_add_chat(chat,
									(GaimGroup*)node->parent->parent,
									node->prev);
							break;
					}
				} else if(GAIM_BLIST_NODE_IS_CONTACT(node) ||
						GAIM_BLIST_NODE_IS_CHAT(node)) {
					switch(position) {
						case GTK_TREE_VIEW_DROP_AFTER:
						case GTK_TREE_VIEW_DROP_INTO_OR_AFTER:
							gaim_blist_add_chat(chat, (GaimGroup*)node->parent, node);
							break;
						case GTK_TREE_VIEW_DROP_BEFORE:
						case GTK_TREE_VIEW_DROP_INTO_OR_BEFORE:
							gaim_blist_add_chat(chat, (GaimGroup*)node->parent, node->prev);
							break;
					}
				} else if (GAIM_BLIST_NODE_IS_GROUP(node)) {
					gaim_blist_add_chat(chat, (GaimGroup*)node, NULL);
				}
			} else if (GAIM_BLIST_NODE_IS_GROUP(n)) {
				GaimGroup *g = (GaimGroup*)n;
				if (GAIM_BLIST_NODE_IS_GROUP(node)) {
					switch (position) {
					case GTK_TREE_VIEW_DROP_INTO_OR_AFTER:
					case GTK_TREE_VIEW_DROP_AFTER:
						gaim_blist_add_group(g, node);
						break;
					case GTK_TREE_VIEW_DROP_INTO_OR_BEFORE:
					case GTK_TREE_VIEW_DROP_BEFORE:
						gaim_blist_add_group(g, node->prev);
						break;
					}
				} else if(GAIM_BLIST_NODE_IS_BUDDY(node)) {
					gaim_blist_add_group(g, node->parent->parent);
				} else if(GAIM_BLIST_NODE_IS_CONTACT(node) ||
						GAIM_BLIST_NODE_IS_CHAT(node)) {
					gaim_blist_add_group(g, node->parent);
				}
			}

			gtk_tree_path_free(path);
			gtk_drag_finish(dc, TRUE, (dc->action == GDK_ACTION_MOVE), t);
		}
	}
	else if (sd->target == gdk_atom_intern("application/x-im-contact",
										   FALSE) && sd->data)
	{
		GaimGroup *group = NULL;
		GtkTreePath *path = NULL;
		GtkTreeViewDropPosition position;
		GaimAccount *account;
		char *protocol = NULL;
		char *username = NULL;
		char *alias = NULL;

		if (gtk_tree_view_get_dest_row_at_pos(GTK_TREE_VIEW(widget),
											  x, y, &path, &position))
		{
			GtkTreeIter iter;
			GaimBlistNode *node;
			GValue val = {0};

			gtk_tree_model_get_iter(GTK_TREE_MODEL(gtkblist->treemodel),
									&iter, path);
			gtk_tree_model_get_value (GTK_TREE_MODEL(gtkblist->treemodel),
									  &iter, NODE_COLUMN, &val);
			node = g_value_get_pointer(&val);

			if (GAIM_BLIST_NODE_IS_BUDDY(node))
			{
				group = (GaimGroup *)node->parent->parent;
			}
			else if (GAIM_BLIST_NODE_IS_CHAT(node) ||
					 GAIM_BLIST_NODE_IS_CONTACT(node))
			{
				group = (GaimGroup *)node->parent;
			}
			else if (GAIM_BLIST_NODE_IS_GROUP(node))
			{
				group = (GaimGroup *)node;
			}
		}

		if (gaim_gtk_parse_x_im_contact(sd->data, FALSE, &account,
										&protocol, &username, &alias))
		{
			if (account == NULL)
			{
				gaim_notify_error(NULL, NULL,
					_("You are not currently signed on with an account that "
					  "can add that buddy."), NULL);
			}
			else
			{
				gaim_blist_request_add_buddy(account, username,
											 (group ? group->name : NULL),
											 alias);
			}
		}

		if (username != NULL) g_free(username);
		if (protocol != NULL) g_free(protocol);
		if (alias    != NULL) g_free(alias);

		if (path != NULL)
			gtk_tree_path_free(path);

		gtk_drag_finish(dc, TRUE, (dc->action == GDK_ACTION_MOVE), t);
	}
	else if (sd->target == gdk_atom_intern("text/x-vcard", FALSE) && sd->data)
	{
		gboolean result;
		GaimGroup *group = NULL;
		GtkTreePath *path = NULL;
		GtkTreeViewDropPosition position;

		if (gtk_tree_view_get_dest_row_at_pos(GTK_TREE_VIEW(widget),
											  x, y, &path, &position))
		{
			GtkTreeIter iter;
			GaimBlistNode *node;
			GValue val = {0};

			gtk_tree_model_get_iter(GTK_TREE_MODEL(gtkblist->treemodel),
									&iter, path);
			gtk_tree_model_get_value (GTK_TREE_MODEL(gtkblist->treemodel),
									  &iter, NODE_COLUMN, &val);
			node = g_value_get_pointer(&val);

			if (GAIM_BLIST_NODE_IS_BUDDY(node))
			{
				group = (GaimGroup *)node->parent->parent;
			}
			else if (GAIM_BLIST_NODE_IS_CHAT(node) ||
					 GAIM_BLIST_NODE_IS_CONTACT(node))
			{
				group = (GaimGroup *)node->parent;
			}
			else if (GAIM_BLIST_NODE_IS_GROUP(node))
			{
				group = (GaimGroup *)node;
			}
		}

		result = parse_vcard(sd->data, group);

		gtk_drag_finish(dc, result, (dc->action == GDK_ACTION_MOVE), t);
	} else if (sd->target == gdk_atom_intern("text/uri-list", FALSE) && sd->data) {
		GtkTreePath *path = NULL;
		GtkTreeViewDropPosition position;

		if (gtk_tree_view_get_dest_row_at_pos(GTK_TREE_VIEW(widget),
											  x, y, &path, &position))
			{
				GtkTreeIter iter;
				GaimBlistNode *node;
				GValue val = {0};
				
				gtk_tree_model_get_iter(GTK_TREE_MODEL(gtkblist->treemodel),
							&iter, path);
				gtk_tree_model_get_value (GTK_TREE_MODEL(gtkblist->treemodel),
							  &iter, NODE_COLUMN, &val);
				node = g_value_get_pointer(&val);
				
				if (GAIM_BLIST_NODE_IS_BUDDY(node) || GAIM_BLIST_NODE_IS_CONTACT(node)) {
					GaimBuddy *b = GAIM_BLIST_NODE_IS_BUDDY(node) ? (GaimBuddy*)node : gaim_contact_get_priority_buddy((GaimContact*)node);
					GList *tmp;
					GList *files = gaim_uri_list_extract_filenames(sd->data);
					for(tmp = files; tmp != NULL ; tmp = g_list_next(tmp)) {
						gchar *filename = tmp->data;
						/* XXX - Make ft API support creating a transfer with more than one file */
						if (g_file_test(filename, G_FILE_TEST_EXISTS)
								&& !g_file_test(filename, G_FILE_TEST_IS_DIR)) {
							serv_send_file(gaim_account_get_connection(b->account), b->name, filename);
						}
						g_free(filename);
					}
					g_list_free(files);
				}
			}	
	}
}

static void gaim_gtk_blist_paint_tip(GtkWidget *widget, GdkEventExpose *event, GaimBlistNode *node)
{
	GtkStyle *style;
	GdkPixbuf *pixbuf = gaim_gtk_blist_get_status_icon(node, GAIM_STATUS_ICON_LARGE);
	PangoLayout *layout;
	char *tooltiptext = gaim_get_tooltip_text(node);

	if(!tooltiptext)
		return;

	layout = gtk_widget_create_pango_layout (gtkblist->tipwindow, NULL);
	pango_layout_set_markup(layout, tooltiptext, strlen(tooltiptext));
	pango_layout_set_wrap(layout, PANGO_WRAP_WORD);
	pango_layout_set_width(layout, 300000);
	style = gtkblist->tipwindow->style;

	gtk_paint_flat_box(style, gtkblist->tipwindow->window, GTK_STATE_NORMAL, GTK_SHADOW_OUT,
					   NULL, gtkblist->tipwindow, "tooltip", 0, 0, -1, -1);

#if GTK_CHECK_VERSION(2,2,0)
	gdk_draw_pixbuf(GDK_DRAWABLE(gtkblist->tipwindow->window), NULL, pixbuf,
			0, 0, 4, 4, -1 , -1, GDK_RGB_DITHER_NONE, 0, 0);
#else
	gdk_pixbuf_render_to_drawable(pixbuf, GDK_DRAWABLE(gtkblist->tipwindow->window), NULL, 0, 0, 4, 4, -1, -1, GDK_RGB_DITHER_NONE, 0, 0);
#endif

	gtk_paint_layout (style, gtkblist->tipwindow->window, GTK_STATE_NORMAL, TRUE,
			  NULL, gtkblist->tipwindow, "tooltip", 38, 4, layout);

	g_object_unref (pixbuf);
	g_object_unref (layout);
	g_free(tooltiptext);

#ifdef WANT_DROP_SHADOW
	shadow_paint(gtkblist, NULL, EAST_SIDE);
	shadow_paint(gtkblist, NULL, SOUTH_SIDE);
#endif

	return;
}

static void gaim_gtk_blist_tooltip_destroy()
{
	if (gtkblist->tipwindow == NULL)
		return;

	gtk_widget_destroy(gtkblist->tipwindow);
	gtkblist->tipwindow = NULL;
#ifdef WANT_DROP_SHADOW
	gdk_window_set_user_data (gtkblist->east_shadow, NULL);
	gdk_window_destroy (gtkblist->east_shadow);
	gtkblist->east_shadow = NULL;

	gdk_window_set_user_data (gtkblist->south_shadow, NULL);
	gdk_window_destroy (gtkblist->south_shadow);
	gtkblist->south_shadow = NULL;
#endif
}

static gboolean gaim_gtk_blist_tooltip_timeout(GtkWidget *tv)
{
	GtkTreePath *path;
	GtkTreeIter iter;
	GaimBlistNode *node;
	GValue val = {0};
	int scr_w, scr_h, w, h, x, y;
#if GTK_CHECK_VERSION(2,2,0)
	int mon_num;
	GdkScreen *screen = NULL;
#endif
	PangoLayout *layout;
	gboolean tooltip_top = FALSE;
	char *tooltiptext = NULL;
	struct _gaim_gtk_blist_node *gtknode;
	GdkRectangle mon_size;
#ifdef WANT_DROP_SHADOW
	GdkWindowAttr attr;
#endif

	if (!gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(tv), gtkblist->tip_rect.x, gtkblist->tip_rect.y, &path, NULL, NULL, NULL))
		return FALSE;
	gtk_tree_model_get_iter(GTK_TREE_MODEL(gtkblist->treemodel), &iter, path);
	gtk_tree_model_get_value (GTK_TREE_MODEL(gtkblist->treemodel), &iter, NODE_COLUMN, &val);
	node = g_value_get_pointer(&val);

	if(!GAIM_BLIST_NODE_IS_CONTACT(node) && !GAIM_BLIST_NODE_IS_BUDDY(node)
			&& !GAIM_BLIST_NODE_IS_CHAT(node))
		return FALSE;

	gtknode = node->ui_data;

	if (node->child && GAIM_BLIST_NODE_IS_CONTACT(node) &&
			(((GaimContact*)node)->online > 1 ||
			 (gaim_blist_node_get_bool(node, "show_offline") &&
			  ((GaimContact*)node)->currentsize > 1)) &&
			!gtknode->contact_expanded &&
			gaim_prefs_get_bool("/gaim/gtk/blist/auto_expand_contacts")) {
		GtkTreeIter i;
		gaim_gtk_blist_expand_contact_cb(NULL, node);
		tooltip_top = TRUE; /* When the person expands, the new screennames will be below.
							   We'll draw the tip above the cursor so that the user can see
							   the included buddies */

		while (gtk_events_pending())
			gtk_main_iteration();

		/* we check to see if we're still supposed to be moving, now that gtk events have
		 * happened, and the mouse might not still be in the buddy list */
		if(!gtkblist->timeout) {
			gaim_gtk_blist_collapse_contact_cb(NULL, node);
			return FALSE;
		}

		gtk_tree_view_get_cell_area(GTK_TREE_VIEW(tv), path, NULL, &gtkblist->contact_rect);
		gdk_drawable_get_size(GDK_DRAWABLE(tv->window), &(gtkblist->contact_rect.width), NULL);
		gtkblist->mouseover_contact = node;
		gtk_tree_path_down (path);
		while (gtk_tree_model_get_iter(GTK_TREE_MODEL(gtkblist->treemodel), &i, path)) {
			GdkRectangle rect;
			gtk_tree_view_get_cell_area(GTK_TREE_VIEW(tv), path, NULL, &rect);
			gtkblist->contact_rect.height += rect.height;
			gtk_tree_path_next(path);
		}
	}

	gtk_tree_path_free(path);

	tooltiptext = gaim_get_tooltip_text(node);

	if(!tooltiptext)
		return FALSE;

	if(gtkblist->tipwindow)
		gtk_widget_destroy(gtkblist->tipwindow);
	gtkblist->tipwindow = gtk_window_new(GTK_WINDOW_POPUP);
	gtk_widget_set_app_paintable(gtkblist->tipwindow, TRUE);
	gtk_window_set_resizable(GTK_WINDOW(gtkblist->tipwindow), FALSE);
	gtk_widget_set_name(gtkblist->tipwindow, "gtk-tooltips");
	g_signal_connect(G_OBJECT(gtkblist->tipwindow), "expose_event",
			G_CALLBACK(gaim_gtk_blist_paint_tip), node);
	gtk_widget_ensure_style (gtkblist->tipwindow);

#ifdef WANT_DROP_SHADOW
	attr.window_type = GDK_WINDOW_TEMP;
	attr.override_redirect = TRUE;
	attr.x = gtkblist->tipwindow->allocation.x;
	attr.y = gtkblist->tipwindow->allocation.y;
	attr.width = gtkblist->tipwindow->allocation.width;
	attr.height = gtkblist->tipwindow->allocation.height;
	attr.wclass = GDK_INPUT_OUTPUT;
	attr.visual = gtk_widget_get_visual (gtkblist->window);
	attr.colormap = gtk_widget_get_colormap (gtkblist->window);

	attr.event_mask = gtk_widget_get_events (gtkblist->tipwindow);

	attr.event_mask |= (GDK_EXPOSURE_MASK | GDK_KEY_PRESS_MASK |
			  GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK );
	if(gtkblist->east_shadow) {
		gdk_window_set_user_data (gtkblist->east_shadow, NULL);
		gdk_window_destroy (gtkblist->east_shadow);
	}
	gtkblist->east_shadow = gdk_window_new(gtk_widget_get_root_window(gtkblist->tipwindow), &attr,
										   GDK_WA_NOREDIR | GDK_WA_VISUAL | GDK_WA_COLORMAP);
	gdk_window_set_user_data (gtkblist->east_shadow, gtkblist->tipwindow);
	gdk_window_set_back_pixmap (gtkblist->east_shadow, NULL, FALSE);

	if(gtkblist->south_shadow) {
		gdk_window_set_user_data (gtkblist->south_shadow, NULL);
		gdk_window_destroy (gtkblist->south_shadow);
	}
	gtkblist->south_shadow = gdk_window_new(gtk_widget_get_root_window(gtkblist->tipwindow), &attr,
											GDK_WA_NOREDIR | GDK_WA_VISUAL | GDK_WA_COLORMAP);
	gdk_window_set_user_data (gtkblist->south_shadow, gtkblist->tipwindow);
	gdk_window_set_back_pixmap (gtkblist->south_shadow, NULL, FALSE);
#endif

	layout = gtk_widget_create_pango_layout (gtkblist->tipwindow, NULL);
	pango_layout_set_wrap(layout, PANGO_WRAP_WORD);
	pango_layout_set_width(layout, 300000);
	pango_layout_set_markup(layout, tooltiptext, strlen(tooltiptext));
	pango_layout_get_size (layout, &w, &h);

#if GTK_CHECK_VERSION(2,2,0)
	gdk_display_get_pointer(gdk_display_get_default(), &screen, &x, &y, NULL);
	mon_num = gdk_screen_get_monitor_at_point(screen, x, y);
	gdk_screen_get_monitor_geometry(screen, mon_num, &mon_size);

	scr_w = mon_size.width + mon_size.x;
	scr_h = mon_size.height + mon_size.y;
#else
	scr_w = gdk_screen_width();
	scr_h = gdk_screen_height();
	gdk_window_get_pointer(NULL, &x, &y, NULL);
	mon_size.x = 0;
	mon_size.y = 0;
#endif

	w = PANGO_PIXELS(w) + 8;
	h = PANGO_PIXELS(h) + 8;

	/* 38 is the size of a large status icon plus 4 pixels padding on each side.
	 *  I should #define this or something */
	w = w + 38;
	h = MAX(h, 38);

#if GTK_CHECK_VERSION(2,2,0)
	if( w > mon_size.width )
	  w = mon_size.width - 10;

	if( h > mon_size.height )
	  h = mon_size.height - 10;
#endif

	if (GTK_WIDGET_NO_WINDOW(gtkblist->window))
		y+=gtkblist->window->allocation.y;

	x -= ((w >> 1) + 4);

	if ((y + h + 4) > scr_h || tooltip_top)
		y = y - h - 5;
	else
		y = y + 6;

	if (y < mon_size.y)
		y = mon_size.y;

	if (y != mon_size.y) {
		if ((x + w) > scr_w)
			x -= (x + w + 5) - scr_w;
		else if (x < mon_size.x)
			x = mon_size.x;
	} else {
		x -= (w / 2 + 10);
		if (x < mon_size.x)
			x = mon_size.x;
	}

	g_object_unref (layout);
	g_free(tooltiptext);
	gtk_widget_set_size_request(gtkblist->tipwindow, w, h);
	gtk_window_move(GTK_WINDOW(gtkblist->tipwindow), x, y);
	gtk_widget_show(gtkblist->tipwindow);

#ifdef WANT_DROP_SHADOW
	map_shadow_windows(gtkblist);
#endif

	return FALSE;
}

static gboolean gaim_gtk_blist_motion_cb (GtkWidget *tv, GdkEventMotion *event, gpointer null)
{
	GtkTreePath *path;
	int delay;

	delay = gaim_prefs_get_int("/gaim/gtk/blist/tooltip_delay");

	if (delay == 0)
		return FALSE;

	if (gtkblist->timeout) {
		if ((event->y > gtkblist->tip_rect.y) && ((event->y - gtkblist->tip_rect.height) < gtkblist->tip_rect.y))
			return FALSE;
		/* We've left the cell.  Remove the timeout and create a new one below */
		gaim_gtk_blist_tooltip_destroy();
		g_source_remove(gtkblist->timeout);
	}

	gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(tv), event->x, event->y, &path, NULL, NULL, NULL);
	gtk_tree_view_get_cell_area(GTK_TREE_VIEW(tv), path, NULL, &gtkblist->tip_rect);

	if (path)
		gtk_tree_path_free(path);
	gtkblist->timeout = g_timeout_add(delay, (GSourceFunc)gaim_gtk_blist_tooltip_timeout, tv);

	if (gtkblist->mouseover_contact) {
		if ((event->y < gtkblist->contact_rect.y) || ((event->y - gtkblist->contact_rect.height) > gtkblist->contact_rect.y)) {
			gaim_gtk_blist_collapse_contact_cb(NULL, gtkblist->mouseover_contact);
			gtkblist->mouseover_contact = NULL;
		}
	}

	return FALSE;
}

static void gaim_gtk_blist_leave_cb (GtkWidget *w, GdkEventCrossing *e, gpointer n)
{

	if (gtkblist->timeout) {
		g_source_remove(gtkblist->timeout);
		gtkblist->timeout = 0;
	}
	gaim_gtk_blist_tooltip_destroy();

	if (gtkblist->mouseover_contact &&
		!((e->x > gtkblist->contact_rect.x) && (e->x < (gtkblist->contact_rect.x + gtkblist->contact_rect.width)) &&
		 (e->y > gtkblist->contact_rect.y) && (e->y < (gtkblist->contact_rect.y + gtkblist->contact_rect.height)))) {
			gaim_gtk_blist_collapse_contact_cb(NULL, gtkblist->mouseover_contact);
		gtkblist->mouseover_contact = NULL;
	}
}

static void
toggle_debug(void)
{
	gaim_prefs_set_bool("/gaim/gtk/debug/enabled",
			!gaim_prefs_get_bool("/gaim/gtk/debug/enabled"));
}


/***************************************************
 *            Crap                                 *
 ***************************************************/
static GtkItemFactoryEntry blist_menu[] =
{
	/* Buddies menu */
	{ N_("/_Buddies"), NULL, NULL, 0, "<Branch>" },
	{ N_("/Buddies/New Instant _Message..."), "<CTL>M", gaim_gtkdialogs_im, 0, "<StockItem>", GAIM_STOCK_IM },
	{ N_("/Buddies/Join a _Chat..."), "<CTL>C", gaim_gtk_blist_joinchat_show, 0, "<StockItem>", GAIM_STOCK_CHAT },
	{ N_("/Buddies/Get User _Info..."), "<CTL>I", gaim_gtkdialogs_info, 0, "<StockItem>", GAIM_STOCK_INFO },
	{ N_("/Buddies/View User _Log..."), "<CTL>L", gaim_gtkdialogs_log, 0, NULL },
	{ "/Buddies/sep1", NULL, NULL, 0, "<Separator>" },
	{ N_("/Buddies/Show _Offline Buddies"), NULL, gaim_gtk_blist_edit_mode_cb, 1, "<CheckItem>"},
	{ N_("/Buddies/Show _Empty Groups"), NULL, gaim_gtk_blist_show_empty_groups_cb, 1, "<CheckItem>"},
	{ N_("/Buddies/_Add Buddy..."), "<CTL>B", gaim_gtk_blist_add_buddy_cb, 0, "<StockItem>", GTK_STOCK_ADD },
	{ N_("/Buddies/Add C_hat..."), NULL, gaim_gtk_blist_add_chat_cb, 0, "<StockItem>", GTK_STOCK_ADD },
	{ N_("/Buddies/Add _Group..."), NULL, gaim_blist_request_add_group, 0, "<StockItem>", GTK_STOCK_ADD },
	{ "/Buddies/sep2", NULL, NULL, 0, "<Separator>" },
	{ N_("/Buddies/_Signoff"), "<CTL>D", gaim_connections_disconnect_all, 0, "<StockItem>", GAIM_STOCK_SIGN_OFF },
	{ N_("/Buddies/_Quit"), "<CTL>Q", gaim_core_quit, 0, "<StockItem>", GTK_STOCK_QUIT },

	/* Tools */
	{ N_("/_Tools"), NULL, NULL, 0, "<Branch>" },
	{ N_("/Tools/_Away"), NULL, NULL, 0, "<Branch>" },
	{ N_("/Tools/Buddy _Pounce"), NULL, NULL, 0, "<Branch>" },
	{ N_("/Tools/Account Ac_tions"), NULL, NULL, 0, "<Branch>" },
	{ N_("/Tools/Pl_ugin Actions"), NULL, NULL, 0, "<Branch>" },
	{ "/Tools/sep1", NULL, NULL, 0, "<Separator>" },
	{ N_("/Tools/A_ccounts"), "<CTL>A", gaim_gtk_accounts_window_show, 0, "<StockItem>", GAIM_STOCK_ACCOUNTS },
	{ N_("/Tools/_File Transfers"), NULL, gaim_show_xfer_dialog, 0, "<StockItem>", GAIM_STOCK_FILE_TRANSFER },
	{ N_("/Tools/R_oom List"), NULL, gaim_gtk_roomlist_dialog_show, 0, NULL },
	{ N_("/Tools/Pr_eferences"), "<CTL>P", gaim_gtk_prefs_show, 0, "<StockItem>", GTK_STOCK_PREFERENCES },
	{ N_("/Tools/Pr_ivacy"), NULL, gaim_gtk_privacy_dialog_show, 0, NULL },
	{ "/Tools/sep2", NULL, NULL, 0, "<Separator>" },
	{ N_("/Tools/View System _Log"), NULL, gtk_blist_show_systemlog_cb, 0, NULL },

	/* Help */
	{ N_("/_Help"), NULL, NULL, 0, "<Branch>" },
	{ N_("/Help/Online _Help"), "F1", gtk_blist_show_onlinehelp_cb, 0, "<StockItem>", GTK_STOCK_HELP },
	{ N_("/Help/_Debug Window"), NULL, toggle_debug, 0, NULL },
	{ N_("/Help/_About"), NULL, gaim_gtkdialogs_about, 0,  "<StockItem>", GAIM_STOCK_ABOUT },
};

/*********************************************************
 * Private Utility functions                             *
 *********************************************************/
static void
rename_group_cb(GaimGroup *g, const char *new_name)
{
	gaim_blist_rename_group(g, new_name);
}

/*
 * Should disallow empty group names.
 */
static void
show_rename_group(GtkWidget *unused, GaimGroup *g)
{
	gaim_request_input(NULL, _("Rename Group"), _("New group name"),
					   _("Please enter a new name for the selected group."),
					   g->name, FALSE, FALSE, NULL,
					   _("OK"), G_CALLBACK(rename_group_cb),
					   _("Cancel"), NULL, g);
}

static char *gaim_get_tooltip_text(GaimBlistNode *node)
{
	GaimPlugin *prpl;
	GaimPluginProtocolInfo *prpl_info = NULL;
	char *text = NULL;

	if(GAIM_BLIST_NODE_IS_CHAT(node)) {
		GaimChat *chat = (GaimChat *)node;
		char *name = NULL;
		struct proto_chat_entry *pce;
		GList *parts, *tmp;
		GString *parts_text = g_string_new("");

		prpl = gaim_find_prpl(gaim_account_get_protocol_id(chat->account));
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(prpl);

		parts = prpl_info->chat_info(chat->account->gc);

		name = g_markup_escape_text(gaim_chat_get_name(chat), -1);

		if(g_list_length(gaim_connections_get_all()) > 1) {
			char *account = g_markup_escape_text(chat->account->username, -1);
			g_string_append_printf(parts_text, _("\n<b>Account:</b> %s"),
					account);
			g_free(account);
		}
		for(tmp = parts; tmp; tmp = tmp->next) {
			char *label, *tmp2, *value;
			pce = tmp->data;

			if(!pce->secret) {

				tmp2 = g_markup_escape_text(pce->label, -1);
				label = gaim_text_strip_mnemonic(tmp2);
				g_free(tmp2);

				value = g_markup_escape_text(g_hash_table_lookup(chat->components,
							pce->identifier), -1);

				g_string_append_printf(parts_text, "\n<b>%s</b> %s", label, value);
				g_free(label);
				g_free(value);
			}
			g_free(pce);
		}
		g_list_free(parts);

		text = g_strdup_printf("<span size='larger' weight='bold'>%s</span>%s",
				name, parts_text->str);
		g_string_free(parts_text, TRUE);
		g_free(name);
	} else if(GAIM_BLIST_NODE_IS_CONTACT(node) ||
			GAIM_BLIST_NODE_IS_BUDDY(node)) {
		GaimBuddy *b;
		char *statustext = NULL;
		char *contactaliastext = NULL;
		char *aliastext = NULL, *nicktext = NULL;
		char *loggedin = NULL, *idletime = NULL;
		char *warning = NULL;
		char *accounttext = NULL;

		if(GAIM_BLIST_NODE_IS_CONTACT(node)) {
			GaimContact *contact = (GaimContact*)node;
			b = gaim_contact_get_priority_buddy(contact);
			if(contact->alias)
				contactaliastext = g_markup_escape_text(contact->alias, -1);
		} else {
			b = (GaimBuddy *)node;
		}

		prpl = gaim_find_prpl(gaim_account_get_protocol_id(b->account));
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(prpl);

		if (prpl_info && prpl_info->tooltip_text) {
			const char *end;
			statustext = prpl_info->tooltip_text(b);

			if(statustext && !g_utf8_validate(statustext, -1, &end)) {
				char *new = g_strndup(statustext,
						g_utf8_pointer_to_offset(statustext, end));
				g_free(statustext);
				statustext = new;
			}
		}

		if (!statustext && !GAIM_BUDDY_IS_ONLINE(b))
			statustext = g_strdup(_("\n<b>Status:</b> Offline"));

		if (b->signon > 0)
			loggedin = gaim_str_seconds_to_string(time(NULL) - b->signon);

		if (b->idle > 0)
			idletime = gaim_str_seconds_to_string(time(NULL) - b->idle);

		if(b->alias && b->alias[0])
			aliastext = g_markup_escape_text(b->alias, -1);

		if(b->server_alias)
			nicktext = g_markup_escape_text(b->server_alias, -1);

		if (b->evil > 0)
			warning = g_strdup_printf(_("%d%%"), b->evil);

		if(g_list_length(gaim_connections_get_all()) > 1)
			accounttext = g_markup_escape_text(b->account->username, -1);

		text = g_strdup_printf("<span size='larger' weight='bold'>%s</span>"
					"%s %s"  /* Account */
					"%s %s"  /* Contact Alias */
					"%s %s"  /* Alias */
					"%s %s"  /* Nickname */
					"%s %s"  /* Logged In */
					"%s %s"  /* Idle */
					"%s %s"  /* Warning */
					"%s"     /* Status */
					"%s",
					b->name,
					accounttext ? _("\n<b>Account:</b>") : "", accounttext ? accounttext : "",
					contactaliastext ? _("\n<b>Contact Alias:</b>") : "", contactaliastext ? contactaliastext : "",
					aliastext ? _("\n<b>Alias:</b>") : "", aliastext ? aliastext : "",
					nicktext ? _("\n<b>Nickname:</b>") : "", nicktext ? nicktext : "",
					loggedin ? _("\n<b>Logged In:</b>") : "", loggedin ? loggedin : "",
					idletime ? _("\n<b>Idle:</b>") : "", idletime ? idletime : "",
					b->evil ? _("\n<b>Warned:</b>") : "", b->evil ? warning : "",
					statustext ? statustext : "",
					!g_ascii_strcasecmp(b->name, "robflynn") ? _("\n<b>Description:</b> Spooky") :
					!g_ascii_strcasecmp(b->name, "seanegn") ? _("\n<b>Status</b>: Awesome") :
					!g_ascii_strcasecmp(b->name, "chipx86") ? _("\n<b>Status</b>: Rockin'") : "");

		if(warning)
			g_free(warning);
		if(loggedin)
			g_free(loggedin);
		if(idletime)
			g_free(idletime);
		if(statustext)
			g_free(statustext);
		if(nicktext)
			g_free(nicktext);
		if(aliastext)
			g_free(aliastext);
		if(accounttext)
			g_free(accounttext);
	}
	gaim_signal_emit(gaim_gtk_blist_get_handle(),
			 "drawing-tooltip", node, &text);
	return text;
}

struct _emblem_data {
	char *filename;
	int x;
	int y;
};

GdkPixbuf *gaim_gtk_blist_get_status_icon(GaimBlistNode *node, GaimStatusIconSize size)
{
	GdkPixbuf *scale, *status = NULL;
	int i, scalesize = 30;
	char *filename;
	const char *protoname = NULL;
	struct _gaim_gtk_blist_node *gtknode = node->ui_data;
	struct _emblem_data emblems[4] = {{NULL, 15, 15}, {NULL, 0, 15},
		{NULL, 0, 0}, {NULL, 15, 0}};

	GaimBuddy *buddy = NULL;
	GaimChat *chat = NULL;

	if(GAIM_BLIST_NODE_IS_CONTACT(node)) {
		if(!gtknode->contact_expanded)
			buddy = gaim_contact_get_priority_buddy((GaimContact*)node);
	} else if(GAIM_BLIST_NODE_IS_BUDDY(node)) {
		buddy = (GaimBuddy*)node;
	} else if(GAIM_BLIST_NODE_IS_CHAT(node)) {
		chat = (GaimChat*)node;
	} else {
		return NULL;
	}

	if(buddy || chat) {
		GaimAccount *account;
		GaimPlugin *prpl;
		GaimPluginProtocolInfo *prpl_info;

		if(buddy)
			account = buddy->account;
		else
			account = chat->account;

		prpl = gaim_find_prpl(gaim_account_get_protocol_id(account));
		if(!prpl)
			return NULL;

		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(prpl);

		if(prpl_info && prpl_info->list_icon) {
			protoname = prpl_info->list_icon(account, buddy);
		}
		if(prpl_info && prpl_info->list_emblems && buddy) {
			if(buddy->present != GAIM_BUDDY_SIGNING_OFF)
				prpl_info->list_emblems(buddy, &emblems[0].filename,
						&emblems[1].filename, &emblems[2].filename,
						&emblems[3].filename);
		}
	}

	if(size == GAIM_STATUS_ICON_SMALL) {
		scalesize = 15;
		/* So that only the se icon will composite */
		emblems[1].filename = emblems[2].filename = emblems[3].filename = NULL;
	}

	if(buddy && buddy->present == GAIM_BUDDY_SIGNING_ON) {
			filename = g_build_filename(DATADIR, "pixmaps", "gaim", "status", "default", "login.png", NULL);
	} else if(buddy && buddy->present == GAIM_BUDDY_SIGNING_OFF) {
			filename = g_build_filename(DATADIR, "pixmaps", "gaim", "status", "default", "logout.png", NULL);
	} else if(buddy || chat) {
		char *image = g_strdup_printf("%s.png", protoname);
		filename = g_build_filename(DATADIR, "pixmaps", "gaim", "status", "default", image, NULL);
		g_free(image);
	} else {
		/* gaim dude */
		filename = g_build_filename(DATADIR, "pixmaps", "gaim.png", NULL);
	}

	status = gdk_pixbuf_new_from_file(filename, NULL);
	g_free(filename);

	if(!status)
		return NULL;

	scale = gdk_pixbuf_scale_simple(status, scalesize, scalesize,
			GDK_INTERP_BILINEAR);
	g_object_unref(status);

	for(i=0; i<4; i++) {
		if(emblems[i].filename) {
			GdkPixbuf *emblem;
			char *image = g_strdup_printf("%s.png", emblems[i].filename);
			filename = g_build_filename(DATADIR, "pixmaps", "gaim", "status", "default", image, NULL);
			g_free(image);
			emblem = gdk_pixbuf_new_from_file(filename, NULL);
			g_free(filename);
			if(emblem) {
				if(i == 0 && size == GAIM_STATUS_ICON_SMALL) {
					gdk_pixbuf_composite(emblem,
							scale, 5, 5,
							10, 10,
							5, 5,
							.6, .6,
							GDK_INTERP_BILINEAR,
							255);
				} else {
					gdk_pixbuf_composite(emblem,
							scale, emblems[i].x, emblems[i].y,
							15, 15,
							emblems[i].x, emblems[i].y,
							1, 1,
							GDK_INTERP_BILINEAR,
							255);
				}
				g_object_unref(emblem);
			}
		}
	}

	if(buddy) {
		if(buddy->present == GAIM_BUDDY_OFFLINE)
			gdk_pixbuf_saturate_and_pixelate(scale, scale, 0.0, FALSE);
		else if(buddy->idle &&
				gaim_prefs_get_bool("/gaim/gtk/blist/grey_idle_buddies"))
			gdk_pixbuf_saturate_and_pixelate(scale, scale, 0.25, FALSE);
	}

	return scale;
}

static GdkPixbuf *gaim_gtk_blist_get_buddy_icon(GaimBuddy *b)
{
	GdkPixbuf *buf, *ret;
	GdkPixbufLoader *loader;
	GaimBuddyIcon *icon;
	const char *data;
	size_t len;

	if (!gaim_prefs_get_bool("/gaim/gtk/blist/show_buddy_icons"))
		return NULL;

	if (!(icon = gaim_buddy_get_icon(b)))
		if (!(icon = gaim_buddy_icons_find(b->account, b->name))) /* Not sure I like this...*/
			return NULL;


	loader = gdk_pixbuf_loader_new();
	data = gaim_buddy_icon_get_data(icon, &len);
	gdk_pixbuf_loader_write(loader, data, len, NULL);
	buf = gdk_pixbuf_loader_get_pixbuf(loader);
	if (buf)
		g_object_ref(G_OBJECT(buf));
	gdk_pixbuf_loader_close(loader, NULL);
	g_object_unref(G_OBJECT(loader));

	if (buf) {
		if (!GAIM_BUDDY_IS_ONLINE(b))
			gdk_pixbuf_saturate_and_pixelate(buf, buf, 0.0, FALSE);
		if (b->idle && gaim_prefs_get_bool("/gaim/gtk/blist/grey_idle_buddies"))
			gdk_pixbuf_saturate_and_pixelate(buf, buf, 0.25, FALSE);

		ret = gdk_pixbuf_scale_simple(buf,30,30, GDK_INTERP_BILINEAR);
		g_object_unref(G_OBJECT(buf));
		return ret;
	}
	return NULL;
}

static gchar *gaim_gtk_blist_get_name_markup(GaimBuddy *b, gboolean selected)
{
	const char *name;
	char *esc, *text = NULL;
	GaimPlugin *prpl;
	GaimPluginProtocolInfo *prpl_info = NULL;
	GaimContact *contact;
	struct _gaim_gtk_blist_node *gtkcontactnode = NULL;
	int ihrs, imin;
	char *idletime = NULL, *warning = NULL, *statustext = NULL;
	time_t t;
	/* XXX Clean up this crap */

	contact = (GaimContact*)((GaimBlistNode*)b)->parent;
	if(contact)
		gtkcontactnode = ((GaimBlistNode*)contact)->ui_data;

	if(gtkcontactnode && !gtkcontactnode->contact_expanded && contact->alias)
		name = contact->alias;
	else
		name = gaim_buddy_get_alias(b);
	esc = g_markup_escape_text(name, strlen(name));

	prpl = gaim_find_prpl(gaim_account_get_protocol_id(b->account));

	if (prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(prpl);

	if (!gaim_prefs_get_bool("/gaim/gtk/blist/show_buddy_icons")) {
		if ((b->idle && !selected &&
			 gaim_prefs_get_bool("/gaim/gtk/blist/grey_idle_buddies")) ||
			!GAIM_BUDDY_IS_ONLINE(b)) {
			if (selected)
				text = g_strdup(esc);
			else
				text =  g_strdup_printf("<span color='dim grey'>%s</span>",
							esc);
			g_free(esc);
			return text;
		}
		else {
			return esc;
		}
	}

	time(&t);
	ihrs = (t - b->idle) / 3600;
	imin = ((t - b->idle) / 60) % 60;

	if (prpl_info && prpl_info->status_text && b->account->gc) {
		char *tmp = prpl_info->status_text(b);
		const char *end;

		if(tmp && !g_utf8_validate(tmp, -1, &end)) {
			char *new = g_strndup(tmp,
					g_utf8_pointer_to_offset(tmp, end));
			g_free(tmp);
			tmp = new;
		}

		if(tmp) {
			char buf[32];
			char *c = tmp;
			int length = 0, vis=0;
			gboolean inside = FALSE;
			g_strdelimit(tmp, "\n", ' ');
			gaim_str_strip_cr(tmp);

			while(*c && vis < 20) {
				if(*c == '&')
					inside = TRUE;
				else if(*c == ';')
					inside = FALSE;
				if(!inside)
					vis++;
				c = g_utf8_next_char(c); /* this is fun */
			}

			length = c - tmp;

			if(vis == 20)
				g_snprintf(buf, sizeof(buf), "%%.%ds...", length);
			else
				g_snprintf(buf, sizeof(buf), "%%s ");

			statustext = g_strdup_printf(buf, tmp);

			g_free(tmp);
		}
	}

	if (b->idle > 0 &&
			gaim_prefs_get_bool("/gaim/gtk/blist/show_idle_time")) {
		if (ihrs)
			idletime = g_strdup_printf(_("Idle (%dh%02dm) "), ihrs, imin);
		else
			idletime = g_strdup_printf(_("Idle (%dm) "), imin);
	}

	if (b->evil > 0 &&
			gaim_prefs_get_bool("/gaim/gtk/blist/show_warning_level"))
		warning = g_strdup_printf(_("Warned (%d%%) "), b->evil);

	if(!GAIM_BUDDY_IS_ONLINE(b) && !statustext)
		statustext = g_strdup(_("Offline "));

	if (b->idle && !selected &&
		gaim_prefs_get_bool("/gaim/gtk/blist/grey_idle_buddies")) {

		text =  g_strdup_printf("<span color='dim grey'>%s</span>\n"
					"<span color='dim grey' size='smaller'>%s%s%s</span>",
					esc,
					statustext != NULL ? statustext : "",
					idletime != NULL ? idletime : "",
					warning != NULL ? warning : "");
	} else if (statustext == NULL && idletime == NULL && warning == NULL &&
			GAIM_BUDDY_IS_ONLINE(b)) {
		text = g_strdup(esc);
	} else {
		text = g_strdup_printf("%s\n"
					"<span %s size='smaller'>%s%s%s</span>", esc,
					selected ? "" : "color='dim grey'",
					statustext != NULL ? statustext :  "",
					idletime != NULL ? idletime : "",
					warning != NULL ? warning : "");
	}
	if (idletime)
		g_free(idletime);
	if (warning)
		g_free(warning);
	if (statustext)
		g_free(statustext);
	if (esc)
		g_free(esc);

	return text;
}

static void gaim_gtk_blist_restore_position()
{
	int blist_x, blist_y, blist_width, blist_height;

	blist_width = gaim_prefs_get_int("/gaim/gtk/blist/width");

	/* if the window exists, is hidden, we're saving positions, and the
	 * position is sane... */
	if (gtkblist && gtkblist->window &&
		!GTK_WIDGET_VISIBLE(gtkblist->window) && blist_width != 0) {

		blist_x      = gaim_prefs_get_int("/gaim/gtk/blist/x");
		blist_y      = gaim_prefs_get_int("/gaim/gtk/blist/y");
		blist_height = gaim_prefs_get_int("/gaim/gtk/blist/height");

		/* ...check position is on screen... */
		if (blist_x >= gdk_screen_width())
			blist_x = gdk_screen_width() - 100;
		else if (blist_x + blist_width < 0)
			blist_x = 100;

		if (blist_y >= gdk_screen_height())
			blist_y = gdk_screen_height() - 100;
		else if (blist_y + blist_height < 0)
			blist_y = 100;

		/* ...and move it back. */
		gtk_window_move(GTK_WINDOW(gtkblist->window), blist_x, blist_y);
		gtk_window_resize(GTK_WINDOW(gtkblist->window), blist_width, blist_height);
	}
}

static gboolean gaim_gtk_blist_refresh_timer(GaimBuddyList *list)
{
	GaimBlistNode *gnode, *cnode;

	for(gnode = list->root; gnode; gnode = gnode->next) {
		if(!GAIM_BLIST_NODE_IS_GROUP(gnode))
			continue;
		for(cnode = gnode->child; cnode; cnode = cnode->next) {
			if(GAIM_BLIST_NODE_IS_CONTACT(cnode)) {
				GaimBuddy *buddy = gaim_contact_get_priority_buddy((GaimContact*)cnode);
				if(buddy && buddy->idle)
						gaim_gtk_blist_update(list, cnode);
			}
		}
	}

	/* keep on going */
	return TRUE;
}

static void gaim_gtk_blist_hide_node(GaimBuddyList *list, GaimBlistNode *node)
{
	struct _gaim_gtk_blist_node *gtknode = (struct _gaim_gtk_blist_node *)node->ui_data;
	GtkTreeIter iter;

	if (!gtknode || !gtknode->row || !gtkblist)
		return;

	if(gtkblist->selected_node == node)
		gtkblist->selected_node = NULL;

	if (get_iter_from_node(node, &iter)) {
		gtk_tree_store_remove(gtkblist->treemodel, &iter);
		if(GAIM_BLIST_NODE_IS_CONTACT(node) || GAIM_BLIST_NODE_IS_BUDDY(node)
				|| GAIM_BLIST_NODE_IS_CHAT(node)) {
			gaim_gtk_blist_update(list, node->parent);
		}
	}
	gtk_tree_row_reference_free(gtknode->row);
	gtknode->row = NULL;
}

static void
sign_on_off_cb(GaimConnection *gc, GaimBuddyList *blist)
{
	GaimGtkBuddyList *gtkblist = GAIM_GTK_BLIST(blist);
	GtkWidget *widget;

	gaim_gtk_blist_update_protocol_actions();
	gaim_gtkpounce_menu_build(gtkblist->bpmenu);

	/* Make menu items sensitive/insensitive where appropriate */
	widget = gtk_item_factory_get_widget(gtkblist->ift, N_("/Buddies/Join a Chat..."));
	gtk_widget_set_sensitive(widget, gaim_gtk_blist_joinchat_is_showable());

	widget = gtk_item_factory_get_widget(gtkblist->ift, N_("/Tools/Room List"));
	gtk_widget_set_sensitive(widget, gaim_gtk_roomlist_is_showable());

	widget = gtk_item_factory_get_widget(gtkblist->ift, N_("/Tools/Privacy"));
	gtk_widget_set_sensitive(widget, gaim_gtk_privacy_is_showable());

	gaim_gtk_blist_update_toolbar();
}


static void
plugin_changed_cb(GaimPlugin *p, gpointer *data)
{
	gaim_gtk_blist_update_plugin_actions();
}


/* this is called on all sorts of signals, and we have no reason to pass
 * it anything, so it remains without arguments. If you need anything
 * more specific, do as below, and create another callback that calls
 * this */
static void
raise_on_events_cb()
{
	if(gtkblist && gtkblist->window &&
			gaim_prefs_get_bool("/gaim/gtk/blist/raise_on_events")) {
		gtk_widget_show(gtkblist->window);
		gtk_window_deiconify(GTK_WINDOW(gtkblist->window));
		gdk_window_raise(gtkblist->window->window);
	}
}

/**********************************************************************************
 * Public API Functions                                                           *
 **********************************************************************************/

static void gaim_gtk_blist_new_list(GaimBuddyList *blist)
{
	GaimGtkBuddyList *gtkblist;

	gtkblist = g_new0(GaimGtkBuddyList, 1);
	blist->ui_data = gtkblist;

	/* All of these signal handlers are for the "Raise on Events" option */
	gaim_signal_connect(gaim_blist_get_handle(), "buddy-signed-on",
			gtkblist, GAIM_CALLBACK(raise_on_events_cb), NULL);
	gaim_signal_connect(gaim_blist_get_handle(), "buddy-signed-off",
			gtkblist, GAIM_CALLBACK(raise_on_events_cb), NULL);
}

static void gaim_gtk_blist_new_node(GaimBlistNode *node)
{
	node->ui_data = g_new0(struct _gaim_gtk_blist_node, 1);
}

void gaim_gtk_blist_update_columns()
{
	if(!gtkblist)
		return;

	if (gaim_prefs_get_bool("/gaim/gtk/blist/show_buddy_icons")) {
		gtk_tree_view_column_set_visible(gtkblist->buddy_icon_column, TRUE);
		gtk_tree_view_column_set_visible(gtkblist->idle_column, FALSE);
		gtk_tree_view_column_set_visible(gtkblist->warning_column, FALSE);
	} else {
		gtk_tree_view_column_set_visible(gtkblist->idle_column,
				gaim_prefs_get_bool("/gaim/gtk/blist/show_idle_time"));
		gtk_tree_view_column_set_visible(gtkblist->warning_column,
				gaim_prefs_get_bool("/gaim/gtk/blist/show_warning_level"));
		gtk_tree_view_column_set_visible(gtkblist->buddy_icon_column, FALSE);
	}
}

enum {DRAG_BUDDY, DRAG_ROW, DRAG_VCARD, DRAG_TEXT, DRAG_URI,NUM_TARGETS};

static char *
item_factory_translate_func (const char *path, gpointer func_data)
{
	return _((char *)path);
}

void gaim_gtk_blist_setup_sort_methods()
{
	gaim_gtk_blist_sort_method_reg("none", _("None"), sort_method_none);
#if GTK_CHECK_VERSION(2,2,1)
	gaim_gtk_blist_sort_method_reg("alphabetical", _("Alphabetical"), sort_method_alphabetical);
	gaim_gtk_blist_sort_method_reg("status", _("By status"), sort_method_status);
	gaim_gtk_blist_sort_method_reg("log_size", _("By log size"), sort_method_log);
#endif
	gaim_gtk_blist_sort_method_set(gaim_prefs_get_string("/gaim/gtk/blist/sort_type"));
}

static void _prefs_change_redo_list() {
	redo_buddy_list(gaim_get_blist(), TRUE);
}

static void _prefs_change_sort_method(const char *pref_name, GaimPrefType type,
		gpointer val, gpointer data) {
	if(!strcmp(pref_name, "/gaim/gtk/blist/sort_type"))
		gaim_gtk_blist_sort_method_set(val);
}

static void gaim_gtk_blist_show(GaimBuddyList *list)
{
	GtkCellRenderer *rend;
	GtkTreeViewColumn *column;
	GtkWidget *sw;
	GtkWidget *button;
	GtkSizeGroup *sg;
	GtkAccelGroup *accel_group;
	GtkTreeSelection *selection;
	GtkTargetEntry dte[] = {{"GAIM_BLIST_NODE", GTK_TARGET_SAME_APP, DRAG_ROW},
				{"application/x-im-contact", 0, DRAG_BUDDY},
				{"text/x-vcard", 0, DRAG_VCARD },
				{"text/uri-list", 0, DRAG_URI},
				{"text/plain", 0, DRAG_TEXT}};
	GtkTargetEntry ste[] = {{"GAIM_BLIST_NODE", GTK_TARGET_SAME_APP, DRAG_ROW},
				{"application/x-im-contact", 0, DRAG_BUDDY},
				{"text/x-vcard", 0, DRAG_VCARD }};
	if (gtkblist && gtkblist->window) {
		gtk_widget_show(gtkblist->window);
		return;
	}

	gtkblist = GAIM_GTK_BLIST(list);

	gtkblist->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_role(GTK_WINDOW(gtkblist->window), "buddy_list");
	gtk_window_set_title(GTK_WINDOW(gtkblist->window), _("Buddy List"));

	gtkblist->vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(gtkblist->window), gtkblist->vbox);

	g_signal_connect(G_OBJECT(gtkblist->window), "delete_event", G_CALLBACK(gtk_blist_delete_cb), NULL);
	g_signal_connect(G_OBJECT(gtkblist->window), "configure_event", G_CALLBACK(gtk_blist_configure_cb), NULL);
	g_signal_connect(G_OBJECT(gtkblist->window), "visibility_notify_event", G_CALLBACK(gtk_blist_visibility_cb), NULL);
	gtk_widget_add_events(gtkblist->window, GDK_VISIBILITY_NOTIFY_MASK);

	/******************************* Menu bar *************************************/
	accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW (gtkblist->window), accel_group);
	g_object_unref(accel_group);
	gtkblist->ift = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<GaimMain>", accel_group);
	gtk_item_factory_set_translate_func(gtkblist->ift,
										item_factory_translate_func,
										NULL, NULL);
	gtk_item_factory_create_items(gtkblist->ift, sizeof(blist_menu) / sizeof(*blist_menu),
								  blist_menu, NULL);
	gaim_gtk_load_accels();
	g_signal_connect(G_OBJECT(accel_group), "accel-changed",
														G_CALLBACK(gaim_gtk_save_accels_cb), NULL);
	gtk_box_pack_start(GTK_BOX(gtkblist->vbox), gtk_item_factory_get_widget(gtkblist->ift, "<GaimMain>"), FALSE, FALSE, 0);

	awaymenu = gtk_item_factory_get_widget(gtkblist->ift, N_("/Tools/Away"));
	do_away_menu();

	gtkblist->bpmenu = gtk_item_factory_get_widget(gtkblist->ift, N_("/Tools/Buddy Pounce"));
	gaim_gtkpounce_menu_build(gtkblist->bpmenu);

	protomenu = gtk_item_factory_get_widget(gtkblist->ift, N_("/Tools/Account Actions"));
	gaim_gtk_blist_update_protocol_actions();

	pluginmenu = gtk_item_factory_get_widget(gtkblist->ift, N_("/Tools/Plugin Actions"));
	gaim_gtk_blist_update_plugin_actions();
	/****************************** GtkTreeView **********************************/
	sw = gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw), GTK_SHADOW_IN);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	gtkblist->treemodel = gtk_tree_store_new(BLIST_COLUMNS,
			GDK_TYPE_PIXBUF, G_TYPE_BOOLEAN, G_TYPE_STRING,
			G_TYPE_STRING, G_TYPE_STRING, GDK_TYPE_PIXBUF, G_TYPE_POINTER);

	gtkblist->treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(gtkblist->treemodel));
	gtk_widget_set_name(gtkblist->treeview, "gaim_gtkblist_treeview");

	/* Set up selection stuff */

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(gtkblist->treeview));
	g_signal_connect(G_OBJECT(selection), "changed", G_CALLBACK(gaim_gtk_blist_selection_changed), NULL);


	/* Set up dnd */
	gtk_tree_view_enable_model_drag_source(GTK_TREE_VIEW(gtkblist->treeview),
										   GDK_BUTTON1_MASK, ste, 3,
										   GDK_ACTION_COPY);
	gtk_tree_view_enable_model_drag_dest(GTK_TREE_VIEW(gtkblist->treeview),
										 dte, 5,
										 GDK_ACTION_COPY | GDK_ACTION_MOVE);

  	g_signal_connect(G_OBJECT(gtkblist->treeview), "drag-data-received", G_CALLBACK(gaim_gtk_blist_drag_data_rcv_cb), NULL);
	g_signal_connect(G_OBJECT(gtkblist->treeview), "drag-data-get", G_CALLBACK(gaim_gtk_blist_drag_data_get_cb), NULL);

	/* Tooltips */
	g_signal_connect(G_OBJECT(gtkblist->treeview), "motion-notify-event", G_CALLBACK(gaim_gtk_blist_motion_cb), NULL);
	g_signal_connect(G_OBJECT(gtkblist->treeview), "leave-notify-event", G_CALLBACK(gaim_gtk_blist_leave_cb), NULL);

	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(gtkblist->treeview), FALSE);

	column = gtk_tree_view_column_new ();

	rend = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_column_pack_start(column, rend, FALSE);
	gtk_tree_view_column_set_attributes(column, rend,
										"pixbuf", STATUS_ICON_COLUMN,
										"visible", STATUS_ICON_VISIBLE_COLUMN,
										NULL);
	g_object_set(rend, "xalign", 0.0, "ypad", 0, NULL);

	rend = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start (column, rend, TRUE);
	gtk_tree_view_column_set_attributes(column, rend,
										"markup", NAME_COLUMN,
										NULL);
	g_object_set(rend, "ypad", 0, "yalign", 0.5, NULL);

	gtk_tree_view_append_column(GTK_TREE_VIEW(gtkblist->treeview), column);

	rend = gtk_cell_renderer_text_new();
	gtkblist->warning_column = gtk_tree_view_column_new_with_attributes("Warning", rend, "markup", WARNING_COLUMN, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(gtkblist->treeview), gtkblist->warning_column);
	g_object_set(rend, "xalign", 1.0, "ypad", 0, NULL);

	rend = gtk_cell_renderer_text_new();
	gtkblist->idle_column = gtk_tree_view_column_new_with_attributes("Idle", rend, "markup", IDLE_COLUMN, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(gtkblist->treeview), gtkblist->idle_column);
	g_object_set(rend, "xalign", 1.0, "ypad", 0, NULL);

	rend = gtk_cell_renderer_pixbuf_new();
	gtkblist->buddy_icon_column = gtk_tree_view_column_new_with_attributes("Buddy Icon", rend, "pixbuf", BUDDY_ICON_COLUMN, NULL);
	g_object_set(rend, "xalign", 1.0, "ypad", 0, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(gtkblist->treeview), gtkblist->buddy_icon_column);

	g_signal_connect(G_OBJECT(gtkblist->treeview), "row-activated", G_CALLBACK(gtk_blist_row_activated_cb), NULL);
	g_signal_connect(G_OBJECT(gtkblist->treeview), "row-expanded", G_CALLBACK(gtk_blist_row_expanded_cb), NULL);
	g_signal_connect(G_OBJECT(gtkblist->treeview), "row-collapsed", G_CALLBACK(gtk_blist_row_collapsed_cb), NULL);
	g_signal_connect(G_OBJECT(gtkblist->treeview), "button-press-event", G_CALLBACK(gtk_blist_button_press_cb), NULL);
	g_signal_connect(G_OBJECT(gtkblist->treeview), "key-press-event", G_CALLBACK(gtk_blist_key_press_cb), NULL);
	g_signal_connect(G_OBJECT(gtkblist->treeview), "popup-menu", G_CALLBACK(gaim_gtk_blist_popup_menu_cb), NULL);

	/* Enable CTRL+F searching */
	gtk_tree_view_set_search_column(GTK_TREE_VIEW(gtkblist->treeview), NAME_COLUMN);

	gtk_box_pack_start(GTK_BOX(gtkblist->vbox), sw, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(sw), gtkblist->treeview);
	gaim_gtk_blist_update_columns();

	/* set the Show Offline Buddies option. must be done
	 * after the treeview or faceprint gets mad. -Robot101
	 */
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_item (gtkblist->ift, N_("/Buddies/Show Offline Buddies"))),
			gaim_prefs_get_bool("/gaim/gtk/blist/show_offline_buddies"));
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(gtk_item_factory_get_item (gtkblist->ift, N_("/Buddies/Show Empty Groups"))),
			gaim_prefs_get_bool("/gaim/gtk/blist/show_empty_groups"));

	/* OK... let's show this bad boy. */
	gaim_gtk_blist_refresh(list);
	gaim_gtk_blist_restore_position();
	gtk_widget_show_all(gtkblist->window);

	/**************************** Button Box **************************************/
	/* add this afterwards so it doesn't force up the width of the window         */
	/* What?  Won't GTK dynamically resize the width to accomodate the widgets?   */

	gtkblist->tooltips = gtk_tooltips_new();

	sg = gtk_size_group_new(GTK_SIZE_GROUP_BOTH);
	gtkblist->bbox = gtk_hbox_new(TRUE, 0);
	gtk_box_pack_start(GTK_BOX(gtkblist->vbox), gtkblist->bbox, FALSE, FALSE, 0);
	gtk_widget_show(gtkblist->bbox);

	button = gaim_pixbuf_button_from_stock(_("I_M"), GAIM_STOCK_IM, GAIM_BUTTON_VERTICAL);
	gtk_box_pack_start(GTK_BOX(gtkblist->bbox), button, FALSE, FALSE, 0);
	gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
	gtk_size_group_add_widget(sg, button);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_blist_button_im_cb),
			 gtkblist->treeview);
	gtk_tooltips_set_tip(GTK_TOOLTIPS(gtkblist->tooltips), button, _("Send a message to the selected buddy"), NULL);
	g_object_set_data(G_OBJECT(button), "button_name", "im");
	gtk_widget_show(button);

	button = gaim_pixbuf_button_from_stock(_("Get _Info"), GAIM_STOCK_INFO, GAIM_BUTTON_VERTICAL);
	gtk_box_pack_start(GTK_BOX(gtkblist->bbox), button, FALSE, FALSE, 0);
	gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
	gtk_size_group_add_widget(sg, button);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_blist_button_info_cb),
			 gtkblist->treeview);
	gtk_tooltips_set_tip(GTK_TOOLTIPS(gtkblist->tooltips), button, _("Get information on the selected buddy"), NULL);
	g_object_set_data(G_OBJECT(button), "button_name", "info");
	gtk_widget_show(button);

	button = gaim_pixbuf_button_from_stock(_("_Chat"), GAIM_STOCK_CHAT, GAIM_BUTTON_VERTICAL);
	gtk_box_pack_start(GTK_BOX(gtkblist->bbox), button, FALSE, FALSE, 0);
	gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
	gtk_size_group_add_widget(sg, button);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_blist_button_chat_cb), gtkblist->treeview);
	gtk_tooltips_set_tip(GTK_TOOLTIPS(gtkblist->tooltips), button, _("Join a chat room"), NULL);
	gtk_widget_set_sensitive(button, gaim_gtk_blist_joinchat_is_showable());
	g_object_set_data(G_OBJECT(button), "button_name", "chat");
	gtk_widget_show(button);

	button = gaim_pixbuf_button_from_stock(_("_Away"), GAIM_STOCK_ICON_AWAY, GAIM_BUTTON_VERTICAL);
	gtk_box_pack_start(GTK_BOX(gtkblist->bbox), button, FALSE, FALSE, 0);
	gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
	gtk_size_group_add_widget(sg, button);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_blist_button_away_cb), NULL);
	gtk_tooltips_set_tip(GTK_TOOLTIPS(gtkblist->tooltips), button, _("Set an away message"), NULL);
	g_object_set_data(G_OBJECT(button), "button_name", "away");
	gtk_widget_show(button);

	/* this will show the right image/label widgets for us */
	gaim_gtk_blist_update_toolbar();

	/* start the refresh timer */
	if (gaim_prefs_get_bool("/gaim/gtk/blist/show_idle_time") ||
		gaim_prefs_get_bool("/gaim/gtk/blist/show_buddy_icons")) {

		gtkblist->refresh_timer = g_timeout_add(30000,
				(GSourceFunc)gaim_gtk_blist_refresh_timer, list);
	}

	/* attach prefs callbacks */
	/* for the toolbar buttons */
	blist_prefs_callbacks = g_slist_prepend(blist_prefs_callbacks,
			GINT_TO_POINTER(
				gaim_prefs_connect_callback("/gaim/gtk/blist/button_style",
					gaim_gtk_blist_update_toolbar, NULL)));
	blist_prefs_callbacks = g_slist_prepend(blist_prefs_callbacks,
			GINT_TO_POINTER(
				gaim_prefs_connect_callback("/gaim/gtk/blist/show_buttons",
					gaim_gtk_blist_update_toolbar, NULL)));

	/* things that affect how buddies are displayed */
	blist_prefs_callbacks = g_slist_prepend(blist_prefs_callbacks,
			GINT_TO_POINTER(
				gaim_prefs_connect_callback("/gaim/gtk/blist/grey_idle_buddies",
					_prefs_change_redo_list, NULL)));
	blist_prefs_callbacks = g_slist_prepend(blist_prefs_callbacks,
			GINT_TO_POINTER(
				gaim_prefs_connect_callback("/gaim/gtk/blist/show_buddy_icons",
					_prefs_change_redo_list, NULL)));
	blist_prefs_callbacks = g_slist_prepend(blist_prefs_callbacks,
			GINT_TO_POINTER(
				gaim_prefs_connect_callback("/gaim/gtk/blist/show_warning_level",
					_prefs_change_redo_list, NULL)));
	blist_prefs_callbacks = g_slist_prepend(blist_prefs_callbacks,
			GINT_TO_POINTER(
				gaim_prefs_connect_callback("/gaim/gtk/blist/show_idle_time",
					_prefs_change_redo_list, NULL)));
	blist_prefs_callbacks = g_slist_prepend(blist_prefs_callbacks,
			GINT_TO_POINTER(
				gaim_prefs_connect_callback("/gaim/gtk/blist/show_empty_groups",
					_prefs_change_redo_list, NULL)));
	blist_prefs_callbacks = g_slist_prepend(blist_prefs_callbacks,
			GINT_TO_POINTER(
				gaim_prefs_connect_callback("/gaim/gtk/blist/show_offline_buddies",
					_prefs_change_redo_list, NULL)));

	/* sorting */
	blist_prefs_callbacks = g_slist_prepend(blist_prefs_callbacks,
			GINT_TO_POINTER(
				gaim_prefs_connect_callback("/gaim/gtk/blist/sort_type",
					_prefs_change_sort_method, NULL)));

	/* things that affect what columns are displayed */
	blist_prefs_callbacks = g_slist_prepend(blist_prefs_callbacks,
			GINT_TO_POINTER(
				gaim_prefs_connect_callback("/gaim/gtk/blist/show_buddy_icons",
					gaim_gtk_blist_update_columns, NULL)));
	blist_prefs_callbacks = g_slist_prepend(blist_prefs_callbacks,
			GINT_TO_POINTER(
				gaim_prefs_connect_callback("/gaim/gtk/blist/show_idle_time",
					gaim_gtk_blist_update_columns, NULL)));
	blist_prefs_callbacks = g_slist_prepend(blist_prefs_callbacks,
			GINT_TO_POINTER(
				gaim_prefs_connect_callback("/gaim/gtk/blist/show_warning_level",
					gaim_gtk_blist_update_columns, NULL)));

	/* Setup some gaim signal handlers. */
	gaim_signal_connect(gaim_connections_get_handle(), "signed-on",
						gtkblist, GAIM_CALLBACK(sign_on_off_cb), list);
	gaim_signal_connect(gaim_connections_get_handle(), "signed-off",
						gtkblist, GAIM_CALLBACK(sign_on_off_cb), list);

	gaim_signal_connect(gaim_plugins_get_handle(), "plugin-load",
			gtkblist, GAIM_CALLBACK(plugin_changed_cb), NULL);
	gaim_signal_connect(gaim_plugins_get_handle(), "plugin-unload",
			gtkblist, GAIM_CALLBACK(plugin_changed_cb), NULL);

	/* emit our created signal */
	gaim_signal_emit(gaim_gtk_blist_get_handle(), "gtkblist-created", list);
}

/* XXX: does this need fixing? */
static void redo_buddy_list(GaimBuddyList *list, gboolean remove)
{
	GaimBlistNode *gnode, *cnode, *bnode;

	for(gnode = list->root; gnode; gnode = gnode->next) {
		if(!GAIM_BLIST_NODE_IS_GROUP(gnode))
			continue;
		for(cnode = gnode->child; cnode; cnode = cnode->next) {
			if(GAIM_BLIST_NODE_IS_CONTACT(cnode)) {
				if(remove)
					gaim_gtk_blist_hide_node(list, cnode);

				for(bnode = cnode->child; bnode; bnode = bnode->next) {
					if(!GAIM_BLIST_NODE_IS_BUDDY(bnode))
						continue;
					if(remove)
						gaim_gtk_blist_hide_node(list, bnode);
					gaim_gtk_blist_update(list, bnode);
				}

				gaim_gtk_blist_update(list, cnode);
			} else if(GAIM_BLIST_NODE_IS_CHAT(cnode)) {
				if(remove)
					gaim_gtk_blist_hide_node(list, cnode);

				gaim_gtk_blist_update(list, cnode);
			}
		}
		gaim_gtk_blist_update(list, gnode);
	}
}

void gaim_gtk_blist_refresh(GaimBuddyList *list)
{
	redo_buddy_list(list, FALSE);
}

void
gaim_gtk_blist_update_refresh_timeout()
{
	GaimBuddyList *blist;
	GaimGtkBuddyList *gtkblist;

	blist = gaim_get_blist();
	gtkblist = GAIM_GTK_BLIST(gaim_get_blist());

	if (gaim_prefs_get_bool("/gaim/gtk/blist/show_idle_time") ||
		gaim_prefs_get_bool("/gaim/gtk/blist/show_buddy_icons")) {

		gtkblist->refresh_timer = g_timeout_add(30000,
				(GSourceFunc)gaim_gtk_blist_refresh_timer, blist);
	} else {
		g_source_remove(gtkblist->refresh_timer);
		gtkblist->refresh_timer = 0;
	}
}

static gboolean get_iter_from_node(GaimBlistNode *node, GtkTreeIter *iter) {
	struct _gaim_gtk_blist_node *gtknode = (struct _gaim_gtk_blist_node *)node->ui_data;
	GtkTreePath *path;

	/* XXX: why do we assume we have a buddy here? */
	if (!gtknode) {
#if 0
		gaim_debug(GAIM_DEBUG_ERROR, "gtkblist", "buddy %s has no ui_data\n", ((GaimBuddy *)node)->name);
#endif
		return FALSE;
	}

	if (!gtkblist) {
		gaim_debug(GAIM_DEBUG_ERROR, "gtkblist", "get_iter_from_node was called, but we don't seem to have a blist\n");
		return FALSE;
	}

	if (!gtknode->row)
		return FALSE;

	if ((path = gtk_tree_row_reference_get_path(gtknode->row)) == NULL)
		return FALSE;
	if (!gtk_tree_model_get_iter(GTK_TREE_MODEL(gtkblist->treemodel), iter, path)) {
		gtk_tree_path_free(path);
		return FALSE;
	}
	gtk_tree_path_free(path);
	return TRUE;
}

static void
gaim_gtk_blist_update_toolbar_icons (GtkWidget *widget, gpointer data)
{
	GaimButtonStyle style = gaim_prefs_get_int("/gaim/gtk/blist/button_style");

	if (GTK_IS_IMAGE(widget)) {
		if (style == GAIM_BUTTON_IMAGE || style == GAIM_BUTTON_TEXT_IMAGE)
			gtk_widget_show(widget);
		else
			gtk_widget_hide(widget);
	}
	else if (GTK_IS_LABEL(widget)) {
		if (style == GAIM_BUTTON_IMAGE)
			gtk_widget_hide(widget);
		else
			gtk_widget_show(widget);
	}
	else if (GTK_IS_CONTAINER(widget)) {
		if (GTK_IS_BUTTON(widget) && !strcmp(g_object_get_data(G_OBJECT(widget), "button_name"), "chat"))
				gtk_widget_set_sensitive(widget, gaim_gtk_blist_joinchat_is_showable());
		gtk_container_foreach(GTK_CONTAINER(widget),
							  gaim_gtk_blist_update_toolbar_icons, NULL);
	}
}

void gaim_gtk_blist_update_toolbar() {
	if (!gtkblist)
		return;

	if (gaim_prefs_get_int("/gaim/gtk/blist/button_style") == GAIM_BUTTON_NONE)
		gtk_widget_hide(gtkblist->bbox);
	else {
		gtk_container_foreach(GTK_CONTAINER(gtkblist->bbox),
							  gaim_gtk_blist_update_toolbar_icons, NULL);
		gtk_widget_show(gtkblist->bbox);
	}
}

static void gaim_gtk_blist_remove(GaimBuddyList *list, GaimBlistNode *node)
{
	gaim_gtk_blist_hide_node(list, node);

	if(node->parent)
		gaim_gtk_blist_update(list, node->parent);

	/* There's something I don't understand here */
	/* g_free(node->ui_data);
	node->ui_data = NULL; */
}

static gboolean do_selection_changed(GaimBlistNode *new_selection)
{
	GaimBlistNode *old_selection = NULL;

	/* test for gtkblist because crazy timeout means we can be called after the blist is gone */
	if (gtkblist && new_selection != gtkblist->selected_node) {
		old_selection = gtkblist->selected_node;
		gtkblist->selected_node = new_selection;
		if(new_selection)
			gaim_gtk_blist_update(NULL, new_selection);
		if(old_selection)
			gaim_gtk_blist_update(NULL, old_selection);
	}

	return FALSE;
}

static void gaim_gtk_blist_selection_changed(GtkTreeSelection *selection, gpointer data)
{
	GaimBlistNode *new_selection = NULL;
	GtkTreeIter iter;

	if(gtk_tree_selection_get_selected(selection, NULL, &iter)){
		gtk_tree_model_get(GTK_TREE_MODEL(gtkblist->treemodel), &iter,
				NODE_COLUMN, &new_selection, -1);
	}

	/* we set this up as a timeout, otherwise the blist flickers */
	g_timeout_add(0, (GSourceFunc)do_selection_changed, new_selection);
}

static gboolean insert_node(GaimBuddyList *list, GaimBlistNode *node, GtkTreeIter *iter)
{
	GtkTreeIter parent_iter, cur, *curptr = NULL;
	struct _gaim_gtk_blist_node *gtknode = node->ui_data;
	GtkTreePath *newpath;

	if(!gtknode || !iter)
		return FALSE;

	if(node->parent && !get_iter_from_node(node->parent, &parent_iter))
		return FALSE;

	if(get_iter_from_node(node, &cur))
		curptr = &cur;

	if(GAIM_BLIST_NODE_IS_CONTACT(node) || GAIM_BLIST_NODE_IS_CHAT(node)) {
		*iter = current_sort_method->func(node, list, parent_iter, curptr);
	} else {
		*iter = sort_method_none(node, list, parent_iter, curptr);
	}

	gtk_tree_row_reference_free(gtknode->row);
	newpath = gtk_tree_model_get_path(GTK_TREE_MODEL(gtkblist->treemodel),
			iter);
	gtknode->row =
		gtk_tree_row_reference_new(GTK_TREE_MODEL(gtkblist->treemodel),
				newpath);
	gtk_tree_path_free(newpath);

	gtk_tree_store_set(gtkblist->treemodel, iter,
			NODE_COLUMN, node,
			-1);

	if(node->parent) {
		GtkTreePath *expand = NULL;
		struct _gaim_gtk_blist_node *gtkparentnode = node->parent->ui_data;

		if(GAIM_BLIST_NODE_IS_GROUP(node->parent)) {
			if(!gaim_blist_node_get_bool(node->parent, "collapsed"))
				expand = gtk_tree_model_get_path(GTK_TREE_MODEL(gtkblist->treemodel), &parent_iter);
		} else if(GAIM_BLIST_NODE_IS_CONTACT(node->parent) &&
				gtkparentnode->contact_expanded) {
			expand = gtk_tree_model_get_path(GTK_TREE_MODEL(gtkblist->treemodel), &parent_iter);
		}
		if(expand) {
			gtk_tree_view_expand_row(GTK_TREE_VIEW(gtkblist->treeview), expand, FALSE);
			gtk_tree_path_free(expand);
		}
	}

	return TRUE;
}

static void gaim_gtk_blist_update_group(GaimBuddyList *list, GaimBlistNode *node)
{
	GaimGroup *group;
	int count;

	g_return_if_fail(GAIM_BLIST_NODE_IS_GROUP(node));

	group = (GaimGroup*)node;

	if(gaim_prefs_get_bool("/gaim/gtk/blist/show_offline_buddies"))
		count = gaim_blist_get_group_size(group, FALSE);
	else
		count = gaim_blist_get_group_online_count(group);
	if(gaim_prefs_get_bool("/gaim/gtk/blist/show_empty_groups") || count > 0) {
		char *mark, *esc;
		GtkTreeIter iter;

		if(!insert_node(list, node, &iter))
			return;

		esc = g_markup_escape_text(group->name, -1);
		mark = g_strdup_printf("<span weight='bold'>%s</span> (%d/%d)",
				esc, gaim_blist_get_group_online_count(group),
				gaim_blist_get_group_size(group, FALSE));
		g_free(esc);

		gtk_tree_store_set(gtkblist->treemodel, &iter,
				STATUS_ICON_COLUMN, NULL,
				STATUS_ICON_VISIBLE_COLUMN, FALSE,
				NAME_COLUMN, mark,
				NODE_COLUMN, node,
				-1);
		g_free(mark);
	} else {
		gaim_gtk_blist_hide_node(list, node);
	}
}

static void buddy_node(GaimBuddy *buddy, GtkTreeIter *iter, GaimBlistNode *node)
{
	GdkPixbuf *status, *avatar;
	char *mark;
	char *warning = NULL, *idle = NULL;

	gboolean selected = (gtkblist->selected_node == node);

	status = gaim_gtk_blist_get_status_icon((GaimBlistNode*)buddy,
			(gaim_prefs_get_bool("/gaim/gtk/blist/show_buddy_icons")
			 ? GAIM_STATUS_ICON_LARGE : GAIM_STATUS_ICON_SMALL));

	avatar = gaim_gtk_blist_get_buddy_icon(buddy);
	mark = gaim_gtk_blist_get_name_markup(buddy, selected);

	if (buddy->idle > 0) {
		time_t t;
		int ihrs, imin;
		time(&t);
		ihrs = (t - buddy->idle) / 3600;
		imin = ((t - buddy->idle) / 60) % 60;
		if(ihrs > 0)
			idle = g_strdup_printf("(%d:%02d)", ihrs, imin);
		else
			idle = g_strdup_printf("(%d)", imin);
	}

	if (buddy->evil > 0)
		warning = g_strdup_printf("%d%%", buddy->evil);

	if (gaim_prefs_get_bool("/gaim/gtk/blist/grey_idle_buddies") &&
			buddy->idle) {

		if(warning && !selected) {
			char *w2 = g_strdup_printf("<span color='dim grey'>%s</span>",
					warning);
			g_free(warning);
			warning = w2;
		}

		if(idle && !selected) {
			char *i2 = g_strdup_printf("<span color='dim grey'>%s</span>",
					idle);
			g_free(idle);
			idle = i2;
		}
	}

	gtk_tree_store_set(gtkblist->treemodel, iter,
			STATUS_ICON_COLUMN, status,
			STATUS_ICON_VISIBLE_COLUMN, TRUE,
			NAME_COLUMN, mark,
			WARNING_COLUMN, warning,
			IDLE_COLUMN, idle,
			BUDDY_ICON_COLUMN, avatar,
			-1);

	g_free(mark);
	if(idle)
		g_free(idle);
	if(warning)
		g_free(warning);
	if(status)
		g_object_unref(status);
	if(avatar)
		g_object_unref(avatar);
}

static void gaim_gtk_blist_update_contact(GaimBuddyList *list, GaimBlistNode *node)
{
	GaimContact *contact;
	GaimBuddy *buddy;
	struct _gaim_gtk_blist_node *gtknode;

	g_return_if_fail(GAIM_BLIST_NODE_IS_CONTACT(node));

	/* First things first, update the group */
	gaim_gtk_blist_update_group(list, node->parent);

	gtknode = (struct _gaim_gtk_blist_node *)node->ui_data;
	contact = (GaimContact*)node;
	buddy = gaim_contact_get_priority_buddy(contact);

	if(buddy && (buddy->present != GAIM_BUDDY_OFFLINE ||
			(gaim_account_is_connected(buddy->account) &&
			 gaim_prefs_get_bool("/gaim/gtk/blist/show_offline_buddies")) ||
			gaim_blist_node_get_bool(node, "show_offline"))) {
		GtkTreeIter iter;

		if(!insert_node(list, node, &iter))
			return;

		if(gtknode->contact_expanded) {
			GdkPixbuf *status;
			char *mark;

			status = gaim_gtk_blist_get_status_icon(node,
					(gaim_prefs_get_bool("/gaim/gtk/blist/show_buddy_icons") ?
					 GAIM_STATUS_ICON_LARGE : GAIM_STATUS_ICON_SMALL));

			mark = g_markup_escape_text(gaim_contact_get_alias(contact), -1);

			gtk_tree_store_set(gtkblist->treemodel, &iter,
					STATUS_ICON_COLUMN, status,
					STATUS_ICON_VISIBLE_COLUMN, TRUE,
					NAME_COLUMN, mark,
					WARNING_COLUMN, NULL,
					IDLE_COLUMN, NULL,
					BUDDY_ICON_COLUMN, NULL,
					-1);
			g_free(mark);
			if(status)
				g_object_unref(status);
		} else {
			buddy_node(buddy, &iter, node);
		}
	} else {
		gaim_gtk_blist_hide_node(list, node);
	}
}

static void gaim_gtk_blist_update_buddy(GaimBuddyList *list, GaimBlistNode *node)
{
	GaimContact *contact;
	GaimBuddy *buddy;
	struct _gaim_gtk_blist_node *gtkparentnode;

	g_return_if_fail(GAIM_BLIST_NODE_IS_BUDDY(node));

	buddy = (GaimBuddy*)node;
	contact = (GaimContact*)node->parent;
	gtkparentnode = (struct _gaim_gtk_blist_node *)node->parent->ui_data;

	/* First things first, update the contact */
	gaim_gtk_blist_update_contact(list, node->parent);

	if(gtkparentnode->contact_expanded &&
			(buddy->present != GAIM_BUDDY_OFFLINE ||
			(gaim_account_is_connected(buddy->account) &&
			 gaim_prefs_get_bool("/gaim/gtk/blist/show_offline_buddies")) ||
			gaim_blist_node_get_bool(node->parent, "show_offline"))) {
		GtkTreeIter iter;

		if(!insert_node(list, node, &iter))
			return;

		buddy_node(buddy, &iter, node);

	} else {
		gaim_gtk_blist_hide_node(list, node);
	}

}

static void gaim_gtk_blist_update_chat(GaimBuddyList *list, GaimBlistNode *node)
{
	GaimChat *chat;

	g_return_if_fail(GAIM_BLIST_NODE_IS_CHAT(node));

	/* First things first, update the group */
	gaim_gtk_blist_update_group(list, node->parent);

	chat = (GaimChat*)node;

	if(gaim_account_is_connected(chat->account)) {
		GtkTreeIter iter;
		GdkPixbuf *status;
		char *mark;

		if(!insert_node(list, node, &iter))
			return;

		status = gaim_gtk_blist_get_status_icon(node,
				(gaim_prefs_get_bool("/gaim/gtk/blist/show_buddy_icons") ?
				 GAIM_STATUS_ICON_LARGE : GAIM_STATUS_ICON_SMALL));

		mark = g_markup_escape_text(gaim_chat_get_name(chat), -1);

		gtk_tree_store_set(gtkblist->treemodel, &iter,
				STATUS_ICON_COLUMN, status,
				STATUS_ICON_VISIBLE_COLUMN, TRUE,
				NAME_COLUMN, mark,
				-1);

		g_free(mark);
		if(status)
			g_object_unref(status);
	} else {
		gaim_gtk_blist_hide_node(list, node);
	}
}

static void gaim_gtk_blist_update(GaimBuddyList *list, GaimBlistNode *node)
{
	if(!gtkblist)
		return;

	switch(node->type) {
		case GAIM_BLIST_GROUP_NODE:
			gaim_gtk_blist_update_group(list, node);
			break;
		case GAIM_BLIST_CONTACT_NODE:
			gaim_gtk_blist_update_contact(list, node);
			break;
		case GAIM_BLIST_BUDDY_NODE:
			gaim_gtk_blist_update_buddy(list, node);
			break;
		case GAIM_BLIST_CHAT_NODE:
			gaim_gtk_blist_update_chat(list, node);
			break;
		case GAIM_BLIST_OTHER_NODE:
			return;
	}

	gtk_tree_view_columns_autosize(GTK_TREE_VIEW(gtkblist->treeview));
}


static void gaim_gtk_blist_destroy(GaimBuddyList *list)
{
	if (!gtkblist)
		return;

	gaim_signal_disconnect(gaim_connections_get_handle(), "signed-on",
						   gtkblist, GAIM_CALLBACK(sign_on_off_cb));
	gaim_signal_disconnect(gaim_connections_get_handle(), "signed-off",
						   gtkblist, GAIM_CALLBACK(sign_on_off_cb));

	gtk_widget_destroy(gtkblist->window);

	gaim_gtk_blist_tooltip_destroy();

	gtk_object_sink(GTK_OBJECT(gtkblist->tooltips));

	if (gtkblist->refresh_timer)
		g_source_remove(gtkblist->refresh_timer);
	if (gtkblist->timeout)
		g_source_remove(gtkblist->timeout);

	gtkblist->refresh_timer = 0;
	gtkblist->timeout = 0;
	gtkblist->window = gtkblist->vbox = gtkblist->treeview = NULL;
	gtkblist->treemodel = NULL;
	gtkblist->idle_column = NULL;
	gtkblist->warning_column = gtkblist->buddy_icon_column = NULL;
	gtkblist->bbox = NULL;
	g_object_unref(G_OBJECT(gtkblist->ift));
	protomenu = NULL;
	pluginmenu = NULL;
	awaymenu = NULL;
	gtkblist = NULL;

	while(blist_prefs_callbacks) {
		gaim_prefs_disconnect_callback(GPOINTER_TO_INT(blist_prefs_callbacks->data));
		blist_prefs_callbacks = g_slist_remove(blist_prefs_callbacks, blist_prefs_callbacks->data);
	}
}

static void gaim_gtk_blist_set_visible(GaimBuddyList *list, gboolean show)
{
	if (!(gtkblist && gtkblist->window))
		return;

	if (show) {
		gaim_gtk_blist_restore_position();
		gtk_window_present(GTK_WINDOW(gtkblist->window));
	} else {
		if (!gaim_connections_get_all() || docklet_count) {
#ifdef _WIN32
			wgaim_systray_minimize(gtkblist->window);
#endif
			gtk_widget_hide(gtkblist->window);
		} else {
			gtk_window_iconify(GTK_WINDOW(gtkblist->window));
		}
	}
}

static GList *
groups_tree(void)
{
	GList *tmp = NULL;
	char *tmp2;
	GaimGroup *g;
	GaimBlistNode *gnode;

	if (gaim_get_blist()->root == NULL)
	{
		tmp2 = g_strdup(_("Buddies"));
		tmp  = g_list_append(tmp, tmp2);
	}
	else
	{
		for (gnode = gaim_get_blist()->root;
			 gnode != NULL;
			 gnode = gnode->next)
		{
			if (GAIM_BLIST_NODE_IS_GROUP(gnode))
			{
				g    = (GaimGroup *)gnode;
				tmp2 = g->name;
				tmp  = g_list_append(tmp, tmp2);
			}
		}
	}

	return tmp;
}

static void
add_buddy_select_account_cb(GObject *w, GaimAccount *account,
							GaimGtkAddBuddyData *data)
{
	/* Save our account */
	data->account = account;
}

static void
destroy_add_buddy_dialog_cb(GtkWidget *win, GaimGtkAddBuddyData *data)
{
	g_free(data);
}

static void
add_buddy_cb(GtkWidget *w, int resp, GaimGtkAddBuddyData *data)
{
	const char *grp, *who, *whoalias;
	GaimConversation *c;
	GaimBuddy *b;
	GaimGroup *g;

	if (resp == GTK_RESPONSE_OK)
	{
		who = gtk_entry_get_text(GTK_ENTRY(data->entry));
		grp = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(data->combo)->entry));
		whoalias = gtk_entry_get_text(GTK_ENTRY(data->entry_for_alias));
		if (*whoalias == '\0')
			whoalias = NULL;

		c = gaim_find_conversation_with_account(who, data->account);

		if ((g = gaim_find_group(grp)) == NULL)
		{
			g = gaim_group_new(grp);
			gaim_blist_add_group(g, NULL);
		}

		b = gaim_buddy_new(data->account, who, whoalias);
		gaim_blist_add_buddy(b, NULL, g, NULL);
		serv_add_buddy(gaim_account_get_connection(data->account), b);

		/*
		 * XXX
		 * It really seems like it would be better if the call to serv_add_buddy()
		 * and gaim_conversation_update() were done in blist.c, possibly in the
		 * gaim_blist_add_buddy() function.  Maybe serv_add_buddy() should be
		 * renamed to gaim_blist_add_new_buddy() or something, and have it call
		 * gaim_blist_add_buddy() after it creates it.  --Mark
		 *
		 * No that's not good.  blist.c should only deal with adding nodes to the
		 * local list.  We need a new, non-gtk file that calls both serv_add_buddy
		 * and gaim_blist_add_buddy().  Or something.  --Mark
		 */

		if (c != NULL) {
			gaim_buddy_icon_update(gaim_conv_im_get_icon(GAIM_CONV_IM(c)));
			gaim_conversation_update(c, GAIM_CONV_UPDATE_ADD);
		}
	}

	gtk_widget_destroy(data->window);
}

static void
gaim_gtk_blist_request_add_buddy(GaimAccount *account, const char *username,
								 const char *group, const char *alias)
{
	GtkWidget *table;
	GtkWidget *label;
	GtkWidget *hbox;
	GtkWidget *vbox;
	GtkWidget *img;
	GaimGtkBuddyList *gtkblist;
	GaimGtkAddBuddyData *data = g_new0(GaimGtkAddBuddyData, 1);

	data->account =
		(account != NULL
		 ? account
		 : gaim_connection_get_account(gaim_connections_get_all()->data));

	img = gtk_image_new_from_stock(GAIM_STOCK_DIALOG_QUESTION,
								   GTK_ICON_SIZE_DIALOG);

	gtkblist = GAIM_GTK_BLIST(gaim_get_blist());

	data->window = gtk_dialog_new_with_buttons(_("Add Buddy"),
			NULL, GTK_DIALOG_NO_SEPARATOR,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_ADD, GTK_RESPONSE_OK,
			NULL);

	gtk_dialog_set_default_response(GTK_DIALOG(data->window), GTK_RESPONSE_OK);
	gtk_container_set_border_width(GTK_CONTAINER(data->window), 6);
	gtk_window_set_resizable(GTK_WINDOW(data->window), FALSE);
	gtk_box_set_spacing(GTK_BOX(GTK_DIALOG(data->window)->vbox), 12);
	gtk_container_set_border_width(GTK_CONTAINER(GTK_DIALOG(data->window)->vbox), 6);
	gtk_window_set_role(GTK_WINDOW(data->window), "add_buddy");
	gtk_window_set_type_hint(GTK_WINDOW(data->window),
							 GDK_WINDOW_TYPE_HINT_DIALOG);

	hbox = gtk_hbox_new(FALSE, 12);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(data->window)->vbox), hbox);
	gtk_box_pack_start(GTK_BOX(hbox), img, FALSE, FALSE, 0);
	gtk_misc_set_alignment(GTK_MISC(img), 0, 0);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(hbox), vbox);

	label = gtk_label_new(
		_("Please enter the screen name of the person you would like "
		  "to add to your buddy list. You may optionally enter an alias, "
		  "or nickname,  for the buddy. The alias will be displayed in "
		  "place of the screen name whenever possible.\n"));

	gtk_widget_set_size_request(GTK_WIDGET(label), 400, -1);
	gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

	hbox = gtk_hbox_new(FALSE, 6);
	gtk_container_add(GTK_CONTAINER(vbox), hbox);

	g_signal_connect(G_OBJECT(data->window), "destroy",
					 G_CALLBACK(destroy_add_buddy_dialog_cb), data);

	table = gtk_table_new(4, 2, FALSE);
	gtk_table_set_row_spacings(GTK_TABLE(table), 5);
	gtk_table_set_col_spacings(GTK_TABLE(table), 5);
	gtk_container_set_border_width(GTK_CONTAINER(table), 0);
	gtk_box_pack_start(GTK_BOX(vbox), table, FALSE, FALSE, 0);

	label = gtk_label_new(_("Screen Name:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 0, 1);

	data->entry = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table), data->entry, 1, 2, 0, 1);
	gtk_widget_grab_focus(data->entry);

	if (username != NULL)
		gtk_entry_set_text(GTK_ENTRY(data->entry), username);

	gtk_entry_set_activates_default (GTK_ENTRY(data->entry), TRUE);
	gaim_set_accessible_label (data->entry, label);

	label = gtk_label_new(_("Alias:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 1, 2);

	data->entry_for_alias = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table),
							  data->entry_for_alias, 1, 2, 1, 2);

	if (alias != NULL)
		gtk_entry_set_text(GTK_ENTRY(data->entry_for_alias), alias);

	gtk_entry_set_activates_default (GTK_ENTRY(data->entry_for_alias), TRUE);
	gaim_set_accessible_label (data->entry_for_alias, label);

	label = gtk_label_new(_("Group:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 2, 3);

	data->combo = gtk_combo_new();
	gtk_combo_set_popdown_strings(GTK_COMBO(data->combo), groups_tree());
	gtk_table_attach_defaults(GTK_TABLE(table), data->combo, 1, 2, 2, 3);
	gaim_set_accessible_label (data->combo, label);

	/* Set up stuff for the account box */
	label = gtk_label_new(_("Account:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 1, 3, 4);

	data->account_box = gaim_gtk_account_option_menu_new(account, FALSE,
			G_CALLBACK(add_buddy_select_account_cb), NULL, data);

	gtk_table_attach_defaults(GTK_TABLE(table), data->account_box, 1, 2, 3, 4);
	gaim_set_accessible_label (data->account_box, label);

	/* End of account box */

	g_signal_connect(G_OBJECT(data->window), "response",
					 G_CALLBACK(add_buddy_cb), data);

	gtk_widget_show_all(data->window);

	if (group != NULL)
		gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(data->combo)->entry), group);
}

static void
add_chat_cb(GtkWidget *w, GaimGtkAddChatData *data)
{
	GHashTable *components;
	GList *tmp;
	GaimChat *chat;
	GaimGroup *group;
	const char *group_name;

	components = g_hash_table_new_full(g_str_hash, g_str_equal,
									   g_free, g_free);

	for (tmp = data->entries; tmp; tmp = tmp->next)
	{
		if (g_object_get_data(tmp->data, "is_spin"))
		{
			g_hash_table_replace(components,
					g_strdup(g_object_get_data(tmp->data, "identifier")),
					g_strdup_printf("%d",
						gtk_spin_button_get_value_as_int(tmp->data)));
		}
		else
		{
			g_hash_table_replace(components,
					g_strdup(g_object_get_data(tmp->data, "identifier")),
					g_strdup(gtk_entry_get_text(tmp->data)));
		}
	}

	chat = gaim_chat_new(data->account,
							   gtk_entry_get_text(GTK_ENTRY(data->alias_entry)),
							   components);

	group_name = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(data->group_combo)->entry));

	if ((group = gaim_find_group(group_name)) == NULL)
	{
		group = gaim_group_new(group_name);
		gaim_blist_add_group(group, NULL);
	}

	if (chat != NULL)
	{
		gaim_blist_add_chat(chat, group, NULL);
	}

	gtk_widget_destroy(data->window);
	g_free(data->default_chat_name);
	g_list_free(data->entries);
	g_free(data);
}

static void
add_chat_resp_cb(GtkWidget *w, int resp, GaimGtkAddChatData *data)
{
	if (resp == GTK_RESPONSE_OK)
	{
		add_chat_cb(NULL, data);
	}
	else
	{
		gtk_widget_destroy(data->window);
		g_free(data->default_chat_name);
		g_list_free(data->entries);
		g_free(data);
	}
}

static void
rebuild_addchat_entries(GaimGtkAddChatData *data)
{
	GaimConnection *gc;
	GList *list, *tmp;
	GHashTable *defaults = NULL;
	struct proto_chat_entry *pce;
	gboolean focus = TRUE;

	gc = gaim_account_get_connection(data->account);

	while (GTK_BOX(data->entries_box)->children)
	{
		gtk_container_remove(GTK_CONTAINER(data->entries_box),
				((GtkBoxChild *)GTK_BOX(data->entries_box)->children->data)->widget);
	}

	if (data->entries != NULL)
		g_list_free(data->entries);

	data->entries = NULL;

	list = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl)->chat_info(gc);

	if (GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl)->chat_info_defaults != NULL)
		defaults = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl)->chat_info_defaults(gc,
					data->default_chat_name);

	for (tmp = list; tmp; tmp = tmp->next)
	{
		GtkWidget *label;
		GtkWidget *rowbox;

		pce = tmp->data;

		rowbox = gtk_hbox_new(FALSE, 5);
		gtk_box_pack_start(GTK_BOX(data->entries_box), rowbox, FALSE, FALSE, 0);

		label = gtk_label_new_with_mnemonic(pce->label);
		gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
		gtk_size_group_add_widget(data->sg, label);
		gtk_box_pack_start(GTK_BOX(rowbox), label, FALSE, FALSE, 0);

		if (pce->is_int)
		{
			GtkObject *adjust;
			GtkWidget *spin;
			adjust = gtk_adjustment_new(pce->min, pce->min, pce->max,
										1, 10, 10);
			spin = gtk_spin_button_new(GTK_ADJUSTMENT(adjust), 1, 0);
			g_object_set_data(G_OBJECT(spin), "is_spin", GINT_TO_POINTER(TRUE));
			g_object_set_data(G_OBJECT(spin), "identifier", pce->identifier);
			data->entries = g_list_append(data->entries, spin);
			gtk_widget_set_size_request(spin, 50, -1);
			gtk_box_pack_end(GTK_BOX(rowbox), spin, FALSE, FALSE, 0);
			gtk_label_set_mnemonic_widget(GTK_LABEL(label), spin);
			gaim_set_accessible_label (spin, label);
		}
		else
		{
			GtkWidget *entry = gtk_entry_new();
			char *value;

			g_object_set_data(G_OBJECT(entry), "identifier", pce->identifier);
			data->entries = g_list_append(data->entries, entry);

			value = g_hash_table_lookup(defaults, pce->identifier);
			if (value != NULL)
				gtk_entry_set_text(GTK_ENTRY(entry), value);

			if (focus)
			{
				gtk_widget_grab_focus(entry);
				focus = FALSE;
			}

			if (pce->secret)
				gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);

			gtk_box_pack_end(GTK_BOX(rowbox), entry, TRUE, TRUE, 0);

			g_signal_connect(G_OBJECT(entry), "activate",
							 G_CALLBACK(add_chat_cb), data);
			gtk_label_set_mnemonic_widget(GTK_LABEL(label), entry);
			gaim_set_accessible_label (entry, label);
		}

		g_free(pce);
	}

	g_list_free(list);
	g_hash_table_destroy(defaults);

	gtk_widget_show_all(data->entries_box);
}

static void
add_chat_select_account_cb(GObject *w, GaimAccount *account,
						   GaimGtkAddChatData *data)
{
	if (strcmp(gaim_account_get_protocol_id(data->account),
		gaim_account_get_protocol_id(account)) == 0)
	{
		data->account = account;
	}
	else
	{
		data->account = account;
		rebuild_addchat_entries(data);
	}
}

static gboolean
add_chat_check_account_func(GaimAccount *account)
{
	GaimConnection *gc = gaim_account_get_connection(account);

	return (GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl)->chat_info != NULL);
}

void
gaim_gtk_blist_request_add_chat(GaimAccount *account, GaimGroup *group,
								const char *alias, const char *name)
{
	GaimGtkAddChatData *data;
	GaimGtkBuddyList *gtkblist;
	GList *l;
	GaimConnection *gc;
	GtkWidget *label;
	GtkWidget *rowbox;
	GtkWidget *hbox;
	GtkWidget *vbox;
	GtkWidget *img;

	if (account != NULL) {
		gc = gaim_account_get_connection(account);

		if (GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl)->join_chat == NULL) {
			gaim_notify_error(gc, NULL, _("This protocol does not support chat rooms."), NULL);
			return;
		}
	} else {
		/* Find an account with chat capabilities */
		for (l = gaim_connections_get_all(); l != NULL; l = l->next) {
			gc = (GaimConnection *)l->data;

			if (GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl)->join_chat != NULL) {
				account = gaim_connection_get_account(gc);
				break;
			}
		}

		if (account == NULL) {
			gaim_notify_error(NULL, NULL,
							  _("You are not currently signed on with any "
								"protocols that have the ability to chat."), NULL);
			return;
		}
	}

	data = g_new0(GaimGtkAddChatData, 1);
	data->account = account;
	data->default_chat_name = g_strdup(name);

	img = gtk_image_new_from_stock(GAIM_STOCK_DIALOG_QUESTION,
								   GTK_ICON_SIZE_DIALOG);

	gtkblist = GAIM_GTK_BLIST(gaim_get_blist());

	data->sg = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);

	data->window = gtk_dialog_new_with_buttons(_("Add Chat"),
		NULL, GTK_DIALOG_NO_SEPARATOR,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_ADD, GTK_RESPONSE_OK,
		NULL);

	gtk_dialog_set_default_response(GTK_DIALOG(data->window), GTK_RESPONSE_OK);
	gtk_container_set_border_width(GTK_CONTAINER(data->window), 6);
	gtk_window_set_resizable(GTK_WINDOW(data->window), FALSE);
	gtk_box_set_spacing(GTK_BOX(GTK_DIALOG(data->window)->vbox), 12);
	gtk_container_set_border_width(GTK_CONTAINER(GTK_DIALOG(data->window)->vbox), 6);
	gtk_window_set_role(GTK_WINDOW(data->window), "add_chat");
	gtk_window_set_type_hint(GTK_WINDOW(data->window),
							 GDK_WINDOW_TYPE_HINT_DIALOG);

	hbox = gtk_hbox_new(FALSE, 12);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(data->window)->vbox), hbox);
	gtk_box_pack_start(GTK_BOX(hbox), img, FALSE, FALSE, 0);
	gtk_misc_set_alignment(GTK_MISC(img), 0, 0);

	vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_add(GTK_CONTAINER(hbox), vbox);

	label = gtk_label_new(
		_("Please enter an alias, and the appropriate information "
		  "about the chat you would like to add to your buddy list.\n"));
	gtk_widget_set_size_request(label, 400, -1);
	gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

	rowbox = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), rowbox, FALSE, FALSE, 0);

	label = gtk_label_new(_("Account:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_size_group_add_widget(data->sg, label);
	gtk_box_pack_start(GTK_BOX(rowbox), label, FALSE, FALSE, 0);

	data->account_menu = gaim_gtk_account_option_menu_new(account, FALSE,
			G_CALLBACK(add_chat_select_account_cb),
			add_chat_check_account_func, data);
	gtk_box_pack_start(GTK_BOX(rowbox), data->account_menu, TRUE, TRUE, 0);
	gaim_set_accessible_label (data->account_menu, label);

	data->entries_box = gtk_vbox_new(FALSE, 5);
	gtk_container_set_border_width(GTK_CONTAINER(data->entries_box), 0);
	gtk_box_pack_start(GTK_BOX(vbox), data->entries_box, TRUE, TRUE, 0);

	rebuild_addchat_entries(data);

	rowbox = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), rowbox, FALSE, FALSE, 0);

	label = gtk_label_new(_("Alias:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_size_group_add_widget(data->sg, label);
	gtk_box_pack_start(GTK_BOX(rowbox), label, FALSE, FALSE, 0);

	data->alias_entry = gtk_entry_new();
	if (alias != NULL)
		gtk_entry_set_text(GTK_ENTRY(data->alias_entry), alias);
	gtk_box_pack_end(GTK_BOX(rowbox), data->alias_entry, TRUE, TRUE, 0);
	gaim_set_accessible_label (data->alias_entry, label);

	rowbox = gtk_hbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), rowbox, FALSE, FALSE, 0);

	label = gtk_label_new(_("Group:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_size_group_add_widget(data->sg, label);
	gtk_box_pack_start(GTK_BOX(rowbox), label, FALSE, FALSE, 0);

	data->group_combo = gtk_combo_new();
	gtk_combo_set_popdown_strings(GTK_COMBO(data->group_combo), groups_tree());
	gtk_box_pack_end(GTK_BOX(rowbox), data->group_combo, TRUE, TRUE, 0);

	if (group)
	{
		gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(data->group_combo)->entry),
						   group->name);
	}
	gaim_set_accessible_label (data->group_combo, label);

	g_signal_connect(G_OBJECT(data->window), "response",
					 G_CALLBACK(add_chat_resp_cb), data);

	gtk_widget_show_all(data->window);
}

static void
add_group_cb(GaimConnection *gc, const char *group_name)
{
	GaimGroup *group;

	group = gaim_group_new(group_name);
	gaim_blist_add_group(group, NULL);
}

void
gaim_gtk_blist_request_add_group(void)
{
	gaim_request_input(NULL, _("Add Group"), NULL,
					   _("Please enter the name of the group to be added."),
					   NULL, FALSE, FALSE, NULL,
					   _("Add"), G_CALLBACK(add_group_cb),
					   _("Cancel"), NULL, NULL);
}

void gaim_gtk_blist_docklet_toggle() {
	/* Useful for the docklet plugin and also for the win32 tray icon*/
	/* This is called when one of those is clicked--it will show/hide the
	   buddy list/login window--depending on which is active */
	if (gaim_connections_get_all()) {
		if (gtkblist && gtkblist->window) {
			if (GTK_WIDGET_VISIBLE(gtkblist->window)) {
				gaim_blist_set_visible(GAIM_WINDOW_ICONIFIED(gtkblist->window) || gaim_gtk_blist_obscured);
			} else {
#if _WIN32
				wgaim_systray_maximize(gtkblist->window);
#endif
				gaim_blist_set_visible(TRUE);
			}
		} else {
			/* we're logging in or something... do nothing */
			/* or should I make the blist? */
			gaim_debug(GAIM_DEBUG_WARNING, "blist",
					   "docklet_toggle called with gaim_connections_get_all() "
					   "but no blist!\n");
		}
	} else if (mainwindow) {
		if (GTK_WIDGET_VISIBLE(mainwindow)) {
			if (GAIM_WINDOW_ICONIFIED(mainwindow)) {
				gtk_window_present(GTK_WINDOW(mainwindow));
			} else {
#if _WIN32
				wgaim_systray_minimize(mainwindow);
#endif
				gtk_widget_hide(mainwindow);
			}
		} else {
#if _WIN32
			wgaim_systray_maximize(mainwindow);
#endif
			show_login();
		}
	} else {
		show_login();
	}
}

void gaim_gtk_blist_docklet_add()
{
	docklet_count++;
}

void gaim_gtk_blist_docklet_remove()
{
	docklet_count--;
	if (!docklet_count) {
		if (gaim_connections_get_all())
			gaim_blist_set_visible(TRUE);
		else if (mainwindow)
			gtk_window_present(GTK_WINDOW(mainwindow));
		else
			show_login();
	}
}

static GaimBlistUiOps blist_ui_ops =
{
	gaim_gtk_blist_new_list,
	gaim_gtk_blist_new_node,
	gaim_gtk_blist_show,
	gaim_gtk_blist_update,
	gaim_gtk_blist_remove,
	gaim_gtk_blist_destroy,
	gaim_gtk_blist_set_visible,
	gaim_gtk_blist_request_add_buddy,
	gaim_gtk_blist_request_add_chat,
	gaim_gtk_blist_request_add_group
};


GaimBlistUiOps *
gaim_gtk_blist_get_ui_ops(void)
{
	return &blist_ui_ops;
}

static void account_signon_cb(GaimConnection *gc, gpointer z)
{
	GaimAccount *account = gaim_connection_get_account(gc);
	GaimBlistNode *gnode, *cnode;
	for(gnode = gaim_get_blist()->root; gnode; gnode = gnode->next)
	{
		if(!GAIM_BLIST_NODE_IS_GROUP(gnode))
			continue;
		for(cnode = gnode->child; cnode; cnode = cnode->next)
		{
			GaimChat *chat;

			if(!GAIM_BLIST_NODE_IS_CHAT(cnode))
				continue;

			chat = (GaimChat *)cnode;

			if(chat->account != account)
				continue;

			if(gaim_blist_node_get_bool((GaimBlistNode*)chat, "gtk-autojoin") ||
					(gaim_blist_node_get_string((GaimBlistNode*)chat,
					 "gtk-autojoin") != NULL))
				serv_join_chat(gc, chat->components);
		}
	}
}

void *
gaim_gtk_blist_get_handle() {
	static int handle;

	return &handle;
}

void gaim_gtk_blist_init(void)
{
	void *gtk_blist_handle = gaim_gtk_blist_get_handle();

	gaim_signal_connect(gaim_connections_get_handle(), "signed-on",
						gtk_blist_handle, GAIM_CALLBACK(account_signon_cb),
						NULL);

	/* Initialize prefs */
	gaim_prefs_add_none("/gaim/gtk/blist");
	gaim_prefs_add_bool("/gaim/gtk/blist/auto_expand_contacts", TRUE);
	gaim_prefs_add_int("/gaim/gtk/blist/button_style", GAIM_BUTTON_TEXT_IMAGE);
	gaim_prefs_add_bool("/gaim/gtk/blist/grey_idle_buddies", TRUE);
	gaim_prefs_add_bool("/gaim/gtk/blist/raise_on_events", FALSE);
	gaim_prefs_add_bool("/gaim/gtk/blist/show_buddy_icons", TRUE);
	gaim_prefs_add_bool("/gaim/gtk/blist/show_empty_groups", FALSE);
	gaim_prefs_add_bool("/gaim/gtk/blist/show_idle_time", TRUE);
	gaim_prefs_add_bool("/gaim/gtk/blist/show_offline_buddies", FALSE);
	gaim_prefs_add_bool("/gaim/gtk/blist/show_warning_level", TRUE);
	gaim_prefs_add_string("/gaim/gtk/blist/sort_type", "alphabetical");
	gaim_prefs_add_int("/gaim/gtk/blist/x", 0);
	gaim_prefs_add_int("/gaim/gtk/blist/y", 0);
	gaim_prefs_add_int("/gaim/gtk/blist/width", 309); /* Golden ratio, baby */
	gaim_prefs_add_int("/gaim/gtk/blist/height", 500); /* Golden ratio, baby */
	gaim_prefs_add_int("/gaim/gtk/blist/tooltip_delay", 500);

	/* Register our signals */
	gaim_signal_register(gtk_blist_handle, "gtkblist-created",
						 gaim_marshal_VOID__POINTER, NULL, 1,
						 gaim_value_new(GAIM_TYPE_SUBTYPE,
						 GAIM_SUBTYPE_BLIST));

	gaim_signal_register(gtk_blist_handle, "drawing-tooltip",
						 gaim_marshal_VOID__POINTER_POINTER, NULL, 2,
						 gaim_value_new(GAIM_TYPE_SUBTYPE, GAIM_SUBTYPE_BLIST_NODE),
						 gaim_value_new_outgoing(GAIM_TYPE_STRING));
}

void
gaim_gtk_blist_uninit(void) {
	gaim_signals_unregister_by_instance(gaim_gtk_blist_get_handle());
}

/*********************************************************************
 * Public utility functions                                          *
 *********************************************************************/

GdkPixbuf *
create_prpl_icon(GaimAccount *account)
{
	GaimPlugin *prpl;
	GaimPluginProtocolInfo *prpl_info = NULL;
	GdkPixbuf *status = NULL;
	char *filename = NULL;
	const char *protoname = NULL;
	char buf[256];

	prpl = gaim_find_prpl(gaim_account_get_protocol_id(account));

	if (prpl != NULL) {
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(prpl);

		if (prpl_info->list_icon != NULL)
			protoname = prpl_info->list_icon(account, NULL);
	}

	if (protoname == NULL)
		return NULL;

	/*
	 * Status icons will be themeable too, and then it will look up
	 * protoname from the theme
	 */
	g_snprintf(buf, sizeof(buf), "%s.png", protoname);

	filename = g_build_filename(DATADIR, "pixmaps", "gaim", "status",
								"default", buf, NULL);
	status = gdk_pixbuf_new_from_file(filename, NULL);
	g_free(filename);

	return status;
}


/*********************************************************************
 * Buddy List sorting functions                                      *
 *********************************************************************/

void gaim_gtk_blist_sort_method_reg(const char *id, const char *name, gaim_gtk_blist_sort_function func)
{
	struct gaim_gtk_blist_sort_method *method = g_new0(struct gaim_gtk_blist_sort_method, 1);
	method->id = g_strdup(id);
	method->name = g_strdup(name);
	method->func = func;
	gaim_gtk_blist_sort_methods = g_slist_append(gaim_gtk_blist_sort_methods, method);
}

void gaim_gtk_blist_sort_method_unreg(const char *id){
	GSList *l = gaim_gtk_blist_sort_methods;

	while(l) {
		struct gaim_gtk_blist_sort_method *method = l->data;
		if(!strcmp(method->id, id)) {
			gaim_gtk_blist_sort_methods = g_slist_remove(gaim_gtk_blist_sort_methods, method);
			g_free(method->id);
			g_free(method->name);
			g_free(method);
			break;
		}
	}
}

void gaim_gtk_blist_sort_method_set(const char *id){
	GSList *l = gaim_gtk_blist_sort_methods;

	if(!id)
		id = "none";

	while (l && strcmp(((struct gaim_gtk_blist_sort_method*)l->data)->id, id))
		l = l->next;

	if (l) {
		current_sort_method = l->data;
	} else if (!current_sort_method) {
		gaim_gtk_blist_sort_method_set("none");
		return;
	}
	redo_buddy_list(gaim_get_blist(), TRUE);

}

/******************************************
 ** Sort Methods
 ******************************************/

static GtkTreeIter sort_method_none(GaimBlistNode *node, GaimBuddyList *blist, GtkTreeIter parent_iter, GtkTreeIter *cur)
{
	GtkTreeIter iter;
	GaimBlistNode *sibling = node->prev;
	GtkTreeIter sibling_iter;

	if(cur)
		return *cur;

	while (sibling && !get_iter_from_node(sibling, &sibling_iter)) {
		sibling = sibling->prev;
	}

	gtk_tree_store_insert_after(gtkblist->treemodel, &iter,
			node->parent ? &parent_iter : NULL,
			sibling ? &sibling_iter : NULL);

	return iter;
}

#if GTK_CHECK_VERSION(2,2,1)

static GtkTreeIter sort_method_alphabetical(GaimBlistNode *node, GaimBuddyList *blist, GtkTreeIter groupiter, GtkTreeIter *cur)
{
	GtkTreeIter more_z, iter;
	GaimBlistNode *n;
	GValue val = {0,};

	const char *my_name;

	if(GAIM_BLIST_NODE_IS_CONTACT(node)) {
		my_name = gaim_contact_get_alias((GaimContact*)node);
	} else if(GAIM_BLIST_NODE_IS_CHAT(node)) {
		my_name = gaim_chat_get_name((GaimChat*)node);
	} else {
		return sort_method_none(node, blist, groupiter, cur);
	}


	if (!gtk_tree_model_iter_children(GTK_TREE_MODEL(gtkblist->treemodel), &more_z, &groupiter)) {
		gtk_tree_store_insert(gtkblist->treemodel, &iter, &groupiter, 0);
		return iter;
	}

	do {
		const char *this_name;
		int cmp;

		gtk_tree_model_get_value (GTK_TREE_MODEL(gtkblist->treemodel), &more_z, NODE_COLUMN, &val);
		n = g_value_get_pointer(&val);

		if(GAIM_BLIST_NODE_IS_CONTACT(n)) {
			this_name = gaim_contact_get_alias((GaimContact*)n);
		} else if(GAIM_BLIST_NODE_IS_CHAT(n)) {
			this_name = gaim_chat_get_name((GaimChat*)n);
		} else {
			this_name = NULL;
		}

		cmp = gaim_utf8_strcasecmp(my_name, this_name);

		if(this_name && (cmp < 0 || (cmp == 0 && node < n))) {
			if(cur) {
				gtk_tree_store_move_before(gtkblist->treemodel, cur, &more_z);
				return *cur;
			} else {
				gtk_tree_store_insert_before(gtkblist->treemodel, &iter,
						&groupiter, &more_z);
				return iter;
			}
		}
		g_value_unset(&val);
	} while (gtk_tree_model_iter_next (GTK_TREE_MODEL(gtkblist->treemodel), &more_z));

	if(cur) {
		gtk_tree_store_move_before(gtkblist->treemodel, cur, NULL);
		return *cur;
	} else {
		gtk_tree_store_append(gtkblist->treemodel, &iter, &groupiter);
		return iter;
	}
}

static GtkTreeIter sort_method_status(GaimBlistNode *node, GaimBuddyList *blist, GtkTreeIter groupiter, GtkTreeIter *cur)
{
	GtkTreeIter more_z, iter;
	GaimBlistNode *n;
	GValue val = {0,};

	GaimBuddy *my_buddy, *this_buddy;

	if(GAIM_BLIST_NODE_IS_CONTACT(node)) {
		my_buddy = gaim_contact_get_priority_buddy((GaimContact*)node);
	} else if(GAIM_BLIST_NODE_IS_CHAT(node)) {
		if(cur)
			return *cur;

		gtk_tree_store_append(gtkblist->treemodel, &iter, &groupiter);
		return iter;
	} else {
		return sort_method_none(node, blist, groupiter, cur);
	}


	if (!gtk_tree_model_iter_children(GTK_TREE_MODEL(gtkblist->treemodel), &more_z, &groupiter)) {
		gtk_tree_store_insert(gtkblist->treemodel, &iter, &groupiter, 0);
		return iter;
	}

	do {
		int cmp;

		gtk_tree_model_get_value (GTK_TREE_MODEL(gtkblist->treemodel), &more_z, NODE_COLUMN, &val);
		n = g_value_get_pointer(&val);

		if(GAIM_BLIST_NODE_IS_CONTACT(n)) {
			this_buddy = gaim_contact_get_priority_buddy((GaimContact*)n);
		} else {
			this_buddy = NULL;
		}

		cmp = gaim_utf8_strcasecmp(my_buddy ?
				gaim_contact_get_alias(gaim_buddy_get_contact(my_buddy))
				: NULL, this_buddy ?
				gaim_contact_get_alias(gaim_buddy_get_contact(this_buddy))
				: NULL);

		/* Hideous */
		if(!this_buddy ||
				((my_buddy->present > this_buddy->present) ||
				(my_buddy->present == this_buddy->present &&
				 (((my_buddy->uc & UC_UNAVAILABLE) < (this_buddy->uc & UC_UNAVAILABLE)) ||
				 (((my_buddy->uc & UC_UNAVAILABLE) == (this_buddy->uc & UC_UNAVAILABLE)) &&
				  (((my_buddy->idle == 0) && (this_buddy->idle != 0)) ||
				   (this_buddy->idle && (my_buddy->idle > this_buddy->idle)) ||
				   ((my_buddy->idle == this_buddy->idle) &&
					(cmp < 0 || (cmp == 0 && node < n))))))))) {
			if(cur) {
				gtk_tree_store_move_before(gtkblist->treemodel, cur, &more_z);
				return *cur;
			} else {
				gtk_tree_store_insert_before(gtkblist->treemodel, &iter,
						&groupiter, &more_z);
				return iter;
			}
		}
		g_value_unset(&val);
	} while (gtk_tree_model_iter_next (GTK_TREE_MODEL(gtkblist->treemodel), &more_z));

	if(cur) {
		gtk_tree_store_move_before(gtkblist->treemodel, cur, NULL);
		return *cur;
	} else {
		gtk_tree_store_append(gtkblist->treemodel, &iter, &groupiter);
		return iter;
	}
}

static GtkTreeIter sort_method_log(GaimBlistNode *node, GaimBuddyList *blist, GtkTreeIter groupiter, GtkTreeIter *cur)
{
	GtkTreeIter more_z, iter;
	GaimBlistNode *n = NULL, *n2;
	GValue val = {0,};

	int log_size = 0, this_log_size = 0;
	const char *buddy_name, *this_buddy_name;

	if(cur && (gtk_tree_model_iter_n_children(GTK_TREE_MODEL(gtkblist->treemodel), &groupiter) == 1))
		return *cur;

	if(GAIM_BLIST_NODE_IS_CONTACT(node)) {
		for (n = node->child; n; n = n->next)
			log_size += gaim_log_get_total_size(GAIM_LOG_IM, ((GaimBuddy*)(n))->name, ((GaimBuddy*)(n))->account);
		buddy_name = gaim_contact_get_alias((GaimContact*)node);
	} else if(GAIM_BLIST_NODE_IS_CHAT(node)) {
		/* we don't have a reliable way of getting the log filename
		 * from the chat info in the blist, yet */
		if(cur)
			return *cur;

		gtk_tree_store_append(gtkblist->treemodel, &iter, &groupiter);
		return iter;
	} else {
		return sort_method_none(node, blist, groupiter, cur);
	}


	if (!gtk_tree_model_iter_children(GTK_TREE_MODEL(gtkblist->treemodel), &more_z, &groupiter)) {
		gtk_tree_store_insert(gtkblist->treemodel, &iter, &groupiter, 0);
		return iter;
	}

	do {
		int cmp;

		gtk_tree_model_get_value (GTK_TREE_MODEL(gtkblist->treemodel), &more_z, NODE_COLUMN, &val);
		n = g_value_get_pointer(&val);
		this_log_size = 0;

		if(GAIM_BLIST_NODE_IS_CONTACT(n)) {
			for (n2 = n->child; n2; n2 = n2->next)
				this_log_size += gaim_log_get_total_size(GAIM_LOG_IM, ((GaimBuddy*)(n2))->name, ((GaimBuddy*)(n2))->account);
			this_buddy_name = gaim_contact_get_alias((GaimContact*)n);
		} else {
			this_buddy_name = NULL;
		}

		cmp = gaim_utf8_strcasecmp(buddy_name, this_buddy_name);

		if (!GAIM_BLIST_NODE_IS_CONTACT(n) || log_size > this_log_size ||
				((log_size == this_log_size) &&
				 (cmp < 0 || (cmp == 0 && node < n)))) {
			if(cur) {
				gtk_tree_store_move_before(gtkblist->treemodel, cur, &more_z);
				return *cur;
			} else {
				gtk_tree_store_insert_before(gtkblist->treemodel, &iter,
						&groupiter, &more_z);
				return iter;
			}
		}
		g_value_unset(&val);
	} while (gtk_tree_model_iter_next (GTK_TREE_MODEL(gtkblist->treemodel), &more_z));

	if(cur) {
		gtk_tree_store_move_before(gtkblist->treemodel, cur, NULL);
		return *cur;
	} else {
		gtk_tree_store_append(gtkblist->treemodel, &iter, &groupiter);
		return iter;
	}
}

#endif


static void
plugin_act(GtkObject *obk, GaimPluginAction *pam)
{
	if (pam->callback) pam->callback(pam);
}



static void
build_plugin_actions(GtkWidget *menu, GaimPlugin *plugin, gpointer context)
{
	GtkWidget *menuitem = NULL;
	GaimPluginAction *action = NULL;
	GList *l, *ll;

	for (l = ll = GAIM_PLUGIN_ACTIONS(plugin, context); l; l = l->next) {
		if (l->data) {
			action = (GaimPluginAction *) l->data;
			action->plugin = plugin;
			action->context = context;

			menuitem = gtk_menu_item_new_with_label(action->label);
			gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
			g_signal_connect(G_OBJECT(menuitem), "activate",
					G_CALLBACK(plugin_act), action);
			g_object_set_data(G_OBJECT(menuitem), "plugin_action", action);
			gtk_widget_show(menuitem);
		}
		else
			gaim_separator(menu);
	}

	g_list_free(ll);
}


void
gaim_gtk_blist_update_protocol_actions(void)
{
	GtkWidget *menuitem, *submenu;
	GList *l;
	GaimConnection *gc = NULL;
	GaimPlugin *plugin = NULL;
	int count = 0;

	if (protomenu == NULL)
		return;

	/* Clear the old Account Actions menu */
	for (l = gtk_container_get_children(GTK_CONTAINER(protomenu)); l; l = l->next) {
		GaimPluginAction *action;
		
		menuitem = l->data;
		action = (GaimPluginAction *) g_object_get_data(G_OBJECT(menuitem),
				"plugin_action");
		g_free(action);

		gtk_container_remove(GTK_CONTAINER(protomenu), GTK_WIDGET(menuitem));
	}

	/* Count the number of accounts with actions */
	for (l = gaim_connections_get_all(); l; l = l->next) {
		gc = l->data;
		plugin = gc->prpl;

		if (GAIM_CONNECTION_IS_CONNECTED(gc) && GAIM_PLUGIN_HAS_ACTIONS(plugin))
			count++;

		/* no need to count past 2, so don't */
		if (count > 1)
			break;
	}

	if (count == 0) {
		menuitem = gtk_menu_item_new_with_label(_("No actions available"));
		gtk_menu_shell_append(GTK_MENU_SHELL(protomenu), menuitem);
		gtk_widget_set_sensitive(menuitem, FALSE);
		gtk_widget_show(menuitem);
	}

	else if (count == 1) {
		/* Find the one account that has actions */
		for (l = gaim_connections_get_all(); l; l = l->next) {
			gc = l->data;
			plugin = gc->prpl;

			if (GAIM_CONNECTION_IS_CONNECTED(gc) && GAIM_PLUGIN_HAS_ACTIONS(plugin))
				break;
		}

		build_plugin_actions(protomenu, plugin, gc);
	}

	else {
		for (l = gaim_connections_get_all(); l; l = l->next) {
			GaimAccount *account;
			GdkPixbuf *pixbuf, *scale;
			GtkWidget *image;
			char *buf;

			gc = l->data;
			plugin = gc->prpl;

			if (!GAIM_CONNECTION_IS_CONNECTED(gc) || !GAIM_PLUGIN_HAS_ACTIONS(plugin))
				continue;

			account = gaim_connection_get_account(gc);
			buf = g_strconcat(gaim_account_get_username(account), " (",
					plugin->info->name, ")", NULL);
			menuitem = gtk_image_menu_item_new_with_label(buf);
			g_free(buf);

			pixbuf = create_prpl_icon(account);
			if (pixbuf) {
				scale = gdk_pixbuf_scale_simple(pixbuf, 16, 16,
						GDK_INTERP_BILINEAR);
				image = gtk_image_new_from_pixbuf(scale);
				g_object_unref(G_OBJECT(pixbuf));
				g_object_unref(G_OBJECT(scale));
				gtk_widget_show(image);
				gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menuitem), image);
			}

			gtk_menu_shell_append(GTK_MENU_SHELL(protomenu), menuitem);
			gtk_widget_show(menuitem);

			submenu = gtk_menu_new();
			gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), submenu);
			gtk_widget_show(submenu);

			build_plugin_actions(submenu, plugin, gc);
		}
	}
}


void
gaim_gtk_blist_update_plugin_actions(void)
{
	GtkWidget *menuitem, *submenu;
	GaimPlugin *plugin = NULL;
	GList *l;
	int count = 0;

	if (pluginmenu == NULL)
		return;

	/* Clear the old Account Actions menu */
	for (l = gtk_container_get_children(GTK_CONTAINER(pluginmenu)); l; l = l->next) {
		GaimPluginAction *action;

		menuitem = l->data;
		action = g_object_get_data(G_OBJECT(menuitem), "plugin_action");
		g_free(action);

		gtk_container_remove(GTK_CONTAINER(pluginmenu), GTK_WIDGET(menuitem));
	}

	/* Count the number of plugins with actions */
	for (l = gaim_plugins_get_loaded(); l; l = l->next) {
		plugin = (GaimPlugin *) l->data;

		if (!GAIM_IS_PROTOCOL_PLUGIN(plugin) && GAIM_PLUGIN_HAS_ACTIONS(plugin))
			count++;

		/* no need to count past 2, so don't */
		if (count > 1)
			break;
	}

	if (count == 0) {
		menuitem = gtk_menu_item_new_with_label(_("No actions available"));
		gtk_menu_shell_append(GTK_MENU_SHELL(pluginmenu), menuitem);
		gtk_widget_set_sensitive(menuitem, FALSE);
		gtk_widget_show(menuitem);
	}

	else if (count == 1) {
		/* Find the one plugin that has actions */
		for (l = gaim_plugins_get_loaded(); l; l = l->next) {
			plugin = (GaimPlugin *) l->data;

			if (!GAIM_IS_PROTOCOL_PLUGIN(plugin) && GAIM_PLUGIN_HAS_ACTIONS(plugin))
				break;
		}

		build_plugin_actions(pluginmenu, plugin, NULL);
	}

	else {
		for (l = gaim_plugins_get_loaded(); l; l = l->next) {
			plugin = (GaimPlugin *) l->data;

			if (GAIM_IS_PROTOCOL_PLUGIN(plugin))
				continue;

			if (!GAIM_PLUGIN_HAS_ACTIONS(plugin))
				continue;

			menuitem = gtk_image_menu_item_new_with_label(plugin->info->name);
			gtk_menu_shell_append(GTK_MENU_SHELL(pluginmenu), menuitem);
			gtk_widget_show(menuitem);

			submenu = gtk_menu_new();
			gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem), submenu);
			gtk_widget_show(submenu);

			build_plugin_actions(submenu, plugin, NULL);
		}
	}
}

