/**
 * @file log.c Logging API
 * @ingroup core
 */

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

#include "internal.h"
#include "account.h"
#include "dbus-maybe.h"
#include "debug.h"
#include "internal.h"
#include "log.h"
#include "prefs.h"
#include "util.h"
#include "stringref.h"
#include "imgstore.h"

static GSList *loggers = NULL;

static PurpleLogLogger *html_logger;
static PurpleLogLogger *txt_logger;
static PurpleLogLogger *old_logger;

typedef struct {
	PurpleLogContext *context;
	PurpleLogFreeCallback cb;
} _purple_log_free_callback_data;

typedef struct {
	PurpleLogContext *context;
	PurpleLogWriteCallback cb;
	struct _purple_logsize_user *lu;
} _purple_log_write_callback_data;

typedef struct {
	PurpleLogContext *context;
	PurpleLogReadCallback cb;
	PurpleLogReadFlags *flags;
} _purple_log_read_callback_data;

typedef struct {
	PurpleLogContext *context;
	PurpleLogSizeCallback cb;
} _purple_log_size_callback_data;

typedef struct {
	PurpleLogContext *context;
	PurpleLogTotalSizeCallback cb;
	struct _purple_logsize_user *lu;
	int counter;
	int ret_int;
} _purple_log_total_size_callback_data;

typedef struct {
	PurpleLogContext *context;
	PurpleLogRemoveLogCallback cb;
} _purple_log_delete_callback_data;

typedef struct {
	PurpleLogContext *context;
	PurpleLogListCallback cb;
	int counter;
} _purple_log_logs_callback_data;

typedef struct {
	PurpleLogContext *context;
	PurpleLogHashTableCallback cb;
	int counter;
	GHashTable *ret_sets;
} _purple_log_sets_callback_data;

typedef struct {
	PurpleLogContext *context;
	PurpleLogSystemListCallback cb;
	int counter;
} _purple_log_system_logs_callback_data;

struct _purple_logsize_user {
	char *name;
	PurpleAccount *account;
};

typedef void (*PurpleLogVoidCallback) (PurpleLogContext *context);

static GHashTable *logsize_users = NULL;

static void log_get_log_sets_common(GHashTable *sets);
static void log_get_log_sets_common_nonblocking(GHashTable *sets, PurpleLogLoggerHashTableCallback cb, void *data);

static gsize html_logger_write(PurpleLog *log, PurpleMessageFlags type,
							  const char *from, time_t time, const char *message);
static void html_logger_finalize(PurpleLog *log);
static GList *html_logger_list(PurpleLogType type, const char *sn, PurpleAccount *account);
static GList *html_logger_list_syslog(PurpleAccount *account);
static char *html_logger_read(PurpleLog *log, PurpleLogReadFlags *flags);
static int html_logger_total_size(PurpleLogType type, const char *name, PurpleAccount *account);

static GList *old_logger_list(PurpleLogType type, const char *sn, PurpleAccount *account);
static int old_logger_total_size(PurpleLogType type, const char *name, PurpleAccount *account);
static char * old_logger_read (PurpleLog *log, PurpleLogReadFlags *flags);
static int old_logger_size (PurpleLog *log);
static void old_logger_get_log_sets(PurpleLogSetCallback cb, GHashTable *sets);
static void old_logger_finalize(PurpleLog *log);

static gsize txt_logger_write(PurpleLog *log,
							 PurpleMessageFlags type,
							 const char *from, time_t time, const char *message);
static void txt_logger_finalize(PurpleLog *log);
static GList *txt_logger_list(PurpleLogType type, const char *sn, PurpleAccount *account);
static GList *txt_logger_list_syslog(PurpleAccount *account);
static char *txt_logger_read(PurpleLog *log, PurpleLogReadFlags *flags);
static int txt_logger_total_size(PurpleLogType type, const char *name, PurpleAccount *account);

static void log_free_cb(PurpleLog *log, void *data);
static void log_write_cb(int size, PurpleLog *log, PurpleMessageFlags flags, char *from, time_t time, char *message, void *data);
static void log_size_cb(int size, PurpleLog *log, void *data);
static void log_total_size_cb(int size, PurpleLogType type, char *name, PurpleAccount *account, void *data);
static void log_delete_cb(gboolean result, PurpleLog *log, void *data);
static void log_list_cb(GList *list, PurpleLogType type, char *name, PurpleAccount *account, void *data);
static void log_system_list_cb(GList *list, PurpleAccount *account, void *data);
static void log_read_cb(char *text, PurpleLog *log, PurpleLogReadFlags *flags, void *data);
static void log_hash_cb(void *data);

static void log_total_size_list_cb(GList *list, PurpleLogType type, char *name, PurpleAccount *account, void *data);

static void log_context_ref(PurpleLogContext *context);
static void log_context_unref(PurpleLogContext *context);
//static int log_context_count_ref(PurpleLogContext *context);
/**************************************************************************
 * PUBLIC LOGGING FUNCTIONS ***********************************************
 **************************************************************************/

PurpleLog *purple_log_new(PurpleLogType type, const char *name, PurpleAccount *account,
                      PurpleConversation *conv, time_t time, const struct tm *tm)
{
	PurpleLog *log;

	/* IMPORTANT: Make sure to initialize all the members of PurpleLog */
	log = g_slice_new(PurpleLog);
	PURPLE_DBUS_REGISTER_POINTER(log, PurpleLog);

	log->type = type;
	log->name = g_strdup(purple_normalize(account, name));
	log->account = account;
	log->conv = conv;
	log->time = time;
	log->logger = purple_log_logger_get();
	log->logger_data = NULL;

	if (tm == NULL)
		log->tm = NULL;
	else
	{
		/* There's no need to zero this as we immediately do a direct copy. */
		log->tm = g_slice_new(struct tm);

		*(log->tm) = *tm;

#ifdef HAVE_STRUCT_TM_TM_ZONE
		/* XXX: This is so wrong... */
		if (log->tm->tm_zone != NULL)
		{
			char *tmp = g_locale_from_utf8(log->tm->tm_zone, -1, NULL, NULL, NULL);
			if (tmp != NULL)
				log->tm->tm_zone = tmp;
			else
				/* Just shove the UTF-8 bytes in and hope... */
				log->tm->tm_zone = g_strdup(log->tm->tm_zone);
		}
#endif
	}

	if (log->logger && log->logger->create)
		log->logger->create(log);
	return log;
}

void purple_log_free(PurpleLog *log)
{
	g_return_if_fail(log != NULL);
	if (log->logger && log->logger->finalize)
		log->logger->finalize(log);
	g_free(log->name);

	if (log->tm != NULL)
	{
#ifdef HAVE_STRUCT_TM_TM_ZONE
		/* XXX: This is so wrong... */
		g_free((char *)log->tm->tm_zone);
#endif
		g_slice_free(struct tm, log->tm);
	}

	PURPLE_DBUS_UNREGISTER_POINTER(log);
	g_slice_free(PurpleLog, log);
}

void purple_log_free_nonblocking(PurpleLog *log, PurpleLogFreeCallback cb, PurpleLogContext *context)
{
	_purple_log_free_callback_data *callback_data;

	g_return_if_fail(log != NULL && log->logger != NULL);

	if (context != NULL && (purple_log_is_cancelled_operation(context) || purple_log_is_closed_context(context))) {
		purple_debug_info("Log", "purple_log_free_nonblocking is not completed due to cancelled operation or closed context\n");
		return;
	}

	if (context != NULL)
		log_context_ref(context);

	callback_data = g_new0(_purple_log_free_callback_data, 1);
	callback_data->context = context;
	callback_data->cb = cb;

	if (log->logger->finalize_nonblocking != NULL)
		log->logger->finalize_nonblocking(log, log_free_cb, callback_data); 
	else if (log->logger->finalize != NULL) {
		log->logger->finalize(log);
		log_free_cb(log, callback_data);
	}
}


void purple_log_write(PurpleLog *log, PurpleMessageFlags type,
		    const char *from, time_t time, const char *message)
{
	struct _purple_logsize_user *lu;
	gsize written, total = 0;
	gpointer ptrsize;

	g_return_if_fail(log != NULL);
	g_return_if_fail(log->logger != NULL);
	g_return_if_fail(log->logger->write != NULL);

	written = (log->logger->write)(log, type, from, time, message);

	lu = g_new(struct _purple_logsize_user, 1);

	lu->name = g_strdup(purple_normalize(log->account, log->name));
	lu->account = log->account;

	if(g_hash_table_lookup_extended(logsize_users, lu, NULL, &ptrsize)) {
		total = GPOINTER_TO_INT(ptrsize);
		total += written;
		g_hash_table_replace(logsize_users, lu, GINT_TO_POINTER(total));
		purple_debug_info("log", "HASH(purple_log_write): total size %i\n", total);
	} else {
		g_free(lu->name);
		g_free(lu);
	}

}

// TODO: Should this perhaps return a boolean indicating success or failure?
// TODO: Then the UI could display a failure message if, for example, a database
// TODO: logger failed to log a message mid-conversation.
void purple_log_write_nonblocking(PurpleLog *log, PurpleMessageFlags type,
								const char *from, time_t time, char *message,
								PurpleLogWriteCallback cb, PurpleLogContext *context)
{
	struct _purple_logsize_user *lu;
	_purple_log_write_callback_data *callback_data;
	char *from_copy;

	g_return_if_fail(log != NULL);
	g_return_if_fail(log->logger != NULL);
	g_return_if_fail(log->logger->write != NULL || log->logger->write_nonblocking != NULL);

	if (context != NULL && (purple_log_is_cancelled_operation(context) || purple_log_is_closed_context(context))) {
		purple_debug_info("Log", "purple_log_write_nonblocking is not completed due to cancelled operation or closed context\n");
		return;
	}

	lu = g_new(struct _purple_logsize_user, 1);

	lu->name = g_strdup(purple_normalize(log->account, log->name));
	lu->account = log->account;

	if (context != NULL)
		log_context_ref(context);

	callback_data = g_new0(_purple_log_write_callback_data, 1);
	callback_data->cb = cb;
	callback_data->context = context;
	callback_data->lu = lu;
	from_copy = g_strdup(from);

	if (log->logger->write_nonblocking) 
		(log->logger->write_nonblocking)(log, type, from_copy, time, message, log_write_cb, callback_data);
	else if (log->logger->write)
		log_write_cb((log->logger->write)(log, type, from_copy, time, message), 
					log, type, from_copy, time, message, callback_data);
}

char *purple_log_read(PurpleLog *log, PurpleLogReadFlags *flags)
{
	PurpleLogReadFlags mflags;
	g_return_val_if_fail(log != NULL, NULL);
	g_return_val_if_fail(log->logger != NULL, NULL);
	
	if (log->logger->read) {
		char *ret = (log->logger->read)(log, flags ? flags : &mflags);
		purple_str_strip_char(ret, '\r');
		return ret;
	}
	return g_strdup(_("<b><font color=\"red\">The logger has no read function</font></b>"));
}


void purple_log_read_nonblocking(PurpleLog *log, PurpleLogReadFlags *flags, PurpleLogReadCallback cb, PurpleLogContext *context)
{
	PurpleLogReadFlags mflags;
	_purple_log_read_callback_data *callback_data;

	if (context != NULL && (purple_log_is_cancelled_operation(context) || purple_log_is_closed_context(context))) {
		purple_debug_info("Log", "purple_log_read_nonblocking is not completed due to cancelled operation or closed context\n");
		return;
	}

	g_return_if_fail(log != NULL);
	g_return_if_fail(log->logger != NULL);

	if (context != NULL)
		log_context_ref(context);

	callback_data = g_new0(_purple_log_read_callback_data, 1);
	callback_data->context = context;
	callback_data->cb = cb;

	if (log->logger->read_nonblocking) 
		log->logger->read_nonblocking(log, flags ? flags : &mflags, log_read_cb, callback_data);
	else if (log->logger->read)
		log_read_cb((log->logger->read)(log, flags ? flags : &mflags), log, flags, callback_data);
	else
		log_read_cb(g_strdup(_("<b><font color=\"red\">The logger has no read function</font></b>")),  log, flags, callback_data);
}

int purple_log_get_size(PurpleLog *log)
{
	g_return_val_if_fail(log != NULL, 0);
	g_return_val_if_fail(log->logger != NULL, 0);

	if (log->logger->size)
		return log->logger->size(log);
	return 0;
}

void purple_log_get_size_nonblocking(PurpleLog *log, PurpleLogSizeCallback cb, PurpleLogContext *context)
{
	_purple_log_size_callback_data *callback_data;
	g_return_if_fail(log != NULL);
	g_return_if_fail(log->logger != NULL);

	if (context != NULL && (purple_log_is_cancelled_operation(context) || purple_log_is_closed_context(context))) {
		purple_debug_info("Log", "purple_log_get_size_nonblocking is not completed due to cancelled operation or closed context\n");
		return;
	}

	if (context != NULL)
		log_context_ref(context);

	callback_data = g_new0(_purple_log_size_callback_data, 1);
	callback_data->context = context;
	callback_data->cb = cb;

	if (log->logger->size_nonblocking)
		log->logger->size_nonblocking(log, log_size_cb, callback_data);
	else if (log->logger->size)
		log_size_cb(log->logger->size(log), log, callback_data);
	else
		log_size_cb(0, log, callback_data);
}

static guint _purple_logsize_user_hash(struct _purple_logsize_user *lu)
{
	return g_str_hash(lu->name);
}

static guint _purple_logsize_user_equal(struct _purple_logsize_user *lu1,
		struct _purple_logsize_user *lu2)
{
	return (lu1->account == lu2->account && (!strcmp(lu1->name, lu2->name)));
}

static void _purple_logsize_user_free_key(struct _purple_logsize_user *lu)
{
	g_free(lu->name);
	g_free(lu);
}

int purple_log_get_total_size(PurpleLogType type, const char *name, PurpleAccount *account)
{
	gpointer ptrsize;
	int size = 0;
	GSList *n;
	struct _purple_logsize_user *lu;

	lu = g_new(struct _purple_logsize_user, 1);
	lu->name = g_strdup(purple_normalize(account, name));
	lu->account = account;

	if(g_hash_table_lookup_extended(logsize_users, lu, NULL, &ptrsize)) {
		size = GPOINTER_TO_INT(ptrsize);
		purple_debug_info("log", "HASH(purple_log_get_total_size): using size from hash %i {name = \"%s\"\n}", size, name);
		g_free(lu->name);
		g_free(lu);
	} else {
		for (n = loggers; n; n = n->next) {
			PurpleLogLogger *logger = n->data;

			if(logger->total_size){
				size += (logger->total_size)(type, name, account);
			} else if(logger->list) {
				GList *logs = (logger->list)(type, name, account);
				int this_size = 0;

				while (logs) {
					PurpleLog *log = (PurpleLog*)(logs->data);
					this_size += purple_log_get_size(log);
					purple_log_free(log);
					logs = g_list_delete_link(logs, logs);
				}

				size += this_size;
			}
		}

		purple_debug_info("log", "HASH(purple_log_get_total_size): write size to hash %i {name = \"%s\"\n}", size, name);
		g_hash_table_replace(logsize_users, lu, GINT_TO_POINTER(size));
	}
	return size;
}

// TODO: Would it make sense to allow the caller to pass in a list of logs, if
// TODO: it just got them from purple_log_get_logs_nonblocking()?  Pidgin asks
// TODO: for the total size, which means that for some loggers, we end up
// TODO: calling list *again* needlessly (to loop over them and size them).
// TODO: If we had a list of logs, we could loop over them and find those
// TODO: where the logger had a size function (but no total_size function).
// TODO: We could size just those with something like this (this ignores
// TODO: the blocking vs. non-blocking distinction for simplicity):
// TODO: for (...) {
// TODO: 	if (!log->logger->total_size && log->logger->size)
// TODO: 		Call the size function.
void purple_log_get_total_size_nonblocking(PurpleLogType type, const char *name, PurpleAccount *account, 
								PurpleLogTotalSizeCallback cb, PurpleLogContext *context)
{
	gpointer ptrsize;
	int size = 0;
	GSList *n;
	struct _purple_logsize_user *lu;

	g_return_if_fail(loggers != NULL);

	if (context != NULL && (purple_log_is_cancelled_operation(context) || purple_log_is_closed_context(context))) {
		purple_debug_info("Log", "purple_log_get_total_size_nonblocking is not completed due to cancelled operation or closed context\n");
		return;
	}

	lu = g_new(struct _purple_logsize_user, 1);
	lu->name = g_strdup(purple_normalize(account, name));
	lu->account = account;

	if(g_hash_table_lookup_extended(logsize_users, lu, NULL, &ptrsize)) {
		size = GPOINTER_TO_INT(ptrsize);

		g_free(lu->name);
		g_free(lu);
		cb(size, type, name, account, context);
	} else {
		_purple_log_total_size_callback_data *callback_data;
		char *name_copy = g_strdup(name);
		if (context != NULL)
			log_context_ref(context);

		callback_data = g_new0(_purple_log_total_size_callback_data, 1);
		callback_data->context = context;
		callback_data->cb = cb;
		callback_data->lu = lu;
		callback_data->ret_int = 0;

		/* imho, this is really the best and simplest way 
		     especially now, because we have blocking total_size_nonblocking function 
		     and list_nonblocking functions*/
		callback_data->counter = g_slist_length(loggers);

		for (n = loggers; n; n = n->next) {
			PurpleLogLogger *logger = n->data;

			if(logger->total_size_nonblocking) {
				logger->total_size_nonblocking(type, name_copy, account, log_total_size_cb, callback_data);
			} else if(logger->list_nonblocking) {
				logger->list_nonblocking(type, name_copy, account, log_total_size_list_cb, callback_data);
			} else if(logger->total_size) { 
				/* As there is no any nonblocking functions we can call blocking analogs */
				log_total_size_cb((logger->total_size)(type, name_copy, account), 
								type, name_copy, account, callback_data);
			} else if(logger->list) {
				/* to prevent code duplication we reuse log_total_size_list_cb */
				GList *logs = (logger->list)(type, name_copy, account);
				log_total_size_list_cb(logs, type, name_copy, account, callback_data);
			} else 
				log_total_size_cb(0, type, name_copy, account, callback_data);
		}
	}
}

gboolean purple_log_is_deletable(PurpleLog *log)
{
	g_return_val_if_fail(log != NULL, FALSE);
	g_return_val_if_fail(log->logger != NULL, FALSE);

	if (log->logger->remove == NULL && log->logger->remove_nonblocking == NULL)
		return FALSE;

	if (log->logger->is_deletable != NULL)
		return log->logger->is_deletable(log);

	return TRUE;
}

gboolean purple_log_delete(PurpleLog *log)
{
	g_return_val_if_fail(log != NULL, FALSE);
	g_return_val_if_fail(log->logger != NULL, FALSE);

	if (log->logger->remove != NULL)
		return log->logger->remove(log);

	return FALSE;
}

void purple_log_delete_nonblocking(PurpleLog *log, PurpleLogRemoveLogCallback cb, PurpleLogContext *context)
{
	_purple_log_delete_callback_data *callback_data;
	gboolean result = FALSE;

	g_return_if_fail(log != NULL);
	g_return_if_fail(log->logger != NULL);

	if (context != NULL && (purple_log_is_cancelled_operation(context) || purple_log_is_closed_context(context))) {
		purple_debug_info("Log", "purple_log_delete_nonblocking is not completed due to cancelled operation or closed context\n");
		return;
	}

	callback_data = g_new(_purple_log_delete_callback_data, 1);
	callback_data->context = context;
	callback_data->cb = cb;

	if (log->logger->remove_nonblocking != NULL) 
		(log->logger->remove_nonblocking)(log, log_delete_cb, callback_data);
	else {
		/* As there is no nonblocking function we can call blocking analog */
		if (log->logger->remove != NULL) 
			result = (log->logger->remove)(log);
		log_delete_cb(result, log, callback_data);
	}
}

char *
purple_log_get_log_dir(PurpleLogType type, const char *name, PurpleAccount *account)
{
	PurplePlugin *prpl;
	PurplePluginProtocolInfo *prpl_info;
	const char *prpl_name;
	char *acct_name;
	const char *target;
	char *dir;

	prpl = purple_find_prpl(purple_account_get_protocol_id(account));
	if (!prpl)
		return NULL;
	prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(prpl);
	prpl_name = prpl_info->list_icon(account, NULL);

	acct_name = g_strdup(purple_escape_filename(purple_normalize(account,
				purple_account_get_username(account))));

	if (type == PURPLE_LOG_CHAT) {
		char *temp = g_strdup_printf("%s.chat", purple_normalize(account, name));
		target = purple_escape_filename(temp);
		g_free(temp);
	} else if(type == PURPLE_LOG_SYSTEM) {
		target = ".system";
	} else {
		target = purple_escape_filename(purple_normalize(account, name));
	}

	dir = g_build_filename(purple_user_dir(), "logs", prpl_name, acct_name, target, NULL);

	g_free(acct_name);

	return dir;
}

/****************************************************************************
 * LOGGER FUNCTIONS *********************************************************
 ****************************************************************************/

static PurpleLogLogger *current_logger = NULL;

static void logger_pref_cb(const char *name, PurplePrefType type,
						   gconstpointer value, gpointer data)
{
	PurpleLogLogger *logger;
	GSList *l = loggers;
	while (l) {
		logger = l->data;
		if (!strcmp(logger->id, value)) {
			purple_log_logger_set(logger);
			return;
		}
		l = l->next;
	}
	purple_log_logger_set(txt_logger);
}


PurpleLogLogger *purple_log_logger_new(const char *id, const char *name, int functions, ...)
{
#if 0
				void(*create)(PurpleLog *),
				gsize(*write)(PurpleLog *, PurpleMessageFlags, const char *, time_t, const char *),
				void(*finalize)(PurpleLog *),
				GList*(*list)(PurpleLogType type, const char*, PurpleAccount*),
				char*(*read)(PurpleLog*, PurpleLogReadFlags*),
				int(*size)(PurpleLog*),
				int(*total_size)(PurpleLogType type, const char *name, PurpleAccount *account),
				GList*(*list_syslog)(PurpleAccount *account),
				void(*get_log_sets)(PurpleLogSetCallback cb, GHashTable *sets),
				gboolean(*remove)(PurpleLog *log),
				gboolean(*is_deletable)(PurpleLog *log))
#endif
	PurpleLogLogger *logger;
	va_list args;

	g_return_val_if_fail(id != NULL, NULL);
	g_return_val_if_fail(name != NULL, NULL);
	g_return_val_if_fail(functions >= 1, NULL);

	logger = g_new0(PurpleLogLogger, 1);
	logger->id = g_strdup(id);
	logger->name = g_strdup(name);

	va_start(args, functions);

	if (functions >= 1)
		logger->create = va_arg(args, void *);
	if (functions >= 2)
		logger->write = va_arg(args, void *);
	if (functions >= 3)
		logger->finalize = va_arg(args, void *);
	if (functions >= 4)
		logger->list = va_arg(args, void *);
	if (functions >= 5)
		logger->read = va_arg(args, void *);
	if (functions >= 6)
		logger->size = va_arg(args, void *);
	if (functions >= 7)
		logger->total_size = va_arg(args, void *);
	if (functions >= 8)
		logger->list_syslog = va_arg(args, void *);
	if (functions >= 9)
		logger->get_log_sets = va_arg(args, void *);
	if (functions >= 10)
		logger->remove = va_arg(args, void *);
	if (functions >= 11)
		logger->is_deletable = va_arg(args, void *);

	/* callbacks functions */
	if (functions >= 12)
		logger->create_nonblocking = va_arg(args, void *);
	if (functions >= 13)
		logger->write_nonblocking = va_arg(args, void *);
	if (functions >= 14)
		logger->finalize_nonblocking = va_arg(args, void *);
	if (functions >= 15)
		logger->list_nonblocking = va_arg(args, void *);
	if (functions >= 16)
		logger->read_nonblocking = va_arg(args, void *);
	if (functions >= 17)
		logger->size_nonblocking = va_arg(args, void *);
	if (functions >= 18)
		logger->total_size_nonblocking = va_arg(args, void *);
	if (functions >= 19)
		logger->list_syslog_nonblocking = va_arg(args, void *);
	if (functions >= 20)
		logger->get_log_sets_nonblocking = va_arg(args, void *);
	if (functions >= 21)
		logger->remove_nonblocking = va_arg(args, void *);

	if (functions >= 22)
		purple_debug_info("log", "Dropping new functions for logger: %s (%s)\n", name, id);

	va_end(args);

	return logger;
}

void purple_log_logger_free(PurpleLogLogger *logger)
{
	g_free(logger->name);
	g_free(logger->id);
	g_free(logger);
}

void purple_log_logger_add (PurpleLogLogger *logger)
{
	g_return_if_fail(logger != NULL);
	if (g_slist_find(loggers, logger))
		return;
	loggers = g_slist_append(loggers, logger);
	if (strcmp(purple_prefs_get_string("/purple/logging/format"), logger->id) == 0) {
		purple_prefs_trigger_callback("/purple/logging/format");
	}
}

void purple_log_logger_remove (PurpleLogLogger *logger)
{
	g_return_if_fail(logger != NULL);
	loggers = g_slist_remove(loggers, logger);
}

void purple_log_logger_set (PurpleLogLogger *logger)
{
	g_return_if_fail(logger != NULL);
	current_logger = logger;
}

PurpleLogLogger *purple_log_logger_get()
{
	return current_logger;
}

GList *purple_log_logger_get_options(void)
{
	GSList *n;
	GList *list = NULL;
	PurpleLogLogger *data;

	for (n = loggers; n; n = n->next) {
		data = n->data;
		if (!data->write && !data->write_nonblocking)
			continue;
		list = g_list_append(list, data->name);
		list = g_list_append(list, data->id);
	}

	return list;
}

gint purple_log_compare(gconstpointer y, gconstpointer z)
{
	const PurpleLog *a = y;
	const PurpleLog *b = z;

	return b->time - a->time;
}

GList *purple_log_get_logs(PurpleLogType type, const char *name, PurpleAccount *account)
{
	GList *logs = NULL;
	GSList *n;
	for (n = loggers; n; n = n->next) {
		PurpleLogLogger *logger = n->data;
		if (logger->list)
			logs = g_list_concat(logger->list(type, name, account), logs);
	}

	return g_list_sort(logs, purple_log_compare);
}

void purple_log_get_logs_nonblocking(PurpleLogType type, const char *name, PurpleAccount *account, PurpleLogListCallback cb, PurpleLogContext *context)
{
	GSList *n;
	_purple_log_logs_callback_data *callback_data;
	char *name_copy;

	g_return_if_fail(loggers != NULL);

	if (context != NULL && (purple_log_is_cancelled_operation(context) || purple_log_is_closed_context(context))) {
		purple_debug_info("Log", "purple_log_get_logs_nonblocking is not completed due to cancelled operation or closed context\n");
		return;
	}
	if (context != NULL)
		log_context_ref(context);

	callback_data = g_new0(_purple_log_logs_callback_data, 1);
	callback_data->context = context;
	callback_data->cb = cb;
	name_copy = g_strdup(name);

	callback_data->counter = g_slist_length(loggers);

	for (n = loggers; n; n = n->next) {
		PurpleLogLogger *logger = n->data;

		if (logger->list_nonblocking) {
			logger->list_nonblocking(type, name_copy, account, log_list_cb, callback_data);
		} else if (logger->list) {
			/* Call the blocking list function instead. */
			GList *logs = logger->list(type, name_copy, account);
			log_list_cb(logs, type, name_copy, account, callback_data);
		} else 
			log_list_cb(NULL, type, name_copy, account, callback_data);
	}
}

gint purple_log_set_compare(gconstpointer y, gconstpointer z)
{
	const PurpleLogSet *a = y;
	const PurpleLogSet *b = z;
	gint ret = 0;

	/* This logic seems weird at first...
	 * If either account is NULL, we pretend the accounts are
	 * equal. This allows us to detect duplicates that will
	 * exist if one logger knows the account and another
	 * doesn't. */
	if (a->account != NULL && b->account != NULL) {
		ret = strcmp(purple_account_get_username(a->account), purple_account_get_username(b->account));
		if (ret != 0)
			return ret;
	}

	ret = strcmp(a->normalized_name, b->normalized_name);
	if (ret != 0)
		return ret;

	return (gint)b->type - (gint)a->type;
}

static guint
log_set_hash(gconstpointer key)
{
	const PurpleLogSet *set = key;

	/* The account isn't hashed because we need PurpleLogSets with NULL accounts
	 * to be found when we search by a PurpleLogSet that has a non-NULL account
	 * but the same type and name. */
	return g_int_hash(&set->type) + g_str_hash(set->name);
}

static gboolean
log_set_equal(gconstpointer a, gconstpointer b)
{
	/* I realize that the choices made for GList and GHashTable
	 * make sense for those data types, but I wish the comparison
	 * functions were compatible. */
	return !purple_log_set_compare(a, b);
}

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

GHashTable *purple_log_get_log_sets(void)
{
	GSList *n;
	GHashTable *sets = g_hash_table_new_full(log_set_hash, log_set_equal,
											 (GDestroyNotify)purple_log_set_free, NULL);

	/* Get the log sets from all the loggers. */
	for (n = loggers; n; n = n->next) {
		PurpleLogLogger *logger = n->data;

		if (!logger->get_log_sets)
			continue;

		logger->get_log_sets(log_add_log_set_to_hash, sets);
	}

	log_get_log_sets_common(sets);

	/* Return the GHashTable of unique PurpleLogSets. */
	return sets;
}

void purple_log_get_log_sets_nonblocking(PurpleLogHashTableCallback cb, PurpleLogContext *context)
{
	GSList *n;
	_purple_log_sets_callback_data *callback_data;

	g_return_if_fail(loggers != NULL);

	if (context != NULL && (purple_log_is_cancelled_operation(context) || purple_log_is_closed_context(context))) {
		purple_debug_info("Log", "purple_log_get_log_sets_nonblocking is not completed due to cancelled operation or closed context\n");
		return;
	}

	if (context != NULL)
		log_context_ref(context);

	callback_data = g_new0(_purple_log_sets_callback_data, 1);
	callback_data->context = context;
	callback_data->cb = cb;
	callback_data->ret_sets = g_hash_table_new_full(log_set_hash, log_set_equal,
											(GDestroyNotify)purple_log_set_free, NULL);

	/* +1 is need special for log_get_log_sets_common_nonblocking call */
	callback_data->counter = g_slist_length(loggers) + 1;
	
	/* Get the log sets from all the loggers. */
	for (n = loggers; n; n = n->next) {
		PurpleLogLogger *logger = n->data;

		if (logger->get_log_sets_nonblocking)
			logger->get_log_sets_nonblocking(log_add_log_set_to_hash, callback_data->ret_sets, log_hash_cb, callback_data);
		else {
			if (logger->get_log_sets)
				/* As there is no nonblocking  function we can call blocking analog*/
				logger->get_log_sets(log_add_log_set_to_hash, callback_data->ret_sets);

			log_hash_cb(callback_data);
		} 
	}

	log_get_log_sets_common_nonblocking(callback_data->ret_sets, log_hash_cb, callback_data);
}

void purple_log_set_free(PurpleLogSet *set)
{
	g_return_if_fail(set != NULL);

	g_free(set->name);
	if (set->normalized_name != set->name)
		g_free(set->normalized_name);

	g_slice_free(PurpleLogSet, set);
}

GList *purple_log_get_system_logs(PurpleAccount *account)
{
	GList *logs = NULL;
	GSList *n;
	for (n = loggers; n; n = n->next) {
		PurpleLogLogger *logger = n->data;
		if (!logger->list_syslog)
			continue;
		logs = g_list_concat(logger->list_syslog(account), logs);
	}

	return g_list_sort(logs, purple_log_compare);
}

void purple_log_get_system_logs_nonblocking(PurpleAccount *account, PurpleLogSystemListCallback cb, PurpleLogContext *context)
{
	GSList *n;
	_purple_log_system_logs_callback_data *callback_data;

	g_return_if_fail(loggers != NULL);

	if (context != NULL && (purple_log_is_cancelled_operation(context) || purple_log_is_closed_context(context))) {
		purple_debug_info("Log", "purple_log_get_log_sets_nonblocking is not completed due to cancelled operation or closed context\n");
		return;
	}

	if (context != NULL)
		log_context_ref(context);

	callback_data = g_new0(_purple_log_system_logs_callback_data, 1);
	callback_data->context = context;
	callback_data->cb = cb;
	callback_data->counter = g_slist_length(loggers);

	for (n = loggers; n; n = n->next) {
		PurpleLogLogger *logger = n->data;

		if (logger->list_syslog_nonblocking) {
			logger->list_syslog_nonblocking(account, log_system_list_cb, callback_data);
		} else if (logger->list_syslog) {
			/* Call the blocking list function instead. */
			GList *logs = logger->list_syslog(account);
			log_system_list_cb(logs, account, callback_data);
		} else 
			log_system_list_cb(NULL, account, callback_data);
	}
}

PurpleLogContext *purple_log_context_new(PurpleLogDestroyContextCallback cb)
{
	PurpleLogContext *context = g_new0(PurpleLogContext, 1);
	context->destroy_user_data_cb = cb;
	return context;
}

static int log_context_count_ref(PurpleLogContext *context)
{
	g_return_val_if_fail(context != NULL, 0);
	return context->ref_count;
}

static void log_context_free(PurpleLogContext *context)
{
	g_return_if_fail(context != NULL);
	if (context->destroy_user_data_cb != NULL)
		context->destroy_user_data_cb(context->user_data);
	g_free(context);
}

void purple_log_context_close(PurpleLogContext *context)
{
	g_return_if_fail(context != NULL);
	context->is_closed = TRUE;
	if (!log_context_count_ref(context))
		log_context_free(context);
}

gboolean purple_log_is_closed_context(PurpleLogContext *context)
{
	g_return_val_if_fail(context != NULL, TRUE);
	return context->is_closed;
}

void purple_log_context_set_userdata(PurpleLogContext *context, void *data)
{
	g_return_if_fail(context != NULL);
	/* XXX: what should we do if context->user_data is not NULL */
	context->user_data = data;
}

void *purple_log_context_get_userdata(PurpleLogContext *context)
{
	g_return_val_if_fail(context != NULL, NULL);
	return context->user_data;
}

void purple_log_cancel_operation(PurpleLogContext *context)
{
	g_return_if_fail(context);
	context->is_cancelled = TRUE;
}

gboolean purple_log_is_cancelled_operation(PurpleLogContext *context)
{
	g_return_val_if_fail(context != NULL, TRUE);
	return context->is_cancelled;
}

static void log_context_ref(PurpleLogContext *context)
{
	g_return_if_fail(context != NULL);
	context->ref_count++;
}

static void log_context_unref(PurpleLogContext *context)
{
	g_return_if_fail(context != NULL);
	context->ref_count--;
	if (!log_context_count_ref(context) && purple_log_is_closed_context(context)) 
		log_context_free(context);
}

/****************************************************************************
 * LOG SUBSYSTEM ************************************************************
 ****************************************************************************/

void *
purple_log_get_handle(void)
{
	static int handle;

	return &handle;
}

void purple_log_init(void)
{
	void *handle = purple_log_get_handle();

	purple_prefs_add_none("/purple/logging");
	purple_prefs_add_bool("/purple/logging/log_ims", FALSE);
	purple_prefs_add_bool("/purple/logging/log_chats", FALSE);
	purple_prefs_add_bool("/purple/logging/log_system", FALSE);

	purple_prefs_add_string("/purple/logging/format", "txt");

	html_logger = purple_log_logger_new("html", _("HTML"), 11,
									  NULL,
									  html_logger_write,
									  html_logger_finalize,
									  html_logger_list,
									  html_logger_read,
									  purple_log_common_sizer,
									  html_logger_total_size,
									  html_logger_list_syslog,
									  NULL,
									  purple_log_common_deleter,
									  purple_log_common_is_deletable);
	purple_log_logger_add(html_logger);

	txt_logger = purple_log_logger_new("txt", _("Plain text"), 11,
									 NULL,
									 txt_logger_write,
									 txt_logger_finalize,
									 txt_logger_list,
									 txt_logger_read,
									 purple_log_common_sizer,
									 txt_logger_total_size,
									 txt_logger_list_syslog,
									 NULL, 
									 purple_log_common_deleter,
									 purple_log_common_is_deletable);
	purple_log_logger_add(txt_logger);

	old_logger = purple_log_logger_new("old", _("Old flat format"), 9,
									 NULL,
									 NULL,
									 old_logger_finalize,
									 old_logger_list,
									 old_logger_read,
									 old_logger_size,
									 old_logger_total_size,
									 NULL,
									 old_logger_get_log_sets);
	purple_log_logger_add(old_logger);

	purple_signal_register(handle, "log-timestamp",
#if SIZEOF_TIME_T == 4
	                     purple_marshal_POINTER__POINTER_INT_BOOLEAN,
#elif SIZEOF_TIME_T == 8
			     purple_marshal_POINTER__POINTER_INT64_BOOLEAN,
#else
#error Unknown size of time_t
#endif
	                     purple_value_new(PURPLE_TYPE_STRING), 3,
	                     purple_value_new(PURPLE_TYPE_SUBTYPE,
	                                    PURPLE_SUBTYPE_LOG),
#if SIZEOF_TIME_T == 4
	                     purple_value_new(PURPLE_TYPE_INT),
#elif SIZEOF_TIME_T == 8
	                     purple_value_new(PURPLE_TYPE_INT64),
#else
# error Unknown size of time_t
#endif
	                     purple_value_new(PURPLE_TYPE_BOOLEAN));

	purple_prefs_connect_callback(NULL, "/purple/logging/format",
							    logger_pref_cb, NULL);
	purple_prefs_trigger_callback("/purple/logging/format");

	logsize_users = g_hash_table_new_full((GHashFunc)_purple_logsize_user_hash,
			(GEqualFunc)_purple_logsize_user_equal,
			(GDestroyNotify)_purple_logsize_user_free_key, NULL);
}

void
purple_log_uninit(void)
{
	purple_signals_unregister_by_instance(purple_log_get_handle());

	purple_log_logger_remove(html_logger);
	purple_log_logger_free(html_logger);
	html_logger = NULL;

	purple_log_logger_remove(txt_logger);
	purple_log_logger_free(txt_logger);
	txt_logger = NULL;

	purple_log_logger_remove(old_logger);
	purple_log_logger_free(old_logger);
	old_logger = NULL;
}

/****************************************************************************
 * LOGGERS ******************************************************************
 ****************************************************************************/

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

/* NOTE: This can return msg (which you may or may not want to g_free())
 * NOTE: or a newly allocated string which you MUST g_free(). */
static char *
convert_image_tags(const PurpleLog *log, const char *msg)
{
	const char *tmp;
	const char *start;
	const char *end;
	GData *attributes;
	GString *newmsg = NULL;

	tmp = msg;

	while (purple_markup_find_tag("img", tmp, &start, &end, &attributes)) {
		int imgid = 0;
		char *idstr = NULL;

		if (newmsg == NULL)
			newmsg = g_string_new("");

		/* copy any text before the img tag */
		if (tmp < start)
			g_string_append_len(newmsg, tmp, start - tmp);

		if ((idstr = g_datalist_get_data(&attributes, "id")) != NULL)
			imgid = atoi(idstr);

		if (imgid != 0)
		{
			FILE *image_file;
			char *dir;
			PurpleStoredImage *image;
			gconstpointer image_data;
			char *new_filename = NULL;
			char *path = NULL;
			size_t image_byte_count;

			image = purple_imgstore_find_by_id(imgid);
			if (image == NULL)
			{
				/* This should never happen. */
				/* This *does* happen for failed Direct-IMs -DAA */
				g_string_free(newmsg, TRUE);
				g_return_val_if_reached((char *)msg);
			}

			image_data       = purple_imgstore_get_data(image);
			image_byte_count = purple_imgstore_get_size(image);
			dir              = purple_log_get_log_dir(log->type, log->name, log->account);
			new_filename     = purple_util_get_image_filename(image_data, image_byte_count);

			path = g_build_filename(dir, new_filename, NULL);

			/* Only save unique files. */
			if (!g_file_test(path, G_FILE_TEST_EXISTS))
			{
				if ((image_file = g_fopen(path, "wb")) != NULL)
				{
					if (!fwrite(image_data, image_byte_count, 1, image_file))
					{
						purple_debug_error("log", "Error writing %s: %s\n",
						                   path, g_strerror(errno));
						fclose(image_file);

						/* Attempt to not leave half-written files around. */
						unlink(path);
					}
					else
					{
						purple_debug_info("log", "Wrote image file: %s\n", path);
						fclose(image_file);
					}
				}
				else
				{
					purple_debug_error("log", "Unable to create file %s: %s\n",
					                   path, g_strerror(errno));
				}
			}

			/* Write the new image tag */
			g_string_append_printf(newmsg, "<IMG SRC=\"%s\">", new_filename);
			g_free(new_filename);
			g_free(path);
		}

		/* Continue from the end of the tag */
		tmp = end + 1;
	}

	if (newmsg == NULL)
	{
		/* No images were found to change. */
		return (char *)msg;
	}

	/* Append any remaining message data */
	g_string_append(newmsg, tmp);

	return g_string_free(newmsg, FALSE);
}

void purple_log_common_writer(PurpleLog *log, const char *ext)
{
	PurpleLogCommonLoggerData *data = log->logger_data;

	if (data == NULL)
	{
		/* This log is new */
		char *dir;
		struct tm *tm;
		const char *tz;
		const char *date;
		char *filename;
		char *path;

		dir = purple_log_get_log_dir(log->type, log->name, log->account);
		if (dir == NULL)
			return;

		purple_build_dir (dir, S_IRUSR | S_IWUSR | S_IXUSR);

		tm = localtime(&log->time);
		tz = purple_escape_filename(purple_utf8_strftime("%Z", tm));
		date = purple_utf8_strftime("%Y-%m-%d.%H%M%S%z", tm);

		filename = g_strdup_printf("%s%s%s", date, tz, ext ? ext : "");

		path = g_build_filename(dir, filename, NULL);
		g_free(dir);
		g_free(filename);

		log->logger_data = data = g_slice_new0(PurpleLogCommonLoggerData);

		data->file = g_fopen(path, "a");
		if (data->file == NULL)
		{
			purple_debug(PURPLE_DEBUG_ERROR, "log",
					"Could not create log file %s\n", path);

			if (log->conv != NULL)
				purple_conversation_write(log->conv, NULL, _("Logging of this conversation failed."),
										PURPLE_MESSAGE_ERROR, time(NULL));

			g_free(path);
			return;
		}
		g_free(path);
	}
}

GList *purple_log_common_lister(PurpleLogType type, const char *name, PurpleAccount *account, const char *ext, PurpleLogLogger *logger)
{
	GDir *dir;
	GList *list = NULL;
	const char *filename;
	char *path;

	if(!account)
		return NULL;

	path = purple_log_get_log_dir(type, name, account);
	if (path == NULL)
		return NULL;

	if (!(dir = g_dir_open(path, 0, NULL)))
	{
		g_free(path);
		return NULL;
	}

	while ((filename = g_dir_read_name(dir)))
	{
		if (purple_str_has_suffix(filename, ext) &&
		    strlen(filename) >= (17 + strlen(ext)))
		{
			PurpleLog *log;
			PurpleLogCommonLoggerData *data;
			struct tm tm;
#if defined (HAVE_TM_GMTOFF) && defined (HAVE_STRUCT_TM_TM_ZONE)
			long tz_off;
			const char *rest, *end;
			time_t stamp = purple_str_to_time(purple_unescape_filename(filename), FALSE, &tm, &tz_off, &rest);

			/* As zero is a valid offset, PURPLE_NO_TZ_OFF means no offset was
			 * provided. See util.h. Yes, it's kinda ugly. */
			if (tz_off != PURPLE_NO_TZ_OFF)
				tm.tm_gmtoff = tz_off - tm.tm_gmtoff;

			if (stamp == 0 || rest == NULL || (end = strchr(rest, '.')) == NULL || strchr(rest, ' ') != NULL)
			{
				log = purple_log_new(type, name, account, NULL, stamp, NULL);
			}
			else
			{
				char *tmp = g_strndup(rest, end - rest);
				tm.tm_zone = tmp;
				log = purple_log_new(type, name, account, NULL, stamp, &tm);
				g_free(tmp);
			}
#else
			time_t stamp = purple_str_to_time(filename, FALSE, &tm, NULL, NULL);

			log = purple_log_new(type, name, account, NULL, stamp, (stamp != 0) ?  &tm : NULL);
#endif

			log->logger = logger;
			log->logger_data = data = g_slice_new0(PurpleLogCommonLoggerData);

			data->path = g_build_filename(path, filename, NULL);
			list = g_list_prepend(list, log);
		}
	}
	g_dir_close(dir);
	g_free(path);
	return list;
}

// TODO: Rather than calling this multiple times with different extensions,
// TODO: could we somehow store up all the extensions and do the loop just
// TODO: once?  This may be possible with the non-blocking stuff...
int purple_log_common_total_sizer(PurpleLogType type, const char *name, PurpleAccount *account, const char *ext)
{
	GDir *dir;
	int size = 0;
	const char *filename;
	char *path;

	if(!account)
		return 0;

	path = purple_log_get_log_dir(type, name, account);
	if (path == NULL)
		return 0;

	if (!(dir = g_dir_open(path, 0, NULL)))
	{
		g_free(path);
		return 0;
	}

	while ((filename = g_dir_read_name(dir)))
	{
		if (purple_str_has_suffix(filename, ext) &&
		    strlen(filename) >= (17 + strlen(ext)))
		{
			char *tmp = g_build_filename(path, filename, NULL);
			struct stat st;
			if (g_stat(tmp, &st))
			{
				purple_debug_error("log", "Error stating log file: %s\n", tmp);
				g_free(tmp);
				continue;
			}
			g_free(tmp);
			size += st.st_size;
		}
	}
	g_dir_close(dir);
	g_free(path);
	return size;
}

void purple_log_common_total_sizer_nonblocking(PurpleLogType type, const char *name, PurpleAccount *account, const char *ext, 
								PurpleLogTotalSizeCallback cb, PurpleLogContext *context)
{
	int size = purple_log_common_total_sizer(type, name, account, ext);
	cb(size, type, name, account, context);
}

int purple_log_common_sizer(PurpleLog *log)
{
	struct stat st;
	PurpleLogCommonLoggerData *data = log->logger_data;

	g_return_val_if_fail(data != NULL, 0);

	if (!data->path || g_stat(data->path, &st))
		st.st_size = 0;

	return st.st_size;
}

void purple_log_common_sizer_nonblocking(PurpleLog *log, PurpleLogSizeCallback cb, PurpleLogContext *context)
{
	int size = purple_log_common_sizer(log);
	cb(size, log, context);
}

/* This will build log sets for all loggers that use the common logger
 * functions because they use the same directory structure. */
static void log_get_log_sets_common(GHashTable *sets)
{
	gchar *log_path = g_build_filename(purple_user_dir(), "logs", NULL);
	GDir *log_dir = g_dir_open(log_path, 0, NULL);
	const gchar *protocol;

	if (log_dir == NULL) {
		g_free(log_path);
		return;
	}

	while ((protocol = g_dir_read_name(log_dir)) != NULL) {
		gchar *protocol_path = g_build_filename(log_path, protocol, NULL);
		GDir *protocol_dir;
		const gchar *username;
		gchar *protocol_unescaped;
		GList *account_iter;
		GList *accounts = NULL;

		if ((protocol_dir = g_dir_open(protocol_path, 0, NULL)) == NULL) {
			g_free(protocol_path);
			continue;
		}

		/* Using g_strdup() to cover the one-in-a-million chance that a
		 * prpl's list_icon function uses purple_unescape_filename(). */
		protocol_unescaped = g_strdup(purple_unescape_filename(protocol));

		/* Find all the accounts for protocol. */
		for (account_iter = purple_accounts_get_all() ; account_iter != NULL ; account_iter = account_iter->next) {
			PurplePlugin *prpl;
			PurplePluginProtocolInfo *prpl_info;

			prpl = purple_find_prpl(purple_account_get_protocol_id((PurpleAccount *)account_iter->data));
			if (!prpl)
				continue;
			prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(prpl);

			if (!strcmp(protocol_unescaped, prpl_info->list_icon((PurpleAccount *)account_iter->data, NULL)))
				accounts = g_list_prepend(accounts, account_iter->data);
		}
		g_free(protocol_unescaped);

		while ((username = g_dir_read_name(protocol_dir)) != NULL) {
			gchar *username_path = g_build_filename(protocol_path, username, NULL);
			GDir *username_dir;
			const gchar *username_unescaped;
			PurpleAccount *account = NULL;
			gchar *name;

			if ((username_dir = g_dir_open(username_path, 0, NULL)) == NULL) {
				g_free(username_path);
				continue;
			}

			/* Find the account for username in the list of accounts for protocol. */
			username_unescaped = purple_unescape_filename(username);
			for (account_iter = g_list_first(accounts) ; account_iter != NULL ; account_iter = account_iter->next) {
				if (!strcmp(((PurpleAccount *)account_iter->data)->username, username_unescaped)) {
					account = account_iter->data;
					break;
				}
			}

			/* Don't worry about the cast, name will point to dynamically allocated memory shortly. */
			while ((name = (gchar *)g_dir_read_name(username_dir)) != NULL) {
				size_t len;
				PurpleLogSet *set;

				/* IMPORTANT: Always initialize all members of PurpleLogSet */
				set = g_slice_new(PurpleLogSet);

				/* Unescape the filename. */
				name = g_strdup(purple_unescape_filename(name));

				/* Get the (possibly new) length of name. */
				len = strlen(name);

				set->type = PURPLE_LOG_IM;
				set->name = name;
				set->account = account;
				/* set->buddy is always set below */
				set->normalized_name = g_strdup(purple_normalize(account, name));

				/* Chat for .chat or .system at the end of the name to determine the type. */
				if (len >= 7) {
					gchar *tmp = &name[len - 7];
					if (!strcmp(tmp, ".system")) {
						set->type = PURPLE_LOG_SYSTEM;
						*tmp = '\0';
					}
				}
				if (len > 5) {
					gchar *tmp = &name[len - 5];
					if (!strcmp(tmp, ".chat")) {
						set->type = PURPLE_LOG_CHAT;
						*tmp = '\0';
					}
				}

				/* Determine if this (account, name) combination exists as a buddy. */
				if (account != NULL && name != NULL && *name != '\0')
					set->buddy = (purple_find_buddy(account, name) != NULL);
				else
					set->buddy = FALSE;

				log_add_log_set_to_hash(sets, set);
			}
			g_free(username_path);
			g_dir_close(username_dir);
		}
		g_free(protocol_path);
		g_dir_close(protocol_dir);
	}
	g_free(log_path);
	g_dir_close(log_dir);
}

static void log_get_log_sets_common_nonblocking(GHashTable *sets, PurpleLogLoggerHashTableCallback cb, void *data)
{
	log_get_log_sets_common(sets);
	if (cb != NULL) 
		cb(data);
}

gboolean purple_log_common_deleter(PurpleLog *log)
{
	PurpleLogCommonLoggerData *data;
	int ret;

	g_return_val_if_fail(log != NULL, FALSE);

	data = log->logger_data;
	if (data == NULL)
		return FALSE;

	if (data->path == NULL)
		return FALSE;

	ret = g_unlink(data->path);
	if (ret == 0)
		return TRUE;
	else if (ret == -1)
	{
		purple_debug_error("log", "Failed to delete: %s - %s\n", data->path, g_strerror(errno));
	}
	else
	{
		/* I'm not sure that g_unlink() will ever return
		 * something other than 0 or -1. -- rlaager */
		purple_debug_error("log", "Failed to delete: %s\n", data->path);
	}

	return FALSE;
}

gboolean purple_log_common_is_deletable(PurpleLog *log)
{
	PurpleLogCommonLoggerData *data;
#ifndef _WIN32
	gchar *dirname;
#endif

	g_return_val_if_fail(log != NULL, FALSE);

	data = log->logger_data;
	if (data == NULL)
		return FALSE;

	if (data->path == NULL)
		return FALSE;

#ifndef _WIN32
	dirname = g_path_get_dirname(data->path);
	if (g_access(dirname, W_OK) == 0)
	{
		g_free(dirname);
		return TRUE;
	}
	purple_debug_info("log", "access(%s) failed: %s\n", dirname, g_strerror(errno));
	g_free(dirname);
#else
	/* Unless and until someone writes equivalent win32 code,
	 * we'll assume the file is deletable. */
	return TRUE;
#endif

	return FALSE;
}

static char *process_txt_log(char *txt, char *to_free)
{
	char *tmp;

	/* The to_free argument allows us to save a
	 * g_strdup() in some cases. */

	if (to_free == NULL)
		to_free = txt;

	/* g_markup_escape_text requires valid UTF-8 */
	if (!g_utf8_validate(txt, -1, NULL))
	{
		tmp = purple_utf8_salvage(txt);
		g_free(to_free);
		to_free = txt = tmp;
	}

	tmp = g_markup_escape_text(txt, -1);
	g_free(to_free);
	txt = purple_markup_linkify(tmp);
	g_free(tmp);

	return txt;
}

#if 0 /* Maybe some other time. */
/****************
 ** XML LOGGER **
 ****************/

static const char *str_from_msg_type (PurpleMessageFlags type)
{

		return "";

}

static void xml_logger_write(PurpleLog *log,
			     PurpleMessageFlags type,
			     const char *from, time_t time, const char *message)
{
	char *xhtml = NULL;

	if (!log->logger_data) {
		/* This log is new.  We could use the loggers 'new' function, but
		 * creating a new file there would result in empty files in the case
		 * that you open a convo with someone, but don't say anything.
		 */
		struct tm *tm;
		const char *tz;
		const char *date;
		char *dir = purple_log_get_log_dir(log->type, log->name, log->account);
		char *name;
		char *filename;

		if (dir == NULL)
			return;

		tm = localtime(&log->time);
		tz = purple_escape_filename(purple_utf8_strftime("%Z", tm);
		date = purple_utf8_strftime("%Y-%m-%d.%H%M%S%z", tm);

		name = g_strdup_printf("%s%s%s", date, tz, ext ? ext : "");

		purple_build_dir (dir, S_IRUSR | S_IWUSR | S_IXUSR);

		filename = g_build_filename(dir, name, NULL);
		g_free(dir);
		g_free(name);

		log->logger_data = g_fopen(filename, "a");
		if (!log->logger_data) {
			purple_debug(PURPLE_DEBUG_ERROR, "log", "Could not create log file %s\n", filename);
			g_free(filename);
			return;
		}
		g_free(filename);
		fprintf(log->logger_data, "<?xml version='1.0' encoding='UTF-8' ?>\n"
			"<?xml-stylesheet href='file:///usr/src/web/htdocs/log-stylesheet.xsl' type='text/xml' ?>\n");

		date = purple_utf8_strftime("%Y-%m-%d %H:%M:%S", localtime(&log->time));
		fprintf(log->logger_data, "<conversation time='%s' screenname='%s' protocol='%s'>\n",
			date, log->name, prpl);
	}

	/* if we can't write to the file, give up before we hurt ourselves */
	if(!data->file)
		return;

	date = log_get_timestamp(log, time);

	purple_markup_html_to_xhtml(message, &xhtml, NULL);
	if (from)
		fprintf(log->logger_data, "<message %s %s from='%s' time='%s'>%s</message>\n",
			str_from_msg_type(type),
			type & PURPLE_MESSAGE_SEND ? "direction='sent'" :
			type & PURPLE_MESSAGE_RECV ? "direction='received'" : "",
			from, date, xhtml);
	else
		fprintf(log->logger_data, "<message %s %s time='%s'>%s</message>\n",
			str_from_msg_type(type),
			type & PURPLE_MESSAGE_SEND ? "direction='sent'" :
			type & PURPLE_MESSAGE_RECV ? "direction='received'" : "",
			date, xhtml):
	fflush(log->logger_data);
	g_free(date);
	g_free(xhtml);
}

 static void xml_logger_finalize(PurpleLog *log)
{
	if (log->logger_data) {
		fprintf(log->logger_data, "</conversation>\n");
		fclose(log->logger_data);
		log->logger_data = NULL;
	}
}

static GList *xml_logger_list(PurpleLogType type, const char *sn, PurpleAccount *account)
{
	return purple_log_common_lister(type, sn, account, ".xml", &xml_logger);
}

static PurpleLogLogger xml_logger =  {
	N_("XML"), "xml",
	NULL,
	xml_logger_write,
	xml_logger_finalize,
	xml_logger_list,
	NULL,
	NULL,
	NULL
};
#endif

/****************************
 ** HTML LOGGER *************
 ****************************/

static gsize html_logger_write(PurpleLog *log, PurpleMessageFlags type,
							  const char *from, time_t time, const char *message)
{
	char *msg_fixed;
	char *image_corrected_msg;
	char *date;
	char *header;
	PurplePlugin *plugin = purple_find_prpl(purple_account_get_protocol_id(log->account));
	PurpleLogCommonLoggerData *data = log->logger_data;
	gsize written = 0;

	if(!data) {
		const char *prpl =
			PURPLE_PLUGIN_PROTOCOL_INFO(plugin)->list_icon(log->account, NULL);
		const char *date;
		purple_log_common_writer(log, ".html");

		data = log->logger_data;

		/* if we can't write to the file, give up before we hurt ourselves */
		if(!data->file)
			return 0;

		date = purple_date_format_full(localtime(&log->time));

		written += fprintf(data->file, "<html><head>");
		written += fprintf(data->file, "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">");
		written += fprintf(data->file, "<title>");
		if (log->type == PURPLE_LOG_SYSTEM)
			header = g_strdup_printf("System log for account %s (%s) connected at %s",
					purple_account_get_username(log->account), prpl, date);
		else
			header = g_strdup_printf("Conversation with %s at %s on %s (%s)",
					log->name, date, purple_account_get_username(log->account), prpl);

		written += fprintf(data->file, "%s", header);
		written += fprintf(data->file, "</title></head><body>");
		written += fprintf(data->file, "<h3>%s</h3>\n", header);
		g_free(header);
	}

	/* if we can't write to the file, give up before we hurt ourselves */
	if(!data->file)
		return 0;

	image_corrected_msg = convert_image_tags(log, message);
	purple_markup_html_to_xhtml(image_corrected_msg, &msg_fixed, NULL);

	/* Yes, this breaks encapsulation.  But it's a static function and
	 * this saves a needless strdup(). */
	if (image_corrected_msg != message)
		g_free(image_corrected_msg);

	date = log_get_timestamp(log, time);

	if(log->type == PURPLE_LOG_SYSTEM){
		written += fprintf(data->file, "---- %s @ %s ----<br/>\n", msg_fixed, date);
	} else {
		if (type & PURPLE_MESSAGE_SYSTEM)
			written += fprintf(data->file, "<font size=\"2\">(%s)</font><b> %s</b><br/>\n", date, msg_fixed);
		else if (type & PURPLE_MESSAGE_RAW)
			written += fprintf(data->file, "<font size=\"2\">(%s)</font> %s<br/>\n", date, msg_fixed);
		else if (type & PURPLE_MESSAGE_ERROR)
			written += fprintf(data->file, "<font color=\"#FF0000\"><font size=\"2\">(%s)</font><b> %s</b></font><br/>\n", date, msg_fixed);
		else if (type & PURPLE_MESSAGE_WHISPER)
			written += fprintf(data->file, "<font color=\"#6C2585\"><font size=\"2\">(%s)</font><b> %s:</b></font> %s<br/>\n",
					date, from, msg_fixed);
		else if (type & PURPLE_MESSAGE_AUTO_RESP) {
			if (type & PURPLE_MESSAGE_SEND)
				written += fprintf(data->file, _("<font color=\"#16569E\"><font size=\"2\">(%s)</font> <b>%s &lt;AUTO-REPLY&gt;:</b></font> %s<br/>\n"), date, from, msg_fixed);
			else if (type & PURPLE_MESSAGE_RECV)
				written += fprintf(data->file, _("<font color=\"#A82F2F\"><font size=\"2\">(%s)</font> <b>%s &lt;AUTO-REPLY&gt;:</b></font> %s<br/>\n"), date, from, msg_fixed);
		} else if (type & PURPLE_MESSAGE_RECV) {
			if(purple_message_meify(msg_fixed, -1))
				written += fprintf(data->file, "<font color=\"#062585\"><font size=\"2\">(%s)</font> <b>***%s</b></font> %s<br/>\n",
						date, from, msg_fixed);
			else
				written += fprintf(data->file, "<font color=\"#A82F2F\"><font size=\"2\">(%s)</font> <b>%s:</b></font> %s<br/>\n",
						date, from, msg_fixed);
		} else if (type & PURPLE_MESSAGE_SEND) {
			if(purple_message_meify(msg_fixed, -1))
				written += fprintf(data->file, "<font color=\"#062585\"><font size=\"2\">(%s)</font> <b>***%s</b></font> %s<br/>\n",
						date, from, msg_fixed);
			else
				written += fprintf(data->file, "<font color=\"#16569E\"><font size=\"2\">(%s)</font> <b>%s:</b></font> %s<br/>\n",
						date, from, msg_fixed);
		} else {
			purple_debug_error("log", "Unhandled message type.\n");
			written += fprintf(data->file, "<font size=\"2\">(%s)</font><b> %s:</b></font> %s<br/>\n",
						date, from, msg_fixed);
		}
	}
	g_free(date);
	g_free(msg_fixed);
	fflush(data->file);

	return written;
}

static void html_logger_finalize(PurpleLog *log)
{
	PurpleLogCommonLoggerData *data = log->logger_data;
	if (data) {
		if(data->file) {
			fprintf(data->file, "</body></html>\n");
			fclose(data->file);
		}
		g_free(data->path);

		g_slice_free(PurpleLogCommonLoggerData, data);
	}
}

static GList *html_logger_list(PurpleLogType type, const char *sn, PurpleAccount *account)
{
	return purple_log_common_lister(type, sn, account, ".html", html_logger);
}

static GList *html_logger_list_syslog(PurpleAccount *account)
{
	return purple_log_common_lister(PURPLE_LOG_SYSTEM, ".system", account, ".html", html_logger);
}

static char *html_logger_read(PurpleLog *log, PurpleLogReadFlags *flags)
{
	char *read;
	PurpleLogCommonLoggerData *data = log->logger_data;
	*flags = PURPLE_LOG_READ_NO_NEWLINE;
	if (!data || !data->path)
		return g_strdup(_("<font color=\"red\"><b>Unable to find log path!</b></font>"));
	if (g_file_get_contents(data->path, &read, NULL, NULL)) {
		char *minus_header = strchr(read, '\n');

		if (!minus_header)
			return read;

		minus_header = g_strdup(minus_header + 1);
		g_free(read);

		return minus_header;
	}
	return g_strdup_printf(_("<font color=\"red\"><b>Could not read file: %s</b></font>"), data->path);
}

static int html_logger_total_size(PurpleLogType type, const char *name, PurpleAccount *account)
{
	return purple_log_common_total_sizer(type, name, account, ".html");
}

/****************************
 ** PLAIN TEXT LOGGER *******
 ****************************/

static gsize txt_logger_write(PurpleLog *log,
							 PurpleMessageFlags type,
							 const char *from, time_t time, const char *message)
{
	char *date;
	PurplePlugin *plugin = purple_find_prpl(purple_account_get_protocol_id(log->account));
	PurpleLogCommonLoggerData *data = log->logger_data;
	char *stripped = NULL;

	gsize written = 0;

	if (data == NULL) {
		/* This log is new.  We could use the loggers 'new' function, but
		 * creating a new file there would result in empty files in the case
		 * that you open a convo with someone, but don't say anything.
		 */
		const char *prpl =
			PURPLE_PLUGIN_PROTOCOL_INFO(plugin)->list_icon(log->account, NULL);
		purple_log_common_writer(log, ".txt");

		data = log->logger_data;

		/* if we can't write to the file, give up before we hurt ourselves */
		if(!data->file)
			return 0;

		if (log->type == PURPLE_LOG_SYSTEM)
			written += fprintf(data->file, "System log for account %s (%s) connected at %s\n",
				purple_account_get_username(log->account), prpl,
				purple_date_format_full(localtime(&log->time)));
		else
			written += fprintf(data->file, "Conversation with %s at %s on %s (%s)\n",
				log->name, purple_date_format_full(localtime(&log->time)),
				purple_account_get_username(log->account), prpl);
	}

	/* if we can't write to the file, give up before we hurt ourselves */
	if(!data->file)
		return 0;

	stripped = purple_markup_strip_html(message);
	date = log_get_timestamp(log, time);

	if(log->type == PURPLE_LOG_SYSTEM){
		written += fprintf(data->file, "---- %s @ %s ----\n", stripped, date);
	} else {
		if (type & PURPLE_MESSAGE_SEND ||
			type & PURPLE_MESSAGE_RECV) {
			if (type & PURPLE_MESSAGE_AUTO_RESP) {
				written += fprintf(data->file, _("(%s) %s <AUTO-REPLY>: %s\n"), date,
						from, stripped);
			} else {
				if(purple_message_meify(stripped, -1))
					written += fprintf(data->file, "(%s) ***%s %s\n", date, from,
							stripped);
				else
					written += fprintf(data->file, "(%s) %s: %s\n", date, from,
							stripped);
			}
		} else if (type & PURPLE_MESSAGE_SYSTEM ||
			type & PURPLE_MESSAGE_ERROR ||
			type & PURPLE_MESSAGE_RAW)
			written += fprintf(data->file, "(%s) %s\n", date, stripped);
		else if (type & PURPLE_MESSAGE_NO_LOG) {
			/* This shouldn't happen */
			g_free(stripped);
			return written;
		} else if (type & PURPLE_MESSAGE_WHISPER)
			written += fprintf(data->file, "(%s) *%s* %s", date, from, stripped);
		else
			written += fprintf(data->file, "(%s) %s%s %s\n", date, from ? from : "",
					from ? ":" : "", stripped);
	}
	g_free(date);
	g_free(stripped);
	fflush(data->file);

	return written;
}

static void txt_logger_finalize(PurpleLog *log)
{
	PurpleLogCommonLoggerData *data = log->logger_data;
	if (data) {
		if(data->file)
			fclose(data->file);
		g_free(data->path);

		g_slice_free(PurpleLogCommonLoggerData, data);
	}
}

static GList *txt_logger_list(PurpleLogType type, const char *sn, PurpleAccount *account)
{
	return purple_log_common_lister(type, sn, account, ".txt", txt_logger);
}

static GList *txt_logger_list_syslog(PurpleAccount *account)
{
	return purple_log_common_lister(PURPLE_LOG_SYSTEM, ".system", account, ".txt", txt_logger);
}

static char *txt_logger_read(PurpleLog *log, PurpleLogReadFlags *flags)
{
	char *read, *minus_header;
	PurpleLogCommonLoggerData *data = log->logger_data;
	*flags = 0;
	if (!data || !data->path)
		return g_strdup(_("<font color=\"red\"><b>Unable to find log path!</b></font>"));
	if (g_file_get_contents(data->path, &read, NULL, NULL)) {
		minus_header = strchr(read, '\n');

		if (minus_header)
			return process_txt_log(minus_header + 1, read);
		else
			return process_txt_log(read, NULL);
	}
	return g_strdup_printf(_("<font color=\"red\"><b>Could not read file: %s</b></font>"), data->path);
}

static int txt_logger_total_size(PurpleLogType type, const char *name, PurpleAccount *account)
{
	return purple_log_common_total_sizer(type, name, account, ".txt");
}

/****************
 * OLD LOGGER ***
 ****************/

/* The old logger doesn't write logs, only reads them.  This is to include
 * old logs in the log viewer transparently.
 */

struct old_logger_data {
	PurpleStringref *pathref;
	int offset;
	int length;
};

static GList *old_logger_list(PurpleLogType type, const char *sn, PurpleAccount *account)
{
	char *logfile = g_strdup_printf("%s.log", purple_normalize(account, sn));
	char *pathstr = g_build_filename(purple_user_dir(), "logs", logfile, NULL);
	PurpleStringref *pathref = purple_stringref_new(pathstr);
	struct stat st;
	time_t log_last_modified;
	FILE *index;
	FILE *file;
	int index_fd;
	char *index_tmp;
	char buf[BUF_LONG];
	struct tm tm;
	char month[4];
	struct old_logger_data *data = NULL;
	char *newlog;
	int logfound = 0;
	int lastoff = 0;
	int newlen;
	time_t lasttime = 0;

	PurpleLog *log = NULL;
	GList *list = NULL;

	g_free(logfile);

	if (g_stat(purple_stringref_value(pathref), &st))
	{
		purple_stringref_unref(pathref);
		g_free(pathstr);
		return NULL;
	}
	else
		log_last_modified = st.st_mtime;

	/* Change the .log extension to .idx */
	strcpy(pathstr + strlen(pathstr) - 3, "idx");

	if (g_stat(pathstr, &st) == 0)
	{
		if (st.st_mtime < log_last_modified)
		{
			purple_debug_warning("log", "Index \"%s\" exists, but is older than the log.\n", pathstr);
		}
		else
		{
			/* The index file exists and is at least as new as the log, so open it. */
			if (!(index = g_fopen(pathstr, "rb")))
			{
				purple_debug_error("log", "Failed to open index file \"%s\" for reading: %s\n",
				                 pathstr, g_strerror(errno));

				/* Fall through so that we'll parse the log file. */
			}
			else
			{
				purple_debug_info("log", "Using index: %s\n", pathstr);
				g_free(pathstr);
				while (fgets(buf, BUF_LONG, index))
				{
					unsigned long idx_time;
					if (sscanf(buf, "%d\t%d\t%lu", &lastoff, &newlen, &idx_time) == 3)
					{
						log = purple_log_new(PURPLE_LOG_IM, sn, account, NULL, -1, NULL);
						log->logger = old_logger;
						log->time = (time_t)idx_time;

						/* IMPORTANT: Always set all members of struct old_logger_data */
						data = g_slice_new(struct old_logger_data);

						data->pathref = purple_stringref_ref(pathref);
						data->offset = lastoff;
						data->length = newlen;

						log->logger_data = data;
						list = g_list_prepend(list, log);
					}
				}
				fclose(index);
				purple_stringref_unref(pathref);

				return list;
			}
		}
	}

	if (!(file = g_fopen(purple_stringref_value(pathref), "rb"))) {
		purple_debug_error("log", "Failed to open log file \"%s\" for reading: %s\n",
		                   purple_stringref_value(pathref), g_strerror(errno));
		purple_stringref_unref(pathref);
		g_free(pathstr);
		return NULL;
	}

	index_tmp = g_strdup_printf("%s.XXXXXX", pathstr);
	if ((index_fd = g_mkstemp(index_tmp)) == -1) {
		purple_debug_error("log", "Failed to open index temp file: %s\n",
		                 g_strerror(errno));
		g_free(pathstr);
		g_free(index_tmp);
		index = NULL;
	} else {
		if ((index = fdopen(index_fd, "wb")) == NULL)
		{
			purple_debug_error("log", "Failed to fdopen() index temp file: %s\n",
			                 g_strerror(errno));
			close(index_fd);
			if (index_tmp != NULL)
			{
				g_unlink(index_tmp);
				g_free(index_tmp);
			}
			g_free(pathstr);
		}
	}

	while (fgets(buf, BUF_LONG, file)) {
		if ((newlog = strstr(buf, "---- New C"))) {
			int length;
			int offset;
			char convostart[32];
			char *temp = strchr(buf, '@');

			if (temp == NULL || strlen(temp) < 2)
				continue;

			temp++;
			length = strcspn(temp, "-");
			if (length > 31) length = 31;

			offset = ftell(file);

			if (logfound) {
				newlen = offset - lastoff - length;
				if(strstr(buf, "----</H3><BR>")) {
					newlen -=
						sizeof("<HR><BR><H3 Align=Center> ---- New Conversation @ ") +
						sizeof("----</H3><BR>") - 2;
				} else {
					newlen -=
						sizeof("---- New Conversation @ ") + sizeof("----") - 2;
				}

				if(strchr(buf, '\r'))
					newlen--;

				if (newlen != 0) {
					log = purple_log_new(PURPLE_LOG_IM, sn, account, NULL, -1, NULL);
					log->logger = old_logger;
					log->time = lasttime;

					/* IMPORTANT: Always set all members of struct old_logger_data */
					data = g_slice_new(struct old_logger_data);

					data->pathref = purple_stringref_ref(pathref);
					data->offset = lastoff;
					data->length = newlen;

					log->logger_data = data;
					list = g_list_prepend(list, log);

					/* XXX: There is apparently Is there a proper way to print a time_t? */
					if (index != NULL)
						fprintf(index, "%d\t%d\t%lu\n", data->offset, data->length, (unsigned long)log->time);
				}
			}

			logfound = 1;
			lastoff = offset;

			g_snprintf(convostart, length, "%s", temp);
			memset(&tm, 0, sizeof(tm));
			sscanf(convostart, "%*s %s %d %d:%d:%d %d",
			       month, &tm.tm_mday, &tm.tm_hour, &tm.tm_min, &tm.tm_sec, &tm.tm_year);
			/* Ugly hack, in case current locale is not English */
			if (strcmp(month, "Jan") == 0) {
				tm.tm_mon= 0;
			} else if (strcmp(month, "Feb") == 0) {
				tm.tm_mon = 1;
			} else if (strcmp(month, "Mar") == 0) {
				tm.tm_mon = 2;
			} else if (strcmp(month, "Apr") == 0) {
				tm.tm_mon = 3;
			} else if (strcmp(month, "May") == 0) {
				tm.tm_mon = 4;
			} else if (strcmp(month, "Jun") == 0) {
				tm.tm_mon = 5;
			} else if (strcmp(month, "Jul") == 0) {
				tm.tm_mon = 6;
			} else if (strcmp(month, "Aug") == 0) {
				tm.tm_mon = 7;
			} else if (strcmp(month, "Sep") == 0) {
				tm.tm_mon = 8;
			} else if (strcmp(month, "Oct") == 0) {
				tm.tm_mon = 9;
			} else if (strcmp(month, "Nov") == 0) {
				tm.tm_mon = 10;
			} else if (strcmp(month, "Dec") == 0) {
				tm.tm_mon = 11;
			}
			tm.tm_year -= 1900;
			lasttime = mktime(&tm);
		}
	}

	if (logfound) {
		if ((newlen = ftell(file) - lastoff) != 0) {
			log = purple_log_new(PURPLE_LOG_IM, sn, account, NULL, -1, NULL);
			log->logger = old_logger;
			log->time = lasttime;

			/* IMPORTANT: Always set all members of struct old_logger_data */
			data = g_slice_new(struct old_logger_data);

			data->pathref = purple_stringref_ref(pathref);
			data->offset = lastoff;
			data->length = newlen;

			log->logger_data = data;
			list = g_list_prepend(list, log);

			/* XXX: Is there a proper way to print a time_t? */
			if (index != NULL)
				fprintf(index, "%d\t%d\t%d\n", data->offset, data->length, (int)log->time);
		}
	}

	purple_stringref_unref(pathref);
	fclose(file);
	if (index != NULL)
	{
		fclose(index);

		if (index_tmp == NULL)
		{
			g_free(pathstr);
			g_return_val_if_reached(list);
		}

		if (g_rename(index_tmp, pathstr))
		{
			purple_debug_warning("log", "Failed to rename index temp file \"%s\" to \"%s\": %s\n",
			                   index_tmp, pathstr, g_strerror(errno));
			g_unlink(index_tmp);
		}
		else
			purple_debug_info("log", "Built index: %s\n", pathstr);

		g_free(index_tmp);
		g_free(pathstr);
	}
	return list;
}

static int old_logger_total_size(PurpleLogType type, const char *name, PurpleAccount *account)
{
	char *logfile = g_strdup_printf("%s.log", purple_normalize(account, name));
	char *pathstr = g_build_filename(purple_user_dir(), "logs", logfile, NULL);
	int size;
	struct stat st;

	if (g_stat(pathstr, &st))
		size = 0;
	else
		size = st.st_size;

	g_free(logfile);
	g_free(pathstr);

	return size;
}

static char * old_logger_read (PurpleLog *log, PurpleLogReadFlags *flags)
{
	size_t result;
	struct old_logger_data *data = log->logger_data;
	const char *path = purple_stringref_value(data->pathref);
	FILE *file = g_fopen(path, "rb");
	char *read = g_malloc(data->length + 1);
	fseek(file, data->offset, SEEK_SET);
	result = fread(read, data->length, 1, file);
	if (result != 1)
		purple_debug_error("log", "Unable to read from log file: %s\n", path);
	fclose(file);
	read[data->length] = '\0';
	*flags = 0;
	if (strstr(read, "<BR>"))
	{
		*flags |= PURPLE_LOG_READ_NO_NEWLINE;
		return read;
	}

	return process_txt_log(read, NULL);
}

static int old_logger_size (PurpleLog *log)
{
	struct old_logger_data *data = log->logger_data;
	return data ? data->length : 0;
}

static void old_logger_get_log_sets(PurpleLogSetCallback cb, GHashTable *sets)
{
	char *log_path = g_build_filename(purple_user_dir(), "logs", NULL);
	GDir *log_dir = g_dir_open(log_path, 0, NULL);
	gchar *name;
	PurpleBlistNode *gnode, *cnode, *bnode;

	g_free(log_path);
	if (log_dir == NULL)
		return;

	/* Don't worry about the cast, name will be filled with a dynamically allocated data shortly. */
	while ((name = (gchar *)g_dir_read_name(log_dir)) != NULL) {
		size_t len;
		gchar *ext;
		PurpleLogSet *set;
		gboolean found = FALSE;

		/* Unescape the filename. */
		name = g_strdup(purple_unescape_filename(name));

		/* Get the (possibly new) length of name. */
		len = strlen(name);

		if (len < 5) {
			g_free(name);
			continue;
		}

		/* Make sure we're dealing with a log file. */
		ext = &name[len - 4];
		if (strcmp(ext, ".log")) {
			g_free(name);
			continue;
		}

		/* IMPORTANT: Always set all members of PurpleLogSet */
		set = g_slice_new(PurpleLogSet);

		/* Chat for .chat at the end of the name to determine the type. */
		*ext = '\0';
		set->type = PURPLE_LOG_IM;
		if (len > 9) {
			char *tmp = &name[len - 9];
			if (!strcmp(tmp, ".chat")) {
				set->type = PURPLE_LOG_CHAT;
				*tmp = '\0';
			}
		}

		set->name = set->normalized_name = name;

		/* Search the buddy list to find the account and to determine if this is a buddy. */
		for (gnode = purple_get_blist()->root; !found && gnode != NULL; gnode = gnode->next)
		{
			if (!PURPLE_BLIST_NODE_IS_GROUP(gnode))
				continue;

			for (cnode = gnode->child; !found && cnode != NULL; cnode = cnode->next)
			{
				if (!PURPLE_BLIST_NODE_IS_CONTACT(cnode))
					continue;

				for (bnode = cnode->child; !found && bnode != NULL; bnode = bnode->next)
				{
					PurpleBuddy *buddy = (PurpleBuddy *)bnode;

					if (!strcmp(buddy->name, name)) {
						set->account = buddy->account;
						set->buddy = TRUE;
						found = TRUE;
					}
				}
			}
		}

		if (!found)
		{
			set->account = NULL;
			set->buddy = FALSE;
		}

		cb(sets, set);
	}
	g_dir_close(log_dir);
}

static void old_logger_finalize(PurpleLog *log)
{
	struct old_logger_data *data = log->logger_data;
	purple_stringref_unref(data->pathref);
	g_slice_free(struct old_logger_data, data);
}

static void log_free_cb(PurpleLog *log, void *data)
{
	_purple_log_free_callback_data *callback_data = data;

	if (callback_data->cb != NULL && (callback_data->context == NULL || !purple_log_is_cancelled_operation(callback_data->context)))
		callback_data->cb(log, callback_data->context);

	if (callback_data->context != NULL)
		log_context_unref(callback_data->context);

	if (log->tm != NULL)
	{
#ifdef HAVE_STRUCT_TM_TM_ZONE
		/* XXX: This is so wrong... */
		g_free((char *)log->tm->tm_zone);
#endif
		g_slice_free(struct tm, log->tm);
	}

	PURPLE_DBUS_UNREGISTER_POINTER(log);
	g_slice_free(PurpleLog, log);
	g_free(log->name);
	g_free(callback_data);
}

static void log_size_cb(int size, PurpleLog *log, void *data)
{
	_purple_log_size_callback_data *callback_data = data;

	g_return_if_fail(callback_data != NULL);

	if (callback_data->context == NULL || !purple_log_is_cancelled_operation(callback_data->context))
		callback_data->cb(size, log, callback_data->context);

	if (callback_data->context != NULL)
		log_context_unref(callback_data->context);

	g_free(callback_data);
}

static void log_total_size_cb(int size, PurpleLogType type, char *name, PurpleAccount *account, void *data) 
{
	_purple_log_total_size_callback_data *callback_data = data;

	g_return_if_fail(callback_data != NULL);

	callback_data->ret_int += size;

	callback_data->counter--;

	if (!callback_data->counter) {
		if (callback_data->context == NULL || !purple_log_is_cancelled_operation(callback_data->context))
			callback_data->cb((int)callback_data->ret_int, type, name, account, callback_data->context);

		g_hash_table_replace(logsize_users, callback_data->lu, GINT_TO_POINTER(callback_data->ret_int));

		if (callback_data->context != NULL)
			log_context_unref(callback_data->context);

		g_free(name);
		g_free(callback_data);
	}
}

static void log_delete_cb(gboolean result, PurpleLog *log, void *data)
{
	_purple_log_delete_callback_data *callback_data = data;

	g_return_if_fail(callback_data != NULL);

	if (callback_data->cb != NULL && (callback_data->context == NULL || !purple_log_is_cancelled_operation(callback_data->context)))
		callback_data->cb(result, log, callback_data->context);

	if (callback_data->context != NULL)
		log_context_unref(callback_data->context);

	g_free(callback_data);
}

static void log_size_list_log_free_cb(int size, PurpleLog *log, PurpleLogContext *context)
{
	gpointer *callback_data = purple_log_context_get_userdata(context);

	purple_log_free_nonblocking(log, NULL, NULL);
	log_total_size_cb(size, (PurpleLogType)callback_data[1], callback_data[2], callback_data[3], callback_data[4]);
}

static void log_total_size_list_cb(GList *list, PurpleLogType type, char *name, PurpleAccount *account, void *data) 
{
	_purple_log_total_size_callback_data *callback_data = data;
	g_return_if_fail(callback_data != NULL);

	callback_data->counter += g_list_length(list);

	while (list) {
		PurpleLogContext *context = purple_log_context_new(g_free);

		gpointer *user_data = g_new(gpointer, 4);
		PurpleLog *log = (PurpleLog*)(list->data);

		user_data[0] = (gpointer)type;
		user_data[1] = name;
		user_data[2] = account;
		user_data[3] = callback_data;

		purple_log_context_set_userdata(context, callback_data);
		purple_log_get_size_nonblocking(log, log_size_list_log_free_cb, context);
		purple_log_context_close(context);

		list = g_list_delete_link(list, list);
	}

	/* extra call, we need it for destroing callback_data and calling UI callback */
	log_total_size_cb(0, type, name, account, callback_data);
}

static void log_list_cb(GList *list, PurpleLogType type, char *name, PurpleAccount *account, void *data)
{
	_purple_log_logs_callback_data *callback_data = data;

	g_return_if_fail(callback_data != NULL);

	callback_data->counter--;

	/* Pass logs up to the caller. */
	if (list != NULL) {

		if (callback_data->context == NULL || !purple_log_is_cancelled_operation(callback_data->context))
			callback_data->cb(list, type, name, account, callback_data->context);
		else {
			GList *logs;
			for (logs = list; logs != NULL; logs = logs->next) 
				purple_log_free_nonblocking(logs->data, NULL, NULL);

			g_list_free(list);
		}
	}

	if (!callback_data->counter) {
		/* Let the caller know we're done. */

		if (callback_data->context == NULL || !purple_log_is_cancelled_operation(callback_data->context))
			callback_data->cb(NULL, type, name, account, callback_data->context);

		if (callback_data->context != NULL)
			log_context_unref(callback_data->context);

		g_free(name);
		g_free(callback_data);
	}
}

static void log_system_list_cb(GList *list, PurpleAccount *account, void *data)
{
	_purple_log_system_logs_callback_data *callback_data = data;

	g_return_if_fail(callback_data != NULL);

	callback_data->counter--;

	/* Pass logs up to the caller. */
	if (list != NULL) {
		if (callback_data->context == NULL || !purple_log_is_cancelled_operation(callback_data->context))
			callback_data->cb(list, account, callback_data->context);
		else {
			GList *logs;
			for (logs = list; logs != NULL; logs = logs->next) 
				purple_log_free_nonblocking(logs->data, NULL, NULL);

			g_list_free(list);
		}
	}

	if (!callback_data->counter) {
		/* Let the caller know we're done. */
		if (callback_data->context == NULL || !purple_log_is_cancelled_operation(callback_data->context))
			callback_data->cb(NULL, account, callback_data->context);

		if (callback_data->context != NULL)
			log_context_unref(callback_data->context);

		g_free(callback_data);
	}
}

static void log_read_cb(char *text, PurpleLog *log, PurpleLogReadFlags *flags, void *data)
{
	_purple_log_read_callback_data *callback_data = data;

	g_return_if_fail(callback_data != NULL);

	if (callback_data->context == NULL || !purple_log_is_cancelled_operation(callback_data->context)) {
		purple_str_strip_char(text, '\r');
		callback_data->cb(text, log, flags, callback_data->context);
	}

	if (callback_data->context != NULL)
		log_context_unref(callback_data->context);

	g_free(callback_data);
}


static void log_write_cb(int size, PurpleLog *log, PurpleMessageFlags flags,
						char *from, time_t time, char *message, void *data)
{
	_purple_log_write_callback_data *callback_data = data;
	struct _purple_logsize_user *lu;
	gpointer ptrsize;
	gsize total = 0;

	g_return_if_fail(callback_data != NULL);
	lu = callback_data->lu;

	if(g_hash_table_lookup_extended(logsize_users, lu, NULL, &ptrsize)) {
		total = GPOINTER_TO_INT(ptrsize);
		total += size;
		g_hash_table_replace(logsize_users, lu, GINT_TO_POINTER(total));
	} else {
		g_free(lu->name);
		g_free(lu);
	}

	if (callback_data->cb && (callback_data->context == NULL || !purple_log_is_cancelled_operation(callback_data->context)))
		callback_data->cb(size, log, flags, from, time, message, callback_data->context);

	if (callback_data->context != NULL)
		log_context_unref(callback_data->context);

	g_free(from);
	g_free(message);
	g_free(callback_data);
}

static void log_hash_cb(void *data)
{
	_purple_log_sets_callback_data *callback_data = data;

	g_return_if_fail(callback_data != NULL);

	callback_data->counter--;

	if (!callback_data->counter) {
		if (callback_data->context == NULL || !purple_log_is_cancelled_operation(callback_data->context))
			callback_data->cb(callback_data->ret_sets, callback_data->context);
		else 
			g_hash_table_destroy(callback_data->ret_sets);

		if (callback_data->context != NULL)
			log_context_unref(callback_data->context);

		g_free(callback_data);
	}
}
