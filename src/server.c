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
#include "conversation.h"
#include "debug.h"
#include "log.h"
#include "notify.h"
#include "prefs.h"
#include "prpl.h"
#include "request.h"
#include "signals.h"
#include "server.h"
#include "sound.h"
#include "util.h"

/* XXX UI Stuff */
#include "away.h"
#include "gtkdialogs.h"
#include "gaim.h"
#include "gtkimhtml.h"
#include "gtkutils.h"

#define SECS_BEFORE_RESENDING_AUTORESPONSE 600
#define SEX_BEFORE_RESENDING_AUTORESPONSE "Only after you're married"

static void add_idle_buddy(GaimBuddy *buddy);
static void remove_idle_buddy(GaimBuddy *buddy);

void serv_login(GaimAccount *account)
{
	GaimPlugin *p = gaim_find_prpl(gaim_account_get_protocol_id(account));
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (account->gc == NULL || p == NULL)
		return;

	prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(p);

	if (prpl_info->login) {
		if (gaim_account_get_password(account) == NULL &&
			!(prpl_info->options & OPT_PROTO_NO_PASSWORD) &&
			!(prpl_info->options & OPT_PROTO_PASSWORD_OPTIONAL)) {
			gaim_notify_error(NULL, NULL,
							  _("Please enter your password"), NULL);
			return;
		}

		gaim_debug_info("server", PACKAGE " " VERSION
						" logging in %s using %s\n",
						gaim_account_get_username(account),
						gaim_account_get_protocol_name(account));

		gaim_signal_emit(gaim_accounts_get_handle(),
						 "account-connecting", account);
		prpl_info->login(account);
	}
}

static gboolean send_keepalive(gpointer d)
{
	GaimConnection *gc = d;
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (gc != NULL && gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	if (prpl_info && prpl_info->keepalive)
		prpl_info->keepalive(gc);

	return TRUE;
}

static void update_keepalive(GaimConnection *gc, gboolean on)
{
	if (on && !gc->keep_alive) {
		gaim_debug(GAIM_DEBUG_INFO, "server", "allowing NOP\n");
		gc->keep_alive = gaim_timeout_add(60000, send_keepalive, gc);
	} else if (!on && gc->keep_alive > 0) {
		gaim_debug(GAIM_DEBUG_INFO, "server", "removing NOP\n");
		gaim_timeout_remove(gc->keep_alive);
		gc->keep_alive = 0;
	}
}

void serv_close(GaimConnection *gc)
{
	GaimPlugin *prpl;
	GaimPluginProtocolInfo *prpl_info = NULL;

	while (gc->buddy_chats) {
		GaimConversation *b = gc->buddy_chats->data;

		gc->buddy_chats = g_slist_remove(gc->buddy_chats, b);
		gaim_conv_chat_left(GAIM_CONV_CHAT(b));
	}

	if (gc->idle_timer > 0)
		gaim_timeout_remove(gc->idle_timer);
	gc->idle_timer = 0;

	update_keepalive(gc, FALSE);

	if (gc->prpl != NULL) {
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

		if (prpl_info->close)
			(prpl_info->close)(gc);
	}

	prpl = gc->prpl;
}

void serv_touch_idle(GaimConnection *gc)
{
	/* Are we idle?  If so, not anymore */
	if (gc->is_idle > 0) {
		gc->is_idle = 0;
		serv_set_idle(gc, 0);
	}
	time(&gc->last_sent_time);
	if (gc->is_auto_away)
		check_idle(gc);
}

void serv_finish_login(GaimConnection *gc)
{
	GaimPluginProtocolInfo *prpl_info = NULL;
	GaimAccount *account;

	if (gc != NULL && gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	account = gaim_connection_get_account(gc);

	if (gaim_account_get_user_info(account) != NULL)
		serv_set_info(gc, gaim_account_get_user_info(account));

	if (gc->idle_timer > 0)
		gaim_timeout_remove(gc->idle_timer);

	gc->idle_timer = gaim_timeout_add(20000, check_idle, gc);
	serv_touch_idle(gc);

	update_keepalive(gc, TRUE);
}

/* This should return the elapsed time in seconds in which Gaim will not send
 * typing notifications.
 * if it returns zero, it will not send any more typing notifications
 * typing is a flag - TRUE for typing, FALSE for stopped typing */
int serv_send_typing(GaimConnection *g, const char *name, int typing) {
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (g != NULL && g->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(g->prpl);

	if (g && prpl_info && prpl_info->send_typing)
		return prpl_info->send_typing(g, name, typing);

	return 0;
}

GSList *last_auto_responses = NULL;
struct last_auto_response {
	GaimConnection *gc;
	char name[80];
	time_t sent;
};

gboolean expire_last_auto_responses(gpointer data)
{
	GSList *tmp, *cur;
	struct last_auto_response *lar;

	tmp = last_auto_responses;

	while (tmp) {
		cur = tmp;
		tmp = tmp->next;
		lar = (struct last_auto_response *)cur->data;

		if ((time(NULL) - lar->sent) > SECS_BEFORE_RESENDING_AUTORESPONSE) {
			last_auto_responses = g_slist_remove(last_auto_responses, lar);
			g_free(lar);
		}
	}

	return FALSE; /* do not run again */
}

struct last_auto_response *get_last_auto_response(GaimConnection *gc, const char *name)
{
	GSList *tmp;
	struct last_auto_response *lar;

	/* because we're modifying or creating a lar, schedule the
	 * function to expire them as the pref dictates */
	gaim_timeout_add((SECS_BEFORE_RESENDING_AUTORESPONSE + 1) * 1000, expire_last_auto_responses, NULL);

	tmp = last_auto_responses;

	while (tmp) {
		lar = (struct last_auto_response *)tmp->data;

		if (gc == lar->gc && !strncmp(name, lar->name, sizeof(lar->name)))
			return lar;

		tmp = tmp->next;
	}

	lar = (struct last_auto_response *)g_new0(struct last_auto_response, 1);
	g_snprintf(lar->name, sizeof(lar->name), "%s", name);
	lar->gc = gc;
	lar->sent = 0;
	last_auto_responses = g_slist_append(last_auto_responses, lar);

	return lar;
}

void flush_last_auto_responses(GaimConnection *gc)
{
	GSList *tmp, *cur;
	struct last_auto_response *lar;

	tmp = last_auto_responses;

	while (tmp) {
		cur = tmp;
		tmp = tmp->next;
		lar = (struct last_auto_response *)cur->data;

		if (lar->gc == gc) {
			last_auto_responses = g_slist_remove(last_auto_responses, lar);
			g_free(lar);
		}
	}
}

int serv_send_im(GaimConnection *gc, const char *name, const char *message,
				 GaimConvImFlags imflags)
{
	GaimConversation *c;
	int val = -EINVAL;
	GaimPluginProtocolInfo *prpl_info = NULL;
	const gchar *auto_reply_pref;

	if (gc != NULL && gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	c = gaim_find_conversation_with_account(name, gc->account);

	if (prpl_info && prpl_info->send_im)
		val = prpl_info->send_im(gc, name, message, imflags);

	if (!(imflags & GAIM_CONV_IM_AUTO_RESP))
		serv_touch_idle(gc);

	/*
	 * XXX - If "only auto-reply when away & idle" is set, then shouldn't
	 * this only reset lar->sent if we're away AND idle?
	 */
	auto_reply_pref = gaim_prefs_get_string("/core/away/auto_reply");
	if (gc->away &&	(gc->flags & GAIM_CONNECTION_AUTO_RESP) &&
		strcmp(auto_reply_pref, "never")) {

		struct last_auto_response *lar;
		lar = get_last_auto_response(gc, name);
		lar->sent = time(NULL);
	}

	if (c && gaim_conv_im_get_type_again_timeout(GAIM_CONV_IM(c)))
		gaim_conv_im_stop_type_again_timeout(GAIM_CONV_IM(c));

	return val;
}

void serv_get_info(GaimConnection *gc, const char *name)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (gc != NULL && gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	if (gc && prpl_info && prpl_info->get_info)
		prpl_info->get_info(gc, name);
}

void serv_set_away(GaimConnection *gc, const char *state, const char *message)
{
	GaimPluginProtocolInfo *prpl_info = NULL;
	GaimAccount *account;

	if (gc->away_state == NULL && state == NULL &&
		gc->away == NULL && message == NULL) {

		return;
	}

	if ((gc->away_state != NULL && state != NULL &&
		 !strcmp(gc->away_state, state) &&
		 !strcmp(gc->away_state, GAIM_AWAY_CUSTOM)) &&
		(gc->away != NULL && message != NULL && !strcmp(gc->away, message))) {

		return;
	}

	if (gc != NULL && gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	account = gaim_connection_get_account(gc);

	if (prpl_info && prpl_info->set_away) {
		if (gc->away_state) {
			g_free(gc->away_state);
			gc->away_state = NULL;
		}

		prpl_info->set_away(gc, state, message);

		if (gc->away && state) {
			gc->away_state = g_strdup(state);
		}

		gaim_signal_emit(gaim_accounts_get_handle(), "account-away",
						 account, state, message);
	}

	/* LOG system_log(log_away, gc, NULL, OPT_LOG_BUDDY_AWAY | OPT_LOG_MY_SIGNON); */
	/* New away message... Clear out the record of sent autoresponses */
	flush_last_auto_responses(gc);
}

void serv_set_away_all(const char *message)
{
	GList *c;
	GaimConnection *g;

	for (c = gaim_connections_get_all(); c != NULL; c = c->next) {
		g = (GaimConnection *)c->data;

		serv_set_away(g, GAIM_AWAY_CUSTOM, message);
	}
}

void serv_set_info(GaimConnection *gc, const char *info)
{
	GaimPluginProtocolInfo *prpl_info = NULL;
	GaimAccount *account;

	if (gc != NULL && gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	if (prpl_info && g_list_find(gaim_connections_get_all(), gc) &&
		prpl_info->set_info) {

		account = gaim_connection_get_account(gc);

		if (gaim_signal_emit_return_1(gaim_accounts_get_handle(),
									  "account-setting-info", account, info))
			return;

		prpl_info->set_info(gc, info);

		gaim_signal_emit(gaim_accounts_get_handle(),
						 "account-set-info", account, info);
	}
}

void serv_change_passwd(GaimConnection *gc, const char *orig, const char *new)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (gc != NULL && gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	if (prpl_info && g_list_find(gaim_connections_get_all(), gc) && prpl_info->change_passwd)
		prpl_info->change_passwd(gc, orig, new);
}

void serv_add_buddy(GaimConnection *gc, GaimBuddy *buddy)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (gc != NULL && gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	if (prpl_info && g_list_find(gaim_connections_get_all(), gc) && prpl_info->add_buddy)
		prpl_info->add_buddy(gc, buddy, gaim_find_buddys_group(buddy));
}

void serv_add_buddies(GaimConnection *gc, GList *buddies)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (gc != NULL && gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	if (prpl_info && g_list_find(gaim_connections_get_all(), gc)) {
		GList *cur, *groups = NULL;

		/* Make a list of what the groups each buddy is in */
		for (cur = buddies; cur != NULL; cur = cur->next) {
			GaimBlistNode *node = cur->data;
			groups = g_list_append(groups, node->parent->parent);
		}

		if (prpl_info->add_buddies)
			prpl_info->add_buddies(gc, buddies, groups);
		else if (prpl_info->add_buddy) {
			GList *curb = buddies;
			GList *curg = groups;
			while ((curb != NULL) && (curg != NULL)) {
				prpl_info->add_buddy(gc, curb->data, curg->data);
				curb = curb->next;
				curg = curg->next;
			}
		}

		g_list_free(groups);
	}
}


void serv_remove_buddy(GaimConnection *gc, GaimBuddy *buddy, GaimGroup *group)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (buddy->idle > 0)
		remove_idle_buddy(buddy);

	if (gc != NULL && gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	if (prpl_info && g_list_find(gaim_connections_get_all(), gc) && prpl_info->remove_buddy)
		prpl_info->remove_buddy(gc, buddy, group);
}

void serv_remove_buddies(GaimConnection *gc, GList *buddies, GList *groups)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (!g_list_find(gaim_connections_get_all(), gc))
		return;

	if (gc != NULL && gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	if (prpl_info && g_list_find(gaim_connections_get_all(), gc)) {
		if (prpl_info->remove_buddies) {
			GList *curb;
			for (curb = buddies; curb != NULL; curb = curb->next) {
				GaimBuddy *buddy = curb->data;
				if (buddy->idle > 0)
					remove_idle_buddy(buddy);
			}
			prpl_info->remove_buddies(gc, buddies, groups);
		} else {
			GList *curb = buddies;
			GList *curg = groups;
			while ((curb != NULL) && (curg != NULL)) {
				serv_remove_buddy(gc, curb->data, curg->data);
				curb = curb->next;
				curg = curg->next;
			}
		}
	}
}

void serv_remove_group(GaimConnection *gc, GaimGroup *group)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (gc != NULL && gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	if (prpl_info && g_list_find(gaim_connections_get_all(), gc) &&
		prpl_info->remove_group)
	{
		prpl_info->remove_group(gc, group);
	}
}

/*
 * Set buddy's alias on server roster/list
 */
void serv_alias_buddy(GaimBuddy *b)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (b != NULL && b->account->gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(b->account->gc->prpl);

	if (b && prpl_info && prpl_info->alias_buddy) {
		prpl_info->alias_buddy(b->account->gc, b->name, b->alias);
	}
}

void
serv_got_alias(GaimConnection *gc, const char *who, const char *alias)
{
	GaimAccount *account = gaim_connection_get_account(gc);
	GSList *buds, *buddies = gaim_find_buddies(account, who);
	GaimBuddy *b;
	GaimConversation *conv;

	for (buds = buddies; buds; buds = buds->next)
	{
		b = buds->data;
		gaim_blist_server_alias_buddy(b, alias);

		conv = gaim_find_conversation_with_account(b->name, account);

		if (conv != NULL && b->server_alias != NULL &&
			strcmp(b->server_alias, alias))
		{
			char *tmp = g_strdup_printf(_("%s is now known as %s.\n"),
										who, alias);

			gaim_conversation_write(conv, NULL, tmp, GAIM_MESSAGE_SYSTEM,
									time(NULL));

			g_free(tmp);
		}
	}
	g_slist_free(buddies);
}

/*
 * Move a buddy from one group to another on server.
 *
 * Note: For now we'll not deal with changing gc's at the same time, but
 * it should be possible.  Probably needs to be done, someday.  Although,
 * the UI for that would be difficult, because groups are Gaim-wide.
 */
void serv_move_buddy(GaimBuddy *b, GaimGroup *og, GaimGroup *ng)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (b->account->gc != NULL && b->account->gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(b->account->gc->prpl);

	if (b && b->account->gc && og && ng) {
		if (prpl_info && prpl_info->group_buddy) {
			prpl_info->group_buddy(b->account->gc, b->name, og->name, ng->name);
		}
	}
}

/*
 * Rename a group on server roster/list.
 */
void serv_rename_group(GaimConnection *gc, const char *old_name,
					   GaimGroup *group, GList *moved_buddies)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (gc != NULL && gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	if (prpl_info && old_name && group && strcmp(old_name, group->name)) {
		if (prpl_info->rename_group) {
			/* prpl's might need to check if the group already
			 * exists or not, and handle that differently */
			prpl_info->rename_group(gc, old_name, group, moved_buddies);
		} else {
			GList *cur, *groups = NULL;

			/* Make a list of what the groups each buddy is in */
			for (cur = moved_buddies; cur != NULL; cur = cur->next) {
				GaimBlistNode *node = cur->data;
				groups = g_list_append(groups, node->parent->parent);
			}

			serv_remove_buddies(gc, moved_buddies, groups);
			g_list_free(groups);
			serv_add_buddies(gc, moved_buddies);
		}
	}
}

void serv_add_permit(GaimConnection *g, const char *name)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (g != NULL && g->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(g->prpl);

	if (prpl_info && g_list_find(gaim_connections_get_all(), g) && prpl_info->add_permit)
		prpl_info->add_permit(g, name);
}

void serv_add_deny(GaimConnection *g, const char *name)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (g != NULL && g->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(g->prpl);

	if (prpl_info && g_list_find(gaim_connections_get_all(), g) && prpl_info->add_deny)
		prpl_info->add_deny(g, name);
}

void serv_rem_permit(GaimConnection *g, const char *name)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (g != NULL && g->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(g->prpl);

	if (prpl_info && g_list_find(gaim_connections_get_all(), g) && prpl_info->rem_permit)
		prpl_info->rem_permit(g, name);
}

void serv_rem_deny(GaimConnection *g, const char *name)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (g != NULL && g->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(g->prpl);

	if (prpl_info && g_list_find(gaim_connections_get_all(), g) && prpl_info->rem_deny)
		prpl_info->rem_deny(g, name);
}

void serv_set_permit_deny(GaimConnection *g)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (g != NULL && g->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(g->prpl);

	/*
	 * this is called when either you import a buddy list, and make lots
	 * of changes that way, or when the user toggles the permit/deny mode
	 * in the prefs. In either case you should probably be resetting and
	 * resending the permit/deny info when you get this.
	 */
	if (prpl_info && g_list_find(gaim_connections_get_all(), g) && prpl_info->set_permit_deny)
		prpl_info->set_permit_deny(g);
}


void serv_set_idle(GaimConnection *g, int time)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (g != NULL && g->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(g->prpl);

	if (prpl_info && g_list_find(gaim_connections_get_all(), g) && prpl_info->set_idle)
		prpl_info->set_idle(g, time);
}

void serv_warn(GaimConnection *g, const char *name, gboolean anonymous)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (g != NULL && g->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(g->prpl);

	if (prpl_info && g_list_find(gaim_connections_get_all(), g) && prpl_info->warn)
		prpl_info->warn(g, name, anonymous);
}

void serv_join_chat(GaimConnection *g, GHashTable *data)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (g != NULL && g->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(g->prpl);

	if (prpl_info && g_list_find(gaim_connections_get_all(), g) && prpl_info->join_chat)
		prpl_info->join_chat(g, data);
}


void serv_reject_chat(GaimConnection *g, GHashTable *data)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (g != NULL && g->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(g->prpl);

	if (prpl_info && g_list_find(gaim_connections_get_all(), g) && prpl_info->reject_chat)
		prpl_info->reject_chat(g, data);
}

void serv_chat_invite(GaimConnection *g, int id, const char *message, const char *name)
{
	GaimPluginProtocolInfo *prpl_info = NULL;
	GaimConversation *conv;
	char *buffy = message && *message ? g_strdup(message) : NULL;

	conv = gaim_find_chat(g, id);

	if (conv == NULL)
		return;

	if (g != NULL && g->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(g->prpl);

	gaim_signal_emit(gaim_conversations_get_handle(), "chat-inviting-user",
					 conv, name, &buffy);

	if (prpl_info && g_list_find(gaim_connections_get_all(), g) && prpl_info->chat_invite)
		prpl_info->chat_invite(g, id, buffy, name);

	gaim_signal_emit(gaim_conversations_get_handle(), "chat-invited-user",
					 conv, name, buffy);

	if (buffy)
		g_free(buffy);
}

/* Ya know, nothing uses this except gaim_conversation_destroy(),
 * I think I'll just merge it into that later...
 * Then again, something might want to use this, from outside prpl-land
 * to leave a chat without destroying the conversation.
 */

void serv_chat_leave(GaimConnection *g, int id)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (!g_list_find(gaim_connections_get_all(), g))
		return;

	if (g->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(g->prpl);

	if (prpl_info && prpl_info->chat_leave)
		prpl_info->chat_leave(g, id);
}

void serv_chat_whisper(GaimConnection *g, int id, const char *who, const char *message)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (g != NULL && g->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(g->prpl);

	if (prpl_info && prpl_info->chat_whisper)
		prpl_info->chat_whisper(g, id, who, message);
}

int serv_chat_send(GaimConnection *g, int id, const char *message)
{
	int val = -EINVAL;
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (g->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(g->prpl);

	if (prpl_info && prpl_info->chat_send)
		val = prpl_info->chat_send(g, id, message);

	serv_touch_idle(g);

	return val;
}

void serv_set_buddyicon(GaimConnection *gc, const char *filename)
{
	GaimPluginProtocolInfo *prpl_info = NULL;

	if (gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	if (prpl_info && prpl_info->set_buddy_icon)
		prpl_info->set_buddy_icon(gc, filename);

}

int find_queue_row_by_name(char *name)
{
	gchar *temp;
	gint i = 0;
	gboolean valid;
	GtkTreeIter iter;

	valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(awayqueuestore), &iter);
	while(valid) {
		gtk_tree_model_get(GTK_TREE_MODEL(awayqueuestore), &iter, 0, &temp, -1);
		if(!strcmp(name, temp))
			return i;
		g_free(temp);

		i++;
		valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(awayqueuestore), &iter);
	}

	return -1;
}

int find_queue_total_by_name(char *name)
{
	GSList *templist;
	int i = 0;

	templist = message_queue;

	while (templist) {
		struct queued_message *qm = (struct queued_message *)templist->data;
		if ((qm->flags & GAIM_MESSAGE_RECV) && !strcmp(name, qm->name))
			i++;

		templist = templist->next;
	}

	return i;
}

/*
 * woo. i'm actually going to comment this function. isn't that fun. make
 * sure to follow along, kids
 */
void serv_got_im(GaimConnection *gc, const char *who, const char *msg,
				 GaimConvImFlags imflags, time_t mtime)
{
	GaimConversation *cnv;
	GaimMessageFlags msgflags;
	char *message, *name;
	char *angel, *buffy;
	int plugin_return;

	/*
	 * We should update the conversation window buttons and menu,
	 * if it exists.
	 */
	cnv = gaim_find_conversation_with_account(who, gc->account);

	/*
	 * Plugin stuff. we pass a char ** but we don't want to pass what's
	 * been given us by the prpls. So we create temp holders and pass
	 * those instead. It's basically just to avoid segfaults.
	 */
	buffy = g_malloc(MAX(strlen(msg) + 1, BUF_LONG));
	strcpy(buffy, msg);
	angel = g_strdup(who);

	plugin_return = GPOINTER_TO_INT(
		gaim_signal_emit_return_1(gaim_conversations_get_handle(),
								  "receiving-im-msg", gc->account,
								  &angel, &buffy, &imflags));

	if (!buffy || !angel || plugin_return) {
		if (buffy)
			g_free(buffy);
		if (angel)
			g_free(angel);
		return;
	}

	name = angel;
	message = buffy;

	gaim_signal_emit(gaim_conversations_get_handle(), "received-im-msg", gc->account,
					 name, message, imflags);

	/* Make sure URLs are clickable */
	buffy = gaim_markup_linkify(message);
	g_free(message);
	message = buffy;

	/*
	 * Um. When we call gaim_conversation_write with the message we received,
	 * it's nice to pass whether or not it was an auto-response. So if it
	 * was an auto-response, we set the appropriate flag. This is just so
	 * prpls don't have to know about GAIM_MESSAGE_* (though some do anyway).
	 */
	msgflags = GAIM_MESSAGE_RECV;
	if (imflags & GAIM_CONV_IM_AUTO_RESP)
		msgflags |= GAIM_MESSAGE_AUTO_RESP;

	/*
	 * Alright. Two cases for how to handle this. Either we're away or
	 * we're not. If we're not, then it's easy. If we are, then there
	 * are three or four different ways of handling it and different
	 * things we have to do for each.
	 */
	if (gc->away) {
		time_t t = time(NULL);
		char *tmpmsg;
		GaimBuddy *b = gaim_find_buddy(gc->account, name);
		const char *alias = b ? gaim_buddy_get_alias(b) : name;
		int row;
		struct last_auto_response *lar;
		const gchar *auto_reply_pref;

		/*
		 * Either we're going to queue it or not. Because of the way
		 * awayness currently works, this is fucked up. It's possible
		 * for an account to be away without the imaway dialog being
		 * shown. In fact, it's possible for *all* the accounts to be
		 * away without the imaway dialog being shown. So in order for
		 * this to be queued properly, we have to make sure that the
		 * imaway dialog actually exists, first.
		 */
		if (!cnv && awayqueue &&
			gaim_prefs_get_bool("/gaim/gtk/away/queue_messages")) {
			/*
			 * Alright, so we're going to queue it. Neat, eh? :)
			 * So first we create something to store the message, and add
			 * it to our queue. Then we update the away dialog to indicate
			 * that we've queued something.
			 */
			struct queued_message *qm;
			GtkTreeIter iter;
			gchar path[10];

			qm = g_new0(struct queued_message, 1);
			g_snprintf(qm->name, sizeof(qm->name), "%s", name);
			if(strcmp(alias, name) != 0)
			    g_snprintf(qm->alias, sizeof(qm->alias), "(%s)", alias);
			qm->message = g_strdup(message);
			qm->account = gc->account;
			qm->tm = mtime;
			qm->flags = msgflags;
			message_queue = g_slist_append(message_queue, qm);

			row = find_queue_row_by_name(qm->name);
			if (row >= 0) {
				char number[32];
				int qtotal;

				qtotal = find_queue_total_by_name(qm->name);
				g_snprintf(number, 32, ngettext("(%d message)",
						   "(%d messages)", qtotal), qtotal);
				g_snprintf(path, 10, "%d", row);
				gtk_tree_model_get_iter_from_string(
								GTK_TREE_MODEL(awayqueuestore), &iter, path);
				gtk_list_store_set(awayqueuestore, &iter,
								2, number, -1);
			} else {
				gtk_tree_model_get_iter_first(GTK_TREE_MODEL(awayqueuestore),
								&iter);
				gtk_list_store_append(awayqueuestore, &iter);
				gtk_list_store_set(awayqueuestore, &iter,
								0, qm->name,
								1, qm->alias,
								2, _("(1 message)"),
								-1);
			}
		} else {
			/*
			 * Make sure the conversation
			 * exists and is updated (partly handled above already), play
			 * the receive sound (sound.c will take care of not playing
			 * while away), and then write it to the convo window.
			 */
			if (cnv == NULL)
				cnv = gaim_conversation_new(GAIM_CONV_IM, gc->account, name);

			gaim_conv_im_write(GAIM_CONV_IM(cnv), NULL, message, msgflags, mtime);
			gaim_conv_window_flash(gaim_conversation_get_window(cnv));
		}

		/*
		 * Regardless of whether we queue it or not, we should send an
		 * auto-response. That is, of course, unless the horse.... no wait.
		 * Don't autorespond if:
		 *
		 *  - it's not supported on this connection
		 *  - or the away message is empty
		 *  - or it's disabled
		 *  - or we're not idle and the 'only auto respond if idle' pref
		 *    is set
		 */
		auto_reply_pref = gaim_prefs_get_string("/core/away/auto_reply");
		if (!(gc->flags & GAIM_CONNECTION_AUTO_RESP) ||
			*gc->away == '\0' ||
			!strcmp(auto_reply_pref, "never") ||
			(!gc->is_idle &&
			 !strcmp(auto_reply_pref, "awayidle"))) {

			g_free(name);
			g_free(message);
			return;
		}

		/*
		 * This used to be based on the conversation window. But um, if
		 * you went away, and someone sent you a message and got your
		 * auto-response, and then you closed the window, and then the
		 * sent you another one, they'd get the auto-response back too
		 * soon. Besides that, we need to keep track of this even if we've
		 * got a queue. So the rest of this block is just the auto-response,
		 * if necessary.
		 */
		lar = get_last_auto_response(gc, name);
		if ((t - lar->sent) < SECS_BEFORE_RESENDING_AUTORESPONSE) {
			g_free(name);
			g_free(message);
			return;
		}
		lar->sent = t;

		/* apply default fonts and colors */
		tmpmsg = stylize(gc->away, MSG_LEN);

		/* Move this to oscar.c! */
		buffy = gaim_str_sub_away_formatters(tmpmsg, alias);
		serv_send_im(gc, name, buffy, GAIM_CONV_IM_AUTO_RESP);
		g_free(buffy);

		if (!cnv && awayqueue &&
			gaim_prefs_get_bool("/gaim/gtk/away/queue_messages")) {

			struct queued_message *qm;

			qm = g_new0(struct queued_message, 1);
			g_snprintf(qm->name, sizeof(qm->name), "%s", name);
			qm->message = g_strdup(gaim_str_sub_away_formatters(tmpmsg, alias));
			qm->account = gc->account;
			qm->tm = mtime;
			qm->flags = GAIM_MESSAGE_SEND | GAIM_MESSAGE_AUTO_RESP;
			message_queue = g_slist_append(message_queue, qm);
		} else if (cnv != NULL)
			gaim_conv_im_write(GAIM_CONV_IM(cnv), NULL, gaim_str_sub_away_formatters(tmpmsg, alias),
						  GAIM_MESSAGE_SEND | GAIM_MESSAGE_AUTO_RESP, mtime);

		g_free(tmpmsg);
	} else {
		/*
		 * We're not away. This is easy. If the convo window doesn't
		 * exist, create and update it (if it does exist it was updated
		 * earlier), then play a sound indicating we've received it and
		 * then display it. Easy.
		 */

		/* XXX UGLY HACK OF THE YEAR
		 * Robot101 will fix this after his exams. honest.
		 * I guess he didn't specify WHICH exams, exactly...
		 */
		if (docklet_count &&
		    gaim_prefs_get_bool("/plugins/gtk/docklet/queue_messages") &&
		    !gaim_find_conversation_with_account(name, gc->account)) {
			/*
			 * We're gonna queue it up and wait for the user to ask for
			 * it... probably by clicking the docklet or windows tray icon.
			 */
			struct queued_message *qm;
			qm = g_new0(struct queued_message, 1);
			g_snprintf(qm->name, sizeof(qm->name), "%s", name);
			qm->message = g_strdup(message);
			qm->account = gc->account;
			qm->tm = mtime;
			qm->flags = msgflags;
			unread_message_queue = g_slist_append(unread_message_queue, qm);
		}
		else {
			if (cnv == NULL)
				cnv = gaim_conversation_new(GAIM_CONV_IM, gc->account, name);

			gaim_conv_im_write(GAIM_CONV_IM(cnv), NULL, message, msgflags, mtime);
			gaim_conv_window_flash(gaim_conversation_get_window(cnv));
		}
	}

	g_free(name);
	g_free(message);
}

/*
 * NOTE: This is a bit hacky, but needed for core support for the
 *       buddy-idle-updated signal. It's temporary, and will be replaced
 *       with better code in the status rewrite.
 */
static GList *idle_buddies = NULL;
static guint idle_buddy_timeout_id = 0;

static gboolean
idle_timeout_cb(void)
{
	GList *l, *l_next;

	for (l = idle_buddies; l != NULL; l = l_next)
	{
		GaimBuddy *buddy = (GaimBuddy *)l->data;

		l_next = l->next;

		if (!GAIM_BUDDY_IS_ONLINE(buddy) || buddy->idle <= 0)
		{
			remove_idle_buddy(buddy);
		}
		else
		{
			gaim_signal_emit(gaim_blist_get_handle(), "buddy-idle-updated",
							 l->data);
		}
	}

	return TRUE;
}

static void
add_idle_buddy(GaimBuddy *buddy)
{
	idle_buddies = g_list_append(idle_buddies, buddy);

	if (idle_buddy_timeout_id == 0)
	{
		idle_buddy_timeout_id = gaim_timeout_add(10000,
			(GSourceFunc)idle_timeout_cb, NULL);
	}
}

static void
remove_idle_buddy(GaimBuddy *buddy)
{
	idle_buddies = g_list_remove(idle_buddies, buddy);

	if (idle_buddies == NULL)
	{
		gaim_timeout_remove(idle_buddy_timeout_id);
		idle_buddy_timeout_id = 0;
	}
}

/*
 * @param idle The time at which the buddy became idle, in seconds
 *             since the epoch.
 */
void serv_got_update(GaimConnection *gc, const char *name, gboolean loggedin,
					 int evil, time_t signon, time_t idle, int type)
{
	GaimAccount *account;
	GaimConversation *c;
	GaimBuddy *b;
	char *alias;
	GSList *buddies;
	int old_idle;
	time_t current_time = time(NULL);
	int signing_on = 0;
	int signing_off = 0;

	account = gaim_connection_get_account(gc);
	b = gaim_find_buddy(account, name);

	if (!b) {
		gaim_debug(GAIM_DEBUG_ERROR, "server", "No such buddy: %s\n", name);
		return;
	}

	c = gaim_find_conversation_with_account(b->name, account);

	/* This code will 'align' the name from the TOC */
	/* server with what's in our record.  We want to */
	/* store things how THEY want it... */
	if (strcmp(name, b->name)) {
		gaim_blist_rename_buddy(b, name);
	}

	alias = gaim_escape_html(gaim_buddy_get_alias(b));

	old_idle = b->idle;

	if (loggedin) {
		if (!GAIM_BUDDY_IS_ONLINE(b)) {
			signing_on = TRUE;
		}
	} else if (GAIM_BUDDY_IS_ONLINE(b)) {
		signing_off = TRUE;
	}


	if (signing_on) {
		if (c != NULL) {
			char *tmp = g_strdup_printf(_("%s logged in."), alias);

			gaim_conversation_write(c, NULL, tmp, GAIM_MESSAGE_SYSTEM,
									time(NULL));
			g_free(tmp);
		}
		else if (awayqueue && find_queue_total_by_name(b->name)) {
			struct queued_message *qm = g_new0(struct queued_message, 1);
			g_snprintf(qm->name, sizeof(qm->name), "%s", b->name);
			qm->message = g_strdup_printf(_("%s logged in."), alias);
			qm->account = gc->account;
			qm->tm = time(NULL);
			qm->flags = GAIM_MESSAGE_SYSTEM;
			message_queue = g_slist_append(message_queue, qm);
		}
		gaim_sound_play_event(GAIM_SOUND_BUDDY_ARRIVE);

		if(gaim_prefs_get_bool("/core/logging/log_system") &&
		   gaim_prefs_get_bool("/core/logging/log_signon_signoff")) {
			GaimAccount *account = gaim_connection_get_account(gc);
			GaimLog *log = gaim_account_get_log(account);
			char *tmp = g_strdup_printf(_("%s signed on"), alias);

			gaim_log_write(log, GAIM_MESSAGE_SYSTEM, (alias ? alias : name),
						   current_time, tmp);
			g_free(tmp);
		}
	}

	if(gaim_prefs_get_bool("/core/logging/log_system") &&
	   gaim_prefs_get_bool("/core/logging/log_away_state")) {
		GaimAccount *account = gaim_connection_get_account(gc);
		GaimLog *log = gaim_account_get_log(account);
		char *tmp = NULL;

		if((b->uc & UC_UNAVAILABLE) && !(type & UC_UNAVAILABLE))
			tmp = g_strdup_printf(_("%s came back"), alias);
		else if(!(b->uc & UC_UNAVAILABLE) && (type & UC_UNAVAILABLE))
			tmp = g_strdup_printf(_("%s went away"), alias);

		if(tmp){
			gaim_log_write(log, GAIM_MESSAGE_SYSTEM, (alias ? alias : name),
						   current_time, tmp);
			g_free(tmp);
		}
	}

	if (!old_idle && idle) {
		if(gaim_prefs_get_bool("/core/logging/log_system") &&
		   gaim_prefs_get_bool("/core/logging/log_idle_state")) {
			GaimAccount *account = gaim_connection_get_account(gc);
			GaimLog *log = gaim_account_get_log(account);
			char *tmp = g_strdup_printf(_("%s became idle"), alias);

			gaim_log_write(log, GAIM_MESSAGE_SYSTEM, (alias ? alias : name),
						   current_time, tmp);
			g_free(tmp);
		}
	} else if (old_idle && !idle) {
		if(gaim_prefs_get_bool("/core/logging/log_system") &&
		   gaim_prefs_get_bool("/core/logging/log_idle_state")) {
			GaimAccount *account = gaim_connection_get_account(gc);
			GaimLog *log = gaim_account_get_log(account);
			char *tmp = g_strdup_printf(_("%s became unidle"), alias);

			gaim_log_write(log, GAIM_MESSAGE_SYSTEM, (alias ? alias : name),
						   current_time, tmp);
			g_free(tmp);
		}
	}

	if (signing_off) {
		if (c != NULL) {
			char *tmp = g_strdup_printf(_("%s logged out."), alias);
			gaim_conversation_write(c, NULL, tmp,
									GAIM_MESSAGE_SYSTEM, time(NULL));
			g_free(tmp);
		} else if (awayqueue && find_queue_total_by_name(b->name)) {
			struct queued_message *qm = g_new0(struct queued_message, 1);
			g_snprintf(qm->name, sizeof(qm->name), "%s", b->name);
			qm->message = g_strdup_printf(_("%s logged out."), alias);
			qm->account = gc->account;
			qm->tm = time(NULL);
			qm->flags = GAIM_MESSAGE_SYSTEM;
			message_queue = g_slist_append(message_queue, qm);
		}
		serv_got_typing_stopped(gc, name); /* obviously not typing */
		gaim_sound_play_event(GAIM_SOUND_BUDDY_LEAVE);

		if(gaim_prefs_get_bool("/core/logging/log_system") &&
		   gaim_prefs_get_bool("/core/logging/log_signon_signoff")) {
			GaimAccount *account = gaim_connection_get_account(gc);
			GaimLog *log = gaim_account_get_log(account);
			char *tmp = g_strdup_printf(_("%s signed off"), alias);

			gaim_log_write(log, GAIM_MESSAGE_SYSTEM, (alias ? alias : name),
						   current_time, tmp);
			g_free(tmp);
		}
	}

	if (gc->login_time_official && gc->login_time && signon > 0)
		signon += gc->login_time_official - gc->login_time;
	gaim_blist_update_buddy_signon(b, signon);
	gaim_blist_update_buddy_idle(b, idle);
	gaim_blist_update_buddy_evil(b, evil);
	gaim_blist_update_buddy_status(b, type);

	if (!old_idle && idle)
	{
		gaim_signal_emit(gaim_blist_get_handle(), "buddy-idle", b);

		add_idle_buddy(b);
	}
	else if (old_idle && !idle)
	{
		gaim_signal_emit(gaim_blist_get_handle(), "buddy-unidle", b);

		remove_idle_buddy(b);
	}

	if (c != NULL)
		gaim_conversation_update(c, GAIM_CONV_UPDATE_AWAY);

	gaim_blist_update_buddy_presence(b, loggedin);

	for (buddies = gaim_find_buddies(account, name); buddies; buddies = g_slist_remove(buddies, buddies->data)) {
		b = buddies->data;
		gaim_blist_update_buddy_presence(b, loggedin);
		gaim_blist_update_buddy_idle(b, idle);
		gaim_blist_update_buddy_evil(b, evil);
		gaim_blist_update_buddy_status(b, type);
	}
	g_free(alias);
}


void serv_got_eviled(GaimConnection *gc, const char *name, int lev)
{
	char buf2[1024];
	GaimAccount *account;

	account = gaim_connection_get_account(gc);

	gaim_signal_emit(gaim_accounts_get_handle(), "account-warned",
					 account, name, lev);

	if (gc->evil >= lev) {
		gc->evil = lev;
		return;
	}

	gc->evil = lev;

	g_snprintf(buf2, sizeof(buf2),
			   _("%s has just been warned by %s.\n"
				 "Your new warning level is %d%%"),
			   gaim_account_get_username(gaim_connection_get_account(gc)),
			   ((name == NULL) ? _("an anonymous person") : name), lev);

	gaim_notify_info(NULL, NULL, buf2, NULL);
}

void serv_got_typing(GaimConnection *gc, const char *name, int timeout,
					 GaimTypingState state) {

	GaimBuddy *b;
	GaimConversation *cnv = gaim_find_conversation_with_account(name, gc->account);
	GaimConvIm *im;

	if (!cnv)
		return;

	im = GAIM_CONV_IM(cnv);

	gaim_conversation_set_account(cnv, gc->account);
	gaim_conv_im_set_typing_state(im, state);
	gaim_conv_im_update_typing(im);

	b = gaim_find_buddy(gc->account, name);

	if (b != NULL)
	{
		if (state == GAIM_TYPING)
		{
			gaim_signal_emit(gaim_conversations_get_handle(),
							 "buddy-typing", cnv);
		}
		else
		{
			gaim_signal_emit(gaim_conversations_get_handle(),
							 "buddy-typing-stopped", cnv);
		}
	}

	if (timeout > 0)
		gaim_conv_im_start_typing_timeout(im, timeout);
}

void serv_got_typing_stopped(GaimConnection *gc, const char *name) {

	GaimConversation *c = gaim_find_conversation_with_account(name, gc->account);
	GaimConvIm *im;
	GaimBuddy *b;

	if (!c)
		return;

	im = GAIM_CONV_IM(c);

	if (im->typing_state == GAIM_NOT_TYPING)
		return;

	gaim_conv_im_stop_typing_timeout(im);
	gaim_conv_im_set_typing_state(im, GAIM_NOT_TYPING);
	gaim_conv_im_update_typing(im);

	b = gaim_find_buddy(gc->account, name);

	if (b != NULL)
	{
		gaim_signal_emit(gaim_conversations_get_handle(),
						 "buddy-typing-stopped", c);
	}
}

struct chat_invite_data {
	GaimConnection *gc;
	GHashTable *components;
};

static void chat_invite_data_free(struct chat_invite_data *cid)
{
	if (cid->components)
		g_hash_table_destroy(cid->components);
	g_free(cid);
}


static void chat_invite_reject(struct chat_invite_data *cid)
{
	serv_reject_chat(cid->gc, cid->components);
	chat_invite_data_free(cid);
}


static void chat_invite_accept(struct chat_invite_data *cid)
{
	serv_join_chat(cid->gc, cid->components);
	chat_invite_data_free(cid);
}



void serv_got_chat_invite(GaimConnection *gc, const char *name,
						  const char *who, const char *message, GHashTable *data)
{
	GaimAccount *account;
	char buf2[BUF_LONG];
	struct chat_invite_data *cid = g_new0(struct chat_invite_data, 1);

	account = gaim_connection_get_account(gc);

	gaim_signal_emit(gaim_conversations_get_handle(),
					 "chat-invited", account, who, name, message, data);

	if (message)
		g_snprintf(buf2, sizeof(buf2),
				   _("User '%s' invites %s to buddy chat room: '%s'\n%s"),
				   who, gaim_account_get_username(account), name, message);
	else
		g_snprintf(buf2, sizeof(buf2),
				   _("User '%s' invites %s to buddy chat room: '%s'\n"),
				   who, gaim_account_get_username(account), name);

	cid->gc = gc;
	cid->components = data;

	gaim_request_accept_cancel(gc, NULL, _("Accept chat invitation?"),
							   buf2, 0, cid,
							   G_CALLBACK(chat_invite_accept),
							   G_CALLBACK(chat_invite_reject));
}

GaimConversation *serv_got_joined_chat(GaimConnection *gc,
											   int id, const char *name)
{
	GaimConversation *conv;
	GaimConvChat *chat;
	GaimAccount *account;

	account = gaim_connection_get_account(gc);

	conv = gaim_conversation_new(GAIM_CONV_CHAT, account, name);
	chat = GAIM_CONV_CHAT(conv);

	if (!g_slist_find(gc->buddy_chats, conv))
		gc->buddy_chats = g_slist_append(gc->buddy_chats, conv);

	gaim_conv_chat_set_id(chat, id);

	gaim_conv_window_show(gaim_conversation_get_window(conv));
	gaim_conv_window_switch_conversation(gaim_conversation_get_window(conv),
									gaim_conversation_get_index(conv));

	gaim_signal_emit(gaim_conversations_get_handle(), "chat-joined", conv);

	return conv;
}

void serv_got_chat_left(GaimConnection *g, int id)
{
	GSList *bcs;
	GaimConversation *conv = NULL;
	GaimConvChat *chat = NULL;
	GaimAccount *account;

	account = gaim_connection_get_account(g);

	for (bcs = g->buddy_chats; bcs != NULL; bcs = bcs->next) {
		conv = (GaimConversation *)bcs->data;

		chat = GAIM_CONV_CHAT(conv);

		if (gaim_conv_chat_get_id(chat) == id)
			break;

		conv = NULL;
	}

	if (!conv)
		return;

	gaim_signal_emit(gaim_conversations_get_handle(), "chat-left", conv);

	gaim_debug(GAIM_DEBUG_INFO, "server", "Leaving room: %s\n",
			   gaim_conversation_get_name(conv));

	g->buddy_chats = g_slist_remove(g->buddy_chats, conv);

	gaim_conv_chat_left(GAIM_CONV_CHAT(conv));
}

void serv_got_chat_in(GaimConnection *g, int id, const char *who,
					  GaimConvChatFlags chatflags, const char *message, time_t mtime)
{
	GaimMessageFlags msgflags = 0;
	GSList *bcs;
	GaimConversation *conv = NULL;
	GaimConvChat *chat = NULL;
	char *buf;
	char *buffy, *angel;
	int plugin_return;

	for (bcs = g->buddy_chats; bcs != NULL; bcs = bcs->next) {
		conv = (GaimConversation *)bcs->data;

		chat = GAIM_CONV_CHAT(conv);

		if (gaim_conv_chat_get_id(chat) == id)
			break;

		conv = NULL;
	}

	if (!conv)
		return;

	/*
	 * Plugin stuff. We pass a char ** but we don't want to pass what's
	 * been given us by the prpls. so we create temp holders and pass those
	 * instead. It's basically just to avoid segfaults. Of course, if the
	 * data is binary, plugins don't see it. Bitch all you want; i really
	 * don't want you to be dealing with it.
	 */

	buffy = g_malloc(MAX(strlen(message) + 1, BUF_LONG));
	strcpy(buffy, message);
	angel = g_strdup(who);

	plugin_return = GPOINTER_TO_INT(
		gaim_signal_emit_return_1(gaim_conversations_get_handle(),
								  "receiving-chat-msg", g->account,
								  &angel, &buffy, conv));

	if (!buffy || !angel || plugin_return) {
		if (buffy)
			g_free(buffy);
		if (angel)
			g_free(angel);
		return;
	}
	who = angel;
	message = buffy;

	gaim_signal_emit(gaim_conversations_get_handle(), "received-chat-msg", g->account,
					 who, message, conv);

	/* Make sure URLs are clickable */
	buf = gaim_markup_linkify(message);

	if (chatflags & GAIM_CONV_CHAT_WHISPER)
		msgflags |= GAIM_MESSAGE_WHISPER;
	if (chatflags & GAIM_CONV_CHAT_DELAYED)
		msgflags |= GAIM_MESSAGE_DELAYED;

	gaim_conv_chat_write(chat, who, buf, msgflags, mtime);

	g_free(angel);
	g_free(buf);
	g_free(buffy);
}

void serv_send_file(GaimConnection *gc, const char *who, const char *file)
{
	GaimPluginProtocolInfo *prpl_info = NULL;
	
	if (gc != NULL && gc->prpl != NULL)
		prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(gc->prpl);

	if (prpl_info && prpl_info->send_file) {
		if (!prpl_info->can_receive_file || prpl_info->can_receive_file(gc, who)) {
			prpl_info->send_file(gc, who, file);
		}
	}
}
