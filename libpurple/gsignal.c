#include "signals.h"

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
		GValue ret = { 0, };

		g_value_init(&ret, sd->return_type);

		g_closure_invoke(closure, &ret, n_params, pvalues, hint);
	} else {
		g_closure_invoke(closure, NULL, n_params, pvalues, hint);
	}

	return TRUE;
}

PurpleGSignalHandle *
purple_g_signal_connect(GType t, const gchar *n, GCallback cb, gpointer d) {
	PurpleGSignalData *sd = NULL;
	PurpleGSignalHandle *handle = NULL;
	GQuark detail = 0;
	GSignalQuery query;
	guint signal_id = 0;
	gulong hook_id = 0;
	gpointer *klass = NULL;

	klass = g_type_class_ref(t);

	if(!g_signal_parse_name(n, t, &signal_id, &detail, TRUE))
		return 0;

	g_signal_query(signal_id, &query);

	sd = g_new(PurpleGSignalData, 1);
	sd->callback = cb;
	sd->user_data = d;
	sd->flags = 0;
	sd->detail = detail;
	sd->type = t;
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

