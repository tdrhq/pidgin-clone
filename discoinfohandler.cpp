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

#include "discoinfohandler.h"
#include "protocols/abstractprotocol.h"

GlooxDiscoInfoHandler::GlooxDiscoInfoHandler(GlooxMessageHandler *parent) : IqHandler(){
	p=parent;
}

GlooxDiscoInfoHandler::~GlooxDiscoInfoHandler(){
}


bool GlooxDiscoInfoHandler::handleIq (const IQ &stanza){

/*	User *user = p->getUserByJID(stanza.from().bare());
	if (user==NULL)
		return true;

	if(stanza.subtype() == IQ::Get) {
		std::list<std::string> temp;
		temp.push_back((std::string)stanza.id());
		temp.push_back((std::string)stanza.from().full());
		vcardRequests[(std::string)stanza.to().username()]=temp;
		
		serv_get_info(purple_account_get_connection(user->account), stanza.to().username().c_str());
	}*/
/*
<iq from='romeo@montague.lit/orchard' 
    id='disco1'
    to='juliet@capulet.lit/chamber' 
    type='result'>
  <query xmlns='http://jabber.org/protocol/disco#info'
         node='http://code.google.com/p/exodus#QgayPKawpkPSDYmwT/WM94uAlu0='>
    <identity category='client' name='Exodus 0.9.1' type='pc'/>
    <feature var='http://jabber.org/protocol/caps'/>
    <feature var='http://jabber.org/protocol/disco#info'/>
    <feature var='http://jabber.org/protocol/disco#items'/>
    <feature var='http://jabber.org/protocol/muc'/>
  </query>
</iq>*/

	std::cout << "DISCO DISCO DISCO INFO\n";
	if(stanza.subtype() == IQ::Get && stanza.to().username()!="") {
		Tag *query = stanza.tag()->findChildWithAttrib("xmlns","http://jabber.org/protocol/disco#info");
		if (query!=NULL){
			IQ _s(IQ::Result, stanza.from(), stanza.id());
			std::string from;
			from.append(stanza.to().full());
			_s.setFrom(from);
			
			Tag *s = _s.tag();
			s->setXmlns("http://jabber.org/protocol/disco#info");
			s->addAttribute("node",query->findAttribute("node"));
			Tag *t;
			t = new Tag("identity");
			t->addAttribute("category","gateway");
			t->addAttribute("name","High Flyer Transport");
			t->addAttribute("type",p->protocol()->gatewayIdentity());
			s->findChild("query")->addChild(t);

			std::list<std::string> features = p->protocol()->transportFeatures();
			for(std::list<std::string>::iterator it = features.begin(); it != features.end(); it++) {
				t = new Tag("feature");
				t->addAttribute("var",*it);
				s->findChild("query")->addChild(t);
			}

			p->j->send( s );

			return true;
		}
	}
	return p->j->disco()->handleIq(stanza);
}

void GlooxDiscoInfoHandler::handleIqID (const IQ &stanza, int context){
	p->j->disco()->handleIqID(stanza,context);
}
