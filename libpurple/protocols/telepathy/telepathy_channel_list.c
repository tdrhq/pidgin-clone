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

#include <telepathy-glib/interfaces.h>

#include "debug.h"

#include "telepathy_avatar.h"
#include "telepathy_contact.h"

void
destroy_group(telepathy_group *tp_group)
{
	g_free(tp_group);	
}

void
create_group_channel_cb (TpConnection *proxy,
                         const gchar *out_Channel,
                         GHashTable *out_Properties,
                         const GError *error,
                         gpointer user_data,
                         GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "CreateChannel for group error: %s\n",
				error->message);
		return;
	}

	purple_debug_info("telepathy", "Group channel created: %s\n", out_Channel);
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

	telepathy_group *group;
	gchar *buddy_name;
	const gchar *group_name;

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

	/* this struct is needed to pass both the connection data and the channel proxy */
	group = g_new0(telepathy_group, 1);

	group->channel = channel;
	group->connection_data = data;

	group_name = tp_channel_get_identifier(channel);

	if (handle_type == TP_HANDLE_TYPE_GROUP)
	{
		/* this is a user-defined group */
		if (handles->len)
		{
			tp_connection_get_contacts_by_handle (connection,
					handles->len, (const TpHandle *) handles->data,
					G_N_ELEMENTS (features), features,
					group_contacts_ready_cb,
					group, NULL, NULL);

		}

		/* save the group in a hash table for later use */
		g_hash_table_insert(data->groups, g_strdup(group_name), group);

		/* Check if we need to add any buddy to this group */
		buddy_name = g_hash_table_lookup(data->buddy_to_be_added, group_name);

		if (buddy_name != NULL)
		{                                                                     
			gchar const *ids[] = { buddy_name, NULL };

			purple_debug_info("telepathy", "Adding %s to group %s\n",
				buddy_name, group_name);

			tp_connection_request_handles(data->connection, -1,
					TP_HANDLE_TYPE_CONTACT, ids,
					add_contact_to_group_cb, group,
					NULL, NULL);

			g_hash_table_remove(data->buddy_to_be_added, group_name);
		} 
	}
	else
	{
		if (handles->len)
		{
			tp_connection_get_contacts_by_handle (connection,
					handles->len, (const TpHandle *) handles->data,
					G_N_ELEMENTS (features), features,
					contacts_ready_cb,
					group, NULL, NULL);
		}

		/* save the list in a hash table for later use */
		g_hash_table_insert(data->lists, g_strdup(group_name), group);
	}

	tp_cli_connection_interface_avatars_call_get_known_avatar_tokens(data->connection, -1,
			handles,
			get_known_avatar_tokens_cb, data,
			NULL, NULL);

	g_array_free (handles, TRUE);

}

