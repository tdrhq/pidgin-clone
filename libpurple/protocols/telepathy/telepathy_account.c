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

#include "telepathy.h"
#include "telepathy_utils.h"

#include <telepathy-glib/account.h>
#include <telepathy-glib/account-manager.h>
#include <telepathy-glib/connection-manager.h>
#include <telepathy-glib/interfaces.h>
#include <telepathy-glib/util.h>

#include "account.h"
#include "debug.h"
#include "../../../pidgin/gtkaccount.h"

static void
update_parameters_cb (TpAccount *proxy,
                      const gchar **out_Reconnect_Required,
                      const GError *error,
                      gpointer user_data,
                      GObject *weak_object)
{
	if (error != NULL)
	{
		purple_debug_error("telepathy", "UpdateParameters error: %s\n",
				error->message);
		return;
	}

	purple_debug_info("telepathy", "UpdateParameters succeeded!\n");
}

static void
build_parameters_from_purple_account (PurpleAccount *account,
                                      TpConnectionManagerParam *params,
				      GHashTable **out_Params,
				      GPtrArray **out_Unset)
{
	GHashTable *params_hash = g_hash_table_new_full (g_str_hash, g_str_equal,
			NULL, (GDestroyNotify) tp_g_value_slice_free);

	GPtrArray *unset = g_ptr_array_new();
	
	int i;

	/* Loop over all parameters */
	for (i = 0; params[i].name != NULL; ++i)
	{
		gchar *name = params[i].name;
		const gchar *signature = params[i].dbus_signature;

		/* Some protocols might not require username or password, so check them before */
		if (g_strcmp0(name, "account") == 0 && account->username != NULL)
			tp_asv_set_string(params_hash, "account", account->username);
		if (g_strcmp0(name, "password") == 0 && account->password != NULL)
			tp_asv_set_string(params_hash, "password", account->password);

		/* Account and password are handled in particular */
		if (g_strcmp0(name, "account") != 0 && g_strcmp0(name, "password"))
		{
			/* Check the type of the parameter and update the hash table,
			 * or add it to the unset array if it's default
			 */
			if (g_strcmp0(signature, "s") == 0)
			{
				if (g_strcmp0(purple_account_get_string(account, name, ""), ""))
					tp_asv_set_string(params_hash, name,
							purple_account_get_string(account, name, ""));
				else
					g_ptr_array_add(unset, name);
			}
			else if (g_strcmp0(signature, "n") == 0)
			{
				if (purple_account_get_int(account, name, 0) != 0)
					tp_asv_set_int32(params_hash, name,
							purple_account_get_int(account, name, 0));
				else
					g_ptr_array_add(unset, name);
			}
			else if (g_strcmp0(signature, "i") == 0)
			{
				if (purple_account_get_int(account, name, 0) != 0)
					tp_asv_set_int32(params_hash, name,
							purple_account_get_int(account, name, 0));
				else
					g_ptr_array_add(unset, name);
			}
			else if (g_strcmp0(signature, "u") == 0)
			{
				if (purple_account_get_int(account, name, 0) != 0)
					tp_asv_set_uint32(params_hash, name,
							purple_account_get_int(account, name, 0));
				else
					g_ptr_array_add(unset, name);
			}
			else if (g_strcmp0(signature, "q") == 0)
			{
				if (purple_account_get_int(account, name, 0) != 0)
					tp_asv_set_uint32(params_hash, name,
							purple_account_get_int(account, name, 0));
				else
					g_ptr_array_add(unset, name);
			}
			else if (g_strcmp0(signature, "b") == 0)
			{
				tp_asv_set_boolean(params_hash, name,
						purple_account_get_bool(account, name, FALSE));
			}
			else
				purple_debug_warning("telepathy", "Unknown signature \"%s\" for"
						" \"%s\"\n", signature, name);
		}
	}

	g_ptr_array_add(unset, NULL);

	*out_Params = params_hash;
	*out_Unset = unset;
}

static void
save_account_parameters (telepathy_account *account_data,
                         TpConnectionManagerParam *params)
{
	PurpleAccount *account = account_data->account;

	GHashTable *params_hash;
	GPtrArray *unset;
	
	build_parameters_from_purple_account(account, params, &params_hash, &unset);

	/* Upload the new parameters to AccountManager */
	tp_cli_account_call_update_parameters(account_data->tp_account, -1,
			params_hash, (const gchar **)unset->pdata,
			update_parameters_cb, account_data,
			NULL, NULL);

	g_ptr_array_free(unset, TRUE);

	g_hash_table_destroy(params_hash);
}

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
	telepathy_account *account_data = user_data;

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
	tokens = g_strsplit(account_data->obj_Path, "/", 8);

	cm = tokens[5];
	proto = tokens[6];

	protocol_id = g_strdup_printf("%s-%s-%s", TELEPATHY_ID, cm, proto);
	
	account_data->cm = g_strdup(cm);
	account_data->protocol = g_strdup(proto);

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

		if (account == NULL)
		{
			purple_debug_error("telepathy", "Error creating PurpleAccount!\n");
			return;
		}

		purple_account_register(account);
	}
	else
	{
		purple_debug_info("telepathy", "Account %s DOES exist in purple-land\n",
				display_name);
	}

	account_data->account = account;

	purple_account_set_int(account, "tp_account_data", (int)account_data);

	/* Sync the parameters with PurpleAccount's parameters */
	set_account_parameters(account, parameters);
}

static void
account_modified_cb (PurpleAccount *account,
                     gpointer user_data)
{
	telepathy_account *account_data = (telepathy_account*)purple_account_get_int(
			account, "tp_account_data", 0);

	PurplePlugin *plugin;
	telepathy_data *data;

	if (account_data == NULL)
	{
		/* This doesn't seem to be a prpl-telepathy account, so there's nothing to do */
		return;
	}

	/* We need to find the plugin of this account in order to have access to the
	 * Connection Manager proxy and the protocol parameters.
	 */
	plugin = purple_find_prpl(g_strdup_printf("%s-%s-%s", TELEPATHY_ID, 
			account_data->cm, account_data->protocol));

	if (plugin == NULL)
	{
		purple_debug_info("telepathy", "There's no plugin for modifiying PurpleAccount\n");
		return;
	}

	data = plugin->extra;

	/* Save the parameters in AccountManager */
	save_account_parameters(account_data, data->protocol->params);
}

static void
account_destroying_cb (PurpleAccount *account,
                       gpointer user_data)
{
	telepathy_account *account_data;

	/* Save the changes to AccountManager and destroy the alocated struct */
	account_modified_cb(account, user_data);
	
	account_data = (telepathy_account*)purple_account_get_int(
			account, "tp_account_data", 0);

	if (account_data != NULL)
	{
		g_free(account_data->obj_Path);
		g_free(account_data->cm);
		g_free(account_data->protocol);

		g_free(account_data);
	}
}

static void
create_account_cb (TpAccountManager *proxy,
                   const gchar *out_Account,
                   const GError *error,
                   gpointer user_data,
                   GObject *weak_object)
{
	telepathy_account *account_data = user_data;
	TpAccount *tp_account;
	TpDBusDaemon *daemon;
	GError *err = NULL;

	if (error != NULL)
	{
		purple_debug_error("telepathy", "CreateAccount error: %s\n",
				error->message);
		return;
	}

	purple_debug_info("telepathy", "Account created!\n");

	daemon = tp_dbus_daemon_dup(&err);

	if (err != NULL)
	{
		purple_debug_error("telepathy", "Error dupping DBus daemon for new account:%s\n",
				error->message);
		return;
	}

	tp_account = tp_account_new(daemon, out_Account, &err);

	if (err != NULL)
	{
		purple_debug_error("telepathy", "Error creating proxy for new Account:%s\n",
				error->message);
		return;
	}

	account_data->obj_Path = g_strdup((gchar *)out_Account);
	account_data->tp_account = tp_account;
}

static void
account_added_cb (PurpleAccount *account,
                  gpointer user_data)
{
	gchar *protocol_id = g_strndup(purple_account_get_protocol_id(account),
			strlen(TELEPATHY_ID));

	PurplePlugin *plugin;
	telepathy_data *data;
	GHashTable *params_hash;
	GPtrArray *unset;
	telepathy_account *account_data;

	if (g_strcmp0(protocol_id, TELEPATHY_ID) != 0)
	{
		/* This is not a Telepathy account, we don't care about it */
		g_free(protocol_id);
		return;
	}

	g_free(protocol_id);

	purple_debug_info("telepathy", "Telepathy account created!\n");

	plugin = purple_find_prpl(
		purple_account_get_protocol_id(account));

	if (plugin == NULL)
	{
		purple_debug_error("telepathy", "No PurplePlugin for new Telepathy account!\n");
		return;
	}

	data = plugin->extra;

	build_parameters_from_purple_account(account, data->protocol->params,
			&params_hash, &unset);

	account_data = g_new0(telepathy_account, 1);

	account_data->account = account;
	account_data->cm = g_strdup((gchar *)tp_connection_manager_get_name(data->cm));
	account_data->protocol = g_strdup(data->protocol->name);

	purple_account_set_int(account, "tp_account_data", (int)account_data);

	tp_cli_account_manager_call_create_account(account_Manager, -1,
			account_data->cm, account_data->protocol,
			tp_asv_get_string(params_hash, "account"), params_hash,
			NULL,
			create_account_cb, account_data,
			NULL, NULL);

	g_hash_table_destroy(params_hash);
	g_ptr_array_free(unset, TRUE);

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
		telepathy_account *account_data;

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

		account_data = g_new0(telepathy_account, 1);

		account_data->tp_account = account;
		account_data->obj_Path = g_strdup(obj_Path);

		/* Get all properties and sync the accounts with libpurple */
		tp_cli_dbus_properties_call_get_all(account, -1, TP_IFACE_ACCOUNT,
				get_account_properties_cb, account_data, NULL, NULL);

	}

	/* FIXME: Is purple_accounts_get_handle() the right one to pass as the handle?
	 * I honestly have no idea, seems to fail with a NULL :|
	 *
	 * FIXME: account-modified is Pidgin-dependent
	 */
	purple_signal_connect(pidgin_account_get_handle(), "account-modified",
		purple_accounts_get_handle(),
		PURPLE_CALLBACK(account_modified_cb),
		NULL);

	purple_signal_connect(purple_accounts_get_handle(), "account-destroying",
		purple_accounts_get_handle(),
		PURPLE_CALLBACK(account_destroying_cb),
		NULL);

	purple_signal_connect(purple_accounts_get_handle(), "account-added",
		purple_accounts_get_handle(),
		PURPLE_CALLBACK(account_added_cb),
		NULL);
	if (daemon)
		g_object_unref(daemon);

}
