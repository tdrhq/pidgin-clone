/**
 * @file gtkroomlist.h Gtk Room List UI
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

#ifndef _GAIM_GTK_ROOMLIST_H_
#define _GAIM_GTK_ROOMLIST_H_

#include "roomlist.h"


typedef struct _GaimGtkRoomlistDialog GaimGtkRoomlistDialog;

/**
 * Initializes the room list subsystem.
 */
void gaim_gtk_roomlist_init(void);

void gaim_gtk_roomlist_dialog_show(void);
/**
 * Create a new room list dialog.
 *
 * @return The new dialog.
 */
GaimGtkRoomlistDialog *gaim_gtk_roomlist_dialog_new(void);

/**
 * Create a new room list dialog and fetch the list on
 * the specified account.
 *
 * @param account The account to force.
 * @return The new dialog.
 */
GaimGtkRoomlistDialog *gaim_gtk_roomlist_dialog_new_with_account(GaimAccount *account);

#endif /* _GAIM_GTK_ROOMLIST_H_ */
