/**
 * XMPP - libpurple transport
 *
 * Copyright (C) 2009, Jan Kaluza <hanzz@soc.pidgin.im>
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
 */

#include "user.h"
#include "main.h"
#include "log.h"
#include "protocols/abstractprotocol.h"
#include "usermanager.h"
#include "gloox/chatstate.h"
#include "muchandler.h"
#include "cmds.h"
#include "parser.h"
#include "proxy.h"

/*
 * Called when contact list has been received from legacy network.
 */
static gboolean sync_cb(gpointer data)
{
	User *d;
	d = (User*) data;
	return d->syncCallback();
}
static void sendXhtmlTag(Tag *tag, Tag *stanzaTag) {
	Tag *html = new Tag("html");
	html->addAttribute("xmlns", "http://jabber.org/protocol/xhtml-im");
	Tag *body = new Tag("body");
	body->addAttribute("xmlns", "http://www.w3.org/1999/xhtml");
	body->addChild(tag->clone());
	html->addChild(body);
	stanzaTag->addChild(html);
	GlooxMessageHandler::instance()->j->send(stanzaTag);
}

User::User(GlooxMessageHandler *parent, const std::string &jid, const std::string &username, const std::string &password){
	p = parent;
	m_jid = jid;
	m_username = username;
	m_password = password;
	m_connected = false;
	m_roster = p->sql()->getRosterByJid(m_jid);
	m_vip = p->sql()->isVIP(m_jid);
	m_settings = p->sql()->getSettings(m_jid);
	m_syncTimer = 0;
	m_readyForConnect = false;
	m_rosterXCalled = false;
	m_account = NULL;
	m_connectionStart = time(NULL);
	m_subscribeLastCount = -1;
	m_reconnectCount = 0;
	m_lang = NULL;
	this->features = 6; // TODO: I can't be hardcoded
	m_mucs = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
	PurpleValue *value;
	
	// check default settings
	if ( (value = getSetting("enable_transport")) == NULL ) {
		p->sql()->addSetting(m_jid, "enable_transport", "1", PURPLE_TYPE_BOOLEAN);
		value = purple_value_new(PURPLE_TYPE_BOOLEAN);
		purple_value_set_boolean(value, true);
		g_hash_table_replace(m_settings, g_strdup("enable_transport"), value);
	}
	if ( (value = getSetting("enable_notify_email")) == NULL ) {
		p->sql()->addSetting(m_jid, "enable_notify_email", "0", PURPLE_TYPE_BOOLEAN);
		value = purple_value_new(PURPLE_TYPE_BOOLEAN);
		purple_value_set_boolean(value, false);
		g_hash_table_replace(m_settings, g_strdup("enable_notify_email"), value);
	}
	if ( (value = getSetting("enable_avatars")) == NULL ) {
		p->sql()->addSetting(m_jid, "enable_avatars", "1", PURPLE_TYPE_BOOLEAN);
		value = purple_value_new(PURPLE_TYPE_BOOLEAN);
		purple_value_set_boolean(value, true);
		g_hash_table_replace(m_settings, g_strdup("enable_avatars"), value);
	}
	if ( (value = getSetting("enable_chatstate")) == NULL ) {
		p->sql()->addSetting(m_jid, "enable_chatstate", "1", PURPLE_TYPE_BOOLEAN);
		value = purple_value_new(PURPLE_TYPE_BOOLEAN);
		purple_value_set_boolean(value, true);
		g_hash_table_replace(m_settings, g_strdup("enable_chatstate"), value);
	}
}

bool User::syncCallback() {
	Log().Get(jid()) << "sync_cb lastCount: " << m_subscribeLastCount << "cacheSize: " << int(m_subscribeCache.size());
	// we have to check, if all users arrived and repeat this call if not
	if (m_subscribeLastCount == int(m_subscribeCache.size())) {
		syncContacts();
		sendRosterX();
		return FALSE;
	}
	else{
		m_subscribeLastCount = int(m_subscribeCache.size());
		return TRUE;
	}
}

/*
 * Returns true if main JID for this User has feature `feature`,
 * otherwise returns false.
 */
bool User::hasFeature(int feature){
	if (m_capsVersion.empty())
		return false;
	if (p->capsCache[m_capsVersion]&feature)
		return true;
	return false;
}

/*
 * Returns true if transport has feature `feature` for this user,
 * otherwise returns false.
 */
bool User::hasTransportFeature(int feature){
	if (this->features&feature)
		return true;
	return false;
}

/*
 * Returns true if user with UIN `name` and subscription `subscription` is
 * in roster. If subscription.empty(), returns true on any subsciption.
 */
bool User::isInRoster(const std::string &name,const std::string &subscription){
	std::map<std::string,RosterRow>::iterator iter = m_roster.begin();
	iter = m_roster.find(name);
	if(iter != m_roster.end()){
		if (subscription.empty())
			return true;
		if (m_roster[name].subscription==subscription)
			return true;
	}
	return false;
}

/*
 * Returns true if exists auth request from user with uin `name`.
 */
bool User::hasAuthRequest(const std::string &name){
	std::map<std::string,authRequest>::iterator iter = m_authRequests.begin();
	iter = m_authRequests.find(name);
	if(iter != m_authRequests.end())
		return true;
	return false;
}

void User::removeAuthRequest(const std::string &name) {
	m_authRequests.erase(name);
}

/*
 * Returns true if exists opened conversation with user with uin `name`.
 */
bool User::isOpenedConversation(const std::string &name){
	std::map<std::string,PurpleConversation *>::iterator iter = m_conversations.begin();
	iter = m_conversations.find(name);
	if(iter != m_conversations.end())
		return true;
	return false;
}

/*
 * Sends all legacy network users which are recorded in subscribeCache
 * to this jabber user by roster item exchange.
 */
void User::sendRosterX()
{
	m_rosterXCalled = true;
	m_syncTimer = 0;
	if (int(m_subscribeCache.size())==0)
		return;
	Log().Get(m_jid) << "Sending rosterX";
	Tag *tag = new Tag("iq");
	tag->addAttribute( "to", m_jid +"/"+m_resource);
	tag->addAttribute( "type", "set");
	tag->addAttribute( "id",p->j->getID());
	std::string from;
	from.append(p->jid());
	tag->addAttribute( "from", from );
	Tag *x = new Tag("x");
	x->addAttribute("xmlns","http://jabber.org/protocol/rosterx");
	Tag *item;

	// adding these users to roster db with subscription=ask
	std::map<std::string,PurpleBuddy*>::iterator it = m_subscribeCache.begin();
	while(it != m_subscribeCache.end()) {
		PurpleBuddy *buddy = (*it).second;
		std::string name(purple_buddy_get_name(buddy));
		std::for_each( name.begin(), name.end(), replaceBadJidCharacters() );
		if (!name.empty()){
			RosterRow user;
			std::string alias;                                                                                                                                          
			if (purple_buddy_get_server_alias(buddy))                                                                                                                   
				alias = (std::string) purple_buddy_get_server_alias(buddy);
			else
				alias = (std::string) purple_buddy_get_alias(buddy);
			user.id = -1;
			user.jid = m_jid;
			user.uin = name;
			user.subscription = "ask";
			user.online = false;
			user.lastPresence = "";
			m_roster[name] = user;
			p->sql()->addUserToRoster(m_jid,name,"ask");

			item = new Tag("item");
			item->addAttribute("action","add");
			item->addAttribute("jid",name+"@"+p->jid());
			item->addAttribute("name",alias);
			item->addChild(new Tag("group",(std::string) purple_group_get_name(purple_buddy_get_group(buddy))));
			x->addChild(item);
		}
		it++;
	}
	tag->addChild(x);
	std::cout << tag->xml() << "\n";
	p->j->send(tag);
	
	m_subscribeCache.clear();
	m_subscribeLastCount = -1;
}

/*
 * Adds contacts which are not in legacy network, but which are in local jabber roster,
 * to the legacy network.
 */
void User::syncContacts()
{
	PurpleBuddy *buddy;
	Log().Get(m_jid) << "Syncing contacts with legacy network.";
	for(std::map<std::string, RosterRow>::iterator u = m_roster.begin(); u != m_roster.end() ; u++){
		std::string name((*u).second.uin);
		std::for_each( name.begin(), name.end(), replaceJidCharacters() );
		buddy = purple_find_buddy(m_account, name.c_str());
		// buddy is not in blist, so it's not on server
		if (!buddy) {
			// add buddy to server
			buddy = purple_buddy_new(m_account,(*u).second.uin.c_str(),(*u).second.uin.c_str());
			purple_blist_add_buddy(buddy, NULL, NULL ,NULL);
			purple_account_add_buddy(m_account, buddy);
		}
	}
}

/*
 * Generates presence stanza for PurpleBuddy `buddy`. This will
 * create whole <presence> stanza without 'to' attribute.
 */
Tag *User::generatePresenceStanza(PurpleBuddy *buddy){
	if (buddy==NULL)
		return NULL;
	std::string alias;
	if (purple_buddy_get_server_alias(buddy))
		alias = (std::string) purple_buddy_get_server_alias(buddy);
	else
		alias = (std::string) purple_buddy_get_alias(buddy);

	std::string name(purple_buddy_get_name(buddy));
	std::for_each( name.begin(), name.end(), replaceBadJidCharacters() );
	PurplePresence *pres = purple_buddy_get_presence(buddy);
	if (pres==NULL)
		return NULL;
	PurpleStatus *stat = purple_presence_get_active_status(pres);
	if (stat==NULL)
		return NULL;
	int s = purple_status_type_get_primitive(purple_status_get_type(stat));
	const char *statusMessage = purple_status_get_attr_string(stat, "message");

	Log().Get(m_jid) << "Generating presence stanza for user " << name;
	Tag *tag = new Tag("presence");
	std::string from;
	from.append(name);
	from.append("@");
	from.append(p->jid()+"/bot");
	tag->addAttribute( "from", from );

	if (statusMessage!=NULL){
		std::string _status(statusMessage);
		Log().Get(m_jid) << "Raw status message: " << _status;
		tag->addChild( new Tag("status", stripHTMLTags(_status)));
	}
	
	switch(s) {
		case PURPLE_STATUS_AVAILABLE: {
			break;
		}
		case PURPLE_STATUS_AWAY: {
			tag->addChild( new Tag("show", "away" ) );
			break;
		}
		case PURPLE_STATUS_UNAVAILABLE: {
			tag->addChild( new Tag("show", "dnd" ) );
			break;
		}
		case PURPLE_STATUS_EXTENDED_AWAY: {
			tag->addChild( new Tag("show", "xa" ) );
			break;
		}
		case PURPLE_STATUS_OFFLINE: {
			tag->addAttribute( "type", "unavailable" );
			break;
		}
	}
	
	// caps
	Tag *c = new Tag("c");
	c->addAttribute("xmlns","http://jabber.org/protocol/caps");
	c->addAttribute("hash","sha-1");
	c->addAttribute("node","http://jabbim.cz/icqtransport");
	c->addAttribute("ver","Q543534fdsfsdsssT/WM94uAlu0=");
	tag->addChild(c);
	
	// vcard-temp:x:update
	char *avatarHash = NULL;
	PurpleBuddyIcon *icon = purple_buddy_icons_find(m_account, name.c_str());
	if (icon != NULL) {
		avatarHash = purple_buddy_icon_get_full_path(icon);
		Log().Get(m_jid) << "avatarHash";
	}

	if (purple_value_get_boolean(getSetting("enable_avatars"))){
		Tag *x = new Tag("x");
		x->addAttribute("xmlns","vcard-temp:x:update");
		if (avatarHash != NULL) {
			Log().Get(m_jid) << "Got avatar hash";
			// Check if it's patched libpurple which saved icons to directories
			char *hash = rindex(avatarHash,'/');
			std::string h;
			if (hash) {
				char *dot;
				hash++;
				dot = strchr(hash, '.');
				if (dot)
					*dot = '\0';
				x->addChild(new Tag("photo",(std::string) hash));
			}
			else
				x->addChild(new Tag("photo",(std::string) avatarHash));
		}
		else{
			Log().Get(m_jid) << "no avatar hash";
			x->addChild(new Tag("photo"));
		}
		tag->addChild(x);
	}

	// update stats...
	if (s==PURPLE_STATUS_OFFLINE){
		if (m_roster[name].online) {
			m_roster[name].online = false;
			p->userManager()->buddyOffline();
		}
	}
	else
		if (!m_roster[name].online) {
			m_roster[name].online = true;
			p->userManager()->buddyOnline();
		}

	return tag;
}

/*
 * Re-requests authorization for buddy if we can do it (if buddy is not authorized).
 */
void User::purpleReauthorizeBuddy(PurpleBuddy *buddy){
	if (!m_connected)
		return;
	if (!buddy)
		return;
	if (!m_account)
		return;
	GList *l, *ll;
	std::string name(purple_buddy_get_name(buddy));
	std::for_each( name.begin(), name.end(), replaceBadJidCharacters() );	
	if (purple_account_get_connection(m_account)){
		PurplePluginProtocolInfo *prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(purple_account_get_connection(m_account)->prpl);
		if(prpl_info && prpl_info->blist_node_menu){
			for(l = ll = prpl_info->blist_node_menu((PurpleBlistNode*)buddy); l; l = l->next) {
				if (l->data) {
					PurpleMenuAction *act = (PurpleMenuAction *) l->data;
					if (act->label) {
						Log().Get(m_jid) << (std::string)act->label;
						if ((std::string)act->label == "Re-request Authorization"){
							Log().Get(m_jid) << "rerequesting authorization for " << name;
							((GSourceFunc) act->callback)(act->data);
							break;
						}
					}
				}
			}
			g_list_free(ll);
		}
	}
}

/*
 * Called when something related to this buddy changed...
 */
void User::purpleBuddyChanged(PurpleBuddy *buddy){
	if (buddy==NULL || m_connected == false)
		return;
	std::string alias;
	if (purple_buddy_get_server_alias(buddy))
		alias = (std::string) purple_buddy_get_server_alias(buddy);
	else
		alias = (std::string) purple_buddy_get_alias(buddy);

	std::string name(purple_buddy_get_name(buddy));
	std::for_each( name.begin(), name.end(), replaceBadJidCharacters() );
	PurplePresence *pres = purple_buddy_get_presence(buddy);
	if (pres==NULL)
		return;
	PurpleStatus *stat = purple_presence_get_active_status(pres);
	if (stat==NULL)
		return;
	int s = purple_status_type_get_primitive(purple_status_get_type(stat));
	// TODO: rewrite me to use prpl_info->status_text(buddy)
// 	const char *statusMessage = purple_status_get_attr_string(stat, "message");

	Log().Get(m_jid) << "purpleBuddyChanged: " << name << " ("<< alias <<") (" << s << ")";

	if (m_syncTimer==0 && !m_rosterXCalled){
		m_syncTimer = purple_timeout_add_seconds(4, sync_cb, this);
	}

// 	bool inRoster = purple_blist_node_get_bool(&buddy->node, "inRoster");
// 	if (!inRoster) {
	bool inRoster = isInRoster(name,"");
// 		purple_blist_node_set_bool(&buddy->node, "inRoster", true);
// 	}

	if (!inRoster) {
		if (!m_rosterXCalled && hasFeature(GLOOX_FEATURE_ROSTERX)){
// 			subscribeContact c;
// 			c.uin = name;
// 			c.alias = alias;
// 			c.group = (std::string) purple_group_get_name(purple_buddy_get_group(buddy));
			m_subscribeCache[name] = buddy;
			Log().Get(m_jid) << "Not in roster => adding to rosterX cache";
		}
		else {
			if (hasFeature(GLOOX_FEATURE_ROSTERX)) {
				Log().Get(m_jid) << "Not in roster => sending rosterx";
				if (m_syncTimer==0){
					m_syncTimer = purple_timeout_add_seconds(4, sync_cb, this);
				}
// 				subscribeContact c;
// 				c.uin = name;
// 				c.alias = alias;
// 				c.group = (std::string) purple_group_get_name(purple_buddy_get_group(buddy));
				m_subscribeCache[name] = buddy;
			}
			else {
				Log().Get(m_jid) << "Not in roster => sending subscribe";
				Tag *tag = new Tag("presence");
				tag->addAttribute("type", "subscribe" );
				tag->addAttribute("from", name + "@" + p->jid());
				tag->addAttribute("to", m_jid);
				Tag *nick = new Tag("nick", alias);
				nick->addAttribute("xmlns","http://jabber.org/protocol/nick");
				tag->addChild(nick);
				p->j->send(tag);
			}
		}
	}
	else {
		Tag *tag = generatePresenceStanza(buddy);
		if (tag){
			if (tag->xml()==m_roster[name].lastPresence){
				Log().Get(m_jid) << "Not sending this presence, because we've already sent it before.";
				delete tag;
				return;
			}
			else{
				m_roster[name].lastPresence=tag->xml();
			}
			tag->addAttribute( "to", m_jid );
			p->j->send( tag );
		}
	}
}

/*
 * Called when PurpleBuddy is removed.
 */
void User::purpleBuddyRemoved(PurpleBuddy *buddy) {
	// we should remove pointer to buddy from subscribCache
	std::string name(purple_buddy_get_name(buddy));
	std::for_each( name.begin(), name.end(), replaceBadJidCharacters() );
	m_subscribeCache.erase(name);
}

/*
 * Called when new message has been received.
 */
void User::purpleMessageReceived(PurpleAccount* account,char * name,char *msg,PurpleConversation *conv,PurpleMessageFlags flags){
	// new message grom legacy network has been received
	if (conv==NULL){
		// make conversation if it doesn't exist
		conv = purple_conversation_new(PURPLE_CONV_TYPE_IM,account,name);
		m_conversations[(std::string)name]=conv;
	}
}

void User::purpleConversationWriteChat(PurpleConversation *conv, const char *who, const char *msg, PurpleMessageFlags flags, time_t mtime) {
	if (who == NULL)
		return;

	std::string name(who);
	MUCHandler *muc = (MUCHandler*) g_hash_table_lookup(m_mucs, purple_conversation_get_name(conv));
	if (!isOpenedConversation(name)) {
		m_conversations[name] = conv;
		if (muc)
			muc->setConversation(conv);
	}

	
	if (muc) {
		muc->messageReceived(who, msg, flags, mtime);
	}
}

void User::purpleConversationWriteIM(PurpleConversation *conv, const char *who, const char *msg, PurpleMessageFlags flags, time_t mtime) {
	if (who == NULL)
		return;
	
	if (flags & PURPLE_MESSAGE_SEND)
		return;
	
	std::string name = (std::string) purple_conversation_get_name(conv);
	
	int pos = name.find("/");
	name.erase(pos, name.length() - pos);
	
	if (name.empty())
		return;
	std::for_each( name.begin(), name.end(), replaceBadJidCharacters() );
	// new message from legacy network has been received
	if (!isOpenedConversation(name)) {
			m_conversations[name] = conv;
	}

	Log().Get(m_jid) <<  "purpleConversationWriteIM:" << name << msg;

	// send message to user
	char *newline = purple_strdup_withhtml(msg);
	char *strip = purple_markup_strip_html(newline);

	std::string message(strip);
	Message s(Message::Chat, m_jid, message);
	std::string from;
	from.append(name);
	from.append("@");
	from.append(p->jid() + "/bot");
	s.setFrom(from);

	// chatstates
	if (purple_value_get_boolean(getSetting("enable_chatstate"))) {
		if (hasFeature(GLOOX_FEATURE_CHATSTATES)) {
			ChatState *c = new ChatState(ChatStateActive);
			s.addExtension(c);
		}
	}
	
	// Delayed messages, we have to count with some delay
	if (int(time(NULL))-10>int(mtime)) {
		char buf[80];
		strftime(buf, sizeof(buf), "%Y%m%dT%H:%M:%S", localtime(&mtime));
		std::string timestamp(buf);
		DelayedDelivery *d = new DelayedDelivery(from, timestamp);
		s.addExtension(d);
	}

	Tag *stanzaTag = s.tag();

	std::string m(msg);
	if (m.find("<body>") == 0) {
		m.erase(0,6);
		m.erase(m.length() - 7, 7);
	}
	Log().Get("TEST") << m << " " << message;
	if (hasFeature(GLOOX_FEATURE_XHTML_IM) && m != message) {
		p->parser()->getTag(m, sendXhtmlTag, stanzaTag);
		g_free(newline);
		g_free(strip);
		return;
	}

	Log().Get("STANZATAG") << stanzaTag->xml();

	p->j->send( stanzaTag );
	g_free(newline);
	g_free(strip);
}

void User::purpleChatTopicChanged(PurpleConversation *conv, const char *who, const char *topic) {
	MUCHandler *muc = (MUCHandler*) g_hash_table_lookup(m_mucs, purple_conversation_get_name(conv));
	if (muc) {
		muc->topicChanged(who, topic);
	}
}

void User::purpleChatAddUsers(PurpleConversation *conv, GList *cbuddies, gboolean new_arrivals) {
	std::string name(purple_conversation_get_name(conv));
	MUCHandler *muc = (MUCHandler*) g_hash_table_lookup(m_mucs, name.c_str());
	if (!muc)
		return;
	if (!isOpenedConversation(name)) {
		m_conversations[name] = conv;
		muc->setConversation(conv);
	}
	muc->addUsers(cbuddies);
}

void User::purpleChatRenameUser(PurpleConversation *conv, const char *old_name, const char *new_name, const char *new_alias) {
	MUCHandler *muc = (MUCHandler*) g_hash_table_lookup(m_mucs, purple_conversation_get_name(conv));
	if (muc) {
		muc->renameUser(old_name, new_name, new_alias);
	}
}

void User::purpleChatRemoveUsers(PurpleConversation *conv, GList *users) {
	MUCHandler *muc = (MUCHandler*) g_hash_table_lookup(m_mucs, purple_conversation_get_name(conv));
	if (muc) {
		muc->removeUsers(users);
	}
}

PurpleValue * User::getSetting(const char *key) {
	PurpleValue *value = (PurpleValue *) g_hash_table_lookup(m_settings, key);
	return value;
}

void User::updateSetting(const std::string &key, PurpleValue *value) {
	if (purple_value_get_type(value) == PURPLE_TYPE_BOOLEAN) {
		if (purple_value_get_boolean(value))
			p->sql()->updateSetting(m_jid, key, "1");
		else
			p->sql()->updateSetting(m_jid, key, "0");
	}
	g_hash_table_replace(m_settings, g_strdup(key.c_str()), value);
}

/*
 * Called when legacy network user stops typing.
 */
void User::purpleBuddyTypingStopped(const std::string &uin){
	if (!hasFeature(GLOOX_FEATURE_CHATSTATES))
		return;
	if (!purple_value_get_boolean(getSetting("enable_chatstate")))
		return;
	Log().Get(m_jid) << uin << " stopped typing";
	std::string username(uin);
	std::for_each( username.begin(), username.end(), replaceBadJidCharacters() );
	

	Tag *s = new Tag("message");
	s->addAttribute("to",m_jid);
	s->addAttribute("type","chat");
	
	std::string from;
	from.append(username);
	from.append("@");
	from.append(p->jid()+"/bot");
	s->addAttribute("from",from);
	
	// chatstates
	Tag *active = new Tag("paused");
	active->addAttribute("xmlns","http://jabber.org/protocol/chatstates");
	s->addChild(active);
	
	p->j->send( s );
}

/*
 * Called when legacy network user starts typing.
 */
void User::purpleBuddyTyping(const std::string &uin){
	if (!hasFeature(GLOOX_FEATURE_CHATSTATES))
		return;
	if (!purple_value_get_boolean(getSetting("enable_chatstate")))
		return;
	Log().Get(m_jid) << uin << " is typing";
	std::string username(uin);
	std::for_each( username.begin(), username.end(), replaceBadJidCharacters() );

	Tag *s = new Tag("message");
	s->addAttribute("to",m_jid);
	s->addAttribute("type","chat");
	
	std::string from;
	from.append(username);
	from.append("@");
	from.append(p->jid()+"/bot");
	s->addAttribute("from",from);
	
	// chatstates
	Tag *active = new Tag("composing");
	active->addAttribute("xmlns","http://jabber.org/protocol/chatstates");
	s->addChild(active);
	
	p->j->send( s );
}

/*
 * Received Chatstate notification from jabber user :).
 */
void User::receivedChatState(const std::string &uin,const std::string &state){
	if (!hasFeature(GLOOX_FEATURE_CHATSTATES))
		return;
	Log().Get(m_jid) << "Sending " << state << " message to " << uin;
	if (state=="composing")
		serv_send_typing(purple_account_get_connection(m_account),uin.c_str(),PURPLE_TYPING);
	else if (state=="paused")
		serv_send_typing(purple_account_get_connection(m_account),uin.c_str(),PURPLE_TYPED);
	else
		serv_send_typing(purple_account_get_connection(m_account),uin.c_str(),PURPLE_NOT_TYPING);

}

/*
 * Received new message from jabber user.
 */
void User::receivedMessage(const Message& msg){
	PurpleConversation * conv;
	std::string username = msg.to().username();
	std::for_each( username.begin(), username.end(), replaceJidCharacters() );
	// open new conversation or get the opened one
	if (!isOpenedConversation(username)){
		conv = purple_conversation_new(PURPLE_CONV_TYPE_IM,m_account,username.c_str());
		m_conversations[username]=conv;
	}
	else{
		conv = m_conversations[username];
	}
	
	std::string body = msg.body();
	
	if (body.find("/transport ") == 0) {
		PurpleCmdStatus status;
		char *error = NULL;
		body.erase(0,11);
		status = purple_cmd_do_command(conv, body.c_str(), body.c_str(), &error);

		switch (status) {
			case PURPLE_CMD_STATUS_OK:
				break;
			case PURPLE_CMD_STATUS_NOT_FOUND:
				{
					purple_conversation_write(conv, "transport", tr(getLang(),_("Transport: Unknown command.")), PURPLE_MESSAGE_RECV, time(NULL));
					break;
				}
			case PURPLE_CMD_STATUS_WRONG_ARGS:
				purple_conversation_write(conv, "transport", tr(getLang(),_("Syntax Error:  You typed the wrong number of arguments to that command.")), PURPLE_MESSAGE_RECV, time(NULL));
				break;
			case PURPLE_CMD_STATUS_FAILED:
				purple_conversation_write(conv, "transport", tr(getLang(),error ? error : _("Your command failed for an unknown reason.")), PURPLE_MESSAGE_RECV, time(NULL));
				break;
			case PURPLE_CMD_STATUS_WRONG_TYPE:
				if(purple_conversation_get_type(conv) == PURPLE_CONV_TYPE_IM)
					purple_conversation_write(conv, "transport", tr(getLang(),_("That command only works in Groupchats, not IMs.")), PURPLE_MESSAGE_RECV, time(NULL));
				else
					purple_conversation_write(conv, "transport", tr(getLang(),_("That command only works in IMs, not Groupchats.")), PURPLE_MESSAGE_RECV, time(NULL));
				break;
			case PURPLE_CMD_STATUS_WRONG_PRPL:
				purple_conversation_write(conv, "transport", tr(getLang(),_("That command doesn't work on this protocol.")), PURPLE_MESSAGE_RECV, time(NULL));
				break;
		}

		if (error)
			g_free(error);
		return;
	}
	
	// send this message
	if (purple_conversation_get_type(conv) == PURPLE_CONV_TYPE_IM) {
		PurpleConvIm *im = purple_conversation_get_im_data(conv);
		purple_conv_im_send(im,body.c_str());
	}
	else if (purple_conversation_get_type(conv) == PURPLE_CONV_TYPE_CHAT) {
		purple_conv_chat_send(PURPLE_CONV_CHAT(conv), body.c_str());
	}
}

/*
 * Somebody wants to authorize us from the legacy network.
 */
void User::purpleAuthorizeReceived(PurpleAccount *account,const char *remote_user,const char *id,const char *alias,const char *message,gboolean on_list,PurpleAccountRequestAuthorizationCb authorize_cb,PurpleAccountRequestAuthorizationCb deny_cb,void *user_data){
	authRequest req;
	req.authorize_cb = authorize_cb;
	req.deny_cb = deny_cb;
	req.user_data = user_data;
	m_authRequests[std::string(remote_user)] = req;
	Log().Get(m_jid) <<  "purpleAuthorizeReceived: " << std::string(remote_user) << "on_list:" << on_list;
	// send subscribe presence to user
	Tag *tag = new Tag("presence");
	tag->addAttribute("type", "subscribe" );
	tag->addAttribute("from", (std::string)remote_user + "@" + p->jid());
	tag->addAttribute("to", m_jid);

	if (alias) {
		Tag *nick = new Tag("nick", (std::string) alias);
		nick->addAttribute("xmlns","http://jabber.org/protocol/nick");
		tag->addChild(nick);
	}

	std::cout << tag->xml() << "\n";
	p->j->send(tag);

}

/*
 * Called when we're ready to connect (we know caps)
 */
void User::connect(){
	if (!m_readyForConnect) {
		Log().Get(m_jid) << "We are not ready for connect";
		return;
	}
	if (m_account) {
		Log().Get(m_jid) << "connect() has been called before";
		return;
	}
	Log().Get(m_jid) << "Connecting with caps: " << m_capsVersion;
	if (purple_accounts_find(m_username.c_str(), this->p->protocol()->protocol().c_str()) != NULL){
		Log().Get(m_jid) << "this account already exists";
		m_account = purple_accounts_find(m_username.c_str(), this->p->protocol()->protocol().c_str());
		purple_account_set_ui_bool(m_account,"hiicq","auto-login",false);
	}
	else{
		Log().Get(m_jid) << "creating new account";
		m_account = purple_account_new(m_username.c_str(), this->p->protocol()->protocol().c_str());
		purple_account_set_string(m_account,"encoding","windows-1250");
		purple_account_set_ui_bool(m_account,"hiicq","auto-login",false);

		purple_accounts_add(m_account);
	}
	m_connectionStart = time(NULL);
	m_readyForConnect = false;
	purple_account_set_string(m_account,"bind",std::string(m_bindIP).c_str());
	purple_account_set_string(m_account,"lastUsedJid",std::string(m_jid +"/"+m_resource).c_str());
	purple_account_set_password(m_account,m_password.c_str());
	Log().Get(m_jid) << "UIN:" << m_username << " PASSWORD:" << m_password;

	if (p->configuration->useProxy) {
		PurpleProxyInfo *info = purple_proxy_info_new();
		purple_proxy_info_set_type(info, PURPLE_PROXY_USE_ENVVAR);
		info->username = NULL;
		info->password = NULL;
		purple_account_set_proxy_info(m_account, info);
	}


	// check if it's valid uin
	bool valid = false;
	if (!m_username.empty()){
		valid = p->protocol()->isValidUsername(m_username);
	}
	if (valid && purple_value_get_boolean(getSetting("enable_transport"))){
		purple_account_set_enabled(m_account, HIICQ_UI, TRUE);
		purple_account_connect(m_account);
	}
}

/*
 * called when we are disconnected from legacy network
 */
void User::disconnected() {
	m_connected = false;
	m_reconnectCount += 1;
}

/*
 * called when we are disconnected from legacy network
 */
void User::connected() {
	m_connected = true;
	m_reconnectCount = 0;
}

void User::receivedSubscription(const Subscription &subscription) {
	if (m_connected){
		if(subscription.subtype() == Subscription::Subscribed) {
			// we've already received auth request from legacy server, so we should respond to this request
			if (hasAuthRequest((std::string)subscription.to().username())){
				Log().Get(m_jid) <<  "subscribed presence for authRequest arrived => accepting the request";
				m_authRequests[(std::string)subscription.to().username()].authorize_cb(m_authRequests[(std::string)subscription.to().username()].user_data);
				m_authRequests.erase(subscription.to().username());
			}
			// subscribed user is not in roster, so we must add him/her there.
			std::string name(subscription.to().username());
			std::for_each( name.begin(), name.end(), replaceJidCharacters() );
			PurpleBuddy *buddy = purple_find_buddy(m_account, name.c_str());
			if(!isInRoster(subscription.to().username(),"both")) {
				if(!buddy) {
					Log().Get(m_jid) << "user is not in legacy network contact lists => nothing to be subscribed";
					// this user is not in ICQ contact list... It's nothing to do here,
					// because there is no user to authorize
				} else {
					Log().Get(m_jid) << "adding this user to local roster and sending presence";
					if (!isInRoster(subscription.to().username(),"ask")){
						// add user to mysql database and to local cache
						RosterRow user;
						user.id=-1;
						user.jid=m_jid;
						user.uin=subscription.to().username();
						user.subscription="both";
						user.online=false;
						user.lastPresence="";
						m_roster[subscription.to().username()]=user;
						p->sql()->addUserToRoster(m_jid,(std::string)subscription.to().username(),"both");
					}
					else{
						m_roster[subscription.to().username()].subscription="both";
						p->sql()->updateUserRosterSubscription(m_jid,(std::string)subscription.to().username(),"both");
					}
					// user is in ICQ contact list so we can inform jabber user
					// about status
					purpleBuddyChanged(buddy);
				}
			}
			// it can be reauthorization...
			if (buddy){
				purpleReauthorizeBuddy(buddy);
			}
			return;
		}
		else if(subscription.subtype() == Subscription::Subscribe) {
			std::string name(subscription.to().username());
			std::for_each( name.begin(), name.end(), replaceJidCharacters() );
			PurpleBuddy *b = purple_find_buddy(m_account, name.c_str());
			if (b){
				purpleReauthorizeBuddy(b);
			}
			if(isInRoster(subscription.to().username(),"")) {
				if (m_roster[subscription.to().username()].subscription=="ask") {
					Tag *reply = new Tag("presence");
					reply->addAttribute( "to", subscription.from().bare() );
					reply->addAttribute( "from", subscription.to().bare() );
					reply->addAttribute( "type", "subscribe" );
					p->j->send( reply );
				}
				Log().Get(m_jid) << "subscribe presence; user is already in roster => sending subscribed";
				// username is in local roster (he/her is in ICQ roster too),
				// so we should send subscribed presence
				Tag *reply = new Tag("presence");
				reply->addAttribute( "to", subscription.from().bare() );
				reply->addAttribute( "from", subscription.to().bare() );
				reply->addAttribute( "type", "subscribed" );
				p->j->send( reply );
			}
			else{
				Log().Get(m_jid) << "subscribe presence; user is not in roster => adding to legacy network";
				// this user is not in local roster, so we have to send add_buddy request
				// to our legacy network and wait for response
				PurpleBuddy *buddy = purple_buddy_new(m_account,subscription.to().username().c_str(),subscription.to().username().c_str());
				purple_blist_add_buddy(buddy, NULL, NULL ,NULL);
				purple_account_add_buddy(m_account, buddy);
			}
			return;
		} else if(subscription.subtype() == Subscription::Unsubscribe || subscription.subtype() == Subscription::Unsubscribed) {
			std::string name(subscription.to().username());
			std::for_each( name.begin(), name.end(), replaceJidCharacters() );
			PurpleBuddy *buddy = purple_find_buddy(m_account, name.c_str());
			if(subscription.subtype() == Subscription::Unsubscribed) {
				// user respond to auth request from legacy network and deny it
				if (hasAuthRequest((std::string)subscription.to().username())){
					Log().Get(m_jid) << "unsubscribed presence for authRequest arrived => rejecting the request";
					m_authRequests[(std::string)subscription.to().username()].deny_cb(m_authRequests[(std::string)subscription.to().username()].user_data);
					m_authRequests.erase(subscription.to().username());
					return;
				}
			}
			if(buddy && isInRoster(subscription.to().username(),"both")) {
				Log().Get(m_jid) << "unsubscribed presence => removing this contact from legacy network";
				// thi contact is in ICQ contact list, so we can remove him/her
				purple_account_remove_buddy(m_account,buddy,purple_buddy_get_group(buddy));
				purple_blist_remove_buddy(buddy);
// 				purple_privacy_permit_remove(m_account, subscription.to().username().c_str(),false);
			} else {
				// this contact is not in ICQ contact list, so there is nothing to remove...
			}
			if(isInRoster(subscription.to().username(),"both")) {
				// this contact is in our local roster, so we have to remove her/him
				Log().Get(m_jid) << "removing this contact from local roster";
				m_roster.erase(subscription.to().username());
				p->sql()->removeUINFromRoster(m_jid,subscription.to().username());
			}
			// inform user about removing this contact
			Tag *tag = new Tag("presence");
			tag->addAttribute( "to", subscription.from().bare() );
			tag->addAttribute( "from", subscription.to().username() + "@" + p->jid()+"/bot" );
			if(subscription.subtype() == Subscription::Unsubscribe) {
				tag->addAttribute( "type", "unsubscribe" );
			} else {
				tag->addAttribute( "type", "unsubscribed" );
			}
			p->j->send( tag );
			return;
		}
	}
}

/*
 * Received jabber presence...
 */
void User::receivedPresence(const Presence &stanza){
	// we're connected



	if (m_connected){

		if (stanza.to().username()!="") {
			MUCHandler *muc = (MUCHandler*) g_hash_table_lookup(m_mucs, stanza.to().username().c_str());
			if (muc) {
				Tag * ret = muc->handlePresence(stanza);
				if (ret)
					p->j->send(ret);
				if (stanza.presence() == Presence::Unavailable) {
					g_hash_table_remove(m_mucs, stanza.to().username().c_str());
					m_conversations.erase(stanza.to().username());
					delete muc;
				}
			}
			else if (p->protocol()->isMUC(this, stanza.to().bare()) && stanza.presence() != Presence::Unavailable) {
				MUCHandler *muc = new MUCHandler(this, stanza.to().bare(), stanza.from().full());
				g_hash_table_replace(m_mucs, g_strdup(stanza.to().username().c_str()), muc);
				Tag * ret = muc->handlePresence(stanza);
				if (ret)
					p->j->send(ret);
			}
		}

		// respond to probe presence
		if (stanza.subtype() == Presence::Probe && stanza.to().username()!=""){
			std::string name(stanza.to().username());
			std::for_each( name.begin(), name.end(), replaceJidCharacters() );
			PurpleBuddy *buddy = purple_find_buddy(m_account, name.c_str());
			if (buddy){
				Tag *probe = generatePresenceStanza(buddy);
				if (probe){
					probe->addAttribute( "to", stanza.from().full() );
					p->j->send(probe);
				}
			}
// 				purpleBuddyChanged(buddy);
			else{
				Log().Get(m_jid) << "answering to probe presence with unavailable presence";
				Tag *tag = new Tag("presence");
				tag->addAttribute("to",stanza.from().full());
				tag->addAttribute("from",stanza.to().bare()+"/bot");
				tag->addAttribute("type","unavailable");
				p->j->send(tag);
			}
		}
	}

	if (m_lang == NULL) {
		Tag *tag = stanza.tag();
		if (!tag)
			return;
		std::string lang = tag->findAttribute("xml:lang");
		Log().Get("LANG") << tag->xml();
		delete tag;
		if (lang == "")
			lang = "en";
		setLang(lang.c_str());
		localization.loadLocale(getLang());
		Log().Get("LANG") << lang << " " << lang.c_str();
	}

	// this presence is for the transport
	if(stanza.to().username() == ""){
		if(stanza.presence() == Presence::Unavailable) {
			// disconnect from legacy network if we are connected
			std::map<std::string,int> ::iterator iter = m_resources.begin();
			if ((m_connected==false && int(time(NULL))>int(m_connectionStart)+10) || m_connected==true){
				iter = m_resources.find(stanza.from().resource());
				if(iter != m_resources.end()){
					m_resources.erase(stanza.from().resource());
				}
			}
			if (m_connected){
				if (m_resources.empty()){
					Log().Get(m_jid) << "disconecting";
					purple_account_disconnect(m_account);
				}
				else{

					iter = m_resources.begin();
					m_resource=(*iter).first;
				}
// 				m_connected=false;
			}
			else {
				if (!m_resources.empty() && int(time(NULL))>int(m_connectionStart)+10){
					iter = m_resources.begin();
					m_resource=(*iter).first;
				}
				else if (m_account){
					Log().Get(m_jid) << "disconecting2";
					purple_account_disconnect(m_account);
				}
			}
		} else {
			m_resource=stanza.from().resource();
			std::map<std::string,int> ::iterator iter = m_resources.begin();
			iter = m_resources.find(m_resource);
			if(iter == m_resources.end()){
				m_resources[m_resource]=stanza.priority();
			}

			Log().Get(m_jid) << "resource: " << m_resource;
			if (!m_connected){
				// we are not connected to legacy network, so we should do it when disco#info arrive :)
				Log().Get(m_jid) << "connecting: capsVersion=" << m_capsVersion;
				if (m_readyForConnect==false){
					m_readyForConnect=true;
					if (m_capsVersion.empty()){
						// caps not arrived yet, so we can't connect just now and we have to wait for caps
					}
					else{
						connect();
					}
				}
			}
			else {
				Log().Get(m_jid) << "mirroring presence to legacy network";
				// we are already connected so we have to change status
				PurpleSavedStatus *status;
				int PurplePresenceType;
				std::string statusMessage;
				
				// mirror presence types
				switch(stanza.presence()) {
					case Presence::Available: {
						PurplePresenceType=PURPLE_STATUS_AVAILABLE;
						break;
					}
					case Presence::Chat: {
						PurplePresenceType=PURPLE_STATUS_AVAILABLE;
						break;
					}
					case Presence::Away: {
						PurplePresenceType=PURPLE_STATUS_AWAY;
						break;
					}
					case Presence::DND: {
						PurplePresenceType=PURPLE_STATUS_UNAVAILABLE;
						break;
					}
					case Presence::XA: {
						PurplePresenceType=PURPLE_STATUS_EXTENDED_AWAY;
						break;
					}
					default: break;
				}
				// send presence to our legacy network
				status = purple_savedstatus_new(NULL, (PurpleStatusPrimitive)PurplePresenceType);

				statusMessage.clear();

				if (hasTransportFeature(TRANSPORT_MANGLE_STATUS)) {
					p->sql()->getRandomStatus(statusMessage);
                    statusMessage.append(" - ");
				}

				statusMessage.append(stanza.status());

				if (!statusMessage.empty())
					purple_savedstatus_set_message(status,statusMessage.c_str());
				purple_savedstatus_activate_for_account(status,m_account);
			}
			
		}
		
		// send presence about tranport status to user
		if(m_connected || m_readyForConnect) {
			Presence tag(stanza.presence(), m_jid, stanza.status());
			tag.setFrom(p->jid());
			p->j->send( tag );
		} else {
			if (m_resource.empty()){
				Tag *tag = new Tag("presence");
				tag->addAttribute( "to", stanza.from().bare() );
				tag->addAttribute( "type", "unavailable" );
				tag->addAttribute( "from", p->jid() );
				p->j->send( tag );
			}
		}
	}
}

User::~User(){

	// send unavailable to online users
	Tag *tag;
	for(std::map<std::string, RosterRow>::iterator u = m_roster.begin(); u != m_roster.end() ; u++){
		if ((*u).second.online==true){
			tag = new Tag("presence");
			tag->addAttribute( "to", m_jid );
			tag->addAttribute( "type", "unavailable" );
			tag->addAttribute( "from", (*u).second.uin+"@"+ p->jid() + "/bot");
			p->j->send( tag );
			p->userManager()->buddyOffline();
		}
	}

	// destroy conversations
// 	GList *l;
// 	for (l = purple_get_conversations(); l != NULL; l = l->next){
// 		PurpleConversation *conv = (PurpleConversation *)l->data;
// 		if (purple_conversation_get_account(conv) == m_account){
// 			purple_conversation_destroy(conv);
// 		}
// 	}
	GList *iter;
	for (iter = purple_get_conversations(); iter; ) {
			PurpleConversation *conv = (PurpleConversation*) iter->data;
			iter = iter->next;
			if (purple_conversation_get_account(conv) == m_account)
				purple_conversation_destroy(conv);
	}



// 	purple_account_destroy(m_account);
// 	delete(m_account);
// 	if (this->save_timer!=0 && this->save_timer!=-1)
// 		std::cout << "* removing timer\n";
// 		purple_timeout_remove(this->save_timer);
	if (m_syncTimer!=0) {
		Log().Get(m_jid) << "removing timer\n";
		purple_timeout_remove(m_syncTimer);
	}
	m_roster.clear();
	m_conversations.clear();
	m_authRequests.clear();
}


