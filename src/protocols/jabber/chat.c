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
#include "multi.h" /* for proto_chat_entry */
#include "notify.h"
#include "util.h"

#include "chat.h"
#include "iq.h"
#include "message.h"
#include "presence.h"
#include "xdata.h"

GList *jabber_chat_info(GaimConnection *gc)
{
	GList *m = NULL;
	struct proto_chat_entry *pce;
	JabberStream *js = gc->proto_data;

	pce = g_new0(struct proto_chat_entry, 1);
	pce->label = _("_Room:");
	pce->identifier = "room";
	m = g_list_append(m, pce);

	/* we're gonna default to a conference server I know is true, until
	 * I can figure out how to disco for a chat server */
	pce = g_new0(struct proto_chat_entry, 1);
	pce->label = _("_Server:");
	pce->identifier = "server";
	pce->def = "conference.jabber.org";
	m = g_list_append(m, pce);

	pce = g_new0(struct proto_chat_entry, 1);
	pce->label = _("_Handle:");
	pce->identifier = "handle";
	pce->def = js->user->node;
	m = g_list_append(m, pce);

	pce = g_new0(struct proto_chat_entry, 1);
	pce->label = _("_Password:");
	pce->identifier = "password";
	pce->secret = TRUE;
	m = g_list_append(m, pce);

	return m;
}

JabberChat *jabber_chat_find(JabberStream *js, const char *room,
		const char *server)
{
	JabberChat *chat;
	char *room_jid;

	room_jid = g_strdup_printf("%s@%s", room, server);

	chat = g_hash_table_lookup(js->chats, jabber_normalize(NULL, room_jid));
	g_free(room_jid);

	return chat;
}

struct _find_by_id_data {
	int id;
	JabberChat *chat;
};

void find_by_id_foreach_cb(gpointer key, gpointer value, gpointer user_data)
{
	JabberChat *chat = value;
	struct _find_by_id_data *fbid = user_data;

	if(chat->id == fbid->id)
		fbid->chat = chat;
}

JabberChat *jabber_chat_find_by_id(JabberStream *js, int id)
{
	JabberChat *chat;
	struct _find_by_id_data *fbid = g_new0(struct _find_by_id_data, 1);
	fbid->id = id;
	g_hash_table_foreach(js->chats, find_by_id_foreach_cb, fbid);
	chat = fbid->chat;
	g_free(fbid);
	return chat;
}

void jabber_chat_invite(GaimConnection *gc, int id, const char *msg,
		const char *name)
{
	JabberStream *js = gc->proto_data;
	JabberChat *chat;
	xmlnode *message, *body, *x, *invite;
	char *room_jid;

	chat = jabber_chat_find_by_id(js, id);
	if(!chat)
		return;

	message = xmlnode_new("message");

	room_jid = g_strdup_printf("%s@%s", chat->room, chat->server);

	if(chat->muc) {
		xmlnode_set_attrib(message, "to", room_jid);
		x = xmlnode_new_child(message, "x");
		xmlnode_set_attrib(x, "xmlns", "http://jabber.org/protocol/muc#user");
		invite = xmlnode_new_child(x, "invite");
		xmlnode_set_attrib(invite, "to", name);
		body = xmlnode_new_child(invite, "reason");
		xmlnode_insert_data(body, msg, -1);
	} else {
		xmlnode_set_attrib(message, "to", name);
		body = xmlnode_new_child(message, "body");
		xmlnode_insert_data(body, msg, -1);
		x = xmlnode_new_child(message, "x");
		xmlnode_set_attrib(x, "jid", room_jid);
		xmlnode_set_attrib(x, "xmlns", "jabber:x:conference");
	}

	jabber_send(js, message);
	xmlnode_free(message);
	g_free(room_jid);
}

void jabber_chat_join(GaimConnection *gc, GHashTable *data)
{
	JabberChat *chat;
	char *room, *server, *handle, *passwd;
	xmlnode *presence, *x;
	char *tmp, *room_jid, *full_jid;
	JabberStream *js = gc->proto_data;

	room = g_hash_table_lookup(data, "room");
	server = g_hash_table_lookup(data, "server");
	handle = g_hash_table_lookup(data, "handle");
	passwd = g_hash_table_lookup(data, "password");

	if(!room || !server || !handle)
		return;

	if(!jabber_nodeprep_validate(room)) {
		char *buf = g_strdup_printf(_("%s is not a valid room name"), room);
		gaim_notify_error(gc, _("Invalid Room Name"), _("Invalid Room Name"),
				buf);
		g_free(buf);
		return;
	} else if(!jabber_nameprep_validate(server)) {
		char *buf = g_strdup_printf(_("%s is not a valid server name"), server);
		gaim_notify_error(gc, _("Invalid Server Name"),
				_("Invalid Server Name"), buf);
		g_free(buf);
		return;
	} else if(!jabber_resourceprep_validate(handle)) {
		char *buf = g_strdup_printf(_("%s is not a valid room handle"), handle);
		gaim_notify_error(gc, _("Invalid Room Handle"),
				_("Invalid Room Handle"), buf);
	}

	if(jabber_chat_find(js, room, server))
		return;

	tmp = g_strdup_printf("%s@%s", room, server);
	room_jid = g_strdup(jabber_normalize(NULL, tmp));
	g_free(tmp);

	chat = g_new0(JabberChat, 1);
	chat->js = gc->proto_data;

	chat->room = g_strdup(room);
	chat->server = g_strdup(server);
	chat->nick = g_strdup(handle);

	g_hash_table_insert(js->chats, room_jid, chat);

	presence = jabber_presence_create(gc->away_state, gc->away);
	full_jid = g_strdup_printf("%s/%s", room_jid, handle);
	xmlnode_set_attrib(presence, "to", full_jid);
	g_free(full_jid);

	x = xmlnode_new_child(presence, "x");
	xmlnode_set_attrib(x, "xmlns", "http://jabber.org/protocol/muc");

	if(passwd && *passwd) {
		xmlnode *password = xmlnode_new_child(x, "password");
		xmlnode_insert_data(password, passwd, -1);
	}

	jabber_send(js, presence);
	xmlnode_free(presence);
}

void jabber_chat_leave(GaimConnection *gc, int id)
{
	JabberStream *js = gc->proto_data;
	JabberChat *chat = jabber_chat_find_by_id(js, id);
	char *room_jid;
	xmlnode *presence;

	if(!chat)
		return;

	room_jid = g_strdup_printf("%s@%s", chat->room, chat->server);
	gaim_debug(GAIM_DEBUG_INFO, "jabber", "%s is leaving chat %s\n",
			chat->nick, room_jid);
	presence = xmlnode_new("presence");
	xmlnode_set_attrib(presence, "to", room_jid);
	xmlnode_set_attrib(presence, "type", "unavailable");
	jabber_send(js, presence);
	xmlnode_free(presence);
}

void jabber_chat_destroy(JabberChat *chat)
{
	JabberStream *js = chat->js;
	char *room_jid = g_strdup_printf("%s@%s", chat->room, chat->server);

	g_hash_table_remove(js->chats, jabber_normalize(NULL, room_jid));
	g_free(room_jid);

	g_free(chat->room);
	g_free(chat->server);
	g_free(chat->nick);
	g_free(chat);
}

gboolean jabber_chat_find_buddy(GaimConversation *conv, const char *name)
{
	GList *m = gaim_conv_chat_get_users(GAIM_CONV_CHAT(conv));

	while(m) {
		if(!strcmp(m->data, name))
			return TRUE;
		m = m->next;
	}

	return FALSE;
}

char *jabber_chat_buddy_real_name(GaimConnection *gc, int id, const char *who)
{
	JabberStream *js = gc->proto_data;
	JabberChat *chat;

	chat = jabber_chat_find_by_id(js, id);

	if(!chat)
		return NULL;

	return g_strdup_printf("%s@%s/%s", chat->room, chat->server, who);
}

static void jabber_chat_room_configure_x_data_cb(JabberStream *js, xmlnode *result, gpointer data)
{
	JabberChat *chat = data;
	xmlnode *query;
	JabberIq *iq;
	char *to = g_strdup_printf("%s@%s", chat->room, chat->server);

	iq = jabber_iq_new_query(js, JABBER_IQ_SET, "http://jabber.org/protocol/muc#owner");
	xmlnode_set_attrib(iq->node, "to", to);
	g_free(to);

	query = xmlnode_get_child(iq->node, "query");

	xmlnode_insert_child(query, result);

	jabber_iq_send(iq);
}

static void jabber_chat_room_configure_cb(JabberStream *js, xmlnode *packet, gpointer data)
{
	xmlnode *query, *x;
	const char *type = xmlnode_get_attrib(packet, "type");
	const char *from = xmlnode_get_attrib(packet, "from");
	char *msg;
	JabberChat *chat;
	JabberID *jid;

	if(!type || !from)
		return;



	if(!strcmp(type, "result")) {
		jid = jabber_id_new(from);

		if(!jid)
			return;

		chat = jabber_chat_find(js, jid->node, jid->domain);
		jabber_id_free(jid);

		if(!chat)
			return;

		if(!(query = xmlnode_get_child(packet, "query")))
			return;

		for(x = query->child; x; x = x->next) {
			const char *xmlns;
			if(strcmp(x->name, "x"))
				continue;

			if(!(xmlns = xmlnode_get_attrib(x, "xmlns")))
				continue;

			if(!strcmp(xmlns, "jabber:x:data")) {
				jabber_x_data_request(js, x, jabber_chat_room_configure_x_data_cb, chat);
				return;
			}
		}
	} else if(!strcmp(type, "error")) {
		xmlnode *errnode = xmlnode_get_child(packet, "error");
		const char *code = NULL;
		char *code_txt = NULL;
		char *msg;
		char *text = NULL;

		if(errnode) {
			code = xmlnode_get_attrib(errnode, "code");
			text = xmlnode_get_data(errnode);
		}

		if(code)
			code_txt = g_strdup_printf(_(" (Code %s)"), code);

		msg = g_strdup_printf("%s%s", text ? text : "", code_txt ? code_txt : "");
		gaim_notify_error(js->gc, _("Configuration error"), _("Configuration error"), msg);

		g_free(msg);
		if(code_txt)
			g_free(code_txt);

		return;
	}

	msg = g_strdup_printf("Unable to configure room %s", from);

	gaim_notify_info(js->gc, _("Unable to configure"), _("Unable to configure"), msg);
	g_free(msg);

}

void jabber_chat_request_room_configure(JabberChat *chat) {
	JabberIq *iq;
	xmlnode *query;
	char *room_jid;

	if(!chat)
		return;

	if(!chat->muc) {
		gaim_notify_error(chat->js->gc, _("Room Configuration Error"), _("Room Configuration Error"),
				_("This room is not capable of being configured"));
		return;
	}

	iq = jabber_iq_new_query(chat->js, JABBER_IQ_SET,
			"http://jabber.org/protocol/muc#owner");
	query = xmlnode_get_child(iq->node, "query");
	room_jid = g_strdup_printf("%s@%s", chat->room, chat->server);

	xmlnode_set_attrib(iq->node, "to", room_jid);

	jabber_iq_set_callback(iq, jabber_chat_room_configure_cb, NULL);

	jabber_iq_send(iq);

	g_free(room_jid);
}

void jabber_chat_create_instant_room(JabberChat *chat) {
	JabberIq *iq;
	xmlnode *query, *x;
	char *room_jid;

	if(!chat)
		return;

	iq = jabber_iq_new_query(chat->js, JABBER_IQ_SET,
			"http://jabber.org/protocol/muc#owner");
	query = xmlnode_get_child(iq->node, "query");
	x = xmlnode_new_child(query, "x");
	room_jid = g_strdup_printf("%s@%s", chat->room, chat->server);

	xmlnode_set_attrib(iq->node, "to", room_jid);
	xmlnode_set_attrib(x, "xmlns", "jabber:x:data");
	xmlnode_set_attrib(x, "type", "submit");

	jabber_iq_send(iq);

	g_free(room_jid);
}

static void jabber_chat_register_x_data_result_cb(JabberStream *js, xmlnode *packet, gpointer data)
{
	const char *type = xmlnode_get_attrib(packet, "type");

	if(type && !strcmp(type, "error")) {
		/* XXX: handle an error (you'll get a 409 if the nick is already registered) */
	}
}

static void jabber_chat_register_x_data_cb(JabberStream *js, xmlnode *result, gpointer data)
{
	JabberChat *chat = data;
	xmlnode *query;
	JabberIq *iq;
	char *to = g_strdup_printf("%s@%s", chat->room, chat->server);

	iq = jabber_iq_new_query(js, JABBER_IQ_SET, "jabber:iq:register");
	xmlnode_set_attrib(iq->node, "to", to);
	g_free(to);

	query = xmlnode_get_child(iq->node, "query");

	xmlnode_insert_child(query, result);

	jabber_iq_set_callback(iq, jabber_chat_register_x_data_result_cb, NULL);

	jabber_iq_send(iq);
}

static void jabber_chat_register_cb(JabberStream *js, xmlnode *packet, gpointer data)
{
	xmlnode *query, *x;
	const char *type = xmlnode_get_attrib(packet, "type");
	const char *from = xmlnode_get_attrib(packet, "from");
	char *msg;
	JabberChat *chat;
	JabberID *jid;

	if(!type || !from)
		return;

	if(!strcmp(type, "result")) {
		jid = jabber_id_new(from);

		if(!jid)
			return;

		chat = jabber_chat_find(js, jid->node, jid->domain);
		jabber_id_free(jid);

		if(!chat)
			return;

		if(!(query = xmlnode_get_child(packet, "query")))
			return;

		for(x = query->child; x; x = x->next) {
			const char *xmlns;
			if(strcmp(x->name, "x"))
				continue;

			if(!(xmlns = xmlnode_get_attrib(x, "xmlns")))
				continue;

			if(!strcmp(xmlns, "jabber:x:data")) {
				jabber_x_data_request(js, x, jabber_chat_register_x_data_cb, chat);
				return;
			}
		}
	} else if(!strcmp(type, "error")) {
		/* XXX: how many places is this code duplicated?  Fix it, immediately */
		xmlnode *errnode = xmlnode_get_child(packet, "error");
		const char *code = NULL;
		char *code_txt = NULL;
		char *msg;
		char *text = NULL;

		if(errnode) {
			code = xmlnode_get_attrib(errnode, "code");
			text = xmlnode_get_data(errnode);
		}

		if(code)
			code_txt = g_strdup_printf(_(" (Code %s)"), code);

		msg = g_strdup_printf("%s%s", text ? text : "", code_txt ? code_txt : "");
		gaim_notify_error(js->gc, _("Registration error"), _("Registration error"), msg);

		g_free(msg);
		if(code_txt)
			g_free(code_txt);

		return;
	}

	msg = g_strdup_printf("Unable to configure room %s", from);

	gaim_notify_info(js->gc, _("Unable to configure"), _("Unable to configure"), msg);
	g_free(msg);

}

void jabber_chat_register(JabberChat *chat)
{
	JabberIq *iq;
	char *room_jid;

	if(!chat)
		return;

	room_jid = g_strdup_printf("%s@%s", chat->room, chat->server);

	iq = jabber_iq_new_query(chat->js, JABBER_IQ_GET, "jabber:iq:register");
	xmlnode_set_attrib(iq->node, "to", room_jid);
	g_free(room_jid);

	jabber_iq_set_callback(iq, jabber_chat_register_cb, NULL);

	jabber_iq_send(iq);
}

/* merge this with the function below when we get everyone on the same page wrt /commands */
void jabber_chat_change_topic(JabberChat *chat, const char *topic)
{
	if(topic && *topic) {
		JabberMessage *jm;
		jm = g_new0(JabberMessage, 1);
		jm->js = chat->js;
		jm->type = JABBER_MESSAGE_GROUPCHAT;
		jm->subject = gaim_markup_strip_html(topic);
		jm->to = g_strdup_printf("%s@%s", chat->room, chat->server);
		jabber_message_send(jm);
		jabber_message_free(jm);
	} else {
		const char *cur = gaim_conv_chat_get_topic(GAIM_CONV_CHAT(chat->conv));
		char *buf;

		if(cur)
			buf = g_strdup_printf(_("current topic is: %s"), topic);
		else
			buf = g_strdup(_("No topic is set"));
		gaim_conv_chat_write(GAIM_CONV_CHAT(chat->conv), "", buf,
				GAIM_MESSAGE_SYSTEM | GAIM_MESSAGE_NO_LOG, time(NULL));
		g_free(buf);
	}

}

void jabber_chat_set_topic(GaimConnection *gc, int id, const char *topic)
{
	JabberStream *js = gc->proto_data;
	JabberChat *chat = jabber_chat_find_by_id(js, id);

	if(!chat)
		return;

	jabber_chat_change_topic(chat, topic);
}


void jabber_chat_change_nick(JabberChat *chat, const char *nick)
{
	xmlnode *presence;
	char *full_jid;

	if(!chat->muc) {
		gaim_conv_chat_write(GAIM_CONV_CHAT(chat->conv), "",
				_("Nick changing not supported in non-MUC chatrooms"),
				GAIM_MESSAGE_SYSTEM, time(NULL));
		return;
	}

	presence = jabber_presence_create(chat->js->gc->away_state, chat->js->gc->away);
	full_jid = g_strdup_printf("%s@%s/%s", chat->room, chat->server, nick);
	xmlnode_set_attrib(presence, "to", full_jid);
	g_free(full_jid);

	jabber_send(chat->js, presence);
	xmlnode_free(presence);
}


