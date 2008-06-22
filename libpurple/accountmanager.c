/**
 * @file accountmanager.c Account Manager API
 * @ingroup core
 */

/* purple
 *
 * Purple is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
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
#include "internal.h"
#include "purple.h"
#include "marshallers.h"

#include <string.h>

static void schedule_accounts_save(PurpleAccountManager *manager);
static void sync_accounts(PurpleAccountManager *manager);

/******************************************************************************
 * PurpleAccountManager API
 *****************************************************************************/
enum
{
	ACCOUNT_ADDED,
	ACCOUNT_REMOVED,
	ACCOUNT_MANAGER_LAST_SIGNAL
};
static int account_manager_signals[ACCOUNT_MANAGER_LAST_SIGNAL];

struct _PurpleAccountManagerPrivate
{
	GList *accounts;
	gboolean accounts_loaded;
	int save_timer;
};

#define PURPLE_ACCOUNT_MANAGER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), PURPLE_TYPE_ACCOUNT_MANAGER, PurpleAccountManagerPrivate))

G_DEFINE_TYPE(PurpleAccountManager, purple_account_manager, G_TYPE_OBJECT)

static GObject *
purple_account_manager_constructor(GType type, guint n_cprops, GObjectConstructParam *cprops)
{
	static GObject *singleton = NULL;
	if (singleton == NULL)
		singleton = G_OBJECT_CLASS(purple_account_manager_parent_class)->constructor(type, n_cprops, cprops);
	return singleton;
}

static void
purple_account_manager_dispose(GObject *object)
{
	PurpleAccountManager *manager = PURPLE_ACCOUNT_MANAGER(object);
	sync_accounts(manager);
}

static void
purple_account_manager_finalize(GObject *object)
{
	PurpleAccountManagerPrivate *priv = PURPLE_ACCOUNT_MANAGER_GET_PRIVATE(object);
	g_list_free(priv->accounts);
}

static void
purple_account_manager_class_init(PurpleAccountManagerClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	obj_class->constructor = purple_account_manager_constructor;
	obj_class->dispose = purple_account_manager_dispose;
	obj_class->finalize = purple_account_manager_finalize;

	account_manager_signals[ACCOUNT_ADDED] =
		g_signal_new("account-added",
				G_OBJECT_CLASS_TYPE(klass),
				G_SIGNAL_RUN_FIRST,
				G_STRUCT_OFFSET(PurpleAccountManagerClass, account_added),
				NULL, NULL,
				purple_smarshal_VOID__OBJECT,
				G_TYPE_NONE,
				1, PURPLE_TYPE_ACCOUNT);

	account_manager_signals[ACCOUNT_REMOVED] =
		g_signal_new("account-removed",
				G_OBJECT_CLASS_TYPE(klass),
				G_SIGNAL_RUN_FIRST,
				G_STRUCT_OFFSET(PurpleAccountManagerClass, account_removed),
				NULL, NULL,
				purple_smarshal_VOID__OBJECT,
				G_TYPE_NONE,
				1, PURPLE_TYPE_ACCOUNT);

	g_type_class_add_private(klass, sizeof(PurpleAccountManagerPrivate));
}

static void
purple_account_manager_init(PurpleAccountManager *manager)
{
	manager->priv = PURPLE_ACCOUNT_MANAGER_GET_PRIVATE(manager);
	manager->priv->accounts = NULL;
	manager->priv->accounts_loaded = FALSE;
}

PurpleAccountManager *purple_account_manager_get(void)
{
	static PurpleAccountManager *manager = NULL;
	if (manager == NULL)
		manager = g_object_new(PURPLE_TYPE_ACCOUNT_MANAGER, NULL);
	return manager;
}

void purple_account_manager_add_account(PurpleAccountManager *manager, PurpleAccount *account)
{
	if (g_list_find(manager->priv->accounts, account))
		return;

	manager->priv->accounts = g_list_append(manager->priv->accounts, account);
	g_signal_emit(manager, account_manager_signals[ACCOUNT_ADDED], 0, account);

	/* Make sure we save the accounts when something changes */
	g_signal_connect_swapped(G_OBJECT(account), "notify", G_CALLBACK(schedule_accounts_save), manager);
	g_signal_connect_swapped(G_OBJECT(account), "settings-changed", G_CALLBACK(schedule_accounts_save), manager);
}

void purple_account_manager_remove_account(PurpleAccountManager *manager, PurpleAccount *account)
{
	if (!g_list_find(manager->priv->accounts, account))
		return;

	manager->priv->accounts = g_list_remove(manager->priv->accounts, account);
	g_signal_emit(manager, account_manager_signals[ACCOUNT_REMOVED], 0, account);

	g_signal_handlers_disconnect_by_func(G_OBJECT(account),
			G_CALLBACK(schedule_accounts_save), manager);
}

void purple_account_manager_reorder_account(PurpleAccountManager *manager, PurpleAccount *account, int new_index)
{
	gint index;
	GList *l;
	GList *accounts = manager->priv->accounts;

	g_return_if_fail(account != NULL);
	g_return_if_fail(new_index <= g_list_length(accounts));

	index = g_list_index(accounts, account);

	if (index == -1) {
		purple_debug_error("account",
				   "Unregistered account (%s) discovered during reorder!\n",
				   purple_account_get_username(account));
		return;
	}

	l = g_list_nth(accounts, index);

	if (new_index > index)
		new_index--;

	/* Remove the old one. */
	accounts = g_list_delete_link(accounts, l);

	/* Insert it where it should go. */
	accounts = g_list_insert(accounts, account, new_index);
	manager->priv->accounts = accounts;
	schedule_accounts_save(manager);
}

GList *purple_account_manager_get_all_accounts(PurpleAccountManager *manager)
{
	return manager->priv->accounts;
}

/*********************************************************************
 * Reading from disk                                                 *
 *********************************************************************/

static void
parse_settings(xmlnode *node, PurpleAccount *account)
{
	const char *ui;
	xmlnode *child;

	/* Get the UI string, if these are UI settings */
	ui = xmlnode_get_attrib(node, "ui");

	/* Read settings, one by one */
	for (child = xmlnode_get_child(node, "setting"); child != NULL;
			child = xmlnode_get_next_twin(child))
	{
		const char *name, *str_type;
		PurplePrefType type;
		char *data;

		name = xmlnode_get_attrib(child, "name");
		if (name == NULL)
			/* Ignore this setting */
			continue;

		str_type = xmlnode_get_attrib(child, "type");
		if (str_type == NULL)
			/* Ignore this setting */
			continue;

		if (!strcmp(str_type, "string"))
			type = PURPLE_PREF_STRING;
		else if (!strcmp(str_type, "int"))
			type = PURPLE_PREF_INT;
		else if (!strcmp(str_type, "bool"))
			type = PURPLE_PREF_BOOLEAN;
		else
			/* Ignore this setting */
			continue;

		data = xmlnode_get_data(child);
		if (data == NULL)
			/* Ignore this setting */
			continue;

		if (ui == NULL)
		{
			if (type == PURPLE_PREF_STRING)
				purple_account_set_string(account, name, data);
			else if (type == PURPLE_PREF_INT)
				purple_account_set_int(account, name, atoi(data));
			else if (type == PURPLE_PREF_BOOLEAN)
				purple_account_set_bool(account, name,
									  (*data == '0' ? FALSE : TRUE));
		} else {
			if (type == PURPLE_PREF_STRING)
				purple_account_set_ui_string(account, ui, name, data);
			else if (type == PURPLE_PREF_INT)
				purple_account_set_ui_int(account, ui, name, atoi(data));
			else if (type == PURPLE_PREF_BOOLEAN)
				purple_account_set_ui_bool(account, ui, name,
										 (*data == '0' ? FALSE : TRUE));
		}

		g_free(data);
	}
}

static GList *
parse_status_attrs(xmlnode *node, PurpleStatus *status)
{
	GList *list = NULL;
	xmlnode *child;
	PurpleValue *attr_value;

	for (child = xmlnode_get_child(node, "attribute"); child != NULL;
			child = xmlnode_get_next_twin(child))
	{
		const char *id = xmlnode_get_attrib(child, "id");
		const char *value = xmlnode_get_attrib(child, "value");

		if (!id || !*id || !value || !*value)
			continue;

		attr_value = purple_status_get_attr_value(status, id);
		if (!attr_value)
			continue;

		list = g_list_append(list, (char *)id);

		switch (purple_value_get_type(attr_value))
		{
			case PURPLE_TYPE_STRING:
				list = g_list_append(list, (char *)value);
				break;
			case PURPLE_TYPE_INT:
			case PURPLE_TYPE_BOOLEAN:
			{
				int v;
				if (sscanf(value, "%d", &v) == 1)
					list = g_list_append(list, GINT_TO_POINTER(v));
				else
					list = g_list_remove(list, id);
				break;
			}
			default:
				break;
		}
	}

	return list;
}

static void
parse_status(xmlnode *node, PurpleAccount *account)
{
	gboolean active = FALSE;
	const char *data;
	const char *type;
	xmlnode *child;
	GList *attrs = NULL;

	/* Get the active/inactive state */
	data = xmlnode_get_attrib(node, "active");
	if (data == NULL)
		return;
	if (g_ascii_strcasecmp(data, "true") == 0)
		active = TRUE;
	else if (g_ascii_strcasecmp(data, "false") == 0)
		active = FALSE;
	else
		return;

	/* Get the type of the status */
	type = xmlnode_get_attrib(node, "type");
	if (type == NULL)
		return;

	/* Read attributes into a GList */
	child = xmlnode_get_child(node, "attributes");
	if (child != NULL)
	{
		attrs = parse_status_attrs(child,
						purple_account_get_status(account, type));
	}

	purple_account_set_status_list(account, type, active, attrs);

	g_list_free(attrs);
}

static void
parse_statuses(xmlnode *node, PurpleAccount *account)
{
	xmlnode *child;

	for (child = xmlnode_get_child(node, "status"); child != NULL;
			child = xmlnode_get_next_twin(child))
	{
		parse_status(child, account);
	}
}

static void
parse_proxy_info(xmlnode *node, PurpleAccount *account)
{
	PurpleProxyInfo *proxy_info;
	xmlnode *child;
	char *data;

	proxy_info = purple_proxy_info_new();

	/* Use the global proxy settings, by default */
	purple_proxy_info_set_type(proxy_info, PURPLE_PROXY_USE_GLOBAL);

	/* Read proxy type */
	child = xmlnode_get_child(node, "type");
	if ((child != NULL) && ((data = xmlnode_get_data(child)) != NULL))
	{
		if (!strcmp(data, "global"))
			purple_proxy_info_set_type(proxy_info, PURPLE_PROXY_USE_GLOBAL);
		else if (!strcmp(data, "none"))
			purple_proxy_info_set_type(proxy_info, PURPLE_PROXY_NONE);
		else if (!strcmp(data, "http"))
			purple_proxy_info_set_type(proxy_info, PURPLE_PROXY_HTTP);
		else if (!strcmp(data, "socks4"))
			purple_proxy_info_set_type(proxy_info, PURPLE_PROXY_SOCKS4);
		else if (!strcmp(data, "socks5"))
			purple_proxy_info_set_type(proxy_info, PURPLE_PROXY_SOCKS5);
		else if (!strcmp(data, "envvar"))
			purple_proxy_info_set_type(proxy_info, PURPLE_PROXY_USE_ENVVAR);
		else
		{
			purple_debug_error("account", "Invalid proxy type found when "
							 "loading account information for %s\n",
							 purple_account_get_username(account));
		}
		g_free(data);
	}

	/* Read proxy host */
	child = xmlnode_get_child(node, "host");
	if ((child != NULL) && ((data = xmlnode_get_data(child)) != NULL))
	{
		purple_proxy_info_set_host(proxy_info, data);
		g_free(data);
	}

	/* Read proxy port */
	child = xmlnode_get_child(node, "port");
	if ((child != NULL) && ((data = xmlnode_get_data(child)) != NULL))
	{
		purple_proxy_info_set_port(proxy_info, atoi(data));
		g_free(data);
	}

	/* Read proxy username */
	child = xmlnode_get_child(node, "username");
	if ((child != NULL) && ((data = xmlnode_get_data(child)) != NULL))
	{
		purple_proxy_info_set_username(proxy_info, data);
		g_free(data);
	}

	/* Read proxy password */
	child = xmlnode_get_child(node, "password");
	if ((child != NULL) && ((data = xmlnode_get_data(child)) != NULL))
	{
		purple_proxy_info_set_password(proxy_info, data);
		g_free(data);
	}

	/* If there are no values set then proxy_info NULL */
	if ((purple_proxy_info_get_type(proxy_info) == PURPLE_PROXY_USE_GLOBAL) &&
		(purple_proxy_info_get_host(proxy_info) == NULL) &&
		(purple_proxy_info_get_port(proxy_info) == 0) &&
		(purple_proxy_info_get_username(proxy_info) == NULL) &&
		(purple_proxy_info_get_password(proxy_info) == NULL))
	{
		purple_proxy_info_destroy(proxy_info);
		return;
	}

	purple_account_set_proxy_info(account, proxy_info);
}

static void
parse_current_error(xmlnode *node, PurpleAccount *account)
{
	guint type;
	char *type_str = NULL, *description = NULL;
	xmlnode *child;
	PurpleConnectionErrorInfo *current_error = NULL;

	child = xmlnode_get_child(node, "type");
	if (child == NULL || (type_str = xmlnode_get_data(child)) == NULL)
		return;
	type = atoi(type_str);
	g_free(type_str);

	if (type > PURPLE_CONNECTION_ERROR_OTHER_ERROR)
	{
		purple_debug_error("account",
			"Invalid PurpleConnectionError value %d found when "
			"loading account information for %s\n",
			type, purple_account_get_username(account));
		type = PURPLE_CONNECTION_ERROR_OTHER_ERROR;
	}

	child = xmlnode_get_child(node, "description");
	if (child)
		description = xmlnode_get_data(child);
	if (description == NULL)
		description = g_strdup("");

	current_error = g_new0(PurpleConnectionErrorInfo, 1);
	current_error->type = type;
	current_error->description = description;
#warning TODO: Perhaps have a purple_account_set_current_error?
#if 0
	set_current_error(account, current_error);
#endif
}

static PurpleAccount *
parse_account(xmlnode *node)
{
	PurpleAccount *ret;
	xmlnode *child;
	char *protocol_id = NULL;
	char *name = NULL;
	char *data;

	child = xmlnode_get_child(node, "protocol");
	if (child != NULL)
		protocol_id = xmlnode_get_data(child);

	child = xmlnode_get_child(node, "name");
	if (child != NULL)
		name = xmlnode_get_data(child);
	if (name == NULL)
	{
		/* Do we really need to do this? */
		child = xmlnode_get_child(node, "username");
		if (child != NULL)
			name = xmlnode_get_data(child);
	}

	if ((protocol_id == NULL) || (name == NULL))
	{
		g_free(protocol_id);
		g_free(name);
		return NULL;
	}

	ret = purple_account_new(name, _purple_oscar_convert(name, protocol_id)); /* XXX: */
	g_free(name);
	g_free(protocol_id);

	/* Read the password */
	child = xmlnode_get_child(node, "password");
	if ((child != NULL) && ((data = xmlnode_get_data(child)) != NULL))
	{
		purple_account_set_remember_password(ret, TRUE);
		purple_account_set_password(ret, data);
		g_free(data);
	}

	/* Read the alias */
	child = xmlnode_get_child(node, "alias");
	if ((child != NULL) && ((data = xmlnode_get_data(child)) != NULL))
	{
		if (*data != '\0')
			purple_account_set_alias(ret, data);
		g_free(data);
	}

	/* Read the statuses */
	child = xmlnode_get_child(node, "statuses");
	if (child != NULL)
	{
		parse_statuses(child, ret);
	}

	/* Read the userinfo */
	child = xmlnode_get_child(node, "userinfo");
	if ((child != NULL) && ((data = xmlnode_get_data(child)) != NULL))
	{
		purple_account_set_user_info(ret, data);
		g_free(data);
	}

	child = xmlnode_get_child(node, "check-mail");
	if (child != NULL) {
		char *value = xmlnode_get_data(child);
		purple_account_set_check_mail(ret, value[0] == '0' ? FALSE : TRUE);
		g_free(value);
	}

	/* Read an old buddyicon */
	child = xmlnode_get_child(node, "buddyicon");
	if ((child != NULL) && ((data = xmlnode_get_data(child)) != NULL))
	{
		const char *dirname = purple_buddy_icons_get_cache_dir();
		char *filename = g_build_filename(dirname, data, NULL);
		gchar *contents;
		gsize len;

		if (g_file_get_contents(filename, &contents, &len, NULL))
		{
			purple_buddy_icons_set_account_icon(ret, (guchar *)contents, len);
		}
		else
		{
			/* Try to see if the icon got left behind in the old cache. */
			g_free(filename);
			filename = g_build_filename(g_get_home_dir(), ".gaim", "icons", data, NULL);
			if (g_file_get_contents(filename, &contents, &len, NULL)) {
				purple_buddy_icons_set_account_icon(ret, (guchar*)contents, len);
			}
		}

		g_free(filename);
		g_free(data);
	}

	/* Read settings (both core and UI) */
	for (child = xmlnode_get_child(node, "settings"); child != NULL;
			child = xmlnode_get_next_twin(child))
	{
		parse_settings(child, ret);
	}

	/* Read proxy */
	child = xmlnode_get_child(node, "proxy");
	if (child != NULL)
	{
		parse_proxy_info(child, ret);
	}

	/* Read current error */
	child = xmlnode_get_child(node, "current_error");
	if (child != NULL)
	{
		parse_current_error(child, ret);
	}

	return ret;
}

void purple_account_manager_load_accounts(PurpleAccountManager *manager)
{
	xmlnode *node, *child;
	PurpleAccountManagerPrivate *priv = PURPLE_ACCOUNT_MANAGER_GET_PRIVATE(manager);

	priv->accounts_loaded = TRUE;

	node = purple_util_read_xml_from_file("accounts.xml", _("accounts"));

	if (node == NULL)
		return;

	for (child = xmlnode_get_child(node, "account"); child != NULL;
			child = xmlnode_get_next_twin(child))
	{
		PurpleAccount *account;
		account = parse_account(child);
		purple_account_manager_add_account(manager, account);
	}

	xmlnode_free(node);

	_purple_buddy_icons_account_loaded_cb();
}

/*********************************************************************
 * Writing to disk                                                   *
 *********************************************************************/

static xmlnode *
accounts_to_xmlnode(PurpleAccountManager *manager)
{
	xmlnode *node, *child;
	GList *cur;

	node = xmlnode_new("account");
	xmlnode_set_attrib(node, "version", "1.0");

	for (cur = purple_account_manager_get_all_accounts(manager);
			cur != NULL; cur = cur->next)
	{
		child = purple_account_to_xmlnode(PURPLE_ACCOUNT(cur->data));
		xmlnode_insert_child(node, child);
	}

	return node;
}

static void
sync_accounts(PurpleAccountManager *manager)
{
	xmlnode *node;
	char *data;
	PurpleAccountManagerPrivate *priv = PURPLE_ACCOUNT_MANAGER_GET_PRIVATE(manager);

	if (priv->save_timer == 0)
		return;

	if (!priv->accounts_loaded) {
		purple_debug_error("account", "Attempted to save accounts before "
						 "they were read!\n");
		return;
	}

	node = accounts_to_xmlnode(manager);
	data = xmlnode_to_formatted_str(node, NULL);
	purple_util_write_data_to_file("accounts.xml", data, -1);
	g_free(data);
	xmlnode_free(node);

	g_source_remove(priv->save_timer);
	priv->save_timer = 0;
}

static gboolean
save_cb(gpointer data)
{
	sync_accounts(PURPLE_ACCOUNT_MANAGER(data));
	return FALSE;
}

static void
schedule_accounts_save(PurpleAccountManager *manager)
{
	PurpleAccountManagerPrivate *priv = PURPLE_ACCOUNT_MANAGER_GET_PRIVATE(manager);
	if (priv->save_timer == 0)
		priv->save_timer = purple_timeout_add_seconds(5, save_cb, manager);
}

