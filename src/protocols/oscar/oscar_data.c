/*
 * Gaim's oscar protocol plugin
 * This file is the legal property of its developers.
 * Please see the AUTHORS file distributed alongside this file.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "oscar.h"

typedef struct _SnacHandler SnacHandler;

struct _SnacHandler
{
	guint16 family;
	guint16 type;
	aim_rxcallback_t handler;
	guint16 flags;
};

static void
oscar_free_buddyinfo(void *data)
{
	struct buddyinfo *bi = data;
	g_free(bi);
}

/**
 * Allocates a new OscarData and initializes it with default values.
 */
OscarData *
oscar_data_new(void)
{
	OscarData *od;

	od = g_new0(OscarData, 1);

	aim_initsnachash(od);
	od->snacid_next = 0x00000001;
	od->buddyinfo = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, oscar_free_buddyinfo);

	/*
	 * Register all the modules for this session...
	 */
	aim__registermodule(od, misc_modfirst); /* load the catch-all first */
	aim__registermodule(od, service_modfirst);
	aim__registermodule(od, locate_modfirst);
	aim__registermodule(od, buddylist_modfirst);
	aim__registermodule(od, msg_modfirst);
	aim__registermodule(od, adverts_modfirst);
	aim__registermodule(od, invite_modfirst);
	aim__registermodule(od, admin_modfirst);
	aim__registermodule(od, popups_modfirst);
	aim__registermodule(od, bos_modfirst);
	aim__registermodule(od, search_modfirst);
	aim__registermodule(od, stats_modfirst);
	aim__registermodule(od, translate_modfirst);
	aim__registermodule(od, chatnav_modfirst);
	aim__registermodule(od, chat_modfirst);
	aim__registermodule(od, odir_modfirst);
	aim__registermodule(od, bart_modfirst);
	/* missing 0x11 - 0x12 */
	aim__registermodule(od, ssi_modfirst);
	/* missing 0x14 */
	aim__registermodule(od, icq_modfirst);
	/* missing 0x16 */
	aim__registermodule(od, auth_modfirst);
	aim__registermodule(od, email_modfirst);

	return od;
}

/**
 * Logoff and deallocate a session.
 *
 * @param od Session to kill
 */
void
oscar_data_destroy(OscarData *od)
{
	aim_cleansnacs(od, -1);

	while (od->oscar_connections != NULL)
		flap_connection_destroy(od->oscar_connections->data);

	while (od->peer_connections != NULL)
		peer_connection_destroy(od->peer_connections->data,
				PEER_DISCONNECT_LOCAL_CLOSED);

	if (od->handlerlist != NULL)
		aim_clearhandlers(od);

	aim__shutdownmodules(od);

	g_hash_table_destroy(od->buddyinfo);

	g_free(od);
}

int oscar_data_addhandler(OscarData *od, guint16 family, guint16 type, aim_rxcallback_t newhandler, guint16 flags)
{
	SnacHandler *snac_handler;

	gaim_debug_misc("oscar", "Adding handler for %04x/%04x\n", family, type);

	snac_handler = g_new0(SnacHandler, 1);

	snac_handler->family = family;
	snac_handler->type = type;
	snac_handler->flags = flags;
	snac_handler->handler = newhandler;

	od->handlerlist = g_list_prepend(od->handlerlist, snac_handler);

	return 0;
}

void
aim_clearhandlers(OscarData *od)
{
	SnacHandler *snac_handler;

	while (od->handlerlist != NULL)
	{
		snac_handler = od->handlerlist->data;
		od->handlerlist = g_list_remove(od->handlerlist, snac_handler);
		g_free(snac_handler);
	}
	od->handlerlist = NULL;
}

aim_rxcallback_t
aim_callhandler(OscarData *od, guint16 family, guint16 type)
{
	GList *cur;
	SnacHandler *snac_handler;

	for (cur = od->handlerlist; cur != NULL; cur = cur->next)
	{
		snac_handler = cur->data;
		if ((snac_handler->family == family) && (snac_handler->type == type))
			return snac_handler->handler;
	}

	return NULL;
}
