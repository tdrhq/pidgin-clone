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

#include "telepathy_channel_list.h"

#include "debug.h"

#include "telepathy_avatar.h"
#include "telepathy_contact.h"

void
destroy_group(telepathy_group *tp_group)
{
	g_free(tp_group);	
}

void
handle_list_channel (TpChannel *channel,
                     telepathy_connection *data)
{
	TpConnection *connection = data->connection;
	const TpIntSet *members;
	TpIntSetIter iter;
	GArray *handles;

	const TpContactFeature features[] = {
		TP_CONTACT_FEATURE_ALIAS,
		TP_CONTACT_FEATURE_PRESENCE
	};

	guint handle;
	guint handle_type;

	handle = tp_channel_get_handle(channel, &handle_type);

	members = tp_channel_group_get_members(channel);

	if (members == NULL)
	{
		purple_debug_error("telepathy", "Error while getting member list\n");
		return;
	}

	iter.set = members;
	iter.element = (guint)(-1);

	handles = tp_intset_to_array (members);

	/* we want to create a TpContact for each member of this channel */
	if (handles->len)
	{
		/* this struct is needed to pass both the connection data and the channel proxy */
		telepathy_group *group = g_new0(telepathy_group, 1);

		group->channel = channel;
		group->connection_data = data;

		if (handle_type == TP_HANDLE_TYPE_GROUP)
		{
			/* this is a user-defined group */
			tp_connection_get_contacts_by_handle (connection,
					handles->len, (const TpHandle *) handles->data,
					G_N_ELEMENTS (features), features,
					group_contacts_ready_cb,
					group, NULL, NULL);
		}
		else
		{
			tp_connection_get_contacts_by_handle (connection,
					handles->len, (const TpHandle *) handles->data,
					G_N_ELEMENTS (features), features,
					contacts_ready_cb,
					group, NULL, NULL);
		}

	}

	tp_cli_connection_interface_avatars_call_get_known_avatar_tokens(data->connection, -1,
			handles,
			get_known_avatar_tokens_cb, data,
			NULL, NULL);

	g_array_free (handles, TRUE);

}

