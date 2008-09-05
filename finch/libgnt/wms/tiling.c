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
 *   resize: r
 *   move: m
 *   TODO:
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
	DIRECTION_LEFT = 1,
	DIRECTION_RIGHT,
	DIRECTION_DOWN,
	DIRECTION_UP
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
twm_hide_window(GntWM *wm, GntWidget *win)
{
	gnt_ws_widget_hide(win, wm->nodes);
	g_object_set_data(G_OBJECT(win), "tiling-visible", GINT_TO_POINTER(0));
}

static void
twm_move_window_to_frame(GntWidget *win, TilingFrame *frame)
{
	gnt_screen_resize_widget(win, frame->width, frame->height);
	gnt_screen_move_widget(win, frame->x, frame->y);
}

static void
twm_show_window_in_frame(GntWM *wm, GntWidget *win, TilingFrame *frame)
{
	TilingWM *twm = (TilingWM*)wm;
	twm_move_window_to_frame(win, frame);
	gnt_ws_widget_show(twm->current->window, wm->nodes);
	g_object_set_data(G_OBJECT(win), "tiling-visible", GINT_TO_POINTER(1));
}

static void
twm_propagate_x_width_change(GntWM *wm, TilingFrame *frame, int d_x, int d_width)
{
	int left_width, right_width;
	frame->x += d_x;
	frame->width += d_width;
	switch (frame->type) {
		case FRAME_SPLIT_NONE:
			if (frame->window) {
				twm_move_window_to_frame(frame->window, frame);
			}
			break;
		case FRAME_SPLIT_V:
			twm_propagate_x_width_change(wm, frame->left_top, d_x, d_width);
			twm_propagate_x_width_change(wm, frame->right_bottom, d_x, d_width);
			break;
		case FRAME_SPLIT_H:
			if (d_width == 1 || d_width == -1) {
				/* resizing is done 1 at a time, so alternate between giving/taking from left and right */
				if ((frame->left_top->width & 0x1) ^ (frame->right_bottom->width & 0x1)) {
					left_width = d_width;
					right_width = 0;
				} else {
					left_width = 0;
					right_width = d_width;
				}
			} else {
				left_width = d_width / 2;
				right_width = d_width - left_width;
			}
			twm_propagate_x_width_change(wm, frame->left_top, d_x, left_width);
			twm_propagate_x_width_change(wm, frame->right_bottom, d_x + left_width, right_width);
			break;
	}
}

static void
twm_propagate_y_height_change(GntWM *wm, TilingFrame *frame, int d_y, int d_height)
{
	int top_height, bottom_height;
	frame->y += d_y;
	frame->height += d_height;
	switch (frame->type) {
		case FRAME_SPLIT_NONE:
			frame->y += d_y;
			frame->height += d_height;
			if (frame->window) {
				twm_move_window_to_frame(frame->window, frame);
			}
			break;
		case FRAME_SPLIT_V:
			if (d_height == 1 || d_height == -1) {
				/* resizing is done 1 at a time, so alternate between giving/taking from top and bottom */
				if ((frame->left_top->height & 0x1) ^ (frame->right_bottom->height & 0x1)) {
					top_height = d_height;
					bottom_height = 0;
				} else {
					top_height = 0;
					bottom_height = d_height;
				}
			} else {
				top_height = d_height / 2;
				bottom_height = d_height - top_height;
			}
			twm_propagate_y_height_change(wm, frame->left_top, d_y, top_height);
			twm_propagate_y_height_change(wm, frame->right_bottom, d_y + top_height, bottom_height);
			break;
		case FRAME_SPLIT_H:
			twm_propagate_y_height_change(wm, frame->left_top, d_y, d_height);
			twm_propagate_y_height_change(wm, frame->right_bottom, d_y, d_height);
			break;
	}
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
			twm_hide_window(wm, twm->current->window);
		}
		twm->current->window = win;
		twm_show_window_in_frame(wm, win, twm->current);
	}
	org_new_window(wm, win);
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

static void
tiling_wm_terminal_refresh(GntWM *wm)
{
	TilingWM *twm = (TilingWM*)wm;
	int xmax = getmaxx(stdscr), ymax = getmaxy(stdscr) - 1;
	if (twm->root.width != xmax) {
		twm_propagate_x_width_change(wm, &twm->root, 0, xmax - twm->root.width);
		if (twm->current->window) {
			window_reverse(twm->current->window, TRUE, wm);
		}
	}
	if (twm->root.height != ymax) {
		twm_propagate_y_height_change(wm, &twm->root, 0, ymax - twm->root.height);
		if (twm->current->window) {
			window_reverse(twm->current->window, TRUE, wm);
		}
	}
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

	if (wid && wid != twm->current->window) {

		/* hide previous window */
		if (twm->current->window) {
			twm_hide_window(wm, twm->current->window);
		}

		/* show new window */
		twm->current->window = wid;
		twm_show_window_in_frame(wm, wid, twm->current);
		window_reverse(wid, TRUE, wm);
		gnt_wm_raise_window(wm, wid);
	}

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
		twm_show_window_in_frame(wm, rgt_bot->window, rgt_bot);
	}

	twm_move_window_to_frame(twm->current->window, twm->current);
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
		if (sibling->type != FRAME_SPLIT_NONE) {
			/* if the sibling has children, copy the sibling's properties to the parent */
			if (parent->type == FRAME_SPLIT_V) {
				if (current == parent->left_top) {
					twm_propagate_y_height_change(wm, sibling, -current->height, current->height);
				} else {
					twm_propagate_y_height_change(wm, sibling, 0, current->height);
				}
			} else {
				if (current == parent->left_top) {
					twm_propagate_x_width_change(wm, sibling, -current->width, current->width);
				} else {
					twm_propagate_x_width_change(wm, sibling, 0, current->width);
				}
			}
			if (current->window) {
				twm_hide_window(wm, current->window);
			}
			parent->type = sibling->type;
			/* parent adopts sibling's children */
			parent->left_top = sibling->left_top;
			parent->right_bottom = sibling->right_bottom;
			/* children, meet your new parent */
			sibling->left_top->parent = parent;
			sibling->right_bottom->parent = parent;
			twm->current = depth_search(sibling);
		} else {
			/* sibling is leaf node, so copy current frame's properties to the parent */
			if (sibling->window) {
				twm_hide_window(wm, sibling->window);
			}
			parent->type = FRAME_SPLIT_NONE;
			parent->left_top = NULL;
			parent->right_bottom = NULL;
			parent->window = twm->current->window;
			twm->current = parent;
		}

		if (twm->current->window) {
			twm_move_window_to_frame(twm->current->window, twm->current);
			window_reverse(twm->current->window, TRUE, wm);
		}
		g_free(current);
		g_free(sibling);
	}

	return TRUE;
}


static void
free_tiling_frames(GntWM *wm, TilingFrame *frame)
{
	if (frame) {
		if (frame->window) {
			twm_hide_window(wm, frame->window);
		}
		free_tiling_frames(wm, frame->left_top);
		free_tiling_frames(wm, frame->right_bottom);
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

		free_tiling_frames(wm, twm->root.left_top);
		free_tiling_frames(wm, twm->root.right_bottom);

		if (twm->root.window) {
			twm_move_window_to_frame(twm->current->window, twm->current);
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
find_child_of_type(TilingFrame *frame, int type)
{
	while (frame && frame->parent) {
		if (frame->parent->type == type) {
			return frame;
		}
		frame = frame->parent;
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

static void
twm_exchange_left_up(GntWM *wm, int type)
{
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *left;
	GntWidget *tmp_win;

	left = find_parent_with_left(twm->current, type);
	if (left) {
		left = find_rightmost_child(left);
		/* exchange the windows */
		tmp_win = left->window;
		left->window = twm->current->window;
		twm->current->window = tmp_win;
		if (left->window) {
			twm_move_window_to_frame(left->window, left);
			window_reverse(left->window, TRUE, wm);
		}
		if (twm->current->window) {
			twm_move_window_to_frame(twm->current->window, twm->current);
		}
		twm->current = left;
	}
}

static gboolean
twm_exchange_left_up_key(GntBindable *bindable, GList *list)
{
	GntWM *wm = GNT_WM(bindable);
	int type = GPOINTER_TO_INT(list->data);

	twm_exchange_left_up(wm, type);

	return TRUE;
}

static void
twm_exchange_right_down(GntWM *wm, int type)
{
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *right;
	GntWidget *tmp_win;

	right = find_parent_with_right(twm->current, type);
	if (right) {
		right = find_leftmost_child(right);
		/* exchange the windows */
		tmp_win = right->window;
		right->window = twm->current->window;
		twm->current->window = tmp_win;
		if (right->window) {
			twm_move_window_to_frame(right->window, right);
			window_reverse(right->window, TRUE, wm);
		}
		if (twm->current->window) {
			twm_move_window_to_frame(twm->current->window, twm->current);
		}
		twm->current = right;
	}
}

static gboolean
twm_exchange_right_down_key(GntBindable *bindable, GList *list)
{
	GntWM *wm = GNT_WM(bindable);
	int type = GPOINTER_TO_INT(list->data);

	twm_exchange_right_down(wm, type);

	return TRUE;
}

static gboolean
twm_resize_move_start(GntBindable *bindable, GList *list)
{
	GntWM *wm = GNT_WM(bindable);
	int mode = GPOINTER_TO_INT(list->data);

	if (wm->_list.window || wm->menu)
		return TRUE;
	if (!wm->cws->ordered)
		return TRUE;

	wm->mode = mode;

	return TRUE;
}

static gboolean
twm_resize_move(GntBindable *bindable, GList *list)
{
	GntWM *wm = GNT_WM(bindable);
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *frame;
	int direction = GPOINTER_TO_INT(list->data);

	/* resize mode */
	if (wm->mode == GNT_KP_MODE_RESIZE) {
		/* search up for the youngest child of the appropriate split type */
		switch (direction) {
			case DIRECTION_LEFT:
			case DIRECTION_RIGHT:
				frame = find_child_of_type(twm->current, FRAME_SPLIT_H);
				break;
			case DIRECTION_UP:
			case DIRECTION_DOWN:
				frame = find_child_of_type(twm->current, FRAME_SPLIT_V);
				break;
		}

		if (frame) {
			int xmin = 0, ymin = 0, xmax = getmaxx(stdscr), ymax = getmaxy(stdscr) - 1;
			switch (direction) {
				/* left decreases the width of current frame */
				case DIRECTION_LEFT:
					if (frame->width > xmin) {
						if (frame->x < frame->sibling->x) {
							twm_propagate_x_width_change(wm, frame, 0, -1);
							twm_propagate_x_width_change(wm, frame->sibling, -1, 1);
						} else {
							twm_propagate_x_width_change(wm, frame, 1, -1);
							twm_propagate_x_width_change(wm, frame->sibling, 0, 1);
						}
					}
					break;
				/* right increases the width of current frame */
				case DIRECTION_RIGHT:
					if (frame->width < xmax) {
						if (frame->x < frame->sibling->x) {
							twm_propagate_x_width_change(wm, frame, 0, 1);
							twm_propagate_x_width_change(wm, frame->sibling, 1, -1);
						} else {
							twm_propagate_x_width_change(wm, frame, -1, 1);
							twm_propagate_x_width_change(wm, frame->sibling, 0, -1);
						}
					}
					break;
				/* up decreases the height of current frame */
				case DIRECTION_UP:
					if (frame->height > ymin) {
						if (frame->y < frame->sibling->y) {
							twm_propagate_y_height_change(wm, frame, 0, -1);
							twm_propagate_y_height_change(wm, frame->sibling, -1, 1);
						} else {
							twm_propagate_y_height_change(wm, frame, 1, -1);
							twm_propagate_y_height_change(wm, frame->sibling, 0, 1);
						}
					}
					break;
				/* down increases the height of current frame */
				case DIRECTION_DOWN:
					if (frame->height < ymax) {
						if (frame->y < frame->sibling->y) {
							twm_propagate_y_height_change(wm, frame, 0, 1);
							twm_propagate_y_height_change(wm, frame->sibling, 1, -1);
						} else {
							twm_propagate_y_height_change(wm, frame, -1, 1);
							twm_propagate_y_height_change(wm, frame->sibling, 0, -1);
						}
					}
					break;
			}
			if (twm->current->window) {
				window_reverse(twm->current->window, TRUE, wm);
			}
		}
		return TRUE;
	} else if (wm->mode == GNT_KP_MODE_MOVE) {
		/* move mode */
		switch (direction) {
			case DIRECTION_LEFT:
				twm_exchange_left_up(wm, FRAME_SPLIT_H);
				break;
			case DIRECTION_RIGHT:
				twm_exchange_right_down(wm, FRAME_SPLIT_H);
				break;
			case DIRECTION_UP:
				twm_exchange_left_up(wm, FRAME_SPLIT_V);
				break;
			case DIRECTION_DOWN:
				twm_exchange_right_down(wm, FRAME_SPLIT_V);
				break;
		}
		if (twm->current->window) {
			window_reverse(twm->current->window, TRUE, wm);
		}
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
	pclass->terminal_refresh = tiling_wm_terminal_refresh;

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
			twm_exchange_left_up_key, "\033" "H", GINT_TO_POINTER(FRAME_SPLIT_H), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "exchange-right",
			twm_exchange_right_down_key, "\033" "L", GINT_TO_POINTER(FRAME_SPLIT_H), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "exchange-down",
			twm_exchange_right_down_key, "\033" "J", GINT_TO_POINTER(FRAME_SPLIT_V), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "exchange-up",
			twm_exchange_left_up_key, "\033" "K", GINT_TO_POINTER(FRAME_SPLIT_V), NULL);

	/* resizing and moving */
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "resize-start",
			twm_resize_move_start, "\033" "r", GINT_TO_POINTER(GNT_KP_MODE_RESIZE), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "move-start",
			twm_resize_move_start, "\033" "m", GINT_TO_POINTER(GNT_KP_MODE_MOVE), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "resize-left",
			twm_resize_move, GNT_KEY_LEFT, GINT_TO_POINTER(DIRECTION_LEFT), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "resize-right",
			twm_resize_move, GNT_KEY_RIGHT, GINT_TO_POINTER(DIRECTION_RIGHT), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "resize-down",
			twm_resize_move, GNT_KEY_DOWN, GINT_TO_POINTER(DIRECTION_DOWN), NULL);
	gnt_bindable_class_register_action(GNT_BINDABLE_CLASS(klass), "resize-up",
			twm_resize_move, GNT_KEY_UP, GINT_TO_POINTER(DIRECTION_UP), NULL);

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
	twm->root.window = NULL;
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


