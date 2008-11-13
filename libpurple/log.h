/**
 * @file log.h Logging API
 * @ingroup core
 * @see @ref log-signals
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
#ifndef _PURPLE_LOG_H_
#define _PURPLE_LOG_H_

#include <stdio.h>


/********************************************************
 * DATA STRUCTURES **************************************
 ********************************************************/

typedef struct _PurpleLog PurpleLog;
typedef struct _PurpleLogLogger PurpleLogLogger;
typedef struct _PurpleLogCommonLoggerData PurpleLogCommonLoggerData;
typedef struct _PurpleLogSet PurpleLogSet;
typedef struct _PurpleLogContext PurpleLogContext;


typedef enum {
	PURPLE_LOG_IM,
	PURPLE_LOG_CHAT,
	PURPLE_LOG_SYSTEM
} PurpleLogType;

typedef enum {
	PURPLE_LOG_READ_NO_NEWLINE = 1
} PurpleLogReadFlags;

#include "account.h"
#include "conversation.h"

typedef void (*PurpleLogSetCallback) (GHashTable *sets, PurpleLogSet *set);

/* Log callback functions */
typedef void (*PurpleLogFreeCallback) (PurpleLog *log, PurpleLogContext *context);
typedef void (*PurpleLogReadCallback) (char *text, PurpleLog *log, PurpleLogReadFlags *flags, 
									PurpleLogContext *context);
typedef void (*PurpleLogWriteCallback) (int size, PurpleLog *log, PurpleMessageFlags flags, 
									const char *from, time_t time, char *message, PurpleLogContext *context);
typedef void (*PurpleLogRemoveLogCallback) (gboolean result, PurpleLog *log, PurpleLogContext *context);
typedef void (*PurpleLogListCallback) (GList *list, PurpleLogType type, const char *name, 
									PurpleAccount *account, PurpleLogContext *context);
typedef void (*PurpleLogSystemListCallback) (GList *list, PurpleAccount *account, PurpleLogContext *context);
typedef void (*PurpleLogSizeCallback) (int size, PurpleLog *log, PurpleLogContext *context);
typedef void (*PurpleLogTotalSizeCallback) (int size, PurpleLogType type, const char *name, PurpleAccount *account, 
										PurpleLogContext *context);
typedef void (*PurpleLogHashTableCallback) (GHashTable *table, PurpleLogContext *context);

typedef void (*PurpleLogDestroyContextCallback) (void *data);

/* Logger callback functions */
typedef void (*PurpleLogLoggerCreateLogCallback) (PurpleLog *log, void *data);
typedef PurpleLogLoggerCreateLogCallback PurpleLogLoggerFinalizeCallback;

typedef void (*PurpleLogLoggerReadCallback) (char *text, PurpleLog *log, PurpleLogReadFlags *flags, 
									void *data);
typedef void (*PurpleLogLoggerWriteCallback) (int size, PurpleLog *log, PurpleMessageFlags flags, 
									char *from, time_t time, char *message, void *data);
typedef void (*PurpleLogLoggerRemoveLogCallback) (gboolean result, PurpleLog *log, void *data);
typedef void (*PurpleLogLoggerListCallback) (GList *list, PurpleLogType type, char *name, 
									PurpleAccount *account, void *data);
typedef void (*PurpleLogLoggerSystemListCallback) (GList *list, PurpleAccount *account, void *data);
typedef void (*PurpleLogLoggerSizeCallback) (int size, PurpleLog *log, void *data);
typedef void (*PurpleLogLoggerTotalSizeCallback) (int size, PurpleLogType type, char *name, PurpleAccount *account, 
										void *data);
typedef void (*PurpleLogLoggerHashTableCallback) (void *data);

// TODO: If we want this to be merged for a minor release instead of a major,
// TODO: we need to use the padding for our new functions. As there is not
// TODO: enough padding, we'd have to temporarily allocate a secondary struct
// TODO: and store a pointer to it in one of the padding slots.

// TODO: Alternatively, we could just drop all the blocking stuff entirely
// TODO: and push all this for 3.0.0.  If that's done, this struct should be
// TODO: hidden and the function signatures copied and descriptions moved to
// TODO: the purple_log_logger_new() documentation block.

/**
 * A log logger.
 *
 * This struct gets filled out and is included in the PurpleLog.  It contains everything
 * needed to write and read from logs.
 */
struct _PurpleLogLogger {
	char *name;               /**< The logger's name */
	char *id;                 /**< an identifier to refer to this logger */

	/** This gets called when the log is first created.
	    I don't think this is actually needed. */
	void (*create)(PurpleLog *log);

	// TODO: We could probably drop this. There's really no reason to use it.
	// TODO: If we do, we should deprecate create at the same time.

	/** This gets called when the log is first created.
	    I don't think this is actually needed.
	    Note: provides callback to make call non-blockable */
	void (*create_nonblocking)(PurpleLog *log, PurpleLogLoggerCreateLogCallback cb, PurpleLogContext *context);

	/** This is used to write to the log file */
	gsize (*write)(PurpleLog *log,
		     PurpleMessageFlags type,
		     const char *from,
		     time_t time,
		     const char *message);

	/** This is used to write to the log
	    Note: provides callback to make call non-blockable */
	void (*write_nonblocking)(PurpleLog *log,
		     PurpleMessageFlags type,
		     char *from,
		     time_t time,
		     char *message, 
			 PurpleLogLoggerWriteCallback cb,
			 void *data);
 
	/** Called when the log is destroyed */
	void (*finalize)(PurpleLog *log);

	/** Called when the log is destroyed 
	    Note: provides callback to make call non-blockable */
	void (*finalize_nonblocking)(PurpleLog *log, PurpleLogLoggerFinalizeCallback cb, void *data);

	/** This function returns a sorted GList of available PurpleLogs */
	GList *(*list)(PurpleLogType type, const char *name, PurpleAccount *account);

	/** This function returns a sorted GList of available PurpleLogs 
	  * Note: provides callback to make call non-blockable */
	void (*list_nonblocking)(PurpleLogType type, char *name, PurpleAccount *account, PurpleLogLoggerListCallback cb, void *data);

	/** Given one of the logs returned by the logger's list function,
	 *  this returns the contents of the log in GtkIMHtml markup */
	char *(*read)(PurpleLog *log, PurpleLogReadFlags *flags);

	/** Given one of the logs returned by the logger's list function,
	 *  this returns the contents of the log in GtkIMHtml markup 
	 * Note: provides callback to make call non-blockable */
	void (*read_nonblocking)(PurpleLog *log, PurpleLogReadFlags *flags, PurpleLogLoggerReadCallback cb, void *data);

	/** Given one of the logs returned by the logger's list function,
	 *  this returns the size of the log in bytes */
	int (*size)(PurpleLog *log);

	/** Given one of the logs returned by the logger's list function,
	*  this returns the size of the log in bytes 
	* Note: provides callback to make call non-blockable */
	void (*size_nonblocking)(PurpleLog *log, PurpleLogLoggerSizeCallback cb, void *data);

	/** Returns the total size of all the logs. If this is undefined a default
	 *  implementation is used */
	int (*total_size)(PurpleLogType type, const char *name, PurpleAccount *account);

	/** Returns the total size of all the logs. If this is undefined a default
	 *  implementation is used
 	 * Note: provides callback to make call non-blockable */
	void (*total_size_nonblocking)(PurpleLogType type, char *name, PurpleAccount *account, PurpleLogLoggerTotalSizeCallback cb, void *data);

	/** This function returns a sorted GList of available system PurpleLogs */
	GList *(*list_syslog)(PurpleAccount *account);

	/** This function returns a sorted GList of available system PurpleLogs 
	   * Note: provides callback to make call non-blockable */
	void (*list_syslog_nonblocking)(PurpleAccount *account, PurpleLogLoggerSystemListCallback cb, void *data);

	/** Adds PurpleLogSets to a GHashTable. By passing the data in the PurpleLogSets
	 *  to list, the caller can get every available PurpleLog from the logger.
	 *  Loggers using purple_log_common_writer() (or otherwise storing their
	 *  logs in the same directory structure as the stock loggers) do not
	 *  need to implement this function.
	 *
	 *  Loggers which implement this function must create a PurpleLogSet,
	 *  then call @a cb with @a sets and the newly created PurpleLogSet. */
	void (*get_log_sets)(PurpleLogSetCallback cb, GHashTable *sets);

	/** Adds PurpleLogSets to a GHashTable. By passing the data in the PurpleLogSets
	 *  to list, the caller can get every available PurpleLog from the logger.
	 *  Loggers using purple_log_common_writer() (or otherwise storing their
	 *  logs in the same directory structure as the stock loggers) do not
	 *  need to implement this function.
	 *
	 *  Loggers which implement this function must create a PurpleLogSet,
	 *  then call @a cb with @a sets and the newly created PurpleLogSet. 
	 * Note: provides callback to make call non-blockable */
	void (*get_log_sets_nonblocking)(PurpleLogSetCallback set_cb, GHashTable *sets, 
		PurpleLogLoggerHashTableCallback cb, void *data);

	/* Attempts to delete the specified log, indicating success or failure */
	gboolean (*remove)(PurpleLog *log);

	/* Attempts to delete the specified log, indicating success or failure 
	 * Note: provides callback to make call non-blockable */
	void (*remove_nonblocking)(PurpleLog *log, PurpleLogLoggerRemoveLogCallback cb, void *data);

	/* Tests whether a log is deletable */
	gboolean (*is_deletable)(PurpleLog *log);

	void (*_purple_reserved1)(void);
	void (*_purple_reserved2)(void);
	void (*_purple_reserved3)(void);
	void (*_purple_reserved4)(void);
};

/**
 * A log.  Not the wooden type.
 */
struct _PurpleLog {
	PurpleLogType type;                     /**< The type of log this is */
	char *name;                           /**< The name of this log */
	PurpleAccount *account;                 /**< The account this log is taking
	                                           place on */
	PurpleConversation *conv;               /**< The conversation being logged */
	time_t time;                          /**< The time this conversation
	                                           started, converted to the local timezone */

	PurpleLogLogger *logger;                /**< The logging mechanism this log
	                                           is to use */
	void *logger_data;                    /**< Data used by the log logger */
	struct tm *tm;                        /**< The time this conversation
	                                           started, saved with original
	                                           timezone data, if available and
	                                           if struct tm has the BSD
	                                           timezone fields, else @c NULL.
	                                           Do NOT modify anything in this struct.*/

	/* IMPORTANT: Some code in log.c allocates these without zeroing them.
	 * IMPORTANT: Update that code if you add members here. */
};

/**
 * A common logger_data struct containing a file handle and path, as well
 * as a pointer to something else for additional data.
 */
struct _PurpleLogCommonLoggerData {
	char *path;
	FILE *file;
	void *extra_data;
};

/**
 * Describes available logs.
 *
 * By passing the elements of this struct to purple_log_get_logs(), the caller
 * can get all available PurpleLogs.
 */
struct _PurpleLogSet {
	PurpleLogType type;                     /**< The type of logs available */
	char *name;                           /**< The name of the logs available */
	PurpleAccount *account;                 /**< The account the available logs
	                                           took place on. This will be
	                                           @c NULL if the account no longer
	                                           exists. (Depending on a
	                                           logger's implementation of
	                                           list, it may not be possible
	                                           to load such logs.) */
	gboolean buddy;                       /**< Is this (account, name) a buddy
	                                           on the buddy list? */
	char *normalized_name;                /**< The normalized version of
	                                           @a name. It must be set, and
	                                           may be set to the same pointer
	                                           value as @a name. */

	/* IMPORTANT: Some code in log.c allocates these without zeroing them.
	 * IMPORTANT: Update that code if you add members here. */
};

/*TODO: short comments */
struct _PurpleLogContext {
	PurpleLogDestroyContextCallback destroy_user_data_cb;  
	void *user_data;                                  /* user data */
	gboolean is_cancelled;                            /* flag means should we make non-blocking callback calls with this context */
	gboolean is_closed;                               /* flag means that this context is closed for new calls*/
	int ref_count;                                    /* */
};

#ifdef __cplusplus
extern "C" {
#endif

/***************************************/
/** @name Log Functions                */
/***************************************/
/*@{*/

/**
 * Creates a new log
 *
 * @param type        The type of log this is.
 * @param name        The name of this conversation (screenname, chat name,
 *                    etc.)
 * @param account     The account the conversation is occurring on
 * @param conv        The conversation being logged
 * @param time        The time this conversation started
 * @param tm          The time this conversation started, with timezone data,
 *                    if available and if struct tm has the BSD timezone fields.
 * @return            The new log
 */
PurpleLog *purple_log_new(PurpleLogType type, const char *name, PurpleAccount *account,
                      PurpleConversation *conv, time_t time, const struct tm *tm);

/**
 * Frees a log
 *
 * @param log         The log to destroy
 */
void purple_log_free(PurpleLog *log);

/**
 * Frees a log
 *
 * @param log         The log to destroy
 * @param cb           The callback (optional: NULL value is valid)
 * @param data         User data
 */
void purple_log_free_nonblocking(PurpleLog *log, PurpleLogFreeCallback cb, PurpleLogContext *context);

/**
 * Writes to a log file. Assumes you have checked preferences already.
 *
 * @param log          The log to write to
 * @param type         The type of message being logged
 * @param from         Whom this message is coming from, or @c NULL for
 *                     system messages
 * @param time         A timestamp in UNIX time
 * @param message      The message to log
 */
void purple_log_write(PurpleLog *log,
		    PurpleMessageFlags type,
		    const char *from,
		    time_t time,
		    const char *message);

/**
 * Writes to a log file. Assumes you have checked preferences already.
 *
 * @param log          The log to write to
 * @param type         The type of message being logged
 * @param from         Whom this message is coming from, or @c NULL for
 *                     system messages
 * @param time         A timestamp in UNIX time
 * @param message      The message to log (will be deleted in function)
 * @param cb           The callback (optional: NULL value is valid)
 * @param data         User data
 */
void purple_log_write_nonblocking(PurpleLog *log,
		    PurpleMessageFlags type,
		    const char *from,
		    time_t time,
		    char *message,
			PurpleLogWriteCallback cb, 
			PurpleLogContext *context);

/**
 * Reads from a log
 *
 * @param log   The log to read from
 * @param flags The returned logging flags.
 *
 * @return The contents of this log in Purple Markup.
 */
char *purple_log_read(PurpleLog *log, PurpleLogReadFlags *flags);

/**
 * Reads from a log
 *
 * @param log   The log to read from
 * @param flags The returned logging flags.
 * @param cb    The callback
 * @param data  User data
 */
void purple_log_read_nonblocking(PurpleLog *log, PurpleLogReadFlags *flags, 
								PurpleLogReadCallback cb, PurpleLogContext *context);

/**
 * Returns a list of all available logs
 *
 * @param type                The type of the log
 * @param name                The name of the log
 * @param account             The account
 * @return                    A sorted list of PurpleLogs
 */
GList *purple_log_get_logs(PurpleLogType type, const char *name, PurpleAccount *account);

/**
 * Returns a list of all available logs. 
 * Note: provides callback to make call non-blockable
 *
 * @param type                The type of the log
 * @param name                The name of the log
 * @param account             The account
 * @param cb                  The callback
 * @param data                User data
 */
void purple_log_get_logs_nonblocking(PurpleLogType type, const char *name, PurpleAccount *account, 
									PurpleLogListCallback cb, PurpleLogContext *context);

/**
 * Returns a GHashTable of PurpleLogSets.
 *
 * A "log set" here means the information necessary to gather the
 * PurpleLogs for a given buddy/chat. This information would be passed
 * to purple_log_list to get a list of PurpleLogs.
 *
 * The primary use of this function is to get a list of everyone the
 * user has ever talked to (assuming he or she uses logging).
 *
 * The GHashTable that's returned will free all log sets in it when
 * destroyed. If a PurpleLogSet is removed from the GHashTable, it
 * must be freed with purple_log_set_free().
 *
 * @return A GHashTable of all available unique PurpleLogSets
 */
GHashTable *purple_log_get_log_sets(void);

/**
 * Returns a GHashTable of PurpleLogSets.
 *
 * A "log set" here means the information necessary to gather the
 * PurpleLogs for a given buddy/chat. This information would be passed
 * to purple_log_list to get a list of PurpleLogs.
 *
 * The primary use of this function is to get a list of everyone the
 * user has ever talked to (assuming he or she uses logging).
 *
 * The GHashTable that's returned will free all log sets in it when
 * destroyed. If a PurpleLogSet is removed from the GHashTable, it
 * must be freed with purple_log_set_free().
 *
 * @param cb      The callback
 * @param data    User data
*/
void purple_log_get_log_sets_nonblocking(PurpleLogHashTableCallback cb, PurpleLogContext *context);

/**
 * Returns a list of all available system logs
 *
 * @param account The account
 * @return        A sorted list of PurpleLogs
 */
GList *purple_log_get_system_logs(PurpleAccount *account);

/**
 * Returns a list of all available system logs
 *
 * @param account The account
 * @param cb                  The callback
 * @param data                User data
 */
void purple_log_get_system_logs_nonblocking(PurpleAccount *account, PurpleLogSystemListCallback cb, PurpleLogContext *context);

/**
 * Returns the size of a log
 *
 * @param log                 The log
 * @return                    The size of the log, in bytes
 */
int purple_log_get_size(PurpleLog *log);

/**
 * Returns the size of a log
 *
 * @param log                 The log
 * @return                    The size of the log, in bytes
 * @param cb                  The callback
 * @param data                User data
 */
void purple_log_get_size_nonblocking(PurpleLog *log, PurpleLogSizeCallback cb, PurpleLogContext *context);

/**
 * Returns the size, in bytes, of all available logs in this conversation
 *
 * @param type                The type of the log
 * @param name                The name of the log
 * @param account             The account
 * @return                    The size in bytes
 */
int purple_log_get_total_size(PurpleLogType type, const char *name, PurpleAccount *account);

/**
 * Returns the size, in bytes, of all available logs in this conversation
 *
 * @param type                The type of the log
 * @param name                The name of the log
 * @param account             The account
 * @return                    The size in bytes
 * @param cb                  The callback
 * @param data                User data
 */
void purple_log_get_total_size_nonblocking(PurpleLogType type, const char *name, PurpleAccount *account, 
									PurpleLogTotalSizeCallback cb,  PurpleLogContext *context);

/**
 * Tests whether a log is deletable
 *
 * A return value of @c FALSE indicates that purple_log_delete() will fail on this
 * log, unless something changes between the two calls.  A return value of @c TRUE,
 * however, does not guarantee the log can be deleted.
 *
 * @param log                 The log
 * @return                    A boolean indicating if the log is deletable
 */
gboolean purple_log_is_deletable(PurpleLog *log);

/**
 * Deletes a log
 *
 * @param log                 The log
 * @return                    A boolean indicating success or failure
 */
gboolean purple_log_delete(PurpleLog *log);

/**
 * Deletes a log
 *
 * @param log                 The log
 * @param cb                  The callback (optional: NULL value is valid)
 * @param data                User data
 */
void purple_log_delete_nonblocking(PurpleLog *log, PurpleLogRemoveLogCallback cb, PurpleLogContext *context);

/**
 * Returns the default logger directory Purple uses for a given account
 * and username.  This would be where Purple stores logs created by
 * the built-in text or HTML loggers.
 *
 * @param type                The type of the log.
 * @param name                The name of the log.
 * @param account             The account.
 * @return                    The default logger directory for Purple.
 */
char *purple_log_get_log_dir(PurpleLogType type, const char *name, PurpleAccount *account);

/**
 * Implements GCompareFunc for PurpleLogs
 *
 * @param y                   A PurpleLog
 * @param z                   Another PurpleLog
 * @return                    A value as specified by GCompareFunc
 */
gint purple_log_compare(gconstpointer y, gconstpointer z);

/**
 * Implements GCompareFunc for PurpleLogSets
 *
 * @param y                   A PurpleLogSet
 * @param z                   Another PurpleLogSet
 * @return                    A value as specified by GCompareFunc
 */
gint purple_log_set_compare(gconstpointer y, gconstpointer z);

/**
 * Frees a log set
 *
 * @param set         The log set to destroy
 */
void purple_log_set_free(PurpleLogSet *set);

/*@}*/

/******************************************/
/** @name Common Logger Functions         */
/******************************************/
/*@{*/

/**
 * Opens a new log file in the standard Purple log location
 * with the given file extension, named for the current time,
 * for writing.  If a log file is already open, the existing
 * file handle is retained.  The log's logger_data value is
 * set to a PurpleLogCommonLoggerData struct containing the log
 * file handle and log path.
 *
 * This function is intended to be used as a "common"
 * implementation of a logger's @c write function.
 * It should only be passed to purple_log_logger_new() and never
 * called directly.
 *
 * @param log   The log to write to.
 * @param ext   The file extension to give to this log file.
 */
void purple_log_common_writer(PurpleLog *log, const char *ext);

/**
 * Returns a sorted GList of PurpleLogs of the requested type.
 *
 * This function should only be used with logs that are written
 * with purple_log_common_writer().  It's intended to be used as
 * a "common" implementation of a logger's @c list function.
 * It should only be passed to purple_log_logger_new() and never
 * called directly.
 *
 * @param type     The type of the logs being listed.
 * @param name     The name of the log.
 * @param account  The account of the log.
 * @param ext      The file extension this log format uses.
 * @param logger   A reference to the logger struct for this log.
 *
 * @return A sorted GList of PurpleLogs matching the parameters.
 */
GList *purple_log_common_lister(PurpleLogType type, const char *name,
							  PurpleAccount *account, const char *ext,
							  PurpleLogLogger *logger);

/**
 * Returns a sorted GList of PurpleLogs of the requested type.
 *
 * This function should only be used with logs that are written
 * with purple_log_common_writer().  It's intended to be used as
 * a "common" implementation of a logger's @c list function.
 * It should only be passed to purple_log_logger_new() and never
 * called directly.
 *
 * @param type     The type of the logs being listed.
 * @param name     The name of the log.
 * @param account  The account of the log.
 * @param ext      The file extension this log format uses.
 * @param logger   A reference to the logger struct for this log.
 * @param cb       The callback 
 * @param data     User data
 */
void purple_log_common_lister_nonblocking(PurpleLogType type, const char *name, 
								PurpleAccount *account, const char *ext, 
								PurpleLogLogger *logger, PurpleLogListCallback cb, PurpleLogContext *context);

/**
 * Returns the total size of all the logs for a given user, with
 * a given extension.
 *
 * This function should only be used with logs that are written
 * with purple_log_common_writer().  It's intended to be used as
 * a "common" implementation of a logger's @c total_size function.
 * It should only be passed to purple_log_logger_new() and never
 * called directly.
 *
 * @param type     The type of the logs being sized.
 * @param name     The name of the logs to size
 *                 (e.g. the username or chat name).
 * @param account  The account of the log.
 * @param ext      The file extension this log format uses.
 *
 * @return The size of all the logs with the specified extension
 *         for the specified user.
 */
int purple_log_common_total_sizer(PurpleLogType type, const char *name,
								PurpleAccount *account, const char *ext);

								/**
 * Returns the total size of all the logs for a given user, with
 * a given extension.
 *
 * This function should only be used with logs that are written
 * with purple_log_common_writer().  It's intended to be used as
 * a "common" implementation of a logger's @c total_size function.
 * It should only be passed to purple_log_logger_new() and never
 * called directly.
 *
 * @param type     The type of the logs being sized.
 * @param name     The name of the logs to size
 *                 (e.g. the username or chat name).
 * @param account  The account of the log.
 * @param ext      The file extension this log format uses.
 *
 * @return The size of all the logs with the specified extension
 *         for the specified user.
 */
void purple_log_common_total_sizer_nonblocking(PurpleLogType type, const char *name,
								PurpleAccount *account, const char *ext,
								PurpleLogTotalSizeCallback cb, PurpleLogContext *context);

/**
 * Returns the size of a given PurpleLog.
 *
 * This function should only be used with logs that are written
 * with purple_log_common_writer().  It's intended to be used as
 * a "common" implementation of a logger's @c size function.
 * It should only be passed to purple_log_logger_new() and never
 * called directly.
 *
 * @param log      The PurpleLog to size.
 *
 * @return An integer indicating the size of the log in bytes.
 */
int purple_log_common_sizer(PurpleLog *log);

/**
 * Returns the size of a given PurpleLog.
 *
 * This function should only be used with logs that are written
 * with purple_log_common_writer().  It's intended to be used as
 * a "common" implementation of a logger's @c size function.
 * It should only be passed to purple_log_logger_new() and never
 * called directly.
 *
 * @param log      The PurpleLog to size.
 * @param cb       The callback 
 * @param data     User data
 */
void purple_log_common_sizer_nonblocking(PurpleLog *log, PurpleLogSizeCallback cb, PurpleLogContext *context);

/**
 * Deletes a log
 *
 * This function should only be used with logs that are written
 * with purple_log_common_writer().  It's intended to be used as
 * a "common" implementation of a logger's @c delete function.
 * It should only be passed to purple_log_logger_new() and never
 * called directly.
 *
 * @param log      The PurpleLog to delete.
 *
 * @return A boolean indicating success or failure.
 */
gboolean purple_log_common_deleter(PurpleLog *log);

/**
 * Checks to see if a log is deletable
 *
 * This function should only be used with logs that are written
 * with purple_log_common_writer().  It's intended to be used as
 * a "common" implementation of a logger's @c is_deletable function.
 * It should only be passed to purple_log_logger_new() and never
 * called directly.
 *
 * @param log      The PurpleLog to check.
 *
 * @return A boolean indicating if the log is deletable.
 */
gboolean purple_log_common_is_deletable(PurpleLog *log);

/**
 * Create log context which used for nonblocking log calls
 * @param cb     User callback which destroy log context data. NULL is valid argument
 *
 .* @return A created log context
 */
PurpleLogContext *purple_log_context_new(PurpleLogDestroyContextCallback cb);

/**
 * Mark context as closed, this mean that no calls will be made with this context
 *
 * @param context     Context
 */
void purple_log_context_close(PurpleLogContext *context);

/**
 * Check whether context is closed for non-blocking calls 
 *
 * @param context     Context
 * @result result of check-up
 */
gboolean purple_log_is_closed_context(PurpleLogContext *context);

/**
 * Set user data in context
 * @param context     PurpleLogContext, in which user data will be inserted
 * @param data     User data
 *
 */
void purple_log_context_set_userdata(PurpleLogContext *context, void *data);

/**
 * Return user data for specified context
 *
 * @param context     Context
 *
 * @return User data for context or NULL if context is empty
 */
void *purple_log_context_get_userdata(PurpleLogContext *context);

/**
 * Cancel all operation with this context
 * 
 * @param context     Context
 */
void purple_log_cancel_operation(PurpleLogContext *context);

/**
 * Check where operations with this context were cancelled 
 * 
 * @param context     Context
 * @return result of check-up
 */
gboolean purple_log_is_cancelled_operation(PurpleLogContext *context);

/*@}*/

/******************************************/
/** @name Logger Functions                */
/******************************************/
/*@{*/

/**
 * Creates a new logger
 *
 * @param id           The logger's id.
 * @param name         The logger's name.
 * @param functions    The number of functions being passed. The following
 *                     functions are currently available (in order): @c create,
 *                     @c write, @c finalize, @c list, @c read, @c size,
 *                     @c total_size, @c list_syslog, @c get_log_sets,
 *                     @c remove, @c is_deletable.
 *                     For details on these functions, see PurpleLogLogger.
 *                     Functions may not be skipped. For example, passing
 *                     @c create and @c write is acceptable (for a total of
 *                     two functions). Passing @c create and @c finalize,
 *                     however, is not. To accomplish that, the caller must
 *                     pass @c create, @c NULL (a placeholder for @c write),
 *                     and @c finalize (for a total of 3 functions).
 *
 * @return The new logger
 */
PurpleLogLogger *purple_log_logger_new(const char *id, const char *name, int functions, ...);

/**
 * Frees a logger
 *
 * @param logger       The logger to free
 */
void purple_log_logger_free(PurpleLogLogger *logger);

/**
 * Adds a new logger
 *
 * @param logger       The new logger to add
 */
void purple_log_logger_add (PurpleLogLogger *logger);

/**
 *
 * Removes a logger
 *
 * @param logger       The logger to remove
 */
void purple_log_logger_remove (PurpleLogLogger *logger);

/**
 *
 * Sets the current logger
 *
 * @param logger       The logger to set
 */
void purple_log_logger_set (PurpleLogLogger *logger);

/**
 *
 * Returns the current logger
 *
 * @return logger      The current logger
 */
PurpleLogLogger *purple_log_logger_get (void);

/**
 * Returns a GList containing the IDs and names of the registered
 * loggers.
 *
 * @return The list of IDs and names.
 */
GList *purple_log_logger_get_options(void);

/**************************************************************************/
/** @name Log Subsystem                                                   */
/**************************************************************************/
/*@{*/

/**
 * Initializes the log subsystem.
 */
void purple_log_init(void);

/**
 * Returns the log subsystem handle.
 *
 * @return The log subsystem handle.
 */
void *purple_log_get_handle(void);

/**
 * Uninitializes the log subsystem.
 */
void purple_log_uninit(void);

/*@}*/


#ifdef __cplusplus
}
#endif

#endif /* _PURPLE_LOG_H_ */
