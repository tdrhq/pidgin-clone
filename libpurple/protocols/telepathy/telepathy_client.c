/**
 * purple - Telepathy Protocol Plugin
 *
 * Copyright (C) 2009, Felix Kerekes <sttwister@soc.pidgin.im>
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

#include "telepathy_client.h"

#include <telepathy-glib/enums.h>
#include <telepathy-glib/connection.h>
#include <telepathy-glib/util.h>
#include <telepathy-glib/dbus.h>
#include <telepathy-glib/proxy-subclass.h>
#include <telepathy-glib/gtypes.h>
#include <telepathy-glib/defs.h>
#include <telepathy-glib/svc-client.h>
#include <telepathy-glib/svc-generic.h>
#include <telepathy-glib/interfaces.h>

#define CLIENT_BUS_NAME TP_CLIENT_BUS_NAME_BASE "prpl_telepathy"
#define CLIENT_OBJECT_PATH TP_CLIENT_OBJECT_PATH_BASE "prpl_telepathy"

#define GET_PRIV(obj) TELEPATHY_GET_PRIV (obj, TelepathyClient)
typedef struct
{
	/* TODO: Fill this in or delete it */
} TelepathyClientPriv;

static void telepathy_client_client_handler_iface_init (gpointer g_iface,
	gpointer g_iface_data);

G_DEFINE_TYPE_WITH_CODE (TelepathyClient,
	telepathy_client,
	G_TYPE_OBJECT,
	G_IMPLEMENT_INTERFACE (TP_TYPE_SVC_DBUS_PROPERTIES,
		tp_dbus_properties_mixin_iface_init);
	G_IMPLEMENT_INTERFACE (TP_TYPE_SVC_CLIENT, NULL);
	G_IMPLEMENT_INTERFACE (TP_TYPE_SVC_CLIENT_HANDLER,
		telepathy_client_client_handler_iface_init);
  );

static const gchar *telepathy_client_interfaces[] = {
	TP_IFACE_CLIENT_HANDLER,
	NULL
};

enum
{
	PROP_INTERFACES = 1,
	PROP_CHANNEL_FILTER,
	PROP_CHANNELS
};

static TelepathyClient *client = NULL;

static GObject *
client_constructor (GType type,
                    guint n_construct_params,
                    GObjectConstructParam *construct_params)
{
	GObject *retval;
	TpDBusDaemon *dbus;

	if (client != NULL)
		return g_object_ref (client);

	retval = G_OBJECT_CLASS (telepathy_client_parent_class)->constructor
		(type, n_construct_params, construct_params);

	client = TELEPATHY_CLIENT (retval);
	g_object_add_weak_pointer (retval, (gpointer) &client);

	dbus = tp_dbus_daemon_dup (NULL);

	g_assert (tp_dbus_daemon_request_name (dbus,
			CLIENT_BUS_NAME, TRUE, NULL));
	dbus_g_connection_register_g_object (tp_get_bus (),
			CLIENT_OBJECT_PATH, retval);

	//DEBUG ("Registering at '%s'", CLIENT_OBJECT_PATH);

	return retval;
}

static void
client_finalize (GObject *object)
{
	TelepathyClientPriv *priv = GET_PRIV (object);
}

static void
client_get_property (GObject *object,
                     guint property_id,
                     GValue *value,
                     GParamSpec *pspec)
{
	TelepathyClient *client = TELEPATHY_CLIENT (object);
	TelepathyClientPriv *priv = GET_PRIV (client);

	switch (property_id)
	{
		case PROP_INTERFACES:
			g_value_set_boxed (value, telepathy_client_interfaces);
		break;
	    
		case PROP_CHANNEL_FILTER:
		{
			GPtrArray *filters = g_ptr_array_new ();
			GHashTable *filter = g_hash_table_new (NULL, NULL);

			g_ptr_array_add (filters, filter);

			g_value_set_boxed (value, filters);

			break;
		}

		case PROP_CHANNELS:
		{
			GPtrArray *accounts;
			GList *l;

			accounts = g_ptr_array_new ();

			for (l = priv->channels; l != NULL; l = g_list_next (l))
			{
				TpProxy *channel = TP_PROXY (l->data);

				g_ptr_array_add (accounts,
						g_strdup (tp_proxy_get_object_path (channel)));
			}

			g_value_set_boxed (value, accounts);

			break;
		}

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}

static void
telepathy_client_class_init (TelepathyClientClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GParamSpec *param_spec;

	static TpDBusPropertiesMixinPropImpl client_props[] = {
		{ "Interfaces", "interfaces", NULL },
		{ NULL }
	};
	static TpDBusPropertiesMixinPropImpl client_handler_props[] = {
		{ "HandlerChannelFilter", "channel-filter", NULL },
		{ "HandledChannels", "channels", NULL },
		{ NULL }
	};

	static TpDBusPropertiesMixinIfaceImpl prop_interfaces[] = {
		{ TP_IFACE_CLIENT,
			tp_dbus_properties_mixin_getter_gobject_properties,
			NULL,
			client_props
		},
		{ TP_IFACE_CLIENT_HANDLER,
			tp_dbus_properties_mixin_getter_gobject_properties,
			NULL,
			client_handler_props
		},
		{ NULL }
	};

	object_class->finalize = client_finalize;
	object_class->constructor = client_constructor;

	object_class->get_property = client_get_property;

	param_spec = g_param_spec_boxed ("interfaces", "interfaces",
		"Available D-Bus interfaces",
		G_TYPE_STRV,
		G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
	g_object_class_install_property (object_class, PROP_INTERFACES, param_spec);

	param_spec = g_param_spec_boxed ("channel-filter", "channel-filter",
		"Filter for channels this handles",
		TP_ARRAY_TYPE_CHANNEL_CLASS_LIST,
		G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
	g_object_class_install_property (object_class,
		PROP_CHANNEL_FILTER, param_spec);

	param_spec = g_param_spec_boxed ("channels", "channels",
		"List of channels we're handling",
		TELEPATHY_ARRAY_TYPE_OBJECT, // FIXME: What type is this???
		G_PARAM_READABLE | G_PARAM_STATIC_STRINGS);
	g_object_class_install_property (object_class,
		PROP_CHANNELS, param_spec);

	g_type_class_add_private (object_class, sizeof (TelepathyClientPriv));


	klass->dbus_props_class.interfaces = prop_interfaces;
	tp_dbus_properties_mixin_class_init (object_class,
		G_STRUCT_OFFSET (TelepathyClientClass, dbus_props_class));
}

static void
telepathy_client_init (TelepathyClient *client)
{
  TelepathyClientPriv *priv = G_TYPE_INSTANCE_GET_PRIVATE (client,
    TELEPATHY_TYPE_CLIENT, TelepathyClientPriv);

  client->priv = priv;
}

TelepathyClient *
telepathy_client_dup_singleton (void)
{
  return TELEPATHY_CLIENT (g_object_new (TELEPATHY_TYPE_CLIENT, NULL));
}

