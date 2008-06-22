/**
 * @file accountmanager.h Account Manager API
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
#ifndef PURPLE_ACCOUNT_MANAGER_H_
#define PURPLE_ACCOUNT_MANAGER_H_

#include <glib.h>
#include <glib-object.h>
#include "account.h"

#define PURPLE_TYPE_ACCOUNT_MANAGER				(purple_account_manager_get_type())
#define PURPLE_ACCOUNT_MANAGER(obj)				(G_TYPE_CHECK_INSTANCE_CAST((obj), PURPLE_TYPE_ACCOUNT_MANAGER, PurpleAccountManager))
#define PURPLE_ACCOUNT_MANAGER_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST((klass), PURPLE_TYPE_ACCOUNT_MANAGER, PurpleAccountManagerClass))
#define PURPLE_IS_ACCOUNT_MANAGER(obj)			(G_TYPE_CHECK_INSTANCE_TYPE((obj), PURPLE_TYPE_ACCOUNT_MANAGER))
#define PURPLE_IS_ACCOUNT_MANAGER_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass), PURPLE_TYPE_ACCOUNT_MANAGER))
#define PURPLE_ACCOUNT_MANAGER_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj), PURPLE_TYPE_ACCOUNT_MANAGER, PurpleAccountManagerClass))

typedef struct _PurpleAccountManager          PurpleAccountManager;
typedef struct _PurpleAccountManagerPrivate   PurpleAccountManagerPrivate;
typedef struct _PurpleAccountManagerClass     PurpleAccountManagerClass;

struct _PurpleAccountManager
{
	GObject gparent;

	PurpleAccountManagerPrivate *priv;

	void (*_purple_reserved[4])(void);
};

struct _PurpleAccountManagerClass
{
	GObjectClass gparent;

	void (*account_added)(PurpleAccountManager *manager, PurpleAccount *);
	void (*account_removed)(PurpleAccountManager *manager, PurpleAccount *);

	void (*_purple_reserved[4])(void);
};

G_BEGIN_DECLS

/**************************************************************************/
/** @name Account Manager */
/**************************************************************************/
/*@{*/

GType purple_account_manager_get_type(void);
PurpleAccountManager *purple_account_manager_get(void);

void purple_account_manager_add_account(PurpleAccountManager *manager, PurpleAccount *account);
void purple_account_manager_remove_account(PurpleAccountManager *manager, PurpleAccount *account);
void purple_account_manager_reorder_account(PurpleAccountManager *manager, PurpleAccount *account, int new_index);
GList *purple_account_manager_get_all_accounts(PurpleAccountManager *manager);

void purple_account_manager_load_accounts(PurpleAccountManager *manager);

/*@}*/
G_END_DECLS

#endif
