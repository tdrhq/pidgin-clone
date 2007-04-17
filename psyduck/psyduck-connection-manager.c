#include "psyduck-connection-manager.h"
#include <glib.h>
#include "prpl.h"

G_DEFINE_TYPE(PsyduckConnectionManager,
    psyduck_connection_manager,
    TP_TYPE_BASE_CONNECTION_MANAGER)

static TpBaseConnection *
_psyduck_connection_manager_new_connection (TpBaseConnectionManager *self,
                                            const gchar *proto,
                                            TpIntSet *params_present,
                                            void *parsed_params,
                                            GError **error)
{
    return NULL;
}

static TpCMProtocolSpec *
get_protocols() {
    GList* iter;
    TpCMProtocolSpec *protocols, *protocol;
    guint n_protocols;
    TpCMParamSpec *params = g_new0(TpCMParamSpec, 1);
    
    iter = purple_plugins_get_protocols();
    n_protocols = g_list_length(iter);
    
    protocols = g_new0(TpCMProtocolSpec, n_protocols + 1);

    for (protocol = protocols; iter; iter = iter->next) {
        PurplePlugin *plugin = iter->data;
        PurplePluginInfo *info = plugin->info;
        if (info && info->id) {
            if(g_str_has_prefix(info->id, "prpl-")) {
                protocol->name = g_strdup(info->id + 5);
                protocol->parameters = params;
                protocol++;
            }
        }
    }
    return protocols;
}

static void
psyduck_connection_manager_class_init (PsyduckConnectionManagerClass *klass)
{
  TpBaseConnectionManagerClass *base_class =
    (TpBaseConnectionManagerClass *)klass;

  base_class->new_connection = _psyduck_connection_manager_new_connection;
  base_class->cm_dbus_name = "psyduck";
  base_class->protocol_params = get_protocols();
}

static void
psyduck_connection_manager_init (PsyduckConnectionManager *self)
{
}
