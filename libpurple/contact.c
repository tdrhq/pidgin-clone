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
#include "contact.h"
#include "dbus-maybe.h"
#include "debug.h"
#include "server.h"
#include "signals.h"
#include "xmlnode.h"

static PurpleBlistNodeClass *parent_class = NULL;

xmlnode *
contact_to_xmlnode(PurpleBlistNode *cnode)
{
	xmlnode *node, *child;
	PurpleContact *contact;
	PurpleBlistNode *bnode;

	contact = PURPLE_CONTACT(cnode);

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
		if (PURPLE_IS_BUDDY(bnode))
		{
			child = buddy_to_xmlnode(bnode);
			xmlnode_insert_child(node, child);
		}
	}

	/* Write contact settings */
	g_hash_table_foreach(purple_blist_node_get_settings(cnode), value_to_xmlnode, node);

	return node;
}

void
parse_contact(PurpleGroup *group, xmlnode *cnode)
{
	PurpleContact *contact = purple_contact_new();
	xmlnode *x;
	const char *alias;

	purple_blist_add_contact(contact, group,
			purple_blist_get_last_child(PURPLE_BLIST_NODE(group)));

	if ((alias = xmlnode_get_attrib(cnode, "alias"))) {
		purple_blist_alias_contact(contact, alias);
	}

	for (x = cnode->child; x; x = x->next) {
		if (x->type != XMLNODE_TYPE_TAG)
			continue;
		if (purple_strequal(x->name, "buddy"))
			parse_buddy(group, contact, x);
		else if (purple_strequal(x->name, "setting"))
			parse_setting(PURPLE_BLIST_NODE(contact), x);
	}

	/* if the contact is empty, don't keep it around.  it causes problems */
	if (!(PURPLE_BLIST_NODE(contact))->child)
		purple_blist_remove_contact(contact);
}

void
purple_contact_compute_priority_buddy(PurpleContact *contact)
{
	PurpleBlistNode *bnode;
	PurpleBuddy *new_priority = NULL;

	g_return_if_fail(contact != NULL);

	contact->priority = NULL;
	for (bnode = (PURPLE_BLIST_NODE(contact))->child;
			bnode != NULL;
			bnode = bnode->next)
	{
		PurpleBuddy *buddy;

		if (!PURPLE_IS_BUDDY(bnode))
			continue;

		buddy = (PurpleBuddy*)bnode;
		if (new_priority == NULL)
		{
			new_priority = buddy;
			continue;
		}

		if (purple_account_is_connected(purple_buddy_get_account(buddy)))
		{
			int cmp = 1;
			if (purple_account_is_connected(purple_buddy_get_account(new_priority)))
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

	if (ops && ops->save_node)
		ops->save_node(PURPLE_BLIST_NODE(contact));

	if (ops && ops->update)
		ops->update(PURPLE_BLIST_NODE(contact));

	for(bnode = (PURPLE_BLIST_NODE(contact))->child; bnode != NULL; bnode = bnode->next)
	{
		PurpleBuddy *buddy = (PurpleBuddy *)bnode;

		conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, purple_buddy_get_name(buddy),
												   purple_buddy_get_account(buddy));
		if (conv)
			purple_conversation_autoset_title(conv);
	}

	purple_signal_emit(purple_blist_get_handle(), "blist-node-aliased",
					 contact, old_alias);
	g_free(old_alias);
}

PurpleContact *purple_contact_new()
{
	return g_object_new(PURPLE_CONTACT_TYPE, NULL);
}

PurpleContact *purple_buddy_get_contact(PurpleBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);

	return PURPLE_CONTACT(PURPLE_BLIST_NODE(buddy)->parent);
}

int
purple_contact_get_online(PurpleContact *contact)
{
	g_return_val_if_fail(contact, 0);
	return contact->online;
}

void
purple_contact_set_online(PurpleContact *contact, int count)
{
	g_return_if_fail(contact);
	contact->online = count;
}

int
purple_contact_get_currentsize(PurpleContact *contact)
{
	g_return_val_if_fail(contact, 0);
	return contact->currentsize;
}

void
purple_contact_buddy_status_update(PurpleContact *contact, PurpleStatus *status, PurpleStatus *old_status)
{

	g_return_if_fail(contact);

	if (purple_status_is_online(status) &&
		!purple_status_is_online(old_status)) {

		if (++(contact->online) == 1)
			PURPLE_GROUP(contact->node.parent)->online++;
	} else if (!purple_status_is_online(status) &&
				purple_status_is_online(old_status)) {

		if (--(contact->online) == 0)
			PURPLE_GROUP(contact->node.parent)->online--;
	}
}

void
purple_contact_destroy(PurpleContact *contact)
{
	/* This function is only a hack for api breakage */
	g_return_if_fail(PURPLE_IS_CONTACT(contact));
	g_object_unref(G_OBJECT(contact));
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

		if (! PURPLE_IS_BUDDY(bnode))
			continue;

		buddy = (PurpleBuddy *)bnode;
		if (purple_buddy_get_account(buddy)== account)
			return TRUE;
	}
	return FALSE;
}

GList *purple_contact_get_buddies(PurpleContact *contact)
{
	GList *buddies = NULL;
	PurpleBlistNode *cnode;
	PurpleBlistNode *itr;

	g_return_val_if_fail(contact, NULL);
	cnode = PURPLE_BLIST_NODE(contact);

	for(itr = cnode->child;itr;itr = itr->next){
		if(PURPLE_IS_BUDDY(itr)){
			buddies = g_list_append(buddies,itr);
		}
	}

	return buddies;
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

static void
purple_contact_add_buddy_child(PurpleBlistNode *parent, PurpleBlistNode *child)
{

}

static void
purple_contact_add_buddy_sibling(PurpleBlistNode *child, PurpleBlistNode *location)
{

}

static void
purple_contact_remove_buddy(PurpleBlistNode *child)
{
	PurpleContact *contact;
	PurpleBuddy *buddy = PURPLE_BUDDY(child);
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	g_return_if_fail(purple_blist_node_parent(child));
	contact = PURPLE_CONTACT(purple_blist_node_parent(child));

	if (PURPLE_BUDDY_IS_ONLINE(buddy))
		contact->online--;
	if (purple_account_is_connected(purple_buddy_get_account(buddy)))
		contact->currentsize--;
	contact->totalsize--;

	/* emit a node-updated signal here */

#warning The ui calls havent been thought out yet, as to where they ought to go.
	/* Re-sort the contact */
	if (purple_blist_node_first_child(PURPLE_BLIST_NODE(contact)) && contact->priority == buddy) {
		purple_contact_invalidate_priority_buddy(contact);
		if (ops && ops->update)
			ops->update(PURPLE_BLIST_NODE(contact));
	}
}

/****************/
/* GObject Code */
/****************/


static void
purple_contact_finalize(GObject *object)
{
	PurpleContact *contact = PURPLE_CONTACT(object);
	g_free(contact->alias);
	PURPLE_DBUS_UNREGISTER_POINTER(contact);
	G_OBJECT_CLASS(parent_class)->finalize(object);
}

static void
purple_contact_class_init(PurpleContactClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);
	parent_class->add_sibling = purple_contact_add_buddy_sibling;
	parent_class->add_child = purple_contact_add_buddy_child;
	parent_class->remove = purple_contact_remove_buddy;

	obj_class->finalize = purple_contact_finalize;
}

static void
purple_contact_init(GTypeInstance *instance, gpointer class)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleContact *contact = PURPLE_CONTACT(instance);

	contact->totalsize = 0;
	contact->currentsize = 0;
	contact->online = 0;

	if (ops && ops->new_node)
		ops->new_node(PURPLE_BLIST_NODE(contact));

	PURPLE_DBUS_REGISTER_POINTER(contact, PurpleContact);
}

GType
purple_contact_get_type(void)
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
									  &info, 0);
	}

	return type;
}
