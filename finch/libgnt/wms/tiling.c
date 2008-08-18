/**
 * GNT - The GLib Ncurses Toolkit
 *
 * GNT is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
 *
 * This library is free software; you can redistribute it and/or modify
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

/**
 * Tiling Window Manger
 * This window manager takes some of the ideas from the default gnt window manager,
 * as well as X tiling window managers wmii and ratpoison
 * Specifically, there will be a common mod key, used in conjunction with command
 * keys to specify actions.
 * Default Mod key: Mod1 (left-alt)
 * Command keys:
 *   split screen V/H: s/S
 *   unsplit all: Q
 *   destroy split: D
 *   cycle frame to next window: n
 *   cycle frame to previous window: p
 *   move between splits: h,j,k,l
 *   exchange: H,J,K,L
 *   TODO:
 *   resize: r
 *   get list of non-visible windows: v
 *   get list of all windows: w
 */
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "gnt.h"
#include "gntbox.h"
#include "gntmenu.h"
#include "gntstyle.h"
#include "gntwm.h"
#include "gntwindow.h"
#include "gntlabel.h"

#define TYPE_TILING_WM				(tiling_wm_get_gtype())

typedef struct _TilingFrame
{
	enum {
		FRAME_SPLIT_NONE,
		FRAME_SPLIT_V,
		FRAME_SPLIT_H
	} type;
	int width;
	int height;
	int x;
	int y;
	struct _TilingFrame *left_top;
	struct _TilingFrame *right_bottom;
	struct _TilingFrame *parent;
	struct _TilingFrame *sibling;
	GntWidget *window;
} TilingFrame;

typedef struct _TilingWM
{
	GntWM inherit;
	TilingFrame root;
	TilingFrame *current;
} TilingWM;

typedef struct _TilingWMClass
{
	GntWMClass inherit;
} TilingWMClass;

enum {
	RESIZE_LEFT = 1,
	RESIZE_RIGHT,
	RESIZE_DOWN,
	RESIZE_UP
};

GType tiling_wm_get_gtype(void);
void gntwm_init(GntWM **wm);

static void (*org_new_window)(GntWM *wm, GntWidget *win);


/* Returns the onscreen width of the character at the position
 * taken from gntwm.c */
static int
reverse_char(WINDOW *d, int y, int x, gboolean set)
{
	chtype ch;
	ch = mvwinch(d, y, x);
	mvwaddch(d, y, x, (set ? ((ch) | A_REVERSE) : ((ch) & ~A_REVERSE)));
	return 1;
}

static void
window_reverse(GntWidget *win, gboolean set, GntWM *wm)
{
	int i;
	int w, h;
	WINDOW *d;

	if (GNT_WIDGET_IS_FLAG_SET(win, GNT_WIDGET_NO_BORDER))
		return;
	
	d = win->window;
	gnt_widget_get_size(win, &w, &h);

	if (gnt_widget_has_shadow(win)) {
		--w;
		--h;
	}

	/* the top and bottom */
	for (i = 0; i < w; i += reverse_char(d, 0, i, set));
	for (i = 0; i < w; i += reverse_char(d, h-1, i, set));

	/* the left and right */
	for (i = 0; i < h; i += reverse_char(d, i, 0, set));
	for (i = 0; i < h; i += reverse_char(d, i, w-1, set));

	gnt_wm_copy_win(win, g_hash_table_lookup(wm->nodes, win));
}

static void
tiling_wm_new_window(GntWM *wm, GntWidget *win)
{
	TilingWM *twm = (TilingWM*)wm;
	int w, h, x, y;

	if (!GNT_IS_MENU(win) && !GNT_WIDGET_IS_FLAG_SET(win, GNT_WIDGET_TRANSIENT)) {
		w = twm->current->width;
		h = twm->current->height;
		x = twm->current->x;
		y = twm->current->y;
		gnt_widget_set_position(win, x, y);
		gnt_widget_set_size(win, w, h);
		mvwin(win->window, x, y);
		if (twm->current->window) {
			gnt_ws_widget_hide(twm->current->window, wm->nodes);
		}
		g_object_set_data(G_OBJECT(twm->current->window), "tiling-visible", GINT_TO_POINTER(0));
		twm->current->window = win;
		g_object_set_data(G_OBJECT(win), "tiling-visible", GINT_TO_POINTER(1));
		org_new_window(wm, win);
	}
}

static gboolean
tiling_wm_window_move_confirm(GntWM *wm, GntWidget *win, int *x, int *y)
{
	return TRUE;
}

static gboolean
tiling_wm_window_resize_confirm(GntWM *wm, GntWidget *win, int *w, int *h)
{
	return TRUE;
}

static gboolean
tiling_wm_close_window(GntWM *wm, GntWidget *win)
{
	return FALSE;
}

static GntWidget *
get_next_window(GntWM *wm, GntWidget *win, int direction)
{
	GntWidget *w = NULL, *wid = NULL;
	w = wm->cws->ordered->data;
	int pos = g_list_index(wm->cws->list, win);
	int visible = 1;

	/* loop until non-visble window is found, or the entire list has been iterated through */
	while (visible && wid != win) {
		pos += direction;
		if (pos < 0) {
			wid = g_list_last(wm->cws->list)->data;
			pos = g_list_length(wm->cws->list) - 1;
		} else if (pos >= g_list_length(wm->cws->list)) {
			wid = wm->cws->list->data;
			pos = 0;
		} else
			wid = g_list_nth_data(wm->cws->list, pos);
		visible = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(wid), "tiling-visible"));
	}

	return wid;
}

static gboolean
twm_next_window(GntBindable *bindable, GList *list)
{
	GntWM *wm = GNT_WM(bindable);
	TilingWM *twm = (TilingWM*)wm;
	GntWidget *w = NULL, *wid = NULL;
	int direction = GPOINTER_TO_INT(list->data);

	if (wm->_list.window || wm->menu)
		return TRUE;

	if (!wm->cws->ordered || !wm->cws->ordered->next)
		return TRUE;

	w = wm->cws->ordered->data;

	wid = get_next_window(wm, w, direction);

	/* hide previous window */
	if (twm->current->window) {
		g_object_set_data(G_OBJECT(twm->current->window), "tiling-visible", GINT_TO_POINTER(0));
		gnt_ws_widget_hide(twm->current->window, wm->nodes);
		window_reverse(twm->current->window, FALSE, wm);
	}
	twm->current->window = wid;
	gnt_screen_resize_widget(wid, twm->current->width, twm->current->height);
	gnt_screen_move_widget(wid, twm->current->x, twm->current->y);
	window_reverse(wid, TRUE, wm);
	/* show new window */
	g_object_set_data(G_OBJECT(twm->current->window), "tiling-visible", GINT_TO_POINTER(1));
	gnt_ws_widget_show(twm->current->window, wm->nodes);
	gnt_wm_raise_window(wm, wid);

	return TRUE;
}

static gboolean
twm_split(GntBindable *bindable, GList *list)
{
	GntWM *wm = GNT_WM(bindable);
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *lft_top, *rgt_bot;
	int type = GPOINTER_TO_INT(list->data);

	lft_top = g_new0(TilingFrame, 1);
	rgt_bot = g_new0(TilingFrame, 1);

	twm->current->type = type;
	twm->current->left_top = lft_top;
	twm->current->right_bottom = rgt_bot;

	if (type == FRAME_SPLIT_V) {
		lft_top->width = rgt_bot->width = twm->current->width;
		lft_top->height = twm->current->height / 2;
		rgt_bot->height = twm->current->height - lft_top->height;
		lft_top->x = rgt_bot->x = twm->current->x;
		lft_top->y = twm->current->y;
		rgt_bot->y = lft_top->y + lft_top->height;
	} else {
		lft_top->width = twm->current->width / 2;
		rgt_bot->width = twm->current->width - lft_top->width;
		lft_top->height = rgt_bot->height = twm->current->height;
		lft_top->x = twm->current->x;
		rgt_bot->x = lft_top->x + lft_top->width;
		lft_top->y = rgt_bot->y = twm->current->y;
	}

	lft_top->parent = rgt_bot->parent = twm->current;
	lft_top->left_top = rgt_bot->left_top = NULL;
	lft_top->right_bottom = rgt_bot->right_bottom = NULL;
	lft_top->sibling = rgt_bot;
	rgt_bot->sibling = lft_top;
	lft_top->type = rgt_bot->type = FRAME_SPLIT_NONE;

	lft_top->window = twm->current->window;
	rgt_bot->window = get_next_window(wm, lft_top->window, 1);
	twm->current->window = NULL;

	/* set current frame to the top or left */
	twm->current = lft_top;

	/* show other window */
	if (rgt_bot->window) {
		gnt_screen_resize_widget(rgt_bot->window, rgt_bot->width, rgt_bot->height);
		gnt_screen_move_widget(rgt_bot->window, rgt_bot->x, rgt_bot->y);
		g_object_set_data(G_OBJECT(rgt_bot->window), "tiling-visible", GINT_TO_POINTER(1));
		gnt_ws_widget_show(rgt_bot->window, wm->nodes);
		window_reverse(rgt_bot->window, FALSE, wm);
	}

	gnt_screen_resize_widget(twm->current->window, twm->current->width, twm->current->height);
	gnt_screen_move_widget(twm->current->window, twm->current->x, twm->current->y);
	window_reverse(twm->current->window, TRUE, wm);

	return TRUE;
}

static TilingFrame *
depth_search(TilingFrame *start)
{
	if (start->left_top) {
		return depth_search(start->left_top);
	} else {
		return start;
	}
}

static gboolean
remove_split(GntBindable *bindable, GList *null)
{
	GntWM *wm = GNT_WM(bindable);
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *parent, *current, *sibling;

	current = twm->current;
	/* root doesn't have a sibling */
	if (current->sibling) {
		parent = current->parent;
		sibling = current->sibling;
		if (sibling->left_top) {
			/* if the sibling has children, copy the sibling's properties to the parent */
			if (parent->type == FRAME_SPLIT_V) {
				if (sibling->type == FRAME_SPLIT_V) {
					sibling->left_top->height = parent->height / 2;
					sibling->right_bottom->height = parent->height - sibling->left_top->height;
				} else {
					sibling->left_top->height = parent->height;
					sibling->right_bottom->height = parent->height;
				}
			} else {
				if (sibling->type == FRAME_SPLIT_V) {
					sibling->left_top->width = parent->width;
					sibling->right_bottom->width = parent->width;
				} else {
					sibling->left_top->width = parent->width / 2;
					sibling->right_bottom->width = parent->width - sibling->left_top->width;
				}
			}
			if (twm->current->window) {
				g_object_set_data(G_OBJECT(current->window), "tiling-visible", GINT_TO_POINTER(0));
				window_reverse(current->window, FALSE, wm);
				gnt_ws_widget_hide(current->window, wm->nodes);
			}
			parent->type = sibling->type;
			parent->left_top = sibling->left_top;
			parent->right_bottom = sibling->right_bottom;
			twm->current = depth_search(sibling);
		} else {
			if (sibling->window) {
				g_object_set_data(G_OBJECT(sibling->window), "tiling-visible", GINT_TO_POINTER(0));
				window_reverse(sibling->window, FALSE, wm);
				gnt_ws_widget_hide(sibling->window, wm->nodes);
			}
			parent->type = FRAME_SPLIT_NONE;
			parent->left_top = NULL;
			parent->right_bottom = NULL;
			parent->window = twm->current->window;
			twm->current = parent;
		}

		gnt_screen_resize_widget(twm->current->window, twm->current->width, twm->current->height);
		gnt_screen_move_widget(twm->current->window, twm->current->x, twm->current->y);
		window_reverse(twm->current->window, TRUE, wm);
		g_free(current);
		g_free(sibling);
	}

	return TRUE;
}


static void
free_tiling_frames(TilingFrame *frame)
{
	if (frame) {
		free_tiling_frames(frame->left_top);
		free_tiling_frames(frame->right_bottom);
		g_free(frame);
	}
}


static gboolean
remove_all_split(GntBindable *bindable, GList *null)
{
	GntWM *wm = GNT_WM(bindable);
	TilingWM *twm = (TilingWM*)wm;

	/* set the root window to the current window and remove all the children */
	if (twm->current != &twm->root) {
		twm->root.window = twm->current->window;
		twm->current = &twm->root;

		free_tiling_frames(twm->root.left_top);
		free_tiling_frames(twm->root.right_bottom);

		if (twm->root.window) {
			gnt_screen_resize_widget(twm->current->window, twm->current->width, twm->current->height);
			gnt_screen_move_widget(twm->current->window, twm->current->x, twm->current->y);
			window_reverse(twm->current->window, TRUE, wm);
		}
	}

	return TRUE;
}

static TilingFrame *
find_parent_with_left(TilingFrame *frame, int type)
{
	TilingFrame *find = frame;
	while (find->parent) {
		if (find->parent->type == type && find->parent->left_top != find) {
			return find->parent->left_top;
		}
		find = find->parent;
	}

	return NULL;
}

static TilingFrame *
find_parent_with_right(TilingFrame *frame, int type)
{
	TilingFrame *find = frame;
	while (find->parent) {
		if (find->parent->type == type && find->parent->right_bottom != find) {
			return find->parent->right_bottom;
		}
		find = find->parent;
	}

	return NULL;
}

static TilingFrame *
find_rightmost_child(TilingFrame *frame)
{
	TilingFrame *child = frame;
	while (child->right_bottom) {
		child = child->right_bottom;
	}

	return child;
}

static TilingFrame *
find_leftmost_child(TilingFrame *frame)
{
	TilingFrame *child = frame;
	while (child->left_top) {
		child = child->left_top;
	}

	return child;
}

static gboolean
twm_move_left_up(GntBindable *bindable, GList *list)
{
	GntWM *wm = GNT_WM(bindable);
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *left;
	int type = GPOINTER_TO_INT(list->data);

	left = find_parent_with_left(twm->current, type);
	if (left) {
		window_reverse(twm->current->window, FALSE, wm);
		left = find_rightmost_child(left);
		twm->current = left;
		if (twm->current->window) {
			window_reverse(twm->current->window, TRUE, wm);
			gnt_wm_raise_window(wm, twm->current->window);
		}
	}

	return TRUE;
}


static gboolean
twm_move_right_down(GntBindable *bindable, GList *list)
{
	GntWM *wm = GNT_WM(bindable);
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *right;
	int type = GPOINTER_TO_INT(list->data);

	right = find_parent_with_right(twm->current, type);
	if (right) {
		window_reverse(twm->current->window, FALSE, wm);
		right = find_leftmost_child(right);
		twm->current = right;
		if (twm->current->window) {
			window_reverse(twm->current->window, TRUE, wm);
			gnt_wm_raise_window(wm, twm->current->window);
		}
	}

	return TRUE;
}

static gboolean
twm_exchange_left_up(GntBindable *bindable, GList *list)
{
	GntWM *wm = GNT_WM(bindable);
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *left;
	GntWidget *tmp_win;
	int type = GPOINTER_TO_INT(list->data);

	left = find_parent_with_left(twm->current, type);
	if (left) {
		left = find_rightmost_child(left);
		/* exchange the windows */
		tmp_win = left->window;
		left->window = twm->current->window;
		twm->current->window = tmp_win;
		if (left->window) {
			gnt_screen_resize_widget(left->window, left->width, left->height);
			gnt_screen_move_widget(left->window, left->x, left->y);
			window_reverse(left->window, TRUE, wm);
		}
		if (twm->current->window) {
			gnt_screen_resize_widget(twm->current->window, twm->current->width, twm->current->height);
			gnt_screen_move_widget(twm->current->window, twm->current->x, twm->current->y);
		}
		twm->current = left;
	}

	return TRUE;
}


static gboolean
twm_exchange_right_down(GntBindable *bindable, GList *list)
{
	GntWM *wm = GNT_WM(bindable);
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *right;
	GntWidget *tmp_win;
	int type = GPOINTER_TO_INT(list->data);

	right = find_parent_with_right(twm->current, type);
	if (right) {
		right = find_leftmost_child(right);
		/* exchange the windows */
		tmp_win = right->window;
		right->window = twm->current->window;
		twm->current->window = tmp_win;
		if (right->window) {
			gnt_screen_resize_widget(right->window, right->width, right->height);
			gnt_screen_move_widget(right->window, right->x, right->y);
			window_reverse(right->window, TRUE, wm);
		}
		if (twm->current->window) {
			gnt_screen_resize_widget(twm->current->window, twm->current->width, twm->current->height);
			gnt_screen_move_widget(twm->current->window, twm->current->x, twm->current->y);
		}
		twm->current = right;
	}

	return TRUE;
}

static gboolean
twm_resize_start(GntBindable *bindable, GList *null)
{
	GntWM *wm = GNT_WM(bindable);
	if (wm->_list.window || wm->menu)
		return TRUE;
	if (!wm->cws->ordered)
		return TRUE;

	wm->mode = GNT_KP_MODE_RESIZE;

	return TRUE;
}

static gboolean
twm_resize(GntBindable *bindable, GList *list)
{
	GntWM *wm = GNT_WM(bindable);
	TilingWM *twm = (TilingWM*)wm;
	int direction = GPOINTER_TO_INT(list->data);

	if (wm->mode == GNT_KP_MODE_RESIZE) {
		/* can't resize root */
		if (twm->current->sibling) {
			int xmin = 0, ymin = 0, xmax = getmaxx(stdscr), ymax = getmaxy(stdscr) - 1;
			switch (direction) {
				case RESIZE_LEFT:
					if (twm->current->width > xmin) {
						twm->current->width--;
						twm->current->sibling->width++;
						if (twm->current->x < twm->current->sibling->x) {
							twm->current->sibling->x--;
							/* TODO: propagate size change down to children */
						} else {
							twm->current->x++;
							/* TODO: propagate size change down to children */
						}
					}
					break;
				case RESIZE_RIGHT:
					if (twm->current->width < xmax) {
						twm->current->width++;
						twm->current->sibling->width--;
						if (twm->current->x < twm->current->sibling->x) {
							twm->current->sibling->x++;
							/* TODO: propagate size change down to children */
						} else {
							twm->current->x--;
							/* TODO: propagate size change down to children */
						}
					}
					break;
				case RESIZE_UP:
					if (twm->current->height > ymin) {
						twm->current->height--;
						twm->current->sibling->height++;
						if (twm->current->y < twm->current->sibling->y) {
							twm->current->sibling->y--;
							/* TODO: propagate size change down to children */
						} else {
							twm->current->y++;
							/* TODO: propagate size change down to children */
						}
					}
					break;
				case RESIZE_DOWN:
					if (twm->current->height < ymax) {
						twm->current->height++;
						twm->current->sibling->height--;
						if (twm->current->y < twm->current->sibling->y) {
							twm->current->sibling->y++;
							/* TODO: propagate size change down to children */
						} else {
							twm->current->y--;
							/* TODO: propagate size change down to children */
						}
					}
					break;
			}
			if (twm->current->window) {
				gnt_screen_resize_widget(twm->current->window, twm->current->width, twm->current->height);
				gnt_screen_move_widget(twm->current->window, twm->current->x, twm->current->y);
			}
			if (twm->current->sibling->window) {
				gnt_screen_resize_widget(twm->current->sibling->window, twm->current->sibling->width, twm->current->sibling->height);
				gnt_screen_move_widget(twm->current->sibling->window, twm->current->sibling->x, twm->current->sibling->y);
			}
		}
		return TRUE;
	}

	return FALSE;
}

static void
tiling_wm_class_init(TilingWMClass *klass)
{
	GntWMClass *pclass = GNT_WM_CLASS(klass);

	org_new_window = pclass->new_window;

	pclass->new_window = tiling_wm_new_window;
	pclass->window_resize_confirm = tiling_wm_window_resize_confirm;
	pclass->window_move_confirm = tiling_wm_window_move_confirm;
	pclass->close_window = tiling_wm_close_window;

	/* moving between windows */
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "next-window",
			twm_next_window, "\033" "n", GINT_TO_POINTER(1), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "prev-window",
			twm_next_window, "\033" "p", GINT_TO_POINTER(-1), NULL);

	/* splitting frames */
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "split-vertical",
			twm_split, "\033" "s", GINT_TO_POINTER(FRAME_SPLIT_V), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "split-horizontal",
			twm_split, "\033" "S", GINT_TO_POINTER(FRAME_SPLIT_H), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "remove-split",
			remove_split, "\033" "D", NULL, NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "remove-all-split",
			remove_all_split, "\033" "Q", NULL, NULL);

	/* moving between frames */
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "move-left",
			twm_move_left_up, "\033" "h", GINT_TO_POINTER(FRAME_SPLIT_H), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "move-right",
			twm_move_right_down, "\033" "l", GINT_TO_POINTER(FRAME_SPLIT_H), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "move-down",
			twm_move_right_down, "\033" "j", GINT_TO_POINTER(FRAME_SPLIT_V), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "move-up",
			twm_move_left_up, "\033" "k", GINT_TO_POINTER(FRAME_SPLIT_V), NULL);

	/* exchange windows between frames */
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "exchange-left",
			twm_exchange_left_up, "\033" "H", GINT_TO_POINTER(FRAME_SPLIT_H), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "exchange-right",
			twm_exchange_right_down, "\033" "L", GINT_TO_POINTER(FRAME_SPLIT_H), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "exchange-down",
			twm_exchange_right_down, "\033" "J", GINT_TO_POINTER(FRAME_SPLIT_V), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "exchange-up",
			twm_exchange_left_up, "\033" "K", GINT_TO_POINTER(FRAME_SPLIT_V), NULL);

	/* resizing */
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "resize-start",
			twm_resize_start, "\033" "r", NULL, NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "resize-left",
			twm_resize, GNT_KEY_LEFT, GINT_TO_POINTER(RESIZE_LEFT), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "resize-right",
			twm_resize, GNT_KEY_RIGHT, GINT_TO_POINTER(RESIZE_RIGHT), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "resize-down",
			twm_resize, GNT_KEY_DOWN, GINT_TO_POINTER(RESIZE_DOWN), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "resize-up",
			twm_resize, GNT_KEY_UP, GINT_TO_POINTER(RESIZE_UP), NULL);

	gnt_style_read_actions(G_OBJECT_CLASS_TYPE(klass), GNT_BINDABLE_CLASS(klass));
	GNTDEBUG;
}

void gntwm_init(GntWM **wm)
{
	TilingWM *twm;

	twm = g_object_new(TYPE_TILING_WM, NULL);
	*wm = GNT_WM(twm);

	twm->root.width = getmaxx(stdscr);
	twm->root.height = getmaxy(stdscr) - 1;
	twm->root.x = 0;
	twm->root.y = 0;
	twm->root.parent = NULL;
	twm->root.left_top = NULL;
	twm->root.right_bottom = NULL;
	twm->root.sibling = NULL;
	twm->root.type = FRAME_SPLIT_NONE;
	twm->current = &twm->root;
}

GType tiling_wm_get_gtype(void)
{
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(TilingWMClass),
			NULL,           /* base_init		*/
			NULL,           /* base_finalize	*/
			(GClassInitFunc)tiling_wm_class_init,
			NULL,
			NULL,           /* class_data		*/
			sizeof(TilingWM),
			0,              /* n_preallocs		*/
			NULL,	        /* instance_init	*/
			NULL
		};

		type = g_type_register_static(GNT_TYPE_WM,
		                              "GntTilingWM",
		                              &info, 0);
	}

	return type;
}


