/**
 * @file msn.c The MSN protocol plugin
 *
 * gaim
 *
 * Copyright (C) 2003 Christian Hammond <chipx86@gnupdate.org>
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
#include "accountopt.h"
#include "msg.h"
#include "page.h"
#include "prefs.h"
#include "session.h"
#include "state.h"
#include "utils.h"
#include "multi.h"
#include "html.h"

#define BUDDY_ALIAS_MAXLEN 388

static GaimPlugin *my_protocol = NULL;

static char *msn_normalize(const char *str);

typedef struct
{
	GaimConnection *gc;
	const char *passport;

} MsnMobileData;

static void
msn_act_id(GaimConnection *gc, const char *entry)
{
	MsnSession *session = gc->proto_data;
	GaimAccount *account = gaim_connection_get_account(gc);
	char outparams[MSN_BUF_LEN];
	char *alias;

	if (entry == NULL || *entry == '\0')
		alias = g_strdup("");
	else
		alias = g_strdup(entry);

	if (strlen(alias) >= BUDDY_ALIAS_MAXLEN) {
		gaim_notify_error(gc, NULL,
						  _("Your new MSN friendly name is too long."), NULL);
		return;
	}

	g_snprintf(outparams, sizeof(outparams), "%s %s",
			   gaim_account_get_username(account), msn_url_encode(alias));

	g_free(alias);

	if (!msn_servconn_send_command(session->notification_conn,
								   "REA", outparams)) {

		gaim_connection_error(gc, _("Write error"));
		return;
	}
}

static void
msn_set_prp(GaimConnection *gc, const char *type, const char *entry)
{
	MsnSession *session = gc->proto_data;
	char outparams[MSN_BUF_LEN];

	if (entry == NULL || *entry == '\0')
		g_snprintf(outparams, sizeof(outparams), "%s  ", type);
	else
		g_snprintf(outparams, sizeof(outparams), "%s %s", type,
				   msn_url_encode(entry));

	if (!msn_servconn_send_command(session->notification_conn,
								   "PRP", outparams)) {

		gaim_connection_error(gc, _("Write error"));
		return;
	}
}

static void
msn_set_home_phone_cb(GaimConnection *gc, const char *entry)
{
	msn_set_prp(gc, "PHH", entry);
}

static void
msn_set_work_phone_cb(GaimConnection *gc, const char *entry)
{
	msn_set_prp(gc, "PHW", entry);
}

static void
msn_set_mobile_phone_cb(GaimConnection *gc, const char *entry)
{
	msn_set_prp(gc, "PHM", entry);
}

static void
enable_msn_pages_cb(GaimConnection *gc)
{
	msn_set_prp(gc, "MOB", "Y");
}

static void
disable_msn_pages_cb(GaimConnection *gc)
{
	msn_set_prp(gc, "MOB", "N");
}

static void
send_to_mobile_cb(MsnMobileData *data, const char *entry)
{
	MsnSession *session = data->gc->proto_data;
	MsnServConn *servconn = session->notification_conn;
	MsnUser *user;
	MsnPage *page;
	char *page_str;

	user = msn_user_new(session, data->passport, NULL);

	page = msn_page_new();
	msn_page_set_receiver(page, user);
	msn_page_set_transaction_id(page, ++session->trId);
	msn_page_set_body(page, entry);

	page_str = msn_page_build_string(page);

	msn_user_destroy(user);
	msn_page_destroy(page);

	if (!msn_servconn_write(servconn, page_str, strlen(page_str))) {

		gaim_connection_error(data->gc, _("Write error"));
	}

	g_free(page_str);
}

static void
close_mobile_page_cb(MsnMobileData *data, const char *entry)
{
	g_free(data);
}

/* -- */

static void
msn_show_set_friendly_name(GaimConnection *gc)
{
	gaim_request_input(gc, NULL, _("Set your friendly name."),
					   _("This is the name that other MSN buddies will "
						 "see you as."),
					   gaim_connection_get_display_name(gc), FALSE, FALSE,
					   _("OK"), G_CALLBACK(msn_act_id),
					   _("Cancel"), NULL, gc);
}

static void
msn_show_set_home_phone(GaimConnection *gc)
{
	MsnSession *session = gc->proto_data;

	gaim_request_input(gc, NULL, _("Set your home phone number."), NULL,
					   msn_user_get_home_phone(session->user), FALSE, FALSE,
					   _("OK"), G_CALLBACK(msn_set_home_phone_cb),
					   _("Cancel"), NULL, gc);
}

static void
msn_show_set_work_phone(GaimConnection *gc)
{
	MsnSession *session = gc->proto_data;

	gaim_request_input(gc, NULL, _("Set your work phone number."), NULL,
					   msn_user_get_work_phone(session->user), FALSE, FALSE,
					   _("OK"), G_CALLBACK(msn_set_work_phone_cb),
					   _("Cancel"), NULL, gc);
}

static void
msn_show_set_mobile_phone(GaimConnection *gc)
{
	MsnSession *session = gc->proto_data;

	gaim_request_input(gc, NULL, _("Set your mobile phone number."), NULL,
					   msn_user_get_mobile_phone(session->user), FALSE, FALSE,
					   _("OK"), G_CALLBACK(msn_set_mobile_phone_cb),
					   _("Cancel"), NULL, gc);
}

static void
msn_show_set_mobile_pages(GaimConnection *gc)
{
	gaim_request_action(gc, NULL, _("Allow MSN Mobile pages?"),
						_("Do you want to allow or disallow people on "
						  "your buddy list to send you MSN Mobile pages "
						  "to your cell phone or other mobile device?"),
						-1, gc, 3,
						_("Allow"), G_CALLBACK(enable_msn_pages_cb),
						_("Disallow"), G_CALLBACK(disable_msn_pages_cb),
						_("Cancel"), NULL);
}

static void
show_send_to_mobile_cb(GaimConnection *gc, const char *passport)
{
	MsnUser *user;
	MsnSession *session = gc->proto_data;
	MsnMobileData *data;

	user = msn_users_find_with_passport(session->users, passport);

	data = g_new0(MsnMobileData, 1);
	data->gc = gc;
	data->passport = passport;

	gaim_request_input(gc, NULL, _("Send a mobile message."), NULL,
					   NULL, TRUE, FALSE,
					   _("Page"), G_CALLBACK(send_to_mobile_cb),
					   _("Close"), G_CALLBACK(close_mobile_page_cb),
					   data);
}

static void
initiate_chat_cb(GaimConnection *gc, const char *passport)
{
	GaimAccount *account = gaim_connection_get_account(gc);
	MsnSession *session = gc->proto_data;
	MsnSwitchBoard *swboard;
	MsnUser *user;

	if ((swboard = msn_session_open_switchboard(session)) == NULL) {
		gaim_connection_error(gc, _("Write error"));

		return;
	}

	user = msn_user_new(session, passport, NULL);

	msn_switchboard_set_user(swboard, user);

	swboard->total_users = 1;

	swboard->chat = serv_got_joined_chat(gc, ++swboard->chat_id, "MSN Chat");

	gaim_chat_add_user(GAIM_CHAT(swboard->chat),
					   gaim_account_get_username(account), NULL);
}

/**************************************************************************
 * Protocol Plugin ops
 **************************************************************************/

static const char *
msn_list_icon(GaimAccount *a, GaimBuddy *b)
{
	return "msn";
}

static void
msn_list_emblems(GaimBuddy *b, char **se, char **sw,
				 char **nw, char **ne)
{
	MsnUser *user;
	char *emblems[4] = { NULL, NULL, NULL, NULL };
	int away_type = MSN_AWAY_TYPE(b->uc);
	int i = 0;

	user = b->proto_data;

	if (b->present == GAIM_BUDDY_OFFLINE)
		emblems[i++] = "offline";
	else if (away_type == MSN_BUSY || away_type == MSN_PHONE)
		emblems[i++] = "occupied";
	else if (away_type != 0)
		emblems[i++] = "away";

	if (user == NULL) {
		/* Shouldn't happen. */
		gaim_debug_error("msn", "buddy %s does not have a MsnUser attached!\n",
						 b->name);

		emblems[0] = "offline";
	}
	else if (user->mobile)
		emblems[i++] = "wireless";

	*se = emblems[0];
	*sw = emblems[1];
	*nw = emblems[2];
	*ne = emblems[3];
}

static char *
msn_status_text(GaimBuddy *b)
{
	if (b->uc & UC_UNAVAILABLE)
		return g_strdup(msn_away_get_text(MSN_AWAY_TYPE(b->uc)));

	return NULL;
}

static char *
msn_tooltip_text(GaimBuddy *b)
{
	char *text = NULL;
	/* MsnUser *user = b->proto_data; */

	if (GAIM_BUDDY_IS_ONLINE(b)) {
		text = g_strdup_printf(_("<b>Status:</b> %s"),
							   msn_away_get_text(MSN_AWAY_TYPE(b->uc)));
	}

	return text;
}

static GList *
msn_away_states(GaimConnection *gc)
{
	GList *m = NULL;

	m = g_list_append(m, _("Available"));
	m = g_list_append(m, _("Away From Computer"));
	m = g_list_append(m, _("Be Right Back"));
	m = g_list_append(m, _("Busy"));
	m = g_list_append(m, _("On The Phone"));
	m = g_list_append(m, _("Out To Lunch"));
	m = g_list_append(m, _("Hidden"));

	return m;
}

static GList *
msn_actions(GaimConnection *gc)
{
	GList *m = NULL;
	struct proto_actions_menu *pam;

	pam = g_new0(struct proto_actions_menu, 1);
	pam->label = _("Set Friendly Name");
	pam->callback = msn_show_set_friendly_name;
	pam->gc = gc;
	m = g_list_append(m, pam);

	m = g_list_append(m, NULL);

	pam = g_new0(struct proto_actions_menu, 1);
	pam->label = _("Set Home Phone Number");
	pam->callback = msn_show_set_home_phone;
	pam->gc = gc;
	m = g_list_append(m, pam);

	pam = g_new0(struct proto_actions_menu, 1);
	pam->label = _("Set Work Phone Number");
	pam->callback = msn_show_set_work_phone;
	pam->gc = gc;
	m = g_list_append(m, pam);

	pam = g_new0(struct proto_actions_menu, 1);
	pam->label = _("Set Mobile Phone Number");
	pam->callback = msn_show_set_mobile_phone;
	pam->gc = gc;
	m = g_list_append(m, pam);

	m = g_list_append(m, NULL);

#if 0
	pam = g_new0(struct proto_actions_menu, 1);
	pam->label = _("Enable/Disable Mobile Devices");
	pam->callback = msn_show_set_mobile_support;
	pam->gc = gc;
	m = g_list_append(m, pam);
#endif

	pam = g_new0(struct proto_actions_menu, 1);
	pam->label = _("Allow/Disallow Mobile Pages");
	pam->callback = msn_show_set_mobile_pages;
	pam->gc = gc;
	m = g_list_append(m, pam);

	return m;
}

static GList *
msn_buddy_menu(GaimConnection *gc, const char *who)
{
	GaimAccount *account = gaim_connection_get_account(gc);
	MsnUser *user;
	struct proto_buddy_menu *pbm;
	GaimBuddy *b;
	GList *m = NULL;

	b = gaim_find_buddy(gc->account, who);
	user = b->proto_data;

	if (user != NULL) {
		if (user->mobile) {
			pbm = g_new0(struct proto_buddy_menu, 1);
			pbm->label    = _("Send to Mobile");
			pbm->callback = show_send_to_mobile_cb;
			pbm->gc       = gc;
			m = g_list_append(m, pbm);
		}
	}

	if (g_ascii_strcasecmp(who, gaim_account_get_username(account))) {
		pbm = g_new0(struct proto_buddy_menu, 1);
		pbm->label    = _("Initiate Chat");
		pbm->callback = initiate_chat_cb;
		pbm->gc       = gc;
		m = g_list_append(m, pbm);
	}

	return m;
}

static void
msn_login(GaimAccount *account)
{
	GaimConnection *gc;
	MsnSession *session;
	const char *username;
	const char *server;
	int port;

	server = gaim_account_get_string(account, "server", MSN_SERVER);
	port   = gaim_account_get_int(account,    "port",   MSN_PORT);

	gc = gaim_account_get_connection(account);

	session = msn_session_new(account, server, port);
	session->prpl = my_protocol;

	gc->proto_data = session;

	gaim_connection_update_progress(gc, _("Connecting"), 0, MSN_CONNECT_STEPS);

	/* Hmm, I don't like this. */
	username = msn_normalize(gaim_account_get_username(account));

	if (strcmp(username, gaim_account_get_username(account)))
		gaim_account_set_username(account, username);

	if (!msn_session_connect(session)) {
		gaim_connection_error(gc, _("Unable to connect"));

		return;
	}
}

static void
msn_close(GaimConnection *gc)
{
	MsnSession *session = gc->proto_data;

	msn_session_destroy(session);

	gc->proto_data = NULL;
}

static int
msn_send_im(GaimConnection *gc, const char *who, const char *message,
			int len, GaimImFlags flags)
{
	GaimAccount *account = gaim_connection_get_account(gc);
	MsnSession *session = gc->proto_data;
	MsnSwitchBoard *swboard;

	swboard = msn_session_find_switch_with_passport(session, who);

	if (g_ascii_strcasecmp(who, gaim_account_get_username(account))) {
		MsnMessage *msg;
		MsnUser *user;

		user = msn_user_new(session, who, NULL);

		msg = msn_message_new();
		msn_message_set_receiver(msg, user);
		msn_message_set_attr(msg, "X-MMS-IM-Format",
							 "FN=Arial; EF=; CO=0; PF=0");
		msn_message_set_body(msg, message);

		if (swboard != NULL) {
			if (!msn_switchboard_send_msg(swboard, msg))
				msn_switchboard_destroy(swboard);
		}
		else {
			if ((swboard = msn_session_open_switchboard(session)) == NULL) {
				msn_message_destroy(msg);

				gaim_connection_error(gc, _("Write error"));

				return 1;
			}

			msn_switchboard_set_user(swboard, user);
			msn_switchboard_send_msg(swboard, msg);
		}

		msn_user_destroy(user);
		msn_message_destroy(msg);
	}
	else {
		/*
		 * In MSN, you can't send messages to yourself, so
		 * we'll fake like we received it ;)
		 */
		serv_got_typing_stopped(gc, (char *)who);
		serv_got_im(gc, who, message, flags,
					time(NULL), -1);
	}

	return 1;
}

static int
msn_send_typing(GaimConnection *gc, const char *who, int typing)
{
	GaimAccount *account = gaim_connection_get_account(gc);
	MsnSession *session = gc->proto_data;
	MsnSwitchBoard *swboard;
	MsnMessage *msg;
	MsnUser *user;

	if (!typing)
		return 0;

	if (!g_ascii_strcasecmp(who, gaim_account_get_username(account))) {
		/* We'll just fake it, since we're sending to ourself. */
		serv_got_typing(gc, who, MSN_TYPING_RECV_TIMEOUT, GAIM_TYPING);

		return MSN_TYPING_SEND_TIMEOUT;
	}

	swboard = msn_session_find_switch_with_passport(session, who);

	if (swboard == NULL)
		return 0;

	user = msn_user_new(session, who, NULL);

	msg = msn_message_new();
	msn_message_set_content_type(msg, "text/x-msmsgscontrol");
	msn_message_set_receiver(msg, user);
	msn_message_set_charset(msg, NULL);
	msn_message_set_flag(msg, 'U');
	msn_message_set_attr(msg, "TypingUser",
						 gaim_account_get_username(account));
	msn_message_set_attr(msg, "User-Agent", NULL);
	msn_message_set_body(msg, "\r\n");

	if (!msn_switchboard_send_msg(swboard, msg))
		msn_switchboard_destroy(swboard);

	msn_user_destroy(user);

	return MSN_TYPING_SEND_TIMEOUT;
}

static void
msn_set_away(GaimConnection *gc, const char *state, const char *msg)
{
	MsnSession *session = gc->proto_data;
	const char *away;

	if (gc->away != NULL) {
		g_free(gc->away);
		gc->away = NULL;
	}

	if (msg != NULL) {
		gc->away = g_strdup("");
		away = "AWY";
	}
	else if (state) {
		gc->away = g_strdup("");

		if (!strcmp(state, _("Away From Computer")))
			away = "AWY";
		else if (!strcmp(state, _("Be Right Back")))
			away = "BRB";
		else if (!strcmp(state, _("Busy")))
			away = "BSY";
		else if (!strcmp(state, _("On The Phone")))
			away = "PHN";
		else if (!strcmp(state, _("Out To Lunch")))
			away = "LUN";
		else if (!strcmp(state, _("Hidden")))
			away = "HDN";
		else {
			g_free(gc->away);
			gc->away = NULL;
			away = "NLN";
		}
	}
	else if (gc->is_idle)
		away = "IDL";
	else
		away = "NLN";

	if (!msn_servconn_send_command(session->notification_conn, "CHG", away)) {
		gaim_connection_error(gc, _("Write error"));
		return;
	}
}

static void
msn_set_idle(GaimConnection *gc, int idle)
{
	MsnSession *session = gc->proto_data;

	if (gc->away != NULL)
		return;

	if (!msn_servconn_send_command(session->notification_conn, "CHG",
								   (idle ? "IDL" : "NLN"))) {

		gaim_connection_error(gc, _("Write error"));
		return;
	}
}

static void
msn_add_buddy(GaimConnection *gc, const char *name)
{
	MsnSession *session = gc->proto_data;
	char *who;
	char outparams[MSN_BUF_LEN];
	GSList *l;

	who = msn_normalize(name);

	if (strchr(who, ' ')) {
		/* This is a broken blist entry. */
		return;
	}

	for (l = session->lists.forward; l != NULL; l = l->next) {
		MsnUser *user = l->data;

		if (!gaim_utf8_strcasecmp(who, msn_user_get_passport(user)))
			break;
	}

	if (l != NULL)
		return;

	g_snprintf(outparams, sizeof(outparams),
			   "FL %s %s", who, who);

	if (!msn_servconn_send_command(session->notification_conn,
								   "ADD", outparams)) {
		gaim_connection_error(gc, _("Write error"));
		return;
	}
}

static void
msn_rem_buddy(GaimConnection *gc, const char *who, const char *group_name)
{
	MsnSession *session = gc->proto_data;
	char outparams[MSN_BUF_LEN];
	MsnGroup *group;

	group = msn_groups_find_with_name(session->groups, group_name);

	if (group == NULL)
		g_snprintf(outparams, sizeof(outparams), "FL %s", who);
	else
		g_snprintf(outparams, sizeof(outparams), "FL %s %d", who,
				   msn_group_get_id(group));

	if (!msn_servconn_send_command(session->notification_conn,
								   "REM", outparams)) {

		gaim_connection_error(gc, _("Write error"));
		return;
	}
}

static void
msn_add_permit(GaimConnection *gc, const char *who)
{
	MsnSession *session = gc->proto_data;
	char buf[MSN_BUF_LEN];

	if (!strchr(who, '@')) {
		g_snprintf(buf, sizeof(buf),
			   _("An MSN screenname must be in the form \"user@server.com\". "
			     "Perhaps you meant %s@hotmail.com. No changes were made "
				 "to your allow list."), who);

		gaim_notify_error(gc, NULL, _("Invalid MSN screenname"), buf);
		gaim_privacy_permit_remove(gc->account, who, TRUE);

		return;
	}

	if (g_slist_find_custom(gc->account->deny, who, (GCompareFunc)strcmp)) {
		gaim_debug(GAIM_DEBUG_INFO, "msn", "Moving %s from BL to AL\n", who);
		gaim_privacy_deny_remove(gc->account, who, TRUE);

		g_snprintf(buf, sizeof(buf), "BL %s", who);

		if (!msn_servconn_send_command(session->notification_conn,
									   "REM", buf)) {

			gaim_connection_error(gc, _("Write error"));
			return;
		}
	}

	g_snprintf(buf, sizeof(buf), "AL %s %s", who, who);

	if (!msn_servconn_send_command(session->notification_conn, "ADD", buf)) {
		gaim_connection_error(gc, _("Write error"));
		return;
	}
}

static void
msn_add_deny(GaimConnection *gc, const char *who)
{
	MsnSession *session = gc->proto_data;
	char buf[MSN_BUF_LEN];

	if (!strchr(who, '@')) {
		g_snprintf(buf, sizeof(buf),
			   _("An MSN screenname must be in the form \"user@server.com\". "
			     "Perhaps you meant %s@hotmail.com. No changes were made "
				 "to your block list."), who);

		gaim_notify_error(gc, NULL, _("Invalid MSN screenname"), buf);

		gaim_privacy_deny_remove(gc->account, who, TRUE);

		return;
	}

	if (g_slist_find_custom(gc->account->permit, who, (GCompareFunc)strcmp)) {
		gaim_debug(GAIM_DEBUG_INFO, "msn", "Moving %s from AL to BL\n", who);
		gaim_privacy_permit_remove(gc->account, who, TRUE);

		g_snprintf(buf, sizeof(buf), "AL %s", who);

		if (!msn_servconn_send_command(session->notification_conn,
									   "REM", buf)) {

			gaim_connection_error(gc, _("Write error"));
			return;
		}
	}

	g_snprintf(buf, sizeof(buf), "BL %s %s", who, who);

	if (!msn_servconn_send_command(session->notification_conn, "ADD", buf)) {
		gaim_connection_error(gc, _("Write error"));
		return;
	}
}

static void
msn_rem_permit(GaimConnection *gc, const char *who)
{
	MsnSession *session = gc->proto_data;
	char buf[MSN_BUF_LEN];

	g_snprintf(buf, sizeof(buf), "AL %s", who);

	if (!msn_servconn_send_command(session->notification_conn, "REM", buf)) {
		gaim_connection_error(gc, _("Write error"));
		return;
	}

	gaim_privacy_deny_add(gc->account, who, TRUE);

	g_snprintf(buf, sizeof(buf), "BL %s %s", who, who);

	if (!msn_servconn_send_command(session->notification_conn, "ADD", buf)) {
		gaim_connection_error(gc, _("Write error"));
		return;
	}
}

static void
msn_rem_deny(GaimConnection *gc, const char *who)
{
	MsnSession *session = gc->proto_data;
	char buf[MSN_BUF_LEN];

	g_snprintf(buf, sizeof(buf), "BL %s", who);

	if (!msn_servconn_send_command(session->notification_conn, "REM", buf)) {
		gaim_connection_error(gc, _("Write error"));
		return;
	}

	gaim_privacy_permit_add(gc->account, who, TRUE);
}

static void
msn_set_permit_deny(GaimConnection *gc)
{
	GaimAccount *account = gaim_connection_get_account(gc);
	MsnSession *session = gc->proto_data;
	char buf[MSN_BUF_LEN];
	GSList *s, *t = NULL;

	if (account->perm_deny == PERMIT_ALL ||
		account->perm_deny == DENY_SOME) {

		strcpy(buf, "AL");
	}
	else
		strcpy(buf, "BL");

	if (!msn_servconn_send_command(session->notification_conn, "BLP", buf)) {
		gaim_connection_error(gc, _("Write error"));
		return;
	}

	/*
	 * This is safe because we'll always come here after we've gotten
	 * the list off the server, and data is never removed. So if the
	 * lengths are equal we don't know about anyone locally and so
	 * there's no sense in going through them all.
	 */
	if (g_slist_length(gc->account->permit) ==
		g_slist_length(session->lists.allow)) {

		g_slist_free(session->lists.allow);
		session->lists.allow = NULL;
	}

	if (g_slist_length(gc->account->deny) ==
		g_slist_length(session->lists.block)) {

		g_slist_free(session->lists.block);
		session->lists.block = NULL;
	}

	if (session->lists.allow == NULL && session->lists.block == NULL)
		return;

	if (session->lists.allow != NULL) {

		for (s = g_slist_nth(gc->account->permit,
							 g_slist_length(session->lists.allow));
			 s != NULL;
			 s = s->next) {

			char *who = s->data;

			if (!strchr(who, '@')) {
				t = g_slist_append(t, who);
				continue;
			}

			if (g_slist_find(session->lists.block, who)) {
				t = g_slist_append(t, who);
				continue;
			}

			g_snprintf(buf, sizeof(buf), "AL %s %s", who, who);

			if (!msn_servconn_send_command(session->notification_conn,
										   "ADD", buf)) {
				gaim_connection_error(gc, _("Write error"));
				return;
			}
		}

		for (; t != NULL; t = t->next)
			gaim_privacy_permit_remove(gc->account, t->data, TRUE);

		if (t != NULL)
			g_slist_free(t);

		t = NULL;
		g_slist_free(session->lists.allow);
		session->lists.allow = NULL;
	}

	if (session->lists.block) {
		for (s = g_slist_nth(gc->account->deny,
							 g_slist_length(session->lists.block));
			 s != NULL;
			 s = s->next) {

			char *who = s->data;

			if (!strchr(who, '@')) {
				t = g_slist_append(t, who);
				continue;
			}

			if (g_slist_find(session->lists.block, who)) {
				t = g_slist_append(t, who);
				continue;
			}

			g_snprintf(buf, sizeof(buf), "BL %s %s", who, who);

			if (!msn_servconn_send_command(session->notification_conn,
										   "ADD", buf)) {
				gaim_connection_error(gc, _("Write error"));
				return;
			}
		}

		for (; t != NULL; t = t->next)
			gaim_privacy_deny_remove(gc->account, t->data, TRUE);

		if (t != NULL)
			g_slist_free(t);

		g_slist_free(session->lists.block);
		session->lists.block = NULL;
	}
}

static void
msn_chat_invite(GaimConnection *gc, int id, const char *msg,
				const char *who)
{
	MsnSession *session = gc->proto_data;
	MsnSwitchBoard *swboard = msn_session_find_switch_with_id(session, id);

	if (swboard == NULL)
		return;

	if (!msn_switchboard_send_command(swboard, "CAL", who))
		msn_switchboard_destroy(swboard);
}

static void
msn_chat_leave(GaimConnection *gc, int id)
{
	MsnSession *session = gc->proto_data;
	MsnSwitchBoard *swboard = msn_session_find_switch_with_id(session, id);
	char buf[6];

	if (swboard == NULL) {
		serv_got_chat_left(gc, id);
		return;
	}

	strcpy(buf, "OUT\r\n");

	if (!msn_servconn_write(swboard->servconn, buf, strlen(buf)))
		msn_switchboard_destroy(swboard);

	serv_got_chat_left(gc, id);
}

static int
msn_chat_send(GaimConnection *gc, int id, const char *message)
{
	GaimAccount *account = gaim_connection_get_account(gc);
	MsnSession *session = gc->proto_data;
	MsnSwitchBoard *swboard = msn_session_find_switch_with_id(session, id);
	MsnMessage *msg;
	char *send;

	if (swboard == NULL)
		return -EINVAL;

	send = add_cr(message);

	msg = msn_message_new();
	msn_message_set_attr(msg, "X-MMS-IM-Format", "FN=Arial; EF=; CO=0; PF=0");
	msn_message_set_body(msg, send);

	g_free(send);

	if (!msn_switchboard_send_msg(swboard, msg)) {
		msn_switchboard_destroy(swboard);

		msn_message_destroy(msg);

		return 0;
	}

	msn_message_destroy(msg);

	serv_got_chat_in(gc, id, gaim_account_get_username(account),
					 0, message, time(NULL));

	return 0;
}

static void
msn_keepalive(GaimConnection *gc)
{
	MsnSession *session = gc->proto_data;
	char buf[MSN_BUF_LEN];

	g_snprintf(buf, sizeof(buf), "PNG\r\n");

	if (msn_servconn_write(session->notification_conn,
						   buf, strlen(buf)) < 0) {

		gaim_connection_error(gc, _("Write error"));
		return;
	}
}

static void
msn_group_buddy(GaimConnection *gc, const char *who,
				const char *old_group_name, const char *new_group_name)
{
	MsnSession *session = gc->proto_data;
	MsnUser *user;
	char outparams[MSN_BUF_LEN];
	MsnGroup *old_group, *new_group;

	old_group = msn_groups_find_with_name(session->groups, old_group_name);
	new_group = msn_groups_find_with_name(session->groups, new_group_name);

	user = msn_users_find_with_passport(session->users, who);

	msn_user_remove_group_id(user, msn_group_get_id(old_group));

	if (new_group == NULL) {
		g_snprintf(outparams, sizeof(outparams), "%s 0",
				   msn_url_encode(new_group_name));

		if (!msn_servconn_send_command(session->notification_conn,
									   "ADG", outparams)) {
			gaim_connection_error(gc, _("Write error"));
			return;
		}

		/* I hate this. So much. */
		session->moving_buddy    = TRUE;
		session->dest_group_name = g_strdup(new_group_name);
		session->old_group       = old_group;

		session->moving_user =
			msn_users_find_with_passport(session->users, who);

		msn_user_ref(session->moving_user);
	}
	else {
		g_snprintf(outparams, sizeof(outparams), "FL %s %s %d",
				   who, who, msn_group_get_id(new_group));

		if (!msn_servconn_send_command(session->notification_conn,
									   "ADD", outparams)) {
			gaim_connection_error(gc, _("Write error"));
			return;
		}
	}

	if (old_group != NULL) {
		g_snprintf(outparams, sizeof(outparams), "FL %s %d",
				   who, msn_group_get_id(old_group));

		if (!msn_servconn_send_command(session->notification_conn,
									   "REM", outparams)) {
			gaim_connection_error(gc, _("Write error"));
			return;
		}

		if (msn_users_get_count(msn_group_get_users(old_group)) <= 0) {
			g_snprintf(outparams, sizeof(outparams), "%d",
					   msn_group_get_id(old_group));

			if (!msn_servconn_send_command(session->notification_conn,
										   "RMG", outparams)) {

				gaim_connection_error(gc, _("Write error"));
				return;
			}
		}
	}
}

static void
msn_rename_group(GaimConnection *gc, const char *old_group_name,
				 const char *new_group_name, GList *members)
{
	MsnSession *session = gc->proto_data;
	char outparams[MSN_BUF_LEN];
	MsnGroup *old_group;

	if ((old_group = msn_groups_find_with_name(session->groups,
											   old_group_name)) != NULL) {

		g_snprintf(outparams, sizeof(outparams), "%d %s 0",
				   msn_group_get_id(old_group),
				   msn_url_encode(new_group_name));

		if (!msn_servconn_send_command(session->notification_conn,
									   "REG", outparams)) {
			gaim_connection_error(gc, _("Write error"));
			return;
		}

		msn_group_set_name(old_group, new_group_name);
	}
	else {
		g_snprintf(outparams, sizeof(outparams), "%s 0",
				   msn_url_encode(new_group_name));

		if (!msn_servconn_send_command(session->notification_conn,
									   "ADG", outparams)) {
			gaim_connection_error(gc, _("Write error"));
			return;
		}
	}
}

static void
msn_buddy_free(GaimBuddy *b)
{
	if (b->proto_data != NULL) {
		msn_user_destroy(b->proto_data);
		b->proto_data = NULL;
	}
}

static void
msn_convo_closed(GaimConnection *gc, const char *who)
{
	GaimAccount *account = gaim_connection_get_account(gc);
	MsnSession *session = gc->proto_data;
	MsnSwitchBoard *swboard;

	swboard = msn_session_find_switch_with_passport(session, who);

	if (swboard != NULL && swboard->chat == NULL) {
		char sendbuf[256];

		g_snprintf(sendbuf, sizeof(sendbuf), "BYE %s\r\n",
				   gaim_account_get_username(account));

		msn_servconn_write(swboard->servconn, sendbuf, strlen(sendbuf));

		msn_switchboard_destroy(swboard);
	}
}

static char *
msn_normalize(const char *str)
{
	static char buf[BUF_LEN];

	g_return_val_if_fail(str != NULL, NULL);

	g_snprintf(buf, sizeof(buf), "%s%s", str,
			   (strchr(str, '@') ? "" : "@hotmail.com"));

	return buf;
}

static void
msn_got_info(gpointer data, char *url_text, unsigned long len)
{
	char *stripped,*p,*q;
	char buf[1024];
	char *user_url = NULL;

	if (!url_text || !strcmp(url_text,"")) {
		g_show_info_text(NULL, NULL, 2,
				_("<html><body><b>Error retrieving profile</b></body></html>"), NULL);
		return;
	}

	/* if they have a homepage link, MSN masks it such that we need to
	 * fetch the url out before strip_html() nukes it */
	if((p = strstr(url_text, "Take a look at my </font><A class=viewDesc title=\"")) != NULL )
	{
		p += 50;
		if((q = strchr(p, '"')) != NULL)
			user_url = g_strndup(p,q-p);
	}

	/* strip_html() doesn't strip out character entities like &nbsp; and &#183; */
	while ((p = strstr(url_text, "&nbsp;")) != NULL) {
		memmove(p, p+6, strlen(p+6));
		url_text[strlen(url_text)-6] = '\0';
	}
	while ((p = strstr(url_text, "&#183;")) != NULL) {
		memmove(p, p+6, strlen(p+6));
		url_text[strlen(url_text)-6] = '\0';
	}

	/* nuke the nasty \r's that just get in the way */
	while ((p = strchr(url_text, '\r')) != NULL) {
		memmove(p, p+1, strlen(p+1));
		url_text[strlen(url_text)-1] = '\0';
	}

	/* MSN always puts in &#39; for apostrophies...replace them */
	while ((p = strstr(url_text, "&#39;")) != NULL) {
		*p = '\'';
		memmove(p+1, p+5, strlen(p+5));
		url_text[strlen(url_text)-4] = '\0';
	}

	/* nuke the html, it's easier than trying to parse the horrid stuff */
	stripped = strip_html(url_text);

	/* gonna re-use the memory we've already got for url_text */
	strcpy(url_text, "<html><body>\n");

	/* extract their Name and put it in */
	info_extract_field(stripped, url_text, "\tName", 0, "\t", '\n',
			"Undisclosed", _("Name"), 0, NULL);

	/* extract their Age and put it in */
	info_extract_field(stripped, url_text, "\tAge", 0, "\t", '\n',
			"Undisclosed", _("Age"), 0, NULL);

	/* extract their Gender and put it in */
	info_extract_field(stripped, url_text, "\tGender", 6, "\t", '\n',
			"Undisclosed", _("Gender"), 0, NULL);

	/* extract their MaritalStatus and put it in */
	info_extract_field(stripped, url_text, "\tMaritalStatus", 0, "\t", '\n',
			"Undisclosed", _("Marital Status"), 0, NULL);

	/* extract their Location and put it in */
	info_extract_field(stripped, url_text, "\tLocation", 0, "\t", '\n',
			"Undisclosed", _("Location"), 0, NULL);

	/* extract their Occupation and put it in */
	info_extract_field(stripped, url_text, "\t Occupation", 6, "\t", '\n',
			"Undisclosed", _("Occupation"), 0, NULL);

	/* the fields, 'A Little About Me', 'Favorite Things', 'Hobbies and Interests',
	 * 'Favorite Quote', and 'My Homepage' may or may not appear, in any combination.
	 * however, they do appear in certain order, so we can successively search to
	 * pin down the distinct values.
	 */

	/* check if they have A Little About Me */
	if(!info_extract_field(stripped, url_text, "\tA Little About Me", 1, "Favorite Things", '\n',
				NULL, _("A Little About Me"), 0, NULL))
		if(!info_extract_field(stripped, url_text, "\tA Little About Me", 1, "Hobbies and Interests", '\n',
					NULL, _("A Little About Me"), 0, NULL))
			if(!info_extract_field(stripped, url_text, "\tA Little About Me", 1, "Favorite Quote", '\n',
						NULL, _("A Little About Me"), 0, NULL))
				if(!info_extract_field(stripped, url_text, "\tA Little About Me", 1, "My Homepage\tTake a look", '\n',
							NULL, _("A Little About Me"), 0, NULL))
					info_extract_field(stripped, url_text, "\tA Little About Me", 1, "last updated", '\n',
							NULL, _("A Little About Me"), 0, NULL);

	/* check if they have Favorite Things */
	if(!info_extract_field(stripped, url_text, "Favorite Things", 1, "Hobbies and Interests", '\n',
				NULL, _("Favorite Things"), 0, NULL))
		if(!info_extract_field(stripped, url_text, "Favorite Things", 1, "Favorite Quote", '\n',
					NULL, "Favorite Things", 0, NULL))
			if(info_extract_field(stripped, url_text, "Favorite Things", 1, "My Homepage\tTake a look", '\n',
						NULL, _("Favorite Things"), 0, NULL))
				info_extract_field(stripped, url_text, "Favorite Things", 1, "last updated", '\n',
						NULL, _("Favorite Things"), 0, NULL);

	/* check if they have Hobbies and Interests */
	if(!info_extract_field(stripped, url_text, "Hobbies and Interests", 1, "Favorite Quote", '\n',
				NULL, _("Hobbies and Interests"), 0, NULL))
		if(info_extract_field(stripped, url_text, "Hobbies and Interests", 1, "My Homepage\tTake a look", '\n',
					NULL, _("Hobbies and Interests"), 0, NULL))
			info_extract_field(stripped, url_text, "Hobbies and Interests", 1, "last updated", '\n',
					NULL, _("Hobbies and Interests"), 0, NULL);

	/* check if they have Favorite Quote */
	if(!info_extract_field(stripped, url_text, "Favorite Quote", 1, "My Homepage\tTake a look", '\n',
				NULL, _("Favorite Quote"), 0, NULL))
		info_extract_field(stripped, url_text, "Favorite Quote", 1, "last updated", '\n',
				NULL, _("Favorite Quote"), 0, NULL);

	/* extract the last updated date and put it in */
	info_extract_field(stripped, url_text, "\tlast updated:", 1, "\n", '\n',
			NULL, _("Last Updated"), 0, NULL);

	/* if we were able to fetch a homepage url earlier, stick it in there */
	if(user_url)
	{
		g_snprintf(buf,sizeof(buf),"<b>%s:</b><br><a href=\"%s\">%s</a><br>\n",_("Homepage"),user_url,user_url);
		strcat(url_text,buf);
	}

	/* finish it off, and show it to them */
	strcat(url_text, "</body></html>\n");
	g_show_info_text(NULL, NULL, 2, url_text, NULL);
	g_free(stripped);
}

static void
msn_get_info(GaimConnection *gc, const char *name)
{
	char url[256];
	g_snprintf(url, sizeof url, "%s%s", PROFILE_URL, name);
	grab_url(url, FALSE, msn_got_info, NULL,"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)",1);
}

static GaimPluginProtocolInfo prpl_info =
{
	GAIM_PROTO_MSN,
	OPT_PROTO_MAIL_CHECK | OPT_PROTO_BUDDY_ICON,
	NULL,
	NULL,
	msn_list_icon,
	msn_list_emblems,
	msn_status_text,
	msn_tooltip_text,
	msn_away_states,
	msn_actions,
	msn_buddy_menu,
	NULL,
	msn_login,
	msn_close,
	msn_send_im,
	NULL,
	msn_send_typing,
	msn_get_info,
	msn_set_away,
	NULL,
	NULL,
	NULL,
	NULL,
	msn_set_idle,
	NULL,
	msn_add_buddy,
	NULL,
	msn_rem_buddy,
	NULL,
	msn_add_permit,
	msn_add_deny,
	msn_rem_permit,
	msn_rem_deny,
	msn_set_permit_deny,
	NULL,
	NULL,
	msn_chat_invite,
	msn_chat_leave,
	NULL,
	msn_chat_send,
	msn_keepalive,
	NULL,
	NULL,
	NULL,
	NULL,
	msn_group_buddy,
	msn_rename_group,
	msn_buddy_free,
	msn_convo_closed,
	msn_normalize
};

static GaimPluginInfo info =
{
	2,                                                /**< api_version    */
	GAIM_PLUGIN_PROTOCOL,                             /**< type           */
	NULL,                                             /**< ui_requirement */
	0,                                                /**< flags          */
	NULL,                                             /**< dependencies   */
	GAIM_PRIORITY_DEFAULT,                            /**< priority       */

	"prpl-msn",                                       /**< id             */
	"MSN",                                            /**< name           */
	VERSION,                                          /**< version        */
	                                                  /**  summary        */
	N_("MSN Protocol Plugin"),
	                                                  /**  description    */
	N_("MSN Protocol Plugin"),
	"Christian Hammond <chipx86@gnupdate.org>",       /**< author         */
	GAIM_WEBSITE,                                     /**< homepage       */

	NULL,                                             /**< load           */
	NULL,                                             /**< unload         */
	NULL,                                             /**< destroy        */

	NULL,                                             /**< ui_info        */
	&prpl_info                                        /**< extra_info     */
};

static void
init_plugin(GaimPlugin *plugin)
{
	GaimAccountOption *option;

	option = gaim_account_option_string_new(_("Login server"), "server",
											MSN_SERVER);
	prpl_info.protocol_options = g_list_append(prpl_info.protocol_options,
											   option);

	option = gaim_account_option_int_new(_("Port"), "port", 1863);
	prpl_info.protocol_options = g_list_append(prpl_info.protocol_options,
											   option);

	my_protocol = plugin;

	gaim_prefs_add_none("/plugins/prpl/msn");
	gaim_prefs_add_bool("/plugins/prpl/msn/conv_close_notice",   TRUE);
	gaim_prefs_add_bool("/plugins/prpl/msn/conv_timeout_notice", TRUE);
}

GAIM_INIT_PLUGIN(msn, init_plugin, info);
