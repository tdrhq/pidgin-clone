/**
 * @file switchboard.c MSN switchboard functions
 *
 * purple
 *
 * Purple is the legal property of its developers, whose names are too numerous
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
#include "prefs.h"
#include "switchboard.h"
#include "notification.h"
#include "msnutils.h"

#include "error.h"

#ifdef USE_MSPACK
#include <mspack.h>
#endif

#define MSN_DEBUG_SB

#define WINK_HTML_TEMPLATE "<h1 style=\"text-align:center\">Playing MSN Wink</h1>\n" \
"<div style=\"width:400; height: 300; border: 1px solid black; margin:auto; text-align: center\">\n" \
"<object type=\"application/x-shockwave-flash\" data=\"%s\" width=\"400\" height=\"300\">\n" \
"<param name=\"movie\" value=\"%s\"/>\n" \
"<param name=\"loop\" value=\"false\"/>\n" \
"<div style=\"padding: 5px\">\n" \
"<h2>Your browser does not support Shockwave Flash.</h2>\n" \
"This software is required to play winks.<p><img src=\"%s\"/></div></object></div>"

#undef close


static MsnTable *cbs_table;
static GHashTable *datacast_table;

static void msg_error_helper(MsnCmdProc *cmdproc, MsnMessage *msg,
							 MsnMsgErrorType error);

/**************************************************************************
 * Main
 **************************************************************************/

MsnSwitchBoard *
msn_switchboard_new(MsnSession *session)
{
	MsnSwitchBoard *swboard;
	MsnServConn *servconn;

	g_return_val_if_fail(session != NULL, NULL);

	swboard = g_new0(MsnSwitchBoard, 1);

	swboard->session = session;
	swboard->servconn = servconn = msn_servconn_new(session, MSN_SERVCONN_SB);
	swboard->cmdproc = servconn->cmdproc;

	swboard->msg_queue = g_queue_new();
	swboard->empty = TRUE;

	swboard->cmdproc->data = swboard;
	swboard->cmdproc->cbs_table = cbs_table;

	session->switches = g_list_append(session->switches, swboard);

	return swboard;
}

void
msn_switchboard_destroy(MsnSwitchBoard *swboard)
{
	MsnSession *session;
	MsnMessage *msg;
	GList *l;

#ifdef MSN_DEBUG_SB
	purple_debug_info("msn", "switchboard_destroy: swboard(%p)\n", swboard);
#endif

	g_return_if_fail(swboard != NULL);

	if (swboard->destroying)
		return;

	swboard->destroying = TRUE;

	/* If it linked us is because its looking for trouble */
	while (swboard->slplinks != NULL)
		msn_slplink_destroy(swboard->slplinks->data);

	/* Destroy the message queue */
	while ((msg = g_queue_pop_head(swboard->msg_queue)) != NULL)
	{
		if (swboard->error != MSN_SB_ERROR_NONE)
		{
			/* The messages could not be sent due to a switchboard error */
			msg_error_helper(swboard->cmdproc, msg,
							 MSN_MSG_ERROR_SB);
		}
		msn_message_unref(msg);
	}

	g_queue_free(swboard->msg_queue);

	/* msg_error_helper will both remove the msg from ack_list and
	   unref it, so we don't need to do either here */
	while ((l = swboard->ack_list) != NULL)
		msg_error_helper(swboard->cmdproc, l->data, MSN_MSG_ERROR_SB);

	g_free(swboard->im_user);
	g_free(swboard->auth_key);
	g_free(swboard->session_id);

	for (l = swboard->users; l != NULL; l = l->next)
		g_free(l->data);

	session = swboard->session;
	session->switches = g_list_remove(session->switches, swboard);

#if 0
	/* This should never happen or we are in trouble. */
	if (swboard->servconn != NULL)
		msn_servconn_destroy(swboard->servconn);
#endif

	swboard->cmdproc->data = NULL;

	msn_servconn_set_disconnect_cb(swboard->servconn, NULL);

	msn_servconn_destroy(swboard->servconn);

	g_free(swboard);
}

void
msn_switchboard_set_auth_key(MsnSwitchBoard *swboard, const char *key)
{
	g_return_if_fail(swboard != NULL);
	g_return_if_fail(key != NULL);

	swboard->auth_key = g_strdup(key);
}

const char *
msn_switchboard_get_auth_key(MsnSwitchBoard *swboard)
{
	g_return_val_if_fail(swboard != NULL, NULL);

	return swboard->auth_key;
}

void
msn_switchboard_set_session_id(MsnSwitchBoard *swboard, const char *id)
{
	g_return_if_fail(swboard != NULL);
	g_return_if_fail(id != NULL);

	if (swboard->session_id != NULL)
		g_free(swboard->session_id);

	swboard->session_id = g_strdup(id);
}

const char *
msn_switchboard_get_session_id(MsnSwitchBoard *swboard)
{
	g_return_val_if_fail(swboard != NULL, NULL);

	return swboard->session_id;
}

void
msn_switchboard_set_invited(MsnSwitchBoard *swboard, gboolean invited)
{
	g_return_if_fail(swboard != NULL);

	swboard->invited = invited;
}

gboolean
msn_switchboard_is_invited(MsnSwitchBoard *swboard)
{
	g_return_val_if_fail(swboard != NULL, FALSE);

	return swboard->invited;
}

/**************************************************************************
 * Utility
 **************************************************************************/

static void
send_clientcaps(MsnSwitchBoard *swboard)
{
	MsnMessage *msg;

	msg = msn_message_new(MSN_MSG_CAPS);
	msn_message_set_content_type(msg, "text/x-clientcaps");
	msn_message_set_flag(msg, 'U');
	msn_message_set_bin_data(msg, MSN_CLIENTINFO, strlen(MSN_CLIENTINFO));

	msn_switchboard_send_msg(swboard, msg, TRUE);

	msn_message_destroy(msg);
}

static void
msn_switchboard_add_user(MsnSwitchBoard *swboard, const char *user)
{
	MsnCmdProc *cmdproc;
	PurpleAccount *account;

	g_return_if_fail(swboard != NULL);

	cmdproc = swboard->cmdproc;
	account = cmdproc->session->account;

	swboard->users = g_list_prepend(swboard->users, g_strdup(user));
	swboard->current_users++;
	swboard->empty = FALSE;

#ifdef MSN_DEBUG_CHAT
	purple_debug_info("msn", "user=[%s], total=%d\n", user,
					swboard->current_users);
#endif

	if (!(swboard->flag & MSN_SB_FLAG_IM) && (swboard->conv != NULL))
	{
		/* This is a helper switchboard. */
		purple_debug_error("msn", "switchboard_add_user: conv != NULL\n");
		return;
	}

	if ((swboard->conv != NULL) &&
		(purple_conversation_get_type(swboard->conv) == PURPLE_CONV_TYPE_CHAT))
	{
		purple_conv_chat_add_user(PURPLE_CONV_CHAT(swboard->conv), user, NULL,
								PURPLE_CBFLAGS_NONE, TRUE);
	}
	else if (swboard->current_users > 1 || swboard->total_users > 1)
	{
		if (swboard->conv == NULL ||
			purple_conversation_get_type(swboard->conv) != PURPLE_CONV_TYPE_CHAT)
		{
			GList *l;

#ifdef MSN_DEBUG_CHAT
			purple_debug_info("msn", "[chat] Switching to chat.\n");
#endif

#if 0
			/* this is bad - it causes msn_switchboard_close to be called on the
			 * switchboard we're in the middle of using :( */
			if (swboard->conv != NULL)
				purple_conversation_destroy(swboard->conv);
#endif

			swboard->chat_id = cmdproc->session->conv_seq++;
			swboard->flag |= MSN_SB_FLAG_IM;
			swboard->conv = serv_got_joined_chat(account->gc,
												 swboard->chat_id,
												 "MSN Chat");

			for (l = swboard->users; l != NULL; l = l->next)
			{
				const char *tmp_user;

				tmp_user = l->data;

#ifdef MSN_DEBUG_CHAT
				purple_debug_info("msn", "[chat] Adding [%s].\n", tmp_user);
#endif

				purple_conv_chat_add_user(PURPLE_CONV_CHAT(swboard->conv),
										tmp_user, NULL, PURPLE_CBFLAGS_NONE, TRUE);
			}

#ifdef MSN_DEBUG_CHAT
			purple_debug_info("msn", "[chat] We add ourselves.\n");
#endif

			purple_conv_chat_add_user(PURPLE_CONV_CHAT(swboard->conv),
									purple_account_get_username(account),
									NULL, PURPLE_CBFLAGS_NONE, TRUE);

			g_free(swboard->im_user);
			swboard->im_user = NULL;
		}
	}
	else if (swboard->conv == NULL)
	{
		swboard->conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM,
															user, account);
	}
	else
	{
		purple_debug_warning("msn", "switchboard_add_user: This should not happen!\n");
	}
}

static PurpleConversation *
msn_switchboard_get_conv(MsnSwitchBoard *swboard)
{
	PurpleAccount *account;

	g_return_val_if_fail(swboard != NULL, NULL);

	if (swboard->conv != NULL)
		return swboard->conv;

	purple_debug_error("msn", "Switchboard with unassigned conversation\n");

	account = swboard->session->account;

	return (swboard->conv = purple_conversation_new(PURPLE_CONV_TYPE_IM,
												  account, swboard->im_user));
}

static void
msn_switchboard_report_user(MsnSwitchBoard *swboard, PurpleMessageFlags flags, const char *msg)
{
	PurpleConversation *conv;

	g_return_if_fail(swboard != NULL);
	g_return_if_fail(msg != NULL);

	if ((conv = msn_switchboard_get_conv(swboard)) != NULL)
	{
		purple_conversation_write(conv, NULL, msg, flags, time(NULL));
	}
}

static void
swboard_error_helper(MsnSwitchBoard *swboard, int reason, const char *passport)
{
	g_return_if_fail(swboard != NULL);

	purple_debug_warning("msg", "Error: Unable to call the user %s for reason %i\n",
					   passport ? passport : "(null)", reason);

	/* TODO: if current_users > 0, this is probably a chat and an invite failed,
	 * we should report that in the chat or something */
	if (swboard->current_users == 0)
	{
		swboard->error = reason;
		msn_switchboard_close(swboard);
	}
}

static void
cal_error_helper(MsnTransaction *trans, int reason)
{
	MsnSwitchBoard *swboard;
	const char *passport;
	char **params;

	params = g_strsplit(trans->params, " ", 0);

	passport = params[0];

	swboard = trans->data;

	purple_debug_warning("msn", "cal_error_helper: command %s failed for reason %i\n",trans->command,reason);

	swboard_error_helper(swboard, reason, passport);

	g_strfreev(params);
}

static void
msg_error_helper(MsnCmdProc *cmdproc, MsnMessage *msg, MsnMsgErrorType error)
{
	MsnSwitchBoard *swboard;

	g_return_if_fail(cmdproc != NULL);
	g_return_if_fail(msg     != NULL);

	if ((error != MSN_MSG_ERROR_SB) && (msg->nak_cb != NULL))
		msg->nak_cb(msg, msg->ack_data);

	swboard = cmdproc->data;

	/* This is not good, and should be fixed somewhere else. */
	g_return_if_fail(swboard != NULL);

	if (msg->type == MSN_MSG_TEXT)
	{
		const char *format, *str_reason;
		char *body_str, *body_enc, *pre, *post;

#if 0
		if (swboard->conv == NULL)
		{
			if (msg->ack_ref)
				msn_message_unref(msg);

			return;
		}
#endif

		if (error == MSN_MSG_ERROR_TIMEOUT)
		{
			str_reason = _("Message may have not been sent "
						   "because a timeout occurred:");
		}
		else if (error == MSN_MSG_ERROR_SB)
		{
			switch (swboard->error)
			{
				case MSN_SB_ERROR_OFFLINE:
					str_reason = _("Message could not be sent, "
								   "not allowed while invisible:");
					break;
				case MSN_SB_ERROR_USER_OFFLINE:
					str_reason = _("Message could not be sent "
								   "because the user is offline:");
					break;
				case MSN_SB_ERROR_CONNECTION:
					str_reason = _("Message could not be sent "
								   "because a connection error occurred:");
					break;
				case MSN_SB_ERROR_TOO_FAST:
					str_reason = _("Message could not be sent "
								   "because we are sending too quickly:");
					break;					
				default:
					str_reason = _("Message could not be sent "
								   "because an error with "
								   "the switchboard occurred:");
					break;
			}
		}
		else
		{
			str_reason = _("Message may have not been sent "
						   "because an unknown error occurred:");
		}

		body_str = msn_message_to_string(msg);
		body_enc = g_markup_escape_text(body_str, -1);
		g_free(body_str);

		format = msn_message_get_attr(msg, "X-MMS-IM-Format");
		msn_parse_format(format, &pre, &post);
		body_str = g_strdup_printf("%s%s%s", pre ? pre : "",
								   body_enc ? body_enc : "", post ? post : "");
		g_free(body_enc);
		g_free(pre);
		g_free(post);

		msn_switchboard_report_user(swboard, PURPLE_MESSAGE_ERROR,
									str_reason);
		msn_switchboard_report_user(swboard, PURPLE_MESSAGE_RAW,
									body_str);

		g_free(body_str);
	}

	/* If a timeout occures we will want the msg around just in case we
	 * receive the ACK after the timeout. */
	if (msg->ack_ref && error != MSN_MSG_ERROR_TIMEOUT)
	{
		swboard->ack_list = g_list_remove(swboard->ack_list, msg);
		msn_message_unref(msg);
	}
}

/**************************************************************************
 * Message Stuff
 **************************************************************************/

/** Called when a message times out. */
static void
msg_timeout(MsnCmdProc *cmdproc, MsnTransaction *trans)
{
	MsnMessage *msg;

	msg = trans->data;

	msg_error_helper(cmdproc, msg, MSN_MSG_ERROR_TIMEOUT);
}

/** Called when we receive an error of a message. */
static void
msg_error(MsnCmdProc *cmdproc, MsnTransaction *trans, int error)
{
	msg_error_helper(cmdproc, trans->data, MSN_MSG_ERROR_UNKNOWN);
}

#if 0
/** Called when we receive an ack of a special message. */
static void
msg_ack(MsnCmdProc *cmdproc, MsnCommand *cmd)
{
	MsnMessage *msg;

	msg = cmd->trans->data;

	if (msg->ack_cb != NULL)
		msg->ack_cb(msg->ack_data);

	msn_message_unref(msg);
}

/** Called when we receive a nak of a special message. */
static void
msg_nak(MsnCmdProc *cmdproc, MsnCommand *cmd)
{
	MsnMessage *msg;

	msg = cmd->trans->data;

	msn_message_unref(msg);
}
#endif

static void
release_msg(MsnSwitchBoard *swboard, MsnMessage *msg)
{
	MsnCmdProc *cmdproc;
	MsnTransaction *trans;
	char *payload;
	gsize payload_len;

	g_return_if_fail(swboard != NULL);
	g_return_if_fail(msg     != NULL);

	cmdproc = swboard->cmdproc;

	payload = msn_message_gen_payload(msg, &payload_len);

#ifdef MSN_DEBUG_SB
	purple_debug_info("MaYuan","SB length:{%d}",payload_len);
	msn_message_show_readable(msg, "SB SEND", FALSE);
#endif

	trans = msn_transaction_new(cmdproc, "MSG", "%c %d",
								msn_message_get_flag(msg), payload_len);

	/* Data for callbacks */
	msn_transaction_set_data(trans, msg);

	if (msg->type == MSN_MSG_TEXT)
	{
		msg->ack_ref = TRUE;
		msn_message_ref(msg);
		swboard->ack_list = g_list_append(swboard->ack_list, msg);
		msn_transaction_set_timeout_cb(trans, msg_timeout);
	}
	else if (msg->type == MSN_MSG_SLP)
	{
		msg->ack_ref = TRUE;
		msn_message_ref(msg);
		swboard->ack_list = g_list_append(swboard->ack_list, msg);
		msn_transaction_set_timeout_cb(trans, msg_timeout);
#if 0
		if (msg->ack_cb != NULL)
		{
			msn_transaction_add_cb(trans, "ACK", msg_ack);
			msn_transaction_add_cb(trans, "NAK", msg_nak);
		}
#endif
	}

	trans->payload = payload;
	trans->payload_len = payload_len;

	msg->trans = trans;

	msn_cmdproc_send_trans(cmdproc, trans);
}

static void
queue_msg(MsnSwitchBoard *swboard, MsnMessage *msg)
{
	g_return_if_fail(swboard != NULL);
	g_return_if_fail(msg     != NULL);

	purple_debug_info("msn", "Appending message to queue.\n");

	g_queue_push_tail(swboard->msg_queue, msg);

	msn_message_ref(msg);
}

static void
process_queue(MsnSwitchBoard *swboard)
{
	MsnMessage *msg;

	g_return_if_fail(swboard != NULL);

	purple_debug_info("msn", "Processing queue\n");

	while ((msg = g_queue_pop_head(swboard->msg_queue)) != NULL)
	{
		purple_debug_info("msn", "Sending message\n");
		release_msg(swboard, msg);
		msn_message_unref(msg);
	}
}

gboolean
msn_switchboard_can_send(MsnSwitchBoard *swboard)
{
	g_return_val_if_fail(swboard != NULL, FALSE);

	if (swboard->empty || !g_queue_is_empty(swboard->msg_queue))
		return FALSE;

	return TRUE;
}

void
msn_switchboard_send_msg(MsnSwitchBoard *swboard, MsnMessage *msg,
						 gboolean queue)
{
	g_return_if_fail(swboard != NULL);
	g_return_if_fail(msg     != NULL);

	purple_debug_info("Ma Yuan","switchboard send msg..\n");
	if (msn_switchboard_can_send(swboard))
		release_msg(swboard, msg);
	else if (queue)
		queue_msg(swboard, msg);
}

/**************************************************************************
 * Switchboard Commands
 **************************************************************************/

static void
ans_cmd(MsnCmdProc *cmdproc, MsnCommand *cmd)
{
	MsnSwitchBoard *swboard;

	swboard = cmdproc->data;
	swboard->ready = TRUE;
}

static void
bye_cmd(MsnCmdProc *cmdproc, MsnCommand *cmd)
{
	MsnSwitchBoard *swboard;
	const char *user;

	swboard = cmdproc->data;
	user = cmd->params[0];

	/* cmdproc->data is set to NULL when the switchboard is destroyed;
	 * we may get a bye shortly thereafter. */
	g_return_if_fail(swboard != NULL);

	if (!(swboard->flag & MSN_SB_FLAG_IM) && (swboard->conv != NULL))
		purple_debug_error("msn_switchboard", "bye_cmd: helper bug\n");

	if (swboard->conv == NULL)
	{
		/* This is a helper switchboard */
		msn_switchboard_destroy(swboard);
	}
	else if ((swboard->current_users > 1) ||
			 (purple_conversation_get_type(swboard->conv) == PURPLE_CONV_TYPE_CHAT))
	{
		/* This is a switchboard used for a chat */
		purple_conv_chat_remove_user(PURPLE_CONV_CHAT(swboard->conv), user, NULL);
		swboard->current_users--;
		if (swboard->current_users == 0)
			msn_switchboard_destroy(swboard);
	}
	else
	{
		/* This is a switchboard used for a im session */
		msn_switchboard_destroy(swboard);
	}
}

static void
iro_cmd(MsnCmdProc *cmdproc, MsnCommand *cmd)
{
	PurpleAccount *account;
	PurpleConnection *gc;
	MsnSwitchBoard *swboard;

	account = cmdproc->session->account;
	gc = account->gc;
	swboard = cmdproc->data;

	swboard->total_users = atoi(cmd->params[2]);

	msn_switchboard_add_user(swboard, cmd->params[3]);
}

static void
joi_cmd(MsnCmdProc *cmdproc, MsnCommand *cmd)
{
	MsnSession *session;
	PurpleAccount *account;
	PurpleConnection *gc;
	MsnSwitchBoard *swboard;
	const char *passport;

	passport = cmd->params[0];

	session = cmdproc->session;
	account = session->account;
	gc = account->gc;
	swboard = cmdproc->data;

	msn_switchboard_add_user(swboard, passport);

	process_queue(swboard);

	if (!session->http_method)
		send_clientcaps(swboard);

	if (swboard->closed)
		msn_switchboard_close(swboard);
}

static void
msg_cmd_post(MsnCmdProc *cmdproc, MsnCommand *cmd, char *payload, size_t len)
{
	MsnMessage *msg;

	msg = msn_message_new_from_cmd(cmdproc->session, cmd);

	msg = msn_message_parse_payload(msg, payload, len, MSG_LINE_DEM,MSG_BODY_DEM);
	if (!msg) return;
#ifdef MSN_DEBUG_SB
	msn_message_show_readable(msg, "SB RECV", FALSE);
#endif

	if (msg->remote_user != NULL)
		g_free (msg->remote_user);

	msg->remote_user = g_strdup(cmd->params[0]);
	msn_cmdproc_process_msg(cmdproc, msg);

	msn_message_destroy(msg);
}

static void
msg_cmd(MsnCmdProc *cmdproc, MsnCommand *cmd)
{
	cmdproc->servconn->payload_len = atoi(cmd->params[2]);
	cmdproc->last_cmd->payload_cb = msg_cmd_post;
}

static void
ubm_cmd(MsnCmdProc *cmdproc, MsnCommand *cmd)
{
	purple_debug_misc("MaYuan","get UBM...\n");
	cmdproc->servconn->payload_len = atoi(cmd->params[4]);
	cmdproc->last_cmd->payload_cb = msg_cmd_post;
}

static void
nak_cmd(MsnCmdProc *cmdproc, MsnCommand *cmd)
{
	MsnMessage *msg;

	msg = cmd->trans->data;
	g_return_if_fail(msg != NULL);

	msg_error_helper(cmdproc, msg, MSN_MSG_ERROR_NAK);
}

static void
ack_cmd(MsnCmdProc *cmdproc, MsnCommand *cmd)
{
	MsnSwitchBoard *swboard;
	MsnMessage *msg;

	msg = cmd->trans->data;

	if (msg->ack_cb != NULL)
		msg->ack_cb(msg, msg->ack_data);

	swboard = cmdproc->data;
	swboard->ack_list = g_list_remove(swboard->ack_list, msg);
	msn_message_unref(msg);
}

static void
out_cmd(MsnCmdProc *cmdproc, MsnCommand *cmd)
{
	PurpleConnection *gc;
	MsnSwitchBoard *swboard;

	gc = cmdproc->session->account->gc;
	swboard = cmdproc->data;

	if (swboard->current_users > 1)
		serv_got_chat_left(gc, swboard->chat_id);

	msn_switchboard_disconnect(swboard);
}

static void
usr_cmd(MsnCmdProc *cmdproc, MsnCommand *cmd)
{
	MsnSwitchBoard *swboard;

	swboard = cmdproc->data;

#if 0
	GList *l;

	for (l = swboard->users; l != NULL; l = l->next)
	{
		const char *user;
		user = l->data;

		msn_cmdproc_send(cmdproc, "CAL", "%s", user);
	}
#endif

	swboard->ready = TRUE;
	msn_cmdproc_process_queue(cmdproc);
}

/**************************************************************************
 * Message Handlers
 **************************************************************************/
static void
plain_msg(MsnCmdProc *cmdproc, MsnMessage *msg)
{
	PurpleConnection *gc;
	MsnSwitchBoard *swboard;
	const char *body;
	char *body_str;
	char *body_enc;
	char *body_final;
	size_t body_len;
	const char *passport;
	const char *value;

	gc = cmdproc->session->account->gc;
	swboard = cmdproc->data;

	body = msn_message_get_bin_data(msg, &body_len);
	body_str = g_strndup(body, body_len);
	body_enc = g_markup_escape_text(body_str, -1);
	g_free(body_str);

	passport = msg->remote_user;

	if (!strcmp(passport, "messenger@microsoft.com") &&
		strstr(body, "immediate security update"))
	{
		return;
	}

#if 0
	if ((value = msn_message_get_attr(msg, "User-Agent")) != NULL)
	{
		purple_debug_misc("msn", "User-Agent = '%s'\n", value);
	}
#endif

	if ((value = msn_message_get_attr(msg, "X-MMS-IM-Format")) != NULL)
	{
		char *pre, *post;

		msn_parse_format(value, &pre, &post);

		body_final = g_strdup_printf("%s%s%s", pre ? pre : "",
									 body_enc ? body_enc : "", post ? post : "");

		g_free(pre);
		g_free(post);
		g_free(body_enc);
	}
	else
	{
		body_final = body_enc;
	}

	swboard->flag |= MSN_SB_FLAG_IM;

	if (swboard->current_users > 1 ||
		((swboard->conv != NULL) &&
		 purple_conversation_get_type(swboard->conv) == PURPLE_CONV_TYPE_CHAT))
	{
		/* If current_users is always ok as it should then there is no need to
		 * check if this is a chat. */
		if (swboard->current_users <= 1)
			purple_debug_misc("msn", "plain_msg: current_users(%d)\n",
							swboard->current_users);

		serv_got_chat_in(gc, swboard->chat_id, passport, 0, body_final,
						 time(NULL));
		if (swboard->conv == NULL)
		{
			swboard->conv = purple_find_chat(gc, swboard->chat_id);
			swboard->flag |= MSN_SB_FLAG_IM;
		}
	}
	else
	{
		serv_got_im(gc, passport, body_final, 0, time(NULL));
		if (swboard->conv == NULL)
		{
			swboard->conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM,
									passport, purple_connection_get_account(gc));
			swboard->flag |= MSN_SB_FLAG_IM;
		}
	}

	g_free(body_final);
}

void
msn_handwritten_msg_show(MsnSwitchBoard *swboard, char* msgid, char* data, char* passport)
{
	guchar *guc;
	size_t body_len;
	PurpleAccount *account;
	
	account = swboard->session->account;
	guc = purple_base64_decode(data, &body_len);
	if (!guc || !body_len) 
		return;
	
	/*Grab the convo for this sboard. If there isn't one and it's an IM 
		then create it, otherwise the smileys won't work*/
	if (swboard->conv == NULL) {
		if (swboard->current_users > 1) 
			swboard->conv = purple_find_chat(account->gc, swboard->chat_id);
		else {
			swboard->conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM,
									passport, account);
			if (swboard->conv == NULL)
				swboard->conv = purple_conversation_new(PURPLE_CONV_TYPE_IM, account, passport);
		}
	}
	swboard->flag |= MSN_SB_FLAG_IM;

	/*Create the image as a custom smiley in the conversation with the GUID of the message that 
		the image arrived in as the smiley text.*/
	if (purple_conv_custom_smiley_add(swboard->conv, msgid, 0, "", 0)) {
		purple_conv_custom_smiley_write(swboard->conv, msgid, guc, body_len);
		purple_conv_custom_smiley_close(swboard->conv, msgid);
	}
	
	/*And put the GUID into the convo window so that it will display...*/
	if (swboard->current_users > 1 ||
		((swboard->conv != NULL) &&
		 purple_conversation_get_type(swboard->conv) == PURPLE_CONV_TYPE_CHAT))
		serv_got_chat_in(account->gc, swboard->chat_id, passport, 0, msgid,
						 time(NULL));
	else
		serv_got_im(account->gc, passport, msgid, 0, time(NULL));

	g_free(guc);
}

/*only called from chats. Handwritten messages for IMs come as a SLP message*/
static void
msn_handwritten_msg(MsnSession *session, MsnCmdProc *cmdproc, MsnMessage *msg)
{
	char *passport;
	char *body, *msgid;
	size_t body_len;
	
	passport = msg->remote_user;
	msgid = msn_message_get_attr(msg, "Message-ID");

	purple_debug_misc("MSN", "Displaying handwritten message from message %s", msgid);

	body = msn_message_get_bin_data(msg, &body_len);
	body = g_strndup(body+7, body_len-7);
	msn_handwritten_msg_show(cmdproc->data, msgid, body, passport);
	g_free(body);
}

static void
control_msg(MsnCmdProc *cmdproc, MsnMessage *msg)
{
	PurpleConnection *gc;
	MsnSwitchBoard *swboard;
	char *passport;

	gc = cmdproc->session->account->gc;
	swboard = cmdproc->data;
	passport = msg->remote_user;

	if (swboard->current_users == 1 &&
		msn_message_get_attr(msg, "TypingUser") != NULL)
	{
		serv_got_typing(gc, passport, MSN_TYPING_RECV_TIMEOUT,
						PURPLE_TYPING);
	}
}

static void
clientcaps_msg(MsnCmdProc *cmdproc, MsnMessage *msg)
{
#if 0
	MsnSession *session;
	MsnSwitchBoard *swboard;
	MsnUser *user;
	GHashTable *clientcaps;
	const char *value;

	char *passport = msg->sender;

	session = cmdproc->session;
	swboard = cmdproc->servconn->swboard;

	clientcaps = msn_message_get_hashtable_from_body(msg);
#endif
}

static void
got_datacast_inform_user(MsnSwitchBoard *swboard, const char *who, const char *msg, 
							const char *emot, const char *emot_name, 
							size_t emot_len)
{
	char *username, *str;
	PurpleAccount *account;
	PurpleBuddy *b;

	account = swboard->session->account;

	if ((b = purple_find_buddy(account, who)) != NULL)
		username = g_markup_escape_text(purple_buddy_get_alias(b), -1);
	else username = g_markup_escape_text(who, -1);
	str = g_strdup_printf("%s %s", username, msg);
	g_free(username);
	
	/*Grab the convo for this sboard. If there isn't one and it's an IM 
		then create it, otherwise the smileys won't work*/
	if (swboard->conv == NULL) {
		if (swboard->current_users > 1) 
			swboard->conv = purple_find_chat(account->gc, swboard->chat_id);
		else {
			swboard->conv = purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM,
									who, account);
			if (swboard->conv == NULL)
				swboard->conv = purple_conversation_new(PURPLE_CONV_TYPE_IM, account, who);
		}
	}
	swboard->flag |= MSN_SB_FLAG_IM;
	
	if (emot && purple_conv_custom_smiley_add(swboard->conv, emot_name, 0, "", 0)) {
		purple_conv_custom_smiley_write(swboard->conv, emot_name, emot, emot_len);
		purple_conv_custom_smiley_close(swboard->conv, emot_name);
	}
	purple_conversation_write(swboard->conv, NULL, str, PURPLE_MESSAGE_SYSTEM, time(NULL));
	g_free(str);

}
static void
got_voice_clip(MsnSlpCall *slpcall,
			 const guchar *data, gsize size)
{
	FILE *f;
	char *path, *msg;
	purple_debug_info("msn-extras", "Got voice clip: %s\n", slpcall->data_info);
	if (!purple_account_get_bool(slpcall->slplink->session->account, "voice_clips", FALSE)) return;
	if ((f = purple_mkstemp(&path, TRUE))) {
		purple_debug_info("msn-extras", "Opened file: %s\n", path);
		fwrite(data, size, 1, f);
		fclose(f);
		msg = g_strdup_printf(
						" sent a voice clip. <a href='audio://%s'>Click here to play it</a>", path);
		got_datacast_inform_user(slpcall->slplink->swboard, slpcall->slplink->remote_user, 
																				msg, NULL, NULL, 0);
		g_free(msg);
	} else {
		purple_debug_error("msn-extras", "Couldn\'t create temp file to store sound\n");
		got_datacast_inform_user(slpcall->slplink->swboard, slpcall->slplink->remote_user, 
			 " sent a voice clip, but it could not be played", NULL, NULL, 0);
	}
	if (path) g_free(path); 
}
static gboolean 
extract_wink (MsnSlpCall *slpcall, const guchar *data, gsize size) {
#ifdef USE_MSPACK
	struct mscab_decompressor *dec; 
	struct mscabd_cabinet *cab; 
	struct mscabd_file *fileincab;
	FILE *f;
	char *msg, *swf_msg, *emot_name, *emot;
	size_t emot_len;
	const gchar *tmpdir;
	char *swf_path, *img_path, *html_path;
	char *path, *craff;
	if (!(f = purple_mkstemp(&path, TRUE))) {
		purple_debug_info("msn-extras", "couldn\'t open temp file for .cab image\n");
		return FALSE;
	}
	fwrite(data, size, 1, f);
	fclose(f);
	if (!(dec = mspack_create_cab_decompressor(NULL))) {
		purple_debug_info("msn-extras", "couldn\'t create decompressor\n");
		return FALSE;
	}
	if (!(cab = dec->open(dec, path))) {
		purple_debug_info("msn-extras", "couldn\'t open .cab file\n");
		return FALSE;
	}
	tmpdir = (gchar*)g_get_tmp_dir();
	fileincab = cab->files;
	swf_path = img_path = NULL;
	while (fileincab) {
		craff = g_build_filename(tmpdir, fileincab->filename, NULL);
		dec->extract(dec, fileincab, craff);
		if (strstr(fileincab->filename, ".swf")) swf_path = craff;
		else if (strstr(fileincab->filename, ".png") || strstr(fileincab->filename, ".jpg") || 
					strstr(fileincab->filename, ".gif"))
			img_path = craff;
		else g_free(craff);
		fileincab = fileincab->next;
	}
	/* don't g_free(tmpdir) - it's just a ref to a global */
	purple_debug_info("CHRIS", "Listed files\n");
	dec->close(dec, cab);
	mspack_destroy_cab_decompressor(dec);
	g_free(path); 
	purple_debug_info("CHRIS", "swf_path %s\n", swf_path);
	emot_name = swf_msg = NULL;
	if (swf_path) {
		if ((f = purple_mkstemp(&html_path, FALSE))) {
			g_fprintf(f, WINK_HTML_TEMPLATE, swf_path, swf_path, img_path);
			fclose(f);
			swf_msg = g_strdup_printf(
						"<a href=\"file://%s\">Click here to view the wink in your web browser</a>", 
						html_path);
			g_free(html_path);
		} else 
			swf_msg = g_strdup_printf(
						"<a href=\"file://%s\">Click here to view the wink in your web browser</a>", 
						swf_path);
	} 
	emot_name = NULL;
	if (img_path) {
		if (g_file_get_contents(img_path, &emot, &emot_len, NULL)) {
			emot_name = g_strdup_printf("{IMAGE:%s}", img_path);
		} else {
			emot=NULL;
		}
	}
	if (emot_name) msg = g_strdup_printf(" sent a wink: %s\n%s", emot_name, swf_msg);
	else msg = g_strdup_printf(" sent a wink\n%s", swf_msg);
	got_datacast_inform_user(slpcall->slplink->swboard, slpcall->slplink->remote_user, 
							msg, emot, emot_name, emot_len);
	g_free(emot_name); 
	/* Blows: probably the smiley code doesn't copy it.. g_free(emot); */
	g_free(msg); g_free(swf_msg);	g_free(img_path); g_free(swf_path);
	return TRUE;
#else
	return FALSE;
#endif
}
static void
got_wink(MsnSlpCall *slpcall, const guchar *data, gsize size)
{
	char* sound;
	if (!purple_account_get_bool(slpcall->slplink->session->account, "winks", FALSE)) return;
	if (!(extract_wink (slpcall, data, size)))
		got_datacast_inform_user(slpcall->slplink->swboard, slpcall->slplink->remote_user,  
			" sent a wink, but it could not be displayed", NULL, NULL, 0);
}

static void
datacast_msg(MsnCmdProc *cmdproc, MsnMessage *msg)
{
	char *body, *body_str, *id, *tmp, *datapos;
	size_t body_len;
	MsnSlpCb datacast_cb;
	
	body = msn_message_get_bin_data(msg, &body_len);
	body_str = g_strndup(body, body_len);
	id = strstr(body_str, "ID:")+4; 
	datapos = strstr(body_str, "Data: ")+6;
	tmp = strchr(id, '\r'); 
	if (tmp) id = g_strndup(id, tmp-id);
	datacast_cb = NULL;
	if (!strcmp(id, "2") && 
#ifdef USE_MSPACK
			purple_account_get_bool(cmdproc->servconn->session->account, "winks", TRUE)) 
#else
			purple_account_get_bool(cmdproc->servconn->session->account, "winks", FALSE)) 
#endif
		datacast_cb = got_wink;
	else if (!strcmp(id, "3") &&
			purple_account_get_bool(cmdproc->servconn->session->account, "voice_clips", TRUE)) 
		datacast_cb = got_voice_clip;

	if (datacast_cb) {
		MsnSession *session;
		MsnSlpLink *slplink;
		MsnObject *obj;
		const char *who;

		session = cmdproc->servconn->session;

		obj = msn_object_new_from_string(datapos);
		who = msn_object_get_creator(obj);

		slplink = msn_session_get_slplink(session, who);
		msn_slplink_request_object(slplink, datapos, datacast_cb, NULL, obj);
			
		msn_object_destroy(obj);
	}
	else
	{
		MsnSwitchBoard *swboard;
		char *username, *str;
		PurpleAccount *account;
		PurpleBuddy *buddy;
		const char *user;

		swboard = cmdproc->data;
		account = cmdproc->session->account;
		user = msg->remote_user;

		if ((buddy = purple_find_buddy(account, user)) != NULL)
			username = g_markup_escape_text(purple_buddy_get_alias(buddy), -1);
		else
			username = g_markup_escape_text(user, -1);

		str = g_strdup_printf(_("%s just sent you a Nudge!"), username);
		g_free(username);
		msn_switchboard_report_user(swboard, PURPLE_MESSAGE_SYSTEM, str);
		g_free(str);
	}
	
	if (tmp) g_free(id);
	g_free(body_str);
}

/**************************************************************************
 * Connect stuff
 **************************************************************************/
static void
connect_cb(MsnServConn *servconn)
{
	MsnSwitchBoard *swboard;
	MsnCmdProc *cmdproc;
	PurpleAccount *account;

	cmdproc = servconn->cmdproc;
	g_return_if_fail(cmdproc != NULL);

	account = cmdproc->session->account;
	swboard = cmdproc->data;
	g_return_if_fail(swboard != NULL);

	if (msn_switchboard_is_invited(swboard))
	{
		swboard->empty = FALSE;

		msn_cmdproc_send(cmdproc, "ANS", "%s %s %s",
						 purple_account_get_username(account),
						 swboard->auth_key, swboard->session_id);
	}
	else
	{
		msn_cmdproc_send(cmdproc, "USR", "%s %s",
						 purple_account_get_username(account),
						 swboard->auth_key);
	}
}

static void
disconnect_cb(MsnServConn *servconn)
{
	MsnSwitchBoard *swboard;

	swboard = servconn->cmdproc->data;
	g_return_if_fail(swboard != NULL);

	msn_servconn_set_disconnect_cb(swboard->servconn, NULL);

	msn_switchboard_destroy(swboard);
}

gboolean
msn_switchboard_connect(MsnSwitchBoard *swboard, const char *host, int port)
{
	g_return_val_if_fail(swboard != NULL, FALSE);

	msn_servconn_set_connect_cb(swboard->servconn, connect_cb);
	msn_servconn_set_disconnect_cb(swboard->servconn, disconnect_cb);

	return msn_servconn_connect(swboard->servconn, host, port);
}

void
msn_switchboard_disconnect(MsnSwitchBoard *swboard)
{
	g_return_if_fail(swboard != NULL);

	msn_servconn_disconnect(swboard->servconn);
}

/**************************************************************************
 * Call stuff
 **************************************************************************/
static void
got_cal(MsnCmdProc *cmdproc, MsnCommand *cmd)
{
#if 0
	MsnSwitchBoard *swboard;
	const char *user;

	swboard = cmdproc->data;

	user = cmd->params[0];

	msn_switchboard_add_user(swboard, user);
#endif
}

static void
cal_timeout(MsnCmdProc *cmdproc, MsnTransaction *trans)
{
	purple_debug_warning("msn", "cal_timeout: command %s timed out\n", trans->command);

	cal_error_helper(trans, MSN_SB_ERROR_UNKNOWN);
}

static void
cal_error(MsnCmdProc *cmdproc, MsnTransaction *trans, int error)
{
	int reason = MSN_SB_ERROR_UNKNOWN;
	MsnMessage *msg;
	MsnSwitchBoard *swboard = trans->data;

	if (error == 215)
	{
		purple_debug_info("msn", "Invited user already in switchboard\n");
		return;
	}
	else if (error == 217)
	{
		reason = MSN_SB_ERROR_USER_OFFLINE;
	}

	purple_debug_warning("msn", "cal_error: command %s gave error %i\n", trans->command, error);
	purple_debug_warning("msn", "Will Use Offline Message to sendit\n");

//	cal_error_helper(trans, reason);
	/*offline Message send Process*/

	while ((msg = g_queue_pop_head(swboard->msg_queue)) != NULL){
		purple_debug_warning("Ma Yuan","offline msg to send:{%s}\n",msg->body);
		/* The messages could not be sent due to a switchboard error */
		swboard->error = MSN_SB_ERROR_USER_OFFLINE;
		msg_error_helper(swboard->cmdproc, msg,
							 MSN_MSG_ERROR_SB);
		msn_message_unref(msg);
	}
	cal_error_helper(trans, reason);
}

void
msn_switchboard_request_add_user(MsnSwitchBoard *swboard, const char *user)
{
	MsnTransaction *trans;
	MsnCmdProc *cmdproc;

	g_return_if_fail(swboard != NULL);

	cmdproc = swboard->cmdproc;

	trans = msn_transaction_new(cmdproc, "CAL", "%s", user);
	/* this doesn't do anything, but users seem to think that
	 * 'Unhandled command' is some kind of error, so we don't report it */
	msn_transaction_add_cb(trans, "CAL", got_cal);

	msn_transaction_set_data(trans, swboard);
	msn_transaction_set_timeout_cb(trans, cal_timeout);

	if (swboard->ready)
		msn_cmdproc_send_trans(cmdproc, trans);
	else
		msn_cmdproc_queue_trans(cmdproc, trans);
}

/**************************************************************************
 * Create & Transfer stuff
 **************************************************************************/

static void
got_swboard(MsnCmdProc *cmdproc, MsnCommand *cmd)
{
	MsnSwitchBoard *swboard;
	char *host;
	int port;
	swboard = cmd->trans->data;

	if (g_list_find(cmdproc->session->switches, swboard) == NULL)
		/* The conversation window was closed. */
		return;

	purple_debug_info("MaYuan","Switchboard:auth:{%s} socket:{%s}\n",cmd->params[4],cmd->params[2]);
	msn_switchboard_set_auth_key(swboard, cmd->params[4]);

	msn_parse_socket(cmd->params[2], &host, &port);

	if (!msn_switchboard_connect(swboard, host, port))
		msn_switchboard_destroy(swboard);

	g_free(host);
}

static void
xfr_error(MsnCmdProc *cmdproc, MsnTransaction *trans, int error)
{
	MsnSwitchBoard *swboard;
	int reason = MSN_SB_ERROR_UNKNOWN;

	if (error == 913)
		reason = MSN_SB_ERROR_OFFLINE;
	else if (error == 800)
		reason = MSN_SB_ERROR_TOO_FAST;

	swboard = trans->data;

	purple_debug_info("msn", "xfr_error %i for %s: trans %x, command %s, reason %i\n",
					error, (swboard->im_user ? swboard->im_user : "(null)"), trans,
					(trans->command ? trans->command : "(null)"), reason);

	swboard_error_helper(swboard, reason, swboard->im_user);
}

void
msn_switchboard_request(MsnSwitchBoard *swboard)
{
	MsnCmdProc *cmdproc;
	MsnTransaction *trans;

	g_return_if_fail(swboard != NULL);

	cmdproc = swboard->session->notification->cmdproc;

	trans = msn_transaction_new(cmdproc, "XFR", "%s", "SB");
	msn_transaction_add_cb(trans, "XFR", got_swboard);

	msn_transaction_set_data(trans, swboard);
	msn_transaction_set_error_cb(trans, xfr_error);

	msn_cmdproc_send_trans(cmdproc, trans);
}

void
msn_switchboard_close(MsnSwitchBoard *swboard)
{
	g_return_if_fail(swboard != NULL);

	if (swboard->error != MSN_SB_ERROR_NONE)
	{
		msn_switchboard_destroy(swboard);
	}
	else if (g_queue_is_empty(swboard->msg_queue) ||
			 !swboard->session->connected)
	{
		MsnCmdProc *cmdproc;
		cmdproc = swboard->cmdproc;
		msn_cmdproc_send_quick(cmdproc, "OUT", NULL, NULL);

		msn_switchboard_destroy(swboard);
	}
	else
	{
		swboard->closed = TRUE;
	}
}

gboolean
msn_switchboard_release(MsnSwitchBoard *swboard, MsnSBFlag flag)
{
	g_return_val_if_fail(swboard != NULL, FALSE);

	swboard->flag &= ~flag;

	if (flag == MSN_SB_FLAG_IM)
		/* Forget any conversation that used to be associated with this
		 * swboard. */
		swboard->conv = NULL;

	if (swboard->flag == 0)
	{
		msn_switchboard_close(swboard);
		return TRUE;
	}

	return FALSE;
}

/**************************************************************************
 * Init stuff
 **************************************************************************/

void
msn_switchboard_init(void)
{
	cbs_table = msn_table_new();

	msn_table_add_cmd(cbs_table, "ANS", "ANS", ans_cmd);
	msn_table_add_cmd(cbs_table, "ANS", "IRO", iro_cmd);

	msn_table_add_cmd(cbs_table, "MSG", "ACK", ack_cmd);
	msn_table_add_cmd(cbs_table, "MSG", "NAK", nak_cmd);

	msn_table_add_cmd(cbs_table, "USR", "USR", usr_cmd);

	msn_table_add_cmd(cbs_table, NULL, "MSG", msg_cmd);
	msn_table_add_cmd(cbs_table, NULL, "UBM", ubm_cmd);
	msn_table_add_cmd(cbs_table, NULL, "JOI", joi_cmd);
	msn_table_add_cmd(cbs_table, NULL, "BYE", bye_cmd);
	msn_table_add_cmd(cbs_table, NULL, "OUT", out_cmd);

#if 0
	/* They might skip the history */
	msn_table_add_cmd(cbs_table, NULL, "ACK", NULL);
#endif

	msn_table_add_error(cbs_table, "MSG", msg_error);
	msn_table_add_error(cbs_table, "CAL", cal_error);

	/* Register the message type callbacks. */
	msn_table_add_msg_type(cbs_table, "text/plain",
						   plain_msg);
	msn_table_add_msg_type(cbs_table, "text/x-msmsgscontrol",
						   control_msg);
	msn_table_add_msg_type(cbs_table, "text/x-clientcaps",
						   clientcaps_msg);
	msn_table_add_msg_type(cbs_table, "text/x-clientinfo",
						   clientcaps_msg);
	msn_table_add_msg_type(cbs_table, "application/x-msnmsgrp2p",
						   msn_p2p_msg);
	msn_table_add_msg_type(cbs_table, "text/x-mms-emoticon",
						   msn_emoticon_msg);
	msn_table_add_msg_type(cbs_table, "text/x-mms-animemoticon",
	                                           msn_emoticon_msg);
	msn_table_add_msg_type(cbs_table, "text/x-msnmsgr-datacast",
						   datacast_msg);
	msn_table_add_msg_type(cbs_table, "image/gif",
						   msn_handwritten_msg);
#if 0
	msn_table_add_msg_type(cbs_table, "text/x-msmmsginvite",
						   msn_invite_msg);
#endif
}

void
msn_switchboard_end(void)
{
	msn_table_destroy(cbs_table);
}
