/*
 * gaim
 *
 * Copyright (C) 1998-1999, Mark Spencer <markster@marko.net>
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

#ifndef _GAIMCONVO_H_
#define _GAIMCONVO_H_

#include <gtk/gtk.h>
#include "gaim.h"


/* we declare all of the global functions for chat and IM windows here, so
 * that it's easy to keep them merged. */

/* chat first */
extern void im_callback(GtkWidget *, struct conversation *);
extern void ignore_callback(GtkWidget *, struct conversation *);
extern void whisper_callback(GtkWidget *, struct conversation *);
extern void invite_callback(GtkWidget *, struct conversation *);

/* now IM */
extern void warn_callback(GtkWidget *, struct conversation *);
extern void block_callback(GtkWidget *, struct conversation *);
extern void add_callback(GtkWidget *, struct conversation *);

/* now both */
extern void info_callback(GtkWidget *, struct conversation *);
extern void do_bold(GtkWidget *, GtkWidget *);
extern void do_italic(GtkWidget *, GtkWidget *);
extern void do_underline(GtkWidget *, GtkWidget *);
extern void do_strike(GtkWidget *, GtkWidget *);
extern void do_small(GtkWidget *, GtkWidget *);
extern void do_normal(GtkWidget *, GtkWidget *);
extern void do_big(GtkWidget *, GtkWidget *);
extern void toggle_font(GtkWidget *, struct conversation *);
extern void toggle_link(GtkWidget *, struct conversation *);
extern void toggle_color(GtkWidget *, struct conversation *);
extern void toggle_loggle(GtkWidget *, struct conversation *);
extern void insert_smiley(GtkWidget *, struct conversation *);
/* sound is handled by set_option */
extern gboolean keypress_callback(GtkWidget *, GdkEventKey *, struct conversation *);
extern void check_spelling( GtkEditable *, gchar *, gint, gint *, gpointer);
extern int entry_key_pressed(GtkWidget *, GtkWidget *);

extern GtkWidget *build_conv_toolbar(struct conversation *);

extern void send_callback(GtkWidget *, struct conversation *);
extern int close_callback(GtkWidget *, struct conversation *);

extern gboolean meify(char *);

#endif
