/**
 * @file trepia.c The Trepia protocol plugin
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
#include "gaim.h"
#include "account.h"
#include "accountopt.h"
#include "md5.h"
#include "profile.h"
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* XXX */
#include "multi.h"

#ifndef _WIN32
# include <sys/socket.h>
# include <sys/ioctl.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <net/if_arp.h>
#endif

#define TREPIA_VERSION "2.50"

static GaimPlugin *my_protocol = NULL;

typedef enum
{
	TREPIA_LOGIN          = 'C',
	TREPIA_PROFILE_REQ    = 'D',
	TREPIA_MSG_OUTGOING   = 'F',
	TREPIA_REGISTER       = 'J',
	TREPIA_USER_LIST      = 'L',
	TREPIA_MEMBER_UPDATE  = 'M',
	TREPIA_MEMBER_OFFLINE = 'N',
	TREPIA_MEMBER_PROFILE = 'O',
	TREPIA_MSG_INCOMING   = 'Q'

} TrepiaMessageType;

typedef struct
{
	GaimConnection *gc;

	int fd;

	GString *rxqueue;

	GList *pending_users;
	GHashTable *user_profiles;
	GHashTable *user_ids;

} TrepiaSession;

typedef struct
{
	TrepiaMessageType *type;
	char *tag;

	GHashTable *keys;

	GString *buffer;

} TrepiaParserData;

#define TREPIA_SERVER    "trepia.com"
#define TREPIA_PORT       8201
#define TREPIA_REG_PORT   8209

static const char alphabet[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

static char *
base64_enc(const char *data, int len)
{
	char *dest;
	char *buf;

	buf = dest = g_malloc(4 * len / 3 + 4);

	/* Encode 3 bytes at a time */
	while (len >= 3) {
		buf[0] = alphabet[(data[0] >> 2) & 0x3F];
		buf[1] = alphabet[((data[0] << 4) & 0x30) | ((data[1] >> 4) & 0x0F)];
		buf[2] = alphabet[((data[1] << 2) & 0x3C) | ((data[2] >> 6) & 0x03)];
		buf[3] = alphabet[data[2] & 0x3F];
		data += 3;
		buf += 4;
		len -= 3;
	}

	if (len > 0) {
		buf[0] = alphabet[(data[0] >> 2) & 0x3F];
		buf[1] = alphabet[(data[0] << 4) & 0x30];

		if (len > 1) {
			buf[1] += (data[1] >> 4) & 0x0F;
			buf[2] = alphabet[(data[1] << 2) & 0x3C];
		}

		else
			buf[2] = '=';

		buf[3] = '=';
		buf += 4;
	}

	*buf = '\0';

	return dest;
}


static int
trepia_write(int fd, const char *data, size_t len)
{
	gaim_debug(GAIM_DEBUG_MISC, "trepia", "C: %s%c", data,
			   (data[strlen(data) - 1] == '\n' ? '\0' : '\n'));

	return write(fd, data, len);
}

static void
__clear_user_list(TrepiaSession *session)
{
	GaimBlistNode *gnode, *bnode;
	for(gnode = gaim_get_blist()->root; gnode; gnode = gnode->next) {
		bnode = gnode->child;
		while(bnode) {
			struct buddy *buddy = (struct buddy *)bnode;
			if(GAIM_BLIST_NODE_IS_BUDDY(bnode) &&
					buddy->account == session->gc->account) {
				bnode = bnode->next;
				gaim_blist_remove_buddy(buddy);
			} else {
				bnode = bnode->next;
			}
		}
	}
}

#if 0
static char *
__get_mac_address(const char *ip)
{
	char *mac = NULL;
#ifndef _WIN32
	struct sockaddr_in sin = { 0 };
	struct arpreq myarp = { { 0 } };
	int sockfd;
	unsigned char *ptr;

	sin.sin_family = AF_INET;

	if (inet_aton(ip, &sin.sin_addr) == 0) {
		gaim_debug(GAIM_DEBUG_ERROR, "trepia", "Invalid IP address %s\n", ip);
		return NULL;
	}

	memcpy(&myarp.arp_pa, &sin, sizeof(myarp.arp_pa));
	strcpy(myarp.arp_dev, "eth0");

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		gaim_debug(GAIM_DEBUG_ERROR, "trepia",
				   "Cannot open socket for retrieving MAC address.\n");
		return NULL;
	}

	if (ioctl(sockfd, SIOCGARP, &myarp) == -1) {
		gaim_debug(GAIM_DEBUG_ERROR, "trepia",
				   "No entry in in arp_cache for %s\n", ip);
		return NULL;
	}

	ptr = &myarp.arp_ha.sa_data[0];

	mac = g_strdup_printf("%x:%x:%x:%x:%x:%x",
						  ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
#else
#endif

	return mac;
}
#endif

/**************************************************************************
 * Callbacks
 **************************************************************************/
#define SET_STRING_FIELD(tag, id) \
	if ((value = gaim_request_fields_get_string(fields, (id))) != NULL) { \
		buf = g_strdup_printf("%s<%s>%s</%s>", temp, (tag), value, (tag)); \
		g_free(temp); \
		temp = buf; \
	}

#define SET_INT_FIELD(tag, id) \
	int_val = gaim_request_fields_get_integer(fields, (id)); \
	buf = g_strdup_printf("%s<%s>%d</%s>", temp, (tag), int_val, (tag)); \
	g_free(temp); \
	temp = buf;

static void
save_profile_cb(GaimConnection *gc, GaimRequestFields *fields)
{
	TrepiaSession *session = gc->proto_data;
	const char *value;
	char *buf, *temp;
	int int_val;

	buf = g_strdup("<K>");
	temp = buf;

	SET_STRING_FIELD("b", "email");
	SET_STRING_FIELD("c", "homepage");
	SET_STRING_FIELD("d", "firstname");
	SET_STRING_FIELD("e", "lastname");
	SET_STRING_FIELD("f", "icq");
	SET_STRING_FIELD("g", "aim");
	SET_STRING_FIELD("h", "msn");
	SET_STRING_FIELD("i", "yahoo");
	SET_INT_FIELD(   "j", "age");

	int_val = gaim_request_fields_get_choice(fields, "gender");

	buf = g_strdup_printf("%s<k>%c</k>", temp, (int_val == 1 ? 'F' : 'M'));
	g_free(temp);
	temp = buf;

	SET_STRING_FIELD("l", "profile");
	SET_STRING_FIELD("n", "country");
	SET_STRING_FIELD("o", "state");
	SET_STRING_FIELD("p", "city");

	buf = g_strdup_printf("%s</K>", temp);

	if (trepia_write(session->fd, buf, strlen(buf)) < 0) {
		gaim_connection_error(session->gc, _("Write error"));
		return;
	}
}

static void
set_profile(GaimConnection *gc)
{
	GaimRequestFields *fields;
	GaimRequestFieldGroup *group;
	GaimRequestField *field;

	fields = gaim_request_fields_new();

	/* Basic Profile group. */
	group = gaim_request_field_group_new(_("Basic Profile"));
	gaim_request_fields_add_group(fields, group);

	/* First Name */
	field = gaim_request_field_string_new("firstname", _("First Name"), NULL,
										  FALSE);
	gaim_request_field_group_add_field(group, field);
	gaim_debug(GAIM_DEBUG_MISC, "trepia", "feld type = %d\n",
			   field->type);

	/* Last Name */
	field = gaim_request_field_string_new("lastname", _("Last Name"), NULL,
										  FALSE);
	gaim_request_field_group_add_field(group, field);

	/* Gender */
	field = gaim_request_field_choice_new("gender", _("Gender"), 0);
	gaim_request_field_choice_add(field, _("Male"));
	gaim_request_field_choice_add(field, _("Female"));
	gaim_request_field_group_add_field(group, field);

	/* Age */
	field = gaim_request_field_int_new("age", _("Age"), 0);
	gaim_request_field_group_add_field(group, field);

	/* Homepage */
	field = gaim_request_field_string_new("homepage", _("Homepage"), NULL,
										  FALSE);
	gaim_request_field_group_add_field(group, field);

	/* E-Mail Address */
	field = gaim_request_field_string_new("email", _("E-Mail Address"), NULL,
										  FALSE);
	gaim_request_field_group_add_field(group, field);

	/* Profile String */
	field = gaim_request_field_string_new("profile",
										  _("Profile Information"), NULL,
										  TRUE);
	gaim_request_field_group_add_field(group, field);


	/* Instant Messagers */
	group = gaim_request_field_group_new(_("Instant Messagers"));
	gaim_request_fields_add_group(fields, group);

	/* AIM */
	field = gaim_request_field_string_new("aim", _("AIM"), NULL, FALSE);
	gaim_request_field_group_add_field(group, field);

	/* ICQ */
	field = gaim_request_field_string_new("icq", _("ICQ UIN"), NULL, FALSE);
	gaim_request_field_group_add_field(group, field);

	/* MSN */
	field = gaim_request_field_string_new("msn", _("MSN"), NULL, FALSE);
	gaim_request_field_group_add_field(group, field);

	/* Yahoo */
	field = gaim_request_field_string_new("yahoo", _("Yahoo"), NULL, FALSE);
	gaim_request_field_group_add_field(group, field);


	/* I'm From */
	group = gaim_request_field_group_new(_("I'm From"));
	gaim_request_fields_add_group(fields, group);

	/* City */
	field = gaim_request_field_string_new("city", _("City"), NULL, FALSE);
	gaim_request_field_group_add_field(group, field);

	/* State */
	field = gaim_request_field_string_new("state", _("State"), NULL, FALSE);
	gaim_request_field_group_add_field(group, field);

	/* Country */
	field = gaim_request_field_string_new("country", _("Country"), NULL, FALSE);
	gaim_request_field_group_add_field(group, field);


	/* Call the dialog. */
	gaim_request_fields(gc, NULL, _("Set your Trepia profile data."),
						NULL, fields,
						_("Save"), G_CALLBACK(save_profile_cb),
						_("Cancel"), NULL, gc);
}

/**************************************************************************
 * Protocol Plugin ops
 **************************************************************************/

static const char *
trepia_list_icon(GaimAccount *a, struct buddy *b)
{
	return "trepia";
}

static void
trepia_list_emblems(struct buddy *b, char **se, char **sw,
				 char **nw, char **ne)
{
	TrepiaProfile *profile = (TrepiaProfile *)b->proto_data;

	if (trepia_profile_get_sex(profile) == 'M')
		*sw = "male";
	else if (trepia_profile_get_sex(profile))
		*sw = "female";
}

static char *
trepia_status_text(struct buddy *b)
{
	TrepiaProfile *profile = (TrepiaProfile *)b->proto_data;
	const char *value;
	char *text = NULL;

	if ((value = trepia_profile_get_profile(profile)) != NULL)
		text = g_markup_escape_text(value, -1);

	return text;
}

static char *
trepia_tooltip_text(struct buddy *b)
{
	TrepiaProfile *profile = (TrepiaProfile *)b->proto_data;
	const char *value;
	const char *first_name, *last_name;
	int int_value;
	char *text = NULL;
	char *tmp, *tmp2;

	text = g_strdup("");

	first_name = trepia_profile_get_first_name(profile);
	last_name  = trepia_profile_get_last_name(profile);

	if (first_name != NULL || last_name != NULL) {
		tmp = g_strdup_printf("<b>Name:</b> %s%s%s\n",
							  (first_name == NULL ? "" : first_name),
							  (first_name == NULL ? "" : " "),
							  (last_name == NULL ? "" : last_name));

		tmp2 = g_strconcat(text, tmp, NULL);
		g_free(tmp);
		g_free(text);
		text = tmp2;
	}

	if ((int_value = trepia_profile_get_age(profile)) != 0) {
		tmp = g_strdup_printf("<b>Age:</b> %d\n", int_value);

		tmp2 = g_strconcat(text, tmp, NULL);
		g_free(tmp);
		g_free(text);
		text = tmp2;
	}

	tmp = g_strdup_printf("<b>Gender:</b> %s\n",
			(trepia_profile_get_sex(profile) == 'F' ? "Female" : "Male"));

	tmp2 = g_strconcat(text, tmp, NULL);
	g_free(tmp);
	g_free(text);
	text = tmp2;

	if ((value = trepia_profile_get_city(profile)) != NULL) {
		tmp = g_strdup_printf("<b>City:</b> %s\n", value);

		tmp2 = g_strconcat(text, tmp, NULL);
		g_free(tmp);
		g_free(text);
		text = tmp2;
	}

	if ((value = trepia_profile_get_state(profile)) != NULL) {
		tmp = g_strdup_printf("<b>State:</b> %s\n", value);

		tmp2 = g_strconcat(text, tmp, NULL);
		g_free(tmp);
		g_free(text);
		text = tmp2;
	}

	if ((value = trepia_profile_get_country(profile)) != NULL) {
		tmp = g_strdup_printf("<b>Country:</b> %s\n", value);

		tmp2 = g_strconcat(text, tmp, NULL);
		g_free(tmp);
		g_free(text);
		text = tmp2;
	}

	if ((value = trepia_profile_get_profile(profile)) != NULL) {
		char *escaped_val = g_markup_escape_text(value, -1);

		tmp = g_strdup_printf("<b>Profile:</b> %s\n", escaped_val);

		g_free(escaped_val);

		tmp2 = g_strconcat(text, tmp, NULL);
		g_free(tmp);
		g_free(text);
		text = tmp2;
	}

	text[strlen(text) - 1] = '\0';

	return text;
}

static GList *
trepia_actions(GaimConnection *gc)
{
	GList *m = NULL;
	struct proto_actions_menu *pam;

	pam = g_new0(struct proto_actions_menu, 1);
	pam->label = _("Set Profile");
	pam->callback = set_profile;
	pam->gc = gc;
	m = g_list_append(m, pam);

	return m;
}

static void
__free_parser_data(gpointer user_data)
{
#if 0
	TrepiaParserData *data = user_data;

	if (data->buffer != NULL)
		g_string_free(data->buffer, TRUE);

	if (data->tag != NULL)
		g_free(data->tag);

	g_free(data);
#endif
}

static void
__start_element_handler(GMarkupParseContext *context,
						const gchar *element_name,
						const gchar **attribute_names,
						const gchar **attribute_values,
						gpointer user_data, GError **error)
{
	TrepiaParserData *data = user_data;

	if (data->buffer != NULL) {
		g_string_free(data->buffer, TRUE);
		data->buffer = NULL;
	}

	if (*data->type == 0) {
		*data->type = *element_name;
	}
	else {
		data->tag = g_strdup(element_name);
	}
}

static void
__end_element_handler(GMarkupParseContext *context, const gchar *element_name,
					  gpointer user_data,  GError **error)
{
	TrepiaParserData *data = user_data;
	gchar *buffer;

	if (*element_name == *data->type)
		return;

	if (data->buffer == NULL || data->tag == NULL) {
		data->tag = NULL;
		return;
	}

	buffer = g_string_free(data->buffer, FALSE);
	data->buffer = NULL;

	g_hash_table_insert(data->keys, data->tag, buffer);

	data->tag = NULL;
}

static void
__text_handler(GMarkupParseContext *context, const gchar *text,
			   gsize text_len, gpointer user_data, GError **error)
{
	TrepiaParserData *data = user_data;

	if (data->buffer == NULL)
		data->buffer = g_string_new_len(text, text_len);
	else
		g_string_append_len(data->buffer, text, text_len);
}

static GMarkupParser accounts_parser =
{
	__start_element_handler,
	__end_element_handler,
	__text_handler,
	NULL,
	NULL
};

static int
__parse_message(const char *buf, TrepiaMessageType *type, GHashTable **info)
{
	TrepiaParserData *parser_data = g_new0(TrepiaParserData, 1);
	GMarkupParseContext *context;
	GHashTable *keys;

	keys = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
	parser_data->keys = keys;
	parser_data->type = type;

	context = g_markup_parse_context_new(&accounts_parser, 0,
										 parser_data, __free_parser_data);

	if (!g_markup_parse_context_parse(context, buf, strlen(buf), NULL)) {
		g_markup_parse_context_free(context);
		g_free(parser_data);
		g_hash_table_destroy(keys);

		return 1;
	}

	if (!g_markup_parse_context_end_parse(context, NULL)) {
		g_markup_parse_context_free(context);
		g_free(parser_data);
		g_hash_table_destroy(keys);

		return 1;
	}

	g_markup_parse_context_free(context);

	*info = keys;

	return 0;
}

static gboolean
__parse_data(TrepiaSession *session, char *buf)
{
	GHashTable *info;
	GaimAccount *account;
	TrepiaMessageType type = 0;
	TrepiaProfile *profile = NULL;
	int ret;
	char *buffer;
	struct buddy *b;
	int id = 0;
	const char *value;
	char *username;
	int *int_p;

	account = gaim_connection_get_account(session->gc);

	ret = __parse_message(buf, &type, &info);

	if (ret == 1)
		return TRUE;

	if (info != NULL) {
		switch (type) {
			case TREPIA_USER_LIST:
				gaim_connection_set_state(session->gc, GAIM_CONNECTED);
				serv_finish_login(session->gc);
				break;

			case TREPIA_MSG_INCOMING: /* Incoming Message */
				id = atoi(g_hash_table_lookup(info, "a"));

				profile = g_hash_table_lookup(session->user_profiles, &id);
				serv_got_im(session->gc,
							trepia_profile_get_login(profile),
							(char *)g_hash_table_lookup(info, "b"),
							0, time(NULL), -1);
				break;

			case TREPIA_MEMBER_UPDATE:
				profile = trepia_profile_new();

				if ((value = g_hash_table_lookup(info, "a")) != NULL) {
					id = atoi(value);
					trepia_profile_set_id(profile, id);
				}

				if ((value = g_hash_table_lookup(info, "b")) != NULL)
					trepia_profile_set_login_time(profile, atoi(value));

				if ((value = g_hash_table_lookup(info, "c")) != NULL)
					trepia_profile_set_type(profile, atoi(value));
				else
					trepia_profile_set_type(profile, 2);

				session->pending_users = g_list_append(session->pending_users,
													   profile);


#if 0
				if (trepia_profile_get_type(profile) == 1) {
					buffer = g_strdup_printf(
						"<D>"
						"<a>%d</a>"
						"<b>1</b>"
						"</D>",
						id);
				}
				else {
#endif
					buffer = g_strdup_printf(
						"<D>"
						"<a>%d</a>"
						"<b>1</b>"
						"</D>"
						"<D>"
						"<a>%d</a>"
						"<b>2</b>"
						"</D>",
						id,
						id);
#if 0
				}
#endif

				if (trepia_write(session->fd, buffer, strlen(buffer)) < 0) {
					gaim_connection_error(session->gc, _("Write error"));
					g_free(buffer);
					return 1;
				}

				g_free(buffer);
				break;

			case TREPIA_MEMBER_PROFILE:
				if ((value = g_hash_table_lookup(info, "a")) != NULL) {
					GList *l;

					id = atoi(value);

					for (l = session->pending_users; l != NULL; l = l->next) {
						profile = l->data;

						if (trepia_profile_get_id(profile) == id)
							break;

						profile = NULL;
					}
				}
				else
					break;

				if (profile == NULL) {
					profile = g_hash_table_lookup(session->user_profiles, &id);

					if (profile == NULL)
						break;
				}

				/* Age */
				if ((value = g_hash_table_lookup(info, "m")) != NULL)
					trepia_profile_set_age(profile, atoi(value));

				/* ICQ */
				if ((value = g_hash_table_lookup(info, "i")) != NULL)
					trepia_profile_set_icq(profile, atoi(value));

				/* Sex */
				if ((value = g_hash_table_lookup(info, "n")) != NULL)
					trepia_profile_set_sex(profile, *value);

				/* Location */
				if ((value = g_hash_table_lookup(info, "p")) != NULL)
					trepia_profile_set_location(profile, value);

				/* First Name */
				if ((value = g_hash_table_lookup(info, "g")) != NULL)
					trepia_profile_set_first_name(profile, value);

				/* Last Name */
				if ((value = g_hash_table_lookup(info, "h")) != NULL)
					trepia_profile_set_last_name(profile, value);

				/* Profile */
				if ((value = g_hash_table_lookup(info, "o")) != NULL)
					trepia_profile_set_profile(profile, value);

				/* E-mail */
				if ((value = g_hash_table_lookup(info, "e")) != NULL)
					trepia_profile_set_email(profile, value);

				/* AIM */
				if ((value = g_hash_table_lookup(info, "j")) != NULL)
					trepia_profile_set_aim(profile, value);

				/* MSN */
				if ((value = g_hash_table_lookup(info, "k")) != NULL)
					trepia_profile_set_msn(profile, value);

				/* Yahoo */
				if ((value = g_hash_table_lookup(info, "l")) != NULL)
					trepia_profile_set_yahoo(profile, value);

				/* Homepage */
				if ((value = g_hash_table_lookup(info, "f")) != NULL)
					trepia_profile_set_homepage(profile, value);

				/* Country */
				if ((value = g_hash_table_lookup(info, "r")) != NULL)
					trepia_profile_set_country(profile, value);

				/* State */
				if ((value = g_hash_table_lookup(info, "s")) != NULL)
					trepia_profile_set_state(profile, value);

				/* City */
				if ((value = g_hash_table_lookup(info, "t")) != NULL)
					trepia_profile_set_city(profile, value);

				/* Languages */
				if ((value = g_hash_table_lookup(info, "u")) != NULL)
					trepia_profile_set_languages(profile, value);

				/* School */
				if ((value = g_hash_table_lookup(info, "v")) != NULL)
					trepia_profile_set_school(profile, value);

				/* Company */
				if ((value = g_hash_table_lookup(info, "w")) != NULL)
					trepia_profile_set_company(profile, value);

				/* Login Name */
				if ((value = g_hash_table_lookup(info, "d")) != NULL) {
					trepia_profile_set_login(profile, value);
					username = g_strdup(value);
				}
				else if ((value = trepia_profile_get_login(profile)) != NULL) {
					username = g_strdup(value);
				}
				else {
					username = g_strdup_printf("%d", id);
					trepia_profile_set_login(profile, username);
				}

				b = gaim_find_buddy(account, username);

				if (b == NULL) {
					struct group *g;

					g = gaim_find_group(_("Local Users"));

					if (g == NULL) {
						g = gaim_group_new(_("Local Users"));
						gaim_blist_add_group(g, NULL);
					}

					b = gaim_buddy_new(account, username, NULL);

					gaim_blist_add_buddy(b, g, NULL);
				}

				profile->buddy = b;

				b->proto_data = profile;

				session->pending_users = g_list_remove(session->pending_users,
													   profile);

				int_p = g_new0(int, 1);
				*int_p = id;
				g_hash_table_insert(session->user_profiles, int_p, profile);

				serv_got_update(session->gc,
								username, 1, 0,
								trepia_profile_get_login_time(profile), 0, 0);

				/* Buddy Icon */
				if ((value = g_hash_table_lookup(info, "q")) != NULL) {
					char *icon;
					int icon_len;

					frombase64(value, &icon, &icon_len);

					set_icon_data(session->gc, username, icon, icon_len);

					g_free(icon);

					serv_got_update(session->gc, username, 1, 0, 0, 0, 0);
				}

				g_free(username);

				break;

			case TREPIA_MEMBER_OFFLINE:
				if ((value = g_hash_table_lookup(info, "a")) != NULL)
					id = atoi(value);
				else
					break;

				profile = g_hash_table_lookup(session->user_profiles, &id);

				if (profile == NULL)
					break;

				g_hash_table_remove(session->user_profiles, &id);

				b = profile->buddy;

				if (b != NULL)
					serv_got_update(session->gc,
									trepia_profile_get_login(profile),
									0, 0, 0, 0, 0);

				gaim_blist_remove_buddy(b);

				break;

			default:
				break;
		}

		g_hash_table_destroy(info);
	}
	else {
		gaim_debug(GAIM_DEBUG_WARNING, "trepia",
				   "Unknown data received. Possibly an image?\n");
	}

	return TRUE;
}

static void
__data_cb(gpointer data, gint source, GaimInputCondition cond)
{
	TrepiaSession *session = data;
	int i = 0;
	char buf[1025];
	gboolean cont = TRUE;

	i = read(session->fd, buf, 1024);

	if (i <= 0) {
		gaim_connection_error(session->gc, _("Read error"));
		return;
	}

	buf[i] = '\0';

	if (session->rxqueue == NULL)
		session->rxqueue = g_string_new(buf);
	else
		g_string_append(session->rxqueue, buf);

	while (cont) {
		char end_tag[5] = "</ >";
		char *end_s;

		end_tag[2] = session->rxqueue->str[1];

		end_s = strstr(session->rxqueue->str, end_tag);

		if (end_s != NULL) {
			char *buffer;
			size_t len;
			int ret;

			end_s += 4;

			len = end_s - session->rxqueue->str;
			buffer = g_new0(char, len + 1);
			strncpy(buffer, session->rxqueue->str, len);

			g_string_erase(session->rxqueue, 0, len);

			if (*session->rxqueue->str == '\n')
				g_string_erase(session->rxqueue, 0, 1);

			gaim_debug(GAIM_DEBUG_MISC, "trepia", "S: %s\n", buffer);

			ret = __parse_data(session, buffer);

			g_free(buffer);
		}
		else
			break;
	}
}

static void
__login_cb(gpointer data, gint source, GaimInputCondition cond)
{
	TrepiaSession *session = data;
	GaimAccount *account;
	const char *password;
	char *buffer;
	char *mac = "00:01:02:03:04:05";
	char buf[3];
	char md5_password[17];
	md5_state_t st;
	md5_byte_t di[16];
	int i;

	if (source < 0) {
		gaim_connection_error(session->gc, _("Write error"));
		return;
	}

#if 0
	mac = __get_mac_address();
#endif

	session->fd = source;

	account = gaim_connection_get_account(session->gc);

	password = gaim_account_get_password(account);

	md5_init(&st);
	md5_append(&st, (const md5_byte_t *)password, strlen(password));
	md5_finish(&st, di);

	*md5_password = '\0';

	for (i = 0; i < 16; i++) {
		g_snprintf(buf, sizeof(buf), "%02x", di[i]);
		strcat(md5_password, buf);
	}

	buffer = g_strdup_printf(
		"<C>\n"
		"<a>%s</a>\n"
		"<b1></b1>\n"
		"<c>%s</c>\n"
		"<d>%s</d>\n"
		"<e>2.50</e>\n"
		"</C>",
		mac, gaim_account_get_username(account),
		md5_password);

#if 0
	g_free(mac);
#endif

	if (trepia_write(session->fd, buffer, strlen(buffer)) < 0) {
		gaim_connection_error(session->gc, _("Write error"));
		return;
	}

	g_free(buffer);

	session->gc->inpa = gaim_input_add(session->fd, GAIM_INPUT_READ,
									   __data_cb, session);
}

static void
trepia_login(GaimAccount *account)
{
	GaimConnection *gc;
	TrepiaSession *session;
	const char *server;
	int port;
	int i;

	server = gaim_account_get_string(account, "server", TREPIA_SERVER);
	port   = gaim_account_get_int(account,    "port",   TREPIA_PORT);

	gc = gaim_account_get_connection(account);

	session = g_new0(TrepiaSession, 1);
	gc->proto_data = session;
	session->gc = gc;
	session->fd = -1;
	session->user_profiles = g_hash_table_new_full(g_int_hash, g_int_equal,
												   g_free, NULL);

	__clear_user_list(session);

	i = gaim_proxy_connect(account, server, port, __login_cb, session);

	if (i != 0)
		gaim_connection_error(gc, _("Unable to create socket"));
}

static void
trepia_close(GaimConnection *gc)
{
	TrepiaSession *session = gc->proto_data;

	if (session->rxqueue != NULL)
		g_string_free(session->rxqueue, TRUE);

	if (session->gc->inpa)
		gaim_input_remove(session->gc->inpa);

	gaim_debug(GAIM_DEBUG_INFO, "trepia", "Destroying user_profiles\n");
	g_hash_table_destroy(session->user_profiles);
	gaim_debug(GAIM_DEBUG_INFO, "trepia", "Destroying pending_users\n");
	g_list_free(session->pending_users);

	gaim_debug(GAIM_DEBUG_INFO, "trepia", "Closing socket\n");
	close(session->fd);

	g_free(session);

	gc->proto_data = NULL;
	gaim_debug(GAIM_DEBUG_INFO, "trepia", "Leaving trepia_close\n");
}

static int
trepia_send_im(GaimConnection *gc, const char *who, const char *message,
			int len, int flags)
{
	TrepiaSession *session = gc->proto_data;
	TrepiaProfile *profile;
	struct buddy *b;
	char *escaped_msg;
	char *buffer;

	b = gaim_find_buddy(gaim_connection_get_account(gc), who);

	if (b == NULL) {
		gaim_debug(GAIM_DEBUG_ERROR, "trepia",
				   "Unable to send to buddy not on your list!\n");
		return 0;
	}

	profile = b->proto_data;

	escaped_msg = g_markup_escape_text(message, -1);

	buffer = g_strdup_printf(
		"<F>\n"
		"<a>%d</a>\n"
		"<b>%s</b>\n"
		"</F>",
		trepia_profile_get_id(profile), escaped_msg);

	g_free(escaped_msg);

	if (trepia_write(session->fd, buffer, strlen(buffer)) < 0) {
		gaim_connection_error(gc, _("Write error"));
		g_free(buffer);
		return 1;
	}

	return 1;
}

static void
trepia_add_buddy(GaimConnection *gc, const char *name)
{
}

static void
trepia_rem_buddy(GaimConnection *gc, char *who, char *group)
{
}

static void
trepia_buddy_free(struct buddy *b)
{
	if (b->proto_data != NULL) {
		trepia_profile_destroy(b->proto_data);

		b->proto_data = NULL;
	}
}

static void
trepia_set_buddy_icon(GaimConnection *gc, const char *filename)
{
	TrepiaSession *session = gc->proto_data;
	struct stat sb;

	if (!stat(filename, &sb)) {
		FILE *fp;

		if ((fp = fopen(filename, "rb")) != NULL) {
			char *buf = g_malloc(sb.st_size + 1);
			char *temp;
			char *out_buf;

			fread(buf, 1, sb.st_size, fp);

			buf[sb.st_size] = '\0';

			temp = base64_enc(buf, sb.st_size);

			out_buf = g_strdup_printf("<K><m>%s</m></K>", temp);

			g_free(temp);
			g_free(buf);

			fclose(fp);

			if (trepia_write(session->fd, out_buf, strlen(out_buf)) < 0) {
				gaim_connection_error(session->gc, _("Write error"));
				return;
			}
		}
	}
}

static void
trepia_register_user(GaimAccount *account)
{
	char *buffer;
	char *mac = "00:01:02:03:04:05";

	buffer = g_strdup_printf(
		"<J><a>%s</a><b1>%s</b1><c>%s</c><d>%s</d><e>%s</e>"
		"<f></f><g></g><h></h><i></i><j></j><k></k><l></l>"
		"<m></m></J>",
		mac, "", TREPIA_VERSION, gaim_account_get_username(account),
		gaim_account_get_password(account));
}

static GaimPluginProtocolInfo prpl_info =
{
	GAIM_PROTO_TREPIA,
	OPT_PROTO_BUDDY_ICON,
	NULL,
	NULL,
	trepia_list_icon,
	trepia_list_emblems,
	trepia_status_text,
	trepia_tooltip_text,
	NULL,
	trepia_actions,
	NULL,
	NULL,
	trepia_login,
	trepia_close,
	trepia_send_im,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	trepia_add_buddy,
	NULL,
	trepia_rem_buddy,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	trepia_register_user,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	trepia_buddy_free,
	NULL,
	NULL,
	trepia_set_buddy_icon
};

static GaimPluginInfo info =
{
	2,                                                /**< api_version    */
	GAIM_PLUGIN_PROTOCOL,                             /**< type           */
	NULL,                                             /**< ui_requirement */
	0,                                                /**< flags          */
	NULL,                                             /**< dependencies   */
	GAIM_PRIORITY_DEFAULT,                            /**< priority       */

	"prpl-trepia",                                    /**< id             */
	"Trepia",                                         /**< name           */
	VERSION,                                          /**< version        */
	                                                  /**  summary        */
	N_("Trepia Protocol Plugin"),
	                                                  /**  description    */
	N_("Trepia Protocol Plugin"),
	"Christian Hammond <chipx86@gnupdate.org>",       /**< author         */
	WEBSITE,                                          /**< homepage       */

	NULL,                                             /**< load           */
	NULL,                                             /**< unload         */
	NULL,                                             /**< destroy        */

	NULL,                                             /**< ui_info        */
	&prpl_info                                        /**< extra_info     */
};

static void
__init_plugin(GaimPlugin *plugin)
{
	GaimAccountOption *option;

	option = gaim_account_option_string_new(_("Login server"), "server",
											TREPIA_SERVER);
	prpl_info.protocol_options = g_list_append(prpl_info.protocol_options,
											   option);

	option = gaim_account_option_int_new(_("Port"), "port", TREPIA_PORT);
	prpl_info.protocol_options = g_list_append(prpl_info.protocol_options,
											   option);

	my_protocol = plugin;
}

GAIM_INIT_PLUGIN(trepia, __init_plugin, info);
