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
#include "fb_connection.h"
#include "fb_blist.h"

static void set_buddies_offline(PurpleBuddy *buddy, GSList *online_buddies_list)
{
	if (g_slist_find(online_buddies_list, buddy) == NULL &&
			PURPLE_BUDDY_IS_ONLINE(buddy))
	{
		purple_prpl_got_user_status(buddy->account, buddy->name,
				purple_primitive_get_id_from_type(PURPLE_STATUS_OFFLINE),
				NULL);
	}
}

static void buddy_icon_cb(FacebookAccount *fba, gchar *data, gsize data_len,
		gpointer user_data)
{
	gchar *buddyname;
	PurpleBuddy *buddy;
	gpointer buddy_icon_data;

	buddyname = user_data;

	purple_debug_info("facebook",
			"buddy icon for buddy %s %" G_GSIZE_FORMAT "\n",
			buddyname, data_len);

	buddy = purple_find_buddy(fba->account, buddyname);
	g_free(buddyname);
	if (buddy == NULL)
		return;

	buddy_icon_data = g_memdup(data, data_len);

	purple_buddy_icons_set_for_user(fba->account, buddy->name,
			buddy_icon_data, data_len, NULL);
}

static void got_buddy_list_cb(FacebookAccount *fba, gchar *data,
		gsize data_len, gpointer userdata)
{
	GSList *buddies_list;
	GSList *online_buddies_list = NULL;
	PurpleBuddy *buddy;
	FacebookBuddy *fbuddy;
	gchar *uid;
	gchar *name;
	gchar *status_text;
	gchar *status_time_text;
	gchar *buddy_icon_url;
	gboolean idle;
	guint32 error_number;

	gchar *search_start;
	gchar *search_tmp;
	gchar *tmp;
	gchar *largest_buddy_search_point = NULL;

	PurpleGroup *fb_group = NULL;

	gboolean current_buddy_online = FALSE;

	purple_debug_info("facebook", "parsing buddy list\n");
	purple_debug_misc("facebook", "buddy list\n%s\n", data);

	if (fba == NULL)
		return;

	if (data == NULL) {
		purple_connection_error_reason(fba->pc,
				PURPLE_CONNECTION_ERROR_NETWORK_ERROR,
				_("Could not retrieve buddy list"));
		return;
	}

	/* Check if the facebook group already exists (fixes #13) */
	fb_group = purple_find_group("Facebook");

	/* if logged out, this comes up */
	/* for (;;);{"error":1357001,"errorSummary":"Not Logged In",
		"errorDescription":"You must be logged in to do that.",
		"payload":null,"bootload":[{"name":"js\/common.js.pkg.php",
		"type":"js","src":"http:\/\/static.ak.fbcdn.net\/rsrc.php\/pkg\/59\
		/98561\/js\/common.js.pkg.php"}]} */
	tmp = g_strstr_len(data, data_len, "\"error\":");
	if (tmp != NULL)
	{
		tmp += 9;
		tmp = g_strndup(tmp, strchr(tmp, ',')-tmp);
		error_number = atoi(tmp);
		g_free(tmp);
		if (error_number)
		{
			/* error :( */
			tmp = g_strstr_len(data, data_len, "\"errorDescription\":");
			tmp += 20;
			tmp = g_strndup(tmp, strchr(tmp, '"')-tmp);
			/* TODO: Use purple_connection_error_reason() */
			purple_connection_error(fba->pc, tmp);
			g_free(tmp);
			return;
		}
	}

	/* look for "userInfos":{ ... }, */
	search_start = strstr(data, "\"userInfos\":{");
	if (search_start == NULL)
		return;
	search_start += 13;

	while (*search_start != '}' && (search_start - data < data_len))
	{
		tmp = strchr(search_start, ':');
		uid = g_strndup(search_start+1, tmp-search_start-2);
		/* purple_debug_misc("facebook", "uid: %s\n", uid); */

		search_start += strlen(uid) + 2;

		search_tmp = strstr(search_start, "\"name\":") + 8;
		if (search_tmp > largest_buddy_search_point)
			largest_buddy_search_point = search_tmp;
		search_tmp = g_strndup(search_tmp, strchr(search_tmp, '"')-search_tmp);
		name = fb_convert_unicode(search_tmp);
		g_free(search_tmp);
		/* purple_debug_misc("facebook", "name: %s\n", name); */

		/* try updating the alias, just in case it was removed locally */
		serv_got_alias(fba->pc, uid, name);

		/* look for "uid":{"i":_____} */
		tmp = g_strdup_printf("\"%s\":{\"i\":", uid);
		search_tmp = g_strstr_len(data, data_len, tmp);
		if (search_tmp != NULL)
		{
			search_tmp += strlen(tmp);
			if (search_tmp > largest_buddy_search_point)
				largest_buddy_search_point = search_tmp;
			search_tmp = g_strndup(search_tmp, strchr(search_tmp, '}')-search_tmp);
			/* purple_debug_misc("facebook", "buddy idle: %s\n", search_tmp); */
			buddy = purple_find_buddy(fba->account, uid);
			idle = g_str_equal(search_tmp, "true");
			g_free(search_tmp);
			current_buddy_online = TRUE;
		} else {
			/* if we're here, the buddy's info has been sent, but they're not actually online */
			current_buddy_online = FALSE;
			idle = FALSE;
		}
		g_free(tmp);

		/* Set the buddy status text and time */
		search_tmp = strstr(search_start, "\"status\":");
		if (search_tmp != NULL && *(search_tmp + 9) == '"')
		{
			search_tmp += 10;
			if (search_tmp > largest_buddy_search_point)
				largest_buddy_search_point = strstr(search_tmp, ",\"statusTime");
			search_tmp = g_strndup(search_tmp, strstr(search_tmp, ",\"statusTime")-1-search_tmp);
			status_text = fb_convert_unicode(search_tmp);
			g_free(search_tmp);
		} else {
			status_text = NULL;
		}

		/* is this us? */
		if (atoi(uid) == fba->uid)
		{
			purple_connection_set_display_name(fba->pc, name);

			/* set our last known status so that we don't re-set it */
			if (status_text && !fba->last_status_message)
				fba->last_status_message = g_strdup(status_text);

			/* check that we don't want to show ourselves */
			if (purple_account_get_bool(fba->account, "facebook_hide_self", TRUE))
			{
				g_free(status_text);
				g_free(name);
				g_free(uid);

				/* Move pointer to the end of the buddy entry */
				search_start = strchr(largest_buddy_search_point, '}') + 1;
				while (*search_start == ',' && (search_start - data < data_len))
					search_start++;
				/* go on to the next buddy */
				continue;
			} else {
				current_buddy_online = TRUE;
			}
		}

		/* Is this a new buddy? */
		buddy = purple_find_buddy(fba->account, uid);
		if (buddy == NULL)
		{
			buddy = purple_buddy_new(fba->account, uid, NULL);
			if (fb_group == NULL)
			{
				fb_group = purple_group_new("Facebook");
				purple_blist_add_group(fb_group, NULL);
			}
			purple_blist_add_buddy(buddy, NULL, fb_group, NULL);
		}
		serv_got_alias(fba->pc, uid, name);
		purple_presence_set_idle(purple_buddy_get_presence(buddy), idle, 0);

		/* Set the FacebookBuddy structure */
		if (buddy->proto_data == NULL)
		{
			fbuddy = g_new0(FacebookBuddy, 1);
			fbuddy->buddy = buddy;
			fbuddy->fba = fba;
			fbuddy->uid = atoi(uid);
			fbuddy->name = g_strdup(name);

			/* load the old buddy icon from the account settings */
			tmp = g_strdup_printf("buddy_icon_%d_cache", fbuddy->uid);
			fbuddy->thumb_url = g_strdup(purple_account_get_string(fba->account, tmp, ""));
			g_free(tmp);

			buddy->proto_data = fbuddy;
		} else {
			fbuddy = buddy->proto_data;
		}

		g_free(uid);
		g_free(name);

		if (status_text != NULL)
		{
			tmp = fb_strdup_withhtml(status_text);
			g_free(status_text);
			status_text = tmp;
			/* purple_debug_misc("facebook", "status: %s\n", status_text); */

			search_tmp = strstr(search_start, "\"statusTimeRel\":") + 17;
			if (search_tmp > largest_buddy_search_point)
				largest_buddy_search_point = strchr(search_tmp, '"');
			search_tmp = g_strndup(search_tmp, strchr(search_tmp, '"')-search_tmp);
			status_time_text = fb_convert_unicode(search_tmp);
			g_free(search_tmp);

			if (g_str_equal(status_time_text, "ull,"))
			{
				g_free(status_time_text);
				status_time_text = NULL;
			}
			g_free(fbuddy->status_rel_time);
			if (status_time_text != NULL)
			{
				fbuddy->status_rel_time = fb_strdup_withhtml(status_time_text);
				g_free(status_time_text);
				/* purple_debug_misc("facebook", "status time: %s\n", fbuddy->status_rel_time); */
			} else {
				fbuddy->status_rel_time = NULL;
			}

			/* if the buddy status has changed, update the contact list */
			if (fbuddy->status == NULL || !g_str_equal(fbuddy->status, status_text))
			{
				tmp = fbuddy->status;
				fbuddy->status = status_text;
				g_free(tmp);
				if (current_buddy_online)
					purple_prpl_got_user_status(fba->account, buddy->name, purple_primitive_get_id_from_type(PURPLE_STATUS_AVAILABLE), NULL);
			} else {
				g_free(status_text);
			}
		} else {
			if (fbuddy->status != NULL)
			{
				g_free(fbuddy->status);
				fbuddy->status = NULL;
				if (current_buddy_online)
				{
					/* update the status in the contact list */
					purple_prpl_got_user_status(fba->account, buddy->name, purple_primitive_get_id_from_type(PURPLE_STATUS_AVAILABLE), NULL);
				}
			}
		}

		/* Set the buddy icon (if it hasn't changed) */
		search_tmp = strstr(search_start, "\"thumbSrc\":") + 12;
		if (search_tmp > largest_buddy_search_point)
			largest_buddy_search_point = search_tmp;
		buddy_icon_url = g_strndup(search_tmp, strchr(search_tmp, '"')-search_tmp);
		if (fbuddy->thumb_url == NULL || !g_str_equal(fbuddy->thumb_url, buddy_icon_url))
		{
			g_free(fbuddy->thumb_url);
			fbuddy->thumb_url = g_strdup(buddy_icon_url);

			/* Save the buddy icon so that they don't all need to be reloaded at startup */
			tmp = g_strdup_printf("buddy_icon_%d_cache", fbuddy->uid);
			purple_account_set_string(fba->account, tmp, buddy_icon_url);
			g_free(tmp);

			/* Turn the \/ into / */
			tmp = g_strcompress(buddy_icon_url);

			if (g_str_equal(tmp, "http://static.ak.fbcdn.net/pics/q_silhouette.gif"))
			{
				/* User has no icon */
				purple_buddy_icons_set_for_user(fba->account,
						purple_buddy_get_name(buddy), NULL, 0, NULL);
			}
			else
			{
				/* small icon at http://profile.ak.facebook.com/profile6/1845/74/q800753867_2878.jpg */
				/* bigger icon at http://profile.ak.facebook.com/profile6/1845/74/n800753867_2878.jpg */
				search_tmp = strstr(tmp, "/q");
				if (search_tmp)
					*(search_tmp + 1) = 'n';
				
				/* Fetch their icon */
				fb_post_or_get(fba, FB_METHOD_GET, "profile.ak.facebook.com",
						tmp + strlen("http://profile.ak.facebook.com"), NULL,
						buddy_icon_cb, g_strdup(purple_buddy_get_name(buddy)),
					FALSE);
			}
			g_free(tmp);
		}
		g_free(buddy_icon_url);

		if (current_buddy_online)
		{
			/* Add buddy to the list of online buddies */
			online_buddies_list = g_slist_append(online_buddies_list, buddy);

			/* Update the display of the buddy in the buddy list and make the user online */
			if (!PURPLE_BUDDY_IS_ONLINE(buddy))
				purple_prpl_got_user_status(fba->account, buddy->name, purple_primitive_get_id_from_type(PURPLE_STATUS_AVAILABLE), NULL);
		}

		/* Move pointer after any user configurable data */
		search_start = search_tmp;
		/* Move pointer to the end of the buddy entry */
		search_start = strchr(largest_buddy_search_point, '}') + 1;
		while (*search_start == ',' && (search_start - data < data_len))
			search_start++;
	}

	buddies_list = purple_find_buddies(fba->account, NULL);
	if (buddies_list != NULL)
	{
		g_slist_foreach(buddies_list, (GFunc)set_buddies_offline, online_buddies_list);
		g_slist_free(buddies_list);
	}
	g_slist_free(online_buddies_list);
}

gboolean fb_get_buddy_list(gpointer data)
{
	FacebookAccount *fba;
	gchar *postdata;

	fba = data;

	postdata = g_strdup_printf(
			"user=%d&popped_out=true&force_render=true&buddy_list=1",
			fba->uid);
	fb_post_or_get(fba, FB_METHOD_POST, NULL, "/ajax/presence/update.php",
			postdata, got_buddy_list_cb, NULL, FALSE);
	g_free(postdata);

	return TRUE;
}

void fb_blist_poke_buddy(PurpleBlistNode *node, gpointer data)
{
	PurpleBuddy *buddy;
	FacebookBuddy *fbuddy;
	FacebookAccount *fba;
	gchar *postdata;
	
	if(!PURPLE_BLIST_NODE_IS_BUDDY(node))
		return;
	buddy = (PurpleBuddy *) node;
	if (!buddy)
		return;
	fbuddy = buddy->proto_data;
	if (!fbuddy)
		return;
	fba = fbuddy->fba;
	if (!fba)
		return;
	
	postdata = g_strdup_printf("uid=%d&pokeback=0&post_form_id=%s", fbuddy->uid, fba->post_form_id);
	
	fb_post_or_get(fba, FB_METHOD_POST, NULL, "/ajax/poke.php",
				postdata, NULL, NULL, FALSE);
	
	g_free(postdata);
}

