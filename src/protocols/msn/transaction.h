/**
 * @file transaction.h MSN transaction functions
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
#ifndef _MSN_TRANSACTION_H
#define _MSN_TRANSACTION_H

typedef struct _MsnTransaction MsnTransaction;

#include "command.h"
#include "cmdproc.h"

typedef void (*MsnTransCb)(MsnCmdProc *cmdproc, MsnCommand *cmd);

/**
 * A transaction. A command that will initiate the transaction.
 */
struct _MsnTransaction
{
	unsigned int trId;

	char *command;
	char *params;

	GHashTable *callbacks;
	void *data;

	char *payload;
	size_t payload_len;

	GQueue *queue;
	MsnCommand *pendent_cmd;
};

MsnTransaction *msn_transaction_new(const char *command,
									const char *format, ...);
void msn_transaction_destroy(MsnTransaction *trans);

char *msn_transaction_to_string(MsnTransaction *trans);
void msn_transaction_queue_cmd(MsnTransaction *trans, MsnCommand *cmd);
void msn_transaction_unqueue_cmd(MsnTransaction *trans, MsnCmdProc *cmdproc);
void msn_transaction_set_payload(MsnTransaction *trans,
								 const char *payload, int payload_len);
void msn_transaction_set_data(MsnTransaction *trans, void *data);
void msn_transaction_add_cb(MsnTransaction *trans, char *answer,
							MsnTransCb cb, void *data);

#endif /* _MSN_TRANSACTION_H */
