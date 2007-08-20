/**
 * @file gnttextview.h Textview API
 * @ingroup gnt
 */
/*
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef GNT_TEXT_VIEW_H
#define GNT_TEXT_VIEW_H

#include "gntwidget.h"
#include "gnt.h"
#include "gntcolors.h"
#include "gntkeys.h"

#define GNT_TYPE_TEXT_VIEW				(gnt_text_view_get_gtype())
#define GNT_TEXT_VIEW(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), GNT_TYPE_TEXT_VIEW, GntTextView))
#define GNT_TEXT_VIEW_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), GNT_TYPE_TEXT_VIEW, GntTextViewClass))
#define GNT_IS_TEXT_VIEW(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), GNT_TYPE_TEXT_VIEW))
#define GNT_IS_TEXT_VIEW_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), GNT_TYPE_TEXT_VIEW))
#define GNT_TEXT_VIEW_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), GNT_TYPE_TEXT_VIEW, GntTextViewClass))

#define GNT_TEXT_VIEW_FLAGS(obj)				(GNT_TEXT_VIEW(obj)->priv.flags)
#define GNT_TEXT_VIEW_SET_FLAGS(obj, flags)		(GNT_TEXT_VIEW_FLAGS(obj) |= flags)
#define GNT_TEXT_VIEW_UNSET_FLAGS(obj, flags)	(GNT_TEXT_VIEW_FLAGS(obj) &= ~(flags))

typedef struct _GntTextView			GntTextView;
typedef struct _GntTextViewPriv		GntTextViewPriv;
typedef struct _GntTextViewClass		GntTextViewClass;

typedef enum
{
	GNT_TEXT_VIEW_NO_SCROLL     = 1 << 0,
	GNT_TEXT_VIEW_WRAP_CHAR     = 1 << 1,
	GNT_TEXT_VIEW_TOP_ALIGN     = 1 << 2,
} GntTextViewFlag;

struct _GntTextView
{
	GntWidget parent;

	GString *string;
	GList *list;        /* List of GntTextLine */

	GList *tags;       /* A list of tags */

  void * searchinfo;  /* A struct with information on the current search */
	GntTextViewFlag flags;
};

typedef enum
{
	GNT_TEXT_FLAG_NORMAL      = 0,
	GNT_TEXT_FLAG_BOLD        = 1 << 0,
	GNT_TEXT_FLAG_UNDERLINE   = 1 << 1,
	GNT_TEXT_FLAG_BLINK       = 1 << 2,
	GNT_TEXT_FLAG_DIM         = 1 << 3,
	GNT_TEXT_FLAG_HIGHLIGHT   = 1 << 4,
} GntTextFormatFlags;

struct _GntTextViewClass
{
	GntWidgetClass parent;

	void (*gnt_reserved1)(void);
	void (*gnt_reserved2)(void);
	void (*gnt_reserved3)(void);
	void (*gnt_reserved4)(void);
};

G_BEGIN_DECLS

/**
 * 
 *
 * @return
 */
GType gnt_text_view_get_gtype(void);

/* XXX: For now, don't set a textview to have any border.
 *      If you want borders real bad, put it in a box. */
/**
 * 
 *
 * @return
 */
GntWidget * gnt_text_view_new(void);

/* scroll > 0 means scroll up, < 0 means scroll down, == 0 means scroll to the end */
/**
 * 
 * @param view
 * @param scroll
 */
void gnt_text_view_scroll(GntTextView *view, int scroll);

/**
 * 
 * @param view
 * @param text
 * @param flags
 */
void gnt_text_view_append_text_with_flags(GntTextView *view, const char *text, GntTextFormatFlags flags);

/**
 * 
 * @param view
 * @param text
 * @param flags
 * @param tag
 */
void gnt_text_view_append_text_with_tag(GntTextView *view, const char *text, GntTextFormatFlags flags, const char *tag);

/* Move the cursor to the beginning of the next line and resets text-attributes.
 * It first completes the current line with the current text-attributes. */
/**
 * 
 * @param view
 */
void gnt_text_view_next_line(GntTextView *view);

/**
 * 
 * @param flags
 *
 * @return
 */
chtype gnt_text_format_flag_to_chtype(GntTextFormatFlags flags);

/**
 * 
 * @param view
 */
void gnt_text_view_clear(GntTextView *view);

/**
 * 
 * @param view
 *
 * @return
 */
int gnt_text_view_get_lines_below(GntTextView *view);

/**
 * 
 * @param view
 *
 * @return
 */
int gnt_text_view_get_lines_above(GntTextView *view);

/* If text is NULL, then the tag is removed. */
/**
 * 
 * @param view
 * @param name
 * @param text
 * @param all
 *
 * @return
 */
int gnt_text_view_tag_change(GntTextView *view, const char *name, const char *text, gboolean all);

/**
 * 
 * @param view
 * @param widget
 */
void gnt_text_view_attach_scroll_widget(GntTextView *view, GntWidget *widget);

/**
 * 
 * @param view
 * @param widget
 */
void gnt_text_view_attach_pager_widget(GntTextView *view, GntWidget *pager);

/**
 * 
 * @param view
 * @param widget
 */
void gnt_text_view_attach_editor_widget(GntTextView *view, GntWidget *pager);

/**
 * Set a GntTextViewFlag for the textview widget.
 *
 * @param view  The textview widget
 * @param flag  The flag to set
 */
void gnt_text_view_set_flag(GntTextView *view, GntTextViewFlag flag);

/**
 * Appends HTML to a textview with proper formatting.
 *
 * @param view The textview widget
 * @param html The HTML to be appended
 * 
 */
void gnt_text_view_append_html(GntTextView *view, const char *html);

/**
 *
 * Searches a textview for a string and formats those strings with a given format. 
 *
 * @param view The textview
 * @param str The string to search for
 *
 * @return The number of occurences
 *
 * @see gnt_text_view_set_search_format
 * @see gnt_text_view_get_search_format
 */
int gnt_text_view_search(GntTextView *view, const char *str);

/**
 * Set the format to use on strings found in a search
 *
 * @param view The text view
 * @param flags The format to use
 *
 * @see gnt_text_view_get_search_format
 */
void gnt_text_view_set_search_format(GntTextView *view, GntTextFormatFlags flags);

/**
 * Get the format to use on strings found in a search
 *
 * @param view The textview
 *
 * @return The format flags
 *
 * @see gnt_text_view_set_search_format
 */
GntTextFormatFlags gnt_text_view_get_search_format(GntTextView *view);

/**
 *
 * Jump to the nth occurence of the current search string.
 *
 * @param view The textview
 * @param n The number to jump to
 */
void gnt_text_view_search_jump(GntTextView *view, int n);
G_END_DECLS

#endif /* GNT_TEXT_VIEW_H */
