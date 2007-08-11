#include "internal.h"

#include "debug.h"
#include "log.h"
#include "plugin.h"
#include "version.h"
#include <dbi/dbi.h>

#ifdef _WIN32
#include <glib.h>
#endif

typedef struct {
	PurpleLogLogger *logger;
	dbi_conn db_conn;
	gboolean conn_established;
	guint db_callback_id;
} DatabaseLoggerInfo;

typedef enum {
	PURPLE_DATABASE_LOGGER_INIT,
	PURPLE_DATABASE_LOGGER_READ,
	PURPLE_DATABASE_LOGGER_WRITE,
	PURPLE_DATABASE_LOGGER_LIST,
	PURPLE_DATABASE_LOGGER_SET,
	PURPLE_DATABASE_LOGGER_SYSTEM_LIST,
	PURPLE_DATABASE_LOGGER_SIZE,
	PURPLE_DATABASE_LOGGER_TOTAL_SIZE,
	PURPLE_DATABASE_LOGGER_CLOSE,
} DatabaseOperationType;

#define DATABASE_OPERATION_COUNT 9

typedef struct  {
	DatabaseOperationType type;
} DatabaseOperation;

static DatabaseLoggerInfo *db_logger = NULL;

static gpointer (*db_thread_func[DATABASE_OPERATION_COUNT])(gpointer data);
static gpointer (*db_callback_func[DATABASE_OPERATION_COUNT])(gpointer data);

/* List of finished in thread operations, such as getting total size, reading or writing */
static GList *db_finished_op = NULL;
static GMutex *db_mutex = NULL;

/**
 * Try to connect to database
 */
static gpointer db_init(gpointer data) 
{
	//purple_debug_info("Database Logger", "db_init\n");
	if (dbi_conn_connect(db_logger->db_conn) < 0) {
		purple_debug_info("Database Logger", "Could not connect. Please check the option settings\n");
	} else {

		g_mutex_lock(db_mutex);
		db_logger->logger = purple_log_logger_new("database", _("Database Logger"), 1,
										   NULL);
		db_logger->conn_established = TRUE;
		g_mutex_lock(db_mutex);

		purple_log_logger_add(db_logger->logger);
	}

	return NULL;
}

/**
 * Thread function, from which we do all operation with DB
 */
static gpointer db_thread(gpointer data) 
{
	DatabaseOperation *op = data;
	gpointer return_val = NULL;

	//purple_debug_info("Database Logger", "Now we in thread \n");
	int id = op->type;
	return_val = db_thread_func[id](data);

	g_mutex_lock(db_mutex);
	db_finished_op = g_list_append(db_finished_op, op);
	g_mutex_unlock(db_mutex);

	return return_val;
}

/**
 * This is a callback from main thread
 * if connection is established and we have finished in thread operations
 * then we notify about this sender
 */
static gboolean db_main_callback(gpointer data)
{
	if (!db_logger->conn_established || !db_finished_op)
		return TRUE;

	purple_debug_info("Database Logger", "Main callback there is finished op\n");

	g_mutex_lock(db_mutex);
	for(; db_finished_op != NULL; db_finished_op = g_list_remove_link(db_finished_op, db_finished_op)) {
		DatabaseOperation *op = db_finished_op->data;
		if (db_callback_func[op->type] != NULL)
			db_callback_func[op->type](op);
		g_free(op);
	}
	g_mutex_unlock(db_mutex);

	return TRUE;
}

static gboolean
plugin_load(PurplePlugin *plugin)
{
	int cnt = 0;

	db_logger = g_new0(DatabaseLoggerInfo, 1);
	g_return_val_if_fail(plugin != NULL, FALSE);

	db_mutex = g_mutex_new();

#ifdef _WIN32
	cnt = dbi_initialize("libdbi-drivers");
#else
	cnt = dbi_initialize(NULL);
#endif
	purple_debug_info("Database Logger", "Count of loaded drivers =  %i\n", cnt);

	g_mutex_lock(db_mutex);
	db_logger->db_conn = dbi_conn_new("mysql");
	if (db_logger->db_conn == NULL) {
		purple_debug_info("Database Logger", "created connection is NULL\n");
		return FALSE;
	}

	/*TODO: make customizable for several DB type*/
	dbi_conn_set_option(db_logger->db_conn, "host", "192.168.0.128");
	dbi_conn_set_option(db_logger->db_conn, "username", "pidgin");
	dbi_conn_set_option(db_logger->db_conn, "password", "pidgin");
	dbi_conn_set_option(db_logger->db_conn, "dbname", "pidgin");
	dbi_conn_set_option(db_logger->db_conn, "encoding", "UTF-8");
	g_mutex_unlock(db_mutex);

	db_logger->db_callback_id = purple_timeout_add(10, db_main_callback, NULL);

	purple_debug_info("Database Logger", "Creating thread and making init operation");
	if (db_thread_func[PURPLE_DATABASE_LOGGER_INIT] != NULL) {
		DatabaseOperation *op = g_new(DatabaseOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_INIT;
		g_thread_create(db_thread, op, FALSE, NULL);
	} 

	return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{
	purple_debug_info("Database Logger", "plugin_unload\n");
	g_return_val_if_fail(plugin != NULL, FALSE);

	if (db_logger != NULL)
		dbi_conn_close(db_logger->db_conn);

	dbi_shutdown();

	g_mutex_free(db_mutex);

	purple_timeout_remove(db_logger->db_callback_id);
	purple_log_logger_remove(db_logger->logger);
	g_free(db_logger);

	return TRUE;
}

static void                        
init_plugin(PurplePlugin *plugin)
{
	db_thread_func[PURPLE_DATABASE_LOGGER_INIT] = db_init;
	db_callback_func[PURPLE_DATABASE_LOGGER_INIT] = NULL;
}

static PurplePluginInfo info =
{
	PURPLE_PLUGIN_MAGIC,
	PURPLE_MAJOR_VERSION,
	PURPLE_MINOR_VERSION,
	PURPLE_PLUGIN_STANDARD,                             /**< type           */
	NULL,                                             /**< ui_requirement */
	0,                                                /**< flags          */
	NULL,                                             /**< dependencies   */
	PURPLE_PRIORITY_DEFAULT,                            /**< priority       */
	"core-database_logger",                                /**< id             */
	N_("Database Logger"),                                 /**< name           */
	VERSION,                                          /**< version        */

	/** summary */
	N_("Store your logs in database"),

	/** description */
	N_("Database Logger allows you to"
	   "store logs in database"),

	"Michael Shkutkov<mish@rsu.ru>",             /**< author         */
	PURPLE_WEBSITE,                                     /**< homepage       */
	plugin_load,                                      /**< load           */
	plugin_unload,                                    /**< unload         */
	NULL,                                             /**< destroy        */
	NULL,                                             /**< ui_info        */
	NULL,                                             /**< extra_info     */
	NULL,                                      /**< prefs_info     */
	NULL,                                             /**< actions        */

	/* padding */
	NULL,
	NULL,
	NULL,
	NULL
};

PURPLE_INIT_PLUGIN(database_logger, init_plugin, info)
