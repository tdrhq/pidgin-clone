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
#include "fb_util.h"
#include "fb_friendlist.h"
#include "blist.h"

static void set_buddies_offline(PurpleBuddy *buddy,
		GHashTable *online_buddies_list)
{
	if (PURPLE_BUDDY_IS_ONLINE(buddy) &&
		g_hash_table_lookup(online_buddies_list, buddy->name) == NULL)
	{
		purple_prpl_got_user_status(buddy->account, buddy->name,
				purple_primitive_get_id_from_type(
					PURPLE_STATUS_OFFLINE),
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

	g_return_if_fail(buddy != NULL);

	fbuddy = buddy->proto_data;

	g_return_if_fail(fbuddy != NULL);

	buddy_icon_data = g_memdup(data, data_len);

	purple_buddy_icons_set_for_user(fba->account, buddy->name,
			buddy_icon_data, data_len, fbuddy->thumb_url);
}

/**
 * Find buddy names 
 */
static GList *get_buddies(FacebookAccount *fba, const gchar *uid,
		const gchar *name, JsonArray *friend_list_ids)
{
	GList *buddies;
	GList *cur;

	buddies = fb_get_buddies_friend_list(fba, uid, friend_list_ids);

	// Initialize proto data for each buddy.
	for (cur = buddies; cur != NULL; cur = cur->next)
	{
		PurpleBuddy *buddy;

		buddy = (PurpleBuddy *) cur->data;

		/* Set the FacebookBuddy structure */
		if (buddy->proto_data == NULL)
		{
			FacebookBuddy *fbuddy;
			gchar *buddy_icon_url;

			fbuddy = g_new0(FacebookBuddy, 1);
			fbuddy->buddy = buddy;
			fbuddy->fba = fba;
			fbuddy->uid = atoll(uid);
			fbuddy->name = g_strdup(name);

			// load the old buddy icon url from the icon 'checksum'
			buddy_icon_url = (char *)
				purple_buddy_icons_get_checksum_for_user(buddy);
			if (buddy_icon_url != NULL)
				fbuddy->thumb_url = g_strdup(buddy_icon_url);

			buddy->proto_data = fbuddy;
		}
	}

	return buddies;
}

static gboolean process_buddy_status(FacebookAccount *fba, PurpleBuddy *buddy,
	JsonObject *userInfo)
{
	FacebookBuddy *fbuddy;
	gboolean status_changed;

	status_changed = FALSE;
	fbuddy = buddy->proto_data;

	if (json_object_has_member(userInfo, "status"))
	{
		gchar *status_text;
		const gchar *status_time_text;

		status_time_text = json_node_get_string(
			json_object_get_member(userInfo, "statusTimeRel"));
		status_text = fb_strdup_withhtml(json_node_get_string(
				json_object_get_member(userInfo, "status")));

		/* set our last known status so that we don't re-set it */
		if (!fba->last_status_message &&
		    atoll(buddy->name) == fba->uid) {
			fba->last_status_message = g_strdup(status_text);
		}

		if (strlen(status_time_text) == 0) {
			status_time_text = NULL;
		}

		g_free(fbuddy->status_rel_time);
		if (status_time_text != NULL) {
			fbuddy->status_rel_time = 
				fb_strdup_withhtml(status_time_text);
		} else {
			fbuddy->status_rel_time = NULL;
		}

		/* if the buddy status has changed, update the contact list */
		if (fbuddy->status == NULL ||
			!g_str_equal(fbuddy->status, status_text))
		{
			g_free(fbuddy->status);
			fbuddy->status = g_strdup(status_text);
			status_changed = TRUE;
		}

		g_free(status_text);
	} else {
		if (fbuddy->status != NULL) {
			g_free(fbuddy->status);
			fbuddy->status = NULL;
			status_changed = TRUE;
		}
	}

	return status_changed;
}

static void process_buddy_icon(FacebookAccount *fba, PurpleBuddy *buddy,
	JsonObject *userInfo)
{
	FacebookBuddy *fbuddy;
	gchar *buddy_icon_url;

	fbuddy = buddy->proto_data;
	
	/* Set the buddy icon (if it hasn't changed) */
	buddy_icon_url = json_node_dup_string(json_object_get_member(
			userInfo, "thumbSrc"));
	if (fbuddy->thumb_url == NULL ||
	    !g_str_equal(fbuddy->thumb_url, buddy_icon_url))
	{
		g_free(fbuddy->thumb_url);
		if (g_str_equal(buddy_icon_url,
			"http://static.ak.fbcdn.net/pics/q_silhouette.gif"))
		{
			fbuddy->thumb_url = NULL;
			/* User has no icon */
			purple_buddy_icons_set_for_user(fba->account,
				purple_buddy_get_name(buddy), NULL, 0, NULL);
		}
		else
		{
			gchar *search_tmp;

			fbuddy->thumb_url = g_strdup(buddy_icon_url);

			/* small icon at http://profile.ak.facebook.com/profile6/1845/74/q800753867_2878.jpg */
			/* bigger icon at http://profile.ak.facebook.com/profile6/1845/74/n800753867_2878.jpg */
			search_tmp = strstr(buddy_icon_url, "/q");
			if (search_tmp)
				*(search_tmp + 1) = 'n';
				
			/* Fetch their icon */
			fb_post_or_get(fba, FB_METHOD_GET, "profile.ak.facebook.com",
					buddy_icon_url, NULL,
					buddy_icon_cb, g_strdup(purple_buddy_get_name(buddy)), FALSE);
		}
	}
	g_free(buddy_icon_url);
}

static void process_buddies(FacebookAccount *fba, GHashTable *online_buddies_list,
	JsonObject *nowAvailableList, gchar *uid, JsonObject *userInfo)
{
	const gchar *name;
	gboolean idle;
	GList *buddies, *cur;
	gboolean current_buddy_online;

	JsonArray *friend_list_ids;

	friend_list_ids = NULL;
	name = json_node_get_string(json_object_get_member(userInfo, "name"));

	/* look for "uid":{"i":_____} */
	if (json_object_has_member(nowAvailableList, uid))
	{
		JsonObject *userBlistInfo;
		userBlistInfo = json_node_get_object(
			json_object_get_member(nowAvailableList, uid));
		idle = json_node_get_boolean(
			json_object_get_member(userBlistInfo, "i"));
		if (json_object_has_member(userBlistInfo, "fl")) {
			friend_list_ids = json_node_get_array(
				json_object_get_member(userBlistInfo, "fl"));
		}

		current_buddy_online = TRUE;
	} else {
		/* if we're here, the buddy's info has been sent, 
		 * but they're not actually online */
		current_buddy_online = FALSE;
		idle = FALSE;
	}

	/* is this us? */
	if (atoll(uid) == fba->uid)
	{
		purple_connection_set_display_name(fba->pc, name);

		/* check that we don't want to show ourselves */
		current_buddy_online = !purple_account_get_bool(
			fba->account, "facebook_hide_self", TRUE);
	}

	buddies = get_buddies(fba, uid, name, friend_list_ids);
	for (cur = buddies; cur != NULL; cur = cur->next)
	{
		PurpleBuddy *buddy;
		gboolean status_changed;

		buddy = (PurpleBuddy *)cur->data;

		process_buddy_icon(fba, buddy, userInfo);
		status_changed = process_buddy_status(fba, buddy, userInfo);

		purple_presence_set_idle(purple_buddy_get_presence(buddy),
				idle, 0);

		if (current_buddy_online)
		{
			/* Add buddy to the list of online buddies */
			g_hash_table_insert(online_buddies_list, buddy->name, buddy);

			// Set buddy as online in buddy list.  We check for several
			// conditions before doing this, because if we set it always
			// Pidgin has a bug where the logs go nuts with "x is online".
			if (!PURPLE_BUDDY_IS_ONLINE(buddy) ||
			    status_changed ||
			    idle != purple_presence_is_idle(
				purple_buddy_get_presence(buddy)))
			{
				purple_prpl_got_user_status(fba->account, buddy->name,
					purple_primitive_get_id_from_type(
						idle ? PURPLE_STATUS_AWAY :
					       	PURPLE_STATUS_AVAILABLE), NULL);
			}
		}
	}

	/* update the blist if we have no previous alias */
	fb_blist_set_alias(fba, uid, name);
}

static void process_notifications(FacebookAccount *fba,
		JsonObject *notifications)
{
	if (notifications != NULL &&
	    purple_account_get_check_mail(fba->account))
	{
		JsonNode *inboxCount_node = json_object_get_member(
			notifications, "inboxCount");
		if (inboxCount_node) {
			gint inbox_count = json_node_get_int(inboxCount_node);
			if (inbox_count &&
			    inbox_count != fba->last_inbox_count) {
				fba->last_inbox_count = inbox_count;
				gchar *url = g_strdup("http://www.facebook.com/inbox/");
				purple_notify_emails(
					fba->pc, inbox_count,
					FALSE, NULL, NULL,
					(const char**) &(fba->account->username),
					(const char**) &(url), NULL, NULL);
				g_free(url);
			}
		}
	}
}

static void got_buddy_list_cb(FacebookAccount *fba, gchar *data,
		gsize data_len, gpointer userdata)
{
	GSList *buddies_list;
	GHashTable *online_buddies_list = g_hash_table_new(
			g_str_hash, g_str_equal);
	gchar *uid;

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

	gchar *error = NULL;
	JsonObject *objnode = fb_get_json_object(parser, &error);
	if (error) {
		purple_connection_error_reason(
				fba->pc,
				PURPLE_CONNECTION_ERROR_NETWORK_ERROR,
				error);
			g_object_unref(parser);
			return;
	}
	
	/* look for "userInfos":{ ... }, */
	if (!json_object_has_member(objnode, "payload"))
	{
		g_object_unref(parser);
		return;
	}
	objnode = json_node_get_object(json_object_get_member(
			objnode, "payload"));
	if (!json_object_has_member(objnode, "buddy_list"))
	{
		g_object_unref(parser);
		return;
	}
	JsonObject *buddy_list = json_node_get_object(json_object_get_member(
			objnode, "buddy_list"));
	if (!json_object_has_member(buddy_list, "userInfos"))
	{
		g_object_unref(parser);
		return;
	}

	fb_process_friend_lists(fba, buddy_list);

	// Iterate through the list of buddy infos sent to us.	
	JsonObject *userInfos;
	JsonObject *nowAvailableList;
	userInfos = json_node_get_object(json_object_get_member(
			buddy_list, "userInfos"));
	nowAvailableList = json_node_get_object(json_object_get_member(
			buddy_list, "nowAvailableList"));
	GList *userIds;
	userIds = json_object_get_members(userInfos);
	GList *currentUserNode;
	for(	currentUserNode = userIds;
		currentUserNode;
		currentUserNode = g_list_next(currentUserNode))
	{
		uid = currentUserNode->data;

		JsonObject *userInfo;
		userInfo = json_node_get_object(json_object_get_member(
					userInfos, uid));
		// Process the user, which generally consists of updating
		// state info such as name, idle item, status message,etc.
		process_buddies(fba, online_buddies_list, nowAvailableList,
				uid, userInfo);
	}
	g_list_free(userIds);

	// Set users offline.  We do this in a seperate function because FB
	// only sends us a list of users who are online.  We find the users
	// that are not in the union of of buddy list users + online, and
	// mark them as offline.
	buddies_list = purple_find_buddies(fba->account, NULL);
	if (buddies_list != NULL)
	{
		g_slist_foreach(
			buddies_list,
			(GFunc)set_buddies_offline, online_buddies_list);
		g_slist_free(buddies_list);
	}
	g_hash_table_destroy(online_buddies_list);
	
	// The buddy list also contains notifications data.  Process and
	// display is appropriate.
	process_notifications(fba, json_node_get_object(
		json_object_get_member(objnode, "notifications")));

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

void fb_blist_init(FacebookAccount *fba)
{
	fb_friendlist_init(fba);

	fb_get_buddy_list(fba);

	/* periodically check for updates to your buddy list */
	fba->buddy_list_timer = purple_timeout_add_seconds(60,
			fb_get_buddy_list, fba);

}

void fb_blist_destroy(FacebookAccount *fba)
{
	if (fba->buddy_list_timer) {
		purple_timeout_remove(fba->buddy_list_timer);
	}

	fb_friendlist_destroy(fba);
}
