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

#include "telepathy_account.h"

#include "telepathy_utils.h"

#include <telepathy-glib/account.h>
#include <telepathy-glib/dbus.h>
#include <telepathy-glib/interfaces.h>

#include "account.h"
#include "debug.h"

static void
set_account_parameters (PurpleAccount *account,
                        GHashTable *parameters)
{
	GHashTableIter iter;
	gpointer key, value;

	/* Loop over all parameters */
	g_hash_table_iter_init (&iter, parameters);
	while (g_hash_table_iter_next (&iter, &key, &value)) 
	{
		gchar *name = key;
		GValue *val = value;

		/* Username and password are special properties */
		if (g_strcmp0(name, "account") == 0)
		{
			purple_account_set_username(account, g_value_get_string(val));
		}
		else if (g_strcmp0(name, "password") == 0)
		{
			purple_account_set_password(account, g_value_get_string(val));
		}
		else
		{
			/* Save the parameters in the account */
			if (G_VALUE_HOLDS_BOOLEAN(val))
			{
				purple_account_set_bool(account, name, g_value_get_boolean(val));
			}
			else if (G_VALUE_HOLDS_INT(val))
			{
				purple_account_set_int(account, name, g_value_get_int(val));
			}
			else if (G_VALUE_HOLDS_UINT(val))
			{
				purple_account_set_int(account, name, g_value_get_uint(val));
			}
			else if (G_VALUE_HOLDS_STRING(val))
			{
				purple_account_set_string(account, name, g_value_get_string(val));
			}
			else
			{
				purple_debug_warning("telepathy", "Unknown value type for %s\n",
						name);
			}
		}
	}
}
                        
static void
get_account_properties_cb (TpProxy *proxy,
                           GHashTable *out_Properties,
                           const GError *error,
                           gpointer user_data,
                           GObject *weak_object)
{
	gchar *obj_Path = user_data;

	GHashTable *parameters;
	const gchar *display_name;
	gchar **tokens;
	gchar *cm, *proto, *protocol_id;

	PurpleAccount *account;

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error getting properties for account: %s\n",
				error->message);
		return;
	}

	purple_debug_info("telepathy", "Got account properties!\n");

	parameters = g_value_get_boxed(g_hash_table_lookup(out_Properties, "Parameters"));

	if (parameters == NULL)
	{
		purple_debug_warning("telepathy", "Account has no parameters!\n");
		return;
	}

	display_name = tp_asv_get_string(parameters, "account");

	/* Parse the object path to find the connection manager and the protocol.
	 * The object path looks like "/org/freedesktop/Telepathy/Account/cm/proto/acct"
	 */
	tokens = g_strsplit(obj_Path, "/", 8);

	cm = tokens[5];
	proto = tokens[6];

	protocol_id = g_strdup_printf("%s-%s-%s", TELEPATHY_ID, cm, proto);
	
	g_strfreev(tokens);

	/* Check if the account already exists in purple-land. If not, we need to manually
	 * create it.
	 */
	account = purple_accounts_find(display_name, protocol_id);

	g_free(protocol_id);

	if (account == NULL)
	{
		purple_debug_info("telepathy", "Account %s does not exist in purple-land,"
				" creating it!\n", display_name);

		account = purple_account_new(display_name, protocol_id);
	}
	else
	{
		purple_debug_info("telepathy", "Account %s DOES exist in purple-land\n",
				display_name);
	}

	purple_account_set_string(account, "objpath", obj_Path);

	/* Sync the parameters with PurpleAccount's parameters */
	set_account_parameters(account, parameters);
}

void
get_valid_accounts_cb (TpProxy *proxy,
                       const GValue *out_Value,
                       const GError *error,
                       gpointer user_data,
                       GObject *weak_object)
{
	int i;

	GPtrArray *accounts;
	GError *err = NULL;
	TpDBusDaemon *daemon;

	if (error != NULL)
	{
		purple_debug_error("telepathy", "Error geting valid accounts: %s\n",
				error->message);
		return;
	}

	daemon = tp_dbus_daemon_dup(&err);

	if (err != NULL)
	{
		purple_debug_error("telepathy", "Error dupping DBus daemon: %s\n",
				err->message);
		g_error_free(err);
		return;
	}

	purple_debug_info("telepathy", "Got valid accounts!\n");

	accounts = g_value_get_boxed(out_Value);

	for (i = 0; i<accounts->len; ++i)
	{
		gchar *obj_Path = g_ptr_array_index(accounts, i);
		TpAccount *account;

		purple_debug_info("telepathy", "  %s\n", obj_Path);

		/* Create a new proxy for each account */
		account = tp_account_new(daemon, obj_Path, &err);

		if (err != NULL)
		{
			purple_debug_error("telepathy", "Error creating TpAccount: %s\n",
					err->message);
			g_error_free(err);
			continue;
		}

		/* Get all properties and sync the accounts with libpurple */
		tp_cli_dbus_properties_call_get_all(account, -1, TP_IFACE_ACCOUNT,
				get_account_properties_cb, g_strdup(obj_Path), g_free, NULL);
	}

	if (daemon)
		g_object_unref(daemon);

}
