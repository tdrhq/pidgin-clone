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

#ifndef _HI_GATEWAY_H
#define _HI_GATEWAY_H

#include <gloox/iqhandler.h>
#include <gloox/clientbase.h>

class GlooxMessageHandler;

using namespace gloox;


class GlooxGatewayHandler : public IqHandler
{

public:
	GlooxGatewayHandler(GlooxMessageHandler *parent);
	~GlooxGatewayHandler();
	bool handleIq (const IQ &iq);
	void handleIqID (const IQ &iq, int context);
	std::string replace(std::string &str, const char *string_to_replace, const char *new_string);
	GlooxMessageHandler *p;
};

#endif
