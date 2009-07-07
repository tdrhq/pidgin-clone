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

static void
add_members_cb (TpChannel *proxy,
                const GError *error,
                gpointer user_data,
                GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "AddMembers error: %s\n", error->message);
		return;
	}
	
	purple_debug_warning("telepathy", "AddMembers succeeded!\n");
}

static void
request_authorization_auth_cb (gpointer user_data)
{
	telepathy_authorization_request *request = user_data;
	GArray *arr = g_array_new(FALSE, FALSE, sizeof(guint));

	purple_debug_info("telepathy", "Accepting request from %u\n", request->handle);

	/* We must add the contact to the publish list */

	g_array_append_val(arr, request->handle);

	tp_cli_channel_interface_group_call_add_members(request->channel, -1,
			arr, NULL,
			add_members_cb, user_data,
			NULL, NULL);

	g_free(request);	
}

static void
remove_members_cb (TpChannel *proxy,
                   const GError *error,
                   gpointer user_data,
                   GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "RemoveMembers error: %s\n", error->message);
		return;
	}
	
	purple_debug_warning("telepathy", "RemoveMembers succeeded!\n");
}

static void
request_authorization_deny_cb (gpointer user_data)
{
	telepathy_authorization_request *request = user_data;
	GArray *arr = g_array_new(FALSE, FALSE, sizeof(guint));

	purple_debug_info("telepathy", "Denying request from %u\n", request->handle);

	/* By removing the handle from the channel's local-pending,
	 * we're actually denying the request.
	 */

	g_array_append_val(arr, request->handle);

	tp_cli_channel_interface_group_call_remove_members(request->channel, -1,
			arr, NULL,
			remove_members_cb, user_data,
			NULL, NULL);

	g_free(request);	
}

static void
request_authorization_cb (TpConnection *connection,
                          guint n_contacts,
                          TpContact * const *contacts,
                          guint n_failed,
                          const TpHandle *failed,
                          const GError *error,
                          gpointer user_data,
                          GObject *weak_object)
{
	telepathy_authorization_request *data = user_data;
	int i;

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error geting contact for"
				"authorization request:%s\n", error->message);
	}

	if (n_failed > 0)
	{
		purple_debug_error("telepathy", "Failed getting TpContact for %u contacts\n",
				n_failed);
	}

	/* Prompt the user for authorization for each contact */
	for (i = 0; i<n_contacts; ++i)
	{
		telepathy_authorization_request *request = g_new0(
				telepathy_authorization_request, 1);

		*request = *data;
		request->handle = tp_contact_get_handle(contacts[i]);

		purple_account_request_authorization(data->connection_data->acct,
				tp_contact_get_identifier(contacts[i]), NULL,
				tp_contact_get_alias(contacts[i]), NULL,
				FALSE, 
				request_authorization_auth_cb,
				request_authorization_deny_cb,
				request);
	}
}

static void
members_changed_cb (TpChannel *proxy,
                    const gchar *arg_Message,
                    const GArray *arg_Added,
                    const GArray *arg_Removed,
                    const GArray *arg_Local_Pending,
                    const GArray *arg_Remote_Pending,
                    guint arg_Actor,
                    guint arg_Reason,
                    gpointer user_data,
                    GObject *weak_object)
{
	purple_debug_info("telepathy", "Members changed for %s! (Reason: %u) (Message: %s)\n",
			tp_channel_get_identifier(proxy), arg_Reason, arg_Message);

	if (arg_Added->len > 0)
	{
		purple_debug_info("telepathy", "  %u added\n",
				arg_Added->len);
	}

	if (arg_Removed->len > 0)
	{
		purple_debug_info("telepathy", "  %u removed\n",
				arg_Removed->len);
	}

	if (arg_Remote_Pending->len > 0)
	{
		purple_debug_info("telepathy", "  %u remote pending\n",
				arg_Remote_Pending->len);
	}

	if (arg_Local_Pending->len > 0)
	{
		telepathy_connection *data = user_data;
		telepathy_authorization_request *request;

		const TpContactFeature features[] = {
			TP_CONTACT_FEATURE_ALIAS,
			TP_CONTACT_FEATURE_PRESENCE
		};

		purple_debug_info("telepathy", "  %u local pending\n",
				arg_Local_Pending->len);

		/* We've got some buddies pending local acception.
		 * First, we request a TpContact for that handle and the promp the user.
		 */

		request = g_new0(telepathy_authorization_request, 1);

		request->connection_data = data;
		request->channel = proxy;

		tp_connection_get_contacts_by_handle(data->connection,
				arg_Local_Pending->len, (const TpHandle*)arg_Local_Pending->data,
				G_N_ELEMENTS (features), features,
				request_authorization_cb, request, g_free, NULL);
	}
}

static void
get_local_pending_members_with_info_cb (TpChannel *proxy,
                                        const GPtrArray *out_Info,
                                        const GError *error,
                                        gpointer user_data,
                                        GObject *weak_object)
{
	telepathy_connection *data = user_data;
	int i;

	if (error != NULL)
	{
		purple_debug_error("telepathy", "GetLocalPending error: %s\n",
				error->message);
		return;
	}

	purple_debug_error("telepathy", "%u contacts are local pending!\n",
			out_Info->len);

	for (i = 0; i<out_Info->len; ++i)
	{
		/* Each object in the array is (uuus) */
		GValueArray *arr = g_ptr_array_index(out_Info, i);

		GValue *val;
		guint handle;
		const gchar *message;

		telepathy_authorization_request *request;

		const TpContactFeature features[] = {
			TP_CONTACT_FEATURE_ALIAS,
			TP_CONTACT_FEATURE_PRESENCE
		};


		/* Extract the handle and message */
		val = g_value_array_get_nth(arr, 0);
		handle = g_value_get_uint(val);

		val = g_value_array_get_nth(arr, 3);
		message = g_value_get_string(val);

		purple_debug_info("telepathy", "Contact %u is local pending\n",
				handle);

		/* We've got some buddies pending local acception.
		 * First, we request a TpContact for that handle and the prompt the user.
		 */

		request = g_new0(telepathy_authorization_request, 1);

		request->connection_data = data;
		request->channel = proxy;

		tp_connection_get_contacts_by_handle(data->connection,
				1, &handle,
				G_N_ELEMENTS (features), features,
				request_authorization_cb, request, g_free, NULL);
	}
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

	GError *error = NULL;

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

	tp_cli_channel_interface_group_connect_to_members_changed(channel, 
			members_changed_cb, data,
			NULL, NULL,
			&error);

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error connecting to MembersChanged: %s\n",
				error->message);
		g_error_free(error);
	}

	/* See if there are any contacts pending local acception */
	tp_cli_channel_interface_group_call_get_local_pending_members_with_info(channel, -1,
			get_local_pending_members_with_info_cb, data, NULL, NULL);
}

