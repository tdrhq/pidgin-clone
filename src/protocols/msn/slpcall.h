/**
 * @file slpcall.h SLP Call functions
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
 */
#ifndef _MSN_SLPCALL_H_
#define _MSN_SLPCALL_H_

typedef struct _MsnSlpCall MsnSlpCall;

typedef void (*MsnSlpCb)(MsnSlpCall *slpcall,
						 const char *data, long long size);

#include "slplink.h"
#include "slpsession.h"

typedef enum
{
	MSN_SLPCALL_ANY,
	MSN_SLPCALL_DC,

} MsnSlpCallType;

struct _MsnSlpCall
{
	/* MsnSession *session; */
	MsnSlpLink *slplink;

	MsnSlpCallType type;

	/* Call-ID */
	char *id;
	char *branch;

	long session_id;
	long app_id;

	void (*progress_cb)(MsnSlpCall *slpcall,
						gsize total_length, gsize len, gsize offset);
	void (*session_init_cb)(MsnSlpSession *slpsession);

	/* Can be checksum, or smile */
	char *data_info;

	void *xfer;

	MsnSlpCb cb;
	gboolean wasted;
	gboolean started;
};

MsnSlpCall *msn_slp_call_new(MsnSlpLink *slplink);
void msn_slp_call_init(MsnSlpCall *slpcall, MsnSlpCallType type);
void msn_slp_call_session_init(MsnSlpCall *slpcall);
void msn_slp_call_destroy(MsnSlpCall *slpcall);
void msn_slp_call_invite(MsnSlpCall *slpcall, const char *euf_guid,
						 int app_id, const char *context);
void msn_slp_call_close(MsnSlpCall *slpcall);

#endif /* _MSN_SLPCALL_H_ */
