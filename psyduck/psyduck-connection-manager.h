#ifndef __PSYDUCK_CONNECTION_MANAGER_H__
#define __PSYDUCK_CONNECTION_MANAGER_H__

#include <glib-object.h>
#include <telepathy-glib/base-connection-manager.h>

G_BEGIN_DECLS

typedef struct _PsyduckConnectionManager PsyduckConnectionManager;
typedef struct _PsyduckConnectionManagerClass PsyduckConnectionManagerClass;

struct _PsyduckConnectionManagerClass {
    TpBaseConnectionManagerClass parent_class;
};

struct _PsyduckConnectionManager {
    TpBaseConnectionManager parent;
};

GType psyduck_connection_manager_get_type (void);

/* TYPE MACROS */
#define PSYDUCK_TYPE_CONNECTION_MANAGER \
  (psyduck_connection_manager_get_type ())
#define PSYDUCK_CONNECTION_MANAGER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), PSYDUCK_TYPE_CONNECTION_MANAGER, \
                              PsyduckConnectionManager))
#define PSYDUCK_CONNECTION_MANAGER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), PSYDUCK_TYPE_CONNECTION_MANAGER, \
                           PsyduckConnectionManagerClass))
#define PSYDUCK_IS_CONNECTION_MANAGER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), PSYDUCK_TYPE_CONNECTION_MANAGER))
#define PSYDUCK_IS_CONNECTION_MANAGER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), PSYDUCK_TYPE_CONNECTION_MANAGER))
#define PSYDUCK_CONNECTION_MANAGER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), PSYDUCK_TYPE_CONNECTION_MANAGER, \
                              PsyduckConnectionManagerClass))

G_END_DECLS

#endif /* #ifndef __PSYDUCK_CONNECTION_MANAGER_H__*/
