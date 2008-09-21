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
 *   get list of all windows: w
 *   TODO:
 *   get list of non-visible windows: v
 *   cycle frame to next non-visible window: N
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
#define MIN_FRAME_WIDTH             2
#define MIN_FRAME_HEIGHT            2

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
	GQueue *windows;
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
static void (*org_give_focus)(GntWM *wm, GntWidget *win);

static void
twm_hide_window(GntWM *wm, GntWidget *win)
{
	gnt_ws_widget_hide(win, wm->nodes);
	g_object_set_data(G_OBJECT(win), "tiling-visible", GINT_TO_POINTER(0));
}

static void
twm_move_window_to_frame(GntWidget *win, TilingFrame *frame)
{
	/* TODO: figure out better way to handle this
	 * the reason resize is called twice is that the resize might not
	 * work correctly if the width/height increases and the current x/y
	 * restricts the width/height */
	gnt_screen_resize_widget(win, frame->width, frame->height);
	gnt_screen_move_widget(win, frame->x, frame->y);
	gnt_screen_resize_widget(win, frame->width, frame->height);
}

static void
twm_show_window_in_frame(GntWM *wm, GntWidget *win, TilingFrame *frame)
{
	TilingWM *twm = (TilingWM*)wm;
	GntWidget *cur_win;
	twm_move_window_to_frame(win, frame);
	/* TODO: why did i have this in here? */
	if ((cur_win = g_queue_peek_head(twm->current->windows))) {
		gnt_ws_widget_show(cur_win, wm->nodes);
	}
	g_object_set_data(G_OBJECT(win), "tiling-visible", GINT_TO_POINTER(1));
}

static void
twm_propagate_x_width_change(GntWM *wm, TilingFrame *frame, int d_x, int d_width)
{
	int left_width, right_width, tmp_width;
	GntWidget *win;

	frame->x += d_x;
	frame->width += d_width;
	switch (frame->type) {
		case FRAME_SPLIT_NONE:
			if ((win = g_queue_peek_head(frame->windows))) {
				twm_move_window_to_frame(win, frame);
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
			/* check to ensure if d_width is (-), that -d_width < width */
			if (-left_width >= frame->left_top->width - MIN_FRAME_WIDTH) {
				tmp_width = left_width;
				left_width = -(frame->left_top->width - MIN_FRAME_WIDTH);
				right_width += tmp_width - left_width;
			}
			/* if these both are true, we have problems */
			if (-right_width >= frame->right_bottom->width - MIN_FRAME_WIDTH) {
				tmp_width = right_width;
				right_width = -(frame->right_bottom->width - MIN_FRAME_WIDTH);
				left_width += tmp_width - left_width;
			}
			twm_propagate_x_width_change(wm, frame->left_top, d_x, left_width);
			twm_propagate_x_width_change(wm, frame->right_bottom, d_x + left_width, right_width);
			break;
	}
}

static void
twm_propagate_y_height_change(GntWM *wm, TilingFrame *frame, int d_y, int d_height)
{
	int top_height, bottom_height, tmp_height;
	GntWidget *win;

	frame->y += d_y;
	frame->height += d_height;
	switch (frame->type) {
		case FRAME_SPLIT_NONE:
			if ((win = g_queue_peek_head(frame->windows))) {
				twm_move_window_to_frame(win, frame);
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
			/* check to ensure if d_height is (-), -d_height < height */
			if (-top_height >= frame->left_top->height) {
				tmp_height = top_height;
				top_height = -(frame->left_top->height - MIN_FRAME_HEIGHT);
				bottom_height += tmp_height - top_height;
			}
			/* if these both are true, we have problems */
			if (-bottom_height >= frame->right_bottom->height) {
				tmp_height = bottom_height;
				bottom_height = -(frame->right_bottom->height - MIN_FRAME_HEIGHT);
				top_height += tmp_height - bottom_height;
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
twm_set_next_window_in_current(GntWM *wm, int direction)
{
	TilingWM *twm = (TilingWM*)wm;
	GntWidget *next_win, *cur_win;

	/* if there are two or more windows in the queue */
	if (twm->current->windows->length >= 2) {
		if (direction > 0) {
			cur_win = g_queue_pop_head(twm->current->windows);
			next_win = g_queue_peek_head(twm->current->windows);
			g_queue_push_tail(twm->current->windows, cur_win);
		} else {
			cur_win = g_queue_peek_head(twm->current->windows);
			next_win = g_queue_pop_tail(twm->current->windows);
			g_queue_push_head(twm->current->windows, next_win);
		}

		/* hide previous window */
		twm_hide_window(wm, cur_win);

		/* show new window */
		twm_show_window_in_frame(wm, next_win, twm->current);
		gnt_wm_raise_window(wm, next_win);
	}
}

static void
twm_g_queue_remove(GQueue *q, gpointer data)
{
#if GLIB_CHECK_VERSION(2,4,0)
	g_queue_remove(q, data);
#else
	int i;
	gpointer head;
	gboolean found = FALSE;

	/* pop from the head and push it back on the tail, but only if it isn't the desired data to remove */
	if (q) {
		for (i = 0; i < q->length; i++) {
			head = g_queue_pop_head(q);
			if(data == head && !found) {
				found = TRUE;
			} else {
				g_queue_push_tail(q, head);
			}
		}
	}
#endif
}

static gboolean
exists_in_g_queue(GQueue *q, gpointer data)
{
	if (q) {
		GList *i = q->head;
		while(i) {
			if(i->data == data) return TRUE;
			i = i->next;
		}
	}
	return FALSE;
}

static void
twm_front_window(GQueue *q, gpointer win)
{
	int i;
	gpointer head;
	if (q) {
		for (i = 0; i < q->length; i++) {
			head = g_queue_pop_head(q);
			if(win == head) {
				g_queue_push_head(q, head);
				break;
			} else {
				g_queue_push_tail(q, head);
			}
		}
	}
}

/* recursive function to find a frame that is currently display a given window */
static TilingFrame *
find_frame_by_window_recurse(TilingFrame *root, GntWidget *win)
{
	TilingFrame *cur = root;
	TilingFrame *left_top, *right_bot;

	if (exists_in_g_queue(root->windows, win)) return root;

	if (cur->left_top) {
		left_top = find_frame_by_window_recurse(cur->left_top, win);
		if (left_top) return left_top;
	}

	if (cur->right_bottom) {
		right_bot = find_frame_by_window_recurse(cur->right_bottom, win);
		if (right_bot) return right_bot;
	}

	return NULL;
}

static TilingFrame *
find_frame_by_window(TilingWM *twm, GntWidget *win)
{
	/* quick shortcut since a majority of resizing will be of the current */
	if (g_queue_peek_head(twm->current->windows) == win) {
		return twm->current;
	} else {
		return find_frame_by_window_recurse(&twm->root, win);
	}
}

static void
tiling_wm_new_window(GntWM *wm, GntWidget *win)
{
	TilingWM *twm = (TilingWM*)wm;
	GntWidget *cur_win;
	int w, h, x, y;

	if (!GNT_IS_MENU(win) && !GNT_WIDGET_IS_FLAG_SET(win, GNT_WIDGET_TRANSIENT)) {
		w = twm->current->width;
		h = twm->current->height;
		x = twm->current->x;
		y = twm->current->y;
		gnt_widget_set_position(win, x, y);
		gnt_widget_set_size(win, w, h);
		mvwin(win->window, x, y);
		if ((cur_win = g_queue_peek_head(twm->current->windows))) {
			twm_hide_window(wm, cur_win);
		}
		g_queue_push_tail(twm->current->windows, win);
		twm_show_window_in_frame(wm, win, twm->current);
	}
	org_new_window(wm, win);

	if(twm->current->windows->length == 1) {
		gnt_wm_raise_window(wm, win);
	}
}

static gboolean
tiling_wm_window_move_confirm(GntWM *wm, GntWidget *win, int *x, int *y)
{
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *frame;

	frame = find_frame_by_window(twm, win);

	if (!frame || (*x == frame->x && *y == frame->y)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

static gboolean
tiling_wm_window_resize_confirm(GntWM *wm, GntWidget *win, int *w, int *h)
{
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *frame;

	frame = find_frame_by_window(twm, win);

	if (!frame || (*w == frame->width && *h == frame->height)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

static gboolean
tiling_wm_close_window(GntWM *wm, GntWidget *win)
{
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *frame;
	GntWidget *wid;

	frame = find_frame_by_window(twm, win);

	if (frame) {
		if (win == g_queue_peek_head(frame->windows)) {
			g_queue_pop_head(frame->windows);

			if ((wid = g_queue_peek_head(frame->windows))) {
				/* show new window */
				twm_show_window_in_frame(wm, wid, frame);
				/* if the window being closed was in the current frame, bring the new one in focus */
				if (frame == twm->current) {
					gnt_wm_raise_window(wm, wid);
				}
			}
		} else {
			/* not displayed window in frame, so just remove it from the queue */
			twm_g_queue_remove(frame->windows, win);
		}
	}

	return FALSE;
}

static void
tiling_wm_terminal_refresh(GntWM *wm)
{
	TilingWM *twm = (TilingWM*)wm;
	int xmax = getmaxx(stdscr), ymax = getmaxy(stdscr) - 1;
	if (twm->root.width != xmax) {
		twm_propagate_x_width_change(wm, &twm->root, 0, xmax - twm->root.width);
	}
	if (twm->root.height != ymax) {
		twm_propagate_y_height_change(wm, &twm->root, 0, ymax - twm->root.height);
	}
}

static void
tiling_wm_give_focus(GntWM *wm, GntWidget *win)
{
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *frame;

	if (win != g_queue_peek_head(twm->current->windows) && !GNT_IS_MENU(win)) {
		frame = find_frame_by_window(twm, win);
		if (frame) {
			twm->current = frame;
			/* move the window to the front of the queue */
			twm_front_window(frame->windows, win);
			twm_move_window_to_frame(win, frame);
		}
	}

	org_give_focus(wm, win);
}

static gboolean
twm_next_window(GntBindable *bindable, GList *list)
{
	GntWM *wm = GNT_WM(bindable);
	int direction = GPOINTER_TO_INT(list->data);

	if (wm->_list.window || wm->menu)
		return TRUE;

	if (!wm->cws->ordered || !wm->cws->ordered->next)
		return TRUE;

	twm_set_next_window_in_current(wm, direction);

	return TRUE;
}

static gboolean
twm_split(GntBindable *bindable, GList *list)
{
	GntWM *wm = GNT_WM(bindable);
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *lft_top, *rgt_bot;
	GntWidget *win = NULL;
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

	/* Left/Top get all the windows from current */
	lft_top->windows = twm->current->windows;
	twm->current->windows = NULL;
	rgt_bot->windows = g_queue_new();

	/* Give the Right/Bottom 1 window */
	if (lft_top->windows->length >= 2) {
		win = g_queue_pop_tail(lft_top->windows);
		g_queue_push_head(rgt_bot->windows, win);
	}

	/* set current frame to the top or left */
	twm->current = lft_top;

	/* show other window */
	if (win) {
		twm_show_window_in_frame(wm, win, rgt_bot);
	}

	if ((win = g_queue_peek_head(twm->current->windows))) {
		twm_move_window_to_frame(win, twm->current);
		gnt_wm_raise_window(wm, win);
	}

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
	GntWidget *win;

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

			parent->type = sibling->type;
			/* parent adopts sibling's children */
			parent->left_top = sibling->left_top;
			parent->right_bottom = sibling->right_bottom;
			/* children, meet your new parent */
			sibling->left_top->parent = parent;
			sibling->right_bottom->parent = parent;
			twm->current = depth_search(sibling);

			/* push current windows on to the 'new current' windows queue */
			if ((win = g_queue_pop_head(current->windows))) {
				twm_hide_window(wm, win);
				while (win) {
					g_queue_push_tail(twm->current->windows, win);
					win = g_queue_pop_head(current->windows);
				}
			}
			g_queue_free(current->windows);
		} else {
			/* sibling is leaf node, so copy current frame's properties to the parent */
			parent->type = FRAME_SPLIT_NONE;
			parent->left_top = NULL;
			parent->right_bottom = NULL;
			/* copy the sibling's window list to the end of the current */
			parent->windows = twm->current->windows;
			if ((win = g_queue_pop_head(sibling->windows))) {
				twm_hide_window(wm, win);
				while (win) {
					g_queue_push_tail(parent->windows, win);
					win = g_queue_pop_head(sibling->windows);
				}
			}
			g_queue_free(sibling->windows);
			twm->current = parent;
		}

		if ((win = g_queue_peek_head(twm->current->windows))) {
			twm_move_window_to_frame(win, twm->current);
		}
		g_free(current);
		g_free(sibling);
	}

	return TRUE;
}


static void
free_tiling_frames(GntWM *wm, TilingFrame *frame)
{
	TilingWM *twm = (TilingWM*)wm;
	GntWidget *win;
	if (frame) {
		if ((win = g_queue_pop_head(frame->windows))) {
			twm_hide_window(wm, win);
			while (win) {
				g_queue_push_tail(twm->root.windows, win);
				win = g_queue_pop_head(frame->windows);
			}
		}
		g_queue_free(frame->windows);
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
	GntWidget *win;

	/* set the root window to the current window and remove all the children */
	if (twm->current != &twm->root) {
		win = g_queue_peek_head(twm->current->windows);
		twm->current = &twm->root;
		twm->current->windows = g_queue_new();

		free_tiling_frames(wm, twm->root.left_top);
		free_tiling_frames(wm, twm->root.right_bottom);

		if (win) {
			g_queue_push_head(twm->current->windows, win);
			twm_move_window_to_frame(win, twm->current);
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

static gboolean
frames_adjacent(int dim1, int delta1, int dim2, int delta2)
{
	return ((dim1 + delta1 == dim2) || (dim2 + delta2 == dim1));
}

static TilingFrame *
find_closest_vertical_child(TilingFrame *frame, TilingFrame *cur)
{
	TilingFrame *child = frame;

	while (child->type != FRAME_SPLIT_NONE) {
		if (frames_adjacent(child->left_top->x, child->left_top->width, cur->x, cur->width)) {
			if (frames_adjacent(child->right_bottom->x, child->right_bottom->width, cur->x, cur->width)) {
				/* both adjacent, pick the one closer */
				if (ABS(child->left_top->y - cur->y) < ABS(child->right_bottom->y - cur->y)) {
					child = child->left_top;
				} else {
					child = child->right_bottom;
				}
			} else {
				child = child->left_top;
			}
		} else {
			child = child->right_bottom;
		}
	}

	return child;
}

static TilingFrame *
find_closest_horizontal_child(TilingFrame *frame, TilingFrame *cur)
{
	TilingFrame *child = frame;

	while (child->type != FRAME_SPLIT_NONE) {
		if (frames_adjacent(child->left_top->y, child->left_top->height, cur->y, cur->height)) {
			if (frames_adjacent(child->right_bottom->y, child->right_bottom->height, cur->y, cur->height)) {
				/* both adjacent, pick the one closer */
				if (ABS(child->left_top->x - cur->x) < ABS(child->right_bottom->x - cur->x)) {
					child = child->left_top;
				} else {
					child = child->right_bottom;
				}
			} else {
				child = child->left_top;
			}
		} else {
			child = child->right_bottom;
		}
	}

	return child;
}

static gboolean
twm_move_left_up(GntBindable *bindable, GList *list)
{
	GntWM *wm = GNT_WM(bindable);
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *left;
	GntWidget *win;
	int type = GPOINTER_TO_INT(list->data);

	left = find_parent_with_left(twm->current, type);
	if (left) {
		if (type == FRAME_SPLIT_H) { /* left */
			left = find_closest_vertical_child(left, twm->current);
		} else { /* up */
			left = find_closest_horizontal_child(left, twm->current);
		}
		twm->current = left;
		if ((win = g_queue_peek_head(twm->current->windows))) {
			gnt_wm_raise_window(wm, win);
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
	GntWidget *win;
	int type = GPOINTER_TO_INT(list->data);

	right = find_parent_with_right(twm->current, type);
	if (right) {
		if (type == FRAME_SPLIT_H) { /* right */
			right = find_closest_vertical_child(right, twm->current);
		} else { /* down */
			right = find_closest_horizontal_child(right, twm->current);
		}
		twm->current = right;
		if ((win = g_queue_peek_head(twm->current->windows))) {
			gnt_wm_raise_window(wm, win);
		}
	}

	return TRUE;
}

static void
twm_move_win_left_up(GntWM *wm, int type)
{
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *left;
	GntWidget *next_win, *cur_win;

	if ((cur_win = g_queue_pop_head(twm->current->windows))) {
		left = find_parent_with_left(twm->current, type);
		if (left) {
			if (type == FRAME_SPLIT_H) { /* left */
				left = find_closest_vertical_child(left, twm->current);
			} else { /* up */
				left = find_closest_horizontal_child(left, twm->current);
			}

			g_queue_push_head(left->windows, cur_win);
			twm_move_window_to_frame(cur_win, left);
			
			if ((next_win = g_queue_peek_head(twm->current->windows))) {
				twm_move_window_to_frame(next_win, twm->current);
			}

			twm->current = left;
		}
	}
}

static void
twm_move_win_right_down(GntWM *wm, int type)
{
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *right;
	GntWidget *next_win, *cur_win;

	if ((cur_win = g_queue_pop_head(twm->current->windows))) {
		right = find_parent_with_right(twm->current, type);
		if (right) {
			if (type == FRAME_SPLIT_H) { /* right */
				right = find_closest_vertical_child(right, twm->current);
			} else { /* down */
				right = find_closest_horizontal_child(right, twm->current);
			}

			g_queue_push_head(right->windows, cur_win);
			twm_move_window_to_frame(cur_win, right);
			
			if ((next_win = g_queue_peek_head(twm->current->windows))) {
				twm_move_window_to_frame(next_win, twm->current);
			}

			twm->current = right;
		}
	}
}

static void
twm_exchange_left_up(GntWM *wm, int type)
{
	TilingWM *twm = (TilingWM*)wm;
	TilingFrame *left;
	GntWidget *left_win, *cur_win;

	left = find_parent_with_left(twm->current, type);
	if (left) {
		if (type == FRAME_SPLIT_H) { /* left */
			left = find_closest_vertical_child(left, twm->current);
		} else { /* up */
			left = find_closest_horizontal_child(left, twm->current);
		}
		/* exchange the windows */
		left_win = g_queue_pop_head(left->windows);
		if (left_win) {
			cur_win = g_queue_pop_head(twm->current->windows);
			if (cur_win) {
				g_queue_push_head(left->windows, cur_win);
				twm_move_window_to_frame(cur_win, left);
			}
			g_queue_push_head(twm->current->windows, left_win);
			twm_move_window_to_frame(left_win, twm->current);
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
	GntWidget *right_win, *cur_win;

	right = find_parent_with_right(twm->current, type);
	if (right) {
		if (type == FRAME_SPLIT_H) { /* right */
			right = find_closest_vertical_child(right, twm->current);
		} else { /* down */
			right = find_closest_horizontal_child(right, twm->current);
		}
		/* exchange the windows */
		right_win = g_queue_pop_head(right->windows);
		if (right_win) {
			cur_win = g_queue_pop_head(twm->current->windows);
			if(cur_win) {
				g_queue_push_head(right->windows, cur_win);
				twm_move_window_to_frame(cur_win, right);
			}
			g_queue_push_head(twm->current->windows, right_win);
			twm_move_window_to_frame(right_win, twm->current);
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
	TilingFrame *frame = NULL;
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
			int xmax, ymax;
			int xmin = MIN_FRAME_WIDTH, ymin = MIN_FRAME_HEIGHT;
			/* root does not have a parent, therefore it's max is getmax{x,y} */
			if (frame->parent) {
				xmax = frame->parent->width - MIN_FRAME_WIDTH;
				ymax = frame->parent->height - MIN_FRAME_HEIGHT;
			} else {
				xmax = getmaxx(stdscr) - MIN_FRAME_WIDTH;
				ymax = getmaxy(stdscr) - MIN_FRAME_HEIGHT - 1;
			}
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
		}
		return TRUE;
	} else if (wm->mode == GNT_KP_MODE_MOVE) {
		/* move mode */
		switch (direction) {
			case DIRECTION_LEFT:
				twm_move_win_left_up(wm, FRAME_SPLIT_H);
				break;
			case DIRECTION_RIGHT:
				twm_move_win_right_down(wm, FRAME_SPLIT_H);
				break;
			case DIRECTION_UP:
				twm_move_win_left_up(wm, FRAME_SPLIT_V);
				break;
			case DIRECTION_DOWN:
				twm_move_win_right_down(wm, FRAME_SPLIT_V);
				break;
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
	org_give_focus = pclass->give_focus;

	pclass->new_window = tiling_wm_new_window;
	pclass->window_resize_confirm = tiling_wm_window_resize_confirm;
	pclass->window_move_confirm = tiling_wm_window_move_confirm;
	pclass->close_window = tiling_wm_close_window;
	pclass->terminal_refresh = tiling_wm_terminal_refresh;
	pclass->give_focus = tiling_wm_give_focus;

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
	twm->root.windows = g_queue_new();
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


