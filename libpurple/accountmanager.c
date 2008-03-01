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
#include "purple.h"
#include "marshallers.h"

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
};

#define PURPLE_ACCOUNT_MANAGER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), PURPLE_TYPE_ACCOUNT_MANAGER, PurpleAccountManagerPrivate))

static void
purple_account_manager_class_init(PurpleAccountManagerClass *klass)
{
	account_manager_signals[ACCOUNT_ADDED] =
		g_signal_new("account-added",
				G_OBJECT_CLASS_TYPE(klass),
				G_SIGNAL_RUN_LAST,
				0, NULL, NULL,
#warning FIXME: Change this to __OBJECT when PurpleAccount is a GObject
				purple_smarshal_VOID__POINTER,
				G_TYPE_NONE,
				1, G_TYPE_POINTER);//PURPLE_TYPE_ACCOUNT);

	account_manager_signals[ACCOUNT_REMOVED] =
		g_signal_new("account-removed",
				G_OBJECT_CLASS_TYPE(klass),
				G_SIGNAL_RUN_LAST,
				0, NULL, NULL,
#warning FIXME: Change this to __OBJECT when PurpleAccount is a GObject
				purple_smarshal_VOID__POINTER,
				G_TYPE_NONE,
				1, G_TYPE_POINTER);//PURPLE_TYPE_ACCOUNT);

	g_type_class_add_private(klass, sizeof(PurpleAccountManagerPrivate));
}

static void
purple_account_manager_init(PurpleAccountManager *manager)
{
	manager->priv = PURPLE_ACCOUNT_MANAGER_GET_PRIVATE(manager);
	manager->priv->accounts = NULL;
}

GType purple_account_manager_get_gtype(void)
{
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleAccountManagerClass),
			NULL,
			NULL,
			(GClassInitFunc)purple_account_manager_class_init,
			NULL,
			NULL,
			sizeof(PurpleAccountManager),
			0,
			(GInstanceInitFunc)purple_account_manager_init,
			NULL,
		};

		type = g_type_register_static(G_TYPE_OBJECT,
				"PurpleAccountManager",
				&info, 0);
	}

	return type;
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
}

void purple_account_manager_remove_account(PurpleAccountManager *manager, PurpleAccount *account)
{
	if (!g_list_find(manager->priv->accounts, account))
		return;

	manager->priv->accounts = g_list_remove(manager->priv->accounts, account);
	g_signal_emit(manager, account_manager_signals[ACCOUNT_REMOVED], 0, account);
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
}

GList *purple_account_manager_get_all_accounts(PurpleAccountManager *manager)
{
	return manager->priv->accounts;
}


