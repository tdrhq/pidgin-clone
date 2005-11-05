/**
 * @file jabber.h The Gaim interface to mDNS and peer to peer Jabber.
 *
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

#ifndef _BONJOUR_JABBER_H_
#define _BONJOUR_JABBER_H_

#include "account.h"

#define DOCTYPE_DECLARATION "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
#define STREAM_START "<stream:stream xmlns=\"jabber:client\" xmlns:stream=\"http://etherx.jabber.org/streams\">"
#define CONVERSATION_START "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><stream:stream xmlns=\"jabber:client\" xmlns:stream=\"http://etherx.jabber.org/streams\">"
#define STREAM_END "</stream:stream>"
#define DOCTYPE "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<stream:stream xmlns=\"jabber:client\" xmlns:stream=\"http://etherx.jabber.org/streams\" >"

typedef struct _BonjourJabber
{
	gint port;
	gint socket;
	gint watcher_id;
	GaimAccount* account;
} BonjourJabber;

typedef struct _BonjourJabberConversation
{
	gint socket;
	gint watcher_id;
	gchar* buddy_name;
	gboolean start_step_one;
	gboolean start_step_two;
	gboolean stream_started;
	gint message_id;
} BonjourJabberConversation;

/**
 * Start listening for jabber connections. Returns 0 if the connection could be
 * stablished, -1 if a problem appears.
 */
gint bonjour_jabber_start(BonjourJabber *data);

int bonjour_jabber_send_message(BonjourJabber *data, const gchar *to, const gchar *body);

void bonjour_jabber_close_conversation(BonjourJabber *data, GaimBuddy *gb);

void bonjour_jabber_stop(BonjourJabber *data);

#endif /* _BONJOUR_JABBER_H_ */
