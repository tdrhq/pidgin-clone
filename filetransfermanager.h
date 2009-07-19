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

#ifndef FILETRANSFERMANAGER_H
#define FILETRANSFERMANAGER_H

#include "sendfile.h"
#include "receivefile.h"
#include "thread.h"

#include <sys/stat.h>
#include <sys/types.h>
class GlooxMessageHandler;
#include "main.h"

#include <gloox/siprofilefthandler.h>
#include <gloox/siprofileft.h>

#include <iostream>
#include <algorithm>

struct progress {
	std::string filename;
	bool incoming;
	int state;
	gloox::JID from;
	gloox::JID to;
	std::string user;
	gloox::Bytestream *stream;
	
};

class FileTransferManager : public gloox::SIProfileFTHandler {
	struct Info {
		std::string filename;
		int size;
	};

	std::map<std::string, Info> m_info;
	
	std::list<std::string> m_sendlist;
	
	
	
	public:
		GlooxMessageHandler *p;
		gloox::SIProfileFT *m_sip;
		MyMutex *mutex;
		std::map<std::string, progress> m_progress;
		void setSIProfileFT(gloox::SIProfileFT *sipft,GlooxMessageHandler *parent);

// 		void handleFTRequest(   const gloox::JID &from,const gloox::JID &to, const std::string &id, const std::string &sid, const std::string &name,
//                              long size, const std::string &hash, const std::string &date, const std::string &mimetype,
//                              const std::string &desc, int stypes, long offset, long length);
 
// 		void handleFTRequestError(gloox::Stanza *stanza, const std::string &sid);

// 		void handleFTSOCKS5Bytestream(gloox::SOCKS5Bytestream *s5b);
		void handleFTRequest (const JID &from, const JID &to, const std::string &sid, const std::string &name, long size, const std::string &hash, const std::string &date, const std::string &mimetype, const std::string &desc, int stypes, long offset, long length);
		void handleFTRequestError (const IQ &iq, const std::string &sid) {}
		void handleFTBytestream (Bytestream *bs);
		const std::string handleOOBRequestResult (const JID &from, const JID &to, const std::string &sid) { return ""; }

		void sendFile(std::string jid,std::string from,std::string name, std::string file);
	};
#endif
