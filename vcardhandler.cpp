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

#include "vcardhandler.h"
#include "usermanager.h"
#include "log.h"
#include "gloox/vcard.h"
#include "protocols/abstractprotocol.h"

void base64encode(const unsigned char * input, int len, std::string & out)
{

    static char b64lookup[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                "abcdefghijklmnopqrstuvwxyz"
                                "0123456789+/";

    int outlen = ((len + 2) / 3) * 4;
    unsigned long octets;
    unsigned char b2, b3, b4;
    int i, remaining;

    out.resize(outlen);

    std::string::iterator outiter = out.begin();

    for (i = 0, remaining = len; i < len;) {
        if (remaining >= 3) {
            octets = input[i] << 16;
            octets |= input[i+1] << 8;
            octets |= input[i+2];

            b4 = b64lookup[octets & 0x3F]; octets >>= 6;
            b3 = b64lookup[octets & 0x3F]; octets >>= 6;
            b2 = b64lookup[octets & 0x3F]; octets >>= 6;
            *outiter++ = b64lookup[octets & 0x3F];
            *outiter++ = b2;
            *outiter++ = b3;
            *outiter++ = b4;
            i+=3;
            remaining -= 3;
        } else if (remaining == 2) {
            octets = input[i] << 16;
            octets |= input[i+1] << 8;

            b4 = '='; octets >>= 6;
            b3 = b64lookup[octets & 0x3F]; octets >>= 6;
            b2 = b64lookup[octets & 0x3F]; octets >>= 6;
            *outiter++ = b64lookup[octets & 0x3F];
            *outiter++ = b2;
            *outiter++ = b3;
            *outiter++ = b4;
            i+=2;
            remaining -= 2; 
        } else {
            octets = input[i] << 16;

            b4 = '='; octets >>= 6;
            b3 = '='; octets >>= 6;
            b2 = b64lookup[octets & 0x3F]; octets >>= 6;
            *outiter++ = b64lookup[octets & 0x3F];
            *outiter++ = b2;
            *outiter++ = b3;
            *outiter++ = b4;
            i++;
            remaining--;  
        }
    }
}   

static void sendVCardTag(Tag *tag, Tag *stanzaTag) {
	std::string id = stanzaTag->findAttribute("id");
	std::string from = stanzaTag->findAttribute("from");
	Tag *vcard = tag->clone();
	vcard->addAttribute("id",id);
	vcard->addAttribute("to",from);
	GlooxMessageHandler::instance()->j->send(vcard);
	delete stanzaTag;
}

GlooxVCardHandler::GlooxVCardHandler(GlooxMessageHandler *parent) : IqHandler(){
	p=parent;
	p->j->registerStanzaExtension( new VCard() );
}

GlooxVCardHandler::~GlooxVCardHandler(){
}

bool GlooxVCardHandler::handleIq (const IQ &stanza){

	if (stanza.to().username()=="")
		return false;

	User *user = p->userManager()->getUserByJID(stanza.from().bare());
	if (user==NULL)
		return false;
	if (!user->isConnected()){
		return false;
	}

	if(stanza.subtype() == IQ::Get) {
		Tag *stanzaTag = stanza.tag();
		
		if (!p->sql()->getVCard(stanza.to().username(), sendVCardTag, stanzaTag)) {
			std::list<std::string> temp;
			temp.push_back((std::string)stanza.id());
			temp.push_back((std::string)stanza.from().full());
			vcardRequests[(std::string)stanza.to().username()]=temp;
			delete stanzaTag;
			serv_get_info(purple_account_get_connection(user->account()), stanza.to().username().c_str());
		}
	}

	return true;
}

bool GlooxVCardHandler::hasVCardRequest(const std::string &name){
	std::map<std::string,std::list<std::string> > ::iterator iter = this->vcardRequests.begin();
	iter = this->vcardRequests.find(name);
	if(iter != this->vcardRequests.end())
		return true;
	return false;
}

void GlooxVCardHandler::userInfoArrived(PurpleConnection *gc,std::string who, PurpleNotifyUserInfo *user_info){
	GList *vcardEntries = purple_notify_user_info_get_entries(user_info);
	User *user = p->userManager()->getUserByAccount(purple_connection_get_account(gc));

	if (user!=NULL){
		if (!user->isConnected())
			return;
		if (!hasVCardRequest(who))
			return;
		std::cout << "VCard received, making vcard IQ\n";
		Tag *reply = new Tag( "iq" );
		reply->addAttribute( "id", vcardRequests[who].front() );
		reply->addAttribute( "type", "result" );
		reply->addAttribute( "to", vcardRequests[who].back() );
		reply->addAttribute( "from", who+"@"+p->jid() );

		Tag *vcard = p->protocol()->getVCardTag(user, vcardEntries);
		if (!vcard) {
			vcard = new Tag( "vCard" );
			vcard->addAttribute( "xmlns", "vcard-temp" );
		}

		if (purple_value_get_boolean(user->getSetting("enable_avatars"))) {
			Tag *photo = new Tag("PHOTO");

			PurpleBuddy *buddy = purple_find_buddy(purple_connection_get_account(gc), who.c_str());
			if (buddy){
				std::cout << "found buddy " << who << "\n";
				gsize len;
				PurpleBuddyIcon *icon = NULL;
				icon = purple_buddy_icons_find(purple_connection_get_account(gc), who.c_str());
				if(icon!=NULL) {
					std::cout << "found icon\n";
					const gchar * data = (gchar*)purple_buddy_icon_get_data(icon, &len);
					if (data!=NULL){
						std::cout << "making avatar " << len <<"\n";
						std::string avatarData((char *)data,len);
						base64encode((unsigned char *)data, len, avatarData);
						photo->addChild( new Tag("BINVAL", avatarData));
						std::cout << "avatar made\n";
	// 					std::cout << photo->xml() << "\n";
					}
				}
			}

			if(!photo->children().empty())
				vcard->addChild(photo);
			else 
				delete photo;
		}

		reply->addChild(vcard);
		p->sql()->updateVCard(who, reply->xml());
		p->j->send(reply);
		vcardRequests.erase(who);
	}
}

void GlooxVCardHandler::handleIqID (const IQ &iq, int context){
}
