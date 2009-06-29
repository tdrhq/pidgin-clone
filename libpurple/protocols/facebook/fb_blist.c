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

#include <json-glib/json-glib.h>

static void set_buddies_offline(PurpleBuddy *buddy, GHashTable *online_buddies_list)
{
	if (PURPLE_BUDDY_IS_ONLINE(buddy) &&
		g_hash_table_lookup(online_buddies_list, buddy->name) == NULL)
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
	FacebookBuddy *fbuddy;
	gpointer buddy_icon_data;

	buddyname = user_data;

	purple_debug_info("facebook",
			"buddy icon for buddy %s %" G_GSIZE_FORMAT "\n",
			buddyname, data_len);

	buddy = purple_find_buddy(fba->account, buddyname);
	g_free(buddyname);
	if (buddy == NULL)
		return;

	fbuddy = buddy->proto_data;

	buddy_icon_data = g_memdup(data, data_len);

	purple_buddy_icons_set_for_user(fba->account, buddy->name,
			buddy_icon_data, data_len, fbuddy->thumb_url);
}

static void got_buddy_list_cb(FacebookAccount *fba, gchar *data,
		gsize data_len, gpointer userdata)
{
	GSList *buddies_list;
	GHashTable *online_buddies_list = g_hash_table_new(g_str_hash, g_str_equal);
	PurpleBuddy *buddy;
	FacebookBuddy *fbuddy;
	gchar *uid;
	gchar *name;
	gchar *status_text;
	gchar *status_time_text;
	gchar *buddy_icon_url;
	gboolean idle;
	guint32 error_number;

	gchar *search_tmp;
	gchar *tmp;

	PurpleGroup *fb_group = NULL;

	gboolean current_buddy_online = FALSE;

	purple_debug_info("facebook", "parsing buddy list\n");

	if (fba == NULL)
		return;

	JsonParser *parser = fb_get_parser(data, data_len);
	if (parser == NULL) {
		purple_connection_error_reason(fba->pc,
				PURPLE_CONNECTION_ERROR_NETWORK_ERROR,
				_("Could not retrieve buddy list"));
		return;
	}
	
	purple_debug_misc("facebook", "buddy list\n%s\n", data);
	
	JsonNode *root;
	root = json_parser_get_root(parser);
	JsonObject *objnode;
	objnode = json_node_get_object(root);

	/* Check if the facebook group already exists (fixes #13) */
	fb_group = purple_find_group("Facebook");

	/* if logged out, this comes up */
	/* for (;;);{"error":1357001,"errorSummary":"Not Logged In",
		"errorDescription":"You must be logged in to do that.",
		"payload":null,"bootload":[{"name":"js\/common.js.pkg.php",
		"type":"js","src":"http:\/\/static.ak.fbcdn.net\/rsrc.php\/pkg\/59\
		/98561\/js\/common.js.pkg.php"}]} */
	if (json_object_has_member(objnode, "error"))
	{
		error_number = json_node_get_int(json_object_get_member(objnode, "error"));
		if (error_number)
		{
			purple_connection_error_reason(fba->pc,
					PURPLE_CONNECTION_ERROR_NETWORK_ERROR,
					json_node_dup_string(json_object_get_member(objnode, "errorDescription")));
			g_object_unref(parser);
			return;
		}
	}

	/* look for "userInfos":{ ... }, */
	if (!json_object_has_member(objnode, "payload"))
	{
		g_object_unref(parser);
		return;
	}
	objnode = json_node_get_object(json_object_get_member(objnode, "payload"));
	if (!json_object_has_member(objnode, "buddy_list"))
	{
		g_object_unref(parser);
		return;
	}
	JsonObject *buddy_list = json_node_get_object(json_object_get_member(objnode, "buddy_list"));
	if (!json_object_has_member(buddy_list, "userInfos"))
	{
		g_object_unref(parser);
		return;
	}
	JsonObject *notifications = json_node_get_object(json_object_get_member(objnode, "notifications"));
	
	JsonObject *userInfos;
	JsonObject *nowAvailableList;
	userInfos = json_node_get_object(json_object_get_member(buddy_list, "userInfos"));
	nowAvailableList = json_node_get_object(json_object_get_member(buddy_list, "nowAvailableList"));
	GList *userIds;
	userIds = json_object_get_members(userInfos);
	GList *currentUserNode;
	for(	currentUserNode = userIds;
		currentUserNode;
		currentUserNode = g_list_next(currentUserNode))
	{
		uid = currentUserNode->data;

		JsonObject *userInfo;
		userInfo = json_node_get_object(json_object_get_member(userInfos, uid));
		name = json_node_dup_string(json_object_get_member(userInfo, "name"));

		/* update the blist if we have no previous alias */
		fb_blist_set_alias(fba, uid, name);

		/* look for "uid":{"i":_____} */
		if (json_object_has_member(nowAvailableList, uid))
		{
			JsonObject *userBlistInfo;
			userBlistInfo = json_node_get_object(json_object_get_member(nowAvailableList, uid));
			idle = json_node_get_boolean(json_object_get_member(userBlistInfo, "i"));
			current_buddy_online = TRUE;
		} else {
			/* if we're here, the buddy's info has been sent, but they're not actually online */
			current_buddy_online = FALSE;
			idle = FALSE;
		}

		/* Set the buddy status text and time */
		if (json_object_has_member(userInfo, "status"))
		{
			status_text = json_node_dup_string(json_object_get_member(userInfo, "status"));
		} else {
			status_text = NULL;
		}

		/* is this us? */
		if (atoll(uid) == fba->uid)
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
		purple_presence_set_idle(purple_buddy_get_presence(buddy), idle, 0);

		/* Set the FacebookBuddy structure */
		if (buddy->proto_data == NULL)
		{
			fbuddy = g_new0(FacebookBuddy, 1);
			fbuddy->buddy = buddy;
			fbuddy->fba = fba;
			fbuddy->uid = atoll(uid);
			fbuddy->name = g_strdup(name);

			/* load the old buddy icon url from the icon 'checksum' */
			buddy_icon_url = (char *)purple_buddy_icons_get_checksum_for_user(buddy);
			if (buddy_icon_url != NULL)
				fbuddy->thumb_url = g_strdup(buddy_icon_url);

			buddy->proto_data = fbuddy;
		} else {
			fbuddy = buddy->proto_data;
		}

		g_free(name);

		if (status_text != NULL)
		{
			tmp = fb_strdup_withhtml(status_text);
			g_free(status_text);
			status_text = tmp;

			status_time_text = json_node_dup_string(json_object_get_member(userInfo, "statusTimeRel"));
			if (strlen(status_time_text) == 0)
			{
				g_free(status_time_text);
				status_time_text = NULL;
			}
			g_free(fbuddy->status_rel_time);
			if (status_time_text != NULL)
			{
				fbuddy->status_rel_time = fb_strdup_withhtml(status_time_text);
				g_free(status_time_text);
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
		buddy_icon_url = json_node_dup_string(json_object_get_member(userInfo, "thumbSrc"));
		if (fbuddy->thumb_url == NULL || !g_str_equal(fbuddy->thumb_url, buddy_icon_url))
		{
			g_free(fbuddy->thumb_url);
			if (g_str_equal(buddy_icon_url, "http://static.ak.fbcdn.net/pics/q_silhouette.gif"))
			{
				fbuddy->thumb_url = NULL;
				/* User has no icon */
				purple_buddy_icons_set_for_user(fba->account,
						purple_buddy_get_name(buddy), NULL, 0, NULL);
			}
			else
			{
				fbuddy->thumb_url = g_strdup(buddy_icon_url);

				/* small icon at http://profile.ak.facebook.com/profile6/1845/74/q800753867_2878.jpg */
				/* bigger icon at http://profile.ak.facebook.com/profile6/1845/74/n800753867_2878.jpg */
				search_tmp = strstr(buddy_icon_url, "/q");
				if (search_tmp)
					*(search_tmp + 1) = 'n';
				
				/* Fetch their icon */
				fb_post_or_get(fba, FB_METHOD_GET, "profile.ak.facebook.com",
						buddy_icon_url + strlen("http://profile.ak.facebook.com"), NULL,
						buddy_icon_cb, g_strdup(purple_buddy_get_name(buddy)), FALSE);
			}
		}
		g_free(buddy_icon_url);

		if (current_buddy_online)
		{
			/* Add buddy to the list of online buddies */
			g_hash_table_insert(online_buddies_list, buddy->name, buddy);

			/* Update the display of the buddy in the buddy list and make the user online */
			if (!PURPLE_BUDDY_IS_ONLINE(buddy))
				purple_prpl_got_user_status(fba->account, buddy->name, purple_primitive_get_id_from_type(PURPLE_STATUS_AVAILABLE), NULL);
		}
	}
	g_list_free(userIds);

	buddies_list = purple_find_buddies(fba->account, NULL);
	if (buddies_list != NULL)
	{
		g_slist_foreach(buddies_list, (GFunc)set_buddies_offline, online_buddies_list);
		g_slist_free(buddies_list);
	}
	g_hash_table_destroy(online_buddies_list);
	
	if (notifications != NULL && purple_account_get_check_mail(fba->account))
	{
		JsonNode *inboxCount_node = json_object_get_member(notifications, "inboxCount");
		if (inboxCount_node)
		{
			gint inbox_count = json_node_get_int(inboxCount_node);
			if (inbox_count && inbox_count != fba->last_inbox_count)
			{
				fba->last_inbox_count = inbox_count;
				gchar *url = g_strdup("http://www.facebook.com/inbox/");
				purple_notify_emails(fba->pc, inbox_count, FALSE, NULL, NULL, (const char**) &(fba->account->username), (const char**) &(url), NULL, NULL);
				g_free(url);
			}
		}
	}
	
	g_object_unref(parser);
}

gboolean fb_get_buddy_list(gpointer data)
{
	FacebookAccount *fba;
	gchar *postdata;

	fba = data;

	postdata = g_strdup_printf(
			"user=%" G_GINT64_FORMAT "&popped_out=true&force_render=true&buddy_list=1&notifications=1",
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
	
	postdata = g_strdup_printf("uid=%" G_GINT64_FORMAT "&pokeback=0&post_form_id=%s", fbuddy->uid, fba->post_form_id);
	
	fb_post_or_get(fba, FB_METHOD_POST, NULL, "/ajax/poke.php",
				postdata, NULL, NULL, FALSE);
	
	g_free(postdata);
}

void fb_blist_set_alias(FacebookAccount *fba, const gchar *id,
		const gchar *name)
{
	const char *current_alias;
	PurpleBuddy *buddy;

	buddy = purple_find_buddy(fba->account, id);
	if (!buddy) {
		return;
	}	

	/* Set an alias if no user-defined alias is set yet.  This provides
	 * a basic name alias for each user which is more useful than a
	 * number.  A small corner case bug here- aliases will not change
	 * in accordance with people changing their names on Facebook.
	 */
	current_alias = purple_buddy_get_alias_only(buddy);
	if (!current_alias) {
		purple_debug_info("facebook", "aliasing %s to %s\n", id, name);
		purple_blist_alias_buddy(buddy, name);
	}

	/* In case user removes an alias, we have the server as fallback */
	serv_got_alias(fba->pc, id, name);
}
