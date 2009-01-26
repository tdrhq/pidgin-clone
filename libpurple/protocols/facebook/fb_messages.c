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

#include "fb_messages.h"
#include "fb_connection.h"

typedef struct _FacebookOutgoingMessage FacebookOutgoingMessage;

struct _FacebookOutgoingMessage {
	FacebookAccount *fba;
	gchar *who;
	time_t time;
	gchar *message;
	gint msg_id;
	guint retry_count;
	guint resend_timer;
};

static gboolean fb_send_im_fom(FacebookOutgoingMessage *msg);
static gboolean fb_resend_im_fom(FacebookOutgoingMessage *msg);
static gboolean fb_get_new_messages(FacebookAccount *fba);

static FacebookOutgoingMessage *fb_msg_create(FacebookAccount *fba)
{
	FacebookOutgoingMessage *msg;
	
	msg = g_new0(FacebookOutgoingMessage, 1);
	msg->fba = fba;
	
	return msg;
}

static void fb_msg_destroy(FacebookOutgoingMessage *msg)
{
	if (msg->resend_timer) {
		purple_timeout_remove(msg->resend_timer);
	}
	g_free(msg->who);
	g_free(msg->message);
	g_free(msg);	
}

void fb_cancel_resending_messages(FacebookAccount *fba)
{
	while (fba->resending_messages != NULL) {
		FacebookOutgoingMessage *msg = fba->resending_messages->data;
		fba->resending_messages = g_slist_remove(fba->resending_messages, msg);
		fb_msg_destroy(msg);
	}	
}
								  
static void got_new_messages(FacebookAccount *fba, gchar *data,
		gsize data_len, gpointer userdata)
{
	gchar *message_text;
	gchar *message_time;
	gchar *from;
	gchar *to;
	gchar *tmp;
	gchar *type;
	gchar *start;
	gchar *end;
	gint64 msgID;
	int i;
	PurpleConnection *pc = userdata;
	PurpleBuddy *buddy;

	/* NULL data will crash on Windows */
	if (data == NULL)
		data = "(null)";

	purple_debug_misc("facebook", "got new messages: %s\n", data);

	/* purple_debug_info("facebook", "fba: %d\n", fba); */
	/* purple_debug_info("facebook", "account: %d\n", fba->account); */

	/* for (;;);{"t":"msg","c":"p_800753867","ms":[{"type":"msg",
		"msg":{"text":"yes","time":1211176515861,"clientTime":1211176514750,
		"msgID":"367146364"},"from":596176850,"to":800753867,
		"from_name":"Jeremy Lawson","to_name":"Eion Robb",
		"from_first_name":"Jeremy","to_first_name":"Eion"}]} */
	/* for (;;);{"t":"refresh"} */
	/* for (;;);{"t":"msg","c":"p_800753867","ms":[{"type":"msg",
		"msg":{"text":"porn head","time":1211177326689,"clientTime":1211177325,
		"msgID":"-1992480367"},"from":800753867,"to":596176850,
		"from_name":"Eion Robb","to_name":"Jeremy Lawson",
		"from_first_name":"Eion","to_first_name":"Jeremy"}]} */
	/* for (;;);{"t":"msg","c":"p_800753867","ms":[{"type":"typ","st":1,
		"from":596176850,"to":800753867},{"type":"msg","msg":{"text":"nubile!",
		"time":1211177334019,"clientTime":1211177326690,"msgID":"696260545"},
		"from":596176850,"to":800753867,"from_name":"Jeremy Lawson",
		"to_name":"Eion Robb","from_first_name":"Jeremy","to_first_name":"Eion"},
		{"type":"msg","msg":{"text":"test2","time":1211177336688,
		"clientTime":1211177326691,"msgID":"1527815367"},"from":596176850,
		"to":800753867,"from_name":"Jeremy Lawson","to_name":"Eion Robb",
		"from_first_name":"Jeremy","to_first_name":"Eion"},{"type":"msg",
		"msg":{"text":"ahhhhhhh!","time":1211177344361,"clientTime":1211177326692,
		"msgID":"4028916254"},"from":596176850,"to":800753867,
		"from_name":"Jeremy Lawson","to_name":"Eion Robb",
		"from_first_name":"Jeremy","to_first_name":"Eion"}]} */
	/* for (;;);{"t":"msg","c":"p_800753867","ms":[{"type":"msg",
		"msg":{"text":"2","time":1211178167261,"clientTime":1211178164656,
		"msgID":"3382240259"},"from":596176850,"to":800753867,
		"from_name":"Jeremy Lawson","to_name":"Eion Robb",
		"from_first_name":"Jeremy","to_first_name":"Eion"}]} */

	/* for (;;);{"t":"refresh", "seq":1} */

	/* look for the start of the JSON, and ignore any proxy headers */
	data = g_strstr_len(data, data_len, "for (;;);");
	if (!data)
	{
		/* Sometimes proxies will return incorrect data, so we just shrug 
		 * it off.
		 * TODO: Only do this for proxies.  And when we do it, keep track
		 * of consecutive failures in the case something is actually
		 * wrong with Facebook.  Eventually this condition should cause
		 * failure */
		/* Continue looping, waiting for more messages */
		purple_debug_error("facebook",
				"got data back, but it's not even json\n");

		fb_get_new_messages(fba);
		return;
	}

	/* refresh means that the session or post_form_id is invalid */
	if (g_str_equal(data, "for (;;);{\"t\":\"refresh\"}"))
	{
		if (fba->post_form_id_refresh_timer == 0)
			fba->post_form_id_refresh_timer = purple_timeout_add_seconds(1, (GSourceFunc)fb_get_post_form_id, fba);
		return;
	}

	/* continue means that the server wants us to remake the connection */
	if (g_str_equal(data, "for (;;);{\"t\":\"continue\"}"))
	{
		/* Continue looping, waiting for more messages */
		fb_get_new_messages(fba);
		return;
	}

	tmp = strstr(data, "\"seq\":");
	if (tmp != NULL)
	{
		tmp = tmp + 6;
		tmp = g_strndup(tmp, strchr(tmp, '}') - tmp);
		purple_debug_info("facebook", "new seq number: %s\n", tmp);
		fba->message_fetch_sequence = atoi(tmp);
		g_free(tmp);
	} else {
		fba->message_fetch_sequence++;
	}

	if (strncmp(data, "for (;;);{\"t\":\"msg\"", 19) == 0)
	{
		start = g_strstr_len(data, data_len, "\"ms\":[");
		if (!start)
		{
			/* Continue looping, waiting for more messages */
			fb_get_new_messages(fba);
			return;
		}
		start += 6;
		while (*start != ']')
		{
			tmp = strstr(start, "\"type\":\"");
			if (tmp)
			{
				tmp += 8;
				type = g_strndup(tmp, strchr(tmp, '"') - tmp);
				purple_debug_info("facebook", "type: %s\n", type);
			} else {
				type = g_strdup("unknown");
			}

			tmp = strstr(start, "\"from\":");
			if (tmp)
			{
				tmp += 7;
				from = g_strndup(tmp, strchr(tmp, ',') - tmp);
				if (from[0] == '"')
					snprintf(from, strlen(from), "%d", atoi(from + 1));
				purple_debug_info("facebook", "from: %s\n", from);
			} else {
				from = NULL;
			}
			tmp = strstr(start, "\"to\":");
			if (tmp)
			{
				tmp += 5;
				end = strchr(tmp, ',');
				if (end == NULL || strchr(tmp, '}') < end)
					end = strchr(tmp, '}');
				to = g_strndup(tmp, end - tmp);
				if (to[0] == '"')
					snprintf(to, strlen(to), "%d", atoi(to + 1));
				purple_debug_info("facebook", "to: %s\n", to);
			} else {
				to = NULL;
			}

			if (from && to && g_str_equal(type, "msg"))
			{
				/* IM message */
				if (fba->uid != atoi(from) || fba->uid == atoi(to))
				{
					tmp = strstr(start, "\"msgID\":");
					tmp += 9;
					tmp = g_strndup(tmp, strchr(tmp, '"') - tmp);
					msgID = atoll(tmp);
					purple_debug_info("facebook", "message id: %s %" G_GINT64_FORMAT " %lld\n", tmp, msgID, atoll(tmp));
					g_free(tmp);
					/* loop through all the previous messages that we have stored */
					/* to make sure that we havn't already received this message */
					for(i = 0; i < FB_LAST_MESSAGE_MAX; i++)
					{
						purple_debug_info("facebook", "last_messages[%d] = %" G_GINT64_FORMAT "\n", i, fba->last_messages[i]);
						if (fba->last_messages[i] == msgID)
							break;
					}
					purple_debug_info("facebook", "i: %d\n", i);
					if (i == FB_LAST_MESSAGE_MAX)
					{
						gchar *postdata;

						/* if we're here, it must be a new message */
						fba->last_messages[fba->next_message_pointer++] = msgID;
						if (fba->next_message_pointer >= FB_LAST_MESSAGE_MAX)
							fba->next_message_pointer = 0;

						tmp = strstr(start, "\"text\":\"");
						tmp += 8;
						tmp = g_strndup(tmp, strstr(tmp, "\",\"time\":") - tmp);
						message_text = fb_convert_unicode(tmp);
						g_free(tmp);
						tmp = fb_strdup_withhtml(message_text);
						g_free(message_text);
						message_text = tmp;
						purple_debug_info("facebook", "text: %s\n", message_text);
						
						tmp = strstr(start, "\"time\":");
						tmp += 7;
						message_time = g_strndup(tmp, strchr(tmp, ',') - tmp - 3);
						purple_debug_info("facebook", "time: %s\n", message_time);
						
						/* Use the in-line buddy name if the buddy list hasn't been downloaded yet */
						buddy = purple_find_buddy(pc->account, from);
						if (buddy == NULL || buddy->server_alias == NULL)
						{
							tmp = strstr(start, "\"from_name\":\"");
							if (tmp)
							{
								tmp += 13;
								tmp = g_strndup(tmp, strstr(tmp, "\",") - tmp);
								serv_got_alias(pc, from, tmp);
								g_free(tmp);
							}
						}

						serv_got_im(pc, from, message_text, PURPLE_MESSAGE_RECV, atoi(message_time));

						/*
						 * Acknowledge receipt of the message by simulating
						 * focusing the window.  Not sure what the window_id
						 * is here, but it doesn't seem to matter.  Maybe
						 * something internal to the Facebook javascript that
						 * is used for maintaining UI state across page loads?
						 */
						postdata = g_strdup_printf(
								"focus_chat=%s&window_id=12345&post_form_id=%s",
								from, fba->post_form_id);
						fb_post_or_get(fba, FB_METHOD_POST, NULL,
								"/ajax/chat/settings.php?_ecdc=false",
								postdata, NULL, NULL, FALSE);
						g_free(postdata);

						g_free(message_text);
						g_free(message_time);
					}
				}
				start = strchr(start, '}')+1;
			} else if (from && g_str_equal(type, "typ"))
			{
				/* typing notification */
				tmp = strstr(start, "\"st\":");
				if (tmp != NULL)
				{
					tmp += 5;
					if (*tmp == '0')
						serv_got_typing(pc, from, 10, PURPLE_TYPED);
					else
						serv_got_typing(pc, from, 10, PURPLE_TYPING);
				}
			}

			/*
			 * we've received something from a buddy, assume they're online
			 * only if it's not from ourselves
			 */
			if (from && fba->uid != atoi(from))
				purple_prpl_got_user_status(fba->account, from, purple_primitive_get_id_from_type(PURPLE_STATUS_AVAILABLE), NULL);

			g_free(from);
			g_free(to);
			g_free(type);

			start = strchr(start, '}')+1;
			while (*start == ',')
				start++;
		}
	}

	/* Continue looping, waiting for more messages */
	fb_get_new_messages(fba);
}

/* Message fetch loop.  An invariant here is that get_new_messages()
 * is the only function which can create a new timer for
 * new_messages_check_timer.  The timer always calls got_new_messages(),
 * and got_new_messages always calls get_new_messages (either directly
 * or indirectly).  To start the loops off, get_new_messages should be
 * called ONCE and only ONCE.  After that the timers will take care of
 * themselves until final cleanup.
 */
static gboolean fb_get_new_messages(FacebookAccount *fba)
{
	/* MARKCONFLICT (r283,r286): Mark uses the fba->chanel_number variable here,
	 * where this patch/fix eschews that in favor of some hacks.
	 */
	time_t now;
	gchar *fetch_url;
	gchar *fetch_server;
	const gchar *channel_number;

	fba->new_messages_check_timer = 0;

	now = time(NULL);
	if (fba->last_messages_download_time > now - 3) {
		/*
		 * Wait a bit before fetching more messages, to make sure we
		 * never hammer their servers.
		 *
		 * TODO: This could be smarter.  Like, allow 3 requests per
		 *       10 seconds or something.
		 */
		fba->new_messages_check_timer = purple_timeout_add_seconds(
				3 - (now - fba->last_messages_download_time),
				(GSourceFunc)fb_get_new_messages, fba);
		return FALSE;
	}

	if (fba->channel_number == NULL)
	{
		channel_number = purple_account_get_string(fba->account, "last_channel_number", NULL);
		if (channel_number == NULL)
		{
			/* channel number is probably updating */
			fba->new_messages_check_timer = purple_timeout_add_seconds(3 - (now - fba->last_messages_download_time), (GSourceFunc)fb_get_new_messages, fba);
			return FALSE;
		}
	} else {
		channel_number = fba->channel_number;
	}

	purple_debug_info("facebook", "getting new messages\n");

	fetch_server = g_strdup_printf("%d.channel%s.facebook.com", 0, channel_number);
	/* use the current time in the url to get past any transparent proxy caches */
	fetch_url = g_strdup_printf("/x/%lu/%s/p_%d=%d", time(NULL), (fba->is_idle?"false":"true"), fba->uid, fba->message_fetch_sequence);

	fb_post_or_get(fba, FB_METHOD_GET, fetch_server, fetch_url, NULL, got_new_messages, fba->pc, TRUE);
	fba->last_messages_download_time = now;

	g_free(fetch_url);
	g_free(fetch_server);

	return FALSE;
}

static void fb_send_im_cb(FacebookAccount *fba, gchar *data, gsize data_len, gpointer user_data)
{
	FacebookOutgoingMessage *msg = user_data;
	gchar *error_summary = NULL;
	gchar *tmp;

	/* NULL data crashes on Windows */
	if (data == NULL)
		data = "(null)";
	
	purple_debug_misc("facebook", "sent im response: %s\n", data);
	/* for (;;);{"error":1356003,"errorSummary":"Send destination not online",
		"errorDescription":"This person is no longer online.","payload":null,
		"bootload":[{"name":"js\/common.js.pkg.php","type":"js",
		"src":"http:\/\/static.ak.fbcdn.net\/rsrc.php\/pkg\/59\/98936\
		/js\/common.js.pkg.php"}]} */
	/* for (;;);{"error":0,"errorSummary":"","errorDescription":"No error.",
		"payload":[],"bootload":[{"name":"js\/common.js.pkg.php","type":"js",
		"src":"http:\/\/static.ak.fbcdn.net\/rsrc.php\/pkg\/59\/98936\
		/js\/common.js.pkg.php"}]} */

	tmp = g_strstr_len(data, data_len, "\"error\":");
	if (tmp != NULL)
	{
		tmp += 8;
		tmp = g_strndup(tmp, strchr(tmp, ',') - tmp);
		if (strlen(tmp) > 1 || tmp[0] != '0')
		{
			g_free(tmp);
			tmp = g_strstr_len(data, data_len, "\"errorSummary\":\"");
			tmp += 16;
			error_summary = g_strndup(tmp, strchr(tmp, '"') - tmp);
			purple_debug_error("facebook", "sent im error: %s\n", error_summary);
			if (*error_summary)
			{
				/* there was an error, either report it or retry */
				if (msg->retry_count++ < FB_MAX_MSG_RETRY)
				{
					msg->resend_timer = purple_timeout_add_seconds(1, (GSourceFunc)fb_resend_im_fom, msg);
					fba->resending_messages = g_slist_prepend(fba->resending_messages, msg);
					g_free(error_summary);
					return;
				}
				else
				{
					PurpleConversation *conv;
					conv = purple_conversation_new(PURPLE_CONV_TYPE_IM,
							fba->account, msg->who);
					purple_conversation_write(conv, NULL, error_summary,
							PURPLE_MESSAGE_ERROR, msg->time);
				}
			}
		}
	}

	g_free(error_summary);
	fb_msg_destroy(msg);
}

static gboolean fb_send_im_fom(FacebookOutgoingMessage *msg)
{
	gchar *encoded_message;
	gchar *postdata;

	encoded_message = g_strdup(purple_url_encode(msg->message));
	postdata = g_strdup_printf("msg_text=%s&msg_id=%d&to=%s&client_time=%lu&post_form_id=%s",
			encoded_message, msg->msg_id, msg->who,
			msg->time,
			msg->fba->post_form_id ? msg->fba->post_form_id : "0");
	g_free(encoded_message);

	fb_post_or_get(msg->fba, FB_METHOD_POST, NULL, "/ajax/chat/send.php", postdata, fb_send_im_cb, msg, FALSE);
	g_free(postdata);

	return FALSE;
}

static gboolean fb_resend_im_fom(FacebookOutgoingMessage *msg)
{
	msg->fba->resending_messages = g_slist_remove(msg->fba->resending_messages, msg);

	return fb_send_im_fom(msg);
}

int fb_send_im(PurpleConnection *pc, const gchar *who, const gchar *message, PurpleMessageFlags flags)
{
	FacebookOutgoingMessage *msg;

	msg = fb_msg_create(pc->proto_data);

	/* convert html to plaintext, removing trailing spaces */
	msg->message = purple_markup_strip_html(message);
	if (strlen(msg->message) > 999)
	{
		fb_msg_destroy(msg);
		return -E2BIG;
	}

	msg->msg_id = g_random_int();
	msg->who = g_strdup(who);
	msg->time = time(NULL);
	msg->retry_count = 0;

	fb_send_im_fom(msg);

	return strlen(message);
}

static void got_form_id_page(FacebookAccount *fba, gchar *data, gsize data_len, gpointer userdata)
{
	const gchar *start_text = "id=\"post_form_id\" name=\"post_form_id\" value=\"";
	gchar *post_form_id;
	gchar *channel_number;
	gchar *tmp = NULL;
	
	/* NULL data crashes on Windows */
	if (data == NULL)
		data = "(null)";
	
	tmp = g_strstr_len(data, data_len, start_text);
	if (tmp == NULL)
	{
		purple_debug_error("facebook", "couldn't find post_form_id\n");
		purple_debug_info("facebook", "page content: %s\n", data);
		/* Maybe they changed their HTML slightly? */
		purple_connection_error_reason(fba->pc,
				PURPLE_CONNECTION_ERROR_AUTHENTICATION_FAILED,
				_("Error getting info from Facebook."));
		return;
	}
	tmp += strlen(start_text);
	post_form_id = g_strndup(tmp, strchr(tmp, '"') - tmp);

	g_free(fba->post_form_id);
	fba->post_form_id = post_form_id;

	/* dodgy as search for channel server number */
	if (!fba->channel_number)
	{
		start_text = "\", \"channel";
		tmp = g_strstr_len(data, data_len, start_text);
		if (tmp == NULL)
		{
			/* Some proxies strip whitepsace */
			start_text = "\",\"channel";
			tmp = g_strstr_len(data, data_len, start_text);
			if (tmp == NULL)
			{
				/* TODO: Is it better to pick a random channel number or to disconnect? */
				/* MARKCONFLICT (r283,r286) */
				channel_number = g_strdup(purple_account_get_string(fba->account, "last_channel_number", ""));
				if (channel_number[0] == '\0')
				{
					purple_debug_error("facebook", "couldn't find channel\n");
					purple_debug_misc("facebook", "page content: %s\n", data);
					purple_connection_error_reason(fba->pc,
							PURPLE_CONNECTION_ERROR_NETWORK_ERROR,
							_("Chat service currently unavailable."));
					return;
				}
			}
		}

		if (tmp != NULL)
		{
			tmp += strlen(start_text);
			channel_number = g_strndup(tmp, strchr(tmp, '"') - tmp);
		}

		purple_account_set_string(fba->account, "last_channel_number", channel_number);

		g_free(fba->channel_number);
		fba->channel_number = channel_number;
	}

	tmp = g_strdup_printf("visibility=true&post_form_id=%s", post_form_id);
	fb_post_or_get(fba, FB_METHOD_POST, "apps.facebook.com", "/ajax/chat/settings.php", tmp, NULL, NULL, FALSE);
	g_free(tmp);

	/*
	 * Now that we have a channel number we can start looping and
	 * waiting for messages
	 */
	fb_get_new_messages(fba);
}

gboolean fb_get_post_form_id(FacebookAccount *fba)
{
	fba->post_form_id_refresh_timer = 0;
	fb_post_or_get(fba, FB_METHOD_GET, NULL, "/home.php", NULL, got_form_id_page, NULL, FALSE);
	return FALSE;
}
