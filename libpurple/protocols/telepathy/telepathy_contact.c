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

#include "telepathy_contact.h"

#include "debug.h"

#include "telepathy_avatar.h"
#include "telepathy_channel_list.h"

void
destroy_contact(telepathy_contact *contact_data)
{
	g_object_unref(contact_data->contact);
	g_free(contact_data);
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
	}
	else
	{
		purple_debug_info("telepathy", "Member removed!\n");
	}
}

void
remove_contact_cb (TpConnection *connection,
                   TpHandleType handle_type,
                   guint n_handles,
                   const TpHandle *handles,
                   const gchar * const *ids,
                   const GError *error,
                   gpointer user_data,
                   GObject *weak_object)
{
	telepathy_connection *data = user_data;
	telepathy_group *stored_list;
	GArray *arr;
	int i;

	if (error != NULL)
	{
		purple_debug_error("telepathy",
				"RequestHandle (remove_contact_cb): %s\n", error->message);
		return;
	}

	/* we are going to remove the handle from the stored list */
	stored_list = g_hash_table_lookup(data->lists, "stored");
	
	arr = g_array_new(0, 0, sizeof(TpHandle));

	purple_debug_info("telepathy", "Got handle for removing buddy (%u)!\n",
		handles[0]);

	for (i = 0; i<n_handles; ++i)
	{
	    g_array_append_val(arr, handles[i]);
	}

	if (stored_list == NULL)
	{
		purple_debug_error("telepathy", "There's no stored list cached!\n");
	}
	else
	{
		tp_cli_channel_interface_group_call_remove_members(stored_list->channel, -1,
				arr, NULL,
				remove_members_cb, data,
				NULL, NULL);

	}

	g_array_free(arr, TRUE);
}

static void
add_member_cb (TpChannel *proxy,
               const GError *error,
               gpointer user_data,
               GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "AddMember error: %s\n", error->message);
	}
	else
	{
		purple_debug_info("telepathy", "Member added to %s!\n",
				tp_channel_get_identifier(proxy));
	}
}

void
add_contact_to_group_cb (TpConnection *connection,
                         TpHandleType handle_type,
                         guint n_handles,
                         const TpHandle *handles,
                         const gchar * const *ids,
                         const GError *error,
                         gpointer user_data,
                         GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy",
			"RequestHandle (add_contact_to_group_cb): %s\n", error->message);
	}
	else
	{
		telepathy_group *group = user_data;
		telepathy_group *subscribe_list; 

		const TpContactFeature features[] = {
			TP_CONTACT_FEATURE_ALIAS,
			TP_CONTACT_FEATURE_PRESENCE
		};

		int i;
		GArray *arr = g_array_new(0, 0, sizeof(TpHandle));

		purple_debug_info("telepathy", "Got handle for adding buddy (%u)!\n",
				handles[0]);

		for (i = 0; i<n_handles; ++i)
		{
			g_array_append_val(arr, handles[i]);
		}

		/* we need to add the buddy to a group and also the subscribe list */

		subscribe_list = g_hash_table_lookup(group->connection_data->lists,
				"subscribe");

		if (subscribe_list == NULL)
		{
			purple_debug_error("telepathy",
					"There is no cached subscribe list!\n");
		}
		else
		{
			/* add buddy to the subscribe list */
			tp_cli_channel_interface_group_call_add_members(
					subscribe_list->channel, -1,
					arr, NULL,
					add_member_cb, group->connection_data,
					NULL, NULL);
		}


		/* this will add the buddy to the specified group */
		tp_cli_channel_interface_group_call_add_members(group->channel, -1,
				arr, NULL,
				add_member_cb, group->connection_data,
				NULL, NULL);

		/* this will return a TpContact so we can bind it to a PurpleBuddy */
		tp_connection_get_contacts_by_handle (connection,
				n_handles, handles,
				G_N_ELEMENTS (features), features,
				group_contacts_ready_cb,
				group, NULL, NULL);

		g_array_free(arr, TRUE);
	}
}

static void
contact_notify_alias_cb (TpContact *contact,
		         GParamSpec *pspec,
          	         gpointer user_data)
{
	telepathy_connection *data = user_data;

	const gchar *name = tp_contact_get_identifier(contact);
	const gchar *alias = tp_contact_get_alias(contact);

	PurpleBuddy *buddy = purple_find_buddy(data->acct, tp_contact_get_identifier(contact));

	if (buddy == NULL)
	{
		purple_debug_warning("telepathy", "Received TpContact alias notify"
				" for non-existent buddy (%s)!\n", name);
		return;
	}

	purple_debug_info("telepathy", "%s's alias changed to %s\n", name, alias);

	purple_blist_alias_buddy(buddy, alias);
}

static void
contact_notify_presence_cb (TpContact *contact,
		            GParamSpec *pspec,
		            gpointer user_data)
{
	telepathy_connection *data = user_data;

	const gchar *name = tp_contact_get_identifier(contact);
	const gchar *presence_status = tp_contact_get_presence_status(contact);
	const gchar *presence_message = tp_contact_get_presence_message(contact);

	PurpleBuddy *buddy = purple_find_buddy(data->acct, tp_contact_get_identifier(contact));

	if (buddy == NULL)
	{
		purple_debug_warning("telepathy", "Received TpContact presence notify"
				" for non-existent buddy (%s)!\n", name);
		return;
	}

	purple_debug_info("telepathy", "%s is now %s\n", name, presence_status);

	purple_prpl_got_user_status(data->acct, name, presence_status,
			"message", presence_message, NULL);
}

void
handle_contacts (telepathy_connection *connection_data,
                 guint n_contacts,
		 TpContact * const *contacts,
		 guint n_failed,
		 PurpleGroup *group)
{
	int i;

	purple_debug_info("telepathy", "Contacts ready: %u (%u failed)\n", n_contacts, n_failed);

	for (i = 0; i<n_contacts; ++i)
	{
		TpContact *contact = contacts[i];
		PurpleBuddy *buddy;
		guint handle;
		telepathy_contact *contact_data;

		/* the buddy might already be stored locally */
		buddy = purple_find_buddy(connection_data->acct, tp_contact_get_identifier(contact));

		if (buddy == NULL)
		{
			/* Buddy was not stored locally */
			buddy = purple_buddy_new(connection_data->acct, tp_contact_get_identifier(contact), tp_contact_get_alias(contact));
			purple_blist_add_buddy(buddy, NULL, group, NULL);
		}
		else
		{
			PurpleGroup *buddy_group = purple_buddy_get_group(buddy);
			PurplePresence *presence = purple_buddy_get_presence(buddy);

			/* is this buddy in the right group */
			if (group != NULL && buddy_group != group)
			{
				purple_debug_info("telepathy", "Contact %s is not in the right group, moving him to %s\n",
						tp_contact_get_identifier(contact), purple_group_get_name(group));

				/* we should move the buddy to the right group */
				purple_blist_remove_buddy(buddy);

				buddy = purple_buddy_new(connection_data->acct, tp_contact_get_identifier(contact), tp_contact_get_alias(contact));
				purple_blist_add_buddy(buddy, NULL, group, NULL);
			}


			/* we should check if it has statuses for the presence,
			 * since the prpl was not yet loaded when status_types was being called
			 */
			if (presence != NULL)
			{
				if (purple_presence_get_statuses(presence) == NULL)
				{
					purple_presence_add_list(presence, purple_prpl_get_statuses(connection_data->acct, presence));
				}
			}
		}

		/* save the contact data to be later accessible by using only the handle */
		handle = tp_contact_get_handle(contact);

		contact_data = g_hash_table_lookup(connection_data->contacts, (gpointer)handle);

		if (contact_data == NULL)
		{
			contact_data = g_new0(telepathy_contact, 1);
			contact_data-> contact = contact;

			g_hash_table_insert(connection_data->contacts, (gpointer)handle, contact_data);

			/* The following signals will fire for any changed parameter of the
			 * contact (status, presence, avatar, alias etc.).
			 *
			 * Are both presence-message and presence-status needed?
			 * They always seem to trigger toghether.
			 */
			g_object_ref(contact);

			g_signal_connect(contact, "notify::alias",
					G_CALLBACK (contact_notify_alias_cb), connection_data);

			g_signal_connect(contact, "notify::presence-message",
					G_CALLBACK (contact_notify_presence_cb), connection_data);

			g_signal_connect(contact, "notify::presence-status",
					G_CALLBACK (contact_notify_presence_cb), connection_data);

			contact_notify_alias_cb (contact, NULL, connection_data);
			contact_notify_presence_cb (contact, NULL, connection_data);
		}
	}
}

/* this the ContactsReady callback for group channels */
void
group_contacts_ready_cb (TpConnection *connection,
                         guint n_contacts,
                         TpContact * const *contacts,
                         guint n_failed,
                         const TpHandle *failed,
                         const GError *error,
                         gpointer user_data,
                         GObject *weak_object)
{
	telepathy_group *data = user_data;

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Contacts ready error: %s\n", error->message);
		g_free(data);
	}
	else
	{
		PurpleGroup *group;

		const gchar *group_name = tp_channel_get_identifier(data->channel);
		group = purple_find_group(group_name);

		if (group == NULL)
		{
			group = purple_group_new(group_name);
			purple_blist_add_group(group, NULL);
		}

		handle_contacts(data->connection_data, n_contacts, contacts, n_failed, group);
	}
}

/* this the ContactsReady callback for list channels */
void
contacts_ready_cb (TpConnection *connection,
                   guint n_contacts,
                   TpContact * const *contacts,
                   guint n_failed,
                   const TpHandle *failed,
                   const GError *error,
                   gpointer user_data,
                   GObject *weak_object)
{
	telepathy_group *data = user_data;

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Contacts ready error: %s\n", error->message);
	}
	else
	{
		handle_contacts(data->connection_data, n_contacts, contacts, n_failed, NULL);
	}
}

