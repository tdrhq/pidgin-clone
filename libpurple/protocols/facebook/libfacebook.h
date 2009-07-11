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

#ifndef LIBFACEBOOK_H
#define LIBFACEBOOK_H

#define FACEBOOK_PLUGIN_VERSION "1.53"
#define FACEBOOK_PLUGIN_ID "prpl-bigbrownchunx-facebookim"

#include <glib.h>

#include <errno.h>
#include <string.h>
#include <glib/gi18n.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef G_GNUC_NULL_TERMINATED
#	if __GNUC__ >= 4
#		define G_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
#	else
#		define G_GNUC_NULL_TERMINATED
#	endif /* __GNUC__ >= 4 */
#endif /* G_GNUC_NULL_TERMINATED */

#ifdef _WIN32
#	include "win32dep.h"
#	define dlopen(a,b) LoadLibrary(a)
#	define RTLD_LAZY
#	define dlsym(a,b) GetProcAddress(a,b)
#	define dlclose(a) FreeLibrary(a)
#else
#	include <arpa/inet.h>
#	include <dlfcn.h>
#	include <netinet/in.h>
#	include <sys/socket.h>
#endif

#ifndef PURPLE_PLUGINS
#	define PURPLE_PLUGINS
#endif

#include "accountopt.h"
#include "connection.h"
#include "debug.h"
#include "dnsquery.h"
#include "proxy.h"
#include "prpl.h"
#include "request.h"
#include "sslconn.h"
#include "version.h"

#ifdef HAVE_ZLIB
#	include <zlib.h>
#endif

#define FB_MAX_MSG_RETRY 2

#include <json-glib/json-glib.h>

typedef struct _FacebookAccount FacebookAccount;
typedef struct _FacebookBuddy FacebookBuddy;

typedef void (*FacebookProxyCallbackFunc)(FacebookAccount *fba, gchar *data, gsize data_len, gpointer user_data);

struct _FacebookAccount {
	PurpleAccount *account;
	PurpleConnection *pc;
	GSList *conns; /**< A list of all active FacebookConnections */
	GSList *dns_queries;
	GHashTable *cookie_table;
	gchar *post_form_id;
	gint64 uid;
	guint buddy_list_timer;
	guint friend_request_timer;
	gchar *channel_number;
	guint message_fetch_sequence;
	gint64 last_message_time;
	GSList *resending_messages;
	GHashTable *auth_buddies;
	GHashTable *hostname_ip_cache;
	guint notifications_timer;
	time_t last_messages_download_time;
	guint new_messages_check_timer;
	guint perpetual_messages_timer;
	gchar *last_status_message;
	gboolean is_idle;
	GHashTable *sent_messages_hash;
	gint last_inbox_count;
};

struct _FacebookBuddy {
	FacebookAccount *fba;
	PurpleBuddy *buddy;
	gint64 uid;
	gchar *name;
	gchar *status;
	gchar *status_rel_time;
	gchar *thumb_url;
};

/* TODO: move util functions into a utils file */
gchar *fb_strdup_withhtml(const gchar *src);
gchar *fb_convert_unicode(const gchar *input);
JsonParser *fb_get_parser(const gchar *data, gsize data_len);
gint64 fb_time_kludge(int initial_time);

#endif /* LIBFACEBOOK_H */
