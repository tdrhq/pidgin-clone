#include <string.h>

#include "gntbutton.h"

enum
{
	SIGS = 1,
};

static GntWidgetClass *parent_class = NULL;
static guint signals[SIGS] = { 0 };

static void
gnt_button_draw(GntWidget *widget)
{
	GntButton *button = GNT_BUTTON(widget);
	GntColorType type;

	if (GNT_WIDGET_FLAGS(widget) & GNT_WIDGET_HAS_FOCUS)
		type = GNT_COLOR_HIGHLIGHT;
	else
		type = GNT_COLOR_NORMAL;
	wbkgdset(widget->window, '\0' | COLOR_PAIR(type));
	mvwprintw(widget->window, 1, 1, button->priv->text);

	DEBUG;
}

static void
gnt_button_size_request(GntWidget *widget)
{
	GntButton *button = GNT_BUTTON(widget);
	widget->priv.width = g_utf8_strlen(button->priv->text, -1) + 2;
	widget->priv.height = 3;
}

static void
gnt_button_map(GntWidget *widget)
{
	if (widget->priv.width == 0 || widget->priv.height == 0)
		gnt_widget_size_request(widget);
	DEBUG;
}

static gboolean
gnt_button_key_pressed(GntWidget *widget, const char *key)
{
	if (strcmp(key, GNT_KEY_ENTER) == 0)
	{
		gnt_widget_activate(widget);
		return TRUE;
	}
	return FALSE;
}

static void
gnt_button_class_init(GntWidgetClass *klass)
{
	parent_class = GNT_WIDGET_CLASS(klass);
	parent_class->draw = gnt_button_draw;
	parent_class->map = gnt_button_map;
	parent_class->size_request = gnt_button_size_request;
	parent_class->key_pressed = gnt_button_key_pressed;

	DEBUG;
}

static void
gnt_button_init(GTypeInstance *instance, gpointer class)
{
	GntButton *button = GNT_BUTTON(instance);
	button->priv = g_new0(GntButtonPriv, 1);
	DEBUG;
}

/******************************************************************************
 * GntButton API
 *****************************************************************************/
GType
gnt_button_get_gtype(void) {
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(GntButtonClass),
			NULL,					/* base_init		*/
			NULL,					/* base_finalize	*/
			(GClassInitFunc)gnt_button_class_init,
			NULL,					/* class_finalize	*/
			NULL,					/* class_data		*/
			sizeof(GntButton),
			0,						/* n_preallocs		*/
			gnt_button_init,			/* instance_init	*/
		};

		type = g_type_register_static(GNT_TYPE_WIDGET,
									  "GntButton",
									  &info, 0);
	}

	return type;
}

GntWidget *gnt_button_new(const char *text)
{
	GntWidget *widget = g_object_new(GNT_TYPE_BUTTON, NULL);
	GntButton *button = GNT_BUTTON(widget);

	button->priv->text = g_strdup(text);
	gnt_widget_set_take_focus(widget, TRUE);

	return widget;
}

