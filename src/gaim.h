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

#ifndef _GAIM_H_
#define _GAIM_H_

#define XPATCH BAD /* Because Kalla Said So */

#include "connection.h"

/* Globals in main.c */
extern int opt_away;
extern char *opt_away_arg;
extern char *opt_rcfile_arg;
extern int opt_debug;

extern GSList *message_queue;
extern GSList *unread_message_queue;
extern GSList *away_time_queue;

/* Functions in dialogs.c */
extern void show_find_info(GaimConnection *);
extern void show_confirm_del(GaimBuddy *);
extern void show_confirm_del_group(GaimGroup *);
extern void show_confirm_del_blist_chat(GaimBlistChat *);
extern void show_confirm_del_contact(GaimContact *);

/* Functions in gaimrc.c */
extern gint sort_awaymsg_list(gconstpointer, gconstpointer);

/* Functions in idle.c */
extern gint check_idle(gpointer);

#endif /* _GAIM_H_ */
