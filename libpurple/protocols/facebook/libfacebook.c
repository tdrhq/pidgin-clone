/*
 * libfacebook
 *
 * libfacebook is the property of its developers.  See the COPYRIGHT file
 * for more details.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "libfacebook.h"
#include "fb_blist.h"
#include "fb_connection.h"
#include "fb_info.h"
#include "fb_managefriends.h"
#include "fb_messages.h"
#include "fb_notifications.h"
#include "fb_search.h"

/******************************************************************************/
/* Utility functions */
/******************************************************************************/

gchar *fb_convert_unicode(const gchar *input)
{
	/* \u00e9t\u00e9 should be été */

	gunichar unicode_char;
	gchar unicode_char_str[6];
	gint unicode_char_len;
	gchar *next_pos;
	gchar *input_string;
	gchar *output_string;

	if (input == NULL)
		return NULL;

	next_pos = input_string = g_strdup(input);

	/* purple_debug_info("facebook", "unicode convert: in: %s\n", input); */
	while ((next_pos = strstr(next_pos, "\\u")))
	{
		/* grab the unicode */
		sscanf(next_pos, "\\u%4x", &unicode_char);
		/* turn it to a char* */
		unicode_char_len = g_unichar_to_utf8(unicode_char, unicode_char_str);
		/* shove it back into the string */
		g_memmove(next_pos, unicode_char_str, unicode_char_len);
		/* move all the data after the \u0000 along */
		g_stpcpy(next_pos + unicode_char_len, next_pos + 6);
	}

	/* purple_debug_info("facebook", "unicode convert: out: %s\n", input); */
	output_string = g_strcompress(input_string);
	g_free(input_string);

	return output_string;
}

/* Like purple_strdup_withhtml, but escapes htmlentities too */
gchar *fb_strdup_withhtml(const gchar *src)
{
	gulong destsize, i, j;
	gchar *dest;

	g_return_val_if_fail(src != NULL, NULL);

	/* New length is (length of src) + (number of \n's * 3) + (number of &'s * 5) +
		(number of <'s * 4) + (number of >'s *4) + (number of "'s * 6) -
		(number of \r's) + 1 */
	destsize = 1;
	for (i = 0; src[i] != '\0'; i++)
	{
		if (src[i] == '\n' || src[i] == '<' || src[i] == '>')
			destsize += 4;
		else if (src[i] == '&')
			destsize += 5;
		else if (src[i] == '"')
			destsize += 6;
		else if (src[i] != '\r')
			destsize++;
	}

	dest = g_malloc(destsize);

	/* Copy stuff, ignoring \r's, because they are dumb */
	for (i = 0, j = 0; src[i] != '\0'; i++) {
		if (src[i] == '\n') {
			strcpy(&dest[j], "<BR>");
			j += 4;
		} else if (src[i] == '<') {
			strcpy(&dest[j], "&lt;");
			j += 4;
		} else if (src[i] == '>') {
			strcpy(&dest[j], "&gt;");
			j += 4;
		} else if (src[i] == '&') {
			strcpy(&dest[j], "&amp;");
			j += 5;
		} else if (src[i] == '"') {
			strcpy(&dest[j], "&quot;");
			j += 6;
		} else if (src[i] != '\r')
			dest[j++] = src[i];
	}

	dest[destsize-1] = '\0';

	return dest;
}

/******************************************************************************/
/* PRPL functions */
/******************************************************************************/

static const char *fb_list_icon(PurpleAccount *account, PurpleBuddy *buddy)
{
	return "facebook";
}

static gchar *fb_status_text(PurpleBuddy *buddy)
{
	FacebookBuddy *fbuddy = buddy->proto_data;

	if (fbuddy && fbuddy->status && *fbuddy->status != '\0')
		return g_strdup(fbuddy->status);

	return NULL;
}

static void fb_tooltip_text(PurpleBuddy *buddy,
		PurpleNotifyUserInfo *userinfo, gboolean full)
{
	FacebookBuddy *fbuddy = buddy->proto_data;
	gchar *status;

	g_return_if_fail(fbuddy);

	if (fbuddy->status && *fbuddy->status != '\0')
	{
		status = g_strdup_printf("%s %s",
				fbuddy->name, fbuddy->status);

		purple_notify_user_info_add_pair(userinfo,
				_("Status"), status);
		g_free(status);
		if (fbuddy->status_rel_time && *fbuddy->status_rel_time)
		{
			purple_notify_user_info_add_pair(userinfo,
					_("Status changed"),
					fbuddy->status_rel_time);
		}
	}
}

static GList *fb_statuses(PurpleAccount *account)
{
	GList *types = NULL;
	PurpleStatusType *status;

	/* Online people have a status message and also a date when it was set */
	status = purple_status_type_new_with_attrs(PURPLE_STATUS_AVAILABLE,
		NULL, _("Online"), FALSE, TRUE, FALSE, "message",
		_("Message"), purple_value_new(PURPLE_TYPE_STRING),
		"message_date", _("Message changed"),
		purple_value_new(PURPLE_TYPE_STRING), NULL);
	types = g_list_append(types, status);

	/* Offline people dont have messages */
	status = purple_status_type_new_full(PURPLE_STATUS_OFFLINE, NULL, _("Offline"), FALSE, TRUE, FALSE);
	types = g_list_append(types, status);

	return types;
}

static gboolean fb_get_messages_failsafe(FacebookAccount *fba)
{
	if (fba->last_messages_download_time < (time(NULL) - (60*5))) {
		/* Messages haven't been downloaded in a while-
		 * something is probably wrong */
		purple_debug_warning("facebook",
				"executing message check failsafe\n");
		fb_get_post_form_id(fba);
	}

	return TRUE;
}

static void fb_login_cb(FacebookAccount *fba, gchar *response, gsize len,
		gpointer userdata)
{
	gchar *user_cookie;

	purple_connection_update_progress(fba->pc, _("Authenticating"), 2, 3);

	/* Look for our uid */
	user_cookie = g_hash_table_lookup(fba->cookie_table, "c_user");
	if (user_cookie == NULL) {
		/*
		 * Server didn't set the c_user cookie, so we must have given
		 * them a bad username or password
		 */
		purple_connection_error_reason(fba->pc,
				PURPLE_CONNECTION_ERROR_AUTHENTICATION_FAILED,
				_("Incorrect username or password."));
		return;
	}
	fba->uid = atoi(user_cookie);
	purple_debug_info("facebook", "uid %d\n", fba->uid);

	/* ok, we're logged in now! */
	purple_connection_set_state(fba->pc, PURPLE_CONNECTED);

	/* This will kick off our long-poll message retrieval loop */
	fb_get_post_form_id(fba);
	fb_get_buddy_list(fba);
	fb_check_friend_requests(fba);

	/* periodically check for people adding you to their facebook friend list */
	fba->friend_request_timer = purple_timeout_add_seconds(60 * 5,
			fb_check_friend_requests, fba);

	/* periodically check for updates to your buddy list */
	fba->buddy_list_timer = purple_timeout_add_seconds(60,
			fb_get_buddy_list, fba);

	/* periodically check for new notifications */
	fba->notifications_timer = purple_timeout_add_seconds(60,
			(GSourceFunc)fb_get_notifications_feed, fba);

	/* Periodically check for new messages.  NOTE: This MUST exist,
	 * regardless of other other mechanisms for checking messages.  This
	 * is because the code needs a failsafe checker in case other one of
	 * the other retrieval mechanisms dies due to a bad request, etc.
	 * Without such a failsafe, a user will receive no messages, which is
	 * one of hardest bugs to debug and get reports about.  Hence, the
	 * importance of this loop.
	 * That said, there is room for tweaking this loop and possibly even
	 * setting it such that it is the primary or only message checker.
	 * The key is that the method must NEVER die until logout.
	 */
	fba->perpetual_messages_timer = purple_timeout_add_seconds(15,
			(GSourceFunc)fb_get_messages_failsafe, fba);
}

static void fb_login(PurpleAccount *account)
{
	FacebookAccount *fba;
	guint16 i;
	gchar *postdata, *encoded_username, *encoded_password;

	/* Create account and initialize state */
	fba = g_new0(FacebookAccount, 1);
	fba->account = account;
	fba->pc = purple_account_get_connection(account);
	fba->uid = -1;
	fba->last_messages_download_time = time(NULL) - 60; /* 60 secs is a safe buffer */
	fba->cookie_table = g_hash_table_new_full(g_str_hash, g_str_equal,
			g_free, g_free);
	fba->hostname_ip_cache = g_hash_table_new_full(g_str_hash, g_str_equal,
			g_free, g_free);

	g_hash_table_replace(fba->cookie_table, g_strdup("test_cookie"),
			g_strdup("1"));

	for (i = 0; i < FB_LAST_MESSAGE_MAX; i++)
		fba->last_messages[i] = 0;

	account->gc->proto_data = fba;

	/* Error localized in libpurple jabber.c */
	if (!purple_ssl_is_supported()) {
		purple_connection_error_reason (purple_account_get_connection(account),
				PURPLE_CONNECTION_ERROR_NO_SSL_SUPPORT,
				_("Server requires TLS/SSL for login.  No TLS/SSL support found."));
		return;
	}

	purple_connection_set_state(fba->pc, PURPLE_CONNECTING);
	purple_connection_update_progress(fba->pc, _("Connecting"), 1, 3);

	encoded_username = g_strdup(purple_url_encode(
			purple_account_get_username(fba->account)));
	encoded_password = g_strdup(purple_url_encode(
			purple_account_get_password(fba->account)));

	postdata = g_strdup_printf(
			"email=%s&pass=%s&persistent=1&login=Login&charset_test=%%E2%%AC%%C2%%B4%%E2%%82%%AC%%C2%%B4%%E6%%B0%%B4%%D0%%94%%D0%%84",
			encoded_username, encoded_password);
	g_free(encoded_username);
	g_free(encoded_password);

	fb_post_or_get(fba, FB_METHOD_POST | FB_METHOD_SSL, "login.facebook.com",
			"/login.php", postdata, fb_login_cb, NULL, FALSE);
	g_free(postdata);
}

static void fb_close(PurpleConnection *pc)
{
	FacebookAccount *fba;
	gchar *postdata;

	purple_debug_info("facebook", "disconnecting account\n");

	fba = pc->proto_data;

	/* Tell Facebook that we've logged out. */
	/*
	 * TODO
	 * This doesn't actually work because the request is non-blocking
	 * and we're in the process of logging out.  So we start making a
	 * connection but then libpurple immediately cancels the attempt
	 * and frees everything.
	 *
	 * There are two ways to fix this:
	 * 1. We could make this request, but not pass in fba or reference
	 *    any other data.  The request could complete normally even
	 *    after this account has logged out, since it really doesn't
	 *    need access to the PurpleConnection or the FacebookAccount.
	 *
	 * 2. The close prpl callback could be changed in libpurple so that
	 *    protocol plugins can have a chance to make network requests
	 *    and do other long cleanup operations.  So the call to
	 *    prpl->close() would become asynchronous.  It tells the
	 *    protocol plugin to begin the shutdown sequence, and the
	 *    protocol plugin tells the core when it's finished.
	 */
	if (fba->post_form_id)
		postdata = g_strdup_printf(
				"visibility=false&post_form_id=%s",
				fba->post_form_id);
	else
		postdata = g_strdup("visibility=false");
	fb_post_or_get(fba, FB_METHOD_POST, NULL, "/ajax/chat/settings.php",
			postdata, NULL, NULL, FALSE);
	g_free(postdata);

	if (fba->buddy_list_timer) {
		purple_timeout_remove(fba->buddy_list_timer);
	}
	if (fba->friend_request_timer) {
		purple_timeout_remove(fba->friend_request_timer);
	}
	if (fba->notifications_timer) {
		purple_timeout_remove(fba->notifications_timer);
	}
	if (fba->new_messages_check_timer) {
		purple_timeout_remove(fba->new_messages_check_timer);
	}
	if (fba->perpetual_messages_timer) {
		purple_timeout_remove(fba->perpetual_messages_timer);
	}

	purple_debug_info("facebook", "destroying %d incomplete connections\n",
			g_slist_length(fba->conns));

	while (fba->conns != NULL)
		fb_connection_destroy(fba->conns->data);

	while (fba->dns_queries != NULL) {
		PurpleDnsQueryData *dns_query = fba->dns_queries->data;
		purple_debug_info("facebook", "canceling dns query for %s\n",
					purple_dnsquery_get_host(dns_query));
		fba->dns_queries = g_slist_remove(fba->dns_queries, dns_query);
		purple_dnsquery_destroy(dns_query);
	}

	g_hash_table_destroy(fba->cookie_table);
	g_hash_table_destroy(fba->hostname_ip_cache);
	g_free(fba->post_form_id);
	g_free(fba->channel_number);
	g_slist_free(fba->auth_buddies);
	g_free(fba->last_status_message);
	g_free(fba);
}

static unsigned int fb_send_typing(PurpleConnection *pc, const gchar *name,
		PurpleTypingState state)
{
	int typing_state;
	gchar *postdata;
	FacebookAccount *fba = pc->proto_data;
	gchar *encoded_name;

	g_return_val_if_fail(fba != NULL, 0);
	g_return_val_if_fail(fba->post_form_id != NULL, 0);

	typing_state = (state == PURPLE_TYPING) ? 1 : 0;

	/* Don't send typing notifications to self */
	if (atoi(name) != fba->uid)
	{
		encoded_name = g_strdup(purple_url_encode(name));
		postdata = g_strdup_printf("typ=%d&to=%s&post_form_id=%s",
				typing_state, encoded_name, fba->post_form_id);
		g_free(encoded_name);
		fb_post_or_get(fba, FB_METHOD_POST, NULL, "/ajax/chat/typ.php",
				postdata, NULL, NULL, FALSE);
		g_free(postdata);
	} else {
		serv_got_typing(pc, name, 10, state);
	}

	/* 7 is the number of seconds before sending the new typing state.  It
	 * corresponds with the default value that Facebook waits. */
	return 7;
}

static void fb_set_status_ok_cb(gpointer data, const gchar *status_text)
{
	PurpleConnection *pc = data;
	FacebookAccount *fba = pc->proto_data;
	gchar *postdata;
	gchar *status_tmp;
	gchar *status_text_new;

	g_return_if_fail(fba->post_form_id != NULL);

	status_text_new = g_strstrip(g_strdup(status_text));

	/* don't set the status if it's idential to what we've already set */
	if (fba->last_status_message && g_str_equal(fba->last_status_message,
				status_text_new)) {
		g_free(status_text_new);
		return;
	}

	g_free(fba->last_status_message);
	fba->last_status_message = status_text_new;

	if (*status_text_new != '\0')
	{
		status_tmp = g_strdup(purple_url_encode(status_text_new));
		postdata = g_strdup_printf("profile_id=%d&status=%s&post_form_id=%s",
				fba->uid, status_tmp, fba->post_form_id);
		g_free(status_tmp);
	}
	else
		postdata = g_strdup_printf("profile_id=%d&clear=1&post_form_id=%s",
				fba->uid, fba->post_form_id);

	fb_post_or_get(fba, FB_METHOD_POST, NULL, "/updatestatus.php",
			postdata, NULL, NULL, FALSE);

	g_free(postdata);
}

static void fb_set_status_p(PurpleAccount *account, PurpleStatus *status)
{
	const gchar *message;
	gchar *stripped;

	/* first check that we actually want to set this through Pidgin */
	if (!purple_account_get_bool(account,
				"facebook_set_status_through_pidgin", FALSE))
	{
		return;
	}

	message = purple_status_get_attr_string(status, "message");
	if (message == NULL)
		message = "";

	stripped = purple_markup_strip_html(message);
	fb_set_status_ok_cb(account->gc, stripped);
	g_free(stripped);
}

static void fb_buddy_free(PurpleBuddy *buddy)
{
	FacebookBuddy *fbuddy = buddy->proto_data;
	if (fbuddy != NULL)
	{
		buddy->proto_data = NULL;

		g_free(fbuddy->name);
		g_free(fbuddy->status);
		g_free(fbuddy->status_rel_time);
		g_free(fbuddy->thumb_url);
		g_free(fbuddy);
	}
}

static void fb_convo_closed(PurpleConnection *gc, const char *who)
{
	FacebookAccount *fba = gc->proto_data;
	gchar *postdata;

	g_return_if_fail(fba->post_form_id != NULL);

	/* notify server that we closed the chat window */
	/* close_chat=589039771&window_id=3168919846&
	 * post_form_id=c258fe42460c7e8b61e242a37ef05afc */
	postdata = g_strdup_printf("close_chat=%s&post_form_id=%s", who,
			fba->post_form_id);
	fb_post_or_get(fba, FB_METHOD_POST, NULL, "/ajax/chat/settings.php",
			postdata, NULL, NULL, FALSE);
	g_free(postdata);
}

static GHashTable *fb_get_account_text_table(PurpleAccount *account)
{
	GHashTable *table;

	table = g_hash_table_new(g_str_hash, g_str_equal);

	g_hash_table_insert(table, "login_label", (gpointer)_("Email Address..."));

	return table;
}

/******************************************************************************/
/* Plugin functions */
/******************************************************************************/

static gboolean plugin_load(PurplePlugin *plugin)
{
#ifdef HAVE_ZLIB
	/* try dynamically loading zlib functions */
	if (zlib_library == NULL)
		/* zlib_library = dlopen("zlib1.dll", RTLD_LAZY); */
		zlib_library = dlopen("libz.dll", RTLD_LAZY);
	if (zlib_library == NULL)
		zlib_library = dlopen("libz.so", RTLD_LAZY);
	if (zlib_library == NULL)
		zlib_library = dlopen("libz.dylib", RTLD_LAZY);
	if (zlib_inflate == NULL && zlib_library != NULL)
	{
		zlib_inflate = (int (*)()) dlsym(zlib_library, "inflate");
		zlib_inflateInit2_ = (int (*) ()) dlsym(zlib_library, "inflateInit2_");
		zlib_inflateEnd = (int (*) ()) dlsym(zlib_library, "inflateEnd");
	}
#endif

	return TRUE;
}

static gboolean plugin_unload(PurplePlugin *plugin)
{
#ifdef HAVE_ZLIB
	if (zlib_library != NULL)
	{
		dlclose(zlib_library);
		zlib_library = NULL;
		zlib_inflate = NULL;
		zlib_inflateInit2_ = NULL;
		zlib_inflateEnd = NULL;
	}
#endif
	return TRUE;
}

static void fb_set_status_cb(PurplePluginAction *action)
{
	PurpleConnection *pc = action->context;
	FacebookAccount *fba = pc->proto_data;
	gchar *uid_str;

	uid_str = g_strdup_printf("%d", fba->uid);

	purple_request_input(pc, NULL, _("Set your Facebook status"),
			purple_account_get_alias(pc->account), "is ",
			FALSE, FALSE, NULL, _("OK"),
			G_CALLBACK(fb_set_status_ok_cb), _("Cancel"),
			NULL, pc->account, uid_str, NULL, pc);

	g_free(uid_str);
}

static void fb_display_plugin_info(PurplePluginAction *action)
{
	purple_notify_info(action->context, _("About Facebook for Pidgin"),
			_("Version"), FACEBOOK_PLUGIN_VERSION);
}

static GList *fb_actions(PurplePlugin *plugin, gpointer context)
{
	GList *m = NULL;
	PurplePluginAction *act;

	act = purple_plugin_action_new(_("About Facebook for Pidgin"),
			fb_display_plugin_info);
	m = g_list_append(m, act);

	act = purple_plugin_action_new(_("Set Facebook status..."),
			fb_set_status_cb);
	m = g_list_append(m, act);

	act = purple_plugin_action_new(_("Search for buddies..."),
			fb_search_users);
	m = g_list_append(m, act);

	return m;
}

static GList *fb_node_menu(PurpleBlistNode *node)
{
	GList *m = NULL;
	PurpleMenuAction *act;
	PurpleBuddy *buddy;
	
	if(PURPLE_BLIST_NODE_IS_BUDDY(node))
	{
		buddy = (PurpleBuddy *)node;
		
		act = purple_menu_action_new(_("_Poke"),
										PURPLE_CALLBACK(fb_blist_poke_buddy),
										NULL, NULL);
		m = g_list_append(m, act);
	}
	return m;
}

static void fb_set_idle(PurpleConnection *gc, int time)
{
	FacebookAccount *fba = gc->proto_data;
	
	g_return_if_fail(fba != NULL);
	
	if (time)
	{
		fba->is_idle = TRUE;
	} else {
		fba->is_idle = FALSE;
	}
}

static void plugin_init(PurplePlugin *plugin)
{
	PurpleAccountOption *option;
	PurplePluginInfo *info = plugin->info;
	PurplePluginProtocolInfo *prpl_info = info->extra_info;

	/* Add options to the advanced screen in the account settings */
	option = purple_account_option_bool_new(_("Hide myself in the Buddy List"), "facebook_hide_self", TRUE);
	prpl_info->protocol_options = g_list_append(prpl_info->protocol_options, option);

	option = purple_account_option_bool_new(_("Set Facebook status through Pidgin status"), "facebook_set_status_through_pidgin", FALSE);
	prpl_info->protocol_options = g_list_append(prpl_info->protocol_options, option);

	option = purple_account_option_bool_new(_("Show Facebook notifications as e-mails in Pidgin"), "facebook_get_notifications", TRUE);
	prpl_info->protocol_options = g_list_append(prpl_info->protocol_options, option);

	option = purple_account_option_bool_new(_("Edit Facebook friends from Pidgin"), "facebook_manage_friends", FALSE);
	prpl_info->protocol_options = g_list_append(prpl_info->protocol_options, option);
}

static PurplePluginProtocolInfo prpl_info = {
	/* options */
	OPT_PROTO_UNIQUE_CHATNAME,

	NULL,                   /* user_splits */
	NULL,                   /* protocol_options */
	/* NO_BUDDY_ICONS */    /* icon_spec */
	{"jpg", 0, 0, 50, 50, -1, PURPLE_ICON_SCALE_SEND}, /* icon_spec */
	fb_list_icon,           /* list_icon */
	NULL,                   /* list_emblems */
	fb_status_text,         /* status_text */
	fb_tooltip_text,        /* tooltip_text */
	fb_statuses,            /* status_types */
	fb_node_menu,           /* blist_node_menu */
	NULL,                   /* chat_info */
	NULL,                   /* chat_info_defaults */
	fb_login,               /* login */
	fb_close,               /* close */
	fb_send_im,             /* send_im */
	NULL,                   /* set_info */
	fb_send_typing,         /* send_typing */
	fb_get_info,            /* get_info */
	fb_set_status_p,        /* set_status */
	fb_set_idle,            /* set_idle */
	NULL,                   /* change_passwd */
	fb_add_buddy,           /* add_buddy */
	NULL,                   /* add_buddies */
	NULL,                   /* remove_buddy */
	NULL,                   /* remove_buddies */
	NULL,                   /* add_permit */
	NULL,                   /* add_deny */
	NULL,                   /* rem_permit */
	NULL,                   /* rem_deny */
	NULL,                   /* set_permit_deny */
	NULL,                   /* join_chat */
	NULL,                   /* reject chat invite */
	NULL,                   /* get_chat_name */
	NULL,                   /* chat_invite */
	NULL,                   /* chat_leave */
	NULL,                   /* chat_whisper */
	NULL,                   /* chat_send */
	NULL,                   /* keepalive */
	NULL,                   /* register_user */
	NULL,                   /* get_cb_info */
	NULL,                   /* get_cb_away */
	NULL,                   /* alias_buddy */
	NULL,                   /* group_buddy */
	NULL,                   /* rename_group */
	fb_buddy_free,          /* buddy_free */
	fb_convo_closed,        /* convo_closed */
	purple_normalize_nocase,/* normalize */
	NULL,                   /* set_buddy_icon */
	NULL,                   /* remove_group */
	NULL,                   /* get_cb_real_name */
	NULL,                   /* set_chat_topic */
	NULL,                   /* find_blist_chat */
	NULL,                   /* roomlist_get_list */
	NULL,                   /* roomlist_cancel */
	NULL,                   /* roomlist_expand_category */
	NULL,                   /* can_receive_file */
	NULL,                   /* send_file */
	NULL,                   /* new_xfer */
	NULL,                   /* offline_message */
	NULL,                   /* whiteboard_prpl_ops */
	NULL,                   /* send_raw */
	NULL,                   /* roomlist_room_serialize */
	NULL,                   /* unregister_user */
	NULL,                   /* send_attention */
	NULL,                   /* attention_types */
	sizeof(PurplePluginProtocolInfo), /* struct_size */
	fb_get_account_text_table, /* get_account_text_table */
};

static PurplePluginInfo info = {
	PURPLE_PLUGIN_MAGIC,
	2, /* major_version */
	3, /* minor version */
	PURPLE_PLUGIN_PROTOCOL, /* type */
	NULL, /* ui_requirement */
	0, /* flags */
	NULL, /* dependencies */
	PURPLE_PRIORITY_DEFAULT, /* priority */
	"prpl-bigbrownchunx-facebookim", /* id */
	"Facebook", /* name */
	FACEBOOK_PLUGIN_VERSION, /* version */
	N_("Facebook Protocol Plugin"), /* summary */
	N_("Facebook Protocol Plugin"), /* description */
	"Eion Robb <eionrobb@gmail.com>", /* author */
	"http://pidgin-facebookchat.googlecode.com/", /* homepage */
	plugin_load, /* load */
	plugin_unload, /* unload */
	NULL, /* destroy */
	NULL, /* ui_info */
	&prpl_info, /* extra_info */
	NULL, /* prefs_info */
	fb_actions, /* actions */

	/* padding */
	NULL,
	NULL,
	NULL,
	NULL
};

PURPLE_INIT_PLUGIN(facebook, plugin_init, info);
