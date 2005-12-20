/**
 * @file gtkblist.h GTK+ Buddy List API
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
#ifndef _GAIM_GTKBLIST_H_
#define _GAIM_GTKBLIST_H_

typedef struct _GaimGtkBuddyList GaimGtkBuddyList;

enum {
	STATUS_ICON_COLUMN,
	STATUS_ICON_VISIBLE_COLUMN,
	NAME_COLUMN,
	IDLE_COLUMN,
	BUDDY_ICON_COLUMN,
	NODE_COLUMN,
	BLIST_COLUMNS

};

typedef enum {
	GAIM_STATUS_ICON_LARGE,
	GAIM_STATUS_ICON_SMALL

} GaimStatusIconSize;

#include "gtkgaim.h"
#include "blist.h"

/**************************************************************************
 * @name Structures
 **************************************************************************/
/**
 * Like, everything you need to know about the gtk buddy list
 */
struct _GaimGtkBuddyList {
	GtkWidget *window;
	GtkWidget *vbox;                /**< This is the vbox that everything gets packed into.  Your plugin might
					   want to pack something in it itself.  Go, plugins! */

	GtkWidget *treeview;            /**< It's a treeview... d'uh. */
	GtkTreeStore *treemodel;        /**< This is the treemodel.  */
	GtkTreeViewColumn *idle_column,
		*warning_column,
		*buddy_icon_column,
		*text_column;

	GtkCellRenderer *text_rend;

	GtkItemFactory *ift;
	GtkWidget *menutray;            /**< The menu tray widget. */
	GtkWidget *menutrayicon;        /**< The menu tray icon. */

	guint refresh_timer;            /**< The timer for refreshing every 30 seconds */

	guint      timeout;              /**< The timeout for the tooltip. */
	guint      drag_timeout;         /**< The timeout for expanding contacts on drags */
	GdkRectangle tip_rect;           /**< This is the bounding rectangle of the
					      cell we're currently hovering over.  This is
					      used for tooltips. */
	GdkRectangle contact_rect;       /**< This is the bounding rectangle of the contact node
					      and its children.  This is used for auto-expand on
					      mouseover. */
	GaimBlistNode *mouseover_contact; /**< This is the contact currently mouse-over expanded */

	GtkWidget *tipwindow;            /**< The window used by the tooltip */
	GList *tooltipdata;              /**< The data for each "chunk" of the tooltip */

	GaimBlistNode *selected_node;   /**< The currently selected node */
        GtkWidget *statusbox;            /**< The status selector dropdown */
	GdkPixbuf *east, *south;                 /**< Drop shadow stuff */
	GdkWindow *east_shadow, *south_shadow;   /**< Drop shadow stuff */

};

#define GAIM_GTK_BLIST(list) ((GaimGtkBuddyList *)(list)->ui_data)
#define GAIM_IS_GTK_BLIST(list) \
	((list)->ui_ops == gaim_gtk_blist_get_ui_ops())

/**************************************************************************
 * @name GTK+ Buddy List API
 **************************************************************************/

/**
 * Get the handle for the GTK+ blist system.
 *
 * @return the handle to the blist system
 */
void *gaim_gtk_blist_get_handle(void);

/**
 * Initializes the GTK+ blist system.
 */
void gaim_gtk_blist_init(void);

/**
 * Uninitializes the GTK+ blist system.
 */
void gaim_gtk_blist_uninit(void);

/**
 * Returns the UI operations structure for the buddy list.
 *
 * @return The GTK+ list operations structure.
 */
GaimBlistUiOps *gaim_gtk_blist_get_ui_ops(void);

/**
 * Returns the default gtk buddy list
 *
 * There's normally only one buddy list window, but that isn't a necessity. This function
 * returns the GaimGtkBuddyList we're most likely wanting to work with. This is slightly
 * cleaner than an externed global.
 *
 * @return The default GTK+ buddy list
 */
GaimGtkBuddyList *gaim_gtk_blist_get_default_gtk_blist(void);

/**
 * Populates a menu with the items shown on the buddy list for a buddy.
 *
 * @param menu  The menu to populate
 * @param buddy The buddy who's menu to get
 * @param sub   TRUE if this is a sub-menu, FALSE otherwise
 */
void gaim_gtk_blist_make_buddy_menu(GtkWidget *menu, GaimBuddy *buddy, gboolean sub);

/**
 * Refreshes all the nodes of the buddy list.
 * This should only be called when something changes to affect most of the nodes (such as a ui preference changing)
 *
 * @param list   This is the core list that gets updated from
 */
void gaim_gtk_blist_refresh(GaimBuddyList *list);

/**
 * Tells the buddy list to update its toolbar based on the preferences
 *
 */
void gaim_gtk_blist_update_toolbar(void);

void gaim_gtk_blist_update_columns(void);
void gaim_gtk_blist_update_refresh_timeout(void);

/**
 * Useful for the buddy ticker
 */
GdkPixbuf *gaim_gtk_blist_get_status_icon(GaimBlistNode *node,
		GaimStatusIconSize size);

/**
 * Returns a boolean indicating if @a node is part of an expanded contact.
 *
 * This only makes sense for contact and buddy nodes. @c FALSE is returned
 * for other types of nodes.
 *
 * @param node The node in question.
 * @return A boolean indicating if @a node is part of an expanded contact.
 */
gboolean gaim_gtk_blist_node_is_contact_expanded(GaimBlistNode *node);

/**
 * Intelligently toggles the visibility of the buddy list. If the buddy
 * list is obscured, it is brought to the front. If it is not obscured,
 * it is hidden. If it is hidden it is shown.
 */
void gaim_gtk_blist_toggle_visibility(void);

/**
 * Increases the reference count of visibility managers. Callers should 
 * call the complementary remove function when no longer managing 
 * visibility. 
 *
 * A visibility manager is something that provides some method for
 * showing the buddy list after it is hidden (e.g. docklet plugin).
 */
void gaim_gtk_blist_visibility_manager_add(void);

/**
 * Decreases the reference count of visibility managers. If the count
 * drops below zero, the buddy list is shown.
 */
void gaim_gtk_blist_visibility_manager_remove(void);


/**************************************************************************
 * @name GTK+ Buddy List sorting functions
 **************************************************************************/

typedef void (*gaim_gtk_blist_sort_function)(GaimBlistNode *new, GaimBuddyList *blist, GtkTreeIter group, GtkTreeIter *cur, GtkTreeIter *iter);

/**
 * Gets the current list of sort methods.
 *
 * @return A GSlist of sort methods
 */
GList *gaim_gtk_blist_get_sort_methods(void);

struct gaim_gtk_blist_sort_method {
	char *id;
	char *name;
	gaim_gtk_blist_sort_function func;
};

typedef struct gaim_gtk_blist_sort_method GaimGtkBlistSortMethod;

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
void gaim_gtk_blist_setup_sort_methods(void);

/**
 * Updates the accounts menu on the GTK+ buddy list window.
 */
void gaim_gtk_blist_update_accounts_menu(void);

/**
 * Updates the plugin actions menu on the GTK+ buddy list window.
 */
void gaim_gtk_blist_update_plugin_actions(void);

/**
 * Updates the Sorting menu on the GTK+ buddy list window.
 */
void gaim_gtk_blist_update_sort_methods(void);

/**
 * Determines if showing the join chat dialog is a valid action.
 *
 * @return Returns TRUE if there are accounts online capable of
 *         joining chat rooms.  Otherwise returns FALSE.
 */
gboolean gaim_gtk_blist_joinchat_is_showable(void);

/**
 * Shows the join chat dialog.
 */
void gaim_gtk_blist_joinchat_show(void);

/**
 * Appends the protocol specific menu items for a GaimBlistNode
 */
void gaim_gtk_append_blist_node_proto_menu (GtkWidget *menu, GaimConnection *gc, GaimBlistNode *node);

/**
 * Appends the extended menu items for a GaimBlistNode
 */
void gaim_gtk_append_blist_node_extended_menu(GtkWidget *menu, GaimBlistNode *node);

#endif /* _GAIM_GTKBLIST_H_ */
