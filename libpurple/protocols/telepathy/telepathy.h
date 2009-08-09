/**
 * purple - Telepathy Protocol Plugin
 *
 * Copyright (C) 2009, Felix Kerekes <sttwister@soc.pidgin.im>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 */

#ifndef _TELEPATHY_H_
#define _TELEPATHY_H_

#include <glib.h>

#include <telepathy-glib/account-manager.h>
#include <telepathy-glib/channel-dispatcher.h>
#include <telepathy-glib/connection-manager.h>

#include "plugin.h"

#define TELEPATHY_ID "prpl-telepathy"
#define TELEPATHY_DISPLAY_VERSION "1.0"

void *module_handle;
gchar *module_path;
TpAccountManager *account_Manager;
TpChannelDispatcher *channel_Dispatcher;

/* This maps account object path to account_data struct */
GHashTable *accounts_Hash_Table;

typedef struct
{
	TpConnectionManager *cm;
	TpConnectionManagerProtocol *protocol;
	PurplePlugin *plugin;

} telepathy_data;

#endif /* _TELEPATHY_H_ */
