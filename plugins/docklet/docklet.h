/* System tray icon (aka docklet) plugin for Gaim
 * Copyright (C) 2002 Robert McQueen <robot101@debian.org>
 * Copyright (C) 2003 Herman Bloggs <hermanator12002@yahoo.com>
 * Inspired by a similar plugin by:
 *  John (J5) Palmieri <johnp@martianrock.com>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */
#ifndef _DOCKLET_H_
#define _DOCKLET_H_

enum docklet_status {
	offline,
	offline_connecting,
	online,
	online_connecting,
	online_pending,
	away,
	away_pending
};

struct gaim_tray_ops
{
	void (*create)();
	void (*destroy)();
	void (*update_icon)(enum docklet_status);
};

void docklet_clicked(int button_type);
void docklet_embedded();
void docklet_set_tray_ops(struct gaim_tray_ops *ops);
void docklet_flush_queue();

#endif /* _DOCKLET_H_ */
