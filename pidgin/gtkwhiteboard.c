/*
 * pidgin
 *
 * Pidgin is the legal property of its developers, whose names are too numerous
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

#include <stdlib.h>
#include <gtk/gtkfontbutton.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>
#include <gtk/gtkwidget.h>
#include <gdk/gdkkeysyms.h>
#include "internal.h"
#include "blist.h"
#include "debug.h"
#include "gtkwhiteboard.h"

#define  LINE_SHAPE -2
#define  RECTANGLE_SHAPE -3
#define  ARC_SHAPE -4
#define  MULTILINE_SHAPE -5
#define  TEXT_SHAPE -7
#define  FILL_SHAPE -8
#define  VIDEO_W   -9

/******************************************************************************
 * Prototypes
 *****************************************************************************/
static void pidgin_whiteboard_create(PurpleWhiteboard *wb);

static void pidgin_whiteboard_destroy(PurpleWhiteboard *wb);
static gboolean whiteboard_close_cb(GtkWidget *widget, GdkEvent *event, PidginWhiteboard *gtkwb);

/*static void PidginWhiteboard_button_start_press(GtkButton *button, gpointer data); */

static gboolean pidgin_whiteboard_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data);
static gboolean pidgin_whiteboard_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data);

static gboolean pidgin_whiteboard_brush_down(GtkWidget *widget, GdkEventButton *event, gpointer data);
static gboolean pidgin_whiteboard_brush_motion(GtkWidget *widget, GdkEventMotion *event, gpointer data);
static gboolean pidgin_whiteboard_brush_up(GtkWidget *widget, GdkEventButton *event, gpointer data);

static void pidgin_whiteboard_draw_brush_point(PurpleWhiteboard *wb,
												  int x, int y, int color, int size);
static void pidgin_whiteboard_draw_brush_line(PurpleWhiteboard *wb, int x0, int y0,
												int x1, int y1, int color, int size);
static void pidgin_whiteboard_draw_line(PurpleWhiteboard *wb, int x0, int y0,
												int x1, int y1, int color, int size);
static void pidgin_whiteboard_draw_rectangle(PurpleWhiteboard *wb, int x0, int y0,
												int x1, int y1, int color, int size);

static void pidgin_whiteboard_draw_arc(PurpleWhiteboard *wb,gboolean filled,gint x,gint y,gint width,gint height,gint angle1,gint angle2 );

static void pidgin_whiteboard_draw_text(PurpleWhiteboard *wb, gint x,gint y,guint text);
static void pidgin_whiteboard_draw_fill(PurpleWhiteboard *wb,int x,int y );

static void pidgin_whiteboard_draw_shape(PurpleWhiteboard *wb, GList*);

static void pidgin_whiteboard_set_dimensions(PurpleWhiteboard *wb, int width, int height);
static void pidgin_whiteboard_set_brush(PurpleWhiteboard *wb, int size, int color);
static void pidgin_whiteboard_clear(PurpleWhiteboard *wb);

static void pidgin_whiteboard_button_clear_press(GtkWidget *widget, gpointer data);
static void pidgin_whiteboard_button_save_press(GtkWidget *widget, gpointer data);

static void pidgin_whiteboard_set_canvas_as_icon(PidginWhiteboard *gtkwb);

static void pidgin_whiteboard_rgb24_to_rgb48(int color_rgb, GdkColor *color);
static void pidgin_whiteboard_rgb24_to_rgb48_fill(int color_rgb, GdkColor *color);

static void color_select_dialog(GtkWidget *widget, PidginWhiteboard *gtkwb);

/******************************************************************************
 * Globals
 *****************************************************************************/

static int LastX;       /* Tracks last position of the mouse when drawing */
static int LastY;
static int MotionCount; /* Tracks how many brush motions made */
static int BrushState = BRUSH_STATE_UP;

static PurpleWhiteboardUiOps ui_ops =
{
	pidgin_whiteboard_create,
	pidgin_whiteboard_destroy,
	pidgin_whiteboard_set_dimensions,
	pidgin_whiteboard_set_brush,
	pidgin_whiteboard_draw_brush_point,
	pidgin_whiteboard_draw_brush_line,
	pidgin_whiteboard_clear,
	pidgin_whiteboard_draw_shape,
	NULL,
	NULL,
	NULL
};

/******************************************************************************
 * API
 *****************************************************************************/
PurpleWhiteboardUiOps *pidgin_whiteboard_get_ui_ops(void)
{
	return &ui_ops;
}
/**********gpaint******* anil **************************////
double brush_spacing = 6.0;
int erase_size = 4;
int brush_size = 9;
const double EPSILON = 0.00001;
double brush_distance = 0;
int brush_last_x = 0;
int brush_last_y = 0;

struct fillinfo
{
    unsigned char *rgb;
    int rowstride;
    int pixelsize;
    int width;
    int height;
    unsigned char or, og, ob, r, g, b;
};

struct fillpixelinfo
{
    int y, xl, xr, dy;
};

int image_buf_width(PidginWhiteboard* ibuf)
{
    if (ibuf->rgbbuf)
        return gdk_pixbuf_get_width(ibuf->rgbbuf);
    return 0;
}

int image_buf_height(PidginWhiteboard* ibuf)
{
    if (ibuf->rgbbuf)
        return gdk_pixbuf_get_height(ibuf->rgbbuf);
    return 0;
}
void image_buf_set_cursor(PidginWhiteboard *ibuf, GdkCursor* cursor)
{
    GdkCursor *oldcursor = ibuf->cursor;
    ibuf->cursor = cursor;
    gdk_window_set_cursor(ibuf->drawing_area->window, cursor);
    if (oldcursor)
        gdk_cursor_destroy(oldcursor);
}

typedef struct toggle_button_info
{
    const char *name;
    DRAWING_TOOL tool;
}
toggle_button_info;

const  toggle_button_info toggle_button_infos[] =
{
        {"erase_button", ERASE},
		{"fill_button", FILL},
        {"line_button", LINE},
        {"multiline_button", MULTILINE},
        {"rectangle_button", RECTANGLE},
        {"pen_button", PEN},
        {"text_button", TEXT},
		{"arc_button", ARC},
		{"oval_button", OVAL},
        {"brush_button", BRUSH},
        {0, NONE},
        {0, PASTE}
};

PidginWhiteboard *widget_get_image(GtkWidget *widget)
{
    PidginWhiteboard *ibuf = (PidginWhiteboard*) g_object_get_data(G_OBJECT(widget),gtk_widget_get_name(widget));
//        gtk_object_get_user_data(GTK_OBJECT(widget));
    GtkWidget *parent;

    if (!ibuf)
    {
        parent =  gtk_widget_get_toplevel(GTK_WIDGET(widget));
        parent = gtk_widget_get_parent(parent);
        ibuf = (PidginWhiteboard*) g_object_get_data(G_OBJECT(parent),gtk_widget_get_name(parent));
        gtk_object_set_user_data(GTK_OBJECT(widget), ibuf);
    }
    return ibuf;
}
void image_buf_pixmap_to_rgbbuf(PidginWhiteboard *ibuf, GdkRectangle *irect)
{
    GdkRectangle fullrect ;
    GdkRectangle rect;
    fullrect.x = 0;
    fullrect.y = 0;
    fullrect.width = image_buf_width(ibuf);
    fullrect.height = image_buf_height(ibuf);

    if (!ibuf->modified && irect)
        gdk_rectangle_intersect(&fullrect, irect, &rect);
    else
        rect = fullrect;
    gdk_pixbuf_get_from_drawable(ibuf->rgbbuf, ibuf->pixmap, gdk_rgb_get_cmap(), rect.x, rect.y, rect.x, rect.y, rect.width, rect.height);
    ibuf->modified = 0;
}

void image_buf_rgbbuf_to_pixmap(PidginWhiteboard *ibuf, GdkRectangle *irect)
{
    GdkRectangle fullrect ;
    GdkRectangle rect;
    fullrect.x = 0;
    fullrect.y = 0;
    fullrect.width = image_buf_width(ibuf);
    fullrect.height = image_buf_height(ibuf);
    purple_debug_error("gtkwhiteboard", "***in rgbbuf to pixmap %d \n",ibuf->current_tool );
    if (irect)
        gdk_rectangle_intersect(&fullrect, irect, &rect);
    else
        rect = fullrect;
    gdk_pixbuf_render_to_drawable(ibuf->rgbbuf, ibuf->pixmap, ibuf->gc, rect.x, rect.y, rect.x, rect.y, rect.width, rect.height, GDK_RGB_DITHER_NORMAL, 0, 0);
}

static int is_old_pixel_value(struct fillinfo *info, int x, int y)
{
    unsigned char *p = info->rgb + y * info->rowstride + x * info->pixelsize;
    unsigned char or, og, ob;
    or = *p;
    og = *(p + 1);
    ob = *(p + 2);
    if ((or == info->or) && (og == info->og) && (ob == info->ob))
        return 1;

    return 0;
}


void GdkColor_to_rgb(const GdkColor *color, unsigned char *r, unsigned char *g, unsigned char *b)
{

    GdkColormap *cmap = gdk_rgb_get_cmap();
    GdkVisual *visual = gdk_rgb_get_visual();
    GdkColor *c;
    switch(visual->type)
    {
    case GDK_VISUAL_STATIC_COLOR:
    case GDK_VISUAL_PSEUDO_COLOR:
        c = cmap->colors + color->pixel;

        *r = c->red >> 8;
        *g = c->green >> 8;
        *b = c->blue >> 8;
        break;
    case GDK_VISUAL_TRUE_COLOR:
    case GDK_VISUAL_DIRECT_COLOR:
        *r = (color->pixel & visual->red_mask) >> visual->red_shift << (sizeof(unsigned char) * 8 - visual->red_prec);
        *g = (color->pixel & visual->green_mask) >> visual->green_shift << (sizeof(unsigned char) * 8 - visual->green_prec);
        *b = (color->pixel & visual->blue_mask) >> visual->blue_shift << (sizeof(unsigned char) * 8 - visual->blue_prec);
        break;

    default:
        g_assert(0);
        break;
    }
}
static void set_new_pixel_value(struct fillinfo *info, int x, int y)
{
    unsigned char *p = info->rgb + y * info->rowstride + x * info->pixelsize;
    *p = info->r;
    *(p + 1) = info->g;
    *(p + 2) = info->b;
}
unsigned char *image_buf_rgbbuf(PidginWhiteboard* ibuf)
{
    if (ibuf->rgbbuf)
        return gdk_pixbuf_get_pixels(ibuf->rgbbuf);
    return 0;
}

int image_buf_rowstride(PidginWhiteboard *ibuf)
{
    return gdk_pixbuf_get_rowstride(ibuf->rgbbuf);
}

int image_buf_pixelsize(PidginWhiteboard *ibuf)
{
    return gdk_pixbuf_get_n_channels(ibuf->rgbbuf) * gdk_pixbuf_get_bits_per_sample(ibuf->rgbbuf) / 8;
}


GtkWidget* lookup_widget(GtkWidget  *widget, const gchar  *widget_name)
{
    GtkWidget *parent, *found_widget;

    for (;;)
    {
        if (GTK_IS_MENU (widget))
            parent = gtk_menu_get_attach_widget (GTK_MENU (widget));
        else
            parent = widget->parent;
        if (parent == NULL)
            break;
        widget = parent;
    }
    found_widget = (GtkWidget*) gtk_object_get_data (GTK_OBJECT (widget),
                   widget_name);
    if (!found_widget)
        g_warning ("Widget not found: %s", widget_name);
    else
        g_warning ("Widget found: %s", widget_name);
    return found_widget;
}
static void insert_point(PidginWhiteboard *ibuf, int x, int y)
{
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    ibuf->pts[ibuf->num_pts].x = x;
    ibuf->pts[ibuf->num_pts].y = y;
    ibuf->num_pts++;
}
static void clear_points(PidginWhiteboard *ibuf)
{
    ibuf->num_pts = 0;

    memset(ibuf->pts, 0, sizeof(ibuf->pts));
}
static void modify_point(PidginWhiteboard *ibuf, int index, int x, int y)
{
    ibuf->pts[index].x = x;
    ibuf->pts[index].y = y;


}
void draw_rectangle(PidginWhiteboard *ibuf, gint filled, gint x1, gint y1, gint x2, gint y2,int is_print)
{
    int ox, w, oy, h;
    if (x1 > x2)
    {
        ox = x2;
        w = x1 - x2;
    }
    else
    {
        ox = x1;
        w = x2 - x1;
    }

    if (y1 > y2)
    {
        oy = y2;
        h = y1 - y2;
    }
    else
    {
        oy = y1;
        h = y2 - y1;
    }
    if (filled)
        w++, h++;
   if(is_print==1){
        gdk_draw_rectangle(ibuf->drawing_area->window, ibuf->gc, filled, ox, oy, w, h);
    }
    else
        pidgin_whiteboard_draw_rectangle(ibuf->wb, ox, oy, ox+w, oy+h, ibuf->brush_color,ibuf->brush_size);
}

static void draw_oval(PidginWhiteboard *ibuf, gint filled, gint x1, gint y1, gint x2, gint y2,gint is_print)
{
    int ox, w, oy, h;
    if (x1 > x2)
    {
        ox = x2;
        w = x1 - x2;
    }
    else
    {
        ox = x1;
        w = x2 - x1;
    }

    if (y1 > y2)
    {
        oy = y2;
        h = y1 - y2;
    }
    else
    {
        oy = y1;
        h = y2 - y1;
    }
    if (filled)
        w++, h++;

    if(is_print==1){
//        gdk_draw_arc(ibuf->pixmap,ibuf->gc, filled, ox, oy, w, h, 0, 360 * 64);
        gdk_draw_arc(ibuf->drawing_area->window,ibuf->gc, filled, ox, oy, w, h, 0, 360 * 64);
    }
    else
        pidgin_whiteboard_draw_arc(ibuf->wb,filled,ox,oy,w,h,0,360 * 64 );
}

static void draw_arc(PidginWhiteboard *ibuf, gboolean filled, gint x1, gint y1, gint x2, gint y2,int is_print)
{
    int ox, w, oy, h;
    int angle1, angle2;

    if (x1 < x2)
    {
        w = (x2 - x1) * 2;

        if (y1 < y2)
        {
            h = (y2 - y1) * 2;
            ox = x2 - w;
            oy = y1;
            angle1 = 90;
            angle2 = -90;
        }
        else
        {
            ox = x1;
            h = (y1 - y2) * 2;
            oy = y2;
            angle1 = 180;
            angle2 = -90;
        }
    }
    else /* x1 > x2 */
    {
        w = (x1 - x2) * 2;

        if (y1 < y2)
        {
            h = (y2 - y1) * 2;
            ox = x1 - w;
            oy = y2 - h;
            angle1 = 0;
            angle2 = -90;
        }
        else
        {
            h = (y1 - y2) * 2;
            oy = y1 - h;
            ox = x2;
            angle1 = -90;
            angle2 = -90;
        }
    }

    if (filled)
        w++, h++;
    if(is_print == 1){
//        gdk_draw_arc(ibuf->pixmap,ibuf->gc, filled, ox, oy, w, h, angle1 * 64, angle2 * 64);
        gdk_draw_arc(ibuf->drawing_area->window,ibuf->gc, filled, ox, oy, w, h, angle1 * 64, angle2 * 64);
    }
    else
        pidgin_whiteboard_draw_arc(ibuf->wb,filled,ox,oy,w,h,angle1 * 64, angle2 * 64);
}



int image_buf_get_fill(PidginWhiteboard *ibuf)
{
    return ibuf->filled;
}

void image_buf_set_font(PidginWhiteboard *ibuf,  const char *font_name)
{
    GdkFont *oldfont = ibuf->font;
    GdkFont* font;
    PangoFontDescription *font_description = pango_font_description_from_string(font_name);

//    GtkFontButton *w = (GtkFontButton *)lookup_widget(ibuf->window, "fontpicker");
//    g_assert(w);
    font = gdk_font_from_description(font_description);
    ibuf->font = font;
//    gtk_font_button_set_show_style(w,TRUE);
//    gtk_font_button_set_show_size (w,12);
//    gtk_font_button_set_use_font (w,font_name);
    gdk_font_ref(ibuf->font);
    if (oldfont)
        gdk_font_unref(oldfont);
}

static void image_buf_draw_flash(PidginWhiteboard *ibuf)
{

    GdkRectangle clientrect ;

    DRAWING_TOOL tool = ibuf->current_tool;
    GdkGCValues gcvalues;
    clientrect.x = clientrect.y = 0;
    clientrect.width = image_buf_width(ibuf);
    clientrect.height = image_buf_height(ibuf);

    gdk_gc_get_values(ibuf->gc, &gcvalues);
    gdk_gc_set_clip_origin(ibuf->gc, 0, 0);
    gdk_gc_set_clip_rectangle(ibuf->gc, &clientrect);

    switch(tool)
    {
    case TEXT:

        if ((ibuf->llx != INT_MIN) && (ibuf->lly != INT_MIN))
        {
            gint width, ascent, descent, lbearing, rbearing, junk;
            GdkColor white;
            gdk_color_white(gdk_rgb_get_cmap(), &white);

            gdk_gc_set_foreground(ibuf->gc, &white);

            gdk_string_extents(ibuf->font, "My", &junk, &junk, &junk, &ascent, &descent);
            gdk_string_extents(ibuf->font, ibuf->textbuf, &lbearing, &rbearing, &width, &junk, &junk);

            gdk_gc_set_function(ibuf->gc, GDK_INVERT);
            gdk_gc_set_line_attributes(ibuf->gc, 2, GDK_LINE_SOLID, GDK_CAP_ROUND, GDK_JOIN_ROUND);
            gdk_draw_line(ibuf->drawing_area->window, ibuf->gc, ibuf->llx + rbearing + 2, ibuf->lly + descent, ibuf->llx + rbearing + 2, ibuf->lly - ascent);
            gdk_draw_line(ibuf->pixmap, ibuf->gc, ibuf->llx + rbearing + 2, ibuf->lly + descent, ibuf->llx + rbearing + 2, ibuf->lly - ascent);

            gdk_gc_set_function(ibuf->gc, gcvalues.function);
            gdk_gc_set_line_attributes(ibuf->gc, gcvalues.line_width, gcvalues.line_style, gcvalues.cap_style, gcvalues.join_style);
            gdk_gc_set_foreground(ibuf->gc, &(gcvalues.foreground));
            ibuf->flash_state = !(ibuf->flash_state);
        }
        break;

    default:
        break;
    }
}

void image_buf_clear_flash(PidginWhiteboard *ibuf)
{
    while (ibuf->flash_state)
        image_buf_draw_flash(ibuf);
}

void image_buf_set_tool(PidginWhiteboard *ibuf, DRAWING_TOOL tool)
{
    image_buf_clear_flash(ibuf);
    switch(ibuf->current_tool) /* final mode cleanup */
    {
    case TEXT:
        if ((ibuf->llx > INT_MIN) && (ibuf->lly > INT_MIN) && strlen(ibuf->textbuf))
        {
            gdk_draw_string(ibuf->drawing_area->window, ibuf->font, ibuf->gc, ibuf->llx, ibuf->lly, ibuf->textbuf);
            gdk_draw_string(ibuf->pixmap, ibuf->font, ibuf->gc, ibuf->llx, ibuf->lly, ibuf->textbuf);
            ibuf->modified = 1;
        }
        ibuf->llx = INT_MIN;
        ibuf->lly = INT_MIN;
        break;

    default:
        break;
    }

    ibuf->current_tool = tool;
    ibuf->flash_state = 0;
    switch(tool) /* new mode initialization */
    {
    case MULTILINE:

    case ERASE:
    case LINE:
	case ARC:
    case RECTANGLE:
    case OVAL:
        image_buf_set_cursor(ibuf, gdk_cursor_new(GDK_CROSSHAIR));
        break;
    case PEN:
        image_buf_set_cursor(ibuf, gdk_cursor_new(GDK_PENCIL));
        break;
    case TEXT:
        image_buf_set_cursor(ibuf, gdk_cursor_new(GDK_XTERM));
        strcpy(ibuf->textbuf, "");
        break;
    default:
        image_buf_set_cursor(ibuf, gdk_cursor_new(GDK_X_CURSOR));
        break;
    }
    ibuf->llx = INT_MIN;
    ibuf->lly = INT_MIN;
    ibuf->lx = INT_MIN;
    ibuf->ly = INT_MIN;
    ibuf->mx = INT_MIN;
    ibuf->my = INT_MIN;
    ibuf->lpx = INT_MIN;
    ibuf->lpy = INT_MIN;
    clear_points(ibuf);
}

void select_toolbar_toggle_button(GtkToggleButton *selected, int setmode ,PidginWhiteboard *gtkwb)
{
    const toggle_button_info *t;
    GtkWidget *b;
    GtkWidget *window ;
    int x, y;
    int inbutton = setmode;
    gtk_widget_get_pointer(GTK_WIDGET(selected), &x, &y);
    if ((x >= 0) && (y >= 0) && (x < GTK_WIDGET(selected)->allocation.width) && (y < GTK_WIDGET(selected)->allocation.height))
        inbutton = 1;

    if (!gtk_toggle_button_get_active(selected))
    {
        if (!inbutton)
            return;

        gtk_toggle_button_set_active(selected, TRUE);

    }

    g_assert(gtkwb->window);
    if (inbutton)
    {
        gtkwb->current_button = GTK_WIDGET(selected);
    }
    for (t = toggle_button_infos; (t->name); t++)
    {
        b = lookup_widget(gtkwb->window, (t->name));
        g_assert(b);
        if (b != GTK_WIDGET(selected))
        {
            if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b)))
            {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(b), FALSE);

            }
            g_assert(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b)));
        }
        else if (inbutton)
        {
            image_buf_set_tool(gtkwb, t->tool);
            purple_debug_error("gtkwhiteboard", "*** %s is selected\n",t->name );
        }
    }
}


void image_buf_clear(PidginWhiteboard *ibuf)
{
    GdkGCValues gcvalues;
    GdkRectangle clientrect ;


    clientrect.x = clientrect.y = 0;
    clientrect.width = image_buf_width(ibuf);
    clientrect.height = image_buf_height(ibuf);

    image_buf_clear_flash(ibuf);

    gdk_gc_get_values(ibuf->gc, &gcvalues);
    gdk_gc_set_clip_origin(ibuf->gc, 0, 0);
    gdk_gc_set_clip_rectangle(ibuf->gc, &clientrect);

    gdk_gc_set_foreground(ibuf->gc, &(gcvalues.background));

    draw_rectangle(ibuf, TRUE, clientrect.x, clientrect.y, clientrect.width, clientrect.height,1);
    draw_rectangle(ibuf, TRUE, clientrect.x, clientrect.y, clientrect.width, clientrect.height,1);
    image_buf_pixmap_to_rgbbuf(ibuf, 0);

    gdk_gc_set_foreground(ibuf->gc, &(gcvalues.foreground));

}

void image_buf_adjust_according_to_points(PidginWhiteboard *ibuf, int *x, int *y)
{

    *x += ibuf->ptdiffx;
    *y += ibuf->ptdiffy;
}

void set_button_pixmap(GtkButton *button, unsigned char **data, PidginWhiteboard *gtkwb )
{
//    PidginWhiteboard *gtkwb = (PidginWhiteboard*)(user_data);
    GdkPixmap *gdkpixmap;
    GdkBitmap *mask;
    GtkWidget *gtkpixmap;
//    GtkWidget*   temp =  gtk_widget_get_toplevel(GTK_WIDGET(button));
//    temp = gtk_widget_get_parent(temp);
    gdkpixmap = gdk_pixmap_create_from_xpm_d(gtkwb->window->window, &mask, NULL, (gchar**)data);
//    gdkpixmap = gtkwb->pixmap;
    g_assert(gdkpixmap);
    gtkpixmap = gtk_pixmap_new(gdkpixmap, mask);
    g_assert(gtkpixmap);
    gdk_pixmap_unref (gdkpixmap);
    gdk_pixmap_unref (mask);

    gtk_container_foreach(GTK_CONTAINER(button), (GtkCallback) gtk_container_remove, button);
    gtk_container_add(GTK_CONTAINER(button), (gtkpixmap));
    gtk_widget_show(gtkpixmap);
}


static void flood_fill(struct fillinfo *info, int x, int y)
{
    struct fillpixelinfo stack[10000];
    struct fillpixelinfo * sp = stack;

    int l, x1, x2, dy;

#define PUSH(py, pxl, pxr, pdy) \
{  struct fillpixelinfo *p = sp;\
   if (((py) + (pdy) >= 0) && ((py) + (pdy) < info->height))\
   {\
      p->y = (py);\
      p->xl = (pxl);\
      p->xr = (pxr);\
      p->dy = (pdy);\
      sp++; \
   }\
}

    
#define POP(py, pxl, pxr, pdy) \
{\
   sp--;\
   (py) = sp->y + sp->dy;\
   (pxl) = sp->xl;\
   (pxr) = sp->xr;\
   (pdy) = sp->dy;\
}

    if ((x >= 0) && (x < info->width) && (y >= 0) && (y < info->height))
    {
        if ((info->or == info->r) && (info->og == info->g) && (info->ob == info->b))
            return;

        PUSH(y, x, x, 1);
        PUSH(y + 1, x, x, -1);

        while (sp > stack)
        {
            POP(y, x1, x2, dy);
            for (x = x1; (x >= 0) && is_old_pixel_value(info, x, y); x--)
                set_new_pixel_value(info, x, y);
            if (x >= x1)
                goto skip;
            l = x + 1;
            if (l < x1)
                PUSH(y, l, x1 - 1, -dy);
            x = x1 + 1;
            do
            {
                for (; (x < info->width) && is_old_pixel_value(info, x, y); x++)
                    set_new_pixel_value(info, x, y);

                PUSH(y, l, x - 1, dy);
                if (x > x2 + 1)
                    PUSH(y, x2 + 1, x - 1, -dy);
skip:
                for (x++; x <= x2 && !is_old_pixel_value(info, x, y); x++)
                    ;

                l = x;
            }
            while (x <= x2);


        }
    }

#undef POP
#undef PUSH

}


void image_buf_flood_fill(PidginWhiteboard *ibuf, int x, int y, const GdkColor *color)
{
    unsigned char r, g, b;
    struct fillinfo fillinfo;
    unsigned char *p ;
    image_buf_pixmap_to_rgbbuf(ibuf, 0);
    //GdkColor_to_rgb(color, &r, &g, &b);

    //added ;ater
    r = color->red;
    g = color->green;
    b = color->blue;
    //added ;ater

    purple_debug_info("image_buf_flood_fill : ", "RED %d\n", r);
    purple_debug_info("image_buf_flood_fill : ", "GREEN %d\n", g);
    purple_debug_info("image_buf_flood_fill : ", "BLUE %d\n", b);


    fillinfo.rgb = image_buf_rgbbuf(ibuf);
    fillinfo.width = image_buf_width(ibuf);
    fillinfo.height = image_buf_height(ibuf);
    fillinfo.rowstride = image_buf_rowstride(ibuf);
    fillinfo.pixelsize = image_buf_pixelsize(ibuf);
    fillinfo.r = r;
    fillinfo.g = g;
    fillinfo.b = b;
    p = fillinfo.rgb + y * fillinfo.rowstride + x * fillinfo.pixelsize;
    fillinfo.or = *p;
    fillinfo.og = *(p + 1);
    fillinfo.ob = *(p + 2);
    flood_fill(&fillinfo, x, y);

    image_buf_rgbbuf_to_pixmap(ibuf, 0);
    ibuf->modified = 1;
}



void on_text_button_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    PidginWhiteboard *gtkwb = (PidginWhiteboard*)(user_data);
    select_toolbar_toggle_button(togglebutton,0, gtkwb);
}

void
on_erase_button_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    PidginWhiteboard *gtkwb = (PidginWhiteboard*)(user_data);
    select_toolbar_toggle_button(togglebutton,0, gtkwb);
}

void
on_fill_button_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    PidginWhiteboard *gtkwb = (PidginWhiteboard*)(user_data);
    select_toolbar_toggle_button(togglebutton,0, gtkwb);

}


void
on_line_button_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    PidginWhiteboard *gtkwb = (PidginWhiteboard*)(user_data);
    select_toolbar_toggle_button(togglebutton,0, gtkwb);

}


void
on_rectangle_button_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    PidginWhiteboard *gtkwb = (PidginWhiteboard*)(user_data);
    select_toolbar_toggle_button(togglebutton,0, gtkwb);

}


void
on_pen_button_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    PidginWhiteboard *gtkwb = (PidginWhiteboard*)(user_data);
    select_toolbar_toggle_button(togglebutton, 0,gtkwb);

}

void
on_arc_button_toggled                  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    PidginWhiteboard *gtkwb = (PidginWhiteboard*)(user_data);
    select_toolbar_toggle_button(togglebutton, 0,gtkwb);
}

void
on_oval_button_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    PidginWhiteboard *gtkwb = (PidginWhiteboard*)(user_data);
    select_toolbar_toggle_button(togglebutton, 0,gtkwb);
}


void
on_brush_button_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    PidginWhiteboard *gtkwb = (PidginWhiteboard*)(user_data);
    select_toolbar_toggle_button(togglebutton, 0,gtkwb);
}

void
on_filled_button_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    char *filename;
    GdkPixbuf *pixbuf;
    GtkWidget *tmp_toolbar_icon;
    PidginWhiteboard *gtkwb = (PidginWhiteboard*)(user_data);
    gtkwb->filled = (gtkwb->filled == 0)?1:0;
    purple_debug_info("gtkwhiteboard", "Filled = %d\n",gtkwb->filled);
    if (gtkwb->filled) {
        purple_debug_info("gtkwhiteboard", "INSIDE Filled\n");

        filename = g_build_filename(DATADIR, "pixmaps", "pidgin","filled.xpm", NULL);
        pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
        g_free(filename);
        tmp_toolbar_icon = gtk_image_new_from_pixbuf(pixbuf);
        //togglebutton = gtk_toggle_button_new_with_label ("");
        gtk_button_set_image(togglebutton, tmp_toolbar_icon );
    }
    else {
        purple_debug_info("gtkwhiteboard", "INSIDE NOTFilled\n");

        filename = g_build_filename(DATADIR, "pixmaps", "pidgin","unfilled.xpm", NULL);
        pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
        g_free(filename);
        tmp_toolbar_icon = gtk_image_new_from_pixbuf(pixbuf);
        //togglebutton = gtk_toggle_button_new_with_label ("");
        gtk_button_set_image(togglebutton, tmp_toolbar_icon );

    }
}

void on_fontpicker_font_set(GtkFontButton *gnomefontpicker,  gpointer  user_data)
{

    PidginWhiteboard *ibuf = (PidginWhiteboard *)user_data;
    g_assert(ibuf);
    char *font_name = gtk_font_button_get_font_name (gnomefontpicker);
    image_buf_set_font(ibuf,font_name);
    gtk_font_button_set_show_style(gnomefontpicker,TRUE);
    gtk_font_button_set_show_size (gnomefontpicker,12);
    gtk_font_button_set_use_font (gnomefontpicker,font_name);
}


void
on_multiline_button_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    PidginWhiteboard *ibuf = (PidginWhiteboard *)user_data;
    select_toolbar_toggle_button(togglebutton, 0, ibuf);
//    set_button_pixmap(GTK_BUTTON(togglebutton),0, ibuf);
}

void set_drawing_tool(PidginWhiteboard *ibuf, DRAWING_TOOL tool)
{
    const char *buttonname = 0;
    GtkWidget *selected = 0;

    if (ibuf->modified)
    {
        image_buf_pixmap_to_rgbbuf(ibuf, 0);
    }

    if (tool < NONE)
        buttonname = toggle_button_infos[tool - ERASE].name;

/*    if (buttonname)
        selected = lookup_widget(ibuf->window, buttonname);

    if (selected)
        select_toolbar_toggle_button(GTK_TOGGLE_BUTTON(selected), 1,ibuf);
    else
*/
    image_buf_set_tool(ibuf, tool);

}

/*
 * Paint along the mouse movement. 
 * adapted from paint_core.c in the GIMP.
 */
void image_buf_paint_interpolate(PidginWhiteboard *ibuf, int x, int y)
{
    int    initial_x = x;
    int    initial_y = y;

    double dx;         /* delta x */
    double dy;         /* delta y */
    double moved;      /* mouse movement */
    double initial;    /* initial paint distance */
    double final;      /* final paint distance   */

    double points;     /* number of paint points */
    double next;       /* distance to the next point */
    double percent;    /* linear interplotation, 0 to 1.0 */
    int    changed = 0;

    /* TODO: object-oriented drawing tools */
    GdkGCValues gcvalues;
    gdk_gc_get_values(ibuf->gc, &gcvalues);
    if (ibuf->current_tool==ERASE)
    {
        gdk_gc_set_foreground(ibuf->gc, &(gcvalues.background));
    }

    /* calculate mouse move distance */
    dx = (double)(x - brush_last_x);
    dy = (double)(y - brush_last_y);
    moved = sqrt(dx*dx + dy*dy);

    initial = brush_distance;
    final = initial + moved;

    /* paint along the movement */
    while (brush_distance < final)
    {

        /* calculate distance to the next point */
        points = (int) (brush_distance / brush_spacing + 1.0 + EPSILON);
        next = points * brush_spacing - brush_distance;
        brush_distance += next;

        if (brush_distance <= (final + EPSILON))
        {

            /* calculate interpolation */
            percent = (brush_distance - initial) / moved;
            x = (int)(brush_last_x + percent * dx);
            y = (int)(brush_last_y + percent * dy);

            /* TODO: object-oriented drawing tools */
            switch (ibuf->current_tool)
            {
            case ERASE:
                draw_rectangle(ibuf, TRUE, x - erase_size, y - erase_size, x + erase_size, y + erase_size,1);
                draw_rectangle(ibuf, TRUE, x - erase_size, y - erase_size, x + erase_size, y + erase_size,1);
                break;

            case BRUSH:
                draw_oval(ibuf, TRUE, x - brush_size, y - brush_size, x + brush_size, y + brush_size,0);
                break;
            default:
                break;
            }
        }
        changed = 1;
    }
    if (changed)
    {
        ibuf->modified = 1;
    }

    brush_distance = final;
    brush_last_x = initial_x;
    brush_last_y = initial_y;

    /* TODO: object-oriented drawing tools */
    if (ibuf->current_tool==ERASE)
    {
        gdk_gc_set_foreground(ibuf->gc, &(gcvalues.foreground));
    }
}

void handle_button_release(PidginWhiteboard *ibuf, int x, int y)
{
    DRAWING_TOOL tool = ibuf->current_tool;
    GdkGCValues gcvalues;
    GdkRectangle rect, clientrect ;
	GdkColor col;
    GList *draw_list = ibuf->wb->draw_list;
    int old_color,old_size;
    if ((ibuf->lx == INT_MIN) || (ibuf->ly == INT_MIN))
        return;

    rect.x = x;
    rect.y = y;
    rect.width = 10;
    rect.height = 10;
    clientrect.x = clientrect.y = 0;
    clientrect.width = image_buf_width(ibuf);
    clientrect.height = image_buf_height(ibuf);

    gdk_gc_get_values(ibuf->gc, &gcvalues);
    gdk_gc_set_clip_origin(ibuf->gc, 0, 0);
    gdk_gc_set_clip_rectangle(ibuf->gc, &clientrect);

    image_buf_clear_flash(ibuf);
    
    insert_point(ibuf, x, y);
    purple_debug_error("gtkwhiteboard", "***Button release %d \n",ibuf->current_tool );
    switch (tool)
    {
        case ERASE:
            old_color = ibuf->brush_color;
            old_size  = ibuf->brush_size;
            ibuf->brush_color = 0xFFFFFFFF;
            ibuf->brush_size  = 6 *ibuf->brush_size ;
            //        pidgin_whiteboard_set_brush(ibuf->wb,ibuf->brush_color, ibuf->brush_size);

            purple_whiteboard_send_brush(ibuf->wb, ibuf->brush_size, ibuf->brush_color);
            if(MotionCount == 0)
            {
                int index;
                for(index = 0; index < 2; index++)
                {
                    draw_list = g_list_append(draw_list, 0);
                    draw_list = g_list_append(draw_list, 0);
                }
            }
            /* Send draw list to prpl draw_list handler */
            purple_whiteboard_send_draw_list(ibuf->wb, draw_list,3);
            pidgin_whiteboard_set_canvas_as_icon(ibuf);
            if(draw_list)
                purple_whiteboard_draw_list_destroy(draw_list);
            ibuf->wb->draw_list = NULL;
            ibuf->brush_color = old_color;
            ibuf->brush_size  = old_size;
            purple_whiteboard_send_brush(ibuf->wb, ibuf->brush_size, ibuf->brush_color);
            break;
        case PEN:
            if(MotionCount == 0)
            {
                int index;
                for(index = 0; index < 2; index++)
                {
                    draw_list = g_list_append(draw_list, 0);
                    draw_list = g_list_append(draw_list, 0);
                }
            }
            /* Send draw list to prpl draw_list handler */
            purple_whiteboard_send_draw_list(ibuf->wb, draw_list,3);
            pidgin_whiteboard_set_canvas_as_icon(ibuf);
            if(draw_list)
                purple_whiteboard_draw_list_destroy(draw_list);
            ibuf->wb->draw_list = NULL;
            break;
        case MULTILINE:
        case LINE:
            pidgin_whiteboard_draw_line(ibuf->wb, ibuf->lx, ibuf->ly, x, y, ibuf->brush_color,ibuf->brush_size);
            ibuf->modified = 1;
            break;
        case OVAL:
            gdk_gc_set_function(ibuf->gc, GDK_INVERT);
            gdk_gc_set_line_attributes(ibuf->gc, ibuf->brush_size, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_MITER);
            draw_oval(ibuf, image_buf_get_fill(ibuf), ibuf->lx, ibuf->ly, x, y,0);
            ibuf->modified = 1;
            break;

        case ARC:
            draw_arc(ibuf, image_buf_get_fill(ibuf), ibuf->lx, ibuf->ly, x, y,0);
            ibuf->modified = 1;
            break;
            
        case RECTANGLE:
            draw_rectangle(ibuf, ibuf->filled, ibuf->lx, ibuf->ly, x, y, 0);
            ibuf->modified = 1;
            break;
        case TEXT:
            gtk_widget_grab_focus(ibuf->drawing_area);
            gtk_widget_grab_default(ibuf->drawing_area);
            break;
        case BRUSH:
            old_color = ibuf->brush_color;
            old_size  = ibuf->brush_size;
            ibuf->brush_size  = 6 *ibuf->brush_size ;
            //        pidgin_whiteboard_set_brush(ibuf->wb,ibuf->brush_color, ibuf->brush_size);

            purple_whiteboard_send_brush(ibuf->wb, ibuf->brush_size, ibuf->brush_color);
            if(MotionCount == 0)
            {
                int index;
                for(index = 0; index < 2; index++)
                {
                    draw_list = g_list_append(draw_list, 0);
                    draw_list = g_list_append(draw_list, 0);
                }
            }
            /* Send draw list to prpl draw_list handler */
            purple_whiteboard_send_draw_list(ibuf->wb, draw_list,3);
            pidgin_whiteboard_set_canvas_as_icon(ibuf);
            if(draw_list)
                purple_whiteboard_draw_list_destroy(draw_list);
            ibuf->wb->draw_list = NULL;
            ibuf->brush_color = old_color;
            ibuf->brush_size  = old_size;
            purple_whiteboard_send_brush(ibuf->wb, ibuf->brush_size, ibuf->brush_color);
            break;
        default:
            break;
    }
    ibuf->lx = INT_MIN;
    ibuf->ly = INT_MIN;
    ibuf->mx = INT_MIN;
    ibuf->my = INT_MIN;
    if (tool == MULTILINE)
    {
        ibuf->llx = x;
        ibuf->lly = y;
    }

    if (tool == TEXT)
    {
        ibuf->llx = x;
        ibuf->lly = y;
    }
    else
        clear_points(ibuf);
}

void handle_button_press(PidginWhiteboard *ibuf, int x, int y)
{
    DRAWING_TOOL tool = ibuf->current_tool;
    GdkGCValues gcvalues;
    GdkRectangle clientrect ;
    GdkPixmap *pixmap = ibuf->pixmap;
	PurpleWhiteboard *wb = ibuf->wb;
	GList *draw_list = wb->draw_list;
    int old_color,old_size ;
//    rect.x = x;
//    rect.y = y;
//    rect.width = 2;
//    rect.height = 2;
    clientrect.x = clientrect.y = 0;
    clientrect.width = image_buf_width(ibuf);
    clientrect.height = image_buf_height(ibuf);
    
    purple_debug_error("gtkwhiteboard", "***Button press %d \n",ibuf->current_tool );
    gdk_gc_get_values(ibuf->gc, &gcvalues);
    gdk_gc_set_clip_origin(ibuf->gc, 0, 0);
    gdk_gc_set_clip_rectangle(ibuf->gc, &clientrect);

    image_buf_clear_flash(ibuf);

    clear_points(ibuf);
    insert_point(ibuf, x, y);
    
    switch (tool)
    {
        case PEN:
            if(draw_list)
            {
                purple_whiteboard_draw_list_destroy(draw_list);
                draw_list = NULL;
            }
            /* Set tracking variables */
            LastX = x;
            LastY = y;
            MotionCount = 0;
            draw_list = g_list_append(draw_list, GINT_TO_POINTER(LastX));
            draw_list = g_list_append(draw_list, GINT_TO_POINTER(LastY));
            pidgin_whiteboard_draw_brush_point(ibuf->wb,
                x, y,
                ibuf->brush_color, ibuf->brush_size);
            
        
            wb->draw_list = draw_list;
            break;
        case MULTILINE:
            if ((ibuf->llx > INT_MIN) && (ibuf->lly > INT_MIN))
            {
                pidgin_whiteboard_draw_line(ibuf->wb, x, y, ibuf->llx, ibuf->lly, ibuf->brush_color,ibuf->brush_size);
//                gdk_draw_line(ibuf->drawing_area->window, ibuf->gc, x, y, ibuf->llx, ibuf->lly);
//                gdk_draw_line(ibuf->pixmap, ibuf->gc, x, y, ibuf->llx, ibuf->lly);
                ibuf->modified = 1;
            }
            
        case LINE:
            break;
        case OVAL:
            break;
        case ARC:
            break;
        case RECTANGLE:
            break;
        case TEXT:    
            purple_debug_error("gtkwhiteboard", "***Button press %d \n",ibuf->current_tool );
            if ((ibuf->llx != INT_MIN) && (ibuf->lly != INT_MIN)) /* previous round of text entry */
            {
                if (strlen(ibuf->textbuf))
                {
                    gdk_draw_string(ibuf->drawing_area->window, ibuf->font, ibuf->gc, ibuf->llx, ibuf->lly, ibuf->textbuf);
                    gdk_draw_string(ibuf->pixmap, ibuf->font, ibuf->gc, ibuf->llx, ibuf->lly, ibuf->textbuf);
                    ibuf->modified = 1;
                }
                strcpy(ibuf->textbuf, "");
                ibuf->llx = INT_MIN;
                ibuf->lly = INT_MIN;
            }
            break;
            
        case ERASE:
            /* TODO: object-oriented brush */
            old_color = ibuf->brush_color;
            old_size  = ibuf->brush_size;
            ibuf->brush_color = 0xFFFFFFFF;
            ibuf->brush_size = 6 * ibuf->brush_size;
            
            purple_whiteboard_send_brush(ibuf->wb, ibuf->brush_size, ibuf->brush_color);
            if(draw_list)
            {
                purple_whiteboard_draw_list_destroy(draw_list);
                draw_list = NULL;
            }
            /* Set tracking variables */
            LastX = x;
            LastY = y;
            MotionCount = 0;
            draw_list = g_list_append(draw_list, GINT_TO_POINTER(LastX));
            draw_list = g_list_append(draw_list, GINT_TO_POINTER(LastY));
            pidgin_whiteboard_draw_brush_point(ibuf->wb,
                    x, y,
                    ibuf->brush_color,  ibuf->brush_size);
            wb->draw_list = draw_list;
            ibuf->brush_color = old_color;
            ibuf->brush_size  = old_size;
            purple_whiteboard_send_brush(ibuf->wb, ibuf->brush_size, ibuf->brush_color);
            break;
        case FILL:
            pidgin_whiteboard_draw_fill(ibuf->wb, x, y );
            //        image_buf_flood_fill(ibuf, x, y, &(gcvalues.foreground));
            gtk_widget_queue_draw(ibuf->drawing_area);
            break;
		case BRUSH:
            /* TODO: object-oriented brush */
            old_color = ibuf->brush_color;
            old_size  = ibuf->brush_size;
            ibuf->brush_size = 6 * ibuf->brush_size;
            
            purple_whiteboard_send_brush(ibuf->wb, ibuf->brush_size, ibuf->brush_color);
            if(draw_list)
            {
                purple_whiteboard_draw_list_destroy(draw_list);
                draw_list = NULL;
            }
            /* Set tracking variables */
            LastX = x;
            LastY = y;
            MotionCount = 0;
            draw_list = g_list_append(draw_list, GINT_TO_POINTER(LastX));
            draw_list = g_list_append(draw_list, GINT_TO_POINTER(LastY));
            pidgin_whiteboard_draw_brush_point(ibuf->wb,
                    x, y,
                    ibuf->brush_color,  ibuf->brush_size);
            wb->draw_list = draw_list;
            ibuf->brush_color = old_color;
            ibuf->brush_size  = old_size;
            purple_whiteboard_send_brush(ibuf->wb, ibuf->brush_size, ibuf->brush_color);
            break;
            
    default:
        gdk_beep();
    }

    brush_distance = 0;
    brush_last_x = x;
    brush_last_y = y;

    ibuf->lx = x;
    ibuf->ly = y;
    ibuf->mx = INT_MIN;
    ibuf->my = INT_MIN;
}

void handle_button_move(PidginWhiteboard *ibuf, int x, int y)
{
    DRAWING_TOOL tool = ibuf->current_tool;
    GdkGCValues gcvalues;
    GdkRectangle rect, clientrect ;
    GdkColor col;
    GdkPixmap *pixmap = ibuf->pixmap;
	int dx, dy,old_color,old_size ;
	PurpleWhiteboard *wb = ibuf->wb;
	GList *draw_list = wb->draw_list;

    if ((ibuf->lx == INT_MIN) || (ibuf->ly == INT_MIN))
        return;

    rect.x = x;
    rect.y = y;
    rect.width = 10;
    rect.height = 10;
    clientrect.x = clientrect.y = 0;
    clientrect.width = image_buf_width(ibuf);
    clientrect.height = image_buf_height(ibuf);

    gdk_gc_get_values(ibuf->gc, &gcvalues);
    gdk_gc_set_clip_origin(ibuf->gc, 0, 0);
    gdk_gc_set_clip_rectangle(ibuf->gc, &clientrect);

    image_buf_clear_flash(ibuf);

    pidgin_whiteboard_rgb24_to_rgb48(ibuf->brush_color, &col);
	gdk_gc_set_rgb_fg_color(ibuf->gc, &col);

    insert_point(ibuf, x, y);
    image_buf_draw_flash(ibuf);

    switch (tool)
    {
        case PEN:
            dx = x - LastX;
            dy = y - LastY;
            MotionCount++;
            if(MotionCount == 100)
            {
                draw_list = g_list_append(draw_list, GINT_TO_POINTER(dx));
                draw_list = g_list_append(draw_list, GINT_TO_POINTER(dy));
                /* Send draw list to the draw_list handler */
                purple_whiteboard_send_draw_list(ibuf->wb, draw_list,3);
                /* The brush stroke is finished, clear the list for another one */
                if(draw_list)
                {
                    purple_whiteboard_draw_list_destroy(draw_list);
                    draw_list = NULL;
                }
                /* Reset motion tracking */
                MotionCount = 0;
                draw_list = g_list_append(draw_list, GINT_TO_POINTER(LastX));
                draw_list = g_list_append(draw_list, GINT_TO_POINTER(LastY));
                dx = x - LastX;
                dy = y - LastY;
            }
            draw_list = g_list_append(draw_list, GINT_TO_POINTER(dx));
            draw_list = g_list_append(draw_list, GINT_TO_POINTER(dy));
            pidgin_whiteboard_draw_brush_line(ibuf->wb,
                    LastX, LastY,
                    x, y,
                    ibuf->brush_color, ibuf->brush_size);
            LastX = x;
            LastY = y;
            wb->draw_list = draw_list;
            gdk_gc_set_line_attributes(ibuf->gc, gcvalues.line_width, gcvalues.line_style, gcvalues.cap_style, gcvalues.join_style);
            //gtk_widget_queue_draw_area(ibuf->drawing_area, rect.x, rect.y, rect.width, rect.height);
            break;
        case MULTILINE:
        case LINE:
            gdk_gc_set_function(ibuf->gc, GDK_INVERT);
            if ((ibuf->mx > INT_MIN) && (ibuf->my > INT_MIN))
            {
                gdk_draw_line(ibuf->drawing_area->window, ibuf->gc, ibuf->lx, ibuf->ly, ibuf->mx, ibuf->my);
            }
            gdk_draw_line(ibuf->drawing_area->window, ibuf->gc, ibuf->lx, ibuf->ly, x, y);
            gdk_gc_set_function(ibuf->gc, gcvalues.function);
            gdk_gc_set_line_attributes(ibuf->gc, gcvalues.line_width, gcvalues.line_style, gcvalues.cap_style, gcvalues.join_style);
            break;
        case OVAL:
            gdk_gc_set_function(ibuf->gc, GDK_INVERT);
            if ((ibuf->mx > INT_MIN) && (ibuf->my > INT_MIN))
            {
                draw_oval(ibuf, FALSE, ibuf->lx, ibuf->ly, ibuf->mx, ibuf->my,1);
            }

            draw_oval(ibuf, FALSE, ibuf->lx, ibuf->ly, x, y,1);
            gdk_gc_set_function(ibuf->gc, gcvalues.function);
            gdk_gc_set_line_attributes(ibuf->gc, gcvalues.line_width, gcvalues.line_style, gcvalues.cap_style, gcvalues.join_style);
            break;

        case ARC:
            gdk_gc_set_function(ibuf->gc, GDK_INVERT);
            if ((ibuf->mx > INT_MIN) && (ibuf->my > INT_MIN))
            {
                draw_arc(ibuf, FALSE, ibuf->lx, ibuf->ly, ibuf->mx, ibuf->my,1);
            }
            draw_arc(ibuf, FALSE, ibuf->lx, ibuf->ly, x, y,1);
            gdk_gc_set_function(ibuf->gc, gcvalues.function);
            gdk_gc_set_line_attributes(ibuf->gc, gcvalues.line_width, gcvalues.line_style, gcvalues.cap_style, gcvalues.join_style);
            break;
            
        case RECTANGLE:
            gdk_gc_set_function(ibuf->gc, GDK_INVERT);
            if ((ibuf->mx > INT_MIN) && (ibuf->my > INT_MIN))
            {
                draw_rectangle(ibuf, FALSE, ibuf->lx, ibuf->ly, ibuf->mx, ibuf->my,1);
            }
            draw_rectangle(ibuf, FALSE, ibuf->lx, ibuf->ly, x, y,1);
            gdk_gc_set_function(ibuf->gc, gcvalues.function);
            
            break;
            
        case ERASE:
            old_color = ibuf->brush_color;
            old_size  = ibuf->brush_size;
            ibuf->brush_color = 0xFFFFFFFF;
            ibuf->brush_size  = 6 * ibuf->brush_size;
            purple_whiteboard_send_brush(ibuf->wb, ibuf->brush_size, ibuf->brush_color);
            dx = x - LastX;
            dy = y - LastY;
            MotionCount++;
            if(MotionCount == 100)
            {
                draw_list = g_list_append(draw_list, GINT_TO_POINTER(dx));
                draw_list = g_list_append(draw_list, GINT_TO_POINTER(dy));
                /* Send draw list to the draw_list handler */
                purple_whiteboard_send_draw_list(ibuf->wb, draw_list,3);
                /* The brush stroke is finished, clear the list for another one */
                if(draw_list)
                {
                    purple_whiteboard_draw_list_destroy(draw_list);
                    draw_list = NULL;
                }
                /* Reset motion tracking */
                MotionCount = 0;
                draw_list = g_list_append(draw_list, GINT_TO_POINTER(LastX));
                draw_list = g_list_append(draw_list, GINT_TO_POINTER(LastY));
                dx = x - LastX;
                dy = y - LastY;
            }
            draw_list = g_list_append(draw_list, GINT_TO_POINTER(dx));
            draw_list = g_list_append(draw_list, GINT_TO_POINTER(dy));
            pidgin_whiteboard_draw_brush_line(ibuf->wb,
                                              LastX, LastY,
                                              x, y,
                                              ibuf->brush_color , ibuf->brush_size);
            LastX = x;
            LastY = y;
            wb->draw_list = draw_list;

            ibuf->brush_color = old_color;
            ibuf->brush_size  = old_size;
            purple_whiteboard_send_brush(ibuf->wb, ibuf->brush_size, ibuf->brush_color);
//            image_buf_paint_interpolate(ibuf, x, y);
            break;
        case BRUSH:
            old_color = ibuf->brush_color;
            old_size  = ibuf->brush_size;
            ibuf->brush_size  = 6 * ibuf->brush_size;
            purple_whiteboard_send_brush(ibuf->wb, ibuf->brush_size, ibuf->brush_color);
            dx = x - LastX;
            dy = y - LastY;
            MotionCount++;
            if(MotionCount == 100)
            {
                draw_list = g_list_append(draw_list, GINT_TO_POINTER(dx));
                draw_list = g_list_append(draw_list, GINT_TO_POINTER(dy));
                /* Send draw list to the draw_list handler */
                purple_whiteboard_send_draw_list(ibuf->wb, draw_list,3);
                /* The brush stroke is finished, clear the list for another one */
                if(draw_list)
                {
                    purple_whiteboard_draw_list_destroy(draw_list);
                    draw_list = NULL;
                }
                /* Reset motion tracking */
                MotionCount = 0;
                draw_list = g_list_append(draw_list, GINT_TO_POINTER(LastX));
                draw_list = g_list_append(draw_list, GINT_TO_POINTER(LastY));
                dx = x - LastX;
                dy = y - LastY;
            }
            draw_list = g_list_append(draw_list, GINT_TO_POINTER(dx));
            draw_list = g_list_append(draw_list, GINT_TO_POINTER(dy));
            pidgin_whiteboard_draw_brush_line(ibuf->wb,
                                              LastX, LastY,
                                              x, y,
                                              ibuf->brush_color , ibuf->brush_size);
            LastX = x;
            LastY = y;
            wb->draw_list = draw_list;

            ibuf->brush_color = old_color;
            ibuf->brush_size  = old_size;
            purple_whiteboard_send_brush(ibuf->wb, ibuf->brush_size, ibuf->brush_color);

        default:
            break;
    }
    ibuf->mx = x;
    ibuf->my = y;
}

gboolean
on_drawingarea_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    PidginWhiteboard *gtkwb = (PidginWhiteboard*)user_data;
    GdkPixmap *pixmap = gtkwb->pixmap;

	if(BrushState != BRUSH_STATE_UP)
	{
		/* Potential double-click DOWN to DOWN? */
		BrushState = BRUSH_STATE_DOWN;

		/* return FALSE; */
	}

	BrushState = BRUSH_STATE_DOWN;

	if(event->button == 1 && pixmap != NULL)
        handle_button_press (gtkwb, event->x, event->y);
	return TRUE;
}

gboolean
on_drawingarea_button_release_event    (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
//    PidginWhiteboard *ibuf = widget_get_image(widget);
    PidginWhiteboard *gtkwb = (PidginWhiteboard*)(user_data);
    GList *draw_list = gtkwb->wb->draw_list;
    
    if(event->button == 1 && gtkwb->pixmap != NULL)
	{
        handle_button_release (gtkwb, event->x, event->y);
	}
//    gtkwb->pixmap = ibuf->pixmap;
    return FALSE;
}

gboolean
on_drawingarea_motion_notify_event     (GtkWidget       *widget,
                                        GdkEventMotion  *event,
                                        gpointer         user_data)
{
    int x, y;
    GdkModifierType state;
//    PidginWhiteboard *ibuf = widget_get_image(widget);
    PidginWhiteboard *gtkwb = (PidginWhiteboard*)(user_data);
    GdkPixmap *pixmap = gtkwb->pixmap;
    if (event->is_hint)
        gdk_window_get_pointer (event->window, &x, &y, &state);
    else
    {
        x = event->x;
        y = event->y;
        state = event->state;
    }
    if (state & GDK_BUTTON1_MASK){
//        handle_button_move (gtkwb , x, y);
        if((BrushState != BRUSH_STATE_DOWN) && (BrushState != BRUSH_STATE_MOTION))
        {
            purple_debug_error("gtkwhiteboard", "***Bad brush state transition %d to MOTION\n", BrushState);
			BrushState = BRUSH_STATE_MOTION;
            return FALSE;
        }
        BrushState = BRUSH_STATE_MOTION;
        handle_button_move (gtkwb , x, y);
    }

    return TRUE;
}

void handle_key_release(PidginWhiteboard *ibuf, GdkEventKey *keyevent)
{
    GdkGCValues gcvalues;
    GdkColor white;
    int length;
    char tmp[5000];
    
    if ((ibuf->llx == INT_MIN) || (ibuf->lly == INT_MIN) || (ibuf->current_tool != TEXT))
    {
        gdk_beep();
        return;
    }
    image_buf_clear_flash(ibuf);

    gdk_gc_get_values(ibuf->gc, &gcvalues);
    strcpy(tmp, ibuf->textbuf);
    length = strlen(ibuf->textbuf);
    if ((keyevent->keyval == GDK_BackSpace) || (keyevent->keyval == GDK_Delete))
    {
        if (length)
            ibuf->textbuf[length - 1] = 0;
    }
    else if (keyevent->keyval == GDK_Return)
        ;
    else if (keyevent->string)
        strcat(ibuf->textbuf, keyevent->string);
    else if ((keyevent->keyval >= GDK_space) && (keyevent->keyval < GDK_Shift_L))
    {
        purple_debug_info("", "%c", keyevent->keyval );
        sprintf(ibuf->textbuf, "%s%c", tmp, keyevent->keyval);
        pidgin_whiteboard_draw_text(ibuf->wb, ibuf->llx, ibuf->lly, keyevent->keyval);
        ibuf->llx +=  (int) (gdk_string_width(ibuf->font, "M") * 1.1 + 0.5);
    }
    if (keyevent->keyval == GDK_Escape)
    {
        strcpy(ibuf->textbuf, "");
    }
    purple_debug_info("", "%c\n", keyevent->keyval );
/*
    gdk_gc_set_function(ibuf->gc, GDK_INVERT);
    gdk_color_white(gdk_rgb_get_cmap(), &white);

    gdk_gc_set_foreground(ibuf->gc, &white);
    if (strlen(tmp))
    {
//        pidgin_whiteboard_draw_text(ibuf->wb, ibuf->llx, ibuf->lly, tmp);
//        gdk_draw_string(ibuf->drawing_area->window, ibuf->font, ibuf->gc, ibuf->llx, ibuf->lly, tmp);
//        gdk_draw_string(ibuf->pixmap, ibuf->font, ibuf->gc, ibuf->llx, ibuf->lly, tmp);
    }

    if (strlen(ibuf->textbuf))
    {
//        pidgin_whiteboard_draw_text(ibuf->wb, ibuf->llx, ibuf->lly, ibuf->textbuf);
//        gdk_draw_string(ibuf->drawing_area->window, ibuf->font, ibuf->gc, ibuf->llx, ibuf->lly, ibuf->textbuf);
//        gdk_draw_string(ibuf->pixmap, ibuf->font, ibuf->gc, ibuf->llx, ibuf->lly, ibuf->textbuf);
    }


    gdk_gc_set_function(ibuf->gc, gcvalues.function);
    gdk_gc_set_foreground(ibuf->gc, &(gcvalues.foreground));
*/
    if (keyevent->keyval == GDK_Return)
    {
        /*
        if (strlen(ibuf->textbuf))
        {
            
            gdk_draw_string(ibuf->drawing_area->window, ibuf->font, ibuf->gc, ibuf->llx, ibuf->lly, ibuf->textbuf);
            gdk_draw_string(ibuf->pixmap, ibuf->font, ibuf->gc, ibuf->llx, ibuf->lly, ibuf->textbuf);
            ibuf->modified = 1;
        }
        */
        strcpy(ibuf->textbuf, "");

        ibuf->lly +=  (int) (gdk_string_height(ibuf->font, "My") * 1.1 + 0.5);
    }
    else if (keyevent->keyval == GDK_Escape)
    {
        ibuf->llx = ibuf->lly = INT_MIN;
    }
}

gboolean
on_drawingarea_key_release_event     (GtkWidget       *widget,
                                      GdkEventKey     *event,
                                      gpointer         user_data)
{
    PidginWhiteboard *ibuf = (PidginWhiteboard*)(user_data);
    g_assert(ibuf);
    if ((ibuf->llx == INT_MIN) || (ibuf->lly == INT_MIN) || (ibuf->current_tool != TEXT))
    {
        gdk_beep();
        return;
    }
    if (event->keyval == GDK_Escape)
    {
        ibuf->llx = ibuf->lly = INT_MIN;
        return;
    }
    if ((event->keyval >= GDK_space) && (event->keyval < GDK_Shift_L)){
        pidgin_whiteboard_draw_text(ibuf->wb, ibuf->llx, ibuf->lly, event->keyval);
        ibuf->llx +=  (int) (gdk_string_width(ibuf->font, "M") * 1.1 + 0.5);
    }
    if (event->keyval == GDK_Return)
    {
        ibuf->llx =  ibuf->llx - 8 * ((int) (gdk_string_width(ibuf->font, "My") * 1.1 + 0.5));
        ibuf->lly +=  (int) (gdk_string_height(ibuf->font, "My") * 1.1 + 0.5);
    }

//    handle_key_release(ibuf, event);
    if (event->string)
        fprintf(stderr, "%s", event->string);
    return FALSE;
}

void
set_title (GtkWindow *window, const char *filename)
{
  char *name = g_filename_display_basename (filename);
  char *title = g_strdup_printf ("%s : Swfplay", name);

  g_free (name);
  gtk_window_set_title (window, title);
  g_free (title);
}

gboolean
on_drawingarea_expose_event            (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data)
{
//    PidginWhiteboard *ibuf = new image;
    PidginWhiteboard *gtkwb = (PidginWhiteboard*)(user_data);

//    purple_debug_error("gtkwhiteboard", "***Button expose event %u \n", gtkwb );

    gdk_draw_pixmap(widget->window, widget->style->fg_gc[GTK_STATE_NORMAL], gtkwb->pixmap , 
                    event->area.x, event->area.y, event->area.x, event->area.y, 
                    event->area.width, event->area.height);

    gdk_draw_drawable(widget->window,
					  widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
					  gtkwb->pixmap,
					  event->area.x, event->area.y,
					  event->area.x, event->area.y,
					  event->area.width, event->area.height);

//    ibuf->pixmap = gtkwb->pixmap ;
//    purple_debug_error("gtkwhiteboard", "***Button expose event 2 %u \n", gtkwb );
    return FALSE;
}
void
on_line_width_combo_combo_entry_changed (GtkEditable     *editable,
                                         gpointer         user_data)
{
    char *tmp;
    int t;
    PidginWhiteboard *ibuf = (PidginWhiteboard*)(user_data);
    g_assert(ibuf);
    tmp = gtk_editable_get_chars(editable, 0, -1);
    g_assert(tmp);
    sscanf(tmp, "%d", &t);
    g_assert(t != 0);
    gdk_gc_set_line_attributes(ibuf->gc, t, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_MITER);
    purple_debug_error("gtkwhiteboard", "*** %d  \n",t );
    
    purple_whiteboard_get_brush(ibuf->wb, &ibuf->brush_size, &ibuf->brush_color);
    purple_whiteboard_send_brush(ibuf->wb, t , ibuf->brush_color);
    ibuf->brush_size = t;
    g_free(tmp);
    gtk_widget_grab_focus(ibuf->drawing_area); /* force the line width entry widget to give up focus */
}

static void pidgin_whiteboard_create(PurpleWhiteboard *wb)
{
	PurpleBuddy *buddy;
	GtkWidget *window;
	GtkWidget *drawing_area;
	GtkWidget *vbox_controls;
	GtkWidget *hbox_canvas_and_controls;

     GtkWidget *toolbar6;
     GtkWidget *tmp_toolbar_icon;
     GtkWidget *table5;
     GtkWidget *Line_Width;
     GtkWidget *Line_Width1;
     GtkWidget *line_width_combo;
     GList *line_width_combo_items = NULL;
     GtkWidget *line_width_combo_combo_entry;
     GtkWidget *table8;
     GtkWidget *fixed1;
     GtkWidget *table4;
     GtkWidget *toolbar4;
     GtkWidget *erase_button;
     GtkWidget *fill_button;
     GtkWidget *line_button;
     GtkWidget *multiline_button;
     GtkWidget *rectangle_button;
     GtkWidget *toolbar5;
     GtkWidget *pen_button;
     GtkWidget *text_button;
     GtkWidget *arc_button;
     GtkWidget *oval_button;
     GtkWidget *brush_button;
	 GtkWidget *filled_button;

     GtkTooltips *tooltips;
     tooltips = gtk_tooltips_new ();
	/*
		--------------------------
		|[][][][palette[][][][][]|
		|------------------------|
		|       canvas     | con |
		|                  | trol|
		|                  | s   |
		|                  |     |
		|                  |     |
		--------------------------
	*/
	GtkWidget *clear_button;
	GtkWidget *save_button;
	GtkWidget *color_button;

//	PidginWhiteboard *gtkwb = g_new0(PidginWhiteboard, 1);
    PidginWhiteboard *gtkwb = g_new0(PidginWhiteboard, 1);
	gtkwb->wb = wb;
	wb->ui_data = gtkwb;

	/* Get dimensions (default?) for the whiteboard canvas */
	if (!purple_whiteboard_get_dimensions(wb, &gtkwb->width, &gtkwb->height))
	{
		/* Give some initial board-size */
		gtkwb->width = 520;
		gtkwb->height = 520;
	}

	if (!purple_whiteboard_get_brush(wb, &gtkwb->brush_size, &gtkwb->brush_color))
	{
		/* Give some initial brush-info */
		gtkwb->brush_size = 2;
		gtkwb->brush_color = 0xff0000;
	}

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtkwb->window = window;
	gtk_widget_set_name(window, wb->who);

//    gtk_window_set_default_size((GtkWindow*)(window),600,400);
	/* Try and set window title as the name of the buddy, else just use their
	 * username
	 */
	buddy = purple_find_buddy(wb->account, wb->who);

	if (buddy != NULL)
		gtk_window_set_title((GtkWindow*)(window), purple_buddy_get_contact_alias(buddy));
	else
		gtk_window_set_title((GtkWindow*)(window), wb->who);

    gtk_window_set_resizable((GtkWindow*)(window), FALSE);
    
    g_signal_connect(G_OBJECT(window), "delete_event",
                     G_CALLBACK(whiteboard_close_cb), gtkwb);

    hbox_canvas_and_controls = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox_canvas_and_controls);

	gtk_container_add(GTK_CONTAINER(window), hbox_canvas_and_controls);
	gtk_container_set_border_width(GTK_CONTAINER(window), PIDGIN_HIG_BORDER);

	
	/* Create the drawing area */

    drawing_area = gtk_drawing_area_new ();
    gtkwb->drawing_area = drawing_area;
    gtk_widget_set_name (drawing_area, "drawingarea");
    gtk_widget_set_size_request(GTK_WIDGET(drawing_area), gtkwb->width , gtkwb->height);
    gtk_widget_ref (drawing_area);
    gtk_object_set_data_full (GTK_OBJECT (window), "drawingarea", drawing_area,
                              (GtkDestroyNotify) gtk_widget_unref);
    gtk_box_pack_start(GTK_BOX(hbox_canvas_and_controls), drawing_area, TRUE, TRUE, PIDGIN_HIG_BOX_SPACE);
    gtk_widget_show (drawing_area);

    gtk_widget_set_events (drawing_area, GDK_EXPOSURE_MASK | GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK | GDK_BUTTON_MOTION_MASK | GDK_BUTTON1_MOTION_MASK | GDK_BUTTON2_MOTION_MASK | GDK_BUTTON3_MOTION_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK | GDK_PROPERTY_CHANGE_MASK | GDK_VISIBILITY_NOTIFY_MASK | GDK_FOCUS_CHANGE_MASK | GDK_STRUCTURE_MASK | GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_PROPERTY_CHANGE_MASK);
    GTK_WIDGET_SET_FLAGS(drawing_area, GTK_CAN_FOCUS);
    GTK_WIDGET_SET_FLAGS(drawing_area, GTK_CAN_DEFAULT);

//    gtkwb->drawing_area = drawing_area;
    gtk_object_set_user_data(GTK_OBJECT(gtkwb->window),  gtkwb);
    gtk_object_set_user_data(GTK_OBJECT(gtkwb->drawing_area), gtkwb);
//    gtk_drawing_area_size(GTK_DRAWING_AREA(gtkwb->drawing_area), 500, 500);

/*	drawing_area = gtk_drawing_area_new();
	gtkwb->drawing_area = drawing_area;
//	gtk_widget_set_size_request(GTK_WIDGET(drawing_area), gtkwb->width, gtkwb->height);
    gtk_widget_set_size_request(GTK_WIDGET(drawing_area), 500 , 500);
	gtk_box_pack_start(GTK_BOX(hbox_canvas_and_controls), drawing_area, TRUE, TRUE, PIDGIN_HIG_BOX_SPACE);
	gtk_widget_show(drawing_area);
*/
    g_signal_connect (G_OBJECT (drawing_area), "configure_event",
                      G_CALLBACK(pidgin_whiteboard_configure_event),
                      gtkwb);
    g_signal_connect (G_OBJECT (drawing_area), "expose_event",
                      G_CALLBACK(on_drawingarea_expose_event),
                      gtkwb);
    g_signal_connect (G_OBJECT (drawing_area), "button_press_event",
                      G_CALLBACK(on_drawingarea_button_press_event),
                      gtkwb);
    g_signal_connect (G_OBJECT (drawing_area), "button_release_event",
                      G_CALLBACK(on_drawingarea_button_release_event),
                      gtkwb);
    g_signal_connect (G_OBJECT (drawing_area), "motion_notify_event",
                      G_CALLBACK (on_drawingarea_motion_notify_event),
                      gtkwb);
    g_signal_connect (GTK_OBJECT (drawing_area), "key_release_event",
                      GTK_SIGNAL_FUNC (on_drawingarea_key_release_event),
                      gtkwb);
//    image_buf_create_pixmap(gtkwb);
    /*
    g_signal_connect(G_OBJECT(drawing_area), "button_press_event",
					 G_CALLBACK(pidgin_whiteboard_brush_down), gtkwb);

	g_signal_connect(G_OBJECT(drawing_area), "motion_notify_event",
					 G_CALLBACK(pidgin_whiteboard_brush_motion), gtkwb);

	g_signal_connect(G_OBJECT(drawing_area), "button_release_event",
					 G_CALLBACK(pidgin_whiteboard_brush_up), gtkwb);
*/
	gtk_widget_set_events(drawing_area,
						  GDK_EXPOSURE_MASK |
						  GDK_LEAVE_NOTIFY_MASK |
						  GDK_BUTTON_PRESS_MASK |
						  GDK_POINTER_MOTION_MASK |
						  GDK_BUTTON_RELEASE_MASK |
						  GDK_POINTER_MOTION_HINT_MASK);


    table4 = gtk_table_new (9, 2, FALSE);
    gtk_box_pack_start(GTK_BOX(hbox_canvas_and_controls),
					table4, FALSE, FALSE, PIDGIN_HIG_BOX_SPACE);
    gtk_widget_set_name (table4, "table4");
    gtk_widget_ref (table4);
    gtk_object_set_data_full (GTK_OBJECT (window), "table4", table4,
                            (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (table4);
    gtk_container_set_border_width (GTK_CONTAINER (table4), 2);

    toolbar4 = gtk_toolbar_new();
    gtk_toolbar_set_orientation(GTK_TOOLBAR(toolbar4),GTK_ORIENTATION_VERTICAL);
    gtk_toolbar_set_style(GTK_TOOLBAR(toolbar4), GTK_TOOLBAR_ICONS);
    gtk_widget_set_name (toolbar4, "toolbar4");
    gtk_widget_ref (toolbar4);
    gtk_object_set_data_full (GTK_OBJECT (window), "toolbar4", toolbar4,
            (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (toolbar4);
    gtk_table_attach (GTK_TABLE (table4), toolbar4, 0, 1, 0, 1,
            (GtkAttachOptions) (0),
            (GtkAttachOptions) (0), 0, 0);
    gtk_container_set_border_width (GTK_CONTAINER (toolbar4), 1);


    clear_button = gtk_button_new_from_stock(GTK_STOCK_CLEAR);
    gtk_widget_show(clear_button);
    g_signal_connect(G_OBJECT(clear_button), "clicked",
            G_CALLBACK(pidgin_whiteboard_button_clear_press), gtkwb);
    //gtk_table_attach (GTK_TABLE (table4), clear_button, 0, 2, 4, 5,
    gtk_table_attach (GTK_TABLE (table4), clear_button, 1, 2, 3, 4,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);

    save_button = gtk_button_new_from_stock(GTK_STOCK_SAVE);
    gtk_widget_show(save_button);
    //gtk_table_attach (GTK_TABLE (table4), save_button , 0, 2, 5, 6,
    gtk_table_attach (GTK_TABLE (table4), save_button , 0, 1, 5, 6,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
    g_signal_connect(G_OBJECT(save_button), "clicked",
          G_CALLBACK(pidgin_whiteboard_button_save_press), gtkwb);

    color_button = gtk_button_new_from_stock(GTK_STOCK_SELECT_COLOR);
    //gtk_table_attach (GTK_TABLE (table4), color_button , 0, 2, 6, 7,
    gtk_table_attach (GTK_TABLE (table4), color_button , 1, 2, 5, 6,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
    gtk_widget_show(color_button);
    g_signal_connect(G_OBJECT(color_button), "clicked",
					 G_CALLBACK(color_select_dialog), gtkwb);

	GdkPixbuf *pixbuf;
	char *filename;

    if(wb->boardType == TEACHER_BOARD){
		
    //GdkPixbuf *pixbuf;
    //char *filename = g_build_filename(DATADIR, "pixmaps", "pidgin","eraseOp.xpm", NULL);
    filename = g_build_filename(DATADIR, "pixmaps", "pidgin","eraseOp.xpm", NULL);
    pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    g_free(filename);
    tmp_toolbar_icon = gtk_image_new_from_pixbuf(pixbuf);
    erase_button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar4),
                                               GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                                               NULL,
                                               "",
                                               _("erase"), NULL,
                                               tmp_toolbar_icon, NULL, NULL);
    gtk_widget_set_name (erase_button, "erase_button");
    gtk_widget_ref (erase_button);
    gtk_object_set_data_full (GTK_OBJECT (window), "erase_button", erase_button,
                            (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (erase_button);
  
  filename = g_build_filename(DATADIR, "pixmaps", "pidgin","lineOp.xpm", NULL);
  pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
  g_free(filename);
  tmp_toolbar_icon = gtk_image_new_from_pixbuf(pixbuf);
  line_button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar4),
                                GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                                NULL,
                                "",
                                _("line"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_widget_set_name (line_button, "line_button");
  gtk_widget_ref (line_button);
  gtk_object_set_data_full (GTK_OBJECT (window), "line_button", line_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (line_button);

  filename = g_build_filename(DATADIR, "pixmaps", "pidgin","clineOp.xpm", NULL);
  pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
  g_free(filename);
  tmp_toolbar_icon = gtk_image_new_from_pixbuf(pixbuf);
  multiline_button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar4),
                                GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                                NULL,
                                "",
                                _("polyline"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_widget_set_name (multiline_button, "multiline_button");
  gtk_widget_ref (multiline_button);
  gtk_object_set_data_full (GTK_OBJECT (window), "multiline_button", multiline_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (multiline_button);

  filename = g_build_filename(DATADIR, "pixmaps", "pidgin","boxOp.xpm", NULL);
  pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
  g_free(filename);
  tmp_toolbar_icon = gtk_image_new_from_pixbuf(pixbuf);
  rectangle_button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar4),
                                GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                                NULL,
                                "",
                                _("rectangle"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_widget_set_name (rectangle_button, "rectangle_button");
  gtk_widget_ref (rectangle_button);
  gtk_object_set_data_full (GTK_OBJECT (window), "rectangle_button", rectangle_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (rectangle_button);
  
  toolbar5 = gtk_toolbar_new ();
  gtk_toolbar_set_orientation(GTK_TOOLBAR(toolbar5),GTK_ORIENTATION_VERTICAL);
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar5), GTK_TOOLBAR_ICONS);

  gtk_widget_set_name (toolbar5, "toolbar5");
  gtk_widget_ref (toolbar5);
  gtk_object_set_data_full (GTK_OBJECT (window), "toolbar5", toolbar5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (toolbar5);
  gtk_table_attach (GTK_TABLE (table4), toolbar5, 1, 2, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (toolbar5), 1);
//  gtk_toolbar_set_space_style (GTK_TOOLBAR (toolbar5), GTK_TOOLBAR_SPACE_LINE);
  
  filename = g_build_filename(DATADIR, "pixmaps", "pidgin","pencilOp.xpm", NULL);
  pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
  g_free(filename);
  tmp_toolbar_icon = gtk_image_new_from_pixbuf(pixbuf);
  pen_button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar5),
                                GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                                NULL,
                                "",
                                _("pen"), NULL,
                                tmp_toolbar_icon, NULL, NULL);
  gtk_widget_set_name (pen_button, "pen_button");
  gtk_widget_ref (pen_button);
  gtk_object_set_data_full (GTK_OBJECT (window), "pen_button", pen_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (pen_button);
  
  filename = g_build_filename(DATADIR, "pixmaps", "pidgin","textOp.xpm", NULL);
  pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
  g_free(filename);
  tmp_toolbar_icon = gtk_image_new_from_pixbuf(pixbuf);
  text_button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar5),
                                GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                                NULL,
                                "",
                                _("text"), NULL,
                                tmp_toolbar_icon , NULL, NULL);
  gtk_widget_set_name (text_button, "text_button");
  gtk_widget_ref (text_button);
  gtk_object_set_data_full (GTK_OBJECT (window), "text_button", text_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (text_button);

  filename = g_build_filename(DATADIR, "pixmaps", "pidgin","arcOp.xpm", NULL);
  pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
  g_free(filename);
  tmp_toolbar_icon = gtk_image_new_from_pixbuf(pixbuf);
  arc_button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar5),
                                GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                                NULL,
                                "",
                                _("arc"), NULL,
                                tmp_toolbar_icon , NULL, NULL);
  gtk_widget_set_name (arc_button, "arc_button");
  gtk_widget_ref (arc_button);
  gtk_object_set_data_full (GTK_OBJECT (window), "arc_button", arc_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (arc_button);


  filename = g_build_filename(DATADIR, "pixmaps", "pidgin","ovalOp.xpm", NULL);
  pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
  g_free(filename);
  tmp_toolbar_icon = gtk_image_new_from_pixbuf(pixbuf);
  oval_button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar5),
                                GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                                NULL,
                                "",
                                _("oval"), NULL,
                                tmp_toolbar_icon , NULL, NULL);
  gtk_widget_set_name (oval_button, "oval_button");
  gtk_widget_ref (oval_button);
  gtk_object_set_data_full (GTK_OBJECT (window), "oval_button", oval_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (oval_button);


  filename = g_build_filename(DATADIR, "pixmaps", "pidgin","brushOp.xpm", NULL);
  pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
  g_free(filename);
  tmp_toolbar_icon = gtk_image_new_from_pixbuf(pixbuf);
  brush_button = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar5),
                                GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                                NULL,
                                "",
                                _("brush"), NULL,
                                tmp_toolbar_icon , NULL, NULL);
  gtk_widget_set_name (brush_button, "brush_button");
  gtk_widget_ref (brush_button);
  gtk_object_set_data_full (GTK_OBJECT (window), "brush_button", brush_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (brush_button);

  filename = g_build_filename(DATADIR, "pixmaps", "pidgin","unfilled.xpm", NULL);
  pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
  g_free(filename);
  tmp_toolbar_icon = gtk_image_new_from_pixbuf(pixbuf);
  filled_button = gtk_toggle_button_new_with_label ("");
  gtk_button_set_image(filled_button, tmp_toolbar_icon );
  gtk_widget_set_name (filled_button, "filled_button");
  gtk_widget_ref (filled_button);
  gtk_object_set_data_full (GTK_OBJECT (window), "filled_button", filled_button,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (filled_button);
  gtk_table_attach (GTK_TABLE (table4), filled_button, 0, 2, 1, 2,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (filled_button), 3);
  GTK_WIDGET_UNSET_FLAGS (filled_button, GTK_CAN_FOCUS);
  gtk_tooltips_set_tip (tooltips, filled_button, _("fill"), NULL);


  /////
//  gnome_font_picker_set_mode (GNOME_FONT_PICKER (fontpicker), GNOME_FONT_PICKER_MODE_FONT_INFO);
//  gnome_font_picker_fi_set_use_font_in_label (GNOME_FONT_PICKER (fontpicker), TRUE, 14);
//  GTK_WIDGET_UNSET_FLAGS (fontpicker, GTK_CAN_FOCUS);
//  gtk_tooltips_set_tip (tooltips, fontpicker, _("fontpicker"), NULL);

  /*Line_Width = gtk_label_new (_("line width"));
  gtk_widget_set_name (Line_Width, "Line_Width");
  gtk_widget_ref (Line_Width);
  gtk_object_set_data_full (GTK_OBJECT (window), "Line_Width", Line_Width,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (Line_Width);
  gtk_table_attach (GTK_TABLE (table4), Line_Width, 0, 1, 3, 4,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);*/

  line_width_combo = gtk_combo_new ();
  gtk_widget_set_name (line_width_combo, "line_width_combo");
  gtk_widget_ref (line_width_combo);
  gtk_object_set_data_full (GTK_OBJECT (window), "line_width_combo", line_width_combo,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (line_width_combo);
  gtk_table_attach (GTK_TABLE (table4), line_width_combo, 0, 1, 3, 4,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_widget_set_usize (line_width_combo, 45, -2);
  gtk_combo_set_value_in_list (GTK_COMBO (line_width_combo), TRUE, FALSE);
  line_width_combo_items = g_list_append (line_width_combo_items, (gpointer) _("1"));
  line_width_combo_items = g_list_append (line_width_combo_items, (gpointer) _("2"));
  line_width_combo_items = g_list_append (line_width_combo_items, (gpointer) _("3"));
  line_width_combo_items = g_list_append (line_width_combo_items, (gpointer) _("4"));
  line_width_combo_items = g_list_append (line_width_combo_items, (gpointer) _("6"));
  line_width_combo_items = g_list_append (line_width_combo_items, (gpointer) _("8"));
  gtk_combo_set_popdown_strings (GTK_COMBO (line_width_combo), line_width_combo_items);
  g_list_free (line_width_combo_items);

  line_width_combo_combo_entry = GTK_COMBO (line_width_combo)->entry;
  gtk_widget_set_name (line_width_combo_combo_entry, "line_width_combo_combo_entry");
  gtk_widget_ref (line_width_combo_combo_entry);
  gtk_object_set_data_full (GTK_OBJECT (window), "line_width_combo_combo_entry", line_width_combo_combo_entry,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (line_width_combo_combo_entry);
  gtk_tooltips_set_tip (tooltips, line_width_combo_combo_entry, _("line width"), NULL);
  gtk_entry_set_text (GTK_ENTRY (line_width_combo_combo_entry), _("1"));


/*****/
    /*   Event handling*/

//    gtk_signal_connect (GTK_OBJECT (fontpicker), "font_set",  GTK_SIGNAL_FUNC (on_fontpicker_font_set), gtkwb);
/* anil */
    gtk_signal_connect (GTK_OBJECT (line_width_combo_combo_entry), "changed",
                      GTK_SIGNAL_FUNC (on_line_width_combo_combo_entry_changed),
                      gtkwb);
    gtk_signal_connect (GTK_OBJECT (erase_button), "toggled",
                      GTK_SIGNAL_FUNC (on_erase_button_toggled),
                      gtkwb);
    gtk_signal_connect (GTK_OBJECT (fill_button), "toggled",
                      GTK_SIGNAL_FUNC (on_fill_button_toggled),
                      gtkwb);
    gtk_signal_connect (GTK_OBJECT (line_button), "toggled",
                      GTK_SIGNAL_FUNC (on_line_button_toggled),
                      gtkwb);
    gtk_signal_connect (GTK_OBJECT (multiline_button), "toggled",
                      GTK_SIGNAL_FUNC (on_multiline_button_toggled),
                      gtkwb);
    gtk_signal_connect (GTK_OBJECT (rectangle_button), "toggled",
                      GTK_SIGNAL_FUNC (on_rectangle_button_toggled),
                      gtkwb);
    gtk_signal_connect (GTK_OBJECT (pen_button), "toggled",
                      GTK_SIGNAL_FUNC (on_pen_button_toggled),
                      gtkwb);

    gtk_signal_connect (GTK_OBJECT (text_button), "toggled",
                      GTK_SIGNAL_FUNC (on_text_button_toggled),
                      gtkwb);

    gtk_signal_connect (GTK_OBJECT (arc_button), "toggled",
                      GTK_SIGNAL_FUNC (on_arc_button_toggled),
                      gtkwb);
    gtk_signal_connect (GTK_OBJECT (oval_button), "toggled",
                      GTK_SIGNAL_FUNC (on_oval_button_toggled),
                      gtkwb);
    gtk_signal_connect (GTK_OBJECT (brush_button), "toggled",
                      GTK_SIGNAL_FUNC (on_brush_button_toggled),
                      gtkwb);
    gtk_signal_connect (GTK_OBJECT (filled_button), "toggled",
                      GTK_SIGNAL_FUNC (on_filled_button_toggled),
                      gtkwb);
    gtk_object_set_data (GTK_OBJECT (window), "tooltips", tooltips);    
    }

    	/* Make all this (window) visible */
	gtk_widget_show(window);


	pidgin_whiteboard_set_canvas_as_icon(gtkwb);


	/* TODO Specific protocol/whiteboard assignment here? Needs a UI Op? */
	/* Set default brush size and color */
	/*
	ds->brush_size = DOODLE_BRUSH_MEDIUM;
	ds->brush_color = 0;
	*/
}

static void pidgin_whiteboard_destroy(PurpleWhiteboard *wb)
{
	PidginWhiteboard *gtkwb;

	g_return_if_fail(wb != NULL);
	gtkwb = wb->ui_data;
	g_return_if_fail(gtkwb != NULL);

	/* TODO Ask if user wants to save picture before the session is closed */

	/* Clear graphical memory */
	if(gtkwb->pixmap)
	{
		g_object_unref(gtkwb->pixmap);
		gtkwb->pixmap = NULL;
	}

	if(gtkwb->window)
	{
		gtk_widget_destroy(gtkwb->window);
		gtkwb->window = NULL;
	}
	g_free(gtkwb);
	wb->ui_data = NULL;
}

static gboolean whiteboard_close_cb(GtkWidget *widget, GdkEvent *event, PidginWhiteboard *gtkwb)
{
	PurpleWhiteboard *wb;

	g_return_val_if_fail(gtkwb != NULL, FALSE);
	wb = gtkwb->wb;
	g_return_val_if_fail(wb != NULL, FALSE);

	purple_whiteboard_destroy(wb);

	return FALSE;
}

/*
 * Whiteboard start button on conversation window (move this code to gtkconv?
 * and use new prpl_info member?)
 */
#if 0
static void PidginWhiteboard_button_start_press(GtkButton *button, gpointer data)
{
	PurpleConversation *conv = data;
	PurpleAccount *account = purple_conversation_get_account(conv);
	PurpleConnection *gc = purple_account_get_connection(account);
	char *to = (char*)(purple_conversation_get_name(conv));

	/* Only handle this if local client requested Doodle session (else local
	 * client would have sent one)
	 */
	PurpleWhiteboard *wb = purple_whiteboard_get(account, to);

	/* Write a local message to this conversation showing that a request for a
	 * Doodle session has been made
	 */
	/* XXXX because otherwise gettext will see this string, even though it's
	 * in an #if 0 block. Remove the XXXX if you want to use this code.
	 * But, it really shouldn't be a Yahoo-specific string. ;) */
	purple_conv_im_write(PURPLE_CONV_IM(conv), "", XXXX_("Sent Doodle request."),
					   PURPLE_MESSAGE_NICK | PURPLE_MESSAGE_RECV, time(NULL));

	yahoo_doodle_command_send_request(gc, to);
	yahoo_doodle_command_send_ready(gc, to);

	/* Insert this 'session' in the list.  At this point, it's only a requested
	 * session.
	 */
	wb = purple_whiteboard_create(account, to, DOODLE_STATE_REQUESTING);
}
#endif

static gboolean pidgin_whiteboard_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
	PidginWhiteboard *gtkwb = (PidginWhiteboard*)data;

//	GdkPixmap *pixmap = gtkwb->pixmap;

	if(gtkwb->pixmap)
		g_object_unref(gtkwb->pixmap);

	gtkwb->pixmap = gdk_pixmap_new(widget->window,
							widget->allocation.width,
							widget->allocation.height,
							-1);


	gdk_draw_rectangle(gtkwb->pixmap,
					   widget->style->white_gc,
					   TRUE,
					   0, 0,
					   widget->allocation.width,
					   widget->allocation.height);

    gtkwb->width = widget->allocation.width;
    gtkwb->height= widget->allocation.width;


    gtkwb->rgbbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, widget->allocation.width, widget->allocation.height);
    g_assert(gtkwb->rgbbuf);
    /* set the initial image to white, so it looks like a blank sheet of paper */
    memset(image_buf_rgbbuf(gtkwb), 255 ,
            widget->allocation.width * widget->allocation.width * sizeof(unsigned char));
//    purple_debug_error("gtkwhiteboard", "*** %d %d %u \n",widget->allocation.width , widget->allocation.height, gtkwb->pixmap );
//    purple_debug_error("gtkwhiteboard", "*** %d %d %u \n",gtkwb->width , gtkwb->height, gtkwb->pixmap );
    gtkwb->gc = gdk_gc_new(gtkwb->pixmap);
    g_assert(gtkwb->gc);

    gtkwb->cursor = 0;
    gtkwb->filled = 0;
    gtkwb->flash_state = 0;
    image_buf_set_font(gtkwb, "Sans 12");
    image_buf_rgbbuf_to_pixmap(gtkwb, 0);
    //        image_buf_set_foreground_to_palette(ibuf, black_palette);
    //        image_buf_set_background_to_palette(ibuf, white_palette);
    memset(gtkwb->textbuf, 0, sizeof(gtkwb->textbuf));
    set_drawing_tool(gtkwb, PEN);
	return TRUE;
}

static void pidgin_whiteboard_draw_brush_point(PurpleWhiteboard *wb, int x, int y, int color, int size)
{
    PidginWhiteboard *gtkwb = wb->ui_data;
	GtkWidget *widget = gtkwb->drawing_area;
	GdkPixmap *pixmap = gtkwb->pixmap;
	GdkRectangle update_rect;
/*  */
	GdkGC *gfx_con = gdk_gc_new(pixmap);
	GdkColor col;

	update_rect.x      = x - size / 2;
	update_rect.y      = y - size / 2;
	update_rect.width  = size;
	update_rect.height = size;

	/* Interpret and convert color */
	pidgin_whiteboard_rgb24_to_rgb48(color, &col);

	gdk_gc_set_rgb_fg_color(gfx_con, &col);
	/* gdk_gc_set_rgb_bg_color(gfx_con, &col); */

	/* NOTE 5 is a size constant for now... this is because of how poorly the
	 * gdk_draw_arc draws small circles
	 */
	if(size < 5)
	{
		/* Draw a rectangle/square */
		gdk_draw_rectangle(pixmap,
						   gfx_con,
						   TRUE,
						   update_rect.x, update_rect.y,
						   update_rect.width, update_rect.height);
	}
	else
	{
		/* Draw a circle */
		gdk_draw_arc(pixmap,
					 gfx_con,
					 TRUE,
					 update_rect.x, update_rect.y,
					 update_rect.width, update_rect.height,
					 0, FULL_CIRCLE_DEGREES);
	}
	gtk_widget_queue_draw_area(widget,
							   update_rect.x, update_rect.y,
							   update_rect.width, update_rect.height);

    gdk_gc_unref(gfx_con);
}

/* Uses Bresenham's algorithm (as provided by Wikipedia) */
static void pidgin_whiteboard_draw_brush_line(PurpleWhiteboard *wb, int x0, int y0, int x1, int y1, int color, int size)
{
    /****/
    PidginWhiteboard *gtkwb = wb->ui_data;
	GtkWidget *widget = gtkwb->drawing_area;
	GdkPixmap *pixmap = gtkwb->pixmap;
    GdkGC *gfx_con = gdk_gc_new(pixmap);
    /****/
	int temp;
	int xstep;
	int ystep;
	int dx;
	int dy;
	int error;
	int derror;
	int x;
	int y;

	gboolean steep = abs(y1 - y0) > abs(x1 - x0);

	if(steep)
	{
		temp = x0; x0 = y0; y0 = temp;
		temp = x1; x1 = y1; y1 = temp;
	}

	dx = abs(x1 - x0);
	dy = abs(y1 - y0);

	error = 0;
	derror = dy;

	x = x0;
	y = y0;

	if(x0 < x1)
		xstep = 1;
	else
		xstep = -1;

	if(y0 < y1)
		ystep = 1;
	else
		ystep = -1;

	if(steep)
		pidgin_whiteboard_draw_brush_point(wb, y, x, color, size);
	else
		pidgin_whiteboard_draw_brush_point(wb, x, y, color, size);

	while(x != x1)
	{
		x += xstep;
		error += derror;

		if((error * 2) >= dx)
		{
			y += ystep;
			error -= dx;
		}

		if(steep)
			pidgin_whiteboard_draw_brush_point(wb, y, x, color, size);
		else
			pidgin_whiteboard_draw_brush_point(wb, x, y, color, size);
	}
}

static void pidgin_whiteboard_draw_shape(PurpleWhiteboard *wb, GList *draw_list)
{

    int brush_color;
	int brush_size;
	int x,x1;
	int y,y1 ,i , npoints ;

    PidginWhiteboard *gtkwb = wb->ui_data;
	GtkWidget *widget = gtkwb->drawing_area;
	GdkPixmap *pixmap = gtkwb->pixmap;
    GdkGC *gfx_con = gdk_gc_new(pixmap);
    
    GdkColor col;
    
    purple_whiteboard_get_brush(wb, &gtkwb->brush_size, &gtkwb->brush_color);
    gdk_gc_set_line_attributes(gfx_con, gtkwb->brush_size, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_MITER);
    purple_debug_info("gtkwhiteboard", " at (%d, %d)...\n",gtkwb->brush_size, gtkwb->brush_color);
//    pidgin_whiteboard_rgb24_to_rgb48(gtkwb->brush_color, &col);
//	gdk_gc_set_rgb_fg_color(gfx_con, &col);
//    gdk_gc_set_line_attributes(gfx_con,gtkwb->brush_size,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_BEVEL);

    brush_color = GPOINTER_TO_INT(draw_list->data);
	draw_list = draw_list->next;
	brush_size = GPOINTER_TO_INT(draw_list->data);
	draw_list = draw_list->next;
	g_return_if_fail(draw_list != NULL);

    int shape=  GPOINTER_TO_INT(draw_list->data);
	draw_list = draw_list->next;
	g_return_if_fail(draw_list != NULL);
    gboolean filled;
    int line_width,line_color;
    switch(shape){
        case LINE_SHAPE:
             line_width = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             line_color = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             pidgin_whiteboard_rgb24_to_rgb48(line_color, &col);
             gdk_gc_set_rgb_fg_color(gfx_con, &col);
             gdk_gc_set_line_attributes(gfx_con,line_width,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_BEVEL);

             x = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             y = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             x1 = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             y1 = GPOINTER_TO_INT(draw_list->data);
             gdk_draw_line(pixmap,gfx_con,x,y,x1,y1);
             gdk_draw_line(gtkwb->drawing_area->window,gfx_con,x,y,x1,y1);
             break;
        case RECTANGLE_SHAPE:
             filled = (gboolean)GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             line_width = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             line_color = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             pidgin_whiteboard_rgb24_to_rgb48(line_color, &col);
             gdk_gc_set_rgb_fg_color(gfx_con, &col);
             gdk_gc_set_line_attributes(gfx_con,line_width,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_BEVEL);

             x = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             y = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             x1 = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             y1 = GPOINTER_TO_INT(draw_list->data);
             gdk_draw_rectangle(pixmap,gfx_con,filled,x,y,abs(x1-x),abs(y1-y));
             gdk_draw_rectangle(gtkwb->drawing_area->window,gfx_con,filled,x,y,abs(x1-x),abs(y1-y));
             break;
        case ARC_SHAPE:
             filled = (gboolean)GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             line_width = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             line_color = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             pidgin_whiteboard_rgb24_to_rgb48(line_color, &col);
             gdk_gc_set_rgb_fg_color(gfx_con, &col);
             gdk_gc_set_line_attributes(gfx_con,line_width,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_BEVEL);

             x = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             y = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             int width = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             int height = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             int angle1 = GPOINTER_TO_INT(draw_list->data);
             draw_list = draw_list->next;
             int angle2 = GPOINTER_TO_INT(draw_list->data);
             gdk_draw_arc(pixmap,gfx_con,filled,x,y,width, height,angle1,angle2);
             gdk_draw_arc(gtkwb->drawing_area->window,gfx_con,filled,x,y,width, height,angle1,angle2);
             break;
        case TEXT_SHAPE:
              line_width = GPOINTER_TO_INT(draw_list->data);
              draw_list = draw_list->next;
              line_color = GPOINTER_TO_INT(draw_list->data);
              draw_list = draw_list->next;
              pidgin_whiteboard_rgb24_to_rgb48(line_color, &col);
              gdk_gc_set_rgb_fg_color(gfx_con, &col);
              gdk_gc_set_line_attributes(gfx_con,line_width,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_BEVEL);

              x = GPOINTER_TO_INT(draw_list->data);
              draw_list = draw_list->next;
              y = GPOINTER_TO_INT(draw_list->data);
              draw_list = draw_list->next;
              int text = GPOINTER_TO_INT(draw_list->data);
              purple_debug_info("gtkwhiteboard", " at (%d, %d)... %c \n",x,y,text);
              char st[3];
              sprintf(st,"%c\0",text);
              gdk_draw_string(pixmap,gtkwb->font,gfx_con,x,y,st);
              gdk_draw_string(gtkwb->drawing_area->window,gtkwb->font,gfx_con,x,y,st);
              break;
        case FILL_SHAPE:

              line_width = GPOINTER_TO_INT(draw_list->data);
              draw_list = draw_list->next;
              line_color = GPOINTER_TO_INT(draw_list->data);
              draw_list = draw_list->next;
              pidgin_whiteboard_rgb24_to_rgb48_fill(line_color, &col);

              x = GPOINTER_TO_INT(draw_list->data);
              draw_list = draw_list->next;
              y = GPOINTER_TO_INT(draw_list->data);
              purple_debug_error("gtkwhiteboard", " in fill color at %d %d\n",x,y );
              image_buf_flood_fill(gtkwb, x, y, &col);
              break;

        default:
              purple_debug_error("gtkwhiteboard", "unknown option %d \n",shape );
    }
    return; 
}

static void pidgin_whiteboard_draw_line(PurpleWhiteboard *wb, int x0, int y0, int x1, int y1, int color, int size)
{
    PidginWhiteboard *gtkwb = wb->ui_data;
	GtkWidget *widget = gtkwb->drawing_area;
	GdkPixmap *pixmap = gtkwb->pixmap;
    GdkGC *gfx_con = gdk_gc_new(pixmap);
    GdkColor col;

	pidgin_whiteboard_rgb24_to_rgb48(gtkwb->brush_color, &col);
	gdk_gc_set_rgb_fg_color(gfx_con, &col);
    gdk_gc_set_line_attributes(gfx_con,gtkwb->brush_size,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_BEVEL);

    GList *draw_list = wb->draw_list;
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(LINE_SHAPE));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(gtkwb->brush_size));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(gtkwb->brush_color));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(x0));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(y0));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(x1));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(y1));
    purple_whiteboard_send_draw_list(wb, draw_list,6);
    gdk_draw_line(pixmap,gfx_con,x0,y0,x1,y1);
    gdk_draw_line(gtkwb->drawing_area->window,gfx_con,x0,y0,x1,y1);
    if(draw_list)
        purple_whiteboard_draw_list_destroy(draw_list);
    wb->draw_list = NULL;

    return;
}
static void pidgin_whiteboard_draw_rectangle(PurpleWhiteboard *wb, int x0, int y0, int x1, int y1, int color, int size)
{
    PidginWhiteboard *gtkwb = wb->ui_data;
	GtkWidget *widget = gtkwb->drawing_area;
	GdkPixmap *pixmap = gtkwb->pixmap;
    GdkGC *gfx_con = gdk_gc_new(pixmap);
    GdkColor col;
    
    purple_whiteboard_get_brush(wb, &gtkwb->brush_size, &gtkwb->brush_color);
    gdk_gc_set_line_attributes(gfx_con, gtkwb->brush_size, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_MITER);

	pidgin_whiteboard_rgb24_to_rgb48(gtkwb->brush_color, &col);
	gdk_gc_set_rgb_fg_color(gfx_con, &col);
    gdk_gc_set_line_attributes(gfx_con,gtkwb->brush_size,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_BEVEL);

    GList *draw_list = wb->draw_list;
    draw_list = g_list_append(draw_list, GINT_TO_POINTER( RECTANGLE_SHAPE));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(gtkwb->filled));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(gtkwb->brush_size));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(gtkwb->brush_color));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(x0));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(y0));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(x1));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(y1));
    purple_whiteboard_send_draw_list(wb, draw_list,7);
    gdk_draw_rectangle (pixmap,gfx_con,gtkwb->filled , x0,y0,abs(x1-x0),abs(y1-y0));
    gdk_draw_rectangle(gtkwb->drawing_area->window,gfx_con,gtkwb->filled,x0,y0,abs(x1-x0),abs(y1-y0));
    if(draw_list)
        purple_whiteboard_draw_list_destroy(draw_list);
    wb->draw_list = NULL;
    return;
}

static void pidgin_whiteboard_draw_arc(PurpleWhiteboard *wb,gboolean filled, gint x,gint y,gint width,gint height,gint angle1,gint angle2 )
{
    PidginWhiteboard *gtkwb = wb->ui_data;
    GtkWidget *widget = gtkwb->drawing_area;
    GdkPixmap *pixmap = gtkwb->pixmap;
    GdkGC *gfx_con = gdk_gc_new(pixmap);
    GdkColor col;
    purple_whiteboard_get_brush(wb, &gtkwb->brush_size, &gtkwb->brush_color);
    gdk_gc_set_line_attributes(gfx_con, gtkwb->brush_size, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_MITER);

    pidgin_whiteboard_rgb24_to_rgb48(gtkwb->brush_color, &col);
    gdk_gc_set_rgb_fg_color(gfx_con, &col);
    gdk_gc_set_line_attributes(gfx_con,gtkwb->brush_size,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_BEVEL);

    GList *draw_list = wb->draw_list;
    draw_list = g_list_append(draw_list, GINT_TO_POINTER( ARC_SHAPE));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER((int)filled));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(gtkwb->brush_size));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(gtkwb->brush_color));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(x));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(y));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(width));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(height));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(angle1));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(angle2));
    purple_whiteboard_send_draw_list(wb, draw_list,8);
    gdk_draw_arc(pixmap,gfx_con,filled,x,y,width, height,angle1,angle2);
    gdk_draw_arc(gtkwb->drawing_area->window,gfx_con,filled,x,y,width, height,angle1,angle2);
    if(draw_list)
        purple_whiteboard_draw_list_destroy(draw_list);
    wb->draw_list = NULL;

    return;
}

static void pidgin_whiteboard_draw_text(PurpleWhiteboard *wb, gint x,gint y,guint text)
{
    PidginWhiteboard *gtkwb = wb->ui_data;
	GtkWidget *widget = gtkwb->drawing_area;
	GdkPixmap *pixmap = gtkwb->pixmap;
    GdkGC *gfx_con = gdk_gc_new(pixmap);
    GdkColor col;int i;
    
    purple_whiteboard_get_brush(wb, &gtkwb->brush_size, &gtkwb->brush_color);
    gdk_gc_set_line_attributes(gfx_con, gtkwb->brush_size, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_MITER);

	pidgin_whiteboard_rgb24_to_rgb48(gtkwb->brush_color, &col);
	gdk_gc_set_rgb_fg_color(gfx_con, &col);
    gdk_gc_set_line_attributes(gfx_con,gtkwb->brush_size,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_BEVEL);

    GList *draw_list = wb->draw_list;
    draw_list = g_list_append(draw_list, GINT_TO_POINTER( TEXT_SHAPE));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(gtkwb->brush_size));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(gtkwb->brush_color));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(x));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(y));
//    purple_debug_info("gtkwhiteboard", " at (%d, %d)... %c \n",x,y,text);
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(text));

    purple_whiteboard_send_draw_list(wb, draw_list,11);
    char st[3];
    sprintf(st,"%c",text);
    gdk_draw_string(pixmap,gtkwb->font,gfx_con,x,y,st);
    gdk_draw_string(gtkwb->drawing_area->window,gtkwb->font,gfx_con,x,y,st);
    if(draw_list)
        purple_whiteboard_draw_list_destroy(draw_list);
    wb->draw_list = NULL;
    return;
}

static void pidgin_whiteboard_draw_fill(PurpleWhiteboard *wb,int x,int y )
{   
    PidginWhiteboard *gtkwb = wb->ui_data;
    GtkWidget *widget = gtkwb->drawing_area;
    GdkPixmap *pixmap = gtkwb->pixmap;
    GdkGC *gfx_con = gdk_gc_new(pixmap); 
    GdkColor col;
    
    purple_whiteboard_get_brush(wb, &gtkwb->brush_size, &gtkwb->brush_color);
    gdk_gc_set_line_attributes(gfx_con, gtkwb->brush_size, GDK_LINE_SOLID, GDK_CAP_BUTT, GDK_JOIN_MITER);
    pidgin_whiteboard_rgb24_to_rgb48_fill(gtkwb->brush_color, &col);
    
    GList *draw_list = wb->draw_list;
    draw_list = g_list_append(draw_list, GINT_TO_POINTER( FILL_SHAPE ));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(gtkwb->brush_size));
    purple_debug_info("COLOR", " %d\n", gtkwb->brush_color);
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(gtkwb->brush_color));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(x));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(y));
    purple_whiteboard_send_draw_list(wb, draw_list,13);


    purple_debug_info("draw_fill : ", "RED %d\n", col.red);
    purple_debug_info("draw_fill : ", "GREEN %d\n", col.green);
    purple_debug_info("draw_fill : ", "BLUE %d\n", col.blue);

    image_buf_flood_fill(gtkwb, x, y, &col);

    if(draw_list)
        purple_whiteboard_draw_list_destroy(draw_list);
    wb->draw_list = NULL;
    return;
}

static void pidgin_whiteboard_set_dimensions(PurpleWhiteboard *wb, int width, int height)
{
	PidginWhiteboard *gtkwb = wb->ui_data;

	gtkwb->width = width;
	gtkwb->height = height;
}

static void pidgin_whiteboard_set_brush(PurpleWhiteboard *wb, int size, int color)
{
	PidginWhiteboard *gtkwb = wb->ui_data;

	gtkwb->brush_size = size;
	gtkwb->brush_color = color;
}

static void pidgin_whiteboard_clear(PurpleWhiteboard *wb)
{
	PidginWhiteboard *gtkwb = wb->ui_data;
	GdkPixmap *pixmap = gtkwb->pixmap;
	GtkWidget *drawing_area = gtkwb->drawing_area;

	gdk_draw_rectangle(pixmap,
					   drawing_area->style->white_gc,
					   TRUE,
					   0, 0,
					   drawing_area->allocation.width,
					   drawing_area->allocation.height);

	gtk_widget_queue_draw_area(drawing_area,
							   0, 0,
							   drawing_area->allocation.width,
							   drawing_area->allocation.height);
}

static void pidgin_whiteboard_button_clear_press(GtkWidget *widget, gpointer data)
{
	PidginWhiteboard *gtkwb = (PidginWhiteboard*)(data);
	pidgin_whiteboard_clear(gtkwb->wb);
	pidgin_whiteboard_set_canvas_as_icon(gtkwb);

	/* Do protocol specific clearing procedures */
	purple_whiteboard_send_clear(gtkwb->wb);
}

static void pidgin_whiteboard_button_save_press(GtkWidget *widget, gpointer data)
{
	PidginWhiteboard *gtkwb = (PidginWhiteboard*)(data);
	GdkPixbuf *pixbuf;

	GtkWidget *dialog;

	int result;

#if GTK_CHECK_VERSION(2,4,0) /* FILECHOOSER */
	dialog = gtk_file_chooser_dialog_new (_("Save File"),
										  GTK_WINDOW(gtkwb->window),
										  GTK_FILE_CHOOSER_ACTION_SAVE,
										  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
										  GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
										  NULL);

	/* gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), (gboolean)(TRUE)); */

	/* if(user_edited_a_new_document) */
	{
	/* gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), default_folder_for_saving); */
		gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "whiteboard.jpg");
	}
	/*
	else
		gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dialog), filename_for_existing_document);
	*/
#else
	dialog = gtk_file_selection_new(_("Save File"));
	gtk_file_selection_set_filename(GTK_FILE_SELECTION(dialog), "whiteboard.jpg");
#endif
	result = gtk_dialog_run(GTK_DIALOG(dialog));

	if(result == GTK_RESPONSE_ACCEPT)
	{
		char *filename;

#if GTK_CHECK_VERSION(2,4,0) /* FILECHOOSER */
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
#else
		filename = g_strdup(gtk_file_selection_get_filename(GTK_FILE_SELECTION(dialog)));
#endif
		gtk_widget_destroy(dialog);

		/* Makes an icon from the whiteboard's canvas 'image' */
		pixbuf = gdk_pixbuf_get_from_drawable(NULL,
											  (GdkDrawable*)(gtkwb->pixmap),
											  gdk_drawable_get_colormap(gtkwb->pixmap),
											  0, 0,
											  0, 0,
											  gtkwb->width, gtkwb->height);

		if(gdk_pixbuf_save(pixbuf, filename, "jpeg", NULL, "quality", "100", NULL))
			purple_debug_info("gtkwhiteboard", "File Saved...\n");
		else
			purple_debug_info("gtkwhiteboard", "File not Saved... Error\n");
		g_free(filename);
	}
	else if(result == GTK_RESPONSE_CANCEL)
	{
		gtk_widget_destroy(dialog);

		purple_debug_info("gtkwhiteboard", "File not Saved... Canceled\n");
	}
}

static void pidgin_whiteboard_set_canvas_as_icon(PidginWhiteboard *gtkwb)
{
	GdkPixbuf *pixbuf;

	/* Makes an icon from the whiteboard's canvas 'image' */
	pixbuf = gdk_pixbuf_get_from_drawable(NULL,
										  (GdkDrawable*)(gtkwb->pixmap),
										  gdk_drawable_get_colormap(gtkwb->pixmap),
										  0, 0,
										  0, 0,
										  gtkwb->width, gtkwb->height);

	gtk_window_set_icon((GtkWindow*)(gtkwb->window), pixbuf);
}

static void pidgin_whiteboard_rgb24_to_rgb48(int color_rgb, GdkColor *color)
{
    color->red   = (color_rgb >> 8) | 0xFF;
    color->green = (color_rgb & 0xFF00) | 0xFF;
    color->blue  = ((color_rgb & 0xFF) << 8) | 0xFF;
}

static void pidgin_whiteboard_rgb24_to_rgb48_fill(int color_rgb, GdkColor *color)
{
    color->red   = color_rgb >> 16;
    color->green = (color_rgb & 0xFF00) >> 8;
    color->blue  = color_rgb & 0xFF;
}


static void
change_color_cb(GtkColorSelection *selection, PidginWhiteboard *gtkwb)
{
	GdkColor color;
	int old_size = 5;
	int old_color = 0;
	int new_color;
	PurpleWhiteboard *wb = gtkwb->wb;

	gtk_color_selection_get_current_color(selection, &color);
	new_color = (color.red & 0xFF00) << 8;
	new_color |= (color.green & 0xFF00);
	new_color |= (color.blue & 0xFF00) >> 8;

	purple_whiteboard_get_brush(wb, &old_size, &old_color);
	purple_whiteboard_send_brush(wb, old_size, new_color);

	/// added later
	gtkwb->brush_color  = new_color;
	/// added later
	purple_debug_info("HELLO", "COLOR SELECTED = %d OLD COLR = %d\n", gtkwb->brush_color, old_color);
}


static void color_selection_dialog_destroy(GtkWidget *w, GtkWidget *destroy)
{
	gtk_widget_destroy(destroy);
}

static void color_select_dialog(GtkWidget *widget, PidginWhiteboard *gtkwb)
{
	GdkColor color;
	GtkColorSelectionDialog *dialog;
	
	dialog = (GtkColorSelectionDialog *)gtk_color_selection_dialog_new(_("Select color"));

	g_signal_connect(G_OBJECT(dialog->colorsel), "color-changed",
					G_CALLBACK(change_color_cb), gtkwb);


	gtk_widget_destroy(dialog->cancel_button);
	gtk_widget_destroy(dialog->help_button);

	g_signal_connect(G_OBJECT(dialog->ok_button), "clicked",
					G_CALLBACK(color_selection_dialog_destroy), dialog);

	gtk_color_selection_set_has_palette(GTK_COLOR_SELECTION(dialog->colorsel), TRUE);

	pidgin_whiteboard_rgb24_to_rgb48(gtkwb->brush_color, &color);
	gtk_color_selection_set_current_color(GTK_COLOR_SELECTION(dialog->colorsel), &color);

	gtk_widget_show_all(GTK_WIDGET(dialog));
}
/*
	int temp;
	int xstep;
	int ystep;
	int dx;
	int dy;
	int error;
	int derror;
	int x;
	int y;
    int tdx,tdy;
    GList *draw_list = wb->draw_list;
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(-2));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(x0));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(y0));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(x1));
    draw_list = g_list_append(draw_list, GINT_TO_POINTER(y1));
    
    purple_whiteboard_send_draw_list(wb, draw_list);
    if(draw_list)
        purple_whiteboard_draw_list_destroy(draw_list);
    wb->draw_list = NULL;
    return;
	gboolean steep = abs(y1 - y0) > abs(x1 - x0);
    purple_debug_info("gtkwhiteboard", "drawing line...(%d %d) to (%d %d)\n",x0,y0,x1,y1); 

    if(steep)
	{
		temp = x0; x0 = y0; y0 = temp;
		temp = x1; x1 = y1; y1 = temp;
	}

	dx = abs(x1 - x0);
	dy = abs(y1 - y0);

	error = 0;
	derror = dy;

	x = x0;
	y = y0;

	if(x0 < x1)
		xstep = 1;
	else
		xstep = -1;

	if(y0 < y1)
		ystep = 1;
	else
		ystep = -1;

	if(steep){
		pidgin_whiteboard_draw_brush_point(wb, y, x, color, size);
    }
	else{
		pidgin_whiteboard_draw_brush_point(wb, x, y, color, size);
    }
    LastX=x;
    LastY=y;
    tdx = x;
    tdy = y;
  	while(x != x1)
	{
		x += xstep;
		error += derror;
		if((error * 2) >= dx)
		{
			y += ystep;
			error -= dx;
		}
        if(steep){
//            purple_debug_info("gtkwhiteboard", "(%d %d) \n",tdy,tdx);
            draw_list = g_list_append(draw_list, GINT_TO_POINTER(tdy));
            draw_list = g_list_append(draw_list, GINT_TO_POINTER(tdx));
            pidgin_whiteboard_draw_brush_point(wb, y, x, color, size);
        }
		else{
//            purple_debug_info("gtkwhiteboard", "(%d %d) \n",tdx,tdy);
            draw_list = g_list_append(draw_list, GINT_TO_POINTER(tdx));
            draw_list = g_list_append(draw_list, GINT_TO_POINTER(tdy));
 			pidgin_whiteboard_draw_brush_point(wb, x, y, color, size);
        }
        tdx = x - LastX;
        tdy = y - LastY;
        LastX = x;
        LastY = y;
	}

    purple_whiteboard_send_draw_list(wb, draw_list);

    if(draw_list)
        purple_whiteboard_draw_list_destroy(draw_list);

    wb->draw_list = NULL;
//    draw_list = g_list_append(draw_list, GINT_TO_POINTER(LastX));
//    draw_list = g_list_append(draw_list, GINT_TO_POINTER(LastY));
//    wb->draw_list = draw_list;
    purple_debug_info("gtkwhiteboard", "drawing line. out..\n");
    */

