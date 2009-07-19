#ifndef PURPLE_SIGNALS_H
#define PURPLE_SIGNALS_H

#include <glib.h>
#include <glib-object.h>

#define PURPLE_TYPE_SIGNAL_HANDLE (purple_signal_handle_get_type())

typedef struct _PurpleGSignalHandle PurpleGSignalHandle;

G_BEGIN_DECLS

PurpleGSignalHandle *purple_g_signal_connect(GType t, const gchar *n, GCallback cb, gpointer d);
void purple_g_signal_disconnect(PurpleGSignalHandle *handle);

GType purple_g_signal_handle_get_gtype(void);
PurpleGSignalHandle *purple_g_signal_handle_new(guint signal_id, gulong hook_id);
PurpleGSignalHandle *purple_g_signal_handle_copy(const PurpleGSignalHandle *handle);
void purple_g_signal_handle_free(PurpleGSignalHandle *handle);

G_END_DECLS

#endif /* PURPLE_SIGNALS_H */
