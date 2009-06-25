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
#define _PURPLE_BLIST_C_
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

static PurpleBlistUiOps *blist_ui_ops = NULL;

PurpleBuddyList *purplebuddylist = NULL;


/*********************************************************************
 * Private utility functions                                         *
 *********************************************************************/

static guint _purple_blist_hbuddy_hash(struct _purple_hbuddy *hb)
{
	return g_str_hash(hb->name);
}

static guint _purple_blist_hbuddy_equal(struct _purple_hbuddy *hb1, struct _purple_hbuddy *hb2)
{
	return (purple_strequal(hb1->name, hb2->name) && hb1->account == hb2->account && hb1->group == hb2->group);
}

static void _purple_blist_hbuddy_free_key(struct _purple_hbuddy *hb)
{
	g_free(hb->name);
	g_free(hb);
}

static void
purple_blist_buddies_cache_add_account(PurpleAccount *account)
{
	GHashTable *account_buddies = g_hash_table_new_full((GHashFunc)_purple_blist_hbuddy_hash,
						(GEqualFunc)_purple_blist_hbuddy_equal,
						(GDestroyNotify)_purple_blist_hbuddy_free_key, NULL);
	g_hash_table_insert(purplebuddylist->buddies_cache, account, account_buddies);
}

static void
purple_blist_buddies_cache_remove_account(const PurpleAccount *account)
{
	g_hash_table_remove(purplebuddylist->buddies_cache, account);
}

/*********************************************************************
 * Writing to disk                                                   *
 *********************************************************************/

void
value_to_xmlnode(gpointer key, gpointer hvalue, gpointer user_data)
{
	const char *name;
	GValue *value;
	xmlnode *node, *child;
	char buf[21];

	name    = (const char *)key;
	value   = (GValue *)hvalue;
	node    = (xmlnode *)user_data;

	g_return_if_fail(value != NULL);

	child = xmlnode_new_child(node, "setting");
	xmlnode_set_attrib(child, "name", name);

	switch (G_VALUE_TYPE(value)) {
		case G_TYPE_INT: {
			gint i = g_value_get_int(value);
			xmlnode_set_attrib(child, "type", "int");
			g_snprintf(buf, sizeof(buf), "%d", i);
			xmlnode_insert_data(child, buf, -1);
			break;
		}
		case G_TYPE_STRING: {
			const gchar *s = g_value_get_string(value);
			xmlnode_set_attrib(child, "type", "string");
			xmlnode_insert_data(child, s, -1);
			break;
		}
		case G_TYPE_BOOLEAN: {
			gboolean b = g_value_get_boolean(value);
			xmlnode_set_attrib(child, "type", "bool");
			g_snprintf(buf, sizeof(buf), "%d", b);
			xmlnode_insert_data(child, buf, -1);
			break;
		}
		default:
			g_assert_not_reached();
	}
}

static xmlnode *
accountprivacy_to_xmlnode(PurpleAccount *account)
{
	xmlnode *node, *child;
	GSList *cur;
	char buf[10];

	node = xmlnode_new("account");
	xmlnode_set_attrib(node, "proto", purple_account_get_protocol_id(account));
	xmlnode_set_attrib(node, "name", purple_account_get_username(account));
	g_snprintf(buf, sizeof(buf), "%d", account->perm_deny);
	xmlnode_set_attrib(node, "mode", buf);

	for (cur = account->permit; cur; cur = cur->next)
	{
		child = xmlnode_new_child(node, "permit");
		xmlnode_insert_data(child, cur->data, -1);
	}

	for (cur = account->deny; cur; cur = cur->next)
	{
		child = xmlnode_new_child(node, "block");
		xmlnode_insert_data(child, cur->data, -1);
	}

	return node;
}

static xmlnode *
blist_to_xmlnode(void)
{
	xmlnode *node, *child, *grandchild;
	PurpleBlistNode *gnode;
	GList *cur;

	node = xmlnode_new("purple");
	xmlnode_set_attrib(node, "version", "1.0");

	/* Write groups */
	child = xmlnode_new_child(node, "blist");
	for (gnode = purplebuddylist->root; gnode != NULL; gnode = gnode->next)
	{
		if (!PURPLE_BLIST_NODE_SHOULD_SAVE(gnode))
			continue;
		if (PURPLE_BLIST_NODE_IS_GROUP(gnode))
		{
			grandchild = group_to_xmlnode(gnode);
			xmlnode_insert_child(child, grandchild);
		}
	}

	/* Write privacy settings */
	child = xmlnode_new_child(node, "privacy");
	for (cur = purple_accounts_get_all(); cur != NULL; cur = cur->next)
	{
		grandchild = accountprivacy_to_xmlnode(cur->data);
		xmlnode_insert_child(child, grandchild);
	}

	return node;
}

static void
purple_blist_sync(void)
{
	xmlnode *node;
	char *data;

	if (!purplebuddylist->blist_loaded)
	{
		purple_debug_error("blist", "Attempted to save buddy list before it "
						 "was read!\n");
		return;
	}

	node = blist_to_xmlnode();
	data = xmlnode_to_formatted_str(node, NULL);
	purple_util_write_data_to_file("blist.xml", data, -1);
	g_free(data);
	xmlnode_free(node);
}

static gboolean
save_cb(gpointer data)
{
	purple_blist_sync();
	purplebuddylist->save_timer = 0;
	return FALSE;
}

void
purple_blist_schedule_save()
{
	if (purplebuddylist && purplebuddylist->save_timer == 0)
		purplebuddylist->save_timer = purple_timeout_add_seconds(5, save_cb, NULL);
}


/*********************************************************************
 * Reading from disk                                                 *
 *********************************************************************/

/* TODO: Make static and rename to load_blist */
void
purple_blist_load()
{
	xmlnode *purple, *blist, *privacy;

	purplebuddylist->blist_loaded = TRUE;

	purple = purple_util_read_xml_from_file("blist.xml", _("buddy list"));

	if (purple == NULL)
		return;

	blist = xmlnode_get_child(purple, "blist");
	if (blist) {
		xmlnode *groupnode;
		for (groupnode = xmlnode_get_child(blist, "group"); groupnode != NULL;
				groupnode = xmlnode_get_next_twin(groupnode)) {
			parse_group(groupnode);
		}
	}

	privacy = xmlnode_get_child(purple, "privacy");
	if (privacy) {
		xmlnode *anode;
		for (anode = privacy->child; anode; anode = anode->next) {
			xmlnode *x;
			PurpleAccount *account;
			int imode;
			const char *acct_name, *proto, *mode, *protocol;

			acct_name = xmlnode_get_attrib(anode, "name");
			protocol = xmlnode_get_attrib(anode, "protocol");
			proto = xmlnode_get_attrib(anode, "proto");
			mode = xmlnode_get_attrib(anode, "mode");

			if (!acct_name || (!proto && !protocol) || !mode)
				continue;

			account = purple_accounts_find(acct_name, proto ? proto : protocol);

			if (!account)
				continue;

			imode = atoi(mode);
			account->perm_deny = (imode != 0 ? imode : PURPLE_PRIVACY_ALLOW_ALL);

			for (x = anode->child; x; x = x->next) {
				char *name;
				if (x->type != XMLNODE_TYPE_TAG)
					continue;

				if (purple_strequal(x->name, "permit")) {
					name = xmlnode_get_data(x);
					purple_privacy_permit_add(account, name, TRUE);
					g_free(name);
				} else if (purple_strequal(x->name, "block")) {
					name = xmlnode_get_data(x);
					purple_privacy_deny_add(account, name, TRUE);
					g_free(name);
				}
			}
		}
	}

	xmlnode_free(purple);

	/* This tells the buddy icon code to do its thing. */
	_purple_buddy_icons_blist_loaded_cb();
}


/*********************************************************************
 * Stuff                                                             *
 *********************************************************************/


/*****************************************************************************
 * Public API functions                                                      *
 *****************************************************************************/

PurpleBuddyList *purple_blist_new()
{
	PurpleBlistUiOps *ui_ops;
	GList *account;
	PurpleBuddyList *gbl = g_object_new(PURPLE_BLIST_TYPE, NULL);
	PURPLE_DBUS_REGISTER_POINTER(gbl, PurpleBuddyList);

	ui_ops = purple_blist_get_ui_ops();

  #warning: This has to be set here or we can\'t add the buddies cache
  if(purplebuddylist) /* In case we're creating a replacement list */
    purple_blist_destroy();
  purple_set_blist(gbl);

	gbl->buddies = g_hash_table_new_full((GHashFunc)_purple_blist_hbuddy_hash,
					 (GEqualFunc)_purple_blist_hbuddy_equal,
					 (GDestroyNotify)_purple_blist_hbuddy_free_key, NULL);

	gbl->buddies_cache = g_hash_table_new_full(g_direct_hash, g_direct_equal,
					 NULL, (GDestroyNotify)g_hash_table_destroy);

	for (account = purple_accounts_get_all(); account != NULL; account = account->next)
	{
		purple_blist_buddies_cache_add_account(account->data);
	}

	if (ui_ops != NULL && ui_ops->new_list != NULL)
		ui_ops->new_list(gbl);

  gbl->save_timer = 0;
  gbl->blist_loaded = FALSE;

	return gbl;
}

void
purple_set_blist(PurpleBuddyList *list)
{
	purplebuddylist = list;
}

PurpleBuddyList *
purple_get_blist()
{
	return purplebuddylist;
}

PurpleBlistNode *
purple_blist_get_root()
{
	return purplebuddylist ? purplebuddylist->root : NULL;
}

void *
purple_blist_get_ui_data()
{
	return purplebuddylist->ui_data;
}

void
purple_blist_set_ui_data(void *ui_data)
{
	purplebuddylist->ui_data = ui_data;
}

void purple_blist_show()
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();

	if (ops && ops->show)
		ops->show(purplebuddylist);
}

void purple_blist_destroy()
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();

	purple_debug(PURPLE_DEBUG_INFO, "blist", "Destroying\n");

	if (ops && ops->destroy)
		ops->destroy(purplebuddylist);
}

void purple_blist_set_visible(gboolean show)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();

	if (ops && ops->set_visible)
		ops->set_visible(purplebuddylist, show);
}



gboolean
purple_strings_are_different(const char *one, const char *two)
{
	return !((one && two && g_utf8_collate(one, two) == 0) ||
			((one == NULL || *one == '\0') && (two == NULL || *two == '\0')));
}

void purple_blist_add_contact(PurpleContact *contact, PurpleGroup *group, PurpleBlistNode *node)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleGroup *g;
	PurpleBlistNode *gnode, *cnode, *bnode;

	g_return_if_fail(contact != NULL);
	g_return_if_fail(PURPLE_BLIST_NODE_IS_CONTACT((PurpleBlistNode*)contact));

	if (PURPLE_BLIST_NODE(contact) == node)
		return;

	if (node && (PURPLE_BLIST_NODE_IS_CONTACT(node) ||
				PURPLE_BLIST_NODE_IS_CHAT(node)))
		g = (PurpleGroup*)node->parent;
	else if (group)
		g = group;
	else {
		g = purple_find_group(_("Buddies"));
		if (g == NULL) {
			g = purple_group_new(_("Buddies"));
			purple_blist_add_group(g,
					purple_blist_get_last_sibling(purplebuddylist->root));
		}
	}

	gnode = (PurpleBlistNode*)g;
	cnode = (PurpleBlistNode*)contact;

	if (cnode->parent) {
		if (cnode->parent->child == cnode)
			cnode->parent->child = cnode->next;
		if (cnode->prev)
			cnode->prev->next = cnode->next;
		if (cnode->next)
			cnode->next->prev = cnode->prev;

		if (cnode->parent != gnode) {
			bnode = cnode->child;
			while (bnode) {
				PurpleBlistNode *next_bnode = bnode->next;
				PurpleBuddy *b = (PurpleBuddy*)bnode;
				GHashTable *account_buddies;

				struct _purple_hbuddy *hb, *hb2;
				
				hb = g_new(struct _purple_hbuddy, 1);
				hb->name = g_strdup(purple_normalize(b->account, b->name));
				hb->account = b->account;
				hb->group = cnode->parent;

				g_hash_table_remove(purplebuddylist->buddies, hb);

				account_buddies = g_hash_table_lookup(purplebuddylist->buddies_cache, b->account);
				g_hash_table_remove(account_buddies, hb);

				if (!purple_find_buddy_in_group(b->account, b->name, g)) {
					hb->group = gnode;
					g_hash_table_replace(purplebuddylist->buddies, hb, b);

					hb2 = g_new(struct _purple_hbuddy, 1);
					hb2->name = g_strdup(hb->name);
					hb2->account = b->account;
					hb2->group = gnode;

					g_hash_table_replace(account_buddies, hb2, b);

					if (purple_account_get_connection(b->account))
						serv_move_buddy(b, (PurpleGroup *)cnode->parent, g);
				} else {
					gboolean empty_contact = FALSE;

					/* this buddy already exists in the group, so we're
					 * gonna delete it instead */
					g_free(hb->name);
					g_free(hb);
					if (purple_account_get_connection(b->account))
						purple_account_remove_buddy(b->account, b, (PurpleGroup *)cnode->parent);

					if (!cnode->child->next)
						empty_contact = TRUE;
					purple_blist_remove_buddy(b);

					/** in purple_blist_remove_buddy(), if the last buddy in a
					 * contact is removed, the contact is cleaned up and
					 * g_free'd, so we mustn't try to reference bnode->next */
					if (empty_contact)
						return;
				}
				bnode = next_bnode;
			}
		}

		if (contact->online > 0)
			((PurpleGroup*)cnode->parent)->online--;
		if (contact->currentsize > 0)
			((PurpleGroup*)cnode->parent)->currentsize--;
		((PurpleGroup*)cnode->parent)->totalsize--;

		if (ops && ops->remove)
			ops->remove(purplebuddylist, cnode);

		purple_blist_schedule_save();
	}

	if (node && (PURPLE_BLIST_NODE_IS_CONTACT(node) ||
				PURPLE_BLIST_NODE_IS_CHAT(node))) {
		if (node->next)
			node->next->prev = cnode;
		cnode->next = node->next;
		cnode->prev = node;
		cnode->parent = node->parent;
		node->next = cnode;
	} else {
		if (gnode->child)
			gnode->child->prev = cnode;
		cnode->prev = NULL;
		cnode->next = gnode->child;
		gnode->child = cnode;
		cnode->parent = gnode;
	}

	if (contact->online > 0)
		g->online++;
	if (contact->currentsize > 0)
		g->currentsize++;
	g->totalsize++;

	purple_blist_schedule_save();

	if (ops && ops->update)
	{
		if (cnode->child)
			ops->update(purplebuddylist, cnode);

		for (bnode = cnode->child; bnode; bnode = bnode->next)
			ops->update(purplebuddylist, bnode);
	}
}

void purple_blist_add_group(PurpleGroup *group, PurpleBlistNode *node)
{
	PurpleBlistUiOps *ops;
	PurpleBlistNode *gnode = (PurpleBlistNode*)group;

	g_return_if_fail(group != NULL);
	g_return_if_fail(PURPLE_BLIST_NODE_IS_GROUP((PurpleBlistNode *)group));

	ops = purple_blist_get_ui_ops();

	if (!purplebuddylist->root) {
		purplebuddylist->root = gnode;
		return;
	}

	/* if we're moving to overtop of ourselves, do nothing */
	if (gnode == node)
		return;

	if (purple_find_group(group->name)) {
		/* This is just being moved */

		if (ops && ops->remove)
			ops->remove(purplebuddylist, (PurpleBlistNode *)group);

		if (gnode == purplebuddylist->root)
			purplebuddylist->root = gnode->next;
		if (gnode->prev)
			gnode->prev->next = gnode->next;
		if (gnode->next)
			gnode->next->prev = gnode->prev;
	}

	if (node && PURPLE_BLIST_NODE_IS_GROUP(node)) {
		gnode->next = node->next;
		gnode->prev = node;
		if (node->next)
			node->next->prev = gnode;
		node->next = gnode;
	} else {
		if (purplebuddylist->root)
			purplebuddylist->root->prev = gnode;
		gnode->next = purplebuddylist->root;
		gnode->prev = NULL;
		purplebuddylist->root = gnode;
	}

	purple_blist_schedule_save();

	if (ops && ops->update) {
		ops->update(purplebuddylist, gnode);
		for (node = gnode->child; node; node = node->next)
			ops->update(purplebuddylist, node);
	}

	purple_signal_emit(purple_blist_get_handle(), "blist-node-added",
			gnode);
}

void purple_blist_remove_contact(PurpleContact *contact)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleBlistNode *node, *gnode;

	g_return_if_fail(contact != NULL);

	node = (PurpleBlistNode *)contact;
	gnode = node->parent;

	if (node->child) {
		/*
		 * If this contact has children then remove them.  When the last
		 * buddy is removed from the contact, the contact is automatically
		 * deleted.
		 */
		while (node->child->next) {
			purple_blist_remove_buddy((PurpleBuddy*)node->child);
		}
		/*
		 * Remove the last buddy and trigger the deletion of the contact.
		 * It would probably be cleaner if contact-deletion was done after
		 * a timeout?  Or if it had to be done manually, like below?
		 */
		purple_blist_remove_buddy((PurpleBuddy*)node->child);
	} else {
		/* Remove the node from its parent */
		if (gnode->child == node)
			gnode->child = node->next;
		if (node->prev)
			node->prev->next = node->next;
		if (node->next)
			node->next->prev = node->prev;

		purple_blist_schedule_save();

		/* Update the UI */
		if (ops && ops->remove)
			ops->remove(purplebuddylist, node);

		purple_signal_emit(purple_blist_get_handle(), "blist-node-removed",
				PURPLE_BLIST_NODE(contact));

		/* Delete the node */
		purple_contact_destroy(contact);
	}
}

void purple_blist_remove_buddy(PurpleBuddy *buddy)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleBlistNode *node, *cnode, *gnode;
	PurpleContact *contact;
	PurpleGroup *group;
	struct _purple_hbuddy hb;
	GHashTable *account_buddies;

	g_return_if_fail(buddy != NULL);

	node = (PurpleBlistNode *)buddy;
	cnode = node->parent;
	gnode = (cnode != NULL) ? cnode->parent : NULL;
	contact = (PurpleContact *)cnode;
	group = (PurpleGroup *)gnode;

	/* Remove the node from its parent */
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	if ((cnode != NULL) && (cnode->child == node))
		cnode->child = node->next;

	/* Adjust size counts */
	if (contact != NULL) {
		if (PURPLE_BUDDY_IS_ONLINE(buddy)) {
			contact->online--;
			if (contact->online == 0)
				group->online--;
		}
		if (purple_account_is_connected(buddy->account)) {
			contact->currentsize--;
			if (contact->currentsize == 0)
				group->currentsize--;
		}
		contact->totalsize--;

		/* Re-sort the contact */
		if (cnode->child && contact->priority == buddy) {
			purple_contact_invalidate_priority_buddy(contact);
			if (ops && ops->update)
				ops->update(purplebuddylist, cnode);
		}
	}

	purple_blist_schedule_save();

	/* Remove this buddy from the buddies hash table */
	hb.name = g_strdup(purple_normalize(buddy->account, buddy->name));
	hb.account = buddy->account;
	hb.group = gnode;
	g_hash_table_remove(purplebuddylist->buddies, &hb);

	account_buddies = g_hash_table_lookup(purplebuddylist->buddies_cache, buddy->account);
	g_hash_table_remove(account_buddies, &hb);

	g_free(hb.name);

	/* Update the UI */
	if (ops && ops->remove)
		ops->remove(purplebuddylist, node);

	/* Signal that the buddy has been removed before freeing the memory for it */
	purple_signal_emit(purple_blist_get_handle(), "buddy-removed", buddy);

	purple_signal_emit(purple_blist_get_handle(), "blist-node-removed",
			PURPLE_BLIST_NODE(buddy));

	purple_buddy_destroy(buddy);

	/* If the contact is empty then remove it */
	if ((contact != NULL) && !cnode->child)
		purple_blist_remove_contact(contact);
}

void purple_blist_remove_chat(PurpleChat *chat)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleBlistNode *node, *gnode;
	PurpleGroup *group;

	g_return_if_fail(chat != NULL);

	node = (PurpleBlistNode *)chat;
	gnode = node->parent;
	group = (PurpleGroup *)gnode;

	if (gnode != NULL)
	{
		/* Remove the node from its parent */
		if (gnode->child == node)
			gnode->child = node->next;
		if (node->prev)
			node->prev->next = node->next;
		if (node->next)
			node->next->prev = node->prev;

		/* Adjust size counts */
		if (purple_account_is_connected(chat->account)) {
			group->online--;
			group->currentsize--;
		}
		group->totalsize--;

		purple_blist_schedule_save();
	}

	/* Update the UI */
	if (ops && ops->remove)
		ops->remove(purplebuddylist, node);

	purple_signal_emit(purple_blist_get_handle(), "blist-node-removed",
			PURPLE_BLIST_NODE(chat));

	/* Delete the node */
	purple_chat_destroy(chat);
}

void purple_blist_remove_group(PurpleGroup *group)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleBlistNode *node;
	GList *l;

	g_return_if_fail(group != NULL);

	node = (PurpleBlistNode *)group;

	/* Make sure the group is empty */
	if (node->child)
		return;

	/* Remove the node from its parent */
	if (purplebuddylist->root == node)
		purplebuddylist->root = node->next;
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;

	purple_blist_schedule_save();

	/* Update the UI */
	if (ops && ops->remove)
		ops->remove(purplebuddylist, node);

	purple_signal_emit(purple_blist_get_handle(), "blist-node-removed",
			PURPLE_BLIST_NODE(group));

	/* Remove the group from all accounts that are online */
	for (l = purple_connections_get_all(); l != NULL; l = l->next)
	{
		PurpleConnection *gc = (PurpleConnection *)l->data;

		if (purple_connection_get_state(gc) == PURPLE_CONNECTION_STATE_CONNECTED)
			purple_account_remove_group(purple_connection_get_account(gc), group);
	}

	/* Delete the node */
	purple_group_destroy(group);
}


PurpleBuddy *purple_find_buddy(PurpleAccount *account, const char *name)
{
	PurpleBuddy *buddy;
	struct _purple_hbuddy hb;
	PurpleBlistNode *group;

	g_return_val_if_fail(purplebuddylist != NULL, NULL);
	g_return_val_if_fail(account != NULL, NULL);
	g_return_val_if_fail((name != NULL) && (*name != '\0'), NULL);

	hb.account = account;
	hb.name = g_strdup(purple_normalize(account, name));

	for (group = purplebuddylist->root; group; group = group->next) {
		hb.group = group;
		if ((buddy = g_hash_table_lookup(purplebuddylist->buddies, &hb))) {
			g_free(hb.name);
			return buddy;
		}
	}
	g_free(hb.name);

	return NULL;
}

static void find_acct_buddies(gpointer key, gpointer value, gpointer data)
{
	PurpleBuddy *buddy = value;
	GSList **list = data;

	*list = g_slist_prepend(*list, buddy);
}

GSList *purple_find_buddies(PurpleAccount *account, const char *name)
{
	PurpleBuddy *buddy;
	PurpleBlistNode *node;
	GSList *ret = NULL;

	g_return_val_if_fail(purplebuddylist != NULL, NULL);
	g_return_val_if_fail(account != NULL, NULL);

	if ((name != NULL) && (*name != '\0')) {
		struct _purple_hbuddy hb;

		hb.name = g_strdup(purple_normalize(account, name));
		hb.account = account;

		for (node = purplebuddylist->root; node != NULL; node = node->next) {
			hb.group = node;
			if ((buddy = g_hash_table_lookup(purplebuddylist->buddies, &hb)) != NULL)
				ret = g_slist_prepend(ret, buddy);
		}
		g_free(hb.name);
	} else {
		GSList *list = NULL;
		GHashTable *buddies = g_hash_table_lookup(purplebuddylist->buddies_cache, account);
		g_hash_table_foreach(buddies, find_acct_buddies, &list);
		ret = list;
	}

	return ret;
}

GSList *purple_group_get_accounts(PurpleGroup *group)
{
	GSList *l = NULL;
	PurpleBlistNode *gnode, *cnode, *bnode;

	gnode = (PurpleBlistNode *)group;

	for (cnode = gnode->child;  cnode; cnode = cnode->next) {
		if (PURPLE_BLIST_NODE_IS_CHAT(cnode)) {
			if (!g_slist_find(l, ((PurpleChat *)cnode)->account))
				l = g_slist_append(l, ((PurpleChat *)cnode)->account);
		} else if (PURPLE_BLIST_NODE_IS_CONTACT(cnode)) {
			for (bnode = cnode->child; bnode; bnode = bnode->next) {
				if (PURPLE_BLIST_NODE_IS_BUDDY(bnode)) {
					if (!g_slist_find(l, ((PurpleBuddy *)bnode)->account))
						l = g_slist_append(l, ((PurpleBuddy *)bnode)->account);
				}
			}
		}
	}

	return l;
}

void purple_blist_add_account(PurpleAccount *account)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleBlistNode *gnode, *cnode, *bnode;

	g_return_if_fail(purplebuddylist != NULL);

	if (!ops || !ops->update)
		return;

	for (gnode = purplebuddylist->root; gnode; gnode = gnode->next) {
		if (!PURPLE_BLIST_NODE_IS_GROUP(gnode))
			continue;
		for (cnode = gnode->child; cnode; cnode = cnode->next) {
			if (PURPLE_BLIST_NODE_IS_CONTACT(cnode)) {
				gboolean recompute = FALSE;
					for (bnode = cnode->child; bnode; bnode = bnode->next) {
						if (PURPLE_BLIST_NODE_IS_BUDDY(bnode) &&
								((PurpleBuddy*)bnode)->account == account) {
							recompute = TRUE;
							((PurpleContact*)cnode)->currentsize++;
							if (((PurpleContact*)cnode)->currentsize == 1)
								((PurpleGroup*)gnode)->currentsize++;
							ops->update(purplebuddylist, bnode);
						}
					}
					if (recompute ||
							purple_blist_node_get_bool(cnode, "show_offline")) {
						purple_contact_invalidate_priority_buddy((PurpleContact*)cnode);
						ops->update(purplebuddylist, cnode);
					}
			} else if (PURPLE_BLIST_NODE_IS_CHAT(cnode) &&
					((PurpleChat*)cnode)->account == account) {
				((PurpleGroup *)gnode)->online++;
				((PurpleGroup *)gnode)->currentsize++;
				ops->update(purplebuddylist, cnode);
			}
		}
		ops->update(purplebuddylist, gnode);
	}
}

void purple_blist_remove_account(PurpleAccount *account)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleBlistNode *gnode, *cnode, *bnode;
	PurpleBuddy *buddy;
	PurpleChat *chat;
	PurpleContact *contact;
	PurpleGroup *group;
	GList *list = NULL, *iter = NULL;

	g_return_if_fail(purplebuddylist != NULL);

	for (gnode = purplebuddylist->root; gnode; gnode = gnode->next) {
		if (!PURPLE_BLIST_NODE_IS_GROUP(gnode))
			continue;

		group = (PurpleGroup *)gnode;

		for (cnode = gnode->child; cnode; cnode = cnode->next) {
			if (PURPLE_BLIST_NODE_IS_CONTACT(cnode)) {
				gboolean recompute = FALSE;
				contact = (PurpleContact *)cnode;

				for (bnode = cnode->child; bnode; bnode = bnode->next) {
					if (!PURPLE_BLIST_NODE_IS_BUDDY(bnode))
						continue;

					buddy = (PurpleBuddy *)bnode;
					if (account == buddy->account) {
						PurplePresence *presence;

						presence = purple_buddy_get_presence(buddy);

						if(purple_presence_is_online(presence)) {
							contact->online--;
							if (contact->online == 0)
								group->online--;

							purple_blist_node_set_int(&buddy->node,
													"last_seen", time(NULL));
						}

						contact->currentsize--;
						if (contact->currentsize == 0)
							group->currentsize--;

						if (!g_list_find(list, presence))
							list = g_list_prepend(list, presence);

						if (contact->priority == buddy)
							purple_contact_invalidate_priority_buddy(contact);
						else
							recompute = TRUE;

						if (ops && ops->remove) {
							ops->remove(purplebuddylist, bnode);
						}
					}
				}
				if (recompute) {
					purple_contact_invalidate_priority_buddy(contact);
					if (ops && ops->update)
						ops->update(purplebuddylist, cnode);
				}
			} else if (PURPLE_BLIST_NODE_IS_CHAT(cnode)) {
				chat = (PurpleChat *)cnode;

				if(chat->account == account) {
					group->currentsize--;
					group->online--;

					if (ops && ops->remove)
						ops->remove(purplebuddylist, cnode);
				}
			}
		}
	}

	for (iter = list; iter; iter = iter->next)
	{
		purple_presence_set_status_active(iter->data, "offline", TRUE);
	}
	g_list_free(list);
}

gboolean purple_group_on_account(PurpleGroup *g, PurpleAccount *account)
{
	PurpleBlistNode *cnode;
	for (cnode = ((PurpleBlistNode *)g)->child; cnode; cnode = cnode->next) {
		if (PURPLE_BLIST_NODE_IS_CONTACT(cnode)) {
			if(purple_contact_on_account((PurpleContact *) cnode, account))
				return TRUE;
		} else if (PURPLE_BLIST_NODE_IS_CHAT(cnode)) {
			PurpleChat *chat = (PurpleChat *)cnode;
			if ((!account && purple_account_is_connected(chat->account))
					|| chat->account == account)
				return TRUE;
		}
	}
	return FALSE;
}

void
purple_blist_request_add_buddy(PurpleAccount *account, const char *username,
							 const char *group, const char *alias)
{
	PurpleBlistUiOps *ui_ops;

	ui_ops = purple_blist_get_ui_ops();

	if (ui_ops != NULL && ui_ops->request_add_buddy != NULL)
		ui_ops->request_add_buddy(account, username, group, alias);
}

void
purple_blist_request_add_chat(PurpleAccount *account, PurpleGroup *group,
							const char *alias, const char *name)
{
	PurpleBlistUiOps *ui_ops;

	ui_ops = purple_blist_get_ui_ops();

	if (ui_ops != NULL && ui_ops->request_add_chat != NULL)
		ui_ops->request_add_chat(account, group, alias, name);
}

void
purple_blist_request_add_group(void)
{
	PurpleBlistUiOps *ui_ops;

	ui_ops = purple_blist_get_ui_ops();

	if (ui_ops != NULL && ui_ops->request_add_group != NULL)
		ui_ops->request_add_group();
}

void
purple_blist_set_ui_ops(PurpleBlistUiOps *ops)
{
	blist_ui_ops = ops;
}

PurpleBlistUiOps *
purple_blist_get_ui_ops(void)
{
	return blist_ui_ops;
}


void *
purple_blist_get_handle(void)
{
	static int handle;

	return &handle;
}

void
purple_blist_init(void)
{
  #warning: Should this be moved to the PurpleBlistClass init func?
	void *handle = purple_blist_get_handle();

	purple_signal_register(handle, "buddy-status-changed",
	                     purple_marshal_VOID__POINTER_POINTER_POINTER, NULL,
	                     3,
	                     purple_value_new(PURPLE_TYPE_SUBTYPE,
	                                    PURPLE_SUBTYPE_BLIST_BUDDY),
	                     purple_value_new(PURPLE_TYPE_SUBTYPE,
										PURPLE_SUBTYPE_STATUS),
	                     purple_value_new(PURPLE_TYPE_SUBTYPE,
										PURPLE_SUBTYPE_STATUS));
	purple_signal_register(handle, "buddy-privacy-changed",
	                     purple_marshal_VOID__POINTER, NULL,
	                     1,
	                     purple_value_new(PURPLE_TYPE_SUBTYPE,
	                                    PURPLE_SUBTYPE_BLIST_BUDDY));

	purple_signal_register(handle, "buddy-idle-changed",
	                     purple_marshal_VOID__POINTER_INT_INT, NULL,
	                     3,
	                     purple_value_new(PURPLE_TYPE_SUBTYPE,
	                                    PURPLE_SUBTYPE_BLIST_BUDDY),
	                     purple_value_new(PURPLE_TYPE_INT),
	                     purple_value_new(PURPLE_TYPE_INT));


	purple_signal_register(handle, "buddy-signed-on",
						 purple_marshal_VOID__POINTER, NULL, 1,
						 purple_value_new(PURPLE_TYPE_SUBTYPE,
										PURPLE_SUBTYPE_BLIST_BUDDY));

	purple_signal_register(handle, "buddy-signed-off",
						 purple_marshal_VOID__POINTER, NULL, 1,
						 purple_value_new(PURPLE_TYPE_SUBTYPE,
										PURPLE_SUBTYPE_BLIST_BUDDY));

	purple_signal_register(handle, "buddy-got-login-time",
						 purple_marshal_VOID__POINTER, NULL, 1,
						 purple_value_new(PURPLE_TYPE_SUBTYPE,
										PURPLE_SUBTYPE_BLIST_BUDDY));

	purple_signal_register(handle, "blist-node-added",
						 purple_marshal_VOID__POINTER, NULL, 1,
						 purple_value_new(PURPLE_TYPE_SUBTYPE,
										PURPLE_SUBTYPE_BLIST_NODE));

	purple_signal_register(handle, "blist-node-removed",
						 purple_marshal_VOID__POINTER, NULL, 1,
						 purple_value_new(PURPLE_TYPE_SUBTYPE,
										PURPLE_SUBTYPE_BLIST_NODE));

	purple_signal_register(handle, "buddy-added",
						 purple_marshal_VOID__POINTER, NULL, 1,
						 purple_value_new(PURPLE_TYPE_SUBTYPE,
										PURPLE_SUBTYPE_BLIST_BUDDY));

	purple_signal_register(handle, "buddy-removed",
						 purple_marshal_VOID__POINTER, NULL, 1,
						 purple_value_new(PURPLE_TYPE_SUBTYPE,
										PURPLE_SUBTYPE_BLIST_BUDDY));

	purple_signal_register(handle, "buddy-icon-changed",
						 purple_marshal_VOID__POINTER, NULL, 1,
						 purple_value_new(PURPLE_TYPE_SUBTYPE,
										PURPLE_SUBTYPE_BLIST_BUDDY));

	purple_signal_register(handle, "update-idle", purple_marshal_VOID, NULL, 0);

	purple_signal_register(handle, "blist-node-extended-menu",
			     purple_marshal_VOID__POINTER_POINTER, NULL, 2,
			     purple_value_new(PURPLE_TYPE_SUBTYPE,
					    PURPLE_SUBTYPE_BLIST_NODE),
			     purple_value_new(PURPLE_TYPE_BOXED, "GList **"));

	purple_signal_register(handle, "blist-node-aliased",
						 purple_marshal_VOID__POINTER_POINTER, NULL, 2,
						 purple_value_new(PURPLE_TYPE_SUBTYPE,
										PURPLE_SUBTYPE_BLIST_NODE),
						 purple_value_new(PURPLE_TYPE_STRING));

	purple_signal_connect(purple_accounts_get_handle(), "account-created",
			handle,
			PURPLE_CALLBACK(purple_blist_buddies_cache_add_account),
			NULL);

	purple_signal_connect(purple_accounts_get_handle(), "account-destroying",
			handle,
			PURPLE_CALLBACK(purple_blist_buddies_cache_remove_account),
			NULL);
}

void
purple_blist_uninit(void)
{
	PurpleBlistNode *node, *next_node;

	/* This happens if we quit before purple_set_blist is called. */
	if (purplebuddylist == NULL)
		return;

	if (purplebuddylist->save_timer != 0) {
		purple_timeout_remove(purplebuddylist->save_timer);
		purplebuddylist->save_timer = 0;
		purple_blist_sync();
	}

	node = purple_blist_get_root();
	while (node) {
		next_node = node->next;
		purple_blist_node_destroy(node);
		node = next_node;
	}
	purplebuddylist->root = NULL;
	
	g_hash_table_destroy(purplebuddylist->buddies);
	g_hash_table_destroy(purplebuddylist->buddies_cache);

	purple_signals_disconnect_by_handle(purple_blist_get_handle());
	purple_signals_unregister_by_instance(purple_blist_get_handle());
};

/******************/
/*  GObject Code  */
/******************/

static void
purple_blist_class_init(PurpleBuddyListClass *klass)
{

}

static void
purple_blist_instance_init(GTypeInstance *instance, gpointer class)
{

}

GType
purple_blist_get_gtype(void)
{
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleBuddyListClass),
			NULL,					/* base_init		*/
			NULL,					/* base_finalize	*/
			(GClassInitFunc)purple_blist_class_init,
			NULL,
			NULL,					/* class_data		*/
			sizeof(PurpleBuddyList),
			0,						/* n_preallocs		*/
			purple_blist_instance_init,					/* instance_init	*/
			NULL					/* value_table		*/
		};

		type = g_type_register_static(PURPLE_TYPE_OBJECT,
									  "PurpleBuddyList",
									  &info, 0);
	}

	return type;
}
