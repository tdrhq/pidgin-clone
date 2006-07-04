#include "gntbox.h"

#include <string.h>

enum
{
	SIGS = 1,
};

static GntWidgetClass *parent_class = NULL;
static guint signals[SIGS] = { 0 };

static GntWidget * find_focusable_widget(GntBox *box);

static void
add_to_focus(gpointer value, gpointer data)
{
	GntBox *box = GNT_BOX(data);
	GntWidget *w = GNT_WIDGET(value);

	if (GNT_IS_BOX(w))
		g_list_foreach(GNT_BOX(w)->list, add_to_focus, box);
	else if (GNT_WIDGET_IS_FLAG_SET(w, GNT_WIDGET_CAN_TAKE_FOCUS))
		box->focus = g_list_append(box->focus, w);
}

static void
gnt_box_draw(GntWidget *widget)
{
	GntBox *box = GNT_BOX(widget);

	if (box->focus == NULL && widget->parent == NULL)
		g_list_foreach(box->list, add_to_focus, box);

	g_list_foreach(box->list, (GFunc)gnt_widget_draw, NULL);

	gnt_box_sync_children(box);

	if (box->title)
	{
		gchar *title = g_strdup(box->title);
		int pos = g_utf8_strlen(title, -1);

		if (pos >= widget->priv.width - 2)
		{
			g_utf8_strncpy(title, box->title, widget->priv.width - 2);
			pos = 1;
		}
		else
		{
			/* XXX: Position of the title might be configurable */
			pos = (widget->priv.width - pos) / 2;
		}

		if (gnt_widget_has_focus(widget))
			wbkgdset(widget->window, '\0' | COLOR_PAIR(GNT_COLOR_TITLE));
		else
			wbkgdset(widget->window, '\0' | COLOR_PAIR(GNT_COLOR_TITLE_D));
		mvwprintw(widget->window, 0, pos, title);
		g_free(title);
	}
	
	DEBUG;
}

static void
reposition_children(GntWidget *widget)
{
	GList *iter;
	GntBox *box = GNT_BOX(widget);
	int w, h, curx, cury, max;
	gboolean has_border = FALSE;

	w = h = 0;
	max = -1;
	curx = widget->priv.x;
	cury = widget->priv.y;
	if (!(GNT_WIDGET_FLAGS(widget) & GNT_WIDGET_NO_BORDER))
	{
		has_border = TRUE;
		curx += 1;
		cury += 1;
	}

	for (iter = box->list; iter; iter = iter->next)
	{
		gnt_widget_set_position(GNT_WIDGET(iter->data), curx, cury);
		gnt_widget_get_size(GNT_WIDGET(iter->data), &w, &h);
		if (box->vertical)
		{
			cury += h + box->pad;
			if (max < w)
				max = w;
		}
		else
		{
			curx += w + box->pad;
			if (max < h)
				max = h;
		}
	}

	if (has_border)
	{
		curx += 1;
		cury += 1;
		max += 2;
	}

	if (box->vertical)
	{
		widget->priv.width = max;
		widget->priv.height = cury - widget->priv.y;
	}
	else
	{
		widget->priv.width = curx - widget->priv.x;
		widget->priv.height = max;
	}
}

static void
gnt_box_set_position(GntWidget *widget, int x, int y)
{
	gnt_widget_size_request(widget);
	reposition_children(widget);
}

static void
gnt_box_size_request(GntWidget *widget)
{
	GntBox *box = GNT_BOX(widget);
	GList *iter;
	
	g_list_foreach(box->list, (GFunc)gnt_widget_size_request, NULL);

	if (box->homogeneous)
	{
		int max = -1, w, h;

		/* XXX: should probably be changed based on vertical-ness */
		for (iter = box->list; iter; iter = iter->next)
		{
			gnt_widget_get_size(GNT_WIDGET(iter->data), &w, NULL);
			if (max < w)
				max = w;
		}

		for (iter = box->list; iter; iter = iter->next)
		{
			gnt_widget_get_size(GNT_WIDGET(iter->data), NULL, &h);
			gnt_widget_set_size(GNT_WIDGET(iter->data), max, h);
		}
	}

	reposition_children(widget);
}

static void
gnt_box_map(GntWidget *widget)
{
	if (widget->priv.width == 0 || widget->priv.height == 0)
	{
		gnt_widget_size_request(widget);
		find_focusable_widget(GNT_BOX(widget));
	}
	DEBUG;
}

/* Ensures that the current widget can take focus */
static GntWidget *
find_focusable_widget(GntBox *box)
{
	if (box->focus == NULL && GNT_WIDGET(box)->parent == NULL)
		g_list_foreach(box->list, add_to_focus, box);

	if (box->active == NULL && box->focus)
		box->active = box->focus->data;

	return box->active;
}

static gboolean
gnt_box_key_pressed(GntWidget *widget, const char *text)
{
	GntBox *box = GNT_BOX(widget);

	if (box->active == NULL && !find_focusable_widget(box))
		return FALSE;

	if (gnt_widget_key_pressed(box->active, text))
		return TRUE;
	
	if (text[0] == 27)
	{
		GntWidget *now = box->active;
		if (strcmp(text+1, GNT_KEY_LEFT) == 0)
		{
			GList *iter = g_list_find(box->focus, box->active);
			if ((!iter || !iter->prev) && box->focus)
			{
				box->active = box->focus->data;
			}
			else
			{
				box->active = iter->prev->data;
			}
		}
		else if (strcmp(text+1, GNT_KEY_RIGHT) == 0)
		{
			GList *iter = g_list_find(box->focus, box->active);
			if (iter && iter->next)
			{
				box->active = iter->next->data;
			}
			else if (box->focus)
			{
				box->active = box->focus->data;
			}
		}

		if (now && now != box->active)
		{
			gnt_widget_set_focus(now, FALSE);
			gnt_widget_set_focus(box->active, TRUE);
			return TRUE;
		}
	}

	return FALSE;
}

static void
gnt_box_lost_focus(GntWidget *widget)
{
	GntWidget *w = GNT_BOX(widget)->active;
	if (w)
		gnt_widget_set_focus(w, FALSE);
	gnt_widget_draw(widget);
}

static void
gnt_box_gained_focus(GntWidget *widget)
{
	GntWidget *w = GNT_BOX(widget)->active;
	if (w)
		gnt_widget_set_focus(w, TRUE);
	gnt_widget_draw(widget);
}

static void
gnt_box_destroy(GntWidget *w)
{
	GntBox *box = GNT_BOX(w);
	GList *iter;

	for (iter = box->list; iter; iter = iter->next)
	{
		gnt_widget_destroy(iter->data);
	}

	gnt_screen_release(w);
	
	g_list_free(box->list);
}

static void
gnt_box_expose(GntWidget *widget, int x, int y, int width, int height)
{
	WINDOW *win = newwin(height, width, widget->priv.y + y, widget->priv.x + x);
	copywin(widget->window, win, y, x, 0, 0, height - 1, width - 1, FALSE);
	wrefresh(win);
	delwin(win);
}

static void
gnt_box_class_init(GntBoxClass *klass)
{
	parent_class = GNT_WIDGET_CLASS(klass);
	parent_class->destroy = gnt_box_destroy;
	parent_class->draw = gnt_box_draw;
	parent_class->expose = gnt_box_expose;
	parent_class->map = gnt_box_map;
	parent_class->size_request = gnt_box_size_request;
	parent_class->set_position = gnt_box_set_position;
	parent_class->key_pressed = gnt_box_key_pressed;
	parent_class->lost_focus = gnt_box_lost_focus;
	parent_class->gained_focus = gnt_box_gained_focus;

	DEBUG;
}

static void
gnt_box_init(GTypeInstance *instance, gpointer class)
{
	DEBUG;
}

/******************************************************************************
 * GntBox API
 *****************************************************************************/
GType
gnt_box_get_gtype(void)
{
	static GType type = 0;

	if(type == 0)
	{
		static const GTypeInfo info = {
			sizeof(GntBoxClass),
			NULL,					/* base_init		*/
			NULL,					/* base_finalize	*/
			(GClassInitFunc)gnt_box_class_init,
			NULL,					/* class_finalize	*/
			NULL,					/* class_data		*/
			sizeof(GntBox),
			0,						/* n_preallocs		*/
			gnt_box_init,			/* instance_init	*/
		};

		type = g_type_register_static(GNT_TYPE_WIDGET,
									  "GntBox",
									  &info, 0);
	}

	return type;
}

GntWidget *gnt_box_new(gboolean homo, gboolean vert)
{
	GntWidget *widget = g_object_new(GNT_TYPE_BOX, NULL);
	GntBox *box = GNT_BOX(widget);

	box->homogeneous = homo;
	box->vertical = vert;
	box->pad = 1;
	gnt_widget_set_take_focus(widget, TRUE);
	GNT_WIDGET_SET_FLAGS(widget, GNT_WIDGET_NO_BORDER | GNT_WIDGET_NO_SHADOW);

	return widget;
}

void gnt_box_add_widget(GntBox *b, GntWidget *widget)
{
	b->list = g_list_append(b->list, widget);
	widget->parent = GNT_WIDGET(b);
}

void gnt_box_set_title(GntBox *b, const char *title)
{
	g_free(b->title);
	b->title = g_strdup(title);
}

void gnt_box_set_pad(GntBox *box, int pad)
{
	box->pad = pad;
	/* XXX: Perhaps redraw if already showing? */
}

void gnt_box_set_toplevel(GntBox *box, gboolean set)
{
	GntWidget *widget = GNT_WIDGET(box);
	if (set)
	{
		GNT_WIDGET_UNSET_FLAGS(widget, GNT_WIDGET_NO_BORDER | GNT_WIDGET_NO_SHADOW);
		GNT_WIDGET_SET_FLAGS(widget, GNT_WIDGET_CAN_TAKE_FOCUS);
	}
	else
	{
		GNT_WIDGET_SET_FLAGS(widget, GNT_WIDGET_NO_BORDER | GNT_WIDGET_NO_SHADOW);
		GNT_WIDGET_UNSET_FLAGS(widget, GNT_WIDGET_CAN_TAKE_FOCUS);
	}
}

void gnt_box_sync_children(GntBox *box)
{
	GList *iter;
	GntWidget *widget = GNT_WIDGET(box);

	/* XXX: werase first? */

	for (iter = box->list; iter; iter = iter->next)
	{
		GntWidget *w = GNT_WIDGET(iter->data);

		if (GNT_IS_BOX(w))
			gnt_box_sync_children(GNT_BOX(w));

		copywin(w->window, widget->window, 0, 0,
				w->priv.y - widget->priv.y,
				w->priv.x - widget->priv.x,
				w->priv.y - widget->priv.y + w->priv.height - 1,
				w->priv.x - widget->priv.x + w->priv.width - 1,
				FALSE);
	}
}

