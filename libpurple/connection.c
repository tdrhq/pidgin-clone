/**
 * @file connection.c Connection API
 * @ingroup core
 */

/* purple
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 */
#include "internal.h"
#include "account.h"
#include "blist.h"
#include "connection.h"
#include "dbus-maybe.h"
#include "debug.h"
#include "log.h"
#include "notify.h"
#include "prefs.h"
#include "proxy.h"
#include "request.h"
#include "server.h"
#include "marshallers.h"
#include "util.h"

/******************************************************************************
 * Structs
 *****************************************************************************/
struct PurpleConnectionPrivate {
	PurpleConnectionFlags flags;

	PurpleConnectionState state;

	PurpleAccount *account;
	gint inpa;

	GSList *buddy_chats;
	
	guint keepalive;

	gboolean wants_to_die;
	guint disconnect_timeout;
};

/******************************************************************************
 * Enums
 *****************************************************************************/
enum {
	PROP_ZERO,
	PROP_FLAGS,
	PROP_STATE,
	PROP_ACCOUNT,
	PROP_INPUT_WATCHER,
	PROP_BUDDY_CHATS,
	PROP_KEEPALIVE,
	PROP_WANTS_TO_DIE,
	PROP_DISCONNECT_TIMEOUT,
	PROP_LAST,
};

enum {
	SIG_SIGNING_ON,
	SIG_SIGNED_ON,
	SIG_SIGNING_OFF,
	SIG_SIGNED_OFF,
	SIG_DISCONNECTED,
};

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;
static guint signals[SIG_LAST] = { 0, };

/******************************************************************************
 * OLD CODE
 *****************************************************************************/
static gboolean
send_keepalive(gpointer data)
{
	PurpleConnection *gc = data;
	PurplePluginProtocolInfo *prpl_info = NULL;

	if (gc != NULL && gc->prpl != NULL)
		prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(gc->prpl);

	if (prpl_info && prpl_info->keepalive)
		prpl_info->keepalive(gc);

	return TRUE;
}

static void
update_keepalive(PurpleConnection *gc, gboolean on)
{
	PurplePluginProtocolInfo *prpl_info = NULL;

	if (gc != NULL && gc->prpl != NULL)
		prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(gc->prpl);

	if (!prpl_info || !prpl_info->keepalive)
		return;

	if (on && !gc->keepalive)
	{
		purple_debug_info("connection", "Activating keepalive.\n");
		gc->keepalive = purple_timeout_add_seconds(30, send_keepalive, gc);
	}
	else if (!on && gc->keepalive > 0)
	{
		purple_debug_info("connection", "Deactivating keepalive.\n");
		purple_timeout_remove(gc->keepalive);
		gc->keepalive = 0;
	}
}

void
purple_connection_new(PurpleAccount *account, gboolean regist, const char *password)
{
	PurpleConnection *gc;
	PurplePlugin *prpl;
	PurplePluginProtocolInfo *prpl_info;

	g_return_if_fail(account != NULL);

	if (!purple_account_is_disconnected(account))
		return;

	prpl = purple_find_prpl(purple_account_get_protocol_id(account));

	if (prpl != NULL)
		prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(prpl);
	else {
		gchar *message;

		message = g_strdup_printf(_("Missing protocol plugin for %s"),
			purple_account_get_username(account));
		purple_notify_error(NULL, regist ? _("Registration Error") :
						  _("Connection Error"), message, NULL);
		g_free(message);
		return;
	}

	if (regist)
	{
		if (prpl_info->register_user == NULL)
			return;
	}
	else
	{
		if (((password == NULL) || (*password == '\0')) &&
			!(prpl_info->options & OPT_PROTO_NO_PASSWORD) &&
			!(prpl_info->options & OPT_PROTO_PASSWORD_OPTIONAL))
		{
			purple_debug_error("connection", "Can not connect to account %s without "
							 "a password.\n", purple_account_get_username(account));
			return;
		}
	}

	gc = g_new0(PurpleConnection, 1);
	PURPLE_DBUS_REGISTER_POINTER(gc, PurpleConnection);

	gc->prpl = prpl;
	if ((password != NULL) && (*password != '\0'))
		gc->password = g_strdup(password);
	purple_connection_set_account(gc, account);
	purple_connection_set_state(gc, PURPLE_CONNECTING);
	connections = g_list_append(connections, gc);
	purple_account_set_connection(account, gc);

	purple_signal_emit(purple_connections_get_handle(), "signing-on", gc);

	if (regist)
	{
		purple_debug_info("connection", "Registering.  gc = %p\n", gc);

		/* set this so we don't auto-reconnect after registering */
		gc->wants_to_die = TRUE;

		prpl_info->register_user(account);
	}
	else
	{
		purple_debug_info("connection", "Connecting. gc = %p\n", gc);

		purple_signal_emit(purple_accounts_get_handle(), "account-connecting", account);
		prpl_info->login(account);
	}
}

void
purple_connection_new_unregister(PurpleAccount *account, const char *password, PurpleAccountUnregistrationCb cb, void *user_data)
{
	/* Lots of copy/pasted code to avoid API changes. You might want to integrate that into the previous function when posssible. */
	PurpleConnection *gc;
	PurplePlugin *prpl;
	PurplePluginProtocolInfo *prpl_info;
	
	g_return_if_fail(account != NULL);
		
	prpl = purple_find_prpl(purple_account_get_protocol_id(account));
	
	if (prpl != NULL)
		prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(prpl);
	else {
		gchar *message;
		
		message = g_strdup_printf(_("Missing protocol plugin for %s"),
								  purple_account_get_username(account));
		purple_notify_error(NULL, _("Unregistration Error"), message, NULL);
		g_free(message);
		return;
	}

	if (!purple_account_is_disconnected(account)) {
		prpl_info->unregister_user(account, cb, user_data);
		return;
	}
	
	if (((password == NULL) || (*password == '\0')) &&
		!(prpl_info->options & OPT_PROTO_NO_PASSWORD) &&
		!(prpl_info->options & OPT_PROTO_PASSWORD_OPTIONAL))
	{
		purple_debug_error("connection", "Can not connect to account %s without "
						   "a password.\n", purple_account_get_username(account));
		return;
	}
	
	gc = g_new0(PurpleConnection, 1);
	PURPLE_DBUS_REGISTER_POINTER(gc, PurpleConnection);
	
	gc->prpl = prpl;
	if ((password != NULL) && (*password != '\0'))
		gc->password = g_strdup(password);
	purple_connection_set_account(gc, account);
	purple_connection_set_state(gc, PURPLE_CONNECTING);
	connections = g_list_append(connections, gc);
	purple_account_set_connection(account, gc);
	
	purple_signal_emit(purple_connections_get_handle(), "signing-on", gc);
	
	purple_debug_info("connection", "Unregistering.  gc = %p\n", gc);
	
	prpl_info->unregister_user(account, cb, user_data);
}

void
purple_connection_destroy(PurpleConnection *gc)
{
	PurpleAccount *account;
	GSList *buddies;
#if 0
	GList *wins;
#endif
	PurplePluginProtocolInfo *prpl_info = NULL;
	gboolean remove = FALSE;

	g_return_if_fail(gc != NULL);

	account = purple_connection_get_account(gc);

	purple_debug_info("connection", "Disconnecting connection %p\n", gc);

	if (purple_connection_get_state(gc) != PURPLE_CONNECTING)
		remove = TRUE;

	purple_signal_emit(purple_connections_get_handle(), "signing-off", gc);

	while (gc->buddy_chats)
	{
		PurpleConversation *b = gc->buddy_chats->data;

		gc->buddy_chats = g_slist_remove(gc->buddy_chats, b);
		purple_conv_chat_left(PURPLE_CONV_CHAT(b));
	}

	update_keepalive(gc, FALSE);

	purple_proxy_connect_cancel_with_handle(gc);

	prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(gc->prpl);
	if (prpl_info->close)
		(prpl_info->close)(gc);

	/* Clear out the proto data that was freed in the prpl close method*/
	buddies = purple_find_buddies(account, NULL);
	while (buddies != NULL) {
		PurpleBuddy *buddy = buddies->data;
		buddy->proto_data = NULL;
		buddies = g_slist_delete_link(buddies, buddies);
	}

	connections = g_list_remove(connections, gc);

	purple_connection_set_state(gc, PURPLE_DISCONNECTED);

	if (remove)
		purple_blist_remove_account(account);

	purple_signal_emit(purple_connections_get_handle(), "signed-off", gc);

#if 0
	/* see comment later in file on if 0'd same code */
	/*
	 * XXX This is a hack! Remove this and replace it with a better event
	 *     notification system.
	 */
	for (wins = purple_get_windows(); wins != NULL; wins = wins->next) {
		PurpleConvWindow *win = (PurpleConvWindow *)wins->data;
		purple_conversation_update(purple_conv_window_get_conversation_at(win, 0),
								 PURPLE_CONV_ACCOUNT_OFFLINE);
	}
#endif

	purple_account_request_close_with_account(account);
	purple_request_close_with_handle(gc);
	purple_notify_close_with_handle(gc);

	purple_debug_info("connection", "Destroying connection %p\n", gc);

	purple_account_set_connection(account, NULL);

	g_free(gc->password);
	g_free(gc->display_name);

	if (gc->disconnect_timeout)
		purple_timeout_remove(gc->disconnect_timeout);

	PURPLE_DBUS_UNREGISTER_POINTER(gc);
	g_free(gc);
}

/*
 * d:)->-<
 *
 * d:O-\-<
 *
 * d:D-/-<
 *
 * d8D->-< DANCE!
 */

void
purple_connection_set_state(PurpleConnection *gc, PurpleConnectionState state)
{
	PurpleConnectionUiOps *ops;
	PurpleConnectionPrivate *pcp;

	g_return_if_fail(gc != NULL);
	
	pcp = gc->priv;

	if (pcp->state == state)
		return;

	pcp->state = state;

	ops = purple_connections_get_ui_ops();

	if (pcp->state == PURPLE_CONNECTING) {
		connections_connecting = g_list_append(connections_connecting, gc);
	}
	else {
		connections_connecting = g_list_remove(connections_connecting, gc);
	}

	if (pcp->state == PURPLE_CONNECTED) {
		PurpleAccount *account;
		PurplePresence *presence;

		account = purple_connection_get_account(gc);
		presence = purple_account_get_presence(account);

		/* Set the time the account came online */
		purple_presence_set_login_time(presence, time(NULL));

		if (purple_prefs_get_bool("/purple/logging/log_system"))
		{
			PurpleLog *log = purple_account_get_log(account, TRUE);

			if (log != NULL)
			{
				char *msg = g_strdup_printf(_("+++ %s signed on"),
											purple_account_get_username(account));
				purple_log_write(log, PURPLE_MESSAGE_SYSTEM,
							   purple_account_get_username(account),
							   purple_presence_get_login_time(presence),
							   msg);
				g_free(msg);
			}
		}

		if (ops != NULL && ops->connected != NULL)
			ops->connected(gc);

		purple_blist_add_account(account);

		purple_signal_emit(purple_connections_get_handle(), "signed-on", gc);

		serv_set_permit_deny(gc);

		update_keepalive(gc, TRUE);
	}
	else if (pcp->state == PURPLE_DISCONNECTED) {
		PurpleAccount *account = purple_connection_get_account(gc);

		if (purple_prefs_get_bool("/purple/logging/log_system"))
		{
			PurpleLog *log = purple_account_get_log(account, FALSE);

			if (log != NULL)
			{
				char *msg = g_strdup_printf(_("+++ %s signed off"),
											purple_account_get_username(account));
				purple_log_write(log, PURPLE_MESSAGE_SYSTEM,
							   purple_account_get_username(account), time(NULL),
							   msg);
				g_free(msg);
			}
		}

		purple_account_destroy_log(account);

		if (ops != NULL && ops->disconnected != NULL)
			ops->disconnected(gc);
	}
}

void
purple_connection_set_account(PurpleConnection *gc, PurpleAccount *account)
{
	g_return_if_fail(gc != NULL);
	g_return_if_fail(account != NULL);

	gc->priv->account = account;
}

void
purple_connection_set_display_name(PurpleConnection *gc, const char *name)
{
	g_return_if_fail(gc != NULL);

	g_free(gc->display_name);
	gc->display_name = g_strdup(name);
}

PurpleConnectionState
purple_connection_get_state(const PurpleConnection *gc)
{
	g_return_val_if_fail(gc != NULL, PURPLE_DISCONNECTED);

	return gc->priv->state;
}

PurpleAccount *
purple_connection_get_account(const PurpleConnection *gc)
{
	g_return_val_if_fail(gc != NULL, NULL);

	return gc->priv->account;
}

const char *
purple_connection_get_password(const PurpleConnection *gc)
{
	g_return_val_if_fail(gc != NULL, NULL);

	return gc->password;
}

const char *
purple_connection_get_display_name(const PurpleConnection *gc)
{
	g_return_val_if_fail(gc != NULL, NULL);

	return gc->display_name;
}

void
purple_connection_update_progress(PurpleConnection *gc, const char *text,
								size_t step, size_t count)
{
	PurpleConnectionUiOps *ops;

	g_return_if_fail(gc   != NULL);
	g_return_if_fail(text != NULL);
	g_return_if_fail(step < count);
	g_return_if_fail(count > 1);

	ops = purple_connections_get_ui_ops();

	if (ops != NULL && ops->connect_progress != NULL)
		ops->connect_progress(gc, text, step, count);
}

void
purple_connection_notice(PurpleConnection *gc, const char *text)
{
	PurpleConnectionUiOps *ops;

	g_return_if_fail(gc   != NULL);
	g_return_if_fail(text != NULL);

	ops = purple_connections_get_ui_ops();

	if (ops != NULL && ops->notice != NULL)
		ops->notice(gc, text);
}

static gboolean
purple_connection_disconnect_cb(gpointer data)
{
	PurpleAccount *account = data;
	char *password = g_strdup(purple_account_get_password(account));
	purple_account_disconnect(account);
	purple_account_set_password(account, password);
	g_free(password);
	return FALSE;
}

void
purple_connection_error(PurpleConnection *gc, const char *text)
{
	PurpleConnectionUiOps *ops;

	g_return_if_fail(gc   != NULL);

	if (text == NULL) {
		purple_debug_error("connection", "purple_connection_error: check `text != NULL' failed\n");
		text = _("Unknown error");
	}

	/* If we've already got one error, we don't need any more */
	if (gc->disconnect_timeout)
		return;

	ops = purple_connections_get_ui_ops();

	if (ops != NULL && ops->report_disconnect != NULL)
		ops->report_disconnect(gc, text);

	gc->disconnect_timeout = purple_timeout_add(0, purple_connection_disconnect_cb,
			purple_connection_get_account(gc));
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
purple_connection_get_property(GObject *obj, guint param_id, GValue *value,
							   GParamSpec *pspec)
{
	PurpleConnection *pc = PURPLE_CONNECTION(obj);

	switch(param_id) {
		case PROP_FLAGS:
			g_value_set_flags(value, purple_connection_get_flags(pc));
			break;
		case PROP_STATE:
			g_value_set_enum(value, purple_connection_get_state(pc));
			break;
		case PROP_ACCOUNT:
			#warning fix me when account is an object
			g_value_set_pointer(value, purple_connection_get_account(pc));
			break;
		case PROP_INPUT_WATCHER:
			g_value_set_int(value, purple_connection_get_input_watcher(pc));
			break;
		case PROP_BUDDY_CHATS:
			g_value_set_pointer(value, purple_connection_get_buddy_chats(pc));
			break;
		case PROP_KEEPALIVE:
			g_value_set_uint(value, purple_connection_get_keepalive(pc));
			break;
		case PROP_WANTS_TO_DIE:
			g_value_set_boolean(value, purple_connection_get_wants_to_die(pc));
			break;
		case PROP_DISCONNECT_TIMEOUT:
			g_value_set_uint(value,
							 purple_connection_get_disconnect_timeout(pc));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
purple_connection_set_property(GObject *obj, guint param_id,
							   const GValue *value, GParamSpec *pspec)
{
	PurpleConnection *pc = PURPLE_CONNECTION(obj);

	switch(param_id) {
		case PROP_FLAGS:
			purple_connection_set_flags(pc, g_value_get_flags(value));
			break;
		case PROP_STATE:
			purple_connection_set_state(pc, g_value_get_enum(value));
			break;
		case PROP_ACCOUNT:
			#warning fix me when account is an object
			purple_connection_set_account(pc, g_value_get_pointer(value));
			break;
		case PROP_INPUT_WATCHER:
			purple_connection_set_input_watcher(pc, g_value_get_int(value));
			break;
		case PROP_BUDDY_CHATS:
			purple_connection_set_buddy_chats(pc, g_value_get_pointer(value));
			break;
		case PROP_KEEPALIVE:
			purple_connection_set_keepalive(pc, g_value_get_uint(value));
			break;
		case PROP_WANTS_TO_DIE:
			purple_connection_set_wants_to_die(pc, g_value_get_boolean(value));
			break;
		case PROP_DISCONNECT_TIMEOUT:
			purple_connection_set_disconnect_timeout(pc,
													 g_value_get_uint(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
purple_connection_finalize(GObject *obj) {
	PurpleConnection *pc = PURPLE_CONNECTION(obj);

	if(PURPLE_IS_ACCOUNT(pc->priv->account))
		g_object_unref(G_OBJECT(pc->priv->account));

	g_free(pc->priv);

	G_OBJECT_CLASS(parent_class)->finalize(obj);
}

static void
purple_connection_init(GTypeInstance *instance, gpointer klass) {
	PurpleConnection *pc = PURPLE_CONNECTION(instance);

	pc->priv = g_new0(PurpleConnectionPrivate, 1);
}

static void
purple_connection_class_init(PurpleConnectionClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);
	GParamSpec *pspec;

	parent_class = g_type_class_peek_parent(klass);

	obj_class->get_property = purple_connection_get_property;
	obj_class->set_property = purple_connection_set_proprety;
	obj_class->finalize = purple_connection_finalize;

	pspec = g_param_spec_enum("flags", "flags",
							  "The flags for this connection.",
							  PURPLE_TYPE_CONNECTION_FLAGS,
							  0,
							  G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
	g_object_class_install_property(obj_class, PROP_FLAGS, pspec);

	pspec = g_param_spec_enum("state", "state",
							  "The state of this connection.",
							  PURPLE_TYPE_CONNECTION_STATE,
							  PURPLE_CONNECTION_STATE_DISCONNECTED,
							  G_PARAM_READWRITE);
	g_object_class_install_property(obj_class, PROP_STATE, pspec);

	#warning This needs to be moved to g_param_spec_object when account is objectified
	pspec = g_param_spec_pointer("account", "account",
								 "The account this connection belongs to.",
								 G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
	g_object_class_install_property(obj_class, PROP_ACCOUNT, pspec);

	pspec = g_param_spec_int("inpa", "inpa",
							 "The input watcher's fd for this connection.",
							 G_MININT, G_MAXINT, -1,
							 G_PARAM_READWRITE);
	g_object_class_install_property(obj_class, PROP_INPA, pspec);

	#warning Does this belong here?  I'm thinking not...
	pspec = g_param_spec_pointer("buddy-chats", "buddy-chats",
								 "A list of buddy chats for this connection.",
								 G_PARAM_READWRITE);
	g_object_class_install_property(obj_class, PROP_BUDDY_CHATS, pspec);

	pspec = g_param_spec_uint("keepalive", "keepalive",
							  "The keepalive value for this connection.",
							  0, G_MAXUINT, 0,
							  G_PARAM_READWRITE);
	g_object_class_install_property(obj_class, PROP_KEEPALIVE, pspec);

	pspec = g_param_spec_boolean("wants-to-die", "wants-to-die",
								 "Whether or not this connection wants to "
								 "die.",
								 FALSE, G_PARAM_READWRITE);
	g_object_class_install_property(obj_class, PROP_WANTS_TO_DIE, pspec);

	pspec = g_param_spec_uint("disconnect-timeout", "disconnect-timeout",
							  "The disconnect timeout handle for this "
							  "connection.",
							  0, G_MAXUINT, 0,
							  G_PARAM_READWRITE);
	g_object_class_install_property(obj_class, PROP_DISCONNECT_TIMEOUT, pspec);

	signals[SIG_SIGNING_ON] =
		g_signal_new("signing-on",
					 G_OBJECT_CLASS_TYPE(klass),
					 G_SIGNAL_RUN_LAST,
					 G_STRUCT_OFFSET(PurpleConnectionClass, signing_on),
					 NULL, NULL,
					 purple_marshal_VOID__OBJECT,
					 G_TYPE_NONE,
					 0);

	signals[SIG_SIGNED_ON] =
		g_signal_new("signed-on",
					 G_OBJECT_CLASS_TYPE(klass),
					 G_SIGNAL_RUN_LAST,
					 G_STRUCT_OFFSET(PurpleConnectionClass, signed_on),
					 NULL, NULL,
					 purple_marshal_VOID__OBJECT,
					 G_TYPE_NONE,
					 0);

	signals[SIG_SIGNING_OFF] =
		g_signal_new("signing-off",
					 G_OBJECT_CLASS_TYPE(klass),
					 G_SIGNAL_RUN_LAST,
					 G_STRUCT_OFFSET(PurpleConnetionClass, signing_off),
					 NULL, NULL,
					 purple_marshal_VOID__OBJECT,
					 G_TYPE_NONE,
					 0);

	signals[SIG_SIGNED_OFF] =
		g_signal_new("signed-off",
					 G_OBJECT_CLASS_TYPE(klass),
					 G_SIGNAL_RUN_LAST,
					 G_STRUCT_OFFSET(PurpleConnectionClass, signed_off),
					 NULL, NULL,
					 purple_marshal_VOID__OBJECT,
					 G_TYPE_NONE,
					 0);

	signals[SIG_DISCONNECTED] =
		g_signal_new("disconnected",
					 G_OBJECT_CLASS_TYPE(klass),
					 G_SIGNAL_RUN_LAST,
					 G_STRUCT_OFFSET(PurpleConnectionClass, disconnected),
					 NULL, NULL,
					 purple_marshal_VOID__OBJECT,
					 G_TYPE_NONE,
					 0);
}

/******************************************************************************
 * API
 *****************************************************************************/
GType
purple_connection_get_gtype(void) {
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleConnectionClass),
			NULL,
			NULL,
			(GClassInitFunc)purple_connection_class_init,
			NULL,
			NULL,
			sizeof(PurpleConnection),
			0,
			purple_connection_init,
			NULL,
		};

		type = g_type_register_static(G_TYPE_OBJECT,
									  "PurpleConnection",
									  &info, 0);
	}

	return type;
}

PurpleConnection *
purple_connection_new(PurpleAccount *account) {
	g_return_val_if_fail(PURPLE_IS_ACCOUNT(account), NULL);

	return g_object_new(PURPLE_TYPE_CONNECTION,
						"account", account,
						NULL);
}

/******************************************************************************
 * PurpleConnectionFlags API
 *****************************************************************************/
GType
purple_connection_flags_get_gtype(void) {
	static GType type = 0;

	if(type == 0) {
		static const GFlagsValue values[] = {
			{ PURPLE_CONNECTION_FLAGS_HTML, "HTML", "HTML" },
			{ PURPLE_CONNECTION_FLAGS_NO_BGCOLOR, "NO BG", "NO BG" },
			{ PURPLE_CONNECTION_FLAGS_AUTO_RESP, "Autoresponse",
			  "Autoresponse"
			},
			{ PURPLE_CONNECTION_FLAGS_FORMATTING_WBFO, "WBFO", "WBFO" },
			{ PURPLE_CONNECTION_FLAGS_NO_NEWLINES, "No newlines",
			  "No newlines"
			},
			{ PURPLE_CONNECTION_FLAGS_NO_FONTSIZE, "No fontsize",
			  "No fontsize"
			},
			{ PURPLE_CONNECTION_FLAGS_NO_URLDESC, "No url desc",
			  "No url desc"
			},
			{ PURPLE_CONNECTION_FLAGS_NO_IMAGES, "No images", "No images", },
			{ 0, NULL, NULL },
		};

		type = g_flags_register_static("PurpleConnectionFlags", values);
	}

	return type;
}

/******************************************************************************
 * PurpleConnectionState API
 *****************************************************************************/
GType
purple_connection_state_get_gtype(void) {
	static GType type = 0;

	if(type == 0) {
		static const GEnumValue values[] = {
			{ PURPLE_CONNECTION_STATE_DISCONNECTED, "Disconnected",
			  "Disconnected"
			},
			{ PURPLE_CONNECTION_STATE_CONNECTING, "Connecting", "Connecting" },
			{ PURPLE_CONNECTION_STATE_CONNECTED, "Connected", "Connected" },
			{ 0, NULL, NULL },
		};

		type = g_enum_register_static("PurpleConnectionState", values);
	}

	return type;
}

