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
#include "pobject.h"

struct _PurpleObjectPrivate
{
	gpointer proto_data;
	gpointer ui_data;
};

enum
{
	SIG_NEW,
	SIG_LAST
};
static guint signals[SIG_LAST] = {0, };

static GObjectClass *parent_class;

static void
purple_object_dispose(GObject *obj)
{
	PurpleObject *pobj = PURPLE_OBJECT(obj);

	if (pobj->priv->proto_data) {
		g_warning("Purple-Object: object destroyed without unsetting the protocol data. This may lead to memory leak.\n");
	}

	if (pobj->priv->ui_data) {
		g_warning("Purple-Object: object destroyed without unsetting the ui data. This may lead to memory leak.\n");
	}

	/* XXX: do _notify_close_with_handle etc here */

	parent_class->dispose(obj);
}

static void
purple_object_class_init(PurpleObjectClass *klass)
{
	GObjectClass *gclass = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

	gclass->dispose = purple_object_dispose;

	/* Signals */

	/* This signal should be emitted after an object is created, and
	 * the essential properties for the object has been set properly. */
	signals[SIG_NEW] = g_signal_new("new", G_OBJECT_CLASS_TYPE(klass),
				G_SIGNAL_ACTION, 0, NULL, NULL,
				g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);

	g_type_class_add_private(klass, sizeof(PurpleObjectPrivate));
}

static void
purple_object_init(GTypeInstance *instance, gpointer class)
{
	PurpleObject *pobj = PURPLE_OBJECT(instance);
	pobj->priv = G_TYPE_INSTANCE_GET_PRIVATE(pobj, PURPLE_TYPE_OBJECT, PurpleObjectPrivate);
	pobj->priv->proto_data = NULL;
}

GType purple_object_get_type(void)
{
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo info = {
			sizeof(PurpleObjectClass),
			NULL,                    /* base_init        */
			NULL,                    /* base_finalize    */
			(GClassInitFunc)purple_object_class_init,
			NULL,
			NULL,                    /* class_data        */
			sizeof(PurpleObject),
			0,                       /* n_preallocs        */
			purple_object_init,      /* instance_init    */
			NULL                     /* value_table        */
		};

		type = g_type_register_static(G_TYPE_OBJECT,
				"PurpleObject",
				&info, G_TYPE_FLAG_ABSTRACT);
	}

	return type;
}

void purple_object_set_protocol_data(PurpleObject *pobj, gpointer proto_data)
{
	g_return_if_fail(pobj);
	pobj->priv->proto_data = proto_data;
}

gpointer purple_object_get_protocol_data(PurpleObject *pobj)
{
	g_return_val_if_fail(pobj, NULL);
	return pobj->priv->proto_data;
}

void purple_object_set_ui_data(PurpleObject *pobj, gpointer ui_data)
{
	g_return_if_fail(pobj);
	pobj->priv->ui_data = ui_data;
}

gpointer purple_object_get_ui_data(PurpleObject *pobj)
{
	g_return_val_if_fail(pobj, NULL);
	return pobj->priv->ui_data;
}

int purple_object_get_int(PurpleObject *pobj, const char *prop)
{
	int ret;
	g_object_get(G_OBJECT(pobj), prop, &ret, NULL);
	return ret;
}

/**
 * Signal emission hooks utility function
 */
typedef struct
{
	GType type;
	GCallback callback;
	gpointer user_data;
	GConnectFlags flags;
	GQuark detail;
} PObjectConnectHook;

static gboolean
signal_emission_hook_fn(GSignalInvocationHint *hint, guint nparams,
		const GValue *pvalues, gpointer data)
{
	PObjectConnectHook *hook = data;
	PurpleObject *obj = PURPLE_OBJECT(g_value_get_object(pvalues + 0));

	if (!G_TYPE_CHECK_INSTANCE_TYPE(obj, hook->type))
		return TRUE;

	if (g_signal_handler_find(obj,
				G_SIGNAL_MATCH_ID | G_SIGNAL_MATCH_FUNC | G_SIGNAL_MATCH_DATA | G_SIGNAL_MATCH_DETAIL,
				hint->signal_id, hook->detail, NULL,
				hook->callback, hook->user_data) == 0) {
		gboolean swap = (hook->flags & G_CONNECT_SWAPPED);
		gboolean after = (hook->flags & G_CONNECT_AFTER);
		GClosure *closure = (swap ? g_cclosure_new_swap : g_cclosure_new)
					(hook->callback, hook->user_data, NULL);
		g_signal_connect_closure_by_id(obj, hint->signal_id, hook->detail, closure, after);
		g_closure_invoke(closure, NULL, nparams, pvalues, hint);
	}
	return TRUE;
}

gulong
purple_type_connect(GType type, const char *detailed_signal, GCallback callback,
		gpointer data)
{
	return purple_type_connect_flags(type, detailed_signal, callback, data, 0);
}

gulong
purple_type_connect_flags(GType type, const char *detailed_signal, GCallback callback,
		gpointer data, GConnectFlags flags)
{
	PObjectConnectHook *hook;
	guint signal_id;
	GQuark detail;

	g_type_class_unref(g_type_class_ref(type));

	if (!g_signal_parse_name(detailed_signal, type, &signal_id, &detail, TRUE))
		return 0;

	hook = g_new0(PObjectConnectHook, 1);
	hook->callback = callback;
	hook->user_data = data;
	hook->flags = flags;
	hook->detail = detail;
	hook->type = type;

	return g_signal_add_emission_hook(signal_id, detail,
			signal_emission_hook_fn, hook, g_free);
}

