/*
 * purple
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
 *
 */
#define _PURPLE_CHAT_C_
#define _BLIST_HELPERS_

#include "internal.h"
#include "blist.h"
#include "conversation.h"
#include "dbus-maybe.h"
#include "debug.h"
#include "notify.h"
#include "prefs.h"
#include "privacy.h"
#include "prpl.h"
#include "server.h"
#include "signals.h"
#include "util.h"
#include "value.h"
#include "xmlnode.h"

xmlnode *
contact_to_xmlnode(PurpleBlistNode *cnode)
{
	xmlnode *node, *child;
	PurpleContact *contact;
	PurpleBlistNode *bnode;

	contact = (PurpleContact *)cnode;

	node = xmlnode_new("contact");

	if (contact->alias != NULL)
	{
		xmlnode_set_attrib(node, "alias", contact->alias);
	}

	/* Write buddies */
	for (bnode = cnode->child; bnode != NULL; bnode = bnode->next)
	{
		if (!PURPLE_BLIST_NODE_SHOULD_SAVE(bnode))
			continue;
		if (PURPLE_BLIST_NODE_IS_BUDDY(bnode))
		{
			child = buddy_to_xmlnode(bnode);
			xmlnode_insert_child(node, child);
		}
	}

	/* Write contact settings */
	g_hash_table_foreach(cnode->settings, value_to_xmlnode, node);

	return node;
}

void
parse_contact(PurpleGroup *group, xmlnode *cnode)
{
	PurpleContact *contact = purple_contact_new();
	xmlnode *x;
	const char *alias;

	purple_blist_add_contact(contact, group,
			purple_blist_get_last_child((PurpleBlistNode*)group));

	if ((alias = xmlnode_get_attrib(cnode, "alias"))) {
		purple_blist_alias_contact(contact, alias);
	}

	for (x = cnode->child; x; x = x->next) {
		if (x->type != XMLNODE_TYPE_TAG)
			continue;
		if (purple_strequal(x->name, "buddy"))
			parse_buddy(group, contact, x);
		else if (purple_strequal(x->name, "setting"))
			parse_setting((PurpleBlistNode*)contact, x);
	}

	/* if the contact is empty, don't keep it around.  it causes problems */
	if (!((PurpleBlistNode*)contact)->child)
		purple_blist_remove_contact(contact);
}

void
purple_contact_compute_priority_buddy(PurpleContact *contact)
{
	PurpleBlistNode *bnode;
	PurpleBuddy *new_priority = NULL;

	g_return_if_fail(contact != NULL);

	contact->priority = NULL;
	for (bnode = ((PurpleBlistNode*)contact)->child;
			bnode != NULL;
			bnode = bnode->next)
	{
		PurpleBuddy *buddy;

		if (!PURPLE_BLIST_NODE_IS_BUDDY(bnode))
			continue;

		buddy = (PurpleBuddy*)bnode;
		if (new_priority == NULL)
		{
			new_priority = buddy;
			continue;
		}

		if (purple_account_is_connected(buddy->account))
		{
			int cmp = 1;
			if (purple_account_is_connected(new_priority->account))
				cmp = purple_presence_compare(purple_buddy_get_presence(new_priority),
						purple_buddy_get_presence(buddy));

			if (cmp > 0 || (cmp == 0 &&
			                purple_prefs_get_bool("/purple/contact/last_match")))
			{
				new_priority = buddy;
			}
		}
	}

	contact->priority = new_priority;
	contact->priority_valid = TRUE;
}

/*****************************************************************************
 * Public API functions                                                      *
 *****************************************************************************/

void purple_blist_alias_contact(PurpleContact *contact, const char *alias)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleConversation *conv;
	PurpleBlistNode *bnode;
	char *old_alias;
	char *new_alias = NULL;

	g_return_if_fail(contact != NULL);

	if ((alias != NULL) && (*alias != '\0'))
		new_alias = purple_utf8_strip_unprintables(alias);

	if (!purple_strings_are_different(contact->alias, new_alias)) {
		g_free(new_alias);
		return;
	}

	old_alias = contact->alias;

	if ((new_alias != NULL) && (*new_alias != '\0'))
		contact->alias = new_alias;
	else {
		contact->alias = NULL;
		g_free(new_alias); /* could be "\0" */
	}

	purple_blist_schedule_save();

	if (ops && ops->update)
		ops->update(purplebuddylist, (PurpleBlistNode *)contact);

	for(bnode = ((PurpleBlistNode *)contact)->child; bnode != NULL; bnode = bnode->next)
	{
		PurpleBuddy *buddy = (PurpleBuddy *)bnode;

		conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, buddy->name,
												   buddy->account);
		if (conv)
			purple_conversation_autoset_title(conv);
	}

	purple_signal_emit(purple_blist_get_handle(), "blist-node-aliased",
					 contact, old_alias);
	g_free(old_alias);
}

PurpleContact *purple_contact_new()
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();

	PurpleContact *contact = g_object_new(PURPLE_CONTACT_TYPE, NULL);
	contact->totalsize = 0;
	contact->currentsize = 0;
	contact->online = 0;
	purple_blist_node_initialize_settings((PurpleBlistNode *)contact);
	((PurpleBlistNode *)contact)->type = PURPLE_BLIST_CONTACT_NODE;

	if (ops && ops->new_node)
		ops->new_node((PurpleBlistNode *)contact);

	PURPLE_DBUS_REGISTER_POINTER(contact, PurpleContact);
	return contact;
}

void
purple_contact_destroy(PurpleContact *contact)
{
	g_hash_table_destroy(contact->node.settings);
	g_free(contact->alias);
	PURPLE_DBUS_UNREGISTER_POINTER(contact);
	g_free(contact);
}

void purple_contact_set_alias(PurpleContact *contact, const char *alias)
{
	purple_blist_alias_contact(contact,alias);
}

const char *purple_contact_get_alias(PurpleContact* contact)
{
	g_return_val_if_fail(contact != NULL, NULL);

	if (contact->alias)
		return contact->alias;

	return purple_buddy_get_alias(purple_contact_get_priority_buddy(contact));
}

gboolean purple_contact_on_account(PurpleContact *c, PurpleAccount *account)
{
	PurpleBlistNode *bnode, *cnode = (PurpleBlistNode *) c;

	g_return_val_if_fail(c != NULL, FALSE);
	g_return_val_if_fail(account != NULL, FALSE);

	for (bnode = cnode->child; bnode; bnode = bnode->next) {
		PurpleBuddy *buddy;

		if (! PURPLE_BLIST_NODE_IS_BUDDY(bnode))
			continue;

		buddy = (PurpleBuddy *)bnode;
		if (buddy->account == account)
			return TRUE;
	}
	return FALSE;
}

void purple_contact_invalidate_priority_buddy(PurpleContact *contact)
{
	g_return_if_fail(contact != NULL);

	contact->priority_valid = FALSE;
}

PurpleBuddy *purple_contact_get_priority_buddy(PurpleContact *contact)
{
	g_return_val_if_fail(contact != NULL, NULL);

	if (!contact->priority_valid)
		purple_contact_compute_priority_buddy(contact);

	return contact->priority;
}

/****************/
/* GObject Code */
/****************/

static void
purple_contact_class_init(PurpleContactClass *klass)
{

}

static void
purple_contact_init(GTypeInstance *instance, gpointer class)
{

}

GType
purple_contact_get_gtype(void)
{
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleContactClass),
			NULL,					/* base_init		*/
			NULL,					/* base_finalize	*/
			(GClassInitFunc)purple_contact_class_init,
			NULL,
			NULL,					/* class_data		*/
			sizeof(PurpleContact),
			0,						/* n_preallocs		*/
			purple_contact_init,					/* instance_init	*/
			NULL					/* value_table		*/
		};

		type = g_type_register_static(PURPLE_BLIST_NODE_TYPE,
									  "PurpleContact",
									  &info, G_TYPE_FLAG_ABSTRACT);
	}

	return type;
}
