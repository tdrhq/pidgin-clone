/**
 * @file gtkblist.h GTK+ Buddy List API
 * @ingroup gtkui
 *
 * gaim
 *
 * Copyright (C) 2002-2003, Sean Egan <sean.egan@binghamton.edu>
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

#ifndef _GAIM_GTK_LIST_H_
#define _GAIM_GTK_LIST_H_

#include "blist.h"

enum {
	STATUS_ICON_COLUMN,
	STATUS_ICON_VISIBLE_COLUMN,
	NAME_COLUMN,
	WARNING_COLUMN,
	IDLE_COLUMN,
	BUDDY_ICON_COLUMN,
	NODE_COLUMN,
	BLIST_COLUMNS
	};

typedef enum {
	GAIM_STATUS_ICON_LARGE,
	GAIM_STATUS_ICON_SMALL
} GaimStatusIconSize;
/**************************************************************************
 * @name Structures
 **************************************************************************/
/**
 * Like, everything you need to know about the gtk buddy list
 */
struct gaim_gtk_buddy_list {
	GtkWidget *window;
	GtkWidget *vbox;                /**< This is the vbox that everything gets packed into.  Your plugin might
					   want to pack something in it itself.  Go, plugins! */

	GtkWidget *treeview;            /**< It's a treeview... d'uh. */
	GtkTreeStore *treemodel;        /**< This is the treemodel.  */
	GtkTreeViewColumn *idle_column,
		*warning_column,
		*buddy_icon_column;

	GtkItemFactory *ift;
	GtkWidget *bpmenu;              /**< The buddy pounce menu. */

	GtkWidget *bbox;                /**< A Button Box. */
	GtkTooltips *tooltips;           /**< Tooltips for the buttons. */

	guint refresh_timer;            /**< The timer for refreshing every 30 seconds */

	guint      timeout;              /**< The timeout for the tooltip. */
	GdkRectangle rect;               /**< This is the bounding rectangle of the
					       cell we're currently hovering over.  This is
					       used for tooltips. */
	GtkWidget *tipwindow;            /**< The window used by the tooltip */

	GaimBlistNode *selected_node;   /**< The currently selected node */
};

#define GAIM_GTK_BLIST(list) ((struct gaim_gtk_buddy_list *)(list)->ui_data)
#define GAIM_IS_GTK_BLIST(list) \
	((list)->ui_ops == gaim_get_gtk_blist_ui_ops())

/**************************************************************************
 * @name GTK+ Buddy List API
 **************************************************************************/

/**
 * Initializes the GTK+ blist system.
 */
void gaim_gtk_blist_init(void);

/**
 * Returns the UI operations structure for the buddy list.
 *
 * @return The GTK list operations structure.
 */
struct gaim_blist_ui_ops *gaim_get_gtk_blist_ui_ops(void);

/**
 * Returns the base image to represent the account, based on the currently selected theme
 *
 * @param account  The account.
 *
 * @return         The icon
 */
GdkPixbuf *create_prpl_icon(GaimAccount *account);

/**
 * Refreshes all the nodes of the buddy list.
 * This should only be called when something changes to affect most of the nodes (such as a ui preference changing)
 *
 * @param list   This is the core list that gets updated from
 */
void gaim_gtk_blist_refresh(struct gaim_buddy_list *list);

/**
 * Tells the buddy list to update its toolbar based on the preferences
 *
 */
void gaim_gtk_blist_update_toolbar();

/**
 * Useful for the docklet plugin and also for the win32 tray icon
 * This is called when one of those is clicked--it will show/hide the
 * buddy list/login window--depending on which is active
 */
void gaim_gtk_blist_docklet_toggle();
void gaim_gtk_blist_docklet_add();
void gaim_gtk_blist_docklet_remove();
void gaim_gtk_blist_update_columns();
void gaim_gtk_blist_update_refresh_timeout();

/**
 * Useful for the buddy ticker
 */
GdkPixbuf *gaim_gtk_blist_get_status_icon(GaimBlistNode *node,
		GaimStatusIconSize size);

/**************************************************************************
 * @name GTK+ Buddy List sorting functions
 **************************************************************************/

typedef GtkTreeIter (*gaim_gtk_blist_sort_function)(GaimBlistNode *new, struct gaim_buddy_list *blist, GtkTreeIter group, GtkTreeIter *cur);

extern GSList *gaim_gtk_blist_sort_methods;

struct gaim_gtk_blist_sort_method {
	char *id;
	char *name;
	gaim_gtk_blist_sort_function func;
};

/**
 * Registers a buddy list sorting method.
 *
 * @param id   The unique ID of the sorting method
 * @param name The method's name.
 * @param func  A pointer to the function.
 *
 */
void gaim_gtk_blist_sort_method_reg(const char *id, const char *name, gaim_gtk_blist_sort_function func);

/**
 * Unregisters a buddy list sorting method.
 *
 * @param id The method's id
 */
void gaim_gtk_blist_sort_method_unreg(const char *id);

/**
 * Sets a buddy list sorting method.
 *
 * @param id The method's id.
 */
void gaim_gtk_blist_sort_method_set(const char *id);

/**
 * Sets up the programs default sort methods
 */
void gaim_gtk_blist_setup_sort_methods();

/**
 * Updates the protocol actions menu on the GTK+ buddy list window.
 */
void gaim_gtk_blist_update_protocol_actions();


#endif /* _GAIM_GTK_LIST_H_ */
