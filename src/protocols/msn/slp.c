/**
 * @file msnslp.c MSNSLP support
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
#include "msn.h"
#include "slp.h"
#include "slpcall.h"
#include "slpmsg.h"
#include "slpsession.h"

#include "object.h"
#include "user.h"
#include "switchboard.h"

/* #include "slplink.h" */
/* #include "directconn.h" */

static void send_ok(MsnSlpCall *slpcall, const char *branch,
					const char *type, const char *content);

static void send_decline(MsnSlpCall *slpcall, const char *branch,
						 const char *type, const char *content);

/**************************************************************************
 * Util
 **************************************************************************/

char *
get_token(const char *str, const char *start, const char *end)
{
	const char *c, *c2;

	if ((c = strstr(str, start)) == NULL)
		return NULL;

	c += strlen(start);

	if (end != NULL)
	{
		if ((c2 = strstr(c, end)) == NULL)
			return NULL;

		return g_strndup(c, c2 - c);
	}
	else
	{
		/* This has to be changed */
		return g_strdup(c);
	}

}

/**************************************************************************
 * Xfer
 **************************************************************************/

static void
msn_xfer_init(GaimXfer *xfer)
{
	MsnSlpCall *slpcall;
	/* MsnSlpLink *slplink; */
	char *content;

	gaim_debug_info("msn", "xfer_init\n");

	slpcall = xfer->data;

	/* Send Ok */
	content = g_strdup_printf("SessionID: %lu\r\n\r\n",
							  slpcall->session_id);

	send_ok(slpcall, slpcall->branch, "application/x-msnmsgr-sessionreqbody",
			content);

	g_free(content);
	gaim_xfer_add(xfer);
	msn_slplink_unleash(slpcall->slplink);
}

void
msn_xfer_cancel(GaimXfer *xfer)
{
	MsnSlpCall *slpcall;
	char *content;

	slpcall = xfer->data;

	if (gaim_xfer_get_status(xfer) == GAIM_XFER_STATUS_CANCEL_LOCAL)
	{
		if (slpcall->started)
		{
			msn_slp_call_close(slpcall);
		}
		else
		{
			content = g_strdup_printf("SessionID: %lu\r\n\r\n",
									slpcall->session_id);

			send_decline(slpcall, slpcall->branch, "application/x-msnmsgr-sessionreqbody",
						content);

			g_free(content);
			msn_slplink_unleash(slpcall->slplink);
		}
	}
}

void
msn_xfer_progress_cb(MsnSlpCall *slpcall, gsize total_length, gsize len, gsize offset)
{
	GaimXfer *xfer;

	xfer = slpcall->xfer;

	xfer->bytes_sent = (offset + len);
	xfer->bytes_remaining = total_length - (offset + len);

	gaim_xfer_update_progress(xfer);
}

void
msn_xfer_end_cb(MsnSlpCall *slpcall)
{
	if (gaim_xfer_get_status(slpcall->xfer) != GAIM_XFER_STATUS_DONE)
	{
		gaim_xfer_cancel_remote(slpcall->xfer);
	}
}

void
msn_xfer_completed_cb(MsnSlpCall *slpcall, const char *body,
					  long long size)
{
	gaim_xfer_set_completed(slpcall->xfer, TRUE);
}

/**************************************************************************
 * SLP Control
 **************************************************************************/

#if 0
static void
got_transresp(MsnSlpCall *slpcall, const char *nonce,
			  const char *ips_str, int port)
{
	MsnDirectConn *directconn;
	char **ip_addrs, **c;

	directconn = msn_directconn_new(slpcall->slplink);

	directconn->initial_call = slpcall;

	/* msn_directconn_parse_nonce(directconn, nonce); */
	directconn->nonce = g_strdup(nonce);

	ip_addrs = g_strsplit(ips_str, " ", -1);

	for (c = ip_addrs; *c != NULL; c++)
	{
		gaim_debug_info("msn", "ip_addr = %s\n", *c);
		if (msn_directconn_connect(directconn, *c, port))
			break;
	}

	g_strfreev(ip_addrs);
}
#endif

static void
send_ok(MsnSlpCall *slpcall, const char *branch,
		const char *type, const char *content)
{
	MsnSlpLink *slplink;
	MsnSlpMessage *slpmsg;

	slplink = slpcall->slplink;

	/* 200 OK */
	slpmsg = msn_slpmsg_sip_new(slpcall, 1,
								"MSNSLP/1.0 200 OK",
								branch, type, content);

#ifdef DEBUG_SLP
	slpmsg->info = "SLP 200 OK";
	slpmsg->text_body = TRUE;
#endif

	msn_slplink_queue_slpmsg(slplink, slpmsg);

	msn_slp_call_session_init(slpcall);
}

static void
send_decline(MsnSlpCall *slpcall, const char *branch,
			 const char *type, const char *content)
{
	MsnSlpLink *slplink;
	MsnSlpMessage *slpmsg;

	slplink = slpcall->slplink;

	/* 603 Decline */
	slpmsg = msn_slpmsg_sip_new(slpcall, 1,
								"MSNSLP/1.0 603 Decline",
								branch, type, content);

#ifdef DEBUG_SLP
	slpmsg->info = "SLP 603 Decline";
	slpmsg->text_body = TRUE;
#endif

	msn_slplink_queue_slpmsg(slplink, slpmsg);
}

static void
got_sessionreq(MsnSlpCall *slpcall, const char *branch,
			   const char *euf_guid, const char *context)
{
	if (!strcmp(euf_guid, "A4268EEC-FEC5-49E5-95C3-F126696BDBF6"))
	{
		/* Emoticon or UserDisplay */
		MsnSlpSession *slpsession;
		MsnSlpLink *slplink;
		MsnSlpMessage *slpmsg;
		MsnObject *obj;
		char *msnobj_data;
		const char *sha1c;
		const char *file_name;
		char *content;
		gsize len;
		int type;

		/* Send Ok */
		content = g_strdup_printf("SessionID: %lu\r\n\r\n",
								  slpcall->session_id);

		send_ok(slpcall, branch, "application/x-msnmsgr-sessionreqbody",
				content);

		g_free(content);

		slplink = slpcall->slplink;

		gaim_base64_decode(context, &msnobj_data, &len);
		obj = msn_object_new_from_string(msnobj_data);
		type = msn_object_get_type(obj);
		sha1c = msn_object_get_sha1c(obj);
		g_free(msnobj_data);

		if (!(type == MSN_OBJECT_USERTILE))
		{
			gaim_debug_error("msn", "Wrong object?\n");
			msn_object_destroy(obj);
			g_return_if_reached();
		}

		file_name = msn_object_get_real_location(obj);

		slpsession = msn_slplink_find_slp_session(slplink,
												   slpcall->session_id);

		/* DATA PREP */
		slpmsg = msn_slpmsg_new(slplink);
		slpmsg->slpsession = slpsession;
		slpmsg->session_id = slpsession->id;
		msn_slpmsg_set_body(slpmsg, NULL, 4);
#ifdef DEBUG_SLP
		slpmsg->info = "SLP DATA PREP";
#endif
		msn_slplink_queue_slpmsg(slplink, slpmsg);

		/* DATA */
		slpmsg = msn_slpmsg_new(slplink);
		slpmsg->slpsession = slpsession;
		slpmsg->flags = 0x20;
#ifdef DEBUG_SLP
		slpmsg->info = "SLP DATA";
#endif
		msn_slpmsg_open_file(slpmsg, file_name);
		msn_slplink_queue_slpmsg(slplink, slpmsg);
	}
	else if (!strcmp(euf_guid, "5D3E02AB-6190-11D3-BBBB-00C04F795683"))
	{
		/* File Transfer */
		GaimAccount *account;
		GaimXfer *xfer;
		char *bin;
		gsize bin_len;
		guint32 file_size;
		char *file_name;

		account = slpcall->slplink->session->account;

		slpcall->cb = msn_xfer_completed_cb;
		slpcall->end_cb = msn_xfer_end_cb;
		slpcall->progress_cb = msn_xfer_progress_cb;
		slpcall->branch = g_strdup(branch);

		xfer = gaim_xfer_new(account, GAIM_XFER_RECEIVE,
							 slpcall->slplink->remote_user);

		gaim_base64_decode(context, &bin, &bin_len);
		file_size = *((gsize *)bin + 2);
		file_name = g_utf16_to_utf8((const gunichar2 *)(bin + 20), -1,
									NULL, NULL, NULL);

		g_free(bin);

		gaim_xfer_set_filename(xfer, file_name);
		gaim_xfer_set_size(xfer, file_size);
		gaim_xfer_set_init_fnc(xfer, msn_xfer_init);
		gaim_xfer_set_request_denied_fnc(xfer, msn_xfer_cancel);
		gaim_xfer_set_cancel_recv_fnc(xfer, msn_xfer_cancel);

		slpcall->xfer = xfer;
		xfer->data = slpcall;

		gaim_xfer_request(xfer);
	}
}

void
send_bye(MsnSlpCall *slpcall, const char *type)
{
	MsnSlpLink *slplink;
	MsnSlpMessage *slpmsg;
	char *header;

	slplink = slpcall->slplink;

	g_return_if_fail(slplink != NULL);

	header = g_strdup_printf("BYE MSNMSGR:%s MSNSLP/1.0",
							 slplink->local_user);

	slpmsg = msn_slpmsg_sip_new(slpcall, 0, header,
								"A0D624A6-6C0C-4283-A9E0-BC97B4B46D32",
								type,
								"\r\n");
	g_free(header);

#ifdef DEBUG_SLP
	slpmsg->info = "SLP BYE";
	slpmsg->text_body = TRUE;
#endif

	msn_slplink_queue_slpmsg(slplink, slpmsg);
}

static void
got_invite(MsnSlpCall *slpcall,
		   const char *branch, const char *type, const char *content)
{
	MsnSlpLink *slplink;

	slplink = slpcall->slplink;

	if (!strcmp(type, "application/x-msnmsgr-sessionreqbody"))
	{
		char *euf_guid, *context;
		char *temp;

		euf_guid = get_token(content, "EUF-GUID: {", "}\r\n");

		temp = get_token(content, "SessionID: ", "\r\n");
		if (temp != NULL)
			slpcall->session_id = atoi(temp);
		g_free(temp);

		temp = get_token(content, "AppID: ", "\r\n");
		if (temp != NULL)
			slpcall->app_id = atoi(temp);
		g_free(temp);

		context = get_token(content, "Context: ", "\r\n");

		got_sessionreq(slpcall, branch, euf_guid, context);

		g_free(context);
		g_free(euf_guid);
	}
	else if (!strcmp(type, "application/x-msnmsgr-transreqbody"))
	{
		/* A direct connection? */

		char *listening, *nonce;
		char *content;

		if (FALSE)
		{
#if 0
			MsnDirectConn *directconn;
			/* const char *ip_addr; */
			char *ip_port;
			int port;

			/* ip_addr = gaim_prefs_get_string("/core/ft/public_ip"); */
			ip_port = "5190";
			listening = "true";
			nonce = rand_guid();

			directconn = msn_directconn_new(slplink);

			/* msn_directconn_parse_nonce(directconn, nonce); */
			directconn->nonce = g_strdup(nonce);

			msn_directconn_listen(directconn);

			port = directconn->port;

			content = g_strdup_printf(
				"Bridge: TCPv1\r\n"
				"Listening: %s\r\n"
				"Nonce: {%s}\r\n"
				"Ipv4Internal-Addrs: 192.168.0.82\r\n"
				"Ipv4Internal-Port: %d\r\n"
				"\r\n",
				listening,
				nonce,
				port);
#endif
		}
		else
		{
			listening = "false";
			nonce = g_strdup("00000000-0000-0000-0000-000000000000");

			content = g_strdup_printf(
				"Bridge: TCPv1\r\n"
				"Listening: %s\r\n"
				"Nonce: {%s}\r\n"
				"\r\n",
				listening,
				nonce);
		}

		send_ok(slpcall, branch,
				"application/x-msnmsgr-transrespbody", content);

		g_free(content);
		g_free(nonce);
	}
	else if (!strcmp(type, "application/x-msnmsgr-transrespbody"))
	{
#if 0
		char *ip_addrs;
		char *temp;
		char *nonce;
		int port;

		nonce = get_token(content, "Nonce: {", "}\r\n");
		ip_addrs = get_token(content, "IPv4Internal-Addrs: ", "\r\n");

		temp = get_token(content, "IPv4Internal-Port: ", "\r\n");
		if (temp != NULL)
			port = atoi(temp);
		else
			port = -1;
		g_free(temp);

		if (ip_addrs == NULL)
			return;

		if (port > 0)
			got_transresp(slpcall, nonce, ip_addrs, port);

		g_free(nonce);
		g_free(ip_addrs);
#endif
	}
}

static void
got_ok(MsnSlpCall *slpcall,
	   const char *type, const char *content)
{
	g_return_if_fail(slpcall != NULL);
	g_return_if_fail(type    != NULL);

	if (!strcmp(type, "application/x-msnmsgr-sessionreqbody"))
	{
#if 0
		if (slpcall->type == MSN_SLPCALL_DC)
		{
			/* First let's try a DirectConnection. */

			MsnSlpLink *slplink;
			MsnSlpMessage *slpmsg;
			char *header;
			char *content;
			char *branch;

			slplink = slpcall->slplink;

			branch = rand_guid();

			content = g_strdup_printf(
				"Bridges: TRUDPv1 TCPv1\r\n"
				"NetID: 0\r\n"
				"Conn-Type: Direct-Connect\r\n"
				"UPnPNat: false\r\n"
				"ICF: false\r\n"
			);

			header = g_strdup_printf("INVITE MSNMSGR:%s MSNSLP/1.0",
									 slplink->remote_user);

			slpmsg = msn_slp_sipmsg_new(slpcall, 0, header, branch,
										"application/x-msnmsgr-transreqbody",
										content);

#ifdef DEBUG_SLP
			slpmsg->info = "SLP INVITE";
			slpmsg->text_body = TRUE;
#endif
			msn_slplink_send_slpmsg(slplink, slpmsg);

			g_free(header);
			g_free(content);

			g_free(branch);
		}
		else
		{
			msn_slp_call_session_init(slpcall);
		}
#else
		msn_slp_call_session_init(slpcall);
#endif
	}
	else if (!strcmp(type, "application/x-msnmsgr-transreqbody"))
	{
		/* Do we get this? */
		gaim_debug_info("msn", "OK with transreqbody\n");
	}
	else if (!strcmp(type, "application/x-msnmsgr-transrespbody"))
	{
#if 0
		char *ip_addrs;
		char *temp;
		char *nonce;
		int port;

		nonce = get_token(content, "Nonce: {", "}\r\n");
		ip_addrs = get_token(content, "IPv4Internal-Addrs: ", "\r\n");

		temp = get_token(content, "IPv4Internal-Port: ", "\r\n");
		if (temp != NULL)
			port = atoi(temp);
		else
			port = -1;
		g_free(temp);

		if (ip_addrs == NULL)
			return;

		if (port > 0)
			got_transresp(slpcall, nonce, ip_addrs, port);

		g_free(nonce);
		g_free(ip_addrs);
#endif
	}
}

MsnSlpCall *
msn_slp_sip_recv(MsnSlpLink *slplink, const char *body, gsize len)
{
	MsnSlpCall *slpcall;

	if (!strncmp(body, "INVITE", strlen("INVITE")))
	{
		char *branch;
		char *content;
		char *content_type;

		slpcall = msn_slp_call_new(slplink);

		/* From: <msnmsgr:buddy@hotmail.com> */
#if 0
		slpcall->remote_user = get_token(body, "From: <msnmsgr:", ">\r\n");
#endif

		branch = get_token(body, ";branch={", "}");

		slpcall->id = get_token(body, "Call-ID: {", "}");

#if 0
		long content_len = -1;

		temp = get_token(body, "Content-Length: ", "\r\n");
		if (temp != NULL)
			content_len = atoi(temp);
		g_free(temp);
#endif
		content_type = get_token(body, "Content-Type: ", "\r\n");

		content = get_token(body, "\r\n\r\n", NULL);

		got_invite(slpcall, branch, content_type, content);

		g_free(content_type);
		g_free(content);
	}
	else if (!strncmp(body, "MSNSLP/1.0 ", strlen("MSNSLP/1.0 ")))
	{
		char *content;
		char *content_type;
		/* Make sure this is "OK" */
		const char *status = body + strlen("MSNSLP/1.0 ");
		char *call_id;

		call_id = get_token(body, "Call-ID: {", "}");
		slpcall = msn_slplink_find_slp_call(slplink, call_id);
		g_free(call_id);

		g_return_val_if_fail(slpcall != NULL, NULL);

		if (strncmp(status, "200 OK", 6))
		{
			/* It's not valid. Kill this off. */
			char temp[32];
			const char *c;

			/* Eww */
			if ((c = strchr(status, '\r')) || (c = strchr(status, '\n')) ||
				(c = strchr(status, '\0')))
			{
				strncpy(temp, status, c - status);
				temp[c - status] = '\0';
			}

			gaim_debug_error("msn", "Received non-OK result: %s\n", temp);

			slpcall->wasted = TRUE;

			/* msn_slp_call_destroy(slpcall); */
			return slpcall;
		}

		content_type = get_token(body, "Content-Type: ", "\r\n");

		content = get_token(body, "\r\n\r\n", NULL);

		got_ok(slpcall, content_type, content);

		g_free(content_type);
		g_free(content);
	}
	else if (!strncmp(body, "BYE", strlen("BYE")))
	{
		char *call_id;

		call_id = get_token(body, "Call-ID: {", "}");
		slpcall = msn_slplink_find_slp_call(slplink, call_id);
		g_free(call_id);

		if (slpcall != NULL)
			slpcall->wasted = TRUE;

		/* msn_slp_call_destroy(slpcall); */
	}
	else
		slpcall = NULL;

	return slpcall;
}

/**************************************************************************
 * Msg Callbacks
 **************************************************************************/

void
msn_p2p_msg(MsnCmdProc *cmdproc, MsnMessage *msg)
{
	MsnSession *session;
	MsnSlpLink *slplink;

	session = cmdproc->servconn->session;
	slplink = msn_session_get_slplink(session, msg->remote_user);

	msn_slplink_process_msg(slplink, msg);
}

void
got_emoticon(MsnSlpCall *slpcall,
			 const char *data, long long size)
{
	gaim_debug_info("msn", "Got smiley: %s\n", slpcall->data_info);

#if 0
	GaimConversation *conv;
	GaimConnection *gc = slpsession->swboard->servconn->session->account->gc;
	serv_got_smiley(gc, info, data, size);
#endif
}

void
msn_emoticon_msg(MsnCmdProc *cmdproc, MsnMessage *msg)
{
	MsnSession *session;
	MsnSlpLink *slplink;
	MsnObject *obj;
	char **tokens;
	char *smile;
	const char *who;

	session = cmdproc->servconn->session;

	tokens = g_strsplit(msg->body, "\t", 2);

	smile = tokens[0];
	obj = msn_object_new_from_string(gaim_url_decode(tokens[1]));

	who = msn_object_get_creator(obj);

	slplink = msn_session_get_slplink(session, who);

	msn_slplink_request_object(slplink, smile, got_emoticon, obj);

	g_strfreev(tokens);
}

void
got_user_display(MsnSlpCall *slpcall,
				 const char *data, long long size)
{
	const char *info;
	GaimAccount *account;
	GSList *sl;

	info = slpcall->data_info;
	gaim_debug_info("msn", "Got User Display: %s\n", info);

	account = slpcall->slplink->session->account;

	/* TODO: I think we need better buddy icon core functions. */
	gaim_buddy_icons_set_for_user(account, slpcall->slplink->remote_user,
								  (void *)data, size);

	sl = gaim_find_buddies(account, slpcall->slplink->remote_user);

	for (; sl != NULL; sl = sl->next)
	{
		GaimBuddy *buddy = (GaimBuddy *)sl->data;
		gaim_blist_node_set_string((GaimBlistNode*)buddy, "icon_checksum", info);
		gaim_blist_save();
	}
}

static gboolean
buddy_icon_cached(GaimConnection *gc, MsnObject *obj)
{
	GaimAccount *account;
	GaimBuddy *buddy;
	GSList *sl;
	const char *old;
	const char *new;

	g_return_val_if_fail(obj != NULL, FALSE);

	account = gaim_connection_get_account(gc);

	sl = gaim_find_buddies(account, msn_object_get_creator(obj));

	if (sl == NULL)
	{
		return FALSE;
	}

	buddy = (GaimBuddy *)sl->data;

	old = gaim_blist_node_get_string((GaimBlistNode *)buddy, "icon_checksum");
	new = msn_object_get_sha1c(obj);

	if (new == NULL)
	{
		return FALSE;
	}

	if (old != NULL && !strcmp(old, new))
		return TRUE;

	return FALSE;
}

void
msn_request_buddy_icon(GaimConnection *gc, const char *passport)
{
	MsnSession *session;
	MsnSlpLink *slplink;
	MsnUser *user;
	MsnObject *obj;
	const char *info;

	session = gc->proto_data;

	g_return_if_fail(session->protocol_ver == 9);

	slplink = msn_session_get_slplink(session, passport);

	user = msn_userlist_find_user(session->userlist, passport);

	obj = msn_user_get_object(user);

	if (obj == NULL)
		/* It seems the user has not set a msnobject */
		return;

	info = msn_object_get_sha1c(obj);

	if (!buddy_icon_cached(gc, obj))
		msn_slplink_request_object(slplink, info, got_user_display, obj);
}
