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
	PurpleLog * log;
	PurpleMessageFlags flags;
	const char *from;
	time_t time;
	char *message;
	PurpleLogSizeCallback cb;
	void *data;
	int ret_value;
} DatabaseWriteOperation;

typedef struct {
	DatabaseOperationType type;
	PurpleLog * log;
	PurpleLogReadFlags *flags;
	PurpleLogReadCallback cb;
	void *data;
	char *ret_value;
} DatabaseReadOperation;

typedef struct {
	DatabaseOperationType type;
	PurpleLogType log_type;
	char *name;
	PurpleAccount *account;
	PurpleLogListCallback cb;
	void *data;
	GList *ret_value;
} DatabaseListOperation;

typedef struct {
	DatabaseOperationType type;
	PurpleAccount *account;
	PurpleLogListCallback cb;
	void *data;
	GList *ret_value;
} DatabaseSyslogListOperation;

typedef struct {
	DatabaseOperationType type;
	PurpleLog *log;
	PurpleLogSizeCallback cb;
	void *data;
	int ret_value;
} DatabaseSizeOperation;

typedef struct {
	DatabaseOperationType type;
	PurpleLogType log_type;
	const char *name;
	PurpleAccount *account;
	PurpleLogSizeCallback cb;
	void *data;
	int ret_value;
} DatabaseTotalSizeOperation;

typedef struct {
	DatabaseOperationType type;
	GHashTable *sets;
	PurpleLogVoidCallback cb;
	void *data;
	GHashTable *ret_value;
} DatabaseSetsOperation;

typedef struct {
	DatabaseOperationType type;
	PurpleLog *log;
	PurpleLogBooleanCallback cb;
	void *data;
	gboolean ret_value;
} DatabaseRemoveLogOperation;

static DatabaseLoggerInfo *db_logger = NULL;

static gpointer (*db_thread_func[DATABASE_OPERATION_COUNT])(gpointer data);
static void (*db_notify_func[DATABASE_OPERATION_COUNT])(gpointer data);

/* List of finished in thread operations, such as getting total size, reading or writing */
static GList *db_finished_op = NULL;
static GMutex *db_mutex = NULL;

static gpointer db_thread(gpointer data);

static void database_logger_write(PurpleLog *log, PurpleMessageFlags type,
							  const char *from, time_t time, char *message,
							  PurpleLogSizeCallback cb, void *data)
{
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

		g_thread_create(db_thread, op, FALSE, NULL);
	} else {
		cb(0, data);
	}
}

static void database_logger_read(PurpleLog *log, PurpleLogReadFlags *flags,
							PurpleLogReadCallback cb, void *data)
{
	if (db_thread_func[PURPLE_DATABASE_LOGGER_READ] != NULL) {
		DatabaseReadOperation *op = g_new(DatabaseReadOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_READ;
		op->log = log;
		op->flags = flags;
		op->cb = cb;
		op->data = data;
		op->ret_value = NULL;
		g_thread_create(db_thread, op, FALSE, NULL);
	} else {
		cb(NULL, flags, data);
	}
}

static void database_logger_list(PurpleLogType type, char *name, PurpleAccount *account, 
								PurpleLogListCallback cb, void *data)
{
	if (db_thread_func[PURPLE_DATABASE_LOGGER_LIST] != NULL) {
		DatabaseListOperation *op = g_new(DatabaseListOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_LIST;
		op->log_type = type;
		op->name = name;
		op->account = account;
		op->cb = cb;
		op->data = data;
		op->ret_value = NULL;
		g_thread_create(db_thread, op, FALSE, NULL);
	} else {
		cb(NULL, data);
	}
}

static void database_logger_list_syslog(PurpleAccount *account, PurpleLogListCallback cb, void *data)
{
	if (db_thread_func[PURPLE_DATABASE_LOGGER_LIST_SYSLOG] != NULL) {
		DatabaseSyslogListOperation *op = g_new(DatabaseSyslogListOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_LIST_SYSLOG;
		op->account = account;
		op->cb = cb;
		op->data = data;
		op->ret_value = NULL;
		g_thread_create(db_thread, op, FALSE, NULL);
	} else {
		cb(NULL, data);
	}
}

static void database_logger_size(PurpleLog *log, PurpleLogSizeCallback cb, void *data)
{
	if (db_thread_func[PURPLE_DATABASE_LOGGER_SIZE] != NULL) {
		DatabaseSizeOperation *op = g_new(DatabaseSizeOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_SIZE;
		op->log = log;
		op->cb = cb;
		op->data = data;
		op->ret_value = 0;
		g_thread_create(db_thread, op, FALSE, NULL);
	} else {
		cb(0, data);
	}
}

static void database_logger_total_size(PurpleLogType type, const char *name, PurpleAccount *account,
									PurpleLogSizeCallback cb, void *data)
{
	if (db_thread_func[PURPLE_DATABASE_LOGGER_TOTAL_SIZE] != NULL) {
		DatabaseTotalSizeOperation *op = g_new(DatabaseTotalSizeOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_TOTAL_SIZE;
		op->log_type = type;
		op->name = name;
		op->account = account;
		op->cb = cb;
		op->data = data;
		op->ret_value = 0;
		g_thread_create(db_thread, op, FALSE, NULL);
	} else {
		cb(0, data);
	}
}

static void database_logger_sets(GHashTable *sets, PurpleLogVoidCallback cb, void *data)
{
	if (db_thread_func[PURPLE_DATABASE_LOGGER_SETS] != NULL) {
		DatabaseSetsOperation *op = g_new(DatabaseSetsOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_SETS;
		op->sets = sets;
		op->cb = cb;
		op->data = data;
		op->ret_value = sets;
		g_thread_create(db_thread, op, FALSE, NULL);
	} else {
		cb(data);
	}
}

static void database_logger_remove_log(PurpleLog *log, PurpleLogBooleanCallback cb, void *data)
{
	if (db_thread_func[PURPLE_DATABASE_LOGGER_REMOVE_LOG] != NULL) {
		DatabaseRemoveLogOperation *op = g_new(DatabaseRemoveLogOperation, 1);
		op->type = PURPLE_DATABASE_LOGGER_REMOVE_LOG;
		op->log = log;
		op->cb = cb;
		op->data = data;
		op->ret_value = 0;
		g_thread_create(db_thread, op, FALSE, NULL);
	} else {
		cb(FALSE, data);
	}
}

/****************************************************************************
* Notify operation which let you know that datbase operation has finished.
****************************************************************************/

static void db_write_notify(gpointer data) 
{
	DatabaseWriteOperation *op = data;

	if (op->cb != NULL) {
		op->cb(op->ret_value, op->data);
	}
}

static void db_read_notify(gpointer data) 
{
	DatabaseReadOperation *op = data;

	if (op->cb != NULL) {
		op->cb(op->ret_value, op->flags, op->data);
	}
}

static void db_list_notify(gpointer data) 
{
	DatabaseListOperation *op = data;
	if (op->cb != NULL) {
		op->cb(op->ret_value, op->data);
	}
}

static void db_syslog_list_notify(gpointer data) 
{
	DatabaseSyslogListOperation *op = data;
	if (op->cb != NULL) {
		op->cb(op->ret_value, op->data);
	}
}

static void db_size_notify(gpointer data) 
{
	DatabaseSizeOperation *op = data;
	if (op->cb != NULL) {
		op->cb(op->ret_value, op->data);
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
		op->cb(op->ret_value, op->data);
	}
}

static void db_remove_notify(gpointer data) 
{
	DatabaseRemoveLogOperation *op = data;
	if (op->cb != NULL) {
		op->cb(op->ret_value, op->data);
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

static void db_print_error() 
{
	const char *message;
	dbi_conn_error(db_logger->db_conn, &message);
	purple_debug_info("Database Logger", "ERROR: %s\n", message);
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

static gboolean db_retrieve_int_value(dbi_result dres, int *value, const char *field_name) 
{
	if (dres) {
		/* TODO: check if there are several rows */
		while (dbi_result_next_row(dres)) {
			*value = dbi_result_get_uint(dres, field_name);
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

	if (!prpl)
		return id;

	prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(prpl);
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
	char *name;
	dbi_result dres =  dbi_conn_queryf(db_logger->db_conn, 
						"SELECT `name` FROM protocols WHERE id = %i", id);
	db_retrieve_string_value(dres, &name, "name");
	return name;
}

static int db_get_account_id(PurpleAccount *account) 
{
	char *account_name = g_strdup(purple_escape_filename(purple_normalize(account,
				purple_account_get_username(account))));
	int id = -1;

	/* TODO: optimization */

	/* we can use dbi_conn_sequence_last to get row ID generated by the last INSERT command
	    when we add an account, but it seems that the result depends on database */
	while (id == -1) {
		dbi_result dres = dbi_conn_queryf(db_logger->db_conn, "SELECT id FROM accounts WHERE name = \"%s\"", account_name);

		if (!db_retrieve_id(dres, &id)) 
			break;

		/*if id not found we should insert new account */
		if (id == -1) {
			dres = dbi_conn_queryf(db_logger->db_conn, "INSERT INTO accounts(`name`, `protocolId`) VALUES(\"%s\", %i)", 
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

	dres = dbi_conn_queryf(db_logger->db_conn, "SELECT `name`, `protocolId` FROM accounts WHERE id =  %i", 
								id);
	if(dres) {
		int protocol_id = -1;
		
		while (dbi_result_next_row(dres)) {
			if (account_name)
				g_free(account_name);
			account_name = g_strdup(dbi_result_get_string(dres, "name"));
			protocol_id = dbi_result_get_int(dres, "protocolId");
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
		dbi_result dres = dbi_conn_queryf(db_logger->db_conn, "SELECT id FROM buddies WHERE name = \"%s\" AND type =%i AND accountID = %i ", 
									buddy_name, buddy_type, db_get_account_id(account));
		if (!db_retrieve_id(dres, &id)) 
			break;

		/*if id not found we should insert new account */
		if (id == -1) {
			dres = dbi_conn_queryf(db_logger->db_conn, "INSERT INTO buddies(`name`, `type`, `accountId`) VALUES(\"%s\", %i, %i)", 
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
	dbi_result dres = dbi_conn_queryf(db_logger->db_conn, "SELECT size FROM conversations WHERE id=%i", id);
	db_retrieve_int_value(dres, &ret_value, "size");
	return ret_value;
}

static void append_message_to_output(const char *ownerName, const char *message, int message_flags, 
								time_t datetime, PurpleLog *log, char **output)
{
	/* txt logger formatting */
	char *str = NULL;
	char *concat_string = NULL;
	char *date = log_get_timestamp(log, datetime);
	/* need get log type from DB */
	if(log->type == PURPLE_LOG_SYSTEM){
		str = g_strdup_printf("---- %s @ %s ----\n", message, date);
	} else {
		if (message_flags & PURPLE_MESSAGE_SEND ||
			message_flags & PURPLE_MESSAGE_RECV) {
			if (message_flags & PURPLE_MESSAGE_AUTO_RESP) {
				str = g_strdup_printf(_("(%s) %s <AUTO-REPLY>: %s\n"), date,
						ownerName, message);
			} else {
				char *duplicate = g_strdup(message);
				if(purple_message_meify(duplicate, -1))
					str = g_strdup_printf("(%s) ***%s %s\n", date, ownerName,
							duplicate);
				else
					str = g_strdup_printf("(%s) %s: %s\n", date, ownerName,
							duplicate);
				g_free(duplicate);
			}
		} else if (message_flags & PURPLE_MESSAGE_SYSTEM ||
			message_flags & PURPLE_MESSAGE_ERROR ||
			message_flags & PURPLE_MESSAGE_RAW)
			str = g_strdup_printf("(%s) %s\n", date, message);
		else if (message_flags & PURPLE_MESSAGE_NO_LOG) {
			/* This shouldn't happen */
			/* doing nothing */
		} else if (message_flags & PURPLE_MESSAGE_WHISPER)
			str = g_strdup_printf("(%s) *%s* %s\n", date, ownerName, message);
		else
			str = g_strdup_printf("(%s) %s%s %s\n", date, ownerName ? ownerName : "",
					ownerName ? ":" : "", message);
	}
	concat_string =  (*output) ? g_strconcat(*output, str, NULL) : g_strdup(str);
	g_free(*output);
	g_free(date);
	g_free(str);

	*output = concat_string;
}

static GList *get_list_log(PurpleLogType type, const char *name, PurpleAccount *account)
{
	dbi_result dres;
	GList *list = NULL;

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
			id = dbi_result_get_int(dres, "id");
			datetime = dbi_result_get_uint(dres, "datetime");

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
 */
static gpointer db_init(gpointer data) 
{
	if (dbi_conn_connect(db_logger->db_conn) < 0) {
		purple_debug_info("Database Logger", "Could not connect. Please check the option settings\n");
	} else {

		/* TODO: check where table has proper infrastructure */

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

		purple_debug_info("Database Logger", "Mutex UNlocked in db_init\n");
		unlock();

		purple_log_logger_add(db_logger->logger);
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

	lock();

	purple_debug_info("Database Logger", "---- XXX ---- db_write: from = %s; message = %s\n", from, message);
	op->ret_value = 0;

	if (log->logger_data == NULL) {
		purple_debug_info("Database Logger", "Init new conversation [new thread]\n");
		log->logger_data = conv_info = g_new(ConversationInfo, 1);
		conv_info->id = -1;

		/* create new conversation in log */
		dres = dbi_conn_queryf(db_logger->db_conn, 
			"INSERT INTO conversations(`datetime`, `size`, `accountId`, `buddyId`) VALUES(%i, %i, %i, %i )", 
			time, 0, db_get_account_id(log->account), db_get_buddy_id(log->type, log->name, log->account));
		db_process_result(dres);

		/* we can use dbi_conn_sequence_last to get row ID generated by the last INSERT command
		when we add an account, but it seems that the result depends on database, because
		some databases need exlicit sequences*/
		dres = dbi_conn_queryf(db_logger->db_conn, 
			"SELECT id FROM conversations WHERE datetime = %i AND accountID = %i AND buddyId = %i", 
									time, db_get_account_id(log->account), db_get_buddy_id(log->type, log->name, log->account));
		db_retrieve_id(dres, &conv_info->id);
	}

	if (conv_info->id != -1) {
		char *stripped = purple_markup_strip_html(message);
		int log_size = db_get_conversation_size(conv_info->id);
		purple_debug_info("Database Logger", "insert new message[new thread]\n");

		dres = dbi_conn_queryf(db_logger->db_conn, 
				"INSERT INTO messages (`conversationId`, `ownerName`, `datetime`, `text`, `flags`) VALUES(%i, \"%s\", %i, \"%s\", %i)",
				conv_info->id, from, time, stripped, flags);
		db_process_result(dres);

		/* updating log size */
		log_size += strlen(stripped);
		dres = dbi_conn_queryf(db_logger->db_conn,
				"UPDATE conversations SET size=%i WHERE id=%i",
				log_size, conv_info->id);
		g_free(stripped);
	} else 
		purple_debug_info("Database Logger", "conv_info->id == -1\n");

	unlock();

	return NULL;
}

static gpointer db_read(gpointer data)
{
	DatabaseReadOperation *op = data;
	PurpleLog *log = op->log;
	PurpleLogReadFlags *flags = op->flags;
	ConversationInfo *conv_info = log->logger_data;
	dbi_result dres;

	lock();

	if (flags)
		*flags = 0;

	if (!conv_info || conv_info->id == -1)
		op->ret_value = g_strdup(_("<font color=\"red\"><b>Unable to find conversation id!</b></font>"));
	else {
		dres = dbi_conn_queryf(db_logger->db_conn, 
				"SELECT `ownerName`, `datetime`, `text`, `flags` FROM `messages` WHERE conversationId=%i ORDER BY `datetime`",
				conv_info->id);
		if (dres) {
			int message_flags;
			time_t datetime;
			const char *ownerName;
			const char *message;

			while(dbi_result_next_row(dres)) {
				ownerName = dbi_result_get_string(dres, "ownerName");
				datetime = dbi_result_get_uint(dres, "datetime");
				message = dbi_result_get_string(dres, "text");
				message_flags = dbi_result_get_int(dres, "flags");
				/* we can form output as we wish */
				append_message_to_output(ownerName, message, message_flags, datetime, log, &op->ret_value);
			}
		}
		db_process_result(dres);
	}

	unlock();

	return NULL;
}


static gpointer db_list(gpointer data)
{
	DatabaseListOperation *op = data;

	purple_debug_info("Database Logger", "---- XXX ---- db_list: account = %s , name = %s\n", purple_account_get_username(op->account), op->name);

	lock();
	op->ret_value = get_list_log(op->log_type, op->name, op->account);
	unlock();

	return NULL;
}

static gpointer db_syslog_list(gpointer data)
{
	DatabaseSyslogListOperation *op = data;

	purple_debug_info("Database Logger", "---- XXX ---- db_syslog_list: account = %s\n", purple_account_get_username(op->account));

	lock();
	op->ret_value = get_list_log(PURPLE_LOG_SYSTEM, SYSTEM_LOG_BUDDY_NAME, op->account);
	unlock();

	return NULL;
}

static gpointer db_size(gpointer data)
{
	DatabaseSizeOperation *op = data;
	ConversationInfo *conv_info = op->log->logger_data;

	purple_debug_info("Database Logger", "---- XXX ---- db_size: account = %s\n", purple_account_get_username(op->log->account));
	lock();
	op->ret_value = 0;
	if (conv_info && conv_info->id != -1){
		dbi_result dres;

		dres = dbi_conn_queryf(db_logger->db_conn, "SELECT `size` FROM `conversations` WHERE conversationId=%i",
							conv_info->id);
		db_retrieve_int_value(dres, &op->ret_value, "size") ;
	}
	unlock();

	return NULL;
}

static gpointer db_total_size(gpointer data)
{
	DatabaseTotalSizeOperation *op = data;
	dbi_result dres;

	purple_debug_info("Database Logger", "---- XXX ---- db_total_size: account = %s\n", purple_account_get_username(op->account));

	lock();

	op->ret_value = 0;
	dres = dbi_conn_queryf(db_logger->db_conn, "SELECT SUM(size) FROM conversations WHERE accountId = %i AND buddyId = %i",
							db_get_account_id(op->account), db_get_buddy_id(op->log_type, op->name, op->account));
							
	if (dres) {
		char *string = NULL;
		/* TODO: check if there are several rows */
		while (dbi_result_next_row(dres)) {
			string = dbi_result_get_binary(dres, "SUM(size)");
			op->ret_value = atoi(string);
		}
	}
	db_process_result(dres);

	unlock();

	return NULL;
}

static gpointer db_sets(gpointer data)
{
	DatabaseSetsOperation *op = data;
	dbi_result dres;

	purple_debug_info("Database Logger", "---- XXX ---- db_sets\n");

	dres = dbi_conn_queryf(db_logger->db_conn, "SELECT `name`, `type`, `accountId` FROM `conversations`");
	if (dres) {
		while(dbi_result_next_row(dres)) {
			const char *buddy_name;
			int type;
			int account_id = -1;
			PurpleAccount *account = NULL;
			PurpleLogSet *set;

			buddy_name = dbi_result_get_string(dres, "name");
			type = dbi_result_get_uint(dres, "type");
			account_id = dbi_result_get_int(dres, "accountId");

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
				log_add_log_set_to_hash(op->ret_value, set);
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

	purple_debug_info("Database Logger", "---- XXX ---- db_remove: account = %s\n", purple_account_get_username(op->log->account));

	lock();
	op->ret_value = FALSE;
	if (conv_info && conv_info->id != -1){
		dbi_result dres;

		dres = dbi_conn_queryf(db_logger->db_conn, "DELETE FROM `messages` WHERE conversationId = %i",
							conv_info->id);
		if (db_process_result(dres)) {
			dres = dbi_conn_queryf(db_logger->db_conn, "DELETE FROM `conversations` WHERE id = %i",
								conv_info->id);
			db_process_result(dres);
			op->ret_value = TRUE;
		}
	}
	unlock();
	return NULL;
}
/**
 * Thread function, from which we do all operation with DB
 */
static gpointer db_thread(gpointer data) 
{
	DatabaseOperation *op = data;
	gpointer return_val = NULL;

	int id = op->type;

	purple_debug_info("Database Logger", "db_thread started: function id = %i\n", id);
	return_val = db_thread_func[id](data);

	/* Locking mutex, because we are going to add new 
	   item to db_funished_op 
	 */
	lock();
	db_finished_op = g_list_append(db_finished_op, op);
	unlock();

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

	purple_debug_info("Database Logger", "Main callback: there is finished operation\n");

	lock();
	for(; db_finished_op != NULL; db_finished_op = g_list_delete_link(db_finished_op, db_finished_op)) {
		DatabaseOperation *op = db_finished_op->data;
		if (db_notify_func[op->type] != NULL)
			db_notify_func[op->type](op);
		g_free(op);
	}
	unlock();

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

	db_logger->db_callback_id = purple_timeout_add(100, db_main_callback, NULL);

	purple_debug_info("Database Logger", "Creating thread and making init operation\n");
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
	db_notify_func[PURPLE_DATABASE_LOGGER_INIT] = NULL;

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

	"Michael Shkutkov <mish@rsu.ru>",             /**< author         */
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
