/*
 * gaim - Jabber Protocol Plugin
 *
 * Copyright (C) 2003, Nathan Walp <faceprint@faceprint.com>
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
#include "debug.h"
#include "server.h"

#include "buddy.h"
#include "presence.h"
#include "roster.h"
#include "iq.h"

#include <string.h>


void jabber_roster_request(JabberStream *js)
{
	JabberIq *iq;

	iq = jabber_iq_new_query(js, JABBER_IQ_GET, "jabber:iq:roster");

	jabber_iq_send(iq);
}

static void remove_gaim_buddies(JabberStream *js, const char *jid)
{
	GSList *buddies, *l;

	buddies = gaim_find_buddies(js->gc->account, jid);

	for(l = buddies; l; l = l->next)
		gaim_blist_remove_buddy(l->data);

	g_slist_free(buddies);
}

static void add_gaim_buddies_in_groups(JabberStream *js, const char *jid,
		const char *alias, GSList *groups)
{
	GSList *buddies, *g2, *l;
	int present =0, idle=0, state=0;

	buddies = gaim_find_buddies(js->gc->account, jid);

	g2 = groups;

	if(!groups) {
		if(!buddies)
			g2 = g_slist_append(g2, g_strdup(_("Buddies")));
		else
			return;
	}

	if(buddies) {
		present = ((GaimBuddy*)buddies->data)->present;
		idle = ((GaimBuddy*)buddies->data)->idle;
		state = ((GaimBuddy*)buddies->data)->uc;
	}

	while(buddies) {
		GaimBuddy *b = buddies->data;
		GaimGroup *g = gaim_find_buddys_group(b);

		buddies = g_slist_remove(buddies, b);

		if((l = g_slist_find_custom(g2, g->name, (GCompareFunc)strcmp))) {
			const char *servernick;

			if((servernick = gaim_blist_node_get_string((GaimBlistNode*)b, "servernick")))
				serv_got_alias(js->gc, jid, servernick);

			if(alias && (!b->alias || strcmp(b->alias, alias)))
				gaim_blist_alias_buddy(b, alias);
			g_free(l->data);
			g2 = g_slist_delete_link(g2, l);
		} else {
			gaim_blist_remove_buddy(b);
		}
	}

	while(g2) {
		GaimBuddy *b = gaim_buddy_new(js->gc->account, jid, alias);
		GaimGroup *g = gaim_find_group(g2->data);

		if(!g) {
			g = gaim_group_new(g2->data);
			gaim_blist_add_group(g, NULL);
		}

		b->present = present;
		b->idle = idle;
		b->uc = state;

		gaim_blist_add_buddy(b, NULL, g, NULL);
		gaim_blist_alias_buddy(b, alias);
		g_free(g2->data);
		g2 = g_slist_delete_link(g2, g2);
	}

	g_slist_free(buddies);
}

void jabber_roster_parse(JabberStream *js, xmlnode *packet)
{
	xmlnode *query, *item, *group;
	const char *from = xmlnode_get_attrib(packet, "from");

	if(from) {
		char *from_norm;
		gboolean invalid;

		from_norm = g_strdup(jabber_normalize(js->gc->account, from));

		if(!from_norm)
			return;

		invalid = g_utf8_collate(from_norm,
				jabber_normalize(js->gc->account,
					gaim_account_get_username(js->gc->account)));

		g_free(from_norm);

		if(invalid)
			return;
	}

	query = xmlnode_get_child(packet, "query");
	if(!query)
		return;

	js->roster_parsed = TRUE;

	for(item = xmlnode_get_child(query, "item"); item; item = xmlnode_get_next_twin(item))
	{
		const char *jid, *name, *subscription, *ask;
		JabberBuddy *jb;

		subscription = xmlnode_get_attrib(item, "subscription");
		jid = xmlnode_get_attrib(item, "jid");
		name = xmlnode_get_attrib(item, "name");
		ask = xmlnode_get_attrib(item, "ask");

		jb = jabber_buddy_find(js, jid, TRUE);

		if(!subscription)
			jb->subscription = JABBER_SUB_NONE;
		else if(!strcmp(subscription, "to"))
			jb->subscription = JABBER_SUB_TO;
		else if(!strcmp(subscription, "from"))
			jb->subscription = JABBER_SUB_FROM;
		else if(!strcmp(subscription, "both"))
			jb->subscription = JABBER_SUB_BOTH;
		else
			jb->subscription = JABBER_SUB_NONE;

		if(ask && !strcmp(ask, "subscribe"))
			jb->subscription |= JABBER_SUB_PENDING;
		else
			jb->subscription &= ~JABBER_SUB_PENDING;

		if(jb->subscription == JABBER_SUB_NONE) {
			remove_gaim_buddies(js, jid);
		} else {
			GSList *groups = NULL;

			for(group = xmlnode_get_child(item, "group"); group; group = xmlnode_get_next_twin(group)) {
				char *group_name;

				if(!(group_name = xmlnode_get_data(group)))
					group_name = g_strdup("");
				groups = g_slist_append(groups, group_name);
			}
			add_gaim_buddies_in_groups(js, jid, name, groups);

			if(jb->subscription == JABBER_SUB_BOTH) {
				char *my_bare_jid = g_strdup_printf("%s@%s", js->user->node, js->user->domain);
				if(!strcmp(jid, my_bare_jid)) {
					jabber_presence_fake_to_self(js, js->gc->away_state, js->gc->away);
				}
				g_free(my_bare_jid);
			}
		}
	}

	gaim_blist_save();
}

static void jabber_roster_update(JabberStream *js, const char *name,
		GSList *grps)
{
	GaimBuddy *b;
	GaimGroup *g;
	GSList *groups = NULL, *l;
	JabberIq *iq;
	xmlnode *query, *item, *group;

	if(grps) {
		groups = grps;
	} else {
		GSList *buddies = gaim_find_buddies(js->gc->account, name);
		if(!buddies)
			return;
		while(buddies) {
			b = buddies->data;
			g = gaim_find_buddys_group(b);
			groups = g_slist_append(groups, g->name);
			buddies = g_slist_remove(buddies, b);
		}
	}

	if(!(b = gaim_find_buddy(js->gc->account, name)))
		return;

	iq = jabber_iq_new_query(js, JABBER_IQ_SET, "jabber:iq:roster");

	query = xmlnode_get_child(iq->node, "query");
	item = xmlnode_new_child(query, "item");

	xmlnode_set_attrib(item, "jid", name);

	if(b->alias)
		xmlnode_set_attrib(item, "name", b->alias);

	for(l = groups; l; l = l->next) {
		group = xmlnode_new_child(item, "group");
		xmlnode_insert_data(group, l->data, -1);
	}

	if(!grps)
		g_slist_free(groups);

	jabber_iq_send(iq);
}

void jabber_roster_add_buddy(GaimConnection *gc, const char *name,
		GaimGroup *grp)
{
	JabberStream *js = gc->proto_data;
	char *who;
	GSList *groups = NULL;
	JabberBuddy *jb;
	JabberBuddyResource *jbr;

	if(!js->roster_parsed)
		return;

	if(!(who = jabber_get_bare_jid(name)))
		return;

	jb = jabber_buddy_find(js, name, FALSE);

	if(!jb || !(jb->subscription & JABBER_SUB_TO)) {
		groups = g_slist_append(groups, grp->name);
	}

	jabber_roster_update(js, who, groups);

	if(!jb || !(jb->subscription & JABBER_SUB_TO))
		jabber_presence_subscription_set(js, who, "subscribe");
	else if((jbr =jabber_buddy_find_resource(jb, NULL)))
		serv_got_update(gc, who, 1, 0, 0, 0, jbr->state);

	g_free(who);
}

void jabber_roster_alias_change(GaimConnection *gc, const char *name, const char *alias)
{
	GaimBuddy *b = gaim_find_buddy(gc->account, name);
	char *a;

	a = g_strdup(alias);
	gaim_blist_alias_buddy(b, a);
	g_free(a);

	jabber_roster_update(gc->proto_data, name, NULL);
}

void jabber_roster_group_change(GaimConnection *gc, const char *name,
		const char *old_group, const char *new_group)
{
	GSList *buddies, *groups = NULL;
	GaimBuddy *b;
	GaimGroup *g;

	if(!old_group || !new_group || !strcmp(old_group, new_group))
		return;

	buddies = gaim_find_buddies(gc->account, name);
	while(buddies) {
		b = buddies->data;
		g = gaim_find_buddys_group(b);
		if(!strcmp(g->name, old_group))
			groups = g_slist_append(groups, (char*)new_group); /* ick */
		else
			groups = g_slist_append(groups, g->name);
		buddies = g_slist_remove(buddies, b);
	}
	jabber_roster_update(gc->proto_data, name, groups);
	g_slist_free(groups);
}

void jabber_roster_group_rename(GaimConnection *gc, const char *old_group,
		const char *new_group, GList *members)
{
	GList *l;
	if(old_group && new_group && strcmp(old_group, new_group)) {
		for(l = members; l; l = l->next) {
			jabber_roster_group_change(gc, l->data, old_group, new_group);
		}
	}
}

void jabber_roster_remove_buddy(GaimConnection *gc, const char *name, const char *group) {
	GSList *buddies = gaim_find_buddies(gc->account, name);
	GSList *groups = NULL;
	GaimGroup *g = gaim_find_group(group);
	GaimBuddy *b = gaim_find_buddy_in_group(gc->account, name, g);

	buddies = g_slist_remove(buddies, b);
	if(g_slist_length(buddies)) {
		while(buddies) {
			b = buddies->data;
			g = gaim_find_buddys_group(b);
			groups = g_slist_append(groups, g->name);
			buddies = g_slist_remove(buddies, b);
		}
		jabber_roster_update(gc->proto_data, name, groups);
	} else {
		JabberIq *iq = jabber_iq_new_query(gc->proto_data, JABBER_IQ_SET,
				"jabber:iq:roster");
		xmlnode *query = xmlnode_get_child(iq->node, "query");
		xmlnode *item = xmlnode_new_child(query, "item");

		xmlnode_set_attrib(item, "jid", name);
		xmlnode_set_attrib(item, "subscription", "remove");

		jabber_iq_send(iq);
	}

	if(buddies)
		g_slist_free(buddies);
	if(groups)
		g_slist_free(groups);
}
