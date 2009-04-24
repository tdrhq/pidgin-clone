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

#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>
#include <map>
#include "account.h"
#include "user.h"

class GlooxMessageHandler;

class UserManager
{
	public:
		UserManager(GlooxMessageHandler *m);
		~UserManager();
		User *getUserByJID(std::string barejid);
		User *getUserByAccount(PurpleAccount *account);
		void addUser(User *user) { m_users[user->jid()] = user; }
		void removeUser(User *user);
		void removeUserTimer(User *user);
		long onlineUserCount();
		int userCount() { return m_users.size(); }
	
	private:
		GlooxMessageHandler *main;
		std::map<std::string, User*> m_users;
	
};

#endif
