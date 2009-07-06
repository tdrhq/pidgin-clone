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
#include "dbus-maybe.h"
#include "debug.h"
#include "server.h"
#include "signals.h"
#include "xmlnode.h"

static GType purple_blist_get_gtype(void);

/** @copydoc _PurpleBlist */
typedef struct _PurpleBuddyListClass PurpleBuddyListClass;
#define PURPLE_BUDDY_LIST_TYPE                  (purple_blist_get_gtype ())
#define PURPLE_BUDDY_LIST(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), PURPLE_BUDDY_LIST_TYPE, PurpleBuddyList))
#define PURPLE_IS_BUDDY_LIST(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PURPLE_BUDDY_LIST_TYPE))
#define PURPLE_BUDDY_LIST_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), PURPLE_BUDDY_LIST_TYPE, PurpleBuddyListClass))
#define PURPLE_IS_BUDDY_LIST_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), PURPLE_BUDDY_LIST_TYPE))
#define PURPLE_GET_BUDDY_LIST_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), PURPLE_BUDDY_LIST_TYPE, PurpleBuddyListClass))

/**
 * The Buddy List
 */
struct _PurpleBuddyList {
PurpleObject parent;
	PurpleBlistNode *root;        /**< The first node in the buddy list */
	GHashTable *buddies;          /**< Every buddy in this list */
	
	/**
	 * A hash table used for efficient lookups of buddies by name.
	 * PurpleAccount* => GHashTable*, with the inner hash table being
	 * struct _purple_hbuddy => PurpleBuddy*
	 */
	GHashTable *buddies_cache;

	guint save_timer;
	gboolean blist_loaded;
	void *ui_data;                /**< UI-specific data. */
};

struct _PurpleBuddyListClass {
	PurpleObjectClass parent;
};

static PurpleBlistUiOps *blist_ui_ops = NULL;

PurpleBuddyList *purplebuddylist = NULL;
static GObjectClass *parent_class = NULL;


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
		if (PURPLE_IS_GROUP(gnode))
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

void
parse_group(xmlnode *groupnode)
{
	const char *name = xmlnode_get_attrib(groupnode, "name");
	PurpleGroup *group;
	xmlnode *cnode;

	if (!name)
		name = _("Buddies");

	group = purple_group_new(name);
	purple_blist_add_group(group,
	purple_blist_get_last_sibling(purplebuddylist->root));

	for (cnode = groupnode->child; cnode; cnode = cnode->next) {
		if (cnode->type != XMLNODE_TYPE_TAG)
			continue;
		if (purple_strequal(cnode->name, "setting"))
			parse_setting((PurpleBlistNode*)group, cnode);
		else if (purple_strequal(cnode->name, "contact") ||
				purple_strequal(cnode->name, "person"))
			parse_contact(group, cnode);
		else if (purple_strequal(cnode->name, "chat"))
			parse_chat(group, cnode);
	}
}

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


/*****************************************************************************
 * Public API functions                                                      *
 *****************************************************************************/

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
	/* This function is only a hack for api breakage */
	g_return_if_fail(purplebuddylist != NULL);
	g_object_unref(G_OBJECT(purplebuddylist));
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

/*
 * TODO: Maybe remove the call to this from server.c and call it
 * from oscar.c and toc.c instead?
 */
void purple_blist_rename_buddy(PurpleBuddy *buddy, const char *name)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	struct _purple_hbuddy *hb, *hb2;
	GHashTable *account_buddies;

	g_return_if_fail(buddy != NULL);

	hb = g_new(struct _purple_hbuddy, 1);
	hb->name = g_strdup(purple_normalize(purple_buddy_get_account(buddy), purple_buddy_get_name(buddy)));
	hb->account = purple_buddy_get_account(buddy);
	hb->group = ((PurpleBlistNode *)buddy)->parent->parent;
	g_hash_table_remove(purplebuddylist->buddies, hb);
	
	account_buddies = g_hash_table_lookup(purplebuddylist->buddies_cache, purple_buddy_get_account(buddy));
	g_hash_table_remove(account_buddies, hb);

	g_free(hb->name);
	hb->name = g_strdup(purple_normalize(purple_buddy_get_account(buddy), name));
	g_hash_table_replace(purplebuddylist->buddies, hb, buddy);

	hb2 = g_new(struct _purple_hbuddy, 1);
	hb2->name = g_strdup(hb->name);
	hb2->account = purple_buddy_get_account(buddy);
	hb2->group = ((PurpleBlistNode *)buddy)->parent->parent;

	g_hash_table_replace(account_buddies, hb2, buddy);

	purple_buddy_set_name(buddy, name);

	purple_blist_schedule_save();

	if (ops && ops->update)
		ops->update(purplebuddylist, (PurpleBlistNode *)buddy);
}

void purple_blist_add_contact(PurpleContact *contact, PurpleGroup *group, PurpleBlistNode *node)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleGroup *g;
	PurpleBlistNode *gnode, *cnode, *bnode;

	g_return_if_fail(contact != NULL);
	g_return_if_fail(PURPLE_IS_CONTACT((PurpleBlistNode*)contact));

	if (PURPLE_BLIST_NODE(contact) == node)
		return;

	if (node && (PURPLE_IS_CONTACT(node) ||
				PURPLE_IS_CHAT(node)))
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
				hb->name = g_strdup(purple_normalize(purple_buddy_get_account(b), purple_buddy_get_name(b)));
				hb->account = purple_buddy_get_account(b);
				hb->group = cnode->parent;

				g_hash_table_remove(purplebuddylist->buddies, hb);

				account_buddies = g_hash_table_lookup(purplebuddylist->buddies_cache, purple_buddy_get_account(b));
				g_hash_table_remove(account_buddies, hb);

				if (!purple_find_buddy_in_group(purple_buddy_get_account(b), purple_buddy_get_name(b), g)) {
					hb->group = gnode;
					g_hash_table_replace(purplebuddylist->buddies, hb, b);

					hb2 = g_new(struct _purple_hbuddy, 1);
					hb2->name = g_strdup(hb->name);
					hb2->account = purple_buddy_get_account(b);
					hb2->group = gnode;

					g_hash_table_replace(account_buddies, hb2, b);

					if (purple_account_get_connection(purple_buddy_get_account(b)))
						serv_move_buddy(b, (PurpleGroup *)cnode->parent, g);
				} else {
					gboolean empty_contact = FALSE;

					/* this buddy already exists in the group, so we're
					 * gonna delete it instead */
					g_free(hb->name);
					g_free(hb);
					if (purple_account_get_connection(purple_buddy_get_account(b)))
						purple_account_remove_buddy(purple_buddy_get_account(b), b, (PurpleGroup *)cnode->parent);

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

		if (purple_contact_get_online(contact) > 0)
			((PurpleGroup*)cnode->parent)->online--;
		if (purple_contact_get_currentsize(contact) > 0)
			((PurpleGroup*)cnode->parent)->currentsize--;
		((PurpleGroup*)cnode->parent)->totalsize--;

		if (ops && ops->remove)
			ops->remove(purplebuddylist, cnode);

		purple_blist_schedule_save();
	}

	if (node && (PURPLE_IS_CONTACT(node) ||
				PURPLE_IS_CHAT(node))) {
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

	if (purple_contact_get_online(contact) > 0)
		g->online++;
	if (purple_contact_get_currentsize(contact) > 0)
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
	g_return_if_fail(PURPLE_IS_GROUP((PurpleBlistNode *)group));

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

	if (node && PURPLE_IS_GROUP(node)) {
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
	struct _purple_hbuddy hb;
	GHashTable *account_buddies;

	g_return_if_fail(buddy != NULL);

	node = PURPLE_BLIST_NODE(buddy);
	cnode = purple_blist_node_get_parent(node);
	gnode = purple_blist_node_get_parent(cnode);

	PURPLE_GET_BLIST_NODE_CLASS(node)->remove_node(gnode, node);
	purple_blist_schedule_save();

	/* Remove this buddy from the buddies hash table */
	hb.name = g_strdup(purple_normalize(purple_buddy_get_account(buddy), purple_buddy_get_name(buddy)));
	hb.account = purple_buddy_get_account(buddy);
	hb.group = gnode;
	g_hash_table_remove(purplebuddylist->buddies, &hb);

	account_buddies = g_hash_table_lookup(purplebuddylist->buddies_cache, purple_buddy_get_account(buddy));
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

PurpleGroup *purple_find_group(const char *name)
{
	PurpleBlistNode *node;

	g_return_val_if_fail(purplebuddylist != NULL, NULL);
	g_return_val_if_fail((name != NULL) && (*name != '\0'), NULL);

	for (node = purplebuddylist->root; node != NULL; node = node->next) {
		if (!purple_utf8_strcasecmp(((PurpleGroup *)node)->name, name))
			return (PurpleGroup *)node;
	}

	return NULL;
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

void purple_blist_add_account(PurpleAccount *account)
{
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	PurpleBlistNode *gnode, *cnode, *bnode;

	g_return_if_fail(purplebuddylist != NULL);

	if (!ops || !ops->update)
		return;

	for (gnode = purplebuddylist->root; gnode; gnode = gnode->next) {
		if (!PURPLE_IS_GROUP(gnode))
			continue;
		for (cnode = gnode->child; cnode; cnode = cnode->next) {
			if (PURPLE_IS_CONTACT(cnode)) {
				gboolean recompute = FALSE;
					for (bnode = cnode->child; bnode; bnode = bnode->next) {
						if (PURPLE_IS_BUDDY(bnode) &&
								purple_buddy_get_account(PURPLE_BUDDY(bnode)) == account) {
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
			} else if (PURPLE_IS_CHAT(cnode) &&
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
		if (!PURPLE_IS_GROUP(gnode))
			continue;

		group = (PurpleGroup *)gnode;

		for (cnode = gnode->child; cnode; cnode = cnode->next) {
			if (PURPLE_IS_CONTACT(cnode)) {
				gboolean recompute = FALSE;
				contact = (PurpleContact *)cnode;

				for (bnode = cnode->child; bnode; bnode = bnode->next) {
					if (!PURPLE_IS_BUDDY(bnode))
						continue;

					buddy = (PurpleBuddy *)bnode;
					if (account == purple_buddy_get_account(buddy)) {
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
			} else if (PURPLE_IS_CHAT(cnode)) {
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
		if (PURPLE_IS_CONTACT(cnode)) {
			if(purple_contact_on_account((PurpleContact *) cnode, account))
				return TRUE;
		} else if (PURPLE_IS_CHAT(cnode)) {
			PurpleChat *chat = (PurpleChat *)cnode;
			if ((!account && purple_account_is_connected(chat->account))
					|| chat->account == account)
				return TRUE;
		}
	}
	return FALSE;
}

void purple_blist_add_chat(PurpleChat *chat, PurpleGroup *group, PurpleBlistNode *node)
{
	PurpleBlistNode *cnode = (PurpleBlistNode*)chat;
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();

	g_return_if_fail(chat != NULL);
	g_return_if_fail(PURPLE_IS_CHAT((PurpleBlistNode *)chat));

	if (node == NULL) {
		if (group == NULL)
			group = purple_group_new(_("Chats"));

		/* Add group to blist if isn't already on it. Fixes #2752. */
		if (!purple_find_group(group->name)) {
			purple_blist_add_group(group,
					purple_blist_get_last_sibling(purplebuddylist->root));
		}
	} else {
		group = (PurpleGroup*)node->parent;
	}

	/* if we're moving to overtop of ourselves, do nothing */
	if (cnode == node)
		return;

	if (cnode->parent) {
		/* This chat was already in the list and is
		 * being moved.
		 */
		((PurpleGroup *)cnode->parent)->totalsize--;
		if (purple_account_is_connected(chat->account)) {
			((PurpleGroup *)cnode->parent)->online--;
			((PurpleGroup *)cnode->parent)->currentsize--;
		}
		if (cnode->next)
			cnode->next->prev = cnode->prev;
		if (cnode->prev)
			cnode->prev->next = cnode->next;
		if (cnode->parent->child == cnode)
			cnode->parent->child = cnode->next;

		if (ops && ops->remove)
			ops->remove(purplebuddylist, cnode);
		/* ops->remove() cleaned up the cnode's ui_data, so we need to
		 * reinitialize it */
		if (ops && ops->new_node)
			ops->new_node(cnode);

		purple_blist_schedule_save();
	}

	if (node != NULL) {
		if (node->next)
			node->next->prev = cnode;
		cnode->next = node->next;
		cnode->prev = node;
		cnode->parent = node->parent;
		node->next = cnode;
		((PurpleGroup *)node->parent)->totalsize++;
		if (purple_account_is_connected(chat->account)) {
			((PurpleGroup *)node->parent)->online++;
			((PurpleGroup *)node->parent)->currentsize++;
		}
	} else {
		if (((PurpleBlistNode *)group)->child)
			((PurpleBlistNode *)group)->child->prev = cnode;
		cnode->next = ((PurpleBlistNode *)group)->child;
		cnode->prev = NULL;
		((PurpleBlistNode *)group)->child = cnode;
		cnode->parent = (PurpleBlistNode *)group;
		group->totalsize++;
		if (purple_account_is_connected(chat->account)) {
			group->online++;
			group->currentsize++;
		}
	}

	purple_blist_schedule_save();

	if (ops && ops->update)
		ops->update(purplebuddylist, (PurpleBlistNode *)cnode);

	purple_signal_emit(purple_blist_get_handle(), "blist-node-added",
			cnode);
}

void purple_blist_add_buddy(PurpleBuddy *buddy, PurpleContact *contact, PurpleGroup *group, PurpleBlistNode *node)
{
	PurpleBlistNode *cnode, *bnode;
	PurpleGroup *g;
	PurpleContact *c;
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();
	struct _purple_hbuddy *hb, *hb2;
	GHashTable *account_buddies;

	g_return_if_fail(buddy != NULL);
	g_return_if_fail(PURPLE_IS_BUDDY((PurpleBlistNode*)buddy));

	bnode = (PurpleBlistNode *)buddy;

	/* if we're moving to overtop of ourselves, do nothing */
	if (bnode == node || (!node && bnode->parent &&
				contact && bnode->parent == (PurpleBlistNode*)contact
				&& bnode == bnode->parent->child))
		return;

	if (node && PURPLE_IS_BUDDY(node)) {
		c = (PurpleContact*)node->parent;
		g = (PurpleGroup*)node->parent->parent;
	} else if (contact) {
		c = contact;
		g = PURPLE_GROUP(PURPLE_BLIST_NODE(c)->parent);
	} else {
		g = group;
		if (g == NULL)
			g = purple_group_new(_("Buddies"));
		/* Add group to blist if isn't already on it. Fixes #2752. */
		if (!purple_find_group(g->name)) {
			purple_blist_add_group(g,
					purple_blist_get_last_sibling(purplebuddylist->root));
		}
		c = purple_contact_new();
		purple_blist_add_contact(c, g,
				purple_blist_get_last_child((PurpleBlistNode*)g));
	}

	cnode = (PurpleBlistNode *)c;

	if (bnode->parent) {
		if (PURPLE_BUDDY_IS_ONLINE(buddy)) {
			((PurpleContact*)bnode->parent)->online--;
			if (((PurpleContact*)bnode->parent)->online == 0)
				((PurpleGroup*)bnode->parent->parent)->online--;
		}
		if (purple_account_is_connected(purple_buddy_get_account(buddy))) {
			((PurpleContact*)bnode->parent)->currentsize--;
			if (((PurpleContact*)bnode->parent)->currentsize == 0)
				((PurpleGroup*)bnode->parent->parent)->currentsize--;
		}
		((PurpleContact*)bnode->parent)->totalsize--;
		/* the group totalsize will be taken care of by remove_contact below */

		if (bnode->parent->parent != (PurpleBlistNode*)g)
			serv_move_buddy(buddy, (PurpleGroup *)bnode->parent->parent, g);

		if (bnode->next)
			bnode->next->prev = bnode->prev;
		if (bnode->prev)
			bnode->prev->next = bnode->next;
		if (bnode->parent->child == bnode)
			bnode->parent->child = bnode->next;

		if (ops && ops->remove)
			ops->remove(purplebuddylist, bnode);

		purple_blist_schedule_save();

		if (bnode->parent->parent != (PurpleBlistNode*)g) {
			hb = g_new(struct _purple_hbuddy, 1);
			hb->name = g_strdup(purple_normalize(purple_buddy_get_account(buddy), purple_buddy_get_name(buddy)));
			hb->account = purple_buddy_get_account(buddy);
			hb->group = bnode->parent->parent;
			g_hash_table_remove(purplebuddylist->buddies, hb);

			account_buddies = g_hash_table_lookup(purplebuddylist->buddies_cache, purple_buddy_get_account(buddy));
			g_hash_table_remove(account_buddies, hb);

			g_free(hb->name);
			g_free(hb);
		}

		if (!bnode->parent->child) {
			purple_blist_remove_contact((PurpleContact*)bnode->parent);
		} else {
			purple_contact_invalidate_priority_buddy((PurpleContact*)bnode->parent);
			if (ops && ops->update)
				ops->update(purplebuddylist, bnode->parent);
		}
	}

	if (node && PURPLE_IS_BUDDY(node)) {
		if (node->next)
			node->next->prev = bnode;
		bnode->next = node->next;
		bnode->prev = node;
		bnode->parent = node->parent;
		node->next = bnode;
	} else {
		if (cnode->child)
			cnode->child->prev = bnode;
		bnode->prev = NULL;
		bnode->next = cnode->child;
		cnode->child = bnode;
		bnode->parent = cnode;
	}

	if (PURPLE_BUDDY_IS_ONLINE(buddy)) {
		if (++(PURPLE_CONTACT(bnode->parent)->online) == 1)
			PURPLE_GROUP(bnode->parent->parent)->online++;
	}
	if (purple_account_is_connected(purple_buddy_get_account(buddy))) {
		if (++(PURPLE_CONTACT(bnode->parent)->currentsize) == 1)
			PURPLE_GROUP(bnode->parent->parent)->currentsize++;
	}
	PURPLE_CONTACT(bnode->parent)->totalsize++;

	hb = g_new(struct _purple_hbuddy, 1);
	hb->name = g_strdup(purple_normalize(purple_buddy_get_account(buddy), purple_buddy_get_name(buddy)));
	hb->account = purple_buddy_get_account(buddy);
	hb->group = ((PurpleBlistNode*)buddy)->parent->parent;

	g_hash_table_replace(purplebuddylist->buddies, hb, buddy);

	account_buddies = g_hash_table_lookup(purplebuddylist->buddies_cache, purple_buddy_get_account(buddy));

	hb2 = g_new(struct _purple_hbuddy, 1);
	hb2->name = g_strdup(hb->name);
	hb2->account = purple_buddy_get_account(buddy);
	hb2->group = ((PurpleBlistNode*)buddy)->parent->parent;

	g_hash_table_replace(account_buddies, hb2, buddy);

	purple_contact_invalidate_priority_buddy(purple_buddy_get_contact(buddy));

	purple_blist_schedule_save();

	if (ops && ops->update)
		ops->update(purplebuddylist, (PurpleBlistNode*)buddy);

	/* Signal that the buddy has been added */
	purple_signal_emit(purple_blist_get_handle(), "buddy-added", buddy);

	purple_signal_emit(purple_blist_get_handle(), "blist-node-added",
			PURPLE_BLIST_NODE(buddy));
}

PurpleChat *
purple_blist_find_chat(PurpleAccount *account, const char *name)
{
	char *chat_name;
	PurpleChat *chat;
	PurplePlugin *prpl;
	PurplePluginProtocolInfo *prpl_info = NULL;
	struct proto_chat_entry *pce;
	PurpleBlistNode *node, *group;
	GList *parts;
	char *normname;

	g_return_val_if_fail(purplebuddylist != NULL, NULL);
	g_return_val_if_fail((name != NULL) && (*name != '\0'), NULL);

	if (!purple_account_is_connected(account))
		return NULL;

	prpl = purple_find_prpl(purple_account_get_protocol_id(account));
	prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(prpl);

	if (prpl_info->find_blist_chat != NULL)
		return prpl_info->find_blist_chat(account, name);

	normname = g_strdup(purple_normalize(account, name));
	for (group = purplebuddylist->root; group != NULL; group = group->next) {
		for (node = group->child; node != NULL; node = node->next) {
			if (PURPLE_IS_CHAT(node)) {

				chat = (PurpleChat*)node;

				if (account != chat->account)
					continue;

				parts = prpl_info->chat_info(
					purple_account_get_connection(chat->account));

				pce = parts->data;
				chat_name = g_hash_table_lookup(chat->components,
												pce->identifier);
				g_list_foreach(parts, (GFunc)g_free, NULL);
				g_list_free(parts);

				if (chat->account == account && chat_name != NULL &&
					normname != NULL && !strcmp(purple_normalize(account, chat_name), normname)) {
					g_free(normname);
					return chat;
				}
			}
		}
	}

	g_free(normname);
	return NULL;
}

PurpleBuddy *purple_find_buddy_in_group(PurpleAccount *account, const char *name,
		PurpleGroup *group)
{
	struct _purple_hbuddy hb;
	PurpleBuddy *ret;

	g_return_val_if_fail(purplebuddylist != NULL, NULL);
	g_return_val_if_fail(account != NULL, NULL);
	g_return_val_if_fail((name != NULL) && (*name != '\0'), NULL);

	hb.name = g_strdup(purple_normalize(account, name));
	hb.account = account;
	hb.group = (PurpleBlistNode*)group;

	ret = g_hash_table_lookup(purplebuddylist->buddies, &hb);
	g_free(hb.name);

	return ret;
}

PurpleGroup *
purple_chat_get_group(PurpleChat *chat)
{
	g_return_val_if_fail(chat != NULL, NULL);

	return (PurpleGroup *)(((PurpleBlistNode *)chat)->parent);
}

static void
append_buddy(gpointer key, gpointer value, gpointer user_data)
{
	GSList **list = user_data;
	*list = g_slist_prepend(*list, value);
}

GSList *
purple_blist_get_buddies()
{
	GSList *buddies = NULL;

	if (!purplebuddylist)
		return NULL;

	g_hash_table_foreach(purplebuddylist->buddies, append_buddy, &buddies);
	return buddies;
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
	purplebuddylist = g_object_new(PURPLE_BUDDY_LIST_TYPE, NULL);
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
purple_blist_finalize(GObject *object)
{
	PurpleBuddyList *purplebuddylist = PURPLE_BUDDY_LIST(object);
	PurpleBlistUiOps *ops = purple_blist_get_ui_ops();

	purple_debug(PURPLE_DEBUG_INFO, "blist", "Destroying\n");

	if (ops && ops->destroy)
		ops->destroy(purplebuddylist);
	parent_class->finalize(object);
}

static void
purple_blist_class_init(PurpleBuddyListClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);
	void *handle = purple_blist_get_handle();

	parent_class = g_type_class_peek_parent(klass);
	obj_class->finalize = purple_blist_finalize;

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

static void
purple_blist_instance_init(GTypeInstance *instance, gpointer class)
{
	PurpleBlistUiOps *ui_ops;
	GList *account;
	PurpleBuddyList *gbl = PURPLE_BUDDY_LIST(instance);
	PURPLE_DBUS_REGISTER_POINTER(gbl, PurpleBuddyList);

	ui_ops = purple_blist_get_ui_ops();

	#warning: This has to be set here or we cant add the buddies cache
	purplebuddylist = gbl;

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

	purple_blist_load();
}

static GType
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
