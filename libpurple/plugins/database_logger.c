#include "internal.h"

#include "debug.h"
#include "log.h"
#include "plugin.h"
#include "version.h"
#include <dbi/dbi.h>


struct _database_logger_info {
	PurpleLogLogger *logger;
	dbi_conn database_connection;
	gboolean connection_established;
};

static struct _database_logger_info *database_logger = NULL;

static gboolean is_connection_established() {
	return (database_logger != NULL && database_logger->connection_established);
}

static gboolean
plugin_load(PurplePlugin *plugin)
{
	int cnt = 0;

	database_logger = g_new0(struct _database_logger_info, 1);
	g_return_val_if_fail(plugin != NULL, FALSE);


#ifdef _WIN32
	cnt = dbi_initialize("libdbi-drivers");
#else
	cnt = dbi_initialize(NULL);
#endif

	purple_debug_info("Database Logger", "Count of loaded drivers =  %i\n", cnt);

	database_logger->database_connection = dbi_conn_new("mysql");
	if (database_logger->database_connection == NULL) {
		purple_debug_info("Database Logger", "created connection is NULL\n");
		return FALSE;
	}
	dbi_conn_set_option(database_logger->database_connection, "host", "192.168.0.128");
	dbi_conn_set_option(database_logger->database_connection, "username", "pidgin");
	dbi_conn_set_option(database_logger->database_connection, "password", "pidgin");
	dbi_conn_set_option(database_logger->database_connection, "dbname", "pidgin");
	dbi_conn_set_option(database_logger->database_connection, "encoding", "UTF-8");

	if (dbi_conn_connect(database_logger->database_connection) < 0) {
		purple_debug_info("Database Logger", "Could not connect. Please check the option settings\n");
		return FALSE;
	}

	database_logger->logger = purple_log_logger_new("database", _("Database Logger"), 1,
									   NULL);

	purple_log_logger_add(database_logger->logger);
	return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{
	purple_debug_info("Database Logger", "plugin_unload\n");
	g_return_val_if_fail(plugin != NULL, FALSE);

	if (database_logger != NULL)
		dbi_conn_close(database_logger->database_connection);

	dbi_shutdown();

	purple_log_logger_remove(database_logger->logger);
	g_free(database_logger);
	return TRUE;
}

static void                        
init_plugin(PurplePlugin *plugin)
{                                  
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
