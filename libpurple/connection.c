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
#define _PURPLE_CONNECTION_C_

#include "internal.h"
#include "account.h"
#include "blist.h"
#include "connection.h"
#include "dbus-maybe.h"
#include "debug.h"
#include "log.h"
#include "marshallers.h"
#include "notify.h"
#include "prefs.h"
#include "proxy.h"
#include "request.h"
#include "server.h"
#include "util.h"

/******************************************************************************
 * Structs
 *****************************************************************************/
struct _PurpleConnectionPrivate
{
	PurpleConnectionFlags flags;

	PurpleConnectionState state;

	PurpleAccount *account;
	char *password;

	gint inpa;

	GSList *buddy_chats;

	guint keepalive;

	gboolean wants_to_die;
	guint disconnect_timeout;
	time_t last_received;
	PurplePlugin *prpl;
	char *display_name;
};

/******************************************************************************
 * Enums
 *****************************************************************************/
enum
{
	PROP_ZERO,
	PROP_FLAGS,
	PROP_STATE,
	PROP_ACCOUNT,
	PROP_INPUT_WATCHER,
#if 0
	PROP_BUDDY_CHATS,
	PROP_KEEPALIVE,
	PROP_WANTS_TO_DIE,
	PROP_DISCONNECT_TIMEOUT,
#endif
	PROP_LAST,
};

enum
{
	SIG_SIGNING_ON,
	SIG_SIGNED_ON,
	SIG_SIGNING_OFF,
	SIG_SIGNED_OFF,
	SIG_DISCONNECTED,
	SIG_ERROR,
	SIG_LAST
};

#define KEEPALIVE_INTERVAL 30

#warning Do we want to use a ConnectionManager for these?
static GList *connections = NULL;
#if 0
static GList *connections_connecting = NULL;
#endif

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

	if (gc == NULL)
		return TRUE;

	/* Only send keep-alives if we haven't heard from the
	 * server in a while.
	 */
	if ((time(NULL) - gc->priv->last_received) < KEEPALIVE_INTERVAL)
		return TRUE;

	if (gc->priv->prpl == NULL)
		return TRUE;

	prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(gc->priv->prpl);

	if (prpl_info && prpl_info->keepalive)
		prpl_info->keepalive(gc);

	return TRUE;
}

static void
update_keepalive(PurpleConnection *gc, gboolean on)
{
	PurplePluginProtocolInfo *prpl_info = NULL;

	if (gc != NULL && gc->priv->prpl != NULL)
		prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(gc->priv->prpl);

	if (!prpl_info || !prpl_info->keepalive)
		return;

	if (on && !gc->priv->keepalive)
	{
		purple_debug_info("connection", "Activating keepalive.\n");
		gc->priv->keepalive = purple_timeout_add_seconds(KEEPALIVE_INTERVAL, send_keepalive, gc);
	}
	else if (!on && gc->priv->keepalive > 0)
	{
		purple_debug_info("connection", "Deactivating keepalive.\n");
		purple_timeout_remove(gc->priv->keepalive);
		gc->priv->keepalive = 0;
	}
}

void
purple_connection_new(PurpleAccount *account, gboolean regist, const char *password)
{
	_purple_connection_new(account, regist, password);
}

void
_purple_connection_new(PurpleAccount *account, gboolean regist, const char *password)
{
	PurpleConnection *gc;
	PurplePlugin *prpl;
	PurplePluginProtocolInfo *prpl_info;

	g_return_if_fail(PURPLE_IS_ACCOUNT(account));

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

	gc = g_object_new(PURPLE_TYPE_CONNECTION, "account", account, NULL);
	PURPLE_DBUS_REGISTER_POINTER(gc, PurpleConnection);

	gc->priv->password = g_strdup(password);

	g_signal_emit(G_OBJECT(gc), signals[SIG_SIGNING_ON], 0);

	if (regist)
	{
		purple_debug_info("connection", "Registering.  gc = %p\n", gc);

		/* set this so we don't auto-reconnect after registering */
		gc->priv->wants_to_die = TRUE;

		prpl_info->register_user(account);
	}
	else
	{
		purple_debug_info("connection", "Connecting. gc = %p\n", gc);

#warning Replace with g_signal_emit when account is gobjectified
		purple_signal_emit(purple_accounts_get_handle(), "account-connecting", account);
		prpl_info->login(account);
	}
}
void
purple_connection_new_unregister(PurpleAccount *account, const char *password, PurpleAccountUnregistrationCb cb, void *user_data)
{
	_purple_connection_new_unregister(account, password, cb, user_data);
}

void
_purple_connection_new_unregister(PurpleAccount *account, const char *password, PurpleAccountUnregistrationCb cb, void *user_data)
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

	gc->priv->prpl = prpl;
	gc->priv->password = g_strdup(password);
	purple_connection_set_account(gc, account);
	purple_connection_set_state(gc, PURPLE_CONNECTION_STATE_CONNECTING);
	connections = g_list_append(connections, gc);
	purple_account_set_connection(account, gc);

	g_signal_emit(G_OBJECT(gc), signals[SIG_SIGNING_ON], 0);

	purple_debug_info("connection", "Unregistering.  gc = %p\n", gc);

	prpl_info->unregister_user(account, cb, user_data);
}

void
purple_connection_destroy(PurpleConnection *gc)
{
	_purple_connection_destroy(gc);
}

void
_purple_connection_destroy(PurpleConnection *gc)
{
	g_return_if_fail(PURPLE_IS_CONNECTION(gc));
	purple_account_set_connection(gc->priv->account, NULL);
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
#if 0
	PurpleConnectionUiOps *ops;
#endif

	g_return_if_fail(PURPLE_IS_CONNECTION(gc));

	if (gc->priv->state == state)
		return;

	gc->priv->state = state;

#if 0
	ops = purple_connections_get_ui_ops();

	if (gc->priv->state == PURPLE_CONNECTION_STATE_CONNECTING) {
		connections_connecting = g_list_append(connections_connecting, gc);
	}
	else {
		connections_connecting = g_list_remove(connections_connecting, gc);
	}
#endif

	if (gc->priv->state == PURPLE_CONNECTION_STATE_CONNECTED) {
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

		purple_blist_add_account(account);

#if 0
		if (ops != NULL && ops->connected != NULL)
			ops->connected(gc);

#endif
		g_signal_emit(G_OBJECT(gc), signals[SIG_SIGNED_ON], 0);

		serv_set_permit_deny(gc);

		update_keepalive(gc, TRUE);
	}
	else if (gc->priv->state == PURPLE_CONNECTION_STATE_DISCONNECTED) {
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

#if 0
		if (ops != NULL && ops->disconnected != NULL)
			ops->disconnected(gc);
#endif
	}
}

void purple_connection_set_flags(PurpleConnection *gc, PurpleConnectionFlags flags)
{
	g_return_if_fail(PURPLE_IS_CONNECTION(gc));
	gc->priv->flags = flags;
}

void
purple_connection_set_account(PurpleConnection *gc, PurpleAccount *account)
{
	g_return_if_fail(PURPLE_IS_CONNECTION(gc));
	g_return_if_fail(account != NULL);
	g_return_if_fail(gc->priv->account == NULL); /* We set the account for a connection exactly once */

	if (gc->priv->account == account)
		return;

	gc->priv->account = account;
	gc->priv->prpl = account ? purple_find_prpl(purple_account_get_protocol_id(account)) : NULL;

	purple_account_set_connection(account, gc);
}

void
purple_connection_set_display_name(PurpleConnection *gc, const char *name)
{
	g_return_if_fail(PURPLE_IS_CONNECTION(gc));

	g_free(gc->priv->display_name);
	gc->priv->display_name = g_strdup(name);
}

PurpleConnectionState
purple_connection_get_state(const PurpleConnection *gc)
{
	g_return_val_if_fail(gc != NULL, PURPLE_CONNECTION_STATE_DISCONNECTED);

	return gc->priv->state;
}

PurpleConnectionFlags
purple_connection_get_flags(PurpleConnection *gc)
{
	g_return_val_if_fail(gc != NULL, 0);

	return gc->priv->flags;
}

PurpleAccount *
purple_connection_get_account(const PurpleConnection *gc)
{
	g_return_val_if_fail(gc != NULL, NULL);

	return gc->priv->account;
}

PurplePlugin *
purple_connection_get_prpl(const PurpleConnection *gc)
{
	g_return_val_if_fail(gc != NULL, NULL);

	return gc->priv->prpl;
}

const char *
purple_connection_get_password(const PurpleConnection *gc)
{
	g_return_val_if_fail(gc != NULL, NULL);

	return gc->priv->password ? gc->priv->password : purple_account_get_password(gc->priv->account);
}

const char *
purple_connection_get_display_name(const PurpleConnection *gc)
{
	g_return_val_if_fail(gc != NULL, NULL);

	return gc->priv->display_name;
}

void
purple_connection_update_progress(PurpleConnection *gc, const char *text,
								size_t step, size_t count)
{
#if 0
	PurpleConnectionUiOps *ops;
#endif

	g_return_if_fail(PURPLE_IS_CONNECTION(gc));
	g_return_if_fail(text != NULL);
	g_return_if_fail(step < count);
	g_return_if_fail(count > 1);

#if 0
	ops = purple_connections_get_ui_ops();

	if (ops != NULL && ops->connect_progress != NULL)
		ops->connect_progress(gc, text, step, count);
#endif
}

void
purple_connection_received_now(PurpleConnection *conn)
{
    conn->priv->last_received = time(NULL);
}

void
purple_connection_notice(PurpleConnection *gc, const char *text)
{
#if 0
	PurpleConnectionUiOps *ops;
#endif

	g_return_if_fail(PURPLE_IS_CONNECTION(gc));
	g_return_if_fail(text != NULL);

#if 0
	ops = purple_connections_get_ui_ops();

	if (ops != NULL && ops->notice != NULL)
		ops->notice(gc, text);
#endif
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
	/* prpls that have not been updated to use disconnection reasons will
	 * be setting wants_to_die before calling this function, so choose
	 * PURPLE_CONNECTION_ERROR_OTHER_ERROR (which is fatal) if it's true,
	 * and PURPLE_CONNECTION_ERROR_NETWORK_ERROR (which isn't) if not.  See
	 * the documentation in connection.h.
	 */
	PurpleConnectionError reason = gc->priv->wants_to_die
	                             ? PURPLE_CONNECTION_ERROR_OTHER_ERROR
	                             : PURPLE_CONNECTION_ERROR_NETWORK_ERROR;
	purple_connection_error_reason (gc, reason, text);
}

void
purple_connection_error_reason (PurpleConnection *gc,
                                PurpleConnectionError reason,
                                const char *description)
{
#if 0
	PurpleConnectionUiOps *ops;
#endif

	g_return_if_fail(PURPLE_IS_CONNECTION(gc));
	/* This sanity check relies on PURPLE_CONNECTION_ERROR_OTHER_ERROR
	 * being the last member of the PurpleConnectionError enum in
	 * connection.h; if other reasons are added after it, this check should
	 * be updated.
	 */
	if (reason > PURPLE_CONNECTION_ERROR_OTHER_ERROR) {
		purple_debug_error("connection",
			"purple_connection_error_reason: reason %u isn't a "
			"valid reason\n", reason);
		reason = PURPLE_CONNECTION_ERROR_OTHER_ERROR;
	}

	if (description == NULL) {
		purple_debug_error("connection", "purple_connection_error_reason called with NULL description\n");
		description = _("Unknown error");
	}

	/* If we've already got one error, we don't need any more */
	if (gc->priv->disconnect_timeout)
		return;

	gc->priv->wants_to_die = purple_connection_error_is_fatal (reason);

#if 0
    /* This will probably eventually move to ConnectionManager */
	ops = purple_connections_get_ui_ops();

	if (ops != NULL)
	{
		if (ops->report_disconnect_reason != NULL)
			ops->report_disconnect_reason (gc, reason, description);
		if (ops->report_disconnect != NULL)
			ops->report_disconnect (gc, description);
	}
#endif
	g_signal_emit(G_OBJECT(gc), signals[SIG_ERROR], 0, reason, description);

	gc->priv->disconnect_timeout = purple_timeout_add(0, purple_connection_disconnect_cb,
			purple_connection_get_account(gc));
}

void
purple_connection_ssl_error (PurpleConnection *gc,
                             PurpleSslErrorType ssl_error)
{
	PurpleConnectionError reason;

	switch (ssl_error) {
		case PURPLE_SSL_HANDSHAKE_FAILED:
			reason = PURPLE_CONNECTION_ERROR_ENCRYPTION_ERROR;
			break;
		case PURPLE_SSL_CONNECT_FAILED:
			reason = PURPLE_CONNECTION_ERROR_NETWORK_ERROR;
			break;
		case PURPLE_SSL_CERTIFICATE_INVALID:
			/* TODO: maybe PURPLE_SSL_* should be more specific? */
			reason = PURPLE_CONNECTION_ERROR_CERT_OTHER_ERROR;
			break;
		default:
			g_assert_not_reached ();
			reason = PURPLE_CONNECTION_ERROR_CERT_OTHER_ERROR;
	}

	purple_connection_error_reason (gc, reason,
		purple_ssl_strerror(ssl_error));
}

gboolean
purple_connection_error_is_fatal (PurpleConnectionError reason)
{
	switch (reason)
	{
		case PURPLE_CONNECTION_ERROR_NETWORK_ERROR:
		case PURPLE_CONNECTION_ERROR_ENCRYPTION_ERROR:
			return FALSE;
		case PURPLE_CONNECTION_ERROR_INVALID_USERNAME:
		case PURPLE_CONNECTION_ERROR_AUTHENTICATION_FAILED:
		case PURPLE_CONNECTION_ERROR_AUTHENTICATION_IMPOSSIBLE:
		case PURPLE_CONNECTION_ERROR_NO_SSL_SUPPORT:
		case PURPLE_CONNECTION_ERROR_NAME_IN_USE:
		case PURPLE_CONNECTION_ERROR_INVALID_SETTINGS:
		case PURPLE_CONNECTION_ERROR_OTHER_ERROR:
		case PURPLE_CONNECTION_ERROR_CERT_NOT_PROVIDED:
		case PURPLE_CONNECTION_ERROR_CERT_UNTRUSTED:
		case PURPLE_CONNECTION_ERROR_CERT_EXPIRED:
		case PURPLE_CONNECTION_ERROR_CERT_NOT_ACTIVATED:
		case PURPLE_CONNECTION_ERROR_CERT_HOSTNAME_MISMATCH:
		case PURPLE_CONNECTION_ERROR_CERT_FINGERPRINT_MISMATCH:
		case PURPLE_CONNECTION_ERROR_CERT_SELF_SIGNED:
		case PURPLE_CONNECTION_ERROR_CERT_OTHER_ERROR:
			return TRUE;
		default:
			g_return_val_if_reached(TRUE);
	}
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
			g_value_set_flags(value, pc->priv->flags);
			break;
		case PROP_STATE:
			g_value_set_enum(value, purple_connection_get_state(pc));
			break;
		case PROP_ACCOUNT:
			g_value_set_object(value, purple_connection_get_account(pc));
			break;
		case PROP_INPUT_WATCHER:
			g_value_set_int(value, pc->priv->inpa);
			break;
#if 0
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
#endif
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
			pc->priv->flags = g_value_get_flags(value);
			break;
		case PROP_STATE:
			purple_connection_set_state(pc, g_value_get_enum(value));
			break;
		case PROP_ACCOUNT:
			purple_connection_set_account(pc, g_value_get_object(value));
			break;
		case PROP_INPUT_WATCHER:
			pc->priv->inpa = g_value_get_int(value);
			break;
#if 0
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
#endif
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
purple_connection_dispose(GObject *obj)
{
	PurpleConnection *pc = PURPLE_CONNECTION(obj);
	PurpleAccount *account;
	GSList *buddies;
	PurplePluginProtocolInfo *prpl_info = NULL;

	g_return_if_fail(PURPLE_IS_CONNECTION(pc));

	account = purple_connection_get_account(pc);

	purple_debug_info("connection", "Disconnecting connection %p\n", pc);

	g_signal_emit(G_OBJECT(pc), signals[SIG_SIGNING_OFF], 0);

	while (pc->priv->buddy_chats)
	{
		PurpleConversation *b = pc->priv->buddy_chats->data;

		pc->priv->buddy_chats = g_slist_remove(pc->priv->buddy_chats, b);
		purple_conv_chat_left(PURPLE_CONV_CHAT(b));
	}

	update_keepalive(pc, FALSE);

	purple_proxy_connect_cancel_with_handle(pc);

	prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(pc->priv->prpl);
	if (prpl_info->close)
		(prpl_info->close)(pc);

	/* Clear out the proto data that was freed in the prpl close method*/
	buddies = purple_find_buddies(account, NULL);
	while (buddies != NULL) {
#warning Uncomment when PurpleBuddy is a GObject
#if 0
		PurpleBuddy *buddy = buddies->data;
		purple_object_set_protocol_data(PURPLE_OBJECT(buddy),NULL);
#endif
		buddies = g_slist_delete_link(buddies, buddies);
	}

	G_OBJECT_CLASS(parent_class)->dispose(obj);
}

static void
purple_connection_finalize(GObject *obj)
{
	PurpleConnection *pc = PURPLE_CONNECTION(obj);
	PurpleAccount *account = pc->priv->account;
	gboolean remove = FALSE;

	connections = g_list_remove(connections, pc);

	if (purple_connection_get_state(pc) != PURPLE_CONNECTION_STATE_CONNECTING)
		remove = TRUE;
	purple_connection_set_state(pc, PURPLE_CONNECTION_STATE_DISCONNECTED);
	if (remove)
		purple_blist_remove_account(account);

	g_signal_emit(G_OBJECT(pc), signals[SIG_SIGNED_OFF], 0);

	purple_account_request_close_with_account(account);
	purple_request_close_with_handle(pc);
	purple_notify_close_with_handle(pc);

	purple_debug_info("connection", "Destroying connection %p\n", pc);

	purple_account_set_connection(account, NULL);

	g_free(pc->priv->password);
	g_free(pc->priv->display_name);

	if (pc->priv->disconnect_timeout)
		purple_timeout_remove(pc->priv->disconnect_timeout);

	PURPLE_DBUS_UNREGISTER_POINTER(pc);
	g_free(pc->priv);

	G_OBJECT_CLASS(parent_class)->finalize(obj);
}

static void
purple_connection_init(GTypeInstance *instance, gpointer klass)
{
	PurpleConnection *pc = PURPLE_CONNECTION(instance);

	pc->priv = g_new0(PurpleConnectionPrivate, 1);
	purple_connection_set_state(pc, PURPLE_CONNECTION_STATE_CONNECTING);
}

static void
purple_connection_class_init(PurpleConnectionClass *klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);
	GParamSpec *pspec;

	parent_class = g_type_class_peek_parent(klass);

	obj_class->get_property = purple_connection_get_property;
	obj_class->set_property = purple_connection_set_property;
	obj_class->finalize = purple_connection_finalize;
	obj_class->dispose = purple_connection_dispose;

	/* explicitly make these pure */
	klass->signing_on = NULL;
	klass->signed_on = NULL;

	klass->signing_off = NULL;
	klass->signed_off = NULL;

	pspec = g_param_spec_flags("flags", "flags",
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

	pspec = g_param_spec_object("account", "account",
								 _("The account this connection belongs to."),
								 PURPLE_TYPE_ACCOUNT,
								 G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
	g_object_class_install_property(obj_class, PROP_ACCOUNT, pspec);

#warning These probably do not need any exposing
	pspec = g_param_spec_int("inpa", "inpa",
							 "The input watcher's fd for this connection.",
							 G_MININT, G_MAXINT, -1,
							 G_PARAM_READWRITE);
	g_object_class_install_property(obj_class, PROP_INPUT_WATCHER, pspec);

#if 0
	pspec = g_param_spec_pointer("buddy-chats", "buddy-chats",
								 "A list of buddy chats for this connection.",
								 G_PARAM_READWRITE);
	g_object_class_install_property(obj_class, PROP_BUDDY_CHATS, pspec);

	pspec = g_param_spec_uint("keepalive", "keepalive",
							  "The keepalive value for this connection.",
							  0, G_MAXUINT, 0,
							  G_PARAM_READWRITE);
	g_object_class_install_property(obj_class, PROP_KEEPALIVE, pspec);

#warning wants-to-die needs-to-die
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
#endif

	signals[SIG_SIGNING_ON] =
		g_signal_new("signing-on",
					 G_OBJECT_CLASS_TYPE(klass),
					 G_SIGNAL_RUN_LAST,
					 G_STRUCT_OFFSET(PurpleConnectionClass, signing_on),
					 NULL, NULL,
					 g_cclosure_marshal_VOID__VOID,
					 G_TYPE_NONE,
					 0);

	signals[SIG_SIGNED_ON] =
		g_signal_new("signed-on",
					 G_OBJECT_CLASS_TYPE(klass),
					 G_SIGNAL_RUN_LAST,
					 G_STRUCT_OFFSET(PurpleConnectionClass, signed_on),
					 NULL, NULL,
					 g_cclosure_marshal_VOID__VOID,
					 G_TYPE_NONE,
					 0);

	signals[SIG_SIGNING_OFF] =
		g_signal_new("signing-off",
					 G_OBJECT_CLASS_TYPE(klass),
					 G_SIGNAL_RUN_LAST,
					 G_STRUCT_OFFSET(PurpleConnectionClass, signing_off),
					 NULL, NULL,
					 g_cclosure_marshal_VOID__VOID,
					 G_TYPE_NONE,
					 0);

	signals[SIG_SIGNED_OFF] =
		g_signal_new("signed-off",
					 G_OBJECT_CLASS_TYPE(klass),
					 G_SIGNAL_RUN_LAST,
					 G_STRUCT_OFFSET(PurpleConnectionClass, signed_off),
					 NULL, NULL,
					 g_cclosure_marshal_VOID__VOID,
					 G_TYPE_NONE,
					 0);

	signals[SIG_DISCONNECTED] =
		g_signal_new("disconnected",
					 G_OBJECT_CLASS_TYPE(klass),
					 G_SIGNAL_RUN_LAST,
					 G_STRUCT_OFFSET(PurpleConnectionClass, disconnected),
					 NULL, NULL,
					 g_cclosure_marshal_VOID__VOID,
					 G_TYPE_NONE,
					 0);

	signals[SIG_ERROR] =
		g_signal_new("connection-error",
					 G_OBJECT_CLASS_TYPE(klass),
					 G_SIGNAL_RUN_LAST,
					 G_STRUCT_OFFSET(PurpleConnectionClass, disconnected),
					 NULL, NULL,
					 purple_smarshal_VOID__ENUM_STRING,
					 G_TYPE_NONE,
					 2, PURPLE_TYPE_CONNECTION_ERROR, G_TYPE_STRING);
}

/******************************************************************************
 * API
 *****************************************************************************/
GType
purple_connection_get_gtype(void)
{
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

		type = g_type_register_static(PURPLE_TYPE_OBJECT,
									  "PurpleConnection",
									  &info, 0);
	}

	return type;
}

/******************************************************************************
 * PurpleConnectionFlags API
 *****************************************************************************/
GType
purple_connection_flags_get_gtype(void)
{
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
			{ PURPLE_CONNECTION_FLAGS_ALLOW_CUSTOM_SMILEY,
			    "Allow custom smileys", "Allow custom smileys", },
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
purple_connection_state_get_gtype(void)
{
	static GType type = 0;

	if(type == 0) {
		static const GEnumValue values[] = {
			{ PURPLE_CONNECTION_STATE_DISCONNECTED, "Disconnected", "Disconnected"},
			{ PURPLE_CONNECTION_STATE_CONNECTING, "Connecting", "Connecting"},
			{ PURPLE_CONNECTION_STATE_CONNECTED, "Connected", "Connected"},
			{ 0, NULL, NULL },
		};

		type = g_enum_register_static("PurpleConnectionState", values);
	}

	return type;
}

/******************************************************************************
 * PurpleConnectionError API
 *****************************************************************************/
GType
purple_connection_error_get_gtype(void)
{
	static GType type = 0;

	if(type == 0) {
		static const GEnumValue values[] = {
			{PURPLE_CONNECTION_ERROR_NETWORK_ERROR, "Network error", ""},
			{PURPLE_CONNECTION_ERROR_INVALID_USERNAME, "Invalid username", ""},
			{PURPLE_CONNECTION_ERROR_AUTHENTICATION_FAILED, "Authentication failed", ""},
			{PURPLE_CONNECTION_ERROR_AUTHENTICATION_IMPOSSIBLE, "Authentication impossible", ""},
			{PURPLE_CONNECTION_ERROR_NO_SSL_SUPPORT, "No SSL support", ""},
			{PURPLE_CONNECTION_ERROR_ENCRYPTION_ERROR, "Encryption error", ""},
			{PURPLE_CONNECTION_ERROR_NAME_IN_USE, "Name already in use", ""},
			{PURPLE_CONNECTION_ERROR_INVALID_SETTINGS, "Invalid settings", ""},
			{PURPLE_CONNECTION_ERROR_CERT_NOT_PROVIDED, "SSL certificate unavailable", ""},
			{PURPLE_CONNECTION_ERROR_CERT_UNTRUSTED, "SSL certificate could not be trusted", ""},
			{PURPLE_CONNECTION_ERROR_CERT_EXPIRED, "SSL certificate expired", ""},
			{PURPLE_CONNECTION_ERROR_CERT_NOT_ACTIVATED, "SSL certificate is not yet valid", ""},
			{PURPLE_CONNECTION_ERROR_CERT_HOSTNAME_MISMATCH, "SSL certificate did not match its hostname", ""},
			{PURPLE_CONNECTION_ERROR_CERT_FINGERPRINT_MISMATCH, "SSL certificate fingerprint mismatch", ""},
			{PURPLE_CONNECTION_ERROR_CERT_SELF_SIGNED, "SSL certificate is self-signed", ""},
			{PURPLE_CONNECTION_ERROR_CERT_OTHER_ERROR, "Other SSL error", ""},
			{PURPLE_CONNECTION_ERROR_OTHER_ERROR, "Unknown error", ""},
			{ 0, NULL, NULL },
		};

		type = g_enum_register_static("PurpleConnectionError", values);
	}

	return type;
}

/***
 * These should move to PurpleConnectionManager
 */
GList *purple_connections_get_all(void)
{
    return connections;
}

