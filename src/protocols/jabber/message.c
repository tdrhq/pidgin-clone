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
#include "notify.h"
#include "server.h"
#include "util.h"

#include "buddy.h"
#include "chat.h"
#include "message.h"
#include "xmlnode.h"

#define JABBER_TYPING_NOTIFY_INT 15

void jabber_message_free(JabberMessage *jm)
{
	if(jm->from)
		g_free(jm->from);
	if(jm->to)
		g_free(jm->to);
	if(jm->subject)
		g_free(jm->subject);
	if(jm->body)
		g_free(jm->body);
	if(jm->xhtml)
		g_free(jm->xhtml);
	if(jm->password)
		g_free(jm->password);
	if(jm->etc)
		g_list_free(jm->etc);

	g_free(jm);
}

static void handle_chat(JabberMessage *jm)
{
	JabberID *jid = jabber_id_new(jm->from);
	char *from;

	JabberBuddy *jb;
	JabberBuddyResource *jbr;

	if(!jid)
		return;

	jb = jabber_buddy_find(jm->js, jm->from, TRUE);
	jbr = jabber_buddy_find_resource(jb, jid->resource);

	if(jabber_find_unnormalized_conv(jm->from, jm->js->gc->account)) {
		from = g_strdup(jm->from);
	} else  if(jid->node) {
		GaimConversation *conv;

		from = g_strdup_printf("%s@%s", jid->node, jid->domain);
		conv = jabber_find_unnormalized_conv(from, jm->js->gc->account);
		if(conv)
			gaim_conversation_set_name(conv, jm->from);
		g_free(from);
		from = g_strdup(jm->from);
	} else {
		from = g_strdup(jid->domain);
	}

	if(!jm->xhtml && !jm->body) {
		if(jm->events & JABBER_MESSAGE_EVENT_COMPOSING)
			serv_got_typing(jm->js->gc, from, 0, GAIM_TYPING);
		else
			serv_got_typing_stopped(jm->js->gc, from);
	} else {
		if(jbr) {
			if(jm->events & JABBER_MESSAGE_EVENT_COMPOSING)
				jbr->capabilities |= JABBER_CAP_COMPOSING;
			if(jbr->thread_id)
				g_free(jbr->thread_id);
			jbr->thread_id = g_strdup(jbr->thread_id);
		}
		serv_got_im(jm->js->gc, from, jm->xhtml ? jm->xhtml : jm->body, 0,
				jm->sent);
	}

	g_free(from);
	jabber_id_free(jid);
}

static void handle_headline(JabberMessage *jm)
{
	char *title;
	GString *body = g_string_new("");
	GList *etc;

	title = g_strdup_printf(_("Message from %s"), jm->from);

	if(jm->xhtml)
		g_string_append(body, jm->xhtml);
	else if(jm->body)
		g_string_append(body, jm->body);

	for(etc = jm->etc; etc; etc = etc->next) {
		xmlnode *x = etc->data;
		const char *xmlns = xmlnode_get_attrib(x, "xmlns");
		if(xmlns && !strcmp(xmlns, "jabber:x:oob")) {
			xmlnode *url, *desc;
			char *urltxt, *desctxt;

			url = xmlnode_get_child(x, "url");
			desc = xmlnode_get_child(x, "desc");

			if(!url || !desc)
				continue;

			urltxt = xmlnode_get_data(url);
			desctxt = xmlnode_get_data(desc);

			/* I'm all about ugly hacks */
			if(body->len && !strcmp(body->str, jm->body))
				g_string_printf(body, "<a href='%s'>%s</a>",
						urltxt, desctxt);
			else
				g_string_append_printf(body, "<br/><a href='%s'>%s</a>",
						urltxt, desctxt);

			g_free(urltxt);
			g_free(desctxt);
		}
	}

	gaim_notify_formatted(jm->js->gc, title, jm->subject ? jm->subject : title,
			NULL, body->str, NULL, NULL);

	g_free(title);
	g_string_free(body, TRUE);
}

static void handle_groupchat(JabberMessage *jm)
{
	JabberID *jid = jabber_id_new(jm->from);
	JabberChat *chat;

	if(!jid)
		return;

	chat = jabber_chat_find(jm->js, jid->node, jid->domain);

	if(!chat)
		return;

	if(jm->subject) {
		gaim_conv_chat_set_topic(GAIM_CONV_CHAT(chat->conv), jid->resource,
				jm->subject);
		if(!jm->xhtml && !jm->body) {
			char *msg;
			if(jid->resource)
				msg = g_strdup_printf(_("%s has set the topic to: %s"), jid->resource, jm->subject);
			else
				msg = g_strdup_printf(_("The topic is: %s"), jm->subject);
			gaim_conv_chat_write(GAIM_CONV_CHAT(chat->conv), "", msg, GAIM_MESSAGE_SYSTEM, jm->sent);
			g_free(msg);
		}
	}

	if(jm->xhtml || jm->body) {
		if(jid->resource)
			serv_got_chat_in(jm->js->gc, chat->id, jid->resource, 0,
							jm->xhtml ? jm->xhtml : jm->body, jm->sent);
		else if(chat->muc)
			gaim_conv_chat_write(GAIM_CONV_CHAT(chat->conv), "",
							jm->xhtml ? jm->xhtml : jm->body,
							GAIM_MESSAGE_SYSTEM, jm->sent);
	}

	jabber_id_free(jid);
}

static void handle_groupchat_invite(JabberMessage *jm)
{
	GHashTable *components;
	JabberID *jid = jabber_id_new(jm->to);
	char *stripped;

	if(!jid)
		return;

	components = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

	g_hash_table_replace(components, g_strdup("room"), g_strdup(jid->node));
	g_hash_table_replace(components, g_strdup("server"), g_strdup(jid->domain));
	g_hash_table_replace(components, g_strdup("handle"),
			g_strdup(jm->js->user->node));
	g_hash_table_replace(components, g_strdup("password"),
			g_strdup(jm->password));

	jabber_id_free(jid);
	stripped = gaim_markup_strip_html(jm->body);
	serv_got_chat_invite(jm->js->gc, jm->to, jm->from, stripped, components);
	g_free(stripped);
}

static void handle_error(JabberMessage *jm)
{
	char *buf;

	if(!jm->body)
		return;

	buf = g_strdup_printf(_("Message delivery to %s failed: %s"),
			jm->from, jm->error);

	gaim_notify_formatted(jm->js->gc, _("Jabber Message Error"), _("Jabber Message Error"), buf,
			jm->xhtml ? jm->xhtml : jm->body, NULL, NULL);

	g_free(buf);
}

void jabber_message_parse(JabberStream *js, xmlnode *packet)
{
	JabberMessage *jm;
	const char *type;
	xmlnode *child;

	if(strcmp(packet->name, "message"))
		return;

	jm = g_new0(JabberMessage, 1);
	jm->js = js;
	jm->sent = time(NULL);

	type = xmlnode_get_attrib(packet, "type");

	if(type) {
		if(!strcmp(type, "normal"))
			jm->type = JABBER_MESSAGE_NORMAL;
		else if(!strcmp(type, "chat"))
			jm->type = JABBER_MESSAGE_CHAT;
		else if(!strcmp(type, "groupchat"))
			jm->type = JABBER_MESSAGE_GROUPCHAT;
		else if(!strcmp(type, "headline"))
			jm->type = JABBER_MESSAGE_HEADLINE;
		else if(!strcmp(type, "error"))
			jm->type = JABBER_MESSAGE_ERROR;
		else
			jm->type = JABBER_MESSAGE_OTHER;
	} else {
		jm->type = JABBER_MESSAGE_NORMAL;
	}

	jm->from = g_strdup(xmlnode_get_attrib(packet, "from"));
	jm->to = g_strdup(xmlnode_get_attrib(packet, "to"));

	for(child = packet->child; child; child = child->next) {
		if(child->type != XMLNODE_TYPE_TAG)
			continue;

		if(!strcmp(child->name, "subject")) {
			if(!jm->subject)
				jm->subject = xmlnode_get_data(child);
		} else if(!strcmp(child->name, "thread")) {
			if(!jm->thread_id)
				jm->thread_id = xmlnode_get_data(child);
		} else if(!strcmp(child->name, "body")) {
			if(!jm->body)
				jm->body = xmlnode_to_str(child, NULL);
		} else if(!strcmp(child->name, "html")) {
			if(!jm->xhtml)
				jm->xhtml = xmlnode_to_str(child, NULL);
		} else if(!strcmp(child->name, "error")) {
			const char *code = xmlnode_get_attrib(child, "code");
			char *code_txt = NULL;
			char *text = xmlnode_get_data(child);

			if(code)
				code_txt = g_strdup_printf(_(" (Code %s)"), code);

			if(!jm->error)
				jm->error = g_strdup_printf("%s%s", text ? text : "",
						code_txt ? code_txt : "");

			g_free(code_txt);
			g_free(text);
		} else if(!strcmp(child->name, "x")) {
			const char *xmlns = xmlnode_get_attrib(child, "xmlns");
			if(xmlns && !strcmp(xmlns, "jabber:x:event")) {
				if(xmlnode_get_child(child, "composing"))
					jm->events |= JABBER_MESSAGE_EVENT_COMPOSING;
			} else if(xmlns && !strcmp(xmlns, "jabber:x:delay")) {
				const char *timestamp = xmlnode_get_attrib(child, "stamp");
				if(timestamp)
					jm->sent = gaim_str_to_time(timestamp, TRUE);
			} else if(xmlns && !strcmp(xmlns, "jabber:x:conference") &&
					jm->type != JABBER_MESSAGE_GROUPCHAT_INVITE) {
				const char *jid = xmlnode_get_attrib(child, "jid");
				if(jid) {
					jm->type = JABBER_MESSAGE_GROUPCHAT_INVITE;
					g_free(jm->to);
					jm->to = g_strdup(jid);
				}
			} else if(xmlns && !strcmp(xmlns,
						"http://jabber.org/protocol/muc#user")) {
				xmlnode *invite = xmlnode_get_child(child, "invite");
				if(invite) {
					xmlnode *reason, *password;
					const char *jid = xmlnode_get_attrib(invite, "from");
					g_free(jm->to);
					jm->to = jm->from;
					jm->from = g_strdup(jid);
					if((reason = xmlnode_get_child(invite, "reason"))) {
						g_free(jm->body);
						jm->body = xmlnode_get_data(reason);
					}
					if((password = xmlnode_get_child(invite, "password")))
						jm->password = xmlnode_get_data(password);

					jm->type = JABBER_MESSAGE_GROUPCHAT_INVITE;
				}
			} else {
				jm->etc = g_list_append(jm->etc, child);
			}
		}
	}

	switch(jm->type) {
		case JABBER_MESSAGE_NORMAL:
		case JABBER_MESSAGE_CHAT:
			handle_chat(jm);
			break;
		case JABBER_MESSAGE_HEADLINE:
			handle_headline(jm);
			break;
		case JABBER_MESSAGE_GROUPCHAT:
			handle_groupchat(jm);
			break;
		case JABBER_MESSAGE_GROUPCHAT_INVITE:
			handle_groupchat_invite(jm);
			break;
		case JABBER_MESSAGE_ERROR:
			handle_error(jm);
			break;
		case JABBER_MESSAGE_OTHER:
			gaim_debug(GAIM_DEBUG_INFO, "jabber",
					"Received message of unknown type: %s\n", type);
			break;
	}
	jabber_message_free(jm);
}

void jabber_message_send(JabberMessage *jm)
{
	xmlnode *message, *child;
	const char *type = NULL;

	message = xmlnode_new("message");

	switch(jm->type) {
		case JABBER_MESSAGE_NORMAL:
			type = "normal";
			break;
		case JABBER_MESSAGE_CHAT:
		case JABBER_MESSAGE_GROUPCHAT_INVITE:
			type = "chat";
			break;
		case JABBER_MESSAGE_HEADLINE:
			type = "headline";
			break;
		case JABBER_MESSAGE_GROUPCHAT:
			type = "groupchat";
			break;
		case JABBER_MESSAGE_ERROR:
			type = "error";
			break;
		case JABBER_MESSAGE_OTHER:
			type = NULL;
			break;
	}

	if(type)
		xmlnode_set_attrib(message, "type", type);

	xmlnode_set_attrib(message, "to", jm->to);

	if(jm->thread_id) {
		child = xmlnode_new_child(message, "thread");
		xmlnode_insert_data(child, jm->thread_id, -1);
	}

	if(jm->events || (!jm->body && !jm->xhtml && !jm->subject)) {
		child = xmlnode_new_child(message, "x");
		xmlnode_set_attrib(child, "xmlns", "jabber:x:event");
		if(jm->events & JABBER_MESSAGE_EVENT_COMPOSING)
			xmlnode_new_child(child, "composing");
	}

	if(jm->subject) {
		child = xmlnode_new_child(message, "subject");
		xmlnode_insert_data(child, jm->subject, -1);
	}

	if(jm->body) {
		child = xmlnode_new_child(message, "body");
		xmlnode_insert_data(child, jm->body, -1);
	}

	if(jm->xhtml) {
		child = xmlnode_from_str(jm->xhtml, -1);
		if(child) {
			xmlnode_insert_child(message, child);
		} else {
			gaim_debug(GAIM_DEBUG_ERROR, "jabber",
					"XHTML translation/validation failed, returning: %s\n",
					jm->xhtml);
		}
	}

	jabber_send(jm->js, message);

	xmlnode_free(message);
}

int jabber_message_send_im(GaimConnection *gc, const char *who, const char *msg,
		GaimConvImFlags flags)
{
	JabberMessage *jm;
	JabberBuddy *jb;
	JabberBuddyResource *jbr;
	char *buf;
	char *xhtml;
	char *resource;

	if(!who || !msg)
		return 0;

	resource = jabber_get_resource(who);

	jb = jabber_buddy_find(gc->proto_data, who, TRUE);
	jbr = jabber_buddy_find_resource(jb, resource);

	g_free(resource);

	jm = g_new0(JabberMessage, 1);
	jm->js = gc->proto_data;
	jm->type = JABBER_MESSAGE_CHAT;
	jm->events = JABBER_MESSAGE_EVENT_COMPOSING;
	jm->to = g_strdup(who);
	if(jbr && jbr->thread_id)
		jm->thread_id = jbr->thread_id;

	buf = g_strdup_printf("<html xmlns='http://jabber.org/protocol/xhtml-im'><body xmlns='http://www.w3.org/1999/xhtml'>%s</body></html>", msg);

	gaim_markup_html_to_xhtml(buf, &xhtml, &jm->body);
	g_free(buf);

	if(!jbr || jbr->capabilities & JABBER_CAP_XHTML)
		jm->xhtml = xhtml;
	else
		g_free(xhtml);

	jabber_message_send(jm);
	jabber_message_free(jm);
	return 1;
}

int jabber_message_send_chat(GaimConnection *gc, int id, const char *msg)
{
	JabberChat *chat;
	JabberMessage *jm;
	JabberStream *js;
	char *buf, *body, *xhtml;

	if(!msg || !gc)
		return 0;

	js = gc->proto_data;
	chat = jabber_chat_find_by_id(js, id);

	if(!chat)
		return 0;

	buf = g_strdup_printf("<html xmlns='http://jabber.org/protocol/xhtml-im'><body xmlns='http://www.w3.org/1999/xhtml'>%s</body></html>", msg);
	gaim_markup_html_to_xhtml(buf, &xhtml, &body);
	g_free(buf);

	if(!strcmp(body, "/configure") || !strcmp(body, "/config")) {
		jabber_chat_request_room_configure(chat);
	} else if(!strcmp(body, "/register")) {
		jabber_chat_register(chat);
	} else if(!strncmp(body, "/topic", 6)) {
		jabber_chat_change_topic(chat, strlen(body) > 7 ? body+7 : NULL);
	} else if(!strncmp(body, "/nick", 5)) {
		if(strlen(body) > 6)
			jabber_chat_change_nick(chat, body+6);
	} else if(!strncmp(body, "/part", 5)) {
		jabber_chat_part(chat, strlen(body) > 6 ? body+6 : NULL);
	} else {
		jm = g_new0(JabberMessage, 1);
		jm->js = gc->proto_data;
		jm->type = JABBER_MESSAGE_GROUPCHAT;
		jm->to = g_strdup_printf("%s@%s", chat->room, chat->server);


		if(chat->xhtml)
			jm->xhtml = xhtml;
		else
			g_free(xhtml);

		jm->body = body;

		jabber_message_send(jm);
		jabber_message_free(jm);
		return 1;
	}

	g_free(body);
	g_free(xhtml);
	return 1;
}

int jabber_send_typing(GaimConnection *gc, const char *who, int typing)
{
	JabberMessage *jm;
	JabberBuddy *jb;
	JabberBuddyResource *jbr;
	char *resource = jabber_get_resource(who);

	jb = jabber_buddy_find(gc->proto_data, who, TRUE);
	jbr = jabber_buddy_find_resource(jb, resource);

	g_free(resource);

	if(!jbr || !(jbr->capabilities & JABBER_CAP_COMPOSING))
		return 0;

	jm = g_new0(JabberMessage, 1);
	jm->js = gc->proto_data;
	jm->type = JABBER_MESSAGE_CHAT;
	jm->to = g_strdup(who);

	if(typing == GAIM_TYPING)
		jm->events = JABBER_MESSAGE_EVENT_COMPOSING;

	jabber_message_send(jm);
	jabber_message_free(jm);

	return JABBER_TYPING_NOTIFY_INT;
}

