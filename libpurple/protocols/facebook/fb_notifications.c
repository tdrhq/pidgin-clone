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

#include "fb_notifications.h"
#include "fb_connection.h"

static void fb_got_notifications_cb(FacebookAccount *fba, gchar *url_text, gsize len, gpointer userdata)
{
	time_t last_fetch_time;
	time_t time_of_message;
	time_t newest_message = 0;
	gchar *tmp;
	gchar month_string[4], weekday[4];
	guint year, month, day, hour, minute, second;
	long timezone;
	gchar *subject, *url, *stripped_subject;
	gchar *search_start = (gchar *)url_text;

	month_string[3] = weekday[3] = '\0';
	year = month = day = hour = minute = second = 0;

	if (!url_text || !len)
		return;

	last_fetch_time = purple_account_get_int(fba->account, "facebook_notifications_last_fetch", 0);
	/* purple_debug_info("facebook", "last fetch time: %zu\n", last_fetch_time); */

	while (search_start && (search_start = strstr(search_start, "<item>")))
	{
		search_start = search_start + 6;

		tmp = strstr(search_start, "<pubDate>");
		if (!tmp)
		{
			purple_debug_error("facebook", "couldn't find date in rss feed\n");
			return;
		}
		tmp += 9;
		tmp = g_strndup(tmp, strchr(tmp, '<')-tmp);

		/* rss times are in Thu, 19 Jun 2008 15:51:25 -1100 format */
		/* purple_debug_info("facebook", "pre time: %s\n", tmp); */
		sscanf(tmp, "%3s, %2u %3s %4u %2u:%2u:%2u %5ld", (char*)&weekday, &day, (char*)&month_string, &year, &hour, &minute, &second, &timezone);
		if (g_str_equal(month_string, "Jan")) month = 0;
		else if (g_str_equal(month_string, "Feb")) month = 1;
		else if (g_str_equal(month_string, "Mar")) month = 2;
		else if (g_str_equal(month_string, "Apr")) month = 3;
		else if (g_str_equal(month_string, "May")) month = 4;
		else if (g_str_equal(month_string, "Jun")) month = 5;
		else if (g_str_equal(month_string, "Jul")) month = 6;
		else if (g_str_equal(month_string, "Aug")) month = 7;
		else if (g_str_equal(month_string, "Sep")) month = 8;
		else if (g_str_equal(month_string, "Oct")) month = 9;
		else if (g_str_equal(month_string, "Nov")) month = 10;
		else if (g_str_equal(month_string, "Dec")) month = 11;
		g_free(tmp);

		/* try using pidgin's functions */
		tmp = g_strdup_printf("%04u%02u%02uT%02u%02u%02u%05ld", year, month, day, hour, minute, second, timezone);
		time_of_message = purple_str_to_time(tmp, FALSE, NULL, NULL, NULL);
		g_free(tmp);

		if (time_of_message <= 0)
		{
			/* there's no cross-platform, portable way of converting string to time */
			/* that doesn't need a new version of glib, so just cheat */
			time_of_message = second + 60*minute + 3600*hour + 86400*day + 2592000*month + 31536000*(year-1970);
		}

		/* purple_debug_info("facebook", "time of message: %zu\n", time_of_message); */
		/* purple_debug_info("facebook", "time of message: %s\n", ctime(&time_of_message)); */

		if (time_of_message > newest_message)
		{
			/* we'll keep the newest message to save */
			newest_message = time_of_message;
		}

		if (time_of_message <= last_fetch_time)
		{
			/* fortunatly, rss messages are ordered from newest to oldest */
			/* so if this message is older than the last one, ignore rest */
			break;
		}

		tmp = strstr(search_start, "<link>");
		if (!tmp)
		{
			url = g_strdup("");
		} else {
			tmp += 6;
			url = g_strndup(tmp, strchr(tmp, '<')-tmp);
			/* convert &amp; to & */
			tmp = purple_unescape_html(url);
			g_free(url);
			url = tmp;
		}

		tmp = strstr(search_start, "<title>");
		if (!tmp)
		{
			subject = g_strdup("");
		} else {
			tmp += 7;
			subject = g_strndup(tmp, strchr(tmp, '<')-tmp);
		}
		stripped_subject = purple_unescape_html(subject);
		g_free(subject);
		subject = stripped_subject;
		
		purple_debug_info("facebook", "subject: %s\n", subject);

		purple_notify_email(fba->pc, subject, NULL, fba->account->username, url, NULL, NULL);
		g_free(subject);
		g_free(url);

		search_start = strstr(search_start, "</item>");
	}

	if (newest_message > last_fetch_time)
	{
		/* update the last fetched time if we had newer messages */
		purple_account_set_int(fba->account, "facebook_notifications_last_fetch", newest_message);
	}
}

static void find_feed_url_cb(FacebookAccount *fba, gchar *data, gsize data_len, gpointer userdata)
{
	const gchar *search_string = "/feeds/notifications.php";
	gchar *feed_url;
	gchar *stripped;

	purple_debug_info("facebook", "find_feed_url_cb\n");

	if (!data)
		data = "(null)";

	feed_url = g_strstr_len(data, data_len, search_string);
	if (!feed_url)
	{
		purple_debug_error("facebook", "received data, but could not find url on page\n");
		return;
	}

	feed_url = g_strndup(feed_url, strchr(feed_url, '"') - feed_url);

	/* convert &amp; to & */
	stripped = purple_unescape_html(feed_url);
	g_free(feed_url);
	feed_url = stripped;
	
	purple_debug_info("facebook", "parsed feed url %s\n", feed_url);

	if (feed_url && *feed_url)
	{
		purple_account_set_string(fba->account, "notifications_feed_url", feed_url);
		fb_get_notifications_feed(fba);
	}
}

static void fb_find_feed_url(FacebookAccount *fba)
{
	fb_post_or_get(fba, FB_METHOD_GET, NULL, "/notifications.php", NULL,
			find_feed_url_cb, NULL, FALSE);
}

gboolean fb_get_notifications_feed(FacebookAccount *fba)
{
	const gchar *feed_url;

	if (purple_account_get_bool(fba->account, "facebook_get_notifications", TRUE)) {
		feed_url = purple_account_get_string(fba->account, "notifications_feed_url", NULL);
		if (!feed_url)
		{
			purple_debug_info("facebook", "no notifications feed url available, searching for it\n");
			fb_find_feed_url(fba);
			return TRUE;
		}

		fb_post_or_get(fba, FB_METHOD_GET, NULL, feed_url, NULL, fb_got_notifications_cb, NULL, FALSE);
	}

	return TRUE;
}
