/*
 * gaim
 *
 * Gaim is the legal property of its developers, whose names are too numerous
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include "internal.h"
#include "blist.h"
#include "conversation.h"
#include "debug.h"
#include "notify.h"
#include "prefs.h"
#include "privacy.h"
#include "prpl.h"
#include "server.h"
#include "signals.h"
#include "util.h"
#include "xmlnode.h"

#define PATHSIZE 1024

GaimBuddyList *gaimbuddylist = NULL;
static GaimBlistUiOps *blist_ui_ops = NULL;
static guint blist_save_timer = 0;

struct gaim_blist_node_setting {
	enum {
		GAIM_BLIST_NODE_SETTING_BOOL,
		GAIM_BLIST_NODE_SETTING_INT,
		GAIM_BLIST_NODE_SETTING_STRING
	} type;
	union {
		gboolean boolean;
		int integer;
		char *string;
	} value;
};



/*****************************************************************************
 * Private Utility functions                                                 *
 *****************************************************************************/
static GaimBlistNode *gaim_blist_get_last_sibling(GaimBlistNode *node)
{
	GaimBlistNode *n = node;
	if (!n)
		return NULL;
	while (n->next)
		n = n->next;
	return n;
}

static GaimBlistNode *gaim_blist_get_last_child(GaimBlistNode *node)
{
	if (!node)
		return NULL;
	return gaim_blist_get_last_sibling(node->child);
}

struct _gaim_hbuddy {
	char *name;
	GaimAccount *account;
	GaimBlistNode *group;
};

static guint _gaim_blist_hbuddy_hash(struct _gaim_hbuddy *hb)
{
	return g_str_hash(hb->name);
}

static guint _gaim_blist_hbuddy_equal(struct _gaim_hbuddy *hb1, struct _gaim_hbuddy *hb2)
{
	return ((!strcmp(hb1->name, hb2->name)) && hb1->account == hb2->account && hb1->group == hb2->group);
}

static void _gaim_blist_hbuddy_free_key(struct _gaim_hbuddy *hb)
{
	g_free(hb->name);
	g_free(hb);
}

static void blist_pref_cb(const char *name, GaimPrefType type, gpointer value, gpointer data)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	GaimBlistNode *gnode, *cnode, *bnode;

	if (!ops || !ops->update)
		return;

	for (gnode = gaimbuddylist->root; gnode; gnode = gnode->next) {
		if (!GAIM_BLIST_NODE_IS_GROUP(gnode))
			continue;
		for (cnode = gnode->child; cnode; cnode = cnode->next) {
			if (GAIM_BLIST_NODE_IS_CONTACT(cnode)) {
				for (bnode = cnode->child; bnode; bnode = bnode->next) {
					if (!GAIM_BLIST_NODE_IS_BUDDY(bnode))
						continue;
					ops->update(gaimbuddylist, bnode);
				}
			} else if (GAIM_BLIST_NODE_IS_CHAT(cnode)) {
				ops->update(gaimbuddylist, cnode);
			}
		}
	}
}

void gaim_contact_compute_priority_buddy(GaimContact *contact)
{
	GaimBlistNode *bnode;
	GaimBuddy *new_priority = NULL;

	g_return_if_fail(contact != NULL);

	contact->priority = NULL;
	for (bnode = ((GaimBlistNode*)contact)->child;
			bnode != NULL;
			bnode = bnode->next)
	{
		GaimBuddy *buddy;
		GaimPresence *presence;

		if (!GAIM_BLIST_NODE_IS_BUDDY(bnode))
			continue;

		buddy = (GaimBuddy*)bnode;

		if (!gaim_account_is_connected(buddy->account))
			continue;
 		if (new_priority == NULL)
 			new_priority = buddy;
 		else
 		{
 			int cmp;
 
 			presence = gaim_buddy_get_presence(buddy);
 
 			cmp = gaim_presence_compare(gaim_buddy_get_presence(new_priority),
					gaim_buddy_get_presence(buddy));

 			if (cmp > 0 || (cmp == 0 &&
						gaim_prefs_get_bool("/core/contact/last_match")))
 			{
 				new_priority = buddy;
 			}
 		}
	}

	contact->priority = new_priority;
}

static gboolean blist_save_callback(gpointer data)
{
	gaim_blist_sync();
	blist_save_timer = 0;
	return FALSE;
}

static void schedule_blist_save()
{
	if (blist_save_timer != 0)
		gaim_timeout_remove(blist_save_timer);
	blist_save_timer = gaim_timeout_add(1000, blist_save_callback, NULL);
}


/*****************************************************************************
 * Public API functions                                                      *
 *****************************************************************************/

GaimBuddyList *gaim_blist_new()
{
	GaimBuddyList *gbl = g_new0(GaimBuddyList, 1);

	gbl->ui_ops = gaim_blist_get_ui_ops();

	gbl->buddies = g_hash_table_new_full((GHashFunc)_gaim_blist_hbuddy_hash,
					 (GEqualFunc)_gaim_blist_hbuddy_equal,
					 (GDestroyNotify)_gaim_blist_hbuddy_free_key, NULL);

	if (gbl->ui_ops != NULL && gbl->ui_ops->new_list != NULL)
		gbl->ui_ops->new_list(gbl);

	gaim_prefs_connect_callback("/core/buddies/use_server_alias",
								blist_pref_cb, NULL);

	return gbl;
}

void
gaim_set_blist(GaimBuddyList *list)
{
	gaimbuddylist = list;
}

GaimBuddyList *
gaim_get_blist()
{
	return gaimbuddylist;
}

void gaim_blist_show()
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;

	if (ops && ops->show)
		ops->show(gaimbuddylist);
}

void gaim_blist_destroy()
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;

	gaim_debug(GAIM_DEBUG_INFO, "blist", "Destroying\n");

	if (ops && ops->destroy)
		ops->destroy(gaimbuddylist);
}

void gaim_blist_set_visible(gboolean show)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;

	if (ops && ops->set_visible)
		ops->set_visible(gaimbuddylist, show);
}

void gaim_blist_update_buddy_status(GaimBuddy *buddy, int status)
{
#if 0
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	int old_status;

	g_return_if_fail(buddy != NULL);

	old_status = buddy->uc;
	if (old_status != status) {
		buddy->uc = status;
		gaim_contact_compute_priority_buddy(gaim_buddy_get_contact(buddy));

		if ((status & UC_UNAVAILABLE) != (old_status & UC_UNAVAILABLE)) {
			if (status & UC_UNAVAILABLE)
				gaim_signal_emit(gaim_blist_get_handle(), "buddy-away", buddy);
			else
				gaim_signal_emit(gaim_blist_get_handle(), "buddy-back", buddy);
		}
	}

	if (ops && ops->update)
		ops->update(gaimbuddylist, (GaimBlistNode*)buddy);
#endif
}

static gboolean presence_update_timeout_cb(GaimBuddy *buddy)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	GaimConversation *conv;

	g_return_val_if_fail(buddy != NULL, FALSE);

	if (buddy->present == GAIM_BUDDY_SIGNING_ON) {
		buddy->present = GAIM_BUDDY_ONLINE;
	} else if (buddy->present == GAIM_BUDDY_SIGNING_OFF) {
		buddy->present = GAIM_BUDDY_OFFLINE;
		((GaimContact*)((GaimBlistNode*)buddy)->parent)->online--;
		if (((GaimContact*)((GaimBlistNode*)buddy)->parent)->online == 0)
			((GaimGroup *)((GaimBlistNode *)buddy)->parent->parent)->online--;
	}

	buddy->timer = 0;

	if (ops && ops->update)
		ops->update(gaimbuddylist, (GaimBlistNode*)buddy);

	conv = gaim_find_conversation_with_account(buddy->name, buddy->account);
	if (conv) {
		if (buddy->present == GAIM_BUDDY_ONLINE)
			gaim_conversation_update(conv, GAIM_CONV_ACCOUNT_ONLINE);
		else if (buddy->present == GAIM_BUDDY_OFFLINE)
			gaim_conversation_update(conv, GAIM_CONV_ACCOUNT_OFFLINE);
	}

	return FALSE;
}

void gaim_blist_update_buddy_presence(GaimBuddy *buddy, gboolean online)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	gboolean did_something = FALSE;

	g_return_if_fail(buddy != NULL);

	if (!GAIM_BUDDY_IS_ONLINE(buddy) && online) {
		int old_present = buddy->present;
		buddy->present = GAIM_BUDDY_SIGNING_ON;
		gaim_signal_emit(gaim_blist_get_handle(), "buddy-signed-on", buddy);
		did_something = TRUE;

		if (old_present != GAIM_BUDDY_SIGNING_OFF) {
			((GaimContact*)((GaimBlistNode*)buddy)->parent)->online++;
			if (((GaimContact*)((GaimBlistNode*)buddy)->parent)->online == 1)
				((GaimGroup *)((GaimBlistNode *)buddy)->parent->parent)->online++;
		}
	} else if (GAIM_BUDDY_IS_ONLINE(buddy) && !online) {
		buddy->present = GAIM_BUDDY_SIGNING_OFF;
		gaim_signal_emit(gaim_blist_get_handle(), "buddy-signed-off", buddy);
		did_something = TRUE;
	}

	if (did_something) {
		if (buddy->timer > 0)
			gaim_timeout_remove(buddy->timer);
		buddy->timer = gaim_timeout_add(10000, (GSourceFunc)presence_update_timeout_cb, buddy);

		gaim_contact_compute_priority_buddy(gaim_buddy_get_contact(buddy));
		if (ops && ops->update)
			ops->update(gaimbuddylist, (GaimBlistNode *)buddy);
	}
}

void gaim_blist_update_buddy_signon(GaimBuddy *buddy, time_t signon)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;

	g_return_if_fail(buddy != NULL);

	if (buddy->signon == signon)
		return;

	buddy->signon = signon;

	if (ops && ops->update)
		ops->update(gaimbuddylist, (GaimBlistNode *)buddy);
}

#if 0
void gaim_blist_update_buddy_idle(GaimBuddy *buddy, int idle)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;

	g_return_if_fail(buddy != NULL);

	if (buddy->idle == idle)
		return;

	buddy->idle = idle;
	gaim_contact_compute_priority_buddy(gaim_buddy_get_contact(buddy));

	if (ops && ops->update)
		ops->update(gaimbuddylist, (GaimBlistNode *)buddy);
}
#endif

void gaim_blist_update_buddy_icon(GaimBuddy *buddy)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;

	g_return_if_fail(buddy != NULL);

	if (ops && ops->update)
		ops->update(gaimbuddylist, (GaimBlistNode *)buddy);
}

/*
 * XXX - Maybe remove the call to this from server.c and call it
 * from oscar.c and toc.c instead?
 */
void gaim_blist_rename_buddy(GaimBuddy *buddy, const char *name)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	struct _gaim_hbuddy *hb;

	g_return_if_fail(buddy != NULL);

	hb = g_new(struct _gaim_hbuddy, 1);
	hb->name = g_strdup(gaim_normalize(buddy->account, buddy->name));
	hb->account = buddy->account;
	hb->group = ((GaimBlistNode *)buddy)->parent->parent;
	g_hash_table_remove(gaimbuddylist->buddies, hb);

	g_free(hb->name);
	hb->name = g_strdup(gaim_normalize(buddy->account, name));
	g_hash_table_replace(gaimbuddylist->buddies, hb, buddy);

	g_free(buddy->name);
	buddy->name = g_strdup(name);

	schedule_blist_save();

	if (ops && ops->update)
		ops->update(gaimbuddylist, (GaimBlistNode *)buddy);
}

void gaim_blist_alias_chat(GaimChat *chat, const char *alias)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;

	g_return_if_fail(chat != NULL);

	g_free(chat->alias);
	if ((alias != NULL) && (*alias != '\0'))
		chat->alias = g_strdup(alias);
	else
		chat->alias = NULL;

	schedule_blist_save();

	if (ops && ops->update)
		ops->update(gaimbuddylist, (GaimBlistNode *)chat);
}

void gaim_blist_alias_buddy(GaimBuddy *buddy, const char *alias)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	GaimConversation *conv;

	g_return_if_fail(buddy != NULL);

	g_free(buddy->alias);
	if ((alias != NULL) && (*alias != '\0'))
		buddy->alias = g_strdup(alias);
	else
		buddy->alias = NULL;

	schedule_blist_save();

	if (ops && ops->update)
		ops->update(gaimbuddylist, (GaimBlistNode *)buddy);

	conv = gaim_find_conversation_with_account(buddy->name, buddy->account);
	if (conv)
		gaim_conversation_autoset_title(conv);
}

void gaim_blist_server_alias_buddy(GaimBuddy *buddy, const char *alias)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	GaimConversation *conv;

	g_return_if_fail(buddy != NULL);

	g_free(buddy->server_alias);
	if ((alias != NULL) && (*alias != '\0') && g_utf8_validate(alias, -1, NULL))
		buddy->server_alias = g_strdup(alias);
	else
		buddy->server_alias = NULL;

	schedule_blist_save();

	if (ops && ops->update)
		ops->update(gaimbuddylist, (GaimBlistNode *)buddy);

	conv = gaim_find_conversation_with_account(buddy->name, buddy->account);
	if (conv)
		gaim_conversation_autoset_title(conv);
}

/*
 * XXX - If merging, prompt the user if they want to merge.
 */
void gaim_blist_rename_group(GaimGroup *source, const char *new_name)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	GaimGroup *dest;
	gchar *old_name;
	GList *moved_buddies = NULL;
	GSList *accts;

	g_return_if_fail(source != NULL);
	g_return_if_fail(new_name != NULL);

	if (*new_name == '\0' || !strcmp(new_name, source->name))
		return;

	dest = gaim_find_group(new_name);
	if (dest != NULL) {
		/* We're merging two groups */
		GaimBlistNode *prev, *child, *next;

		prev = gaim_blist_get_last_child((GaimBlistNode*)dest);
		child = ((GaimBlistNode*)source)->child;

		/*
		 * XXX - This seems like a dumb way to do this... why not just
		 * append all children from the old group to the end of the new
		 * one?  PRPLs might be expecting to receive an add_buddy() for
		 * each moved buddy...
		 */
		while (child)
		{
			next = child->next;
			if (GAIM_BLIST_NODE_IS_CONTACT(child)) {
				GaimBlistNode *bnode;
				gaim_blist_add_contact((GaimContact *)child, dest, prev);
				for (bnode = child->child; bnode != NULL; bnode = bnode->next) {
					gaim_blist_add_buddy((GaimBuddy *)bnode, (GaimContact *)child,
							NULL, bnode->prev);
					moved_buddies = g_list_append(moved_buddies, bnode);
				}
				prev = child;
			} else if (GAIM_BLIST_NODE_IS_CHAT(child)) {
				gaim_blist_add_chat((GaimChat *)child, dest, prev);
				prev = child;
			} else {
				gaim_debug(GAIM_DEBUG_ERROR, "blist",
						"Unknown child type in group %s\n", source->name);
			}
			child = next;
		}

		/* Make a copy of the old group name and then delete the old group */
		old_name = g_strdup(source->name);
		gaim_blist_remove_group(source);
	} else {
		/* A simple rename */
		GaimBlistNode *cnode, *bnode;

		/* Build a GList of all buddies in this group */
		for (cnode = ((GaimBlistNode *)source)->child; cnode != NULL; cnode = cnode->next) {
			if (GAIM_BLIST_NODE_IS_CONTACT(cnode))
				for (bnode = cnode->child; bnode != NULL; bnode = bnode->next)
					moved_buddies = g_list_append(moved_buddies, bnode);
		}

		old_name = source->name;
		source->name = g_strdup(new_name);

	}

	/* Save our changes */
	schedule_blist_save();

	/* Update the UI */
	if (ops && ops->update)
		ops->update(gaimbuddylist, (GaimBlistNode*)source);

	/* Notify all PRPLs */
	for (accts = gaim_group_get_accounts(source); accts; accts = g_slist_remove(accts, accts->data)) {
		GaimAccount *account = accts->data;
		serv_rename_group(account->gc, old_name, source, moved_buddies);
	}
	g_list_free(moved_buddies);
	g_free(old_name);
}

static void gaim_blist_node_initialize_settings(GaimBlistNode *node);

GaimChat *gaim_chat_new(GaimAccount *account, const char *alias, GHashTable *components)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	GaimChat *chat;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(components != NULL, FALSE);

	chat = g_new0(GaimChat, 1);
	chat->account = account;
	if ((alias != NULL) && (*alias != '\0'))
		chat->alias = g_strdup(alias);
	chat->components = components;
	gaim_blist_node_initialize_settings((GaimBlistNode *)chat);
	((GaimBlistNode *)chat)->type = GAIM_BLIST_CHAT_NODE;

	if (ops != NULL && ops->new_node != NULL)
		ops->new_node((GaimBlistNode *)chat);

	return chat;
}

char *gaim_chat_get_display_name(GaimChat *chat)
{
	char *name;

	g_return_val_if_fail(chat != NULL, FALSE);

	if (chat->alias != NULL) {
		 name = g_strdup(chat->alias);
	} else {
		 GList *parts;
		 GaimPlugin *prpl;
		 GaimPluginProtocolInfo *prpl_info;
		 struct proto_chat_entry *pce;

		 prpl = gaim_find_prpl(gaim_account_get_protocol_id(chat->account));
		 prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(prpl);

		 parts = prpl_info->chat_info(chat->account->gc);

		 pce = parts->data;
		 name = g_markup_escape_text(g_hash_table_lookup(chat->components,
														 pce->identifier), -1);
		 g_list_free(parts);
	}

	return name;
}

GaimBuddy *gaim_buddy_new(GaimAccount *account, const char *screenname, const char *alias)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	GaimBuddy *buddy;

	g_return_val_if_fail(account != NULL, FALSE);
	g_return_val_if_fail(screenname != NULL, FALSE);

	buddy = g_new0(GaimBuddy, 1);
	buddy->account  = account;
	buddy->name     = g_strdup(screenname);
	buddy->alias    = g_strdup(alias);
	buddy->presence = gaim_presence_new_for_buddy(buddy);

	gaim_blist_node_initialize_settings((GaimBlistNode *)buddy);
	((GaimBlistNode *)buddy)->type = GAIM_BLIST_BUDDY_NODE;

	if (ops && ops->new_node)
		ops->new_node((GaimBlistNode *)buddy);

	return buddy;
}

void
gaim_buddy_set_icon(GaimBuddy *buddy, GaimBuddyIcon *icon)
{
	g_return_if_fail(buddy != NULL);

	if (buddy->icon != icon)
	{
		if (buddy->icon != NULL)
			gaim_buddy_icon_unref(buddy->icon);

		buddy->icon = (icon == NULL ? NULL : gaim_buddy_icon_ref(icon));
	}

	if (buddy->icon)
		gaim_buddy_icon_cache(icon, buddy);
	else
		gaim_blist_node_remove_setting((GaimBlistNode *)buddy, "buddy_icon");

	schedule_blist_save();

	gaim_blist_update_buddy_icon(buddy);
}

GaimBuddyIcon *
gaim_buddy_get_icon(const GaimBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);

	return buddy->icon;
}

void gaim_blist_add_chat(GaimChat *chat, GaimGroup *group, GaimBlistNode *node)
{
	GaimBlistNode *cnode = (GaimBlistNode*)chat;
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;

	g_return_if_fail(chat != NULL);
	g_return_if_fail(GAIM_BLIST_NODE_IS_CHAT((GaimBlistNode *)chat));

	if (node == NULL) {
		if (group == NULL) {
			group = gaim_group_new(_("Chats"));
			gaim_blist_add_group(group,
					gaim_blist_get_last_sibling(gaimbuddylist->root));
		}
	} else {
		group = (GaimGroup*)node->parent;
	}

	/* if we're moving to overtop of ourselves, do nothing */
	if (cnode == node)
		return;

	if (cnode->parent) {
		/* This chat was already in the list and is
		 * being moved.
		 */
		((GaimGroup *)cnode->parent)->totalsize--;
		if (gaim_account_is_connected(chat->account)) {
			((GaimGroup *)cnode->parent)->online--;
			((GaimGroup *)cnode->parent)->currentsize--;
		}
		if (cnode->next)
			cnode->next->prev = cnode->prev;
		if (cnode->prev)
			cnode->prev->next = cnode->next;
		if (cnode->parent->child == cnode)
			cnode->parent->child = cnode->next;

		ops->remove(gaimbuddylist, cnode);

		schedule_blist_save();
	}

	if (node != NULL) {
		if (node->next)
			node->next->prev = cnode;
		cnode->next = node->next;
		cnode->prev = node;
		cnode->parent = node->parent;
		node->next = cnode;
		((GaimGroup *)node->parent)->totalsize++;
		if (gaim_account_is_connected(chat->account)) {
			((GaimGroup *)node->parent)->online++;
			((GaimGroup *)node->parent)->currentsize++;
		}
	} else {
		if (((GaimBlistNode *)group)->child)
			((GaimBlistNode *)group)->child->prev = cnode;
		cnode->next = ((GaimBlistNode *)group)->child;
		cnode->prev = NULL;
		((GaimBlistNode *)group)->child = cnode;
		cnode->parent = (GaimBlistNode *)group;
		group->totalsize++;
		if (gaim_account_is_connected(chat->account)) {
			group->online++;
			group->currentsize++;
		}
	}

	schedule_blist_save();

	if (ops && ops->update)
		ops->update(gaimbuddylist, (GaimBlistNode *)cnode);
}

void gaim_blist_add_buddy(GaimBuddy *buddy, GaimContact *contact, GaimGroup *group, GaimBlistNode *node)
{
	GaimBlistNode *cnode, *bnode;
	GaimGroup *g;
	GaimContact *c;
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	struct _gaim_hbuddy *hb;

	g_return_if_fail(buddy != NULL);
	g_return_if_fail(GAIM_BLIST_NODE_IS_BUDDY((GaimBlistNode*)buddy));

	bnode = (GaimBlistNode *)buddy;

	/* if we're moving to overtop of ourselves, do nothing */
	if (bnode == node || (!node && bnode->parent &&
				contact && bnode->parent == (GaimBlistNode*)contact
				&& bnode == bnode->parent->child))
		return;

	if (node && GAIM_BLIST_NODE_IS_BUDDY(node)) {
		c = (GaimContact*)node->parent;
		g = (GaimGroup*)node->parent->parent;
	} else if (contact) {
		c = contact;
		g = (GaimGroup *)((GaimBlistNode *)c)->parent;
	} else {
		if (group) {
			g = group;
		} else {
			g = gaim_group_new(_("Buddies"));
			gaim_blist_add_group(g,
					gaim_blist_get_last_sibling(gaimbuddylist->root));
		}
		c = gaim_contact_new();
		gaim_blist_add_contact(c, g,
				gaim_blist_get_last_child((GaimBlistNode*)g));
	}

	cnode = (GaimBlistNode *)c;

	if (bnode->parent) {
		if (GAIM_BUDDY_IS_ONLINE(buddy)) {
			((GaimContact*)bnode->parent)->online--;
			if (((GaimContact*)bnode->parent)->online == 0)
				((GaimGroup*)bnode->parent->parent)->online--;
		}
		if (gaim_account_is_connected(buddy->account)) {
			((GaimContact*)bnode->parent)->currentsize--;
			if (((GaimContact*)bnode->parent)->currentsize == 0)
				((GaimGroup*)bnode->parent->parent)->currentsize--;
		}
		((GaimContact*)bnode->parent)->totalsize--;
		/* the group totalsize will be taken care of by remove_contact below */

		if (bnode->parent->parent != (GaimBlistNode*)g)
			serv_move_buddy(buddy, (GaimGroup *)bnode->parent->parent, g);

		if (bnode->next)
			bnode->next->prev = bnode->prev;
		if (bnode->prev)
			bnode->prev->next = bnode->next;
		if (bnode->parent->child == bnode)
			bnode->parent->child = bnode->next;

		ops->remove(gaimbuddylist, bnode);

		schedule_blist_save();

		if (bnode->parent->parent != (GaimBlistNode*)g) {
			hb = g_new(struct _gaim_hbuddy, 1);
			hb->name = g_strdup(gaim_normalize(buddy->account, buddy->name));
			hb->account = buddy->account;
			hb->group = bnode->parent->parent;
			g_hash_table_remove(gaimbuddylist->buddies, hb);
			g_free(hb->name);
			g_free(hb);
		}

		if (!bnode->parent->child) {
			gaim_blist_remove_contact((GaimContact*)bnode->parent);
		} else {
			gaim_contact_compute_priority_buddy((GaimContact*)bnode->parent);
			ops->update(gaimbuddylist, bnode->parent);
		}
	}

	if (node && GAIM_BLIST_NODE_IS_BUDDY(node)) {
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

	if (GAIM_BUDDY_IS_ONLINE(buddy)) {
		((GaimContact*)bnode->parent)->online++;
		if (((GaimContact*)bnode->parent)->online == 1)
			((GaimGroup*)bnode->parent->parent)->online++;
	}
	if (gaim_account_is_connected(buddy->account)) {
		((GaimContact*)bnode->parent)->currentsize++;
		if (((GaimContact*)bnode->parent)->currentsize == 1)
			((GaimGroup*)bnode->parent->parent)->currentsize++;
	}
	((GaimContact*)bnode->parent)->totalsize++;

	hb = g_new(struct _gaim_hbuddy, 1);
	hb->name = g_strdup(gaim_normalize(buddy->account, buddy->name));
	hb->account = buddy->account;
	hb->group = ((GaimBlistNode*)buddy)->parent->parent;

	g_hash_table_replace(gaimbuddylist->buddies, hb, buddy);

	gaim_contact_compute_priority_buddy(gaim_buddy_get_contact(buddy));

	schedule_blist_save();

	if (ops && ops->update)
		ops->update(gaimbuddylist, (GaimBlistNode*)buddy);
}

GaimContact *gaim_contact_new()
{
	GaimBlistUiOps *ops = gaim_blist_get_ui_ops();

	GaimContact *contact = g_new0(GaimContact, 1);
	contact->totalsize = 0;
	contact->currentsize = 0;
	contact->online = 0;
	gaim_blist_node_initialize_settings((GaimBlistNode *)contact);
	((GaimBlistNode *)contact)->type = GAIM_BLIST_CONTACT_NODE;

	if (ops && ops->new_node)
		ops->new_node((GaimBlistNode *)contact);

	return contact;
}

void gaim_contact_set_alias(GaimContact *contact, const char *alias)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;

	g_return_if_fail(contact != NULL);

	if (contact->alias != NULL)
		g_free(contact->alias);

	if ((alias != NULL) && (*alias != '\0'))
		contact->alias = g_strdup(alias);
	else
		contact->alias = NULL;

	schedule_blist_save();

	if (ops && ops->update)
		ops->update(gaimbuddylist, (GaimBlistNode*)contact);
}

const char *gaim_contact_get_alias(GaimContact* contact)
{
	g_return_val_if_fail(contact != NULL, NULL);

	if (contact->alias)
		return contact->alias;

	return gaim_buddy_get_alias(contact->priority);
}

gboolean gaim_contact_on_account(GaimContact *c, GaimAccount *account)
{
	GaimBlistNode *bnode, *cnode = (GaimBlistNode *) c;

	g_return_val_if_fail(c != NULL, FALSE);
	g_return_val_if_fail(account != NULL, FALSE);

	for (bnode = cnode->child; bnode; bnode = bnode->next) {
		GaimBuddy *buddy;

		if (! GAIM_BLIST_NODE_IS_BUDDY(bnode))
			continue;

		buddy = (GaimBuddy *)bnode;
		if (buddy->account == account)
			return TRUE;
	}
	return FALSE;
}

GaimGroup *gaim_group_new(const char *name)
{
	GaimBlistUiOps *ops = gaim_blist_get_ui_ops();
	GaimGroup *group = gaim_find_group(name);

	if (group != NULL)
		return group;

	group = g_new0(GaimGroup, 1);
	group->name = g_strdup(name);
	group->totalsize = 0;
	group->currentsize = 0;
	group->online = 0;
	gaim_blist_node_initialize_settings((GaimBlistNode *)group);
	((GaimBlistNode *)group)->type = GAIM_BLIST_GROUP_NODE;

	if (ops && ops->new_node)
		ops->new_node((GaimBlistNode *)group);

	return group;
}

void gaim_blist_add_contact(GaimContact *contact, GaimGroup *group, GaimBlistNode *node)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	GaimGroup *g;
	GaimBlistNode *gnode, *cnode, *bnode;

	g_return_if_fail(contact != NULL);
	g_return_if_fail(GAIM_BLIST_NODE_IS_CONTACT((GaimBlistNode*)contact));

	if ((GaimBlistNode*)contact == node)
		return;

	if (node && (GAIM_BLIST_NODE_IS_CONTACT(node) ||
				GAIM_BLIST_NODE_IS_CHAT(node)))
		g = (GaimGroup*)node->parent;
	else if (group)
		g = group;
	else {
		g = gaim_group_new(_("Buddies"));
		gaim_blist_add_group(g,
				gaim_blist_get_last_sibling(gaimbuddylist->root));
	}

	gnode = (GaimBlistNode*)g;
	cnode = (GaimBlistNode*)contact;

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
				GaimBlistNode *next_bnode = bnode->next;
				GaimBuddy *b = (GaimBuddy*)bnode;

				struct _gaim_hbuddy *hb = g_new(struct _gaim_hbuddy, 1);
				hb->name = g_strdup(gaim_normalize(b->account, b->name));
				hb->account = b->account;
				hb->group = cnode->parent;

				g_hash_table_remove(gaimbuddylist->buddies, hb);

				if (!gaim_find_buddy_in_group(b->account, b->name, g)) {
					hb->group = gnode;
					g_hash_table_replace(gaimbuddylist->buddies, hb, b);

					if (b->account->gc)
						serv_move_buddy(b, (GaimGroup *)cnode->parent, g);
				} else {
					gboolean empty_contact = FALSE;

					/* this buddy already exists in the group, so we're
					 * gonna delete it instead */
					g_free(hb->name);
					g_free(hb);
					if (b->account->gc)
						serv_remove_buddy(b->account->gc, b, (GaimGroup *)cnode->parent);

					if (!cnode->child->next)
						empty_contact = TRUE;
					gaim_blist_remove_buddy(b);

					/** in gaim_blist_remove_buddy(), if the last buddy in a
					 * contact is removed, the contact is cleaned up and
					 * g_free'd, so we mustn't try to reference bnode->next */
					if (empty_contact)
						return;
				}
				bnode = next_bnode;
			}
		}

		if (contact->online > 0)
			((GaimGroup*)cnode->parent)->online--;
		if (contact->currentsize > 0)
			((GaimGroup*)cnode->parent)->currentsize--;
		((GaimGroup*)cnode->parent)->totalsize--;

		ops->remove(gaimbuddylist, cnode);

		schedule_blist_save();
	}

	if (node && (GAIM_BLIST_NODE_IS_CONTACT(node) ||
				GAIM_BLIST_NODE_IS_CHAT(node))) {
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

	schedule_blist_save();

	if (ops && cnode->child)
		ops->update(gaimbuddylist, cnode);

	for (bnode = cnode->child; bnode; bnode = bnode->next)
		ops->update(gaimbuddylist, bnode);
}

void gaim_blist_merge_contact(GaimContact *source, GaimBlistNode *node)
{
	GaimBlistNode *sourcenode = (GaimBlistNode*)source;
	GaimBlistNode *targetnode;
	GaimBlistNode *prev, *cur, *next;
	GaimContact *target;

	g_return_if_fail(source != NULL);
	g_return_if_fail(node != NULL);

	if (GAIM_BLIST_NODE_IS_CONTACT(node)) {
		target = (GaimContact *)node;
		prev = gaim_blist_get_last_child(node);
	} else if (GAIM_BLIST_NODE_IS_BUDDY(node)) {
		target = (GaimContact *)node->parent;
		prev = node;
	} else {
		return;
	}

	if (source == target || !target)
		return;

	targetnode = (GaimBlistNode *)target;
	next = sourcenode->child;

	while (next) {
		cur = next;
		next = cur->next;
		if (GAIM_BLIST_NODE_IS_BUDDY(cur)) {
			gaim_blist_add_buddy((GaimBuddy *)cur, target, NULL, prev);
			prev = cur;
		}
	}
}

void gaim_blist_add_group(GaimGroup *group, GaimBlistNode *node)
{
	GaimBlistUiOps *ops;
	GaimBlistNode *gnode = (GaimBlistNode*)group;

	g_return_if_fail(group != NULL);
	g_return_if_fail(GAIM_BLIST_NODE_IS_GROUP((GaimBlistNode *)group));

	ops = gaimbuddylist->ui_ops;

	if (!gaimbuddylist->root) {
		gaimbuddylist->root = gnode;
		return;
	}

	/* if we're moving to overtop of ourselves, do nothing */
	if (gnode == node)
		return;

	if (gaim_find_group(group->name)) {
		/* This is just being moved */

		ops->remove(gaimbuddylist, (GaimBlistNode *)group);

		if (gnode == gaimbuddylist->root)
			gaimbuddylist->root = gnode->next;
		if (gnode->prev)
			gnode->prev->next = gnode->next;
		if (gnode->next)
			gnode->next->prev = gnode->prev;
	}

	if (node && GAIM_BLIST_NODE_IS_GROUP(node)) {
		gnode->next = node->next;
		gnode->prev = node;
		if (node->next)
			node->next->prev = gnode;
		node->next = gnode;
	} else {
		if (gaimbuddylist->root)
			gaimbuddylist->root->prev = gnode;
		gnode->next = gaimbuddylist->root;
		gnode->prev = NULL;
		gaimbuddylist->root = gnode;
	}

	schedule_blist_save();

	if (ops && ops->update) {
		ops->update(gaimbuddylist, gnode);
		for (node = gnode->child; node; node = node->next)
			ops->update(gaimbuddylist, node);
	}
}

void gaim_blist_remove_contact(GaimContact *contact)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	GaimBlistNode *node, *gnode;

	g_return_if_fail(contact != NULL);

	node = (GaimBlistNode *)contact;
	gnode = node->parent;

	if (node->child) {
		/*
		 * If this contact has children then remove them.  When the last
		 * buddy is removed from the contact, the contact is deleted.
		 */
		while (node->child) {
			gaim_blist_remove_buddy((GaimBuddy*)node->child);
		}
	} else {
		/* Remove the node from its parent */
		if (gnode->child == node)
			gnode->child = node->next;
		if (node->prev)
			node->prev->next = node->next;
		if (node->next)
			node->next->prev = node->prev;

		schedule_blist_save();

		/* Update the UI */
		if (ops && ops->remove)
			ops->remove(gaimbuddylist, node);

		/* Delete the node */
		g_free(contact);
	}
}

void gaim_blist_remove_buddy(GaimBuddy *buddy)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	GaimBlistNode *node, *cnode, *gnode;
	GaimContact *contact;
	GaimGroup *group;
	struct _gaim_hbuddy hb;

	g_return_if_fail(buddy != NULL);

	node = (GaimBlistNode *)buddy;
	cnode = node->parent;
	gnode = cnode->parent;
	contact = (GaimContact *)cnode;
	group = (GaimGroup *)gnode;

	/* Remove the node from its parent */
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	if (cnode->child == node)
		cnode->child = node->next;

	/* Adjust size counts */
	if (GAIM_BUDDY_IS_ONLINE(buddy)) {
		contact->online--;
		if (contact->online == 0)
			group->online--;
	}
	if (gaim_account_is_connected(buddy->account)) {
		contact->currentsize--;
		if (contact->currentsize == 0)
			group->currentsize--;
	}
	contact->totalsize--;

	schedule_blist_save();

	/* Re-sort the contact */
	if (contact->priority == buddy) {
		gaim_contact_compute_priority_buddy(contact);
		if (ops && ops->update)
			ops->update(gaimbuddylist, cnode);
	}

	/* Remove this buddy from the buddies hash table */
	hb.name = g_strdup(gaim_normalize(buddy->account, buddy->name));
	hb.account = buddy->account;
	hb.group = ((GaimBlistNode*)buddy)->parent->parent;
	g_hash_table_remove(gaimbuddylist->buddies, &hb);
	g_free(hb.name);

	/* Update the UI */
	if (ops && ops->remove)
		ops->remove(gaimbuddylist, node);

	/* Delete the node */
	if (buddy->timer > 0)
		gaim_timeout_remove(buddy->timer);
	if (buddy->icon != NULL)
		gaim_buddy_icon_unref(buddy->icon);
	g_hash_table_destroy(buddy->node.settings);
	gaim_presence_remove_buddy(buddy->presence, buddy);
	gaim_presence_destroy(buddy->presence);
	g_free(buddy->name);
	g_free(buddy->alias);
	g_free(buddy);

	/* If the contact is empty then remove it */
	if (!cnode->child)
		gaim_blist_remove_contact(contact);
}

void gaim_blist_remove_chat(GaimChat *chat)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	GaimBlistNode *node, *gnode;
	GaimGroup *group;

	g_return_if_fail(chat != NULL);

	node = (GaimBlistNode *)chat;
	gnode = node->parent;
	group = (GaimGroup *)gnode;

	/* Remove the node from its parent */
	if (gnode->child == node)
		gnode->child = node->next;
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;

	/* Adjust size counts */
	if (gaim_account_is_connected(chat->account)) {
		group->online--;
		group->currentsize--;
	}
	group->totalsize--;

	schedule_blist_save();

	/* Update the UI */
	if (ops && ops->remove)
		ops->remove(gaimbuddylist, node);

	/* Delete the node */
	g_hash_table_destroy(chat->components);
	g_free(chat->alias);
	g_free(chat);
}

void gaim_blist_remove_group(GaimGroup *group)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	GaimBlistNode *node;
	GList *l;

	g_return_if_fail(group != NULL);

	node = (GaimBlistNode *)group;

	/* Make sure the group is empty */
	if (node->child) {
		char *buf;
		int count = 0;
		GaimBlistNode *child;

		for (child = node->child; child != NULL; child = child->next)
			count++;

		buf = g_strdup_printf(ngettext("%d buddy from group %s was not removed "
									   "because its account was not logged in."
									   "  This buddy and the group were not "
									   "removed.\n", 
									   "%d buddies from group %s were not "
									   "removed because their accounts were "
									   "not logged in.  These buddies and "
									   "the group were not removed.\n", count),
							  count, group->name);
		gaim_notify_error(NULL, NULL, _("Group not removed"), buf);
		g_free(buf);
		return;
	}

	/* Remove the node from its parent */
	if (gaimbuddylist->root == node)
		gaimbuddylist->root = node->next;
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;

	schedule_blist_save();

	/* Update the UI */
	if (ops && ops->remove)
		ops->remove(gaimbuddylist, node);

	/* Remove the group from all accounts that are online */
	for (l = gaim_connections_get_all(); l != NULL; l = l->next)
	{
		GaimConnection *gc = (GaimConnection *)l->data;

		if (gaim_connection_get_state(gc) == GAIM_CONNECTED)
			serv_remove_group(gc, group);
	}

	/* Delete the node */
	g_free(group->name);
	g_free(group);
}

GaimBuddy *gaim_contact_get_priority_buddy(GaimContact *contact)
{
	g_return_val_if_fail(contact != NULL, NULL);

	return contact->priority;
}

const char *gaim_buddy_get_alias_only(GaimBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);

	if ((buddy->alias != NULL) && (*buddy->alias != '\0')) {
		return buddy->alias;
	} else if ((buddy->server_alias != NULL) &&
			   (*buddy->server_alias != '\0') &&
			   (gaim_prefs_get_bool("/core/buddies/use_server_alias"))) {

		return buddy->server_alias;
	}

	return NULL;
}


const char *gaim_buddy_get_contact_alias(GaimBuddy *buddy)
{
	GaimContact *c;

	g_return_val_if_fail(buddy != NULL, NULL);

	/* Search for an alias for the buddy. In order of precedence: */
	/* The buddy alias */
	if (buddy->alias != NULL)
		return buddy->alias;

	/* The contact alias */
	c = gaim_buddy_get_contact(buddy);
	if ((c != NULL) && (c->alias != NULL))
		return c->alias;

	/* The server alias, if preferences say so */
	if ((buddy->server_alias) && (*buddy->server_alias) &&
			(gaim_prefs_get_bool("/core/buddies/use_server_alias")))
		return buddy->server_alias;

	/* The buddy's user name (i.e. no alias) */
	return buddy->name;
}


const char *gaim_buddy_get_alias(GaimBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);

	/* Search for an alias for the buddy. In order of precedence: */
	/* The buddy alias */
	if (buddy->alias != NULL)
		return buddy->alias;

	/* The server alias, if preferences say so */
	if ((buddy->server_alias) && (*buddy->server_alias) &&
			(gaim_prefs_get_bool("/core/buddies/use_server_alias")))
		return buddy->server_alias;

	/* The buddy's user name (i.e. no alias) */
	return buddy->name;
}


const char *gaim_chat_get_name(GaimChat *chat)
{
	struct proto_chat_entry *pce;
	GList *parts, *tmp;
	char *ret;

	g_return_val_if_fail(chat != NULL, NULL);

	if ((chat->alias != NULL) && (*chat->alias != '\0'))
		return chat->alias;

	parts = GAIM_PLUGIN_PROTOCOL_INFO(chat->account->gc->prpl)->chat_info(chat->account->gc);
	pce = parts->data;
	ret = g_hash_table_lookup(chat->components, pce->identifier);
	for (tmp = parts; tmp; tmp = tmp->next)
		g_free(tmp->data);
	g_list_free(parts);

	return ret;
}

GaimBuddy *gaim_find_buddy(GaimAccount *account, const char *name)
{
	GaimBuddy *buddy;
	struct _gaim_hbuddy hb;
	GaimBlistNode *group;

	g_return_val_if_fail(gaimbuddylist != NULL, NULL);
	g_return_val_if_fail(account != NULL, NULL);
	g_return_val_if_fail((name != NULL) && (*name != '\0'), NULL);

	hb.account = account;
	hb.name = g_strdup(gaim_normalize(account, name));

	for (group = gaimbuddylist->root; group; group = group->next) {
		hb.group = group;
		if ((buddy = g_hash_table_lookup(gaimbuddylist->buddies, &hb))) {
			g_free(hb.name);
			return buddy;
		}
	}
	g_free(hb.name);

	return NULL;
}

GaimBuddy *gaim_find_buddy_in_group(GaimAccount *account, const char *name,
		GaimGroup *group)
{
	struct _gaim_hbuddy hb;
	GaimBuddy *ret;

	g_return_val_if_fail(gaimbuddylist != NULL, NULL);
	g_return_val_if_fail(account != NULL, NULL);
	g_return_val_if_fail((name != NULL) && (*name != '\0'), NULL);

	hb.name = g_strdup(gaim_normalize(account, name));
	hb.account = account;
	hb.group = (GaimBlistNode*)group;

	ret = g_hash_table_lookup(gaimbuddylist->buddies, &hb);
	g_free(hb.name);

	return ret;
}

GSList *gaim_find_buddies(GaimAccount *account, const char *name)
{
	struct buddy *buddy;
	struct _gaim_hbuddy hb;
	GaimBlistNode *node;
	GSList *ret = NULL;

	g_return_val_if_fail(gaimbuddylist != NULL, NULL);
	g_return_val_if_fail(account != NULL, NULL);
	g_return_val_if_fail((name != NULL) && (*name != '\0'), NULL);

	hb.name = g_strdup(gaim_normalize(account, name));
	hb.account = account;

	for (node = gaimbuddylist->root; node != NULL; node = node->next) {
		hb.group = node;
		if ((buddy = g_hash_table_lookup(gaimbuddylist->buddies, &hb)) != NULL)
			ret = g_slist_append(ret, buddy);
	}
	g_free(hb.name);

	return ret;
}

GaimGroup *gaim_find_group(const char *name)
{
	GaimBlistNode *node;

	g_return_val_if_fail(gaimbuddylist != NULL, NULL);
	g_return_val_if_fail((name != NULL) && (*name != '\0'), NULL);

	for (node = gaimbuddylist->root; node != NULL; node = node->next) {
		if (!strcmp(((GaimGroup *)node)->name, name))
			return (GaimGroup *)node;
	}

	return NULL;
}

GaimChat *
gaim_blist_find_chat(GaimAccount *account, const char *name)
{
	char *chat_name;
	GaimChat *chat;
	GaimPlugin *prpl;
	GaimPluginProtocolInfo *prpl_info = NULL;
	struct proto_chat_entry *pce;
	GaimBlistNode *node, *group;
	GList *parts;

	g_return_val_if_fail(gaimbuddylist != NULL, NULL);
	g_return_val_if_fail((name != NULL) && (*name != '\0'), NULL);

	if (!gaim_account_is_connected(account))
		return NULL;

	prpl = gaim_find_prpl(gaim_account_get_protocol_id(account));
	prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(prpl);

	if (prpl_info->find_blist_chat != NULL)
		return prpl_info->find_blist_chat(account, name);

	for (group = gaimbuddylist->root; group != NULL; group = group->next) {
		for (node = group->child; node != NULL; node = node->next) {
			if (GAIM_BLIST_NODE_IS_CHAT(node)) {

				chat = (GaimChat*)node;

				if (account != chat->account)
					continue;

				parts = prpl_info->chat_info(
					gaim_account_get_connection(chat->account));

				pce = parts->data;
				chat_name = g_hash_table_lookup(chat->components,
												pce->identifier);

				if (chat->account == account && chat_name != NULL &&
					name != NULL && !strcmp(chat_name, name)) {

					return chat;
				}
			}
		}
	}

	return NULL;
}

GaimGroup *
gaim_chat_get_group(GaimChat *chat)
{
	g_return_val_if_fail(chat != NULL, NULL);

	return (GaimGroup *)(((GaimBlistNode *)chat)->parent);
}

GaimContact *gaim_buddy_get_contact(GaimBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);

	return (GaimContact*)((GaimBlistNode*)buddy)->parent;
}

GaimPresence *gaim_buddy_get_presence(const GaimBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);
 	return buddy->presence;
}


GaimGroup *gaim_find_buddys_group(GaimBuddy *buddy)
{
	g_return_val_if_fail(buddy != NULL, NULL);

	if (((GaimBlistNode *)buddy)->parent == NULL)
		return NULL;

	return (GaimGroup *)(((GaimBlistNode*)buddy)->parent->parent);
}

GSList *gaim_group_get_accounts(GaimGroup *group)
{
	GSList *l = NULL;
	GaimBlistNode *gnode, *cnode, *bnode;

	gnode = (GaimBlistNode *)group;

	for (cnode = gnode->child;  cnode; cnode = cnode->next) {
		if (GAIM_BLIST_NODE_IS_CHAT(cnode)) {
			if (!g_slist_find(l, ((GaimChat *)cnode)->account))
				l = g_slist_append(l, ((GaimChat *)cnode)->account);
		} else if (GAIM_BLIST_NODE_IS_CONTACT(cnode)) {
			for (bnode = cnode->child; bnode; bnode = bnode->next) {
				if (GAIM_BLIST_NODE_IS_BUDDY(bnode)) {
					if (!g_slist_find(l, ((GaimBuddy *)bnode)->account))
						l = g_slist_append(l, ((GaimBuddy *)bnode)->account);
				}
			}
		}
	}

	return l;
}

void gaim_blist_add_account(GaimAccount *account)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	GaimBlistNode *gnode, *cnode, *bnode;

	g_return_if_fail(gaimbuddylist != NULL);

	if (!ops || !ops->update)
		return;

	for (gnode = gaimbuddylist->root; gnode; gnode = gnode->next) {
		if (!GAIM_BLIST_NODE_IS_GROUP(gnode))
			continue;
		for (cnode = gnode->child; cnode; cnode = cnode->next) {
			if (GAIM_BLIST_NODE_IS_CONTACT(cnode)) {
				gboolean recompute = FALSE;
					for (bnode = cnode->child; bnode; bnode = bnode->next) {
						if (GAIM_BLIST_NODE_IS_BUDDY(bnode) &&
								((GaimBuddy*)bnode)->account == account) {
							recompute = TRUE;
							((GaimContact*)cnode)->currentsize++;
							if (((GaimContact*)cnode)->currentsize == 1)
								((GaimGroup*)gnode)->currentsize++;
							ops->update(gaimbuddylist, bnode);
						}
					}
					if (recompute ||
							gaim_blist_node_get_bool(cnode, "show_offline")) {
						gaim_contact_compute_priority_buddy((GaimContact*)cnode);
						ops->update(gaimbuddylist, cnode);
					}
			} else if (GAIM_BLIST_NODE_IS_CHAT(cnode) &&
					((GaimChat*)cnode)->account == account) {
				((GaimGroup *)gnode)->online++;
				((GaimGroup *)gnode)->currentsize++;
				ops->update(gaimbuddylist, cnode);
			}
		}
		ops->update(gaimbuddylist, gnode);
	}
}

void gaim_blist_remove_account(GaimAccount *account)
{
	GaimBlistUiOps *ops = gaimbuddylist->ui_ops;
	GaimBlistNode *gnode, *cnode, *bnode;

	g_return_if_fail(gaimbuddylist != NULL);

	for (gnode = gaimbuddylist->root; gnode; gnode = gnode->next) {
		if (!GAIM_BLIST_NODE_IS_GROUP(gnode))
			continue;
		for (cnode = gnode->child; cnode; cnode = cnode->next) {
			if (GAIM_BLIST_NODE_IS_CONTACT(cnode)) {
				gboolean recompute = FALSE;
				for (bnode = cnode->child; bnode; bnode = bnode->next) {
					if (!GAIM_BLIST_NODE_IS_BUDDY(bnode))
						continue;
					if (account == ((GaimBuddy *)bnode)->account) {
						recompute = TRUE;
						if (((GaimBuddy*)bnode)->present == GAIM_BUDDY_ONLINE ||
								((GaimBuddy*)bnode)->present == GAIM_BUDDY_SIGNING_ON) {
							((GaimContact*)cnode)->online--;
							if (((GaimContact*)cnode)->online == 0)
								((GaimGroup*)gnode)->online--;
						}
						((GaimContact*)cnode)->currentsize--;
						if (((GaimContact*)cnode)->currentsize == 0)
							((GaimGroup*)gnode)->currentsize--;

						((GaimBuddy*)bnode)->present = GAIM_BUDDY_OFFLINE;

						((GaimBuddy*)bnode)->uc = 0;
						/* XXX ((GaimBuddy*)bnode)->idle = 0; */


						if (ops && ops->remove)
							ops->remove(gaimbuddylist, bnode);
					}
				}
				if (recompute) {
					gaim_contact_compute_priority_buddy((GaimContact*)cnode);
					if (ops && ops->update)
						ops->update(gaimbuddylist, cnode);
				}
			} else if (GAIM_BLIST_NODE_IS_CHAT(cnode) &&
					((GaimChat*)cnode)->account == account) {
				((GaimGroup*)gnode)->currentsize--;
				((GaimGroup*)gnode)->online--;
				if (ops && ops->remove)
					ops->remove(gaimbuddylist, cnode);
			}
		}
	}
}

gboolean gaim_group_on_account(GaimGroup *g, GaimAccount *account)
{
	GaimBlistNode *cnode;
	for (cnode = ((GaimBlistNode *)g)->child; cnode; cnode = cnode->next) {
		if (GAIM_BLIST_NODE_IS_CONTACT(cnode)) {
			if(gaim_contact_on_account((GaimContact *) cnode, account))
				return TRUE;
		} else if (GAIM_BLIST_NODE_IS_CHAT(cnode)) {
			GaimChat *chat = (GaimChat *)cnode;
			if ((!account && gaim_account_is_connected(chat->account))
					|| chat->account == account)
				return TRUE;
		}
	}
	return FALSE;
}

static gboolean blist_safe_to_write = FALSE;

static void parse_setting(GaimBlistNode *node, xmlnode *setting)
{
	const char *name = xmlnode_get_attrib(setting, "name");
	const char *type = xmlnode_get_attrib(setting, "type");
	char *value = xmlnode_get_data(setting);

	if (!value)
		return;

	if (!type || !strcmp(type, "string"))
		gaim_blist_node_set_string(node, name, value);
	else if (!strcmp(type, "bool"))
		gaim_blist_node_set_bool(node, name, atoi(value));
	else if (!strcmp(type, "int"))
		gaim_blist_node_set_int(node, name, atoi(value));

	g_free(value);
}

static void parse_buddy(GaimGroup *group, GaimContact *contact, xmlnode *bnode)
{
	GaimAccount *account;
	GaimBuddy *buddy;
	char *name = NULL, *alias = NULL;
	const char *acct_name, *proto, *protocol;
	xmlnode *x;

	acct_name = xmlnode_get_attrib(bnode, "account");
	protocol = xmlnode_get_attrib(bnode, "protocol");
	proto = xmlnode_get_attrib(bnode, "proto");

	if (!acct_name || (!proto && !protocol))
		return;

	account = gaim_accounts_find(acct_name, proto ? proto : protocol);

	if (!account)
		return;

	if ((x = xmlnode_get_child(bnode, "name")))
		name = xmlnode_get_data(x);

	if (!name)
		return;

	if ((x = xmlnode_get_child(bnode, "alias")))
		alias = xmlnode_get_data(x);

	buddy = gaim_buddy_new(account, name, alias);
	gaim_blist_add_buddy(buddy, contact, group,
			gaim_blist_get_last_child((GaimBlistNode*)contact));

	for (x = xmlnode_get_child(bnode, "setting"); x; x = xmlnode_get_next_twin(x)) {
		parse_setting((GaimBlistNode*)buddy, x);
	}

	g_free(name);
	if (alias)
		g_free(alias);
}

static void parse_contact(GaimGroup *group, xmlnode *cnode)
{
	GaimContact *contact = gaim_contact_new();
	xmlnode *x;
	const char *alias;

	gaim_blist_add_contact(contact, group,
			gaim_blist_get_last_child((GaimBlistNode*)group));

	if ((alias = xmlnode_get_attrib(cnode, "alias"))) {
		gaim_contact_set_alias(contact, alias);
	}

	for (x = cnode->child; x; x = x->next) {
		if (x->type != XMLNODE_TYPE_TAG)
			continue;
		if (!strcmp(x->name, "buddy"))
			parse_buddy(group, contact, x);
		else if (!strcmp(x->name, "setting"))
			parse_setting((GaimBlistNode*)contact, x);
	}

	/* if the contact is empty, don't keep it around.  it causes problems */
	if (!((GaimBlistNode*)contact)->child)
		gaim_blist_remove_contact(contact);
}

static void parse_chat(GaimGroup *group, xmlnode *cnode)
{
	GaimChat *chat;
	GaimAccount *account;
	const char *acct_name, *proto, *protocol;
	xmlnode *x;
	char *alias = NULL;
	GHashTable *components;

	acct_name = xmlnode_get_attrib(cnode, "account");
	protocol = xmlnode_get_attrib(cnode, "protocol");
	proto = xmlnode_get_attrib(cnode, "proto");

	if (!acct_name || (!proto && !protocol))
		return;

	account = gaim_accounts_find(acct_name, proto ? proto : protocol);

	if (!account)
		return;

	if ((x = xmlnode_get_child(cnode, "alias")))
		alias = xmlnode_get_data(x);

	components = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

	for (x = xmlnode_get_child(cnode, "component"); x; x = xmlnode_get_next_twin(x)) {
		const char *name;
		char *value;

		name = xmlnode_get_attrib(x, "name");
		value = xmlnode_get_data(x);
		g_hash_table_replace(components, g_strdup(name), value);
	}

	chat = gaim_chat_new(account, alias, components);
	gaim_blist_add_chat(chat, group,
			gaim_blist_get_last_child((GaimBlistNode*)group));

	for (x = xmlnode_get_child(cnode, "setting"); x; x = xmlnode_get_next_twin(x)) {
		parse_setting((GaimBlistNode*)chat, x);
	}

	if (alias)
		g_free(alias);
}


static void parse_group(xmlnode *groupnode)
{
	const char *name = xmlnode_get_attrib(groupnode, "name");
	GaimGroup *group;
	xmlnode *cnode;

	if (!name)
		name = _("Buddies");

	group = gaim_group_new(name);
	gaim_blist_add_group(group,
			gaim_blist_get_last_sibling(gaimbuddylist->root));

	for (cnode = groupnode->child; cnode; cnode = cnode->next) {
		if (cnode->type != XMLNODE_TYPE_TAG)
			continue;
		if (!strcmp(cnode->name, "setting"))
			parse_setting((GaimBlistNode*)group, cnode);
		else if (!strcmp(cnode->name, "contact") ||
				!strcmp(cnode->name, "person"))
			parse_contact(group, cnode);
		else if (!strcmp(cnode->name, "chat"))
			parse_chat(group, cnode);
	}
}

static gboolean gaim_blist_read(const char *filename)
{
	GError *error;
	gchar *contents = NULL;
	gsize length;
	xmlnode *gaim, *blist, *privacy;

	gaim_debug(GAIM_DEBUG_INFO, "blist import",
			   "Reading %s\n", filename);
	if (!g_file_get_contents(filename, &contents, &length, &error)) {
		gaim_debug(GAIM_DEBUG_ERROR, "blist import",
				   "Error reading blist: %s\n", error->message);
		g_error_free(error);
		return FALSE;
	}

	gaim = xmlnode_from_str(contents, length);
	
	if (!gaim) {
		FILE *backup;
		char *name;
		gaim_debug(GAIM_DEBUG_ERROR, "blist import", "Error parsing %s\n",
				filename);
		name = g_build_filename(gaim_user_dir(), "blist.xml~", NULL);

		if ((backup = fopen(name, "w"))) {
			fwrite(contents, length, 1, backup);
			fclose(backup);
			chmod(name, S_IRUSR | S_IWUSR);
		} else {
			gaim_debug(GAIM_DEBUG_ERROR, "blist load", "Unable to write backup %s\n",
				   name);
		}
		g_free(name);
		g_free(contents);
		return FALSE;
	}
	
	g_free(contents);
	
	blist = xmlnode_get_child(gaim, "blist");
	if (blist) {
		xmlnode *groupnode;
		for (groupnode = xmlnode_get_child(blist, "group"); groupnode;
				groupnode = xmlnode_get_next_twin(groupnode)) {
			parse_group(groupnode);
		}
	}

	privacy = xmlnode_get_child(gaim, "privacy");
	if (privacy) {
		xmlnode *anode;
		for (anode = privacy->child; anode; anode = anode->next) {
			xmlnode *x;
			GaimAccount *account;
			const char *acct_name, *proto, *mode, *protocol;

			acct_name = xmlnode_get_attrib(anode, "name");
			protocol = xmlnode_get_attrib(anode, "protocol");
			proto = xmlnode_get_attrib(anode, "proto");
			mode = xmlnode_get_attrib(anode, "mode");

			if (!acct_name || (!proto && !protocol) || !mode)
				continue;

			account = gaim_accounts_find(acct_name, proto ? proto : protocol);

			if (!account)
				continue;

			account->perm_deny = atoi(mode);

			for (x = anode->child; x; x = x->next) {
				char *name;
				if (x->type != XMLNODE_TYPE_TAG)
					continue;

				if (!strcmp(x->name, "permit")) {
					name = xmlnode_get_data(x);
					gaim_privacy_permit_add(account, name, TRUE);
					g_free(name);
				} else if (!strcmp(x->name, "block")) {
					name = xmlnode_get_data(x);
					gaim_privacy_deny_add(account, name, TRUE);
					g_free(name);
				}
			}
		}
	}

	gaim_debug(GAIM_DEBUG_INFO, "blist import", "Finished reading %s\n",
			   filename);

	xmlnode_free(gaim);
	return TRUE;
}

void gaim_blist_load()
{
	char *user_dir = gaim_user_dir();
	char *filename;
	char *msg;

	blist_safe_to_write = TRUE;

	if (!user_dir)
		return;

	filename = g_build_filename(user_dir, "blist.xml", NULL);

	if (g_file_test(filename, G_FILE_TEST_EXISTS)) {
		if (!gaim_blist_read(filename)) {
			msg = g_strdup_printf(_("An error was encountered parsing your "
						"buddy list.  It has not been loaded, "
						"and the old file has moved to blist.xml~."));
			gaim_notify_error(NULL, NULL, _("Buddy List Error"), msg);
			g_free(msg);
		}
	}

	g_free(filename);
}

void
gaim_blist_request_add_buddy(GaimAccount *account, const char *username,
							 const char *group, const char *alias)
{
	GaimBlistUiOps *ui_ops;

	ui_ops = gaim_blist_get_ui_ops();

	if (ui_ops != NULL && ui_ops->request_add_buddy != NULL)
		ui_ops->request_add_buddy(account, username, group, alias);
}

void
gaim_blist_request_add_chat(GaimAccount *account, GaimGroup *group,
							const char *alias, const char *name)
{
	GaimBlistUiOps *ui_ops;

	ui_ops = gaim_blist_get_ui_ops();

	if (ui_ops != NULL && ui_ops->request_add_chat != NULL)
		ui_ops->request_add_chat(account, group, alias, name);
}

void
gaim_blist_request_add_group(void)
{
	GaimBlistUiOps *ui_ops;

	ui_ops = gaim_blist_get_ui_ops();

	if (ui_ops != NULL && ui_ops->request_add_group != NULL)
		ui_ops->request_add_group();
}

static void blist_print_setting(const char *key,
		struct gaim_blist_node_setting *setting, FILE *file, int indent)
{
	char *key_val, *data_val = NULL;
	const char *type = NULL;
	int i;

	if (!key)
		return;

	switch(setting->type) {
		case GAIM_BLIST_NODE_SETTING_BOOL:
			type = "bool";
			data_val = g_strdup_printf("%d", setting->value.boolean);
			break;
		case GAIM_BLIST_NODE_SETTING_INT:
			type = "int";
			data_val = g_strdup_printf("%d", setting->value.integer);
			break;
		case GAIM_BLIST_NODE_SETTING_STRING:
			if (!setting->value.string)
				return;

			type = "string";
			data_val = g_markup_escape_text(setting->value.string, -1);
			break;
	}

	/* this can't happen */
	if (!type || !data_val)
		return;

	for (i=0; i<indent; i++) fprintf(file, "\t");

	key_val = g_markup_escape_text(key, -1);
	fprintf(file, "<setting name=\"%s\" type=\"%s\">%s</setting>\n", key_val, type,
			data_val);

	g_free(key_val);
	g_free(data_val);
}

static void blist_print_group_settings(gpointer key, gpointer data,
		gpointer user_data)
{
	blist_print_setting(key, data, user_data, 3);
}

static void blist_print_buddy_settings(gpointer key, gpointer data,
		gpointer user_data)
{
	blist_print_setting(key, data, user_data, 5);
}

static void blist_print_cnode_settings(gpointer key, gpointer data,
		gpointer user_data)
{
	blist_print_setting(key, data, user_data, 4);
}

static void blist_print_chat_components(gpointer key, gpointer data,
		gpointer user_data) {
	char *key_val;
	char *data_val;
	FILE *file = user_data;

	if (!key || !data)
		return;

	key_val = g_markup_escape_text(key, -1);
	data_val = g_markup_escape_text(data, -1);

	fprintf(file, "\t\t\t\t<component name=\"%s\">%s</component>\n", key_val,
			data_val);
	g_free(key_val);
	g_free(data_val);
}

static void print_buddy(FILE *file, GaimBuddy *buddy)
{
	char *bud_name = g_markup_escape_text(buddy->name, -1);
	char *bud_alias = NULL;
	char *acct_name = g_markup_escape_text(buddy->account->username, -1);
	if (buddy->alias)
		bud_alias= g_markup_escape_text(buddy->alias, -1);
	fprintf(file, "\t\t\t\t<buddy account=\"%s\" proto=\"%s\">\n", acct_name,
			gaim_account_get_protocol_id(buddy->account));

	fprintf(file, "\t\t\t\t\t<name>%s</name>\n", bud_name);
	if (bud_alias) {
		fprintf(file, "\t\t\t\t\t<alias>%s</alias>\n", bud_alias);
	}
	g_hash_table_foreach(buddy->node.settings, blist_print_buddy_settings, file);
	fprintf(file, "\t\t\t\t</buddy>\n");
	g_free(bud_name);
	g_free(bud_alias);
	g_free(acct_name);
}


/* check for flagging and account exclusion on buddy */
static gboolean blist_buddy_should_save(GaimAccount *exp_acct, GaimBuddy *buddy)
{
	if (! GAIM_BLIST_NODE_SHOULD_SAVE((GaimBlistNode *) buddy))
		return FALSE;

	if (exp_acct && buddy->account != exp_acct)
		return FALSE;

	return TRUE;
}


static void blist_write_buddy(FILE *file, GaimAccount *exp_acct, GaimBuddy *buddy)
{
	if (blist_buddy_should_save(exp_acct, buddy))
		print_buddy(file, buddy);
}


/* check for flagging and account exclusion on contact and all members */
static gboolean blist_contact_should_save(GaimAccount *exp_acct, GaimContact *contact)
{
	GaimBlistNode *bnode, *cnode = (GaimBlistNode *) contact;

	if (! GAIM_BLIST_NODE_SHOULD_SAVE(cnode))
		return FALSE;

	for (bnode = cnode->child; bnode; bnode = bnode->next) {
		if (! GAIM_BLIST_NODE_IS_BUDDY(bnode))
			continue;

		if (blist_buddy_should_save(exp_acct, (GaimBuddy *) bnode))
			return TRUE;
	}
	
	return FALSE;
}


static void blist_write_contact(FILE *file, GaimAccount *exp_acct, GaimContact *contact)
{
	GaimBlistNode *bnode, *cnode = (GaimBlistNode *) contact;

	if (! blist_contact_should_save(exp_acct, contact))
		return;

	fprintf(file, "\t\t\t<contact");
	if (contact->alias) {
		char *alias = g_markup_escape_text(contact->alias, -1);
		fprintf(file, " alias=\"%s\"", alias);
		g_free(alias);
	}
	fprintf(file, ">\n");

	for (bnode = cnode->child; bnode; bnode = bnode->next) {
		if (GAIM_BLIST_NODE_IS_BUDDY(bnode)) {
			blist_write_buddy(file, exp_acct, (GaimBuddy *) bnode);
		}
	}

	g_hash_table_foreach(cnode->settings, blist_print_cnode_settings, file);
	fprintf(file, "\t\t\t</contact>\n");
}


static void blist_write_chat(FILE *file, GaimAccount *exp_acct, GaimChat *chat)
{
	char *acct_name;

	if (! GAIM_BLIST_NODE_SHOULD_SAVE((GaimBlistNode *) chat))
		return;

	if (exp_acct && chat->account != exp_acct)
		return;
	
	acct_name = g_markup_escape_text(chat->account->username, -1);
	fprintf(file, "\t\t\t<chat proto=\"%s\" account=\"%s\">\n",
			gaim_account_get_protocol_id(chat->account), acct_name);
	g_free(acct_name);

	if (chat->alias) {
		char *chat_alias = g_markup_escape_text(chat->alias, -1);
		fprintf(file, "\t\t\t\t<alias>%s</alias>\n", chat_alias);
		g_free(chat_alias);
	}

	g_hash_table_foreach(chat->components, blist_print_chat_components, file);
	g_hash_table_foreach(chat->node.settings, blist_print_cnode_settings, file);

	fprintf(file, "\t\t\t</chat>\n");
}


static void blist_write_group(FILE *file, GaimAccount *exp_acct, GaimGroup *group)
{
	GaimBlistNode *cnode, *gnode = (GaimBlistNode *) group;
	char *group_name;

	if (! GAIM_BLIST_NODE_SHOULD_SAVE(gnode))
		return;

	if (exp_acct && ! gaim_group_on_account(group, exp_acct))
		return; 

	group_name = g_markup_escape_text(group->name, -1);
	fprintf(file, "\t\t<group name=\"%s\">\n", group_name);
	g_free(group_name);

	g_hash_table_foreach(group->node.settings,
			blist_print_group_settings, file);

	for (cnode = gnode->child; cnode; cnode = cnode->next) {
		if (GAIM_BLIST_NODE_IS_CONTACT(cnode)) {
			blist_write_contact(file, exp_acct, (GaimContact *) cnode);
		} else if (GAIM_BLIST_NODE_IS_CHAT(cnode)) {
			blist_write_chat(file, exp_acct, (GaimChat *) cnode);
		}
	}

	fprintf(file, "\t\t</group>\n");
}


static void blist_write_privacy_account(FILE *file, GaimAccount *exp_acct, GaimAccount *account)
{
	char *acct_name;
	GSList *buds;

	if(exp_acct && exp_acct != account)
		return;

	acct_name = g_markup_escape_text(account->username, -1);
	fprintf(file, "\t\t<account proto=\"%s\" name=\"%s\" mode=\"%d\">\n",
			gaim_account_get_protocol_id(account),
			acct_name, account->perm_deny);
	g_free(acct_name);

	for (buds = account->permit; buds; buds = buds->next) {
		char *bud_name = g_markup_escape_text(buds->data, -1);
		fprintf(file, "\t\t\t<permit>%s</permit>\n", bud_name);
		g_free(bud_name);
	}

	for (buds = account->deny; buds; buds = buds->next) {
		char *bud_name = g_markup_escape_text(buds->data, -1);
		fprintf(file, "\t\t\t<block>%s</block>\n", bud_name);
		g_free(bud_name);
	}

	fprintf(file, "\t\t</account>\n");
}


static void gaim_blist_write(FILE *file, GaimAccount *exp_acct)
{
	GList *accounts;
	GaimBlistNode *gnode;

	fprintf(file, "<?xml version='1.0' encoding='UTF-8' ?>\n");
	fprintf(file, "<gaim version=\"1\">\n");
	fprintf(file, "\t<blist>\n");

	for (gnode = gaimbuddylist->root; gnode; gnode = gnode->next) {
		if (GAIM_BLIST_NODE_IS_GROUP(gnode))
			blist_write_group(file, exp_acct, (GaimGroup *) gnode);
	}

	fprintf(file, "\t</blist>\n");
	fprintf(file, "\t<privacy>\n");

	for (accounts = gaim_accounts_get_all(); accounts; accounts = accounts->next) {
		blist_write_privacy_account(file, exp_acct, (GaimAccount *) accounts->data);
	}

	fprintf(file, "\t</privacy>\n");
	fprintf(file, "</gaim>\n");
}


void gaim_blist_sync()
{
	FILE *file;
	char *user_dir = gaim_user_dir();
	char *filename;
	char *filename_real;

	if (!user_dir)
		return;

	if (!blist_safe_to_write) {
		gaim_debug(GAIM_DEBUG_WARNING, "blist save",
				   "AHH!! Tried to write the blist before we read it!\n");
		return;
	}

	file = fopen(user_dir, "r");
	if (!file)
		mkdir(user_dir, S_IRUSR | S_IWUSR | S_IXUSR);
	else
		fclose(file);

	filename = g_build_filename(user_dir, "blist.xml.save", NULL);

	if ((file = fopen(filename, "w"))) {
		gaim_blist_write(file, NULL);
		fclose(file);
		chmod(filename, S_IRUSR | S_IWUSR);
	} else {
		gaim_debug(GAIM_DEBUG_ERROR, "blist save", "Unable to write %s\n",
				   filename);
		g_free(filename);
		return;
	}

	filename_real = g_build_filename(user_dir, "blist.xml", NULL);

	if (rename(filename, filename_real) < 0)
		gaim_debug(GAIM_DEBUG_ERROR, "blist save",
				   "Error renaming %s to %s\n", filename, filename_real);


	g_free(filename);
	g_free(filename_real);
}


static void gaim_blist_node_setting_free(struct gaim_blist_node_setting *setting)
{
	switch(setting->type) {
		case GAIM_BLIST_NODE_SETTING_BOOL:
		case GAIM_BLIST_NODE_SETTING_INT:
			break;
		case GAIM_BLIST_NODE_SETTING_STRING:
			g_free(setting->value.string);
			break;
	}
	g_free(setting);
}

static void gaim_blist_node_initialize_settings(GaimBlistNode *node)
{
	if (node->settings)
		return;

	node->settings = g_hash_table_new_full(g_str_hash, g_str_equal, g_free,
			(GDestroyNotify)gaim_blist_node_setting_free);
}

void gaim_blist_node_remove_setting(GaimBlistNode *node, const char *key)
{
	g_return_if_fail(node != NULL);
	g_return_if_fail(node->settings != NULL);
	g_return_if_fail(key != NULL);

	g_hash_table_remove(node->settings, key);

	schedule_blist_save();
}


void gaim_blist_node_set_bool(GaimBlistNode* node, const char *key, gboolean value)
{
	struct gaim_blist_node_setting *setting;

	g_return_if_fail(node != NULL);
	g_return_if_fail(node->settings != NULL);
	g_return_if_fail(key != NULL);

	setting = g_new0(struct gaim_blist_node_setting, 1);
	setting->type = GAIM_BLIST_NODE_SETTING_BOOL;
	setting->value.boolean = value;

	g_hash_table_replace(node->settings, g_strdup(key), setting);

	schedule_blist_save();
}

gboolean gaim_blist_node_get_bool(GaimBlistNode* node, const char *key)
{
	struct gaim_blist_node_setting *setting;

	g_return_val_if_fail(node != NULL, FALSE);
	g_return_val_if_fail(node->settings != NULL, FALSE);
	g_return_val_if_fail(key != NULL, FALSE);

	setting = g_hash_table_lookup(node->settings, key);

	if (!setting)
		return FALSE;

	g_return_val_if_fail(setting->type == GAIM_BLIST_NODE_SETTING_BOOL, FALSE);

	return setting->value.boolean;
}

void gaim_blist_node_set_int(GaimBlistNode* node, const char *key, int value)
{
	struct gaim_blist_node_setting *setting;

	g_return_if_fail(node != NULL);
	g_return_if_fail(node->settings != NULL);
	g_return_if_fail(key != NULL);

	setting = g_new0(struct gaim_blist_node_setting, 1);
	setting->type = GAIM_BLIST_NODE_SETTING_INT;
	setting->value.integer = value;

	g_hash_table_replace(node->settings, g_strdup(key), setting);

	schedule_blist_save();
}

int gaim_blist_node_get_int(GaimBlistNode* node, const char *key)
{
	struct gaim_blist_node_setting *setting;

	g_return_val_if_fail(node != NULL, 0);
	g_return_val_if_fail(node->settings != NULL, 0);
	g_return_val_if_fail(key != NULL, 0);

	setting = g_hash_table_lookup(node->settings, key);

	if (!setting)
		return 0;

	g_return_val_if_fail(setting->type == GAIM_BLIST_NODE_SETTING_INT, 0);

	return setting->value.integer;
}

void gaim_blist_node_set_string(GaimBlistNode* node, const char *key,
		const char *value)
{
	struct gaim_blist_node_setting *setting;

	g_return_if_fail(node != NULL);
	g_return_if_fail(node->settings != NULL);
	g_return_if_fail(key != NULL);

	setting = g_new0(struct gaim_blist_node_setting, 1);
	setting->type = GAIM_BLIST_NODE_SETTING_STRING;
	setting->value.string = g_strdup(value);

	g_hash_table_replace(node->settings, g_strdup(key), setting);

	schedule_blist_save();
}

const char *gaim_blist_node_get_string(GaimBlistNode* node, const char *key)
{
	struct gaim_blist_node_setting *setting;

	g_return_val_if_fail(node != NULL, NULL);
	g_return_val_if_fail(node->settings != NULL, NULL);
	g_return_val_if_fail(key != NULL, NULL);

	setting = g_hash_table_lookup(node->settings, key);

	if (!setting)
		return NULL;

	g_return_val_if_fail(setting->type == GAIM_BLIST_NODE_SETTING_STRING, NULL);

	return setting->value.string;
}

GList *gaim_blist_node_get_extended_menu(GaimBlistNode *n)
{
	GList *menu = NULL;

	g_return_val_if_fail(n, NULL);

	gaim_signal_emit(gaim_blist_get_handle(),
			"blist-node-extended-menu",
			n, &menu);
	return menu;
}


GaimBlistNodeAction *
gaim_blist_node_action_new(char *label,
			   void (*callback)(GaimBlistNode *, gpointer),
			   gpointer data)
{
	GaimBlistNodeAction *act = g_new0(GaimBlistNodeAction, 1);
	act->label = label;
	act->callback = callback;
	act->data = data;
	return act;
}


int gaim_blist_get_group_size(GaimGroup *group, gboolean offline)
{
	if (!group)
		return 0;

	return offline ? group->totalsize : group->currentsize;
}

int gaim_blist_get_group_online_count(GaimGroup *group)
{
	if (!group)
		return 0;

	return group->online;
}

void
gaim_blist_set_ui_ops(GaimBlistUiOps *ops)
{
	blist_ui_ops = ops;
}

GaimBlistUiOps *
gaim_blist_get_ui_ops(void)
{
	return blist_ui_ops;
}


void *
gaim_blist_get_handle(void)
{
	static int handle;

	return &handle;
}

void
gaim_blist_init(void)
{
	void *handle = gaim_blist_get_handle();

	gaim_signal_register(handle, "buddy-away",
						 gaim_marshal_VOID__POINTER, NULL, 1,
						 gaim_value_new(GAIM_TYPE_SUBTYPE,
										GAIM_SUBTYPE_BLIST_BUDDY));

	gaim_signal_register(handle, "buddy-back",
						 gaim_marshal_VOID__POINTER, NULL, 1,
						 gaim_value_new(GAIM_TYPE_SUBTYPE,
										GAIM_SUBTYPE_BLIST_BUDDY));

	gaim_signal_register(handle, "buddy-idle",
						 gaim_marshal_VOID__POINTER, NULL, 1,
						 gaim_value_new(GAIM_TYPE_SUBTYPE,
										GAIM_SUBTYPE_BLIST_BUDDY));
	gaim_signal_register(handle, "buddy-unidle",
						 gaim_marshal_VOID__POINTER, NULL, 1,
						 gaim_value_new(GAIM_TYPE_SUBTYPE,
										GAIM_SUBTYPE_BLIST_BUDDY));
	gaim_signal_register(handle, "buddy-idle-updated",
						 gaim_marshal_VOID__POINTER, NULL, 1,
						 gaim_value_new(GAIM_TYPE_SUBTYPE,
										GAIM_SUBTYPE_BLIST_BUDDY));

	gaim_signal_register(handle, "buddy-signed-on",
						 gaim_marshal_VOID__POINTER, NULL, 1,
						 gaim_value_new(GAIM_TYPE_SUBTYPE,
										GAIM_SUBTYPE_BLIST_BUDDY));

	gaim_signal_register(handle, "buddy-signed-off",
						 gaim_marshal_VOID__POINTER, NULL, 1,
						 gaim_value_new(GAIM_TYPE_SUBTYPE,
										GAIM_SUBTYPE_BLIST_BUDDY));

	gaim_signal_register(handle, "update-idle", gaim_marshal_VOID, NULL, 0);

	gaim_signal_register(handle, "blist-node-extended-menu",
			     gaim_marshal_VOID__POINTER_POINTER, NULL, 2,
			     gaim_value_new(GAIM_TYPE_SUBTYPE,
					    GAIM_SUBTYPE_BLIST_NODE),
			     gaim_value_new(GAIM_TYPE_BOXED, "GList **"));
}

void
gaim_blist_uninit(void)
{
	if (blist_save_timer != 0) {
		gaim_timeout_remove(blist_save_timer);
		blist_save_timer = 0;
		gaim_blist_sync();
	}

	gaim_signals_unregister_by_instance(gaim_blist_get_handle());
}
