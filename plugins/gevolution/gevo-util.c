/*
 * Evolution integration plugin for Gaim
 *
 * Copyright (C) 2003 Christian Hammond.
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
#include "gtkinternal.h"
#include "gtkblist.h"
#include "gtkutils.h"

#include <libebook/e-book.h>

void
gevo_add_buddy(GaimAccount *account, const char *group_name,
			   const char *screenname, const char *alias)
{
	GaimConversation *conv = NULL;
	GaimBuddy *buddy;
	GaimGroup *group;

	conv = gaim_find_conversation_with_account(screenname, account);

	if ((group = gaim_find_group(group_name)) == NULL)
	{
		group = gaim_group_new(group_name);
		gaim_blist_add_group(group, NULL);
	}

	buddy = gaim_buddy_new(account, screenname, alias);
	gaim_blist_add_buddy(buddy, NULL, group, NULL);
	serv_add_buddy(gaim_account_get_connection(account), screenname, group);

	if (conv != NULL)
	{
		gaim_buddy_icon_update(gaim_conv_im_get_icon(GAIM_CONV_IM(conv)));
		gaim_conversation_update(conv, GAIM_CONV_UPDATE_ADD);

		gaim_blist_save();
	}
}

GList *
gevo_get_groups(void)
{
	GList *tmp = NULL;
	char *tmp2;
	GaimGroup *g;
	GaimBlistNode *gnode;

	if (gaim_get_blist()->root == NULL)
	{
		tmp2 = g_strdup(_("Buddies"));
		tmp  = g_list_append(tmp, tmp2);
	}
	else
	{
		for (gnode = gaim_get_blist()->root;
			 gnode != NULL;
			 gnode = gnode->next)
		{
			if (GAIM_BLIST_NODE_IS_GROUP(gnode))
			{
				g    = (GaimGroup *)gnode;
				tmp2 = g->name;
				tmp  = g_list_append(tmp, tmp2);
			}
		}
	}

	return tmp;
}

EContactField
gevo_prpl_get_field(GaimAccount *account, GaimBuddy *buddy)
{
	EContactField protocol_field = 0;
	const char *protocol_id;

	g_return_val_if_fail(account != NULL, 0);

	protocol_id = gaim_account_get_protocol_id(account);

	if (!strcmp(protocol_id, "prpl-oscar"))
	{
		GaimConnection *gc;
		GaimPluginProtocolInfo *prpl_info;

		gc = gaim_account_get_connection(account);

		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

		if (!strcmp("aim", prpl_info->list_icon(account, buddy)))
		{
			protocol_field = E_CONTACT_IM_AIM;
		}
		else
			protocol_field = E_CONTACT_IM_ICQ;
	}
	else if (!strcmp(protocol_id, "prpl-msn"))
		protocol_field = E_CONTACT_IM_MSN;
	else if (!strcmp(protocol_id, "prpl-yahoo"))
		protocol_field = E_CONTACT_IM_YAHOO;
	else if (!strcmp(protocol_id, "prpl-jabber"))
		protocol_field = E_CONTACT_IM_JABBER;

	return protocol_field;
}

gboolean
gevo_prpl_is_supported(GaimAccount *account, GaimBuddy *buddy)
{
	return (gevo_prpl_get_field(account, buddy) != 0);
}

gboolean
gevo_load_addressbook(EBook **book, GError **error)
{
	gboolean result;

	g_return_val_if_fail(book != NULL, FALSE);

	*book = e_book_new();

	result = e_book_load_local_addressbook (*book, error);

	if (!result)
	{
		g_object_unref(*book);
		*book = NULL;
	}

	return result;
}
