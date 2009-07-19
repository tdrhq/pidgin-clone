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
#include "gsignal.h"

#include "debug.h"

/******************************************************************************
 * Structs
 *****************************************************************************/
struct _PurpleGSignalHandle {
	guint signal_id;
	gulong hook_id;
};

typedef struct {
	GType type;
	GCallback callback;
	gpointer user_data;
	GConnectFlags flags;
	GQuark detail;
	GType return_type;
} PurpleGSignalData;

/******************************************************************************
 * Helpers
 *****************************************************************************/
static gboolean
purple_g_signal_emission_hook(GSignalInvocationHint *hint, guint n_params,
							  const GValue *pvalues, gpointer data)
{
	PurpleGSignalData *sd = data;
	GObject *obj = NULL;
	gboolean swap = FALSE, after = FALSE;
	GClosure *closure = NULL;

	obj = g_value_get_object(pvalues);

	if(!G_TYPE_CHECK_INSTANCE_TYPE(obj, sd->type))
		return TRUE;

	swap = (sd->flags & G_CONNECT_SWAPPED);
	after = (sd->flags & G_CONNECT_AFTER);

	if(swap)
		closure = g_cclosure_new_swap(sd->callback, sd->user_data, NULL);
	else
		closure = g_cclosure_new(sd->callback, sd->user_data, NULL);

	g_signal_connect_closure_by_id(obj, hint->signal_id, sd->detail, closure, after);

	if(sd->return_type != G_TYPE_NONE) {
		GValue ret;

		g_value_init(&ret, sd->return_type);

		g_closure_invoke(closure, &ret, n_params, pvalues, hint);
	} else {
		g_closure_invoke(closure, NULL, n_params, pvalues, hint);
	}

	return TRUE;
}

/******************************************************************************
 * PurpleGSignal API
 *****************************************************************************/
PurpleGSignalHandle *
purple_g_signal_connect_flags(GType type, const gchar *name,
							  GConnectFlags flags,
							  GCallback callback, gpointer data)
{
	PurpleGSignalData *sd = NULL;
	PurpleGSignalHandle *handle = NULL;
	GQuark detail = 0;
	GSignalQuery query;
	guint signal_id = 0;
	gulong hook_id = 0;
	gpointer *klass = NULL;

	klass = g_type_class_ref(type);

	if(!g_signal_parse_name(name, type, &signal_id, &detail, TRUE)) {
		purple_debug_warning("gsignal",
		                     "Failed to find information for signal '%s' on "
		                     "type '%s'!\n",
		                     name, g_type_name(type));


		g_type_class_unref(klass);

		return NULL;
	}

	g_signal_query(signal_id, &query);

	if(query.signal_id == 0) {
		purple_debug_warning("gsignal",
		                     "Failed to query signal '%s' on type '%s'!\n",
		                     name, g_type_name(type));

		g_type_class_unref(klass);

		return NULL;
	}

	sd = g_new(PurpleGSignalData, 1);
	sd->callback = callback;
	sd->user_data = data;
	sd->flags = 0;
	sd->detail = detail;
	sd->type = type;
	sd->return_type = query.return_type;

	hook_id = g_signal_add_emission_hook(signal_id, detail,
										 purple_g_signal_emission_hook,
										 sd, g_free);

	g_type_class_unref(klass);

	handle = purple_g_signal_handle_new(signal_id, hook_id);

	return handle;
}

void
purple_g_signal_disconnect(PurpleGSignalHandle *handle) {
	g_return_if_fail(handle);

	g_signal_remove_emission_hook(handle->signal_id, handle->hook_id);

	purple_g_signal_handle_free(handle);
}

/******************************************************************************
 * PurpleSignalHandle API
 *****************************************************************************/
GType
purple_g_signal_handle_get_type(void) {
	static GType type = 0;

	if(G_UNLIKELY(type == 0)) {
		type =
			g_boxed_type_register_static("PurpleSignalHandle",
										 (GBoxedCopyFunc)purple_g_signal_handle_copy,
										 (GBoxedFreeFunc)purple_g_signal_handle_free);

	}

	return type;
}

PurpleGSignalHandle *
purple_g_signal_handle_new(guint signal_id, gulong hook_id) {
	PurpleGSignalHandle *handle = NULL;

	handle = g_new(PurpleGSignalHandle, 1);

	handle->signal_id = signal_id;
	handle->hook_id = hook_id;

	return handle;
}

PurpleGSignalHandle *
purple_g_signal_handle_copy(const PurpleGSignalHandle *handle) {
	PurpleGSignalHandle *copy = NULL;

	g_return_val_if_fail(handle, NULL);

	copy = purple_g_signal_handle_new(handle->signal_id, handle->hook_id);

	return copy;
}

void
purple_g_signal_handle_free(PurpleGSignalHandle *handle) {
	g_free(handle);
}

