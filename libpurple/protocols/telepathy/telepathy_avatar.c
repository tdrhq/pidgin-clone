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

#include "telepathy_avatar.h"

#include "debug.h"

#include "telepathy_connection.h"
#include "telepathy_contact.h"

#include <string.h>

void
request_avatars_cb (TpConnection *proxy,
                    const GError *error,
                    gpointer user_data,
                    GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "RequestAvatars error: %s\n", error->message);
	}
}

void
get_known_avatar_tokens_cb (TpConnection *proxy,
                            GHashTable *out_Tokens,
                            const GError *error,
                            gpointer user_data,
                            GObject *weak_object)
{
	telepathy_connection *data = user_data;
	GHashTableIter iter;
	guint handle;
	gchar *token;

	if (error != NULL)
	{
		purple_debug_error("telepathy", "GetAvatarTokens error: %s\n", error->message);
		return;
	}

	g_hash_table_iter_init(&iter, out_Tokens);

	/* got throught all known tokens */
	while (g_hash_table_iter_next(&iter, (gpointer)&handle, (gpointer)&token))
	{
		telepathy_contact *contact_data = g_hash_table_lookup(
				data->contacts, (gpointer)handle);

		purple_debug_info("telepathy", "Known token: %u -> (%s)\n", handle, (gchar *)token);	

		/* make sure there's enough information available about the contact */
		if (contact_data && contact_data->contact)
		{
			TpContact *contact = contact_data->contact;

			PurpleBuddy* buddy = purple_find_buddy(data->acct,
					tp_contact_get_identifier(contact));

			PurpleBuddyIcon *icon;

			/* clear the avatar in case of an empty token */
			if (*token == 0)
			{
				purple_buddy_icons_set_for_user(data->acct,
			    			tp_contact_get_identifier(contact),
						NULL, 0, "");
				continue;
			}

			icon = purple_buddy_get_icon(buddy);

			/* is the cached avatar the same with this one? */
			if (g_strcmp0(purple_buddy_icon_get_checksum(icon), token) != 0)
			{
				/* request a new avatar */
				GArray *avatar_handles;

				avatar_handles = g_array_new(FALSE, FALSE, sizeof(guint));

				g_array_append_val(avatar_handles, handle);

				tp_cli_connection_interface_avatars_call_request_avatars(
						data->connection, -1,
						avatar_handles,
						request_avatars_cb, data,
						NULL, NULL);

				g_array_free(avatar_handles, TRUE);
			}

		}
	}



	
}

void
avatar_retrieved_cb (TpConnection *proxy,
                     guint arg_Contact,
                     const gchar *arg_Token,
                     const GArray *arg_Avatar,
                     const gchar *arg_Type,
                     gpointer user_data,
                     GObject *weak_object)
{
	telepathy_connection *data = user_data;
	PurpleBuddyIcon *icon;
	telepathy_contact *contact_data = g_hash_table_lookup(data->contacts, (gpointer)arg_Contact);
	TpContact *contact;
	gpointer avatar;

	if (contact_data == NULL)
	{
		purple_debug_warning("telepathy", "Updated avatar for handle %u which has no contact_data struct cached!\n", arg_Contact);
		return;
	}

	contact = contact_data->contact;

	if (contact == NULL)
	{
		purple_debug_warning("telepathy", "Retrieved avatar for handle %u which has no TpContact proxy cached!\n", arg_Contact);
		return;
	}

	purple_debug_info("telepathy", "Got avatar token for %u: (%s - %s)\n", arg_Contact, arg_Token, arg_Type);	

	avatar = g_memdup(arg_Avatar->data, arg_Avatar->len);

	icon = purple_buddy_icon_new(data->acct, tp_contact_get_identifier(contact), avatar, arg_Avatar->len, arg_Token);
}

void
avatar_updated_cb (TpConnection *proxy,
                   guint arg_Contact,
                   const gchar *arg_New_Avatar_Token,
                   gpointer user_data,
                   GObject *weak_object)
{
	telepathy_connection *data = user_data;
	telepathy_contact *contact_data = g_hash_table_lookup(data->contacts, (gpointer)arg_Contact);
	TpContact *contact;
	PurpleBuddy *buddy;
	PurpleBuddyIcon *icon;

	purple_debug_warning("telepathy", "Avatar token updated for %u: (%s)\n", arg_Contact, arg_New_Avatar_Token);	

	if (contact_data == NULL)
	{
		purple_debug_warning("telepathy", "Updated avatar for handle %u which has no contact_data struct cached!\n", arg_Contact);
		return;
	}

	contact = contact_data->contact;

	if (contact == NULL)
	{
		purple_debug_warning("telepathy", "Updated avatar for handle %u which has no TpContact proxy chaced!\n", arg_Contact);
		return;
	}

	buddy = purple_find_buddy(data->acct, tp_contact_get_identifier(contact));

	if (buddy == NULL)
	{
		purple_debug_warning("telepathy", "AvatarUpdated: There's no buddy named %s\n", tp_contact_get_identifier(contact));
		return;
	}

	/* clear the avatar in case of an empty token */
	if (*arg_New_Avatar_Token == 0)
	{
		purple_buddy_icons_set_for_user(data->acct, tp_contact_get_identifier(contact), NULL, 0, "");
		return;
	}


	icon = purple_buddy_get_icon(buddy);

	if (g_strcmp0(purple_buddy_icon_get_checksum(icon), arg_New_Avatar_Token) != 0)
	{
		/* updated the buddy avatar */
		GArray *avatar_handles;

		purple_debug_info("telepathy", "Avatar for %u (%s) has changed! Calling RequestAvatars\n", arg_Contact, tp_contact_get_identifier(contact));

		avatar_handles = g_array_new(FALSE, FALSE, sizeof(guint));

		g_array_append_val(avatar_handles, arg_Contact);

		tp_cli_connection_interface_avatars_call_request_avatars(data->connection, -1,
				avatar_handles,
				request_avatars_cb, data,
				NULL, NULL);

		g_array_free(avatar_handles, TRUE);
	}
}

void
get_avatar_properties_cb (TpProxy *proxy,
                          GHashTable *out_Properties,
                          const GError *error,
                          gpointer user_data,
                          GObject *weak_object)
{
	telepathy_connection *data = user_data;
	PurplePlugin *plugin = purple_connection_get_prpl(data->gc);
	PurplePluginProtocolInfo *prpl_info = plugin->info->extra_info;

	GStrv formats;

	PurpleBuddyIconSpec icon_spec = NO_BUDDY_ICONS;
	icon_spec.scale_rules = PURPLE_ICON_SCALE_SEND;

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error getting avatar properties: %s\n", error->message);
		return;
	}

	purple_debug_info("telepathy", "Got avatar properties!\n");

	/* fetch primitive types */
	icon_spec.min_width = tp_asv_get_uint32 (out_Properties, "MinimumAvatarWidth", NULL);
	icon_spec.min_height = tp_asv_get_uint32 (out_Properties, "MinimumAvatarHeight", NULL);
	icon_spec.max_width = tp_asv_get_uint32 (out_Properties, "MaximumAvatarWidth", NULL);
	icon_spec.max_height = tp_asv_get_uint32 (out_Properties, "MaximumAvatarHeight", NULL);
	icon_spec.max_filesize = tp_asv_get_uint32 (out_Properties, "MaximumAvatarBytes", NULL);


	/* fetch the list of supported mime types */
	formats = tp_asv_get_boxed (out_Properties, "SupportedMimeTypes", G_TYPE_STRV);

	if (formats != NULL)
	{
		/* this will hold ONLY the extensions */
		GPtrArray *extensions = g_ptr_array_new ();

		gchar **s;
		for (s = formats; *s != NULL; s++)
		{
			/* extract th extension from the MIME type */
			if (g_str_has_prefix (*s, "image/"))
				g_ptr_array_add (extensions, *s + strlen("image/"));
			else
				purple_debug_warning("telepathy", "Unkown mime type %s\n", *s);
		}

		g_ptr_array_add(extensions, NULL);

		icon_spec.format = g_strjoinv (",", (gchar **)extensions->pdata);

		g_ptr_array_free (extensions, FALSE);
	}

	prpl_info->icon_spec = icon_spec;
}

void
set_avatar_cb (TpConnection *proxy,
               const gchar *out_Token,
               const GError *error,
               gpointer user_data,
               GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "SetAvatar error: %s\n", error->message);
	}
}

void
clear_avatar_cb (TpConnection *proxy,
                 const GError *error,
                 gpointer user_data,
                 GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "ClearAvatar error: %s\n", error->message);
	}
}

