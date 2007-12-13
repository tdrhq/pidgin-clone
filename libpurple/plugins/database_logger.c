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

typedef struct {
	int id;
} ConversationInfo;

typedef enum {
	PURPLE_DATABASE_LOGGER_INIT,
	PURPLE_DATABASE_LOGGER_READ,
	PURPLE_DATABASE_LOGGER_WRITE,
	PURPLE_DATABASE_LOGGER_LIST,
	PURPLE_DATABASE_LOGGER_SETS,
	PURPLE_DATABASE_LOGGER_LIST_SYSLOG,
	PURPLE_DATABASE_LOGGER_SIZE,
	PURPLE_DATABASE_LOGGER_TOTAL_SIZE,
	PURPLE_DATABASE_LOGGER_REMOVE_LOG,
	PURPLE_DATABASE_LOGGER_CLOSE,
} DatabaseOperationType;

#define DATABASE_OPERATION_COUNT 10
#define SYSTEM_LOG_BUDDY_NAME "--SYSTEM-LOG--"
#define CHAT_BUDDY_NAME_TEMPLATE "--%s-CHAT--"

#define BUDDY_IM_TYPE 		0
#define BUDDY_CHAT_TYPE 	1
#define BUDDY_SYSTEM_TYPE 	2
#define BUDDY_ME_TYPE		3

typedef struct  {
	DatabaseOperationType type;
} DatabaseOperation;

typedef struct {
	DatabaseOperationType type;
	char *error_message;
	gboolean ret_value;
} DatabaseInitOperation;

typedef struct {
	DatabaseOperationType type;
	PurpleLog * log;
	PurpleMessageFlags flags;
	char *from;
	time_t time;
	char *message;
	PurpleLogLoggerWriteCallback cb;
	void *data;
	int ret_value;
} DatabaseWriteOperation;

typedef struct {
	DatabaseOperationType type;
	PurpleLog * log;
	PurpleLogReadFlags *flags;
	PurpleLogLoggerReadCallback cb;
	void *data;
	char *ret_value;
} DatabaseReadOperation;

typedef struct {
	DatabaseOperationType type;
	PurpleLogType log_type;
	char *name;
	PurpleAccount *account;
	PurpleLogLoggerListCallback cb;
	void *data;
	GList *ret_value;
} DatabaseListOperation;

typedef struct {
	DatabaseOperationType type;
	PurpleAccount *account;
	PurpleLogLoggerSystemListCallback cb;
	void *data;
	GList *ret_value;
} DatabaseSyslogListOperation;

typedef struct {
	DatabaseOperationType type;
	PurpleLog *log;
	PurpleLogLoggerSizeCallback cb;
	void *data;
	int ret_value;
} DatabaseSizeOperation;

typedef struct {
	DatabaseOperationType type;
	PurpleLogType log_type;
	char *name;
	PurpleAccount *account;
	PurpleLogLoggerTotalSizeCallback cb;
	void *data;
	int ret_value;
} DatabaseTotalSizeOperation;

typedef struct {
	DatabaseOperationType type;
	PurpleLogSetCallback set_cb;
	GHashTable *sets;
	PurpleLogLoggerHashTableCallback cb;
	void *data;
} DatabaseSetsOperation;

typedef struct {
	DatabaseOperationType type;
	PurpleLog *log;
	PurpleLogLoggerRemoveLogCallback cb;
	void *data;
	gboolean ret_value;
} DatabaseRemoveLogOperation;


typedef struct {
	PurpleDebugLevel level;
	const char *category;
	const char *message;
} AsynchronousDebugEntry;
GAsyncQueue* async_debug_entries = NULL;

void purple_debug_commit();
void purple_debug_store_async(PurpleDebugLevel level, const char *category, const char *format, ...);


static DatabaseLoggerInfo *db_logger = NULL;

static gpointer (*db_thread_func[DATABASE_OPERATION_COUNT])(gpointer data);
static void (*db_notify_func[DATABASE_OPERATION_COUNT])(gpointer data);

/* List of finished in thread operations, such as getting total size, reading or writing */
static GList *db_finished_op = NULL;

static GList *db_op_queue = NULL;

/* GLib specific part */
static GMutex *db_mutex = NULL;
static GThread *db_thread_id = NULL;

static void db_add_operation(gpointer data);

static const char *db_get_current_engine() 
{
	return purple_prefs_get_string("/plugins/core/database_logger/engine");
}

static char *db_current_pref_path(const char *pref) 
{
	return g_strdup_printf("/plugins/core/database_logger/%s/%s", db_get_current_engine(), pref);
}

static void database_logger_write(PurpleLog *log, PurpleMessageFlags type,
							  char *from, time_t time, char *message,
							  PurpleLogLoggerWriteCallback cb, void *data)
{
	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "Database logger write functions called\n");
	if (log == NULL || from == NULL || message == NULL)
		purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "ERROR wrong arguments: log = %x\t from = %x\t message = %x\n", log, from, message);

	if (db_thread_func[PURPLE_DATABASE_LOGGER_WRITE] != NULL) {
		DatabaseWriteOperation *op = g_new(DatabaseWriteOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_WRITE;
		op->log = log;
		op->flags = type;
		op->from = from;
		op->time = time;
		op->message = message;
		op->cb = cb;
		op->data = data;
		op->ret_value = 0;

		purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "Database logger write operation was added\n");
		db_add_operation(op);
	} else {
		cb(0, log, type, from, time, message, data);
	}
}

static void database_logger_read(PurpleLog *log, PurpleLogReadFlags *flags,
							PurpleLogLoggerReadCallback cb, void *data)
{
	if (log == NULL)
		purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "ERROR wrong arguments: log = %x\n", log);

	if (db_thread_func[PURPLE_DATABASE_LOGGER_READ] != NULL) {
		DatabaseReadOperation *op = g_new(DatabaseReadOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_READ;
		op->log = log;
		op->flags = flags;
		op->cb = cb;
		op->data = data;
		op->ret_value = NULL;

		db_add_operation(op);
	} else {
		cb(NULL, log, flags, data);
	}
}

static void database_logger_list(PurpleLogType type, char *name, PurpleAccount *account, 
								PurpleLogLoggerListCallback cb, void *data)
{
	if (name == NULL || account == NULL)
		purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "ERROR wrong arguments: name= %x\t account = %x\n", name, account);

	if (db_thread_func[PURPLE_DATABASE_LOGGER_LIST] != NULL) {
		DatabaseListOperation *op = g_new(DatabaseListOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_LIST;
		op->log_type = type;
		op->name = name;
		op->account = account;
		op->cb = cb;
		op->data = data;
		op->ret_value = NULL;

		db_add_operation(op);
	} else {
		cb(NULL, type, name, account, data);
	}
}

static void database_logger_list_syslog(PurpleAccount *account, PurpleLogLoggerSystemListCallback cb, void *data)
{
	if (account != NULL)
		purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "ERROR wrong arguments: account= %x\n", account);

	if (db_thread_func[PURPLE_DATABASE_LOGGER_LIST_SYSLOG] != NULL) {
		DatabaseSyslogListOperation *op = g_new(DatabaseSyslogListOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_LIST_SYSLOG;
		op->account = account;
		op->cb = cb;
		op->data = data;
		op->ret_value = NULL;

		db_add_operation(op);
	} else {
		cb(NULL, account, data);
	}
}

static void database_logger_size(PurpleLog *log, PurpleLogLoggerSizeCallback cb, void *data)
{
	if (log == NULL)
		purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "ERROR wrong arguments: log = %x\n", log);

	if (db_thread_func[PURPLE_DATABASE_LOGGER_SIZE] != NULL) {
		DatabaseSizeOperation *op = g_new(DatabaseSizeOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_SIZE;
		op->log = log;
		op->cb = cb;
		op->data = data;
		op->ret_value = 0;

		db_add_operation(op);
	} else {
		cb(0, log, data);
	}
}

static void database_logger_total_size(PurpleLogType type, char *name, PurpleAccount *account,
									PurpleLogLoggerTotalSizeCallback cb, void *data)
{
	if (name == NULL || account == NULL)
		purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "ERROR wrong arguments: name = %x\t account = %x\n", name, account);

	if (db_thread_func[PURPLE_DATABASE_LOGGER_TOTAL_SIZE] != NULL) {
		DatabaseTotalSizeOperation *op = g_new(DatabaseTotalSizeOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_TOTAL_SIZE;
		op->log_type = type;
		op->name = name;
		op->account = account;
		op->cb = cb;
		op->data = data;
		op->ret_value = 0;

		db_add_operation(op);
	} else {
		cb(0, type, name, account, data);
	}
}

static void database_logger_sets(PurpleLogSetCallback set_cb, GHashTable *sets, PurpleLogLoggerHashTableCallback cb, void *data)
{
	if (db_thread_func[PURPLE_DATABASE_LOGGER_SETS] != NULL) {
		DatabaseSetsOperation *op = g_new(DatabaseSetsOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_SETS;
		op->set_cb = set_cb;
		op->sets = sets;
		op->cb = cb;
		op->data = data;

		db_add_operation(op);
	} else {
		cb(data);
	}
}

static void database_logger_remove_log(PurpleLog *log, PurpleLogLoggerRemoveLogCallback cb, void *data)
{
	if (log == NULL)
		purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "ERROR wrong arguments: log = %x\n", log);

	if (db_thread_func[PURPLE_DATABASE_LOGGER_REMOVE_LOG] != NULL) {
		DatabaseRemoveLogOperation *op = g_new(DatabaseRemoveLogOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_REMOVE_LOG;
		op->log = log;
		op->cb = cb;
		op->data = data;
		op->ret_value = 0;

		db_add_operation(op);
	} else {
		cb(FALSE, log, data);
	}
}

/****************************************************************************
* Notify operation which let you know that datbase operation has finished.
****************************************************************************/

static void db_init_notify(gpointer data)
{
	DatabaseInitOperation *op = data;
	if (!op->ret_value) {
		const char *message = op->error_message;
		if (message == NULL)
			message = _("unknown error");
		purple_notify_error(NULL, _("Database Logger"), _("Connection not established"), message);
	}
	g_free(op->error_message);
}

static void db_write_notify(gpointer data) 
{
	DatabaseWriteOperation *op = data;

	if (op->cb != NULL) {
		op->cb(op->ret_value, op->log, op->flags, op->from, 
			op->time, op->message, op->data);
	}
}

static void db_read_notify(gpointer data) 
{
	DatabaseReadOperation *op = data;

	if (op->cb != NULL) {
		op->cb(op->ret_value, op->log, op->flags, op->data);
	}
}

static void db_list_notify(gpointer data) 
{
	DatabaseListOperation *op = data;
	if (op->cb != NULL) {
		op->cb(op->ret_value, op->type, op->name, op->account, op->data);
	}
}

static void db_syslog_list_notify(gpointer data) 
{
	DatabaseSyslogListOperation *op = data;
	if (op->cb != NULL) {
		op->cb(op->ret_value, op->account, op->data);
	}
}

static void db_size_notify(gpointer data) 
{
	DatabaseSizeOperation *op = data;
	if (op->cb != NULL) {
		op->cb(op->ret_value, op->log, op->data);
	}
}

static void db_sets_notify(gpointer data) 
{
	DatabaseSetsOperation *op = data;
	if (op->cb != NULL) {
		op->cb(op->data);
	}
}

static void db_total_size_notify(gpointer data) 
{
	DatabaseTotalSizeOperation *op = data;
	if (op->cb != NULL) {
		op->cb(op->ret_value, op->type, op->name, op->account, op->data);
	}
}

static void db_remove_notify(gpointer data) 
{
	DatabaseRemoveLogOperation *op = data;
	if (op->cb != NULL) {
		op->cb(op->ret_value, op->log, op->data);
	}
}

/****************************************************************************
* 						UTILS FUNCTIONS 
****************************************************************************/
/* This function is a copy of log.c static one. May be we should make this function public */
static char *log_get_timestamp(PurpleLog *log, time_t when)
{
	gboolean show_date;
	char *date;
	struct tm tm;

	show_date = (log->type == PURPLE_LOG_SYSTEM) || (time(NULL) > when + 20*60);

	date = purple_signal_emit_return_1(purple_log_get_handle(),
	                          "log-timestamp",
	                          log, when, show_date);
	if (date != NULL)
		return date;

	tm = *(localtime(&when));
	if (show_date)
		return g_strdup(purple_date_format_long(&tm));
	else
		return g_strdup(purple_time_format(&tm));
}

/* This function is a copy of log.c static one. May be we should make this function public */
static void
log_add_log_set_to_hash(GHashTable *sets, PurpleLogSet *set)
{
	PurpleLogSet *existing_set = g_hash_table_lookup(sets, set);

	if (existing_set == NULL)
		g_hash_table_insert(sets, set, set);
	else if (existing_set->account == NULL && set->account != NULL)
		g_hash_table_replace(sets, set, set);
	else
		purple_log_set_free(set);
}

static void lock()
{
	g_mutex_lock(db_mutex);
}

static void unlock()
{
	g_mutex_unlock(db_mutex);
}

static int db_result_get_int(dbi_result dres, const char *fieldname) 
{
#ifdef dbi_result_get_int
	return dbi_result_get_int(dres, fieldname);
#else
	return dbi_result_get_long(dres, fieldname);
#endif
}

static void db_print_error() 
{
	const char *message = NULL;
	dbi_conn_error(db_logger->db_conn, &message);
	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "ERROR: %s\n", message);
}

static gboolean db_process_result(dbi_result dres)
{
	if (dres) {
		dbi_result_free(dres);
		return TRUE;
	} else {
		db_print_error();
		return FALSE;
	}
}

static char *db_escape_string(const char *orig_string)
{
	char *ret_value = NULL;
#ifdef dbi_conn_quote_string_copy
	dbi_conn_quote_string_copy(db_logger->db_conn, orig_string, &ret_value);
#else
	dbi_driver_quote_string_copy(dbi_conn_get_driver(db_logger->db_conn), orig_string, &ret_value);
#endif
	return ret_value;
}

static gboolean db_retrieve_int_value(dbi_result dres, int *value, const char *field_name) 
{
	if (dres) {
		/* TODO: check if there are several rows */
		while (dbi_result_next_row(dres)) {
			if (value)
				*value = db_result_get_int(dres, field_name);
		}
	}
	return db_process_result(dres);
}

static gboolean db_retrieve_string_value(dbi_result dres, char **value, const char *field_name) 
{
	*value = NULL;
	if (dres) {
		/* TODO: check if there are several rows */
		while (dbi_result_next_row(dres)) {
			if (*value)
				g_free(*value);
			*value = g_strdup(dbi_result_get_string(dres, field_name));
		}
	}
	return db_process_result(dres);
}

static gboolean db_retrieve_id(dbi_result dres, int *id) 
{
	return db_retrieve_int_value(dres, id, "id");
}


static int db_get_protocol_id(PurpleAccount *account) 
{
	PurplePlugin *prpl = purple_find_prpl(purple_account_get_protocol_id(account));
	PurplePluginProtocolInfo *prpl_info;
	const char *protocol_name;
	int id = -1;

	g_return_val_if_fail(prpl != NULL, id);

	prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(prpl);
	g_return_val_if_fail(prpl_info != NULL, id);
	protocol_name = prpl_info->list_icon(account, NULL);

	/* TODO: optimization */

	/* we can use dbi_conn_sequence_last to get row ID generated by the last INSERT command
	    when we add an account, but it seems that the result depends on database */
	while (id == -1) {
		dbi_result dres = dbi_conn_queryf(db_logger->db_conn, 
						"SELECT id FROM protocols WHERE name = \"%s\"", protocol_name);
		if (!db_retrieve_id(dres, &id)) 
			break;

		/*if id not found we should insert new protocol */
		if (id == -1) {
			dres = dbi_conn_queryf(db_logger->db_conn, "INSERT INTO protocols(`name`) VALUES(\"%s\")", 
								protocol_name);
			if (!db_process_result(dres))
				break;
		}
	}

	return id;
}

static char *db_get_protocol_name(int id)
{
	char *name = NULL;
	dbi_result dres =  dbi_conn_queryf(db_logger->db_conn, 
						"SELECT `name` FROM `protocols` WHERE `id` = %i", id);
	db_retrieve_string_value(dres, &name, "name");
	return name;
}

static int db_get_account_id(PurpleAccount *account) 
{
	char *account_name;
	int id = -1;

	g_return_val_if_fail(account != NULL, id);

	account_name = g_strdup(purple_escape_filename(purple_normalize(account,
				purple_account_get_username(account))));

	/* TODO: optimization */

	/* we can use dbi_conn_sequence_last to get row ID generated by the last INSERT command
	    when we add an account, but it seems that the result depends on database */
	while (id == -1) {
		dbi_result dres = dbi_conn_queryf(db_logger->db_conn, "SELECT id FROM accounts WHERE name = \"%s\"", account_name);

		if (!db_retrieve_id(dres, &id)) 
			break;

		/*if id not found we should insert new account */
		if (id == -1) {
			dres = dbi_conn_queryf(db_logger->db_conn, "INSERT INTO `accounts`(`name`, `protocolId`) VALUES(\"%s\", %i)", 
								account_name, db_get_protocol_id(account));
			if (!db_process_result(dres))
				break;
		}
	}

	g_free(account_name);
	return id;
}

static PurpleAccount *db_get_account(int id) 
{
	char *protocol_name = NULL;
	char *account_name = NULL;
	PurpleAccount *account = NULL;
	dbi_result dres;

	g_return_val_if_fail(id != -1, account);

	dres = dbi_conn_queryf(db_logger->db_conn, "SELECT `name`, `protocolId` FROM `accounts` WHERE `id` =  %i", id);
	if(dres) {
		int protocol_id = -1;
		
		while (dbi_result_next_row(dres)) {
			if (account_name)
				g_free(account_name);
			account_name = g_strdup(dbi_result_get_string(dres, "name"));
			protocol_id = db_result_get_int(dres, "protocolId");
		}
		if (protocol_id != -1)
			protocol_name = db_get_protocol_name(protocol_id);
	}
	db_process_result(dres);

	if (protocol_name != NULL && account_name != NULL) {
		GList *account_iter;

		for (account_iter = purple_accounts_get_all() ; account_iter != NULL ; account_iter = account_iter->next) {
			PurplePlugin *prpl;
			PurplePluginProtocolInfo *prpl_info;

			prpl = purple_find_prpl(purple_account_get_protocol_id((PurpleAccount *)account_iter->data));
			if (!prpl)
				continue;
			prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(prpl);

			if (!strcmp(protocol_name, prpl_info->list_icon((PurpleAccount *)account_iter->data, NULL)) &&
				!strcmp(((PurpleAccount *)account_iter->data)->username, account_name)) {
					account = account_iter->data;
					break;
				}
		}
	}
	g_free(protocol_name);
	g_free(account_name);

	return account;
}

static int db_get_buddy_id(PurpleLogType type, const char *name, PurpleAccount *account) 
{
	char *buddy_name = NULL;
	int id = -1;
	int buddy_type = 0;

	g_return_val_if_fail(account != NULL, id);

	/* TODO: optimization */

	if (type == PURPLE_LOG_SYSTEM) {
		buddy_name = g_strdup(SYSTEM_LOG_BUDDY_NAME);
		buddy_type = BUDDY_SYSTEM_TYPE;
	} else if (type == PURPLE_LOG_CHAT) {
		buddy_name = g_strdup_printf(CHAT_BUDDY_NAME_TEMPLATE, purple_normalize(account, name));
		buddy_type = BUDDY_CHAT_TYPE;
	} else {
		buddy_name = g_strdup(purple_normalize(account, name));
		buddy_type = BUDDY_IM_TYPE;
	}

	/* we can use dbi_conn_sequence_last to get row ID generated by the last INSERT command
	    when we add an account, but it seems that the result depends on database */
	while (id == -1) {
		dbi_result dres = dbi_conn_queryf(db_logger->db_conn, "SELECT `id` FROM `buddies` WHERE `name`= \"%s\" AND `type` = %i AND `accountID` = %i ", 
									buddy_name, buddy_type, db_get_account_id(account));
		if (!db_retrieve_id(dres, &id)) 
			break;

		/*if id not found we should insert new account */
		if (id == -1) {
			dres = dbi_conn_queryf(db_logger->db_conn, "INSERT INTO `buddies` (`name`, `type`, `accountId`) VALUES(\"%s\", %i, %i)", 
								buddy_name, buddy_type, db_get_account_id(account));
			if (!db_process_result(dres))
				break;
		}
	}

	g_free(buddy_name);
	return id;
}

static int db_get_conversation_size(int id)
{
	int ret_value = 0;
	dbi_result dres;
	g_return_val_if_fail(id != -1, ret_value);

	dres = dbi_conn_queryf(db_logger->db_conn, "SELECT `size` FROM `conversations` WHERE `id` = %i", id);
	db_retrieve_int_value(dres, &ret_value, "size");
	return ret_value;
}

static char *txt_message_formatting(const char *from, const char *message, int message_flags, 
								time_t datetime, PurpleLog *log)
{
	/* txt logger formatting */
	char *str = NULL;
	char *date = log_get_timestamp(log, datetime);
	/* need get log type from DB */

	g_return_val_if_fail(from != NULL && message != NULL && log != NULL, NULL);

	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "append_message_to_output: flags = %x from = %s\n", message_flags, from);

	date = log_get_timestamp(log, time);

	if(log->type == PURPLE_LOG_SYSTEM){
		str = g_strdup_printf("---- %s @ %s ----\n", message, date);
	} else {
		if (message_flags & PURPLE_MESSAGE_SYSTEM)
			str = g_strdup_printf("<font size=\"2\">(%s)</font><b> %s</b>\n", date, message);
		else if (message_flags & PURPLE_MESSAGE_RAW)
			str = g_strdup_printf("<font size=\"2\">(%s)</font> %s\n", date, message);
		else if (message_flags & PURPLE_MESSAGE_ERROR)
			str = g_strdup_printf("<font color=\"#FF0000\"><font size=\"2\">(%s)</font><b> %s</b></font>\n", date, message);
		else if (message_flags & PURPLE_MESSAGE_WHISPER)
			str = g_strdup_printf("<font color=\"#6C2585\"><font size=\"2\">(%s)</font><b> %s:</b></font> %s\n",
					date, from, message);
		else if (message_flags & PURPLE_MESSAGE_AUTO_RESP) {
			if (message_flags & PURPLE_MESSAGE_SEND)
				str = g_strdup_printf(_("<font color=\"#16569E\"><font size=\"2\">(%s)</font> <b>%s &lt;AUTO-REPLY&gt;:</b></font> %s\n"), date, from, message);
			else if (message_flags & PURPLE_MESSAGE_RECV)
				str = g_strdup_printf(_("<font color=\"#A82F2F\"><font size=\"2\">(%s)</font> <b>%s &lt;AUTO-REPLY&gt;:</b></font> %s\n"), date, from, message);
		} else if (message_flags & PURPLE_MESSAGE_RECV) {
			if(purple_message_meify(message, -1))
				str = g_strdup_printf("<font color=\"#062585\"><font size=\"2\">(%s)</font> <b>***%s</b></font> %s\n",
						date, from, message);
			else
				str = g_strdup_printf("<font color=\"#A82F2F\"><font size=\"2\">(%s)</font> <b>%s:</b></font> %s\n",
						date, from, message);
		} else if (message_flags & PURPLE_MESSAGE_SEND) {
			if(purple_message_meify(message, -1))
				str = g_strdup_printf("<font color=\"#062585\"><font size=\"2\">(%s)</font> <b>***%s</b></font> %s\n",
						date, from, message);
			else
				str = g_strdup_printf("<font color=\"#16569E\"><font size=\"2\">(%s)</font> <b>%s:</b></font> %s\n",
						date, from, message);
		} else {
			purple_debug_error("log", "Unhandled message message_flags.\n");
			str = g_strdup_printf("<font size=\"2\">(%s)</font><b> %s:</b></font> %s\n",
						date, from, message);
		}
	}
	g_free(date);

	return str;
}

static char *txt_concat_messages(GList *list)
{
	char *result = NULL;
	char *buffer;
	int size = 0;
	GList *item;

	if (list == NULL)
		return g_strdup("<font color=\"red\"><b>empty log!</b></font>");

	for (item = list; item != NULL; item = g_list_next(item))
		size += strlen((char *)item->data);

	result = g_new(char, size + 1);
	buffer = result;
	for (item = list; item != NULL; item = g_list_next(item)) {
		buffer = g_stpcpy(buffer, item->data);
		g_free(item->data);
	}

	g_list_free(list);

	return result;
}

static GList *get_list_log(PurpleLogType type, const char *name, PurpleAccount *account)
{
	dbi_result dres;
	GList *list = NULL;

	g_return_val_if_fail(name != NULL && account != NULL, list);

	dres = dbi_conn_queryf(db_logger->db_conn,
					"SELECT `id`, `datetime` FROM `conversations` WHERE `accountId` = %i AND `buddyId` = %i",
					db_get_account_id(account), db_get_buddy_id(type, name, account));
	if (dres) {
		int id;
		time_t datetime;

		while(dbi_result_next_row(dres)) {
			PurpleLog *log;
			ConversationInfo *conv_info;
			struct tm *tm;
			id = db_result_get_int(dres, "id");
			datetime = db_result_get_int(dres, "datetime");

			tm = localtime(&datetime);
			log = purple_log_new(type, name, account, NULL, datetime, (datetime != 0) ?  tm : NULL);
			log->logger = db_logger->logger;
			log->logger_data = conv_info = g_slice_new0(ConversationInfo);
			conv_info->id = id;
			list = g_list_prepend(list, log);
		}
	}
	db_process_result(dres);
	return list;
}

/****************************************************************************
* Main database functions which perform in thread
****************************************************************************/
/**
 * Try to connect to database and create database logger
 * Try to connect to database and create database logger
 */
static gpointer db_init(gpointer data) 
{
	DatabaseInitOperation *op = data;
	if (dbi_conn_connect(db_logger->db_conn) < 0) {
		const char *message = NULL;
		purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "Could not connect. Please check the option settings\n");
		db_print_error();
		dbi_conn_error(db_logger->db_conn, &message);
		op->error_message = g_strdup(message);
		op->ret_value = FALSE;
	} else {

		lock();

		db_logger->logger = purple_log_logger_new("database", _("Database Logger"), 21,
										   NULL,
										   NULL,
										   NULL,
										   NULL,
										   NULL,
										   NULL,
										   NULL,
										   NULL,
										   NULL,
										   NULL,
										   NULL,
										   NULL,
										   database_logger_write,
										   NULL,
										   database_logger_list,
										   database_logger_read,
										   database_logger_size,
										   database_logger_total_size,
										   database_logger_list_syslog,
										   database_logger_sets,
										   database_logger_remove_log);
		db_logger->conn_established = TRUE;
		purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "Logger was successfully added\n");
		purple_log_logger_add(db_logger->logger);

		unlock();
		op->ret_value = TRUE;
	}
	return NULL;
}

static gpointer db_write(gpointer data)
{
	DatabaseWriteOperation *op = data;
	PurpleLog *log = op->log;
	PurpleMessageFlags flags = op->flags;
	const char *from = op->from;
	time_t time = op->time;
	char *message = op->message;
	ConversationInfo *conv_info = log->logger_data;
	dbi_result dres;

	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "---- XXX ---- db_write: from = %s; message = %s\n", from, message);
	op->ret_value = 0;

	if (log->logger_data == NULL) {
		log->logger_data = conv_info = g_new(ConversationInfo, 1);
		conv_info->id = -1;

		/* create new conversation in log */
		dres = dbi_conn_queryf(db_logger->db_conn, 
			"INSERT INTO `conversations` (`datetime`, `size`, `accountId`, `buddyId`) VALUES(%i, %i, %i, %i)", 
			time, 0, db_get_account_id(log->account), db_get_buddy_id(log->type, log->name, log->account));
		db_process_result(dres);

		/* we can use dbi_conn_sequence_last to get row ID generated by the last INSERT command
		when we add an account, but it seems that the result depends on database, because
		some databases need exlicit sequences*/
		dres = dbi_conn_queryf(db_logger->db_conn, 
			"SELECT `id` FROM `conversations` WHERE `datetime` = %i AND `accountID` = %i AND `buddyId` = %i", 
									time, db_get_account_id(log->account), db_get_buddy_id(log->type, log->name, log->account));
		db_retrieve_id(dres, &conv_info->id);
	}

	if (conv_info->id != -1) {
		int log_size = db_get_conversation_size(conv_info->id);
		char *escaped_string = db_escape_string(message);
		purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "---- XXX ---- db_write: escaped_string = %s\n", escaped_string);

		dres = dbi_conn_queryf(db_logger->db_conn, 
			"INSERT INTO `messages` (`conversationId`, `ownerName`, `datetime`, `text`, `flags`) VALUES(%i, \"%s\", %i, %s, %i)",
			conv_info->id, from, time, escaped_string, flags);
		db_process_result(dres);

		/* updating log size */
		log_size += strlen(escaped_string);
		dres = dbi_conn_queryf(db_logger->db_conn,
				"UPDATE `conversations` SET `size` = %i WHERE `id` = %i",
				log_size, conv_info->id);
		g_free(escaped_string);

	} else 
		purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "ERROR: conv_info->id == -1\n");

	return NULL;
}

static gpointer db_read(gpointer data)
{
	DatabaseReadOperation *op = data;
	PurpleLog *log = op->log;
	PurpleLogReadFlags *flags = op->flags;
	ConversationInfo *conv_info = log->logger_data;
	dbi_result dres;

	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "---- XXX ---- db_read \n");
	if (flags)
		*flags = 0;

	op->ret_value = NULL;

	if (!conv_info || conv_info->id == -1)
		op->ret_value = g_strdup(_("<font color=\"red\"><b>Unable to find conversation id!</b></font>"));
	else {
		dres = dbi_conn_queryf(db_logger->db_conn, 
				"SELECT `ownerName`, `datetime`, `text`, `flags` FROM `messages` WHERE `conversationId` = %i ORDER BY `datetime`",
				conv_info->id);
		if (dres) {
			int message_flags;
			time_t datetime;
			const char *ownerName;
			const char *message;
			GList *list = NULL;

			while(dbi_result_next_row(dres)) {
				char *formatted_message;

				ownerName = dbi_result_get_string(dres, "ownerName");
				datetime = db_result_get_int(dres, "datetime");
				message = dbi_result_get_string(dres, "text");
				message_flags = db_result_get_int(dres, "flags");

				/* we can form output as we wish */
				formatted_message = txt_message_formatting(ownerName, message, message_flags, datetime, log);
				if (formatted_message != NULL)
					list = g_list_append(list, formatted_message);

			}
			op->ret_value = txt_concat_messages(list);
		}
		db_process_result(dres);
	}

	return NULL;
}


static gpointer db_list(gpointer data)
{
	DatabaseListOperation *op = data;

	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "---- XXX ---- db_list: account = %s , name = %s\n", purple_account_get_username(op->account), op->name);

	op->ret_value = get_list_log(op->log_type, op->name, op->account);

	return NULL;
}

static gpointer db_syslog_list(gpointer data)
{
	DatabaseSyslogListOperation *op = data;

	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "---- XXX ---- db_syslog_list: account = %s\n", purple_account_get_username(op->account));

	op->ret_value = get_list_log(PURPLE_LOG_SYSTEM, SYSTEM_LOG_BUDDY_NAME, op->account);

	return NULL;
}

static gpointer db_size(gpointer data)
{
	DatabaseSizeOperation *op = data;
	ConversationInfo *conv_info = op->log->logger_data;

	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "---- XXX ---- db_size: account = %s\n", purple_account_get_username(op->log->account));

	op->ret_value = 0;
	if (conv_info && conv_info->id != -1){
		dbi_result dres;

		dres = dbi_conn_queryf(db_logger->db_conn, "SELECT `size` FROM `conversations` WHERE `conversationId` = %i",
							conv_info->id);
		db_retrieve_int_value(dres, &op->ret_value, "size") ;
	}

	return NULL;
}

static gpointer db_total_size(gpointer data)
{
	DatabaseTotalSizeOperation *op = data;
	dbi_result dres;

	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "---- XXX ---- db_total_size: account = %s\n", purple_account_get_username(op->account));

	op->ret_value = 0;
	dres = dbi_conn_queryf(db_logger->db_conn, "SELECT SUM(`size`) FROM `conversations` WHERE `accountId` = %i AND `buddyId` = %i",
							db_get_account_id(op->account), db_get_buddy_id(op->log_type, op->name, op->account));
							
	if (dres) {
		const char *string = NULL;
		/* TODO: check if there are several rows */
		while (dbi_result_next_row(dres)) {
			string = (const char *)dbi_result_get_binary(dres, "SUM(`size`)");
			op->ret_value = atoi(string);
		}
	}
	db_process_result(dres);

	return NULL;
}

static gpointer db_sets(gpointer data)
{
	DatabaseSetsOperation *op = data;
	dbi_result dres;

	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "---- XXX ---- db_sets\n");

	dres = dbi_conn_queryf(db_logger->db_conn, "SELECT `buddyId`, `accountId` FROM `conversations`");
	if (dres) {
		while(dbi_result_next_row(dres)) {
			int buddy_id = -1;
			int account_id = -1;
			PurpleAccount *account = NULL;
			PurpleLogSet *set;

			buddy_id = db_result_get_int(dres, "buddyId");
			account_id = db_result_get_int(dres, "accountId");

			char* buddy_name = "";
			int type = 0;
			dbi_result dres2;
			dres2 = dbi_conn_queryf(db_logger->db_conn, "SELECT `name`, `type` FROM `buddies` WHERE `id` = %i", buddy_id);
			if(dres2) {
				dbi_result_next_row(dres2);
				buddy_name = dbi_result_get_string(dres2, "name");
				type = db_result_get_int(dres2, "type");
			} else purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "dres2 = null\n");
			db_process_result(dres2);

			account = db_get_account(account_id);
			if (account != NULL) {
				set = g_slice_new(PurpleLogSet);

				set->name = g_strdup(buddy_name);
				switch (type){
					case BUDDY_IM_TYPE:
						set->type = PURPLE_LOG_IM;
						break;
					case BUDDY_CHAT_TYPE:
						set->type = PURPLE_LOG_CHAT;
						break;
					case BUDDY_SYSTEM_TYPE:
						set->type = PURPLE_LOG_SYSTEM;
						break;
					default:
						/* let it be default value */
						set->type = PURPLE_LOG_IM;
				}
				set->account = account;
				set->buddy = (purple_find_buddy(account, buddy_name) != NULL);
				set->normalized_name = g_strdup(purple_normalize(account, buddy_name));
				op->set_cb(op->sets, set);
			}
		}
	}
	db_process_result(dres);
	return NULL;
}

static gpointer db_remove(gpointer data)
{
	DatabaseRemoveLogOperation *op = data;
	ConversationInfo *conv_info = op->log->logger_data;

	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "---- XXX ---- db_remove: account = %s\n", purple_account_get_username(op->log->account));

	op->ret_value = FALSE;
	if (conv_info && conv_info->id != -1){
		dbi_result dres = dbi_conn_queryf(db_logger->db_conn, "DELETE FROM `messages` WHERE `conversationId` = %i",
							conv_info->id);
		if (db_process_result(dres)) {
			dres = dbi_conn_queryf(db_logger->db_conn, "DELETE FROM `conversations` WHERE `id` = %i",
								conv_info->id);
			db_process_result(dres);
			op->ret_value = TRUE;
		}
	}
	return NULL;
}

/**
 * Thread function, from which we do all operation with DB
 */
static gpointer db_thread(gpointer data) 
{
	gpointer return_val = NULL;
	GList *op_queue = NULL;
	gboolean read_queue = TRUE;

	while(read_queue) {
	
		lock();
		op_queue = db_op_queue;
		db_op_queue = NULL;
		unlock();

		for(; op_queue != NULL; op_queue = g_list_delete_link(op_queue, op_queue)) {
			DatabaseOperation *op = op_queue->data;
			int id = op->type;

			if (db_thread_func[id] != NULL)
				return_val = db_thread_func[id](op);

			/* Locking mutex, because we are going to add new 
			   item to db_funished_op */
			lock();
			db_finished_op = g_list_append(db_finished_op, op);
			unlock();
		}

		lock();
		read_queue = (db_op_queue != NULL);
		unlock();
	}

	lock();
	db_thread_id = NULL;
	unlock();

	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "Thread finished successfully: %x\n", db_thread_id);
	return return_val;
}

static void db_add_operation(gpointer data)
{
	gboolean need_create_thread = FALSE;
	DatabaseOperation *op = data;

	lock();

	db_op_queue = g_list_append(db_op_queue, op);
	need_create_thread = (db_thread_id == NULL);

	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "before creation db_thread_id = %x\n", db_thread_id);

	if (need_create_thread) {
		GError *error = NULL;

		purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", " -- Thread created -- \n");

		db_thread_id = g_thread_create(db_thread, NULL, FALSE, &error);
		if (error != NULL) {
			purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "error: %s\n", error->message);
			g_error_free(error);
		}
	}
	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "after creation db_thread_id = %x\n", db_thread_id);

	unlock();
}


/**
 * This is a callback from main thread
 * if connection is established and we have finished in thread operations
 * then we notify about this sender
 */
static gboolean db_main_callback(gpointer data)
{
	GList *op_queue = NULL;
	gboolean exit_callback = TRUE;

	lock();
	exit_callback = (!db_finished_op);
	unlock();

	if (!exit_callback) {

		purple_debug(PURPLE_DEBUG_INFO,"Database Logger", "Main callback: there is finished operation\n");

		lock();
		op_queue = db_finished_op;
		db_finished_op = NULL;
		unlock();

		purple_debug_commit();

		for(; op_queue != NULL; op_queue = g_list_delete_link(op_queue, op_queue)) {
			DatabaseOperation *op = op_queue->data;

			if (op == NULL) purple_debug(PURPLE_DEBUG_INFO,"Database Logger", "ERROR: db_main_callback op == NULL\n");
			else if (db_notify_func[op->type] != NULL) {
				db_notify_func[op->type](op);
			}
			g_free(op);
		}

		purple_debug_commit();
	}

	return TRUE;
}

static gboolean db_create_connection()
{
	char *pref_path;
	db_logger->db_conn = dbi_conn_new(db_get_current_engine());

	if (db_logger->db_conn == NULL) {
		purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "created connection is NULL\n");
		return FALSE;
	}

	/*TODO: make customizable for several DB type*/
	pref_path = db_current_pref_path("host");
	dbi_conn_set_option(db_logger->db_conn, "host", purple_prefs_get_string(pref_path));
	g_free(pref_path);

	pref_path = db_current_pref_path("username");
	dbi_conn_set_option(db_logger->db_conn, "username", purple_prefs_get_string(pref_path));
	g_free(pref_path);

	pref_path = db_current_pref_path("password");
	dbi_conn_set_option(db_logger->db_conn, "password", purple_prefs_get_string(pref_path));
	g_free(pref_path);

	pref_path = db_current_pref_path("dbname");
	dbi_conn_set_option(db_logger->db_conn, "dbname", purple_prefs_get_string(pref_path));
	g_free(pref_path);

	pref_path = db_current_pref_path("encoding");
	dbi_conn_set_option(db_logger->db_conn, "encoding", purple_prefs_get_string(pref_path));
	g_free(pref_path);

	pref_path = db_current_pref_path("port");
	dbi_conn_set_option_numeric(db_logger->db_conn, "port", purple_prefs_get_int(pref_path));
	g_free(pref_path);

	db_logger->db_callback_id = purple_timeout_add(100, db_main_callback, NULL);

	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "Creating thread and making init operation\n");
	if (db_thread_func[PURPLE_DATABASE_LOGGER_INIT] != NULL) {
		DatabaseInitOperation *op = g_new0(DatabaseInitOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_INIT;
		db_add_operation(op);
	} 
	return TRUE;
}

static gboolean db_close_connection()
{
	dbi_conn_close(db_logger->db_conn);
}

static gboolean
plugin_load(PurplePlugin *plugin)
{
	int cnt = 0;

	g_return_val_if_fail(plugin != NULL, FALSE);

	db_logger = g_new0(DatabaseLoggerInfo, 1);

	g_return_val_if_fail(db_logger != NULL, FALSE);

	db_mutex = g_mutex_new();
	async_debug_entries = g_async_queue_new();

#ifdef _WIN32
	cnt = dbi_initialize("libdbi-drivers");
#else
	cnt = dbi_initialize(NULL);
#endif

	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "Count of loaded drivers =  %i\n", cnt);

	return db_create_connection();
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{
	purple_debug_store_async(PURPLE_DEBUG_INFO,"Database Logger", "plugin_unload\n");
	g_return_val_if_fail(plugin != NULL, FALSE);

	g_mutex_free(db_mutex);
	g_async_queue_unref(async_debug_entries);

	purple_timeout_remove(db_logger->db_callback_id);

	if (db_logger->logger != NULL) 
		purple_log_logger_remove(db_logger->logger);

	db_close_connection();
	dbi_shutdown();

	g_free(db_logger);

	return TRUE;
}

static void                        
init_plugin(PurplePlugin *plugin)
{
	db_thread_func[PURPLE_DATABASE_LOGGER_INIT] = db_init;
	db_notify_func[PURPLE_DATABASE_LOGGER_INIT] = db_init_notify;

	db_thread_func[PURPLE_DATABASE_LOGGER_WRITE] = db_write;
	db_notify_func[PURPLE_DATABASE_LOGGER_WRITE] = db_write_notify;

	db_thread_func[PURPLE_DATABASE_LOGGER_READ] = db_read;
	db_notify_func[PURPLE_DATABASE_LOGGER_READ] = db_read_notify;

	db_thread_func[PURPLE_DATABASE_LOGGER_LIST] = db_list;
	db_notify_func[PURPLE_DATABASE_LOGGER_LIST] = db_list_notify;

	db_thread_func[PURPLE_DATABASE_LOGGER_LIST_SYSLOG] = db_syslog_list;
	db_notify_func[PURPLE_DATABASE_LOGGER_LIST_SYSLOG] = db_syslog_list_notify;

	db_thread_func[PURPLE_DATABASE_LOGGER_SIZE] = db_size;
	db_notify_func[PURPLE_DATABASE_LOGGER_SIZE] = db_size_notify;

	db_thread_func[PURPLE_DATABASE_LOGGER_TOTAL_SIZE] = db_total_size;
	db_notify_func[PURPLE_DATABASE_LOGGER_TOTAL_SIZE] = db_total_size_notify;

	db_thread_func[PURPLE_DATABASE_LOGGER_SETS] = db_sets;
	db_notify_func[PURPLE_DATABASE_LOGGER_SETS] = db_sets_notify;

	db_thread_func[PURPLE_DATABASE_LOGGER_REMOVE_LOG] = db_remove;
	db_notify_func[PURPLE_DATABASE_LOGGER_REMOVE_LOG] = db_remove_notify;

	purple_prefs_add_none("/plugins/core/database_logger");
	purple_prefs_add_string("/plugins/core/database_logger/engine", "mysql");

	/* mysql prefs part */
	purple_prefs_add_none("/plugins/core/database_logger/mysql");
	purple_prefs_add_string("/plugins/core/database_logger/mysql/host", "127.0.0.1");
	purple_prefs_add_string("/plugins/core/database_logger/mysql/username", "");
	purple_prefs_add_string("/plugins/core/database_logger/mysql/password", "");
	purple_prefs_add_string("/plugins/core/database_logger/mysql/dbname", "");
	purple_prefs_add_string("/plugins/core/database_logger/mysql/encoding", "");
	purple_prefs_add_int("/plugins/core/database_logger/mysql/port", 3306);

}

static PurplePluginPrefFrame *
get_plugin_pref_frame(PurplePlugin *plugin)
{
	PurplePluginPrefFrame *frame;
	PurplePluginPref *ppref;
	char *pref_path;

	g_return_val_if_fail(plugin != NULL, FALSE);

	frame = purple_plugin_pref_frame_new();

	/* Add general preferences. */

	ppref = purple_plugin_pref_new_with_label(_("Database Logger Configuration"));
	purple_plugin_pref_frame_add(frame, ppref);

	pref_path = db_current_pref_path("host");
	ppref = purple_plugin_pref_new_with_name_and_label(
		pref_path, _("Hostname"));
	purple_plugin_pref_frame_add(frame, ppref);
	g_free(pref_path);

	pref_path = db_current_pref_path("port");
	ppref = purple_plugin_pref_new_with_name_and_label(
		pref_path, _("Port"));
	purple_plugin_pref_frame_add(frame, ppref);
	g_free(pref_path);

	pref_path = db_current_pref_path("username");
	ppref = purple_plugin_pref_new_with_name_and_label(
		pref_path, _("Username"));
	purple_plugin_pref_frame_add(frame, ppref);
	g_free(pref_path);

	pref_path = db_current_pref_path("password");
	ppref = purple_plugin_pref_new_with_name_and_label(
		pref_path, _("Password"));
	purple_plugin_pref_frame_add(frame, ppref);
	g_free(pref_path);

	pref_path = db_current_pref_path("dbname");
	ppref = purple_plugin_pref_new_with_name_and_label(
		pref_path, _("DB name"));
	purple_plugin_pref_frame_add(frame, ppref);
	g_free(pref_path);

	/* XXX: maybe we should have list of possible choices */
	pref_path = db_current_pref_path("encoding");
	ppref = purple_plugin_pref_new_with_name_and_label(
		pref_path, _("encoding"));
	purple_plugin_pref_frame_add(frame, ppref);
	g_free(pref_path);

	return frame;
}

static PurplePluginUiInfo prefs_info = {
	get_plugin_pref_frame,
	0,   /* page_num (reserved) */
	NULL, /* frame (reserved) */

	/* padding */
	NULL,
	NULL,
	NULL,
	NULL
};

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

	"Michael Shkutkov <mish@rsu.ru>",             /**< author         */
	PURPLE_WEBSITE,                                     /**< homepage       */
	plugin_load,                                      /**< load           */
	plugin_unload,                                    /**< unload         */
	NULL,                                             /**< destroy        */
	NULL,                                             /**< ui_info        */
	NULL,                                             /**< extra_info     */
	&prefs_info,                                      /**< prefs_info     */
	NULL,                                             /**< actions        */

	/* padding */
	NULL,
	NULL,
	NULL,
	NULL
};

PURPLE_INIT_PLUGIN(database_logger, init_plugin, info)



void purple_debug_commit() {
	AsynchronousDebugEntry* entry;
	while( (entry = (AsynchronousDebugEntry*) g_async_queue_try_pop(async_debug_entries)) != NULL ) {
		purple_debug(entry->level,entry->category,"%s",entry->message);
		g_free(entry->category);
		g_free(entry->message);
		g_free(entry);
	}
}
void purple_debug_store_async(PurpleDebugLevel level, const char *category, const char *format, ...) {
	AsynchronousDebugEntry* entry = g_new(AsynchronousDebugEntry,1);
	entry->level = level;
	entry->category = g_strdup(category);
	va_list ap;
	va_start(ap, format);
	entry->message = g_strdup_vprintf(format, ap);
	va_end(ap);
	g_async_queue_push(async_debug_entries, entry);
}
