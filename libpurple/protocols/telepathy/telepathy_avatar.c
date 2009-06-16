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
	if (error != NULL)
	{
		purple_debug_error("telepathy", "GetAvatarTokens error: %s\n", error->message);
	}
	else
	{
		GHashTableIter iter;
		guint key;
		gpointer value;

		g_hash_table_iter_init(&iter, out_Tokens);

		while (g_hash_table_iter_next(&iter, (gpointer)&key, &value))
		{
			purple_debug_info("telepathy", "Known token: %u -> (%s)\n", key, (gchar *)value);	
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
	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error getting avatar properties: %s\n", error->message);
	}
	else
	{
		telepathy_connection *data = user_data;
		PurplePlugin *plugin = purple_connection_get_prpl(data->gc);
		PurplePluginProtocolInfo *prpl_info = plugin->info->extra_info;


		GHashTableIter iter;
		gpointer key, value;

		PurpleBuddyIconSpec icon_spec = NO_BUDDY_ICONS;
		icon_spec.scale_rules = PURPLE_ICON_SCALE_SEND;

		purple_debug_info("telepathy", "Got avatar properties!\n");

		g_hash_table_iter_init(&iter, out_Properties);

		/* iterate over all properties */
		while (g_hash_table_iter_next(&iter, &key, &value))
		{
			gchar *name = key;
			GValue *val = value;

			if (g_strcmp0("SupportedAvatarMIMETypes", name) == 0)
			{
				/* This parameter is of dbus type "as"
				 * It's exposed as a GValue holding a (gchar **)
				 * Or is it now??? wtf!?
				 */

				gchar *format = NULL;

				int i;
				gchar **arr = g_value_get_boxed(val);

				for (i = 0; arr[i] != NULL; ++i)
				{
					const gchar *mime_type = arr[i];

					int j;

					/* We want to get the part after the / 
					 * Split the string using / as a delimiter and use the last part
					 */
					gchar **split = g_strsplit(mime_type, "/", 0);
					gchar *old = format;

					/* j will be the last non-NULL token */
					for (j = 0; split[j] != NULL; ++j);
					--j;


					/* if this is the first type, don't prepend a comma */
					if (i == 0)
						format = g_strdup(split[j]);
					else
						format = g_strdup_printf("%s,%s", format, split[j]);

					g_free(old);
					g_strfreev(split);
				}

				purple_debug_info("telepathy", "    Supported types: %s\n", format);

				icon_spec.format = format;
			}
			else if (g_strcmp0("MinimumAvatarWidth", name) == 0)
			{
				icon_spec.min_width = g_value_get_uint(val);
			}
			else if (g_strcmp0("MinimumAvatarHeight", name) == 0)
			{
				icon_spec.min_height = g_value_get_uint(val);
			}
			else if (g_strcmp0("MaximumAvatarWidth", name) == 0)
			{
				icon_spec.max_width = g_value_get_uint(val);
			}
			else if (g_strcmp0("MaximumAvatarHeight", name) == 0)
			{
				icon_spec.max_height = g_value_get_uint(val);
			}
			else if (g_strcmp0("MaximumAvatarBytes", name) == 0)
			{
				icon_spec.max_filesize = g_value_get_uint(val);
			}
		}

		prpl_info->icon_spec = icon_spec;
	}
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

