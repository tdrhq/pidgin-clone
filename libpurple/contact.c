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

#include "internal.h"
#include "contact.h"
#include "dbus-maybe.h"
#include "debug.h"
#include "server.h"
#include "signals.h"
#include "xmlnode.h"

#define PURPLE_CONTACT_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), PURPLE_CONTACT_TYPE, PurpleContactPrivate))

struct _PurpleContactPrivate {
	char *alias;            /**< The user-set alias of the contact */
	int totalsize;		    /**< The number of buddies in this contact */
	int currentsize;	    /**< The number of buddies in this contact corresponding to online accounts */
	int online;			    /**< The number of buddies in this contact who are currently online */
	PurpleBuddy *priority;    /**< The "top" buddy for this contact */
	gboolean priority_valid; /**< Is priority valid? */
};

static PurpleBlistNodeClass *parent_class = NULL;

xmlnode *
contact_to_xmlnode(PurpleBlistNode *cnode)
{
	xmlnode *node, *child;
	PurpleBlistNode *bnode;
	PurpleContactPrivate *priv;

	g_return_val_if_fail(PURPLE_IS_CONTACT(cnode), NULL);
	priv = PURPLE_CONTACT_GET_PRIVATE(cnode);

	node = xmlnode_new("contact");

	if (priv->alias != NULL)
	{
		xmlnode_set_attrib(node, "alias", priv->alias);
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

	purple_blist_node_add_sibling(PURPLE_BLIST_NODE(contact),
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
		purple_blist_node_remove(PURPLE_BLIST_NODE(contact));
}

void
purple_contact_compute_priority_buddy(PurpleContact *contact)
{
	PurpleBlistNode *bnode;
	PurpleBuddy *new_priority = NULL;
	PurpleContactPrivate *priv;
	g_return_if_fail(PURPLE_IS_CONTACT(contact));
	priv = PURPLE_CONTACT_GET_PRIVATE(contact);

	priv->priority = NULL;
	for (bnode = purple_blist_node_first_child(PURPLE_BLIST_NODE(contact));
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

	priv->priority = new_priority;
	priv->priority_valid = TRUE;
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
	PurpleContactPrivate *priv;
	g_return_if_fail(PURPLE_IS_CONTACT(contact));
	priv = PURPLE_CONTACT_GET_PRIVATE(contact);

	g_return_if_fail(contact != NULL);

	if ((alias != NULL) && (*alias != '\0'))
		new_alias = purple_utf8_strip_unprintables(alias);

	if (!purple_strings_are_different(priv->alias, new_alias)) {
		g_free(new_alias);
		return;
	}

	old_alias = priv->alias;

	if ((new_alias != NULL) && (*new_alias != '\0'))
		priv->alias = new_alias;
	else {
		priv->alias = NULL;
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

int purple_contact_get_totalsize(PurpleContact *contact)
{
	PurpleContactPrivate *priv;
	g_return_val_if_fail(PURPLE_IS_CONTACT(contact), 0);

	priv = PURPLE_CONTACT_GET_PRIVATE(contact);
	return priv->totalsize;
}

void purple_contact_set_totalsize(PurpleContact *contact, int value)
{
	PurpleContactPrivate *priv;
	g_return_if_fail(PURPLE_IS_CONTACT(contact));

	priv = PURPLE_CONTACT_GET_PRIVATE(contact);
	priv->totalsize = value;
}

int
purple_contact_get_online(PurpleContact *contact)
{
	PurpleContactPrivate *priv;
	g_return_val_if_fail(PURPLE_IS_CONTACT(contact), 0);

	priv = PURPLE_CONTACT_GET_PRIVATE(contact);
	return priv->online;
}

void
purple_contact_set_online(PurpleContact *contact, int count)
{
	PurpleContactPrivate *priv;
	g_return_if_fail(PURPLE_IS_CONTACT(contact));

	priv = PURPLE_CONTACT_GET_PRIVATE(contact);
	priv->online = count;
}

int
purple_contact_get_currentsize(PurpleContact *contact)
{
	PurpleContactPrivate *priv;
	g_return_val_if_fail(PURPLE_IS_CONTACT(contact), 0);

	priv = PURPLE_CONTACT_GET_PRIVATE(contact);
	return priv->currentsize;
}

void
purple_contact_set_currentsize(PurpleContact *contact, int value)
{
	PurpleContactPrivate *priv;
	g_return_if_fail(PURPLE_IS_CONTACT(contact));
	priv = PURPLE_CONTACT_GET_PRIVATE(contact);
	priv->currentsize = value;
}

void
purple_contact_buddy_status_update(PurpleContact *contact, PurpleStatus *status, PurpleStatus *old_status)
{
	PurpleContactPrivate *priv;
	g_return_if_fail(PURPLE_IS_CONTACT(contact));
	priv = PURPLE_CONTACT_GET_PRIVATE(contact);

	if (purple_status_is_online(status) &&
		!purple_status_is_online(old_status)) {
#warning: broken, fix with signals
		/* if (++(priv->online) == 1)
			PURPLE_GROUP(contact->node.parent)->online++;*/
	} else if (!purple_status_is_online(status) &&
				purple_status_is_online(old_status)) {

		/*if (--(priv->online) == 0)
			PURPLE_GROUP(contact->node.parent)->online--;*/
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
	PurpleContactPrivate *priv;
	g_return_val_if_fail(PURPLE_IS_CONTACT(contact), NULL);
	priv = PURPLE_CONTACT_GET_PRIVATE(contact);

	if (priv->alias)
		return priv->alias;

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
	PurpleContactPrivate *priv;
	g_return_if_fail(PURPLE_IS_CONTACT(contact));
	priv = PURPLE_CONTACT_GET_PRIVATE(contact);

	priv->priority_valid = FALSE;
}

PurpleBuddy *purple_contact_get_priority_buddy(PurpleContact *contact)
{
	PurpleContactPrivate *priv;
	g_return_val_if_fail(PURPLE_IS_CONTACT(contact), NULL);
	priv = PURPLE_CONTACT_GET_PRIVATE(contact);

	if (!priv->priority_valid)
		purple_contact_compute_priority_buddy(contact);

	return priv->priority;
}

static void
purple_contact_child_updated(PurpleContact *contact, PurpleBuddy *child)
{
	PurpleContactPrivate *priv;
	g_return_if_fail(PURPLE_IS_CONTACT(contact));
	g_return_if_fail(PURPLE_IS_BUDDY(child));
	priv = PURPLE_CONTACT_GET_PRIVATE(contact);

	if(purple_blist_node_is_online(PURPLE_BLIST_NODE(child))){
		priv->online++;
	} else {
		priv->online--;
	}
}

static void
purple_contact_add_helper(PurpleContact *contact, PurpleBuddy *buddy)
{
	g_return_if_fail(PURPLE_IS_CONTACT(contact));
	g_return_if_fail(PURPLE_IS_BUDDY(buddy));

	#warning: gotta be a better way than just repeating this over and over
	purple_signal_connect(purple_blist_node_get_handle(), "node-updated", /* What to connect to */
		contact, /* Object receiving the signal */
		PURPLE_CALLBACK(purple_contact_child_updated), /* Callback function */
		contact /* Data to pass to the callback function */
	);

	purple_contact_child_updated(contact, buddy);
	/* Need to fire a signal for the containing group can do whatever */
}

static void
purple_contact_add_buddy_child(PurpleBlistNode *parent, PurpleBlistNode *child)
{
	g_return_if_fail(PURPLE_IS_CONTACT(parent));
	g_return_if_fail(PURPLE_IS_BUDDY(child));

	purple_contact_add_helper(PURPLE_CONTACT(parent), PURPLE_BUDDY(child));
}

static void
purple_contact_add_buddy_sibling(PurpleBlistNode *child, PurpleBlistNode *location)
{
	PurpleContact *contact;

	g_return_if_fail(PURPLE_IS_CONTACT(purple_blist_node_parent(location)));
	g_return_if_fail(PURPLE_IS_BUDDY(child));

	contact = PURPLE_CONTACT(purple_blist_node_parent(location));

	purple_contact_add_helper(contact, PURPLE_BUDDY(child));
}

static gboolean
purple_contact_is_online(PurpleBlistNode *contact)
{
	g_return_val_if_fail(PURPLE_IS_CONTACT(contact), FALSE);
	return purple_contact_get_online(PURPLE_CONTACT(contact)) > 0;
}

static void
purple_contact_remove_buddy(PurpleBlistNode *child)
{
	PurpleContact *contact;
	PurpleBuddy *buddy = PURPLE_BUDDY(child);
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleContactPrivate *priv;
	g_return_if_fail(PURPLE_IS_CONTACT(purple_blist_node_parent(child)));
	contact = PURPLE_CONTACT(purple_blist_node_parent(child));
	priv = PURPLE_CONTACT_GET_PRIVATE(contact);

	if (PURPLE_BUDDY_IS_ONLINE(buddy))
		priv->online--;
	if (purple_account_is_connected(purple_buddy_get_account(buddy)))
		priv->currentsize--;
	priv->totalsize--;

	/* emit a node-updated signal here */

	purple_signal_emit(purple_blist_node_get_handle(), "node-updated", contact);

#warning The ui calls havent been thought out yet, as to where they ought to go.
	/* Re-sort the contact */
	if (purple_blist_node_first_child(PURPLE_BLIST_NODE(contact)) && priv->priority == buddy) {
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
	PurpleContactPrivate *priv = PURPLE_CONTACT_GET_PRIVATE(object);
	g_free(priv->alias);
	purple_signals_disconnect_by_handle(object);
	PURPLE_DBUS_UNREGISTER_POINTER(object);
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
	parent_class->is_online = purple_contact_is_online;

	g_type_class_add_private(klass, sizeof(PurpleContactPrivate));

	obj_class->finalize = purple_contact_finalize;
}

static void
purple_contact_init(GTypeInstance *instance, gpointer class)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleContact *contact = PURPLE_CONTACT(instance);
	PurpleContactPrivate *priv = contact->priv = PURPLE_CONTACT_GET_PRIVATE(contact);

	priv->totalsize = 0;
	priv->currentsize = 0;
	priv->online = 0;

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
