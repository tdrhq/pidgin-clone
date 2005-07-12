/**
 * @file log.c Logging API
 * @ingroup core
 *
 * gaim
 *
 * Gaim is the legal property of its developers, whose names are too numerous
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "account.h"
#include "debug.h"
#include "internal.h"
#include "log.h"
#include "prefs.h"
#include "util.h"
#include "stringref.h"

static GSList *loggers = NULL;

static GaimLogLogger html_logger;
static GaimLogLogger txt_logger;
static GaimLogLogger old_logger;

struct _gaim_logsize_user {
	char *name;
	GaimAccount *account;
};
static GHashTable *logsize_users = NULL;


/**************************************************************************
 * PUBLIC LOGGING FUNCTIONS ***********************************************
 **************************************************************************/

GaimLog *gaim_log_new(GaimLogType type, const char *name, GaimAccount *account, time_t time)
{
	GaimLog *log = g_new0(GaimLog, 1);
	log->name = g_strdup(gaim_normalize(account, name));
	log->account = account;
	log->time = time;
	log->type = type;
	log->logger_data = NULL;
	log->logger = gaim_log_logger_get();
	if (log->logger && log->logger->create)
		log->logger->create(log);
	return log;
}

void gaim_log_free(GaimLog *log)
{
	g_return_if_fail(log);
	if (log->logger && log->logger->finalize)
		log->logger->finalize(log);
	g_free(log->name);
	g_free(log);
}

void gaim_log_write(GaimLog *log, GaimMessageFlags type,
		    const char *from, time_t time, const char *message)
{
	struct _gaim_logsize_user *lu;

	g_return_if_fail(log);
	g_return_if_fail(log->logger);
	g_return_if_fail(log->logger->write);

	(log->logger->write)(log, type, from, time, message);

	lu = g_new(struct _gaim_logsize_user, 1);

	lu->name = g_strdup(gaim_normalize(log->account, log->name));
	lu->account = log->account;
	g_hash_table_remove(logsize_users, lu);
	g_free(lu->name);
	g_free(lu);

}

char *gaim_log_read(GaimLog *log, GaimLogReadFlags *flags)
{
	GaimLogReadFlags mflags;
	g_return_val_if_fail(log && log->logger, NULL);
	if (log->logger->read) {
		char *ret = (log->logger->read)(log, flags ? flags : &mflags);
		gaim_str_strip_cr(ret);
		return ret;
	}
	return (_("<b><font color=\"red\">The logger has no read function</font></b>"));
}

int gaim_log_get_size(GaimLog *log)
{
	g_return_val_if_fail(log && log->logger, 0);

	if (log->logger->size)
		return log->logger->size(log);
	return 0;
}

static guint _gaim_logsize_user_hash(struct _gaim_logsize_user *lu)
{
	return g_str_hash(lu->name);
}

static guint _gaim_logsize_user_equal(struct _gaim_logsize_user *lu1,
		struct _gaim_logsize_user *lu2)
{
	return ((!strcmp(lu1->name, lu2->name)) && lu1->account == lu2->account);
}

static void _gaim_logsize_user_free_key(struct _gaim_logsize_user *lu)
{
	g_free(lu->name);
	g_free(lu);
}

int gaim_log_get_total_size(GaimLogType type, const char *name, GaimAccount *account)
{
	gpointer ptrsize;
	int size = 0;
	GSList *n;
	struct _gaim_logsize_user *lu;

	lu = g_new(struct _gaim_logsize_user, 1);
	lu->name = g_strdup(gaim_normalize(account, name));
	lu->account = account;

	if(g_hash_table_lookup_extended(logsize_users, lu, NULL, &ptrsize)) {
		size = GPOINTER_TO_INT(ptrsize);
		g_free(lu->name);
		g_free(lu);
	} else {
		for (n = loggers; n; n = n->next) {
			GaimLogLogger *logger = n->data;

			if(logger->total_size){
				size += (logger->total_size)(type, name, account);
			} else if(logger->list) {
				GList *logs = (logger->list)(type, name, account);
				int this_size = 0;

				while (logs) {
					GList *logs2 = logs->next;
					GaimLog *log = (GaimLog*)(logs->data);
					this_size += gaim_log_get_size(log);
					gaim_log_free(log);
					g_list_free_1(logs);
					logs = logs2;
				}

				size += this_size;
			}
		}

		g_hash_table_replace(logsize_users, lu, GINT_TO_POINTER(size));
	}
	return size;
}

static char *
gaim_log_get_log_dir(GaimLogType type, const char *name, GaimAccount *account)
{
	GaimPlugin *prpl;
	GaimPluginProtocolInfo *prpl_info;
	const char *prpl_name;
	char *acct_name;
	const char *target;
	char *dir;

	prpl = gaim_find_prpl(gaim_account_get_protocol_id(account));
	if (!prpl)
		return NULL;
	prpl_info = GAIM_PLUGIN_PROTOCOL_INFO(prpl);
	prpl_name = prpl_info->list_icon(account, NULL);

	acct_name = g_strdup(gaim_escape_filename(gaim_normalize(account,
				gaim_account_get_username(account))));

	if (type == GAIM_LOG_CHAT) {
		char *temp = g_strdup_printf("%s.chat", gaim_normalize(account, name));
		target = gaim_escape_filename(temp);
		g_free(temp);
	} else if(type == GAIM_LOG_SYSTEM) {
		target = ".system";
	} else {
		target = gaim_escape_filename(gaim_normalize(account, name));
	}

	dir = g_build_filename(gaim_user_dir(), "logs", prpl_name, acct_name, target, NULL);

	g_free(acct_name);

	return dir;
}

/****************************************************************************
 * LOGGER FUNCTIONS *********************************************************
 ****************************************************************************/

static GaimLogLogger *current_logger = NULL;

static void logger_pref_cb(const char *name, GaimPrefType type,
			   gpointer value, gpointer data)
{
	GaimLogLogger *logger;
	GSList *l = loggers;
	while (l) {
		logger = l->data;
		if (!strcmp(logger->id, value)) {
			gaim_log_logger_set(logger);
			return;
		}
		l = l->next;
	}
	gaim_log_logger_set(&txt_logger);
}


GaimLogLogger *gaim_log_logger_new(
				void(*create)(GaimLog *),
				void(*write)(GaimLog *, GaimMessageFlags, const char *, time_t, const char *),
				void(*finalize)(GaimLog *),
				GList*(*list)(GaimLogType type, const char*, GaimAccount*),
				char*(*read)(GaimLog*, GaimLogReadFlags*),
				int(*size)(GaimLog*))
{
	GaimLogLogger *logger = g_new0(GaimLogLogger, 1);
	logger->create = create;
	logger->write = write;
	logger->finalize = finalize;
	logger->list = list;
	logger->read = read;
	logger->size = size;
	return logger;
}

void gaim_log_logger_free(GaimLogLogger *logger)
{
	g_free(logger);
}

void gaim_log_logger_add (GaimLogLogger *logger)
{
	g_return_if_fail(logger);
	if (g_slist_find(loggers, logger))
		return;
	loggers = g_slist_append(loggers, logger);
}

void gaim_log_logger_remove (GaimLogLogger *logger)
{
	g_return_if_fail(logger);
	g_slist_remove(loggers, logger);
}

void gaim_log_logger_set (GaimLogLogger *logger)
{
	g_return_if_fail(logger);
	current_logger = logger;
}

GaimLogLogger *gaim_log_logger_get()
{
	return current_logger;
}

GList *gaim_log_logger_get_options(void)
{
	GSList *n;
	GList *list = NULL;
	GaimLogLogger *data;

	for (n = loggers; n; n = n->next) {
		data = n->data;
		if (!data->write)
			continue;
		list = g_list_append(list, _(data->name));
		list = g_list_append(list, data->id);
	}

	return list;
}

gint gaim_log_compare(gconstpointer y, gconstpointer z)
{
	const GaimLog *a = y;
	const GaimLog *b = z;

	return b->time - a->time;
}

GList *gaim_log_get_logs(GaimLogType type, const char *name, GaimAccount *account)
{
	GList *logs = NULL;
	GSList *n;
	for (n = loggers; n; n = n->next) {
		GaimLogLogger *logger = n->data;
		if (!logger->list)
			continue;
		logs = g_list_concat(logs, logger->list(type, name, account));
	}

	return g_list_sort(logs, gaim_log_compare);
}

GList *gaim_log_get_system_logs(GaimAccount *account)
{
	GList *logs = NULL;
	GSList *n;
	for (n = loggers; n; n = n->next) {
		GaimLogLogger *logger = n->data;
		if (!logger->list_syslog)
			continue;
		logs = g_list_concat(logs, logger->list_syslog(account));
	}

	return g_list_sort(logs, gaim_log_compare);
}

void gaim_log_init(void)
{
	gaim_prefs_add_none("/core/logging");
	gaim_prefs_add_bool("/core/logging/log_ims", FALSE);
	gaim_prefs_add_bool("/core/logging/log_chats", FALSE);
	gaim_prefs_add_bool("/core/logging/log_system", FALSE);
	gaim_prefs_add_bool("/core/logging/log_signon_signoff", FALSE);
	gaim_prefs_add_bool("/core/logging/log_idle_state", FALSE);
	gaim_prefs_add_bool("/core/logging/log_away_state", FALSE);
	gaim_prefs_add_bool("/core/logging/log_own_states", FALSE);

	gaim_prefs_add_string("/core/logging/format", "txt");
	gaim_log_logger_add(&html_logger);
	gaim_log_logger_add(&txt_logger);
	gaim_log_logger_add(&old_logger);
	gaim_prefs_connect_callback("/core/logging/format",
				    logger_pref_cb, NULL);
	gaim_prefs_trigger_callback("/core/logging/format");

	logsize_users = g_hash_table_new_full((GHashFunc)_gaim_logsize_user_hash,
			(GEqualFunc)_gaim_logsize_user_equal,
			(GDestroyNotify)_gaim_logsize_user_free_key, NULL);
}

/****************************************************************************
 * LOGGERS ******************************************************************
 ****************************************************************************/

struct generic_logger_data {
	char *path;
	FILE *file;
};

static void log_writer_common(GaimLog *log, GaimMessageFlags type,
		time_t time, const char *ext)
{
	char date[64];
	struct generic_logger_data *data = log->logger_data;

	if(!data) {
		/* This log is new */
		char *dir, *filename, *path;

		dir = gaim_log_get_log_dir(log->type, log->name, log->account);
		if (dir == NULL)
			return;

		gaim_build_dir (dir, S_IRUSR | S_IWUSR | S_IXUSR);

		strftime(date, sizeof(date), "%Y-%m-%d.%H%M%S", localtime(&log->time));

		filename = g_strdup_printf("%s%s", date, ext ? ext : "");

		path = g_build_filename(dir, filename, NULL);
		g_free(dir);
		g_free(filename);

		log->logger_data = data = g_new0(struct generic_logger_data, 1);

		data->file = g_fopen(path, "a");
		if (!data->file) {
			gaim_debug(GAIM_DEBUG_ERROR, "log",
					"Could not create log file %s\n", path);
			g_free(path);
			return;
		}
		g_free(path);
	}
}

static GList *log_lister_common(GaimLogType type, const char *name, GaimAccount *account, const char *ext, GaimLogLogger *logger)
{
	GDir *dir;
	GList *list = NULL;
	const char *filename;
	char *path;

	if(!account)
		return NULL;

	path = gaim_log_get_log_dir(type, name, account);
	if (path == NULL)
		return NULL;

	if (!(dir = g_dir_open(path, 0, NULL))) {
		g_free(path);
		return NULL;
	}

	while ((filename = g_dir_read_name(dir))) {
		if (gaim_str_has_suffix(filename, ext) &&
				strlen(filename) == 17 + strlen(ext)) {
			GaimLog *log;
			struct generic_logger_data *data;
			time_t stamp = gaim_str_to_time(filename, FALSE);

			log = gaim_log_new(type, name, account, stamp);
			log->logger = logger;
			log->logger_data = data = g_new0(struct generic_logger_data, 1);
			data->path = g_build_filename(path, filename, NULL);
			list = g_list_append(list, log);
		}
	}
	g_dir_close(dir);
	g_free(path);
	return list;
}

/* Only to be used with logs listed from log_lister_common */
int log_sizer_common(GaimLog *log)
{
	struct stat st;
	struct generic_logger_data *data = log->logger_data;

	if (!data->path || g_stat(data->path, &st))
		st.st_size = 0;

	return st.st_size;
}

#if 0 /* Maybe some other time. */
/****************
 ** XML LOGGER **
 ****************/

static const char *str_from_msg_type (GaimMessageFlags type)
{

		return "";

}

static void xml_logger_write(GaimLog *log,
			     GaimMessageFlags type,
			     const char *from, time_t time, const char *message)
{
	char date[64];
	char *xhtml = NULL;

	if (!log->logger_data) {
		/* This log is new.  We could use the loggers 'new' function, but
		 * creating a new file there would result in empty files in the case
		 * that you open a convo with someone, but don't say anything.
		 */
		char *dir = gaim_log_get_log_dir(log->type, log->name, log->account);
		FILE *file;

		if (dir == NULL)
			return;

		strftime(date, sizeof(date), "%Y-%m-%d.%H%M%S.xml", localtime(&log->time));

		gaim_build_dir (dir, S_IRUSR | S_IWUSR | S_IXUSR);

		char *filename = g_build_filename(dir, date, NULL);
		g_free(dir);

		log->logger_data = g_fopen(filename, "a");
		if (!log->logger_data) {
			gaim_debug(GAIM_DEBUG_ERROR, "log", "Could not create log file %s\n", filename);
			g_free(filename);
			return;
		}
		g_free(filename);
		fprintf(log->logger_data, "<?xml version='1.0' encoding='UTF-8' ?>\n"
			"<?xml-stylesheet href='file:///usr/src/web/htdocs/log-stylesheet.xsl' type='text/xml' ?>\n");

		strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", localtime(&log->time));
		fprintf(log->logger_data, "<conversation time='%s' screenname='%s' protocol='%s'>\n",
			date, log->name, prpl);
	}

	/* if we can't write to the file, give up before we hurt ourselves */
	if(!data->file)
		return;

	strftime(date, sizeof(date), "%H:%M:%S", localtime(&time));
	gaim_markup_html_to_xhtml(message, &xhtml, NULL);
	if (from)
		fprintf(log->logger_data, "<message %s %s from='%s' time='%s'>%s</message>\n",
			str_from_msg_type(type),
			type & GAIM_MESSAGE_SEND ? "direction='sent'" :
			type & GAIM_MESSAGE_RECV ? "direction='received'" : "",
			from, date, xhtml);
	else
		fprintf(log->logger_data, "<message %s %s time='%s'>%s</message>\n",
			str_from_msg_type(type),
			type & GAIM_MESSAGE_SEND ? "direction='sent'" :
			type & GAIM_MESSAGE_RECV ? "direction='received'" : "",
			date, xhtml):
	fflush(log->logger_data);
	g_free(xhtml);
}

 static void xml_logger_finalize(GaimLog *log)
{
	if (log->logger_data) {
		fprintf(log->logger_data, "</conversation>\n");
		fclose(log->logger_data);
		log->logger_data = NULL;
	}
}

static GList *xml_logger_list(GaimLogType type, const char *sn, GaimAccount *account)
{
	return log_lister_common(type, sn, account, ".xml", &xml_logger);
}

static GaimLogLogger xml_logger =  {
	N_("XML"), "xml",
	NULL,
	xml_logger_write,
	xml_logger_finalize,
	xml_logger_list,
	NULL,
	NULL
};
#endif

/****************************
 ** HTML LOGGER *************
 ****************************/

static void html_logger_write(GaimLog *log, GaimMessageFlags type,
		const char *from, time_t time, const char *message)
{
	char *msg_fixed;
	char date[64];
	GaimPlugin *plugin = gaim_find_prpl(gaim_account_get_protocol_id(log->account));
	const char *prpl_name = plugin->info->name;
	struct generic_logger_data *data = log->logger_data;

	if(!data) {
		const char *prpl =
			GAIM_PLUGIN_PROTOCOL_INFO(plugin)->list_icon(log->account, NULL);
		log_writer_common(log, type, time, ".html");

		data = log->logger_data;

		/* if we can't write to the file, give up before we hurt ourselves */
		if(!data->file)
			return;

		strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", localtime(&log->time));
		fprintf(data->file, "<html><head>");
		fprintf(data->file, "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">");
		fprintf(data->file, "<title>");
		fprintf(data->file, "Conversation with %s at %s on %s (%s)",
			log->name, date, gaim_account_get_username(log->account), prpl);
		fprintf(data->file, "</title></head><body>");
		fprintf(data->file,
			"<h3>Conversation with %s at %s on %s (%s)</h3>\n",
			log->name, date, gaim_account_get_username(log->account), prpl);

	}

	/* if we can't write to the file, give up before we hurt ourselves */
	if(!data->file)
		return;

	gaim_markup_html_to_xhtml(message, &msg_fixed, NULL);

	if(log->type == GAIM_LOG_SYSTEM){
		strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", localtime(&time));
		fprintf(data->file, "---- %s @ %s ----<br/>\n", msg_fixed, date);
	} else {
		strftime(date, sizeof(date), "%H:%M:%S", localtime(&time));
		if (type & GAIM_MESSAGE_SYSTEM)
			fprintf(data->file, "<font size=\"2\">(%s)</font><b> %s</b><br/>\n", date, msg_fixed);
		else if (type & GAIM_MESSAGE_WHISPER)
			fprintf(data->file, "<font color=\"#6C2585\"><font size=\"2\">(%s)</font><b> %s:</b></font> %s<br/>\n",
					date, from, msg_fixed);
		else if (type & GAIM_MESSAGE_AUTO_RESP) {
			if (type & GAIM_MESSAGE_SEND)
				fprintf(data->file, _("<font color=\"#16569E\"><font size=\"2\">(%s)</font> <b>%s &lt;AUTO-REPLY&gt;:</b></font> %s<br/>\n"), date, from, msg_fixed);
			else if (type & GAIM_MESSAGE_RECV)
				fprintf(data->file, _("<font color=\"#A82F2F\"><font size=\"2\">(%s)</font> <b>%s &lt;AUTO-REPLY&gt;:</b></font> %s<br/>\n"), date, from, msg_fixed);
		} else if (type & GAIM_MESSAGE_RECV) {
			if(gaim_message_meify(msg_fixed, -1))
				fprintf(data->file, "<font color=\"#6C2585\"><font size=\"2\">(%s)</font> <b>***%s</b></font> <font sml=\"%s\">%s</font><br/>\n",
						date, from, prpl_name, msg_fixed);
			else
				fprintf(data->file, "<font color=\"#A82F2F\"><font size=\"2\">(%s)</font> <b>%s:</b></font> <font sml=\"%s\">%s</font><br/>\n",
						date, from, prpl_name, msg_fixed);
		} else if (type & GAIM_MESSAGE_SEND) {
			if(gaim_message_meify(msg_fixed, -1))
				fprintf(data->file, "<font color=\"#6C2585\"><font size=\"2\">(%s)</font> <b>***%s</b></font> <font sml=\"%s\">%s</font><br/>\n",
						date, from, prpl_name, msg_fixed);
			else
				fprintf(data->file, "<font color=\"#16569E\"><font size=\"2\">(%s)</font> <b>%s:</b></font> <font sml=\"%s\">%s</font><br/>\n",
						date, from, prpl_name, msg_fixed);
		}
	}

	g_free(msg_fixed);
	fflush(data->file);
}

static void html_logger_finalize(GaimLog *log)
{
	struct generic_logger_data *data = log->logger_data;
	if (data) {
		if(data->file) {
			fprintf(data->file, "</body></html>");
			fclose(data->file);
		}
		g_free(data->path);
		g_free(data);
	}
}

static GList *html_logger_list(GaimLogType type, const char *sn, GaimAccount *account)
{
	return log_lister_common(type, sn, account, ".html", &html_logger);
}

static GList *html_logger_list_syslog(GaimAccount *account)
{
	return log_lister_common(GAIM_LOG_SYSTEM, ".system", account, ".html", &html_logger);
}

static char *html_logger_read(GaimLog *log, GaimLogReadFlags *flags)
{
	char *read, *minus_header;
	struct generic_logger_data *data = log->logger_data;
	*flags = GAIM_LOG_READ_NO_NEWLINE;
	if (!data || !data->path)
		return g_strdup(_("<font color=\"red\"><b>Unable to find log path!</b></font>"));
	if (g_file_get_contents(data->path, &read, NULL, NULL)) {
		minus_header = strchr(read, '\n');
		if (!minus_header)
			minus_header = g_strdup(read);
		else
			minus_header = g_strdup(minus_header + 1);
		g_free(read);
		return minus_header;
	}
	return g_strdup_printf(_("<font color=\"red\"><b>Could not read file: %s</b></font>"), data->path);
}

static GaimLogLogger html_logger = {
	N_("HTML"), "html",
	NULL,
	html_logger_write,
	html_logger_finalize,
	html_logger_list,
	html_logger_read,
	log_sizer_common,
	NULL,
	html_logger_list_syslog
};




/****************************
 ** PLAIN TEXT LOGGER *******
 ****************************/

static void txt_logger_write(GaimLog *log,
			     GaimMessageFlags type,
			     const char *from, time_t time, const char *message)
{
	char date[64];
	GaimPlugin *plugin = gaim_find_prpl(gaim_account_get_protocol_id(log->account));
	struct generic_logger_data *data = log->logger_data;
	char *stripped = NULL;

	if(!data) {
		/* This log is new.  We could use the loggers 'new' function, but
		 * creating a new file there would result in empty files in the case
		 * that you open a convo with someone, but don't say anything.
		 */
		const char *prpl =
			GAIM_PLUGIN_PROTOCOL_INFO(plugin)->list_icon(log->account, NULL);
		log_writer_common(log, type, time, ".txt");

		data = log->logger_data;

		/* if we can't write to the file, give up before we hurt ourselves */
		if(!data->file)
			return;

		strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", localtime(&log->time));
		fprintf(data->file, "Conversation with %s at %s on %s (%s)\n",
			log->name, date, gaim_account_get_username(log->account), prpl);
	}

	/* if we can't write to the file, give up before we hurt ourselves */
	if(!data->file)
		return;

 	stripped = gaim_markup_strip_html(message);
  
 	if(log->type == GAIM_LOG_SYSTEM){
 		strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", localtime(&time));
 		fprintf(data->file, "---- %s @ %s ----\n", stripped, date);
 	} else {
 		strftime(date, sizeof(date), "%H:%M:%S", localtime(&time));
 		if (type & GAIM_MESSAGE_SEND ||
 			type & GAIM_MESSAGE_RECV) {
 			if (type & GAIM_MESSAGE_AUTO_RESP) {
 				fprintf(data->file, _("(%s) %s <AUTO-REPLY>: %s\n"), date,
 						from, stripped);
 			} else {
 				if(gaim_message_meify(stripped, -1))
 					fprintf(data->file, "(%s) ***%s %s\n", date, from,
 							stripped);
 				else
 					fprintf(data->file, "(%s) %s: %s\n", date, from,
 							stripped);
 			}
 		} else if (type & GAIM_MESSAGE_SYSTEM)
 			fprintf(data->file, "(%s) %s\n", date, stripped);
 		else if (type & GAIM_MESSAGE_NO_LOG) {
 			/* This shouldn't happen */
 			g_free(stripped);
 			return;
 		} else if (type & GAIM_MESSAGE_WHISPER)
 			fprintf(data->file, "(%s) *%s* %s", date, from, stripped);
 		else
 			fprintf(data->file, "(%s) %s%s %s\n", date, from ? from : "",
 					from ? ":" : "", stripped);
 	}
 
 	fflush(data->file);
 	g_free(stripped);
}

static void txt_logger_finalize(GaimLog *log)
{
	struct generic_logger_data *data = log->logger_data;
	if (data) {
		if(data->file)
			fclose(data->file);
		if(data->path)
			g_free(data->path);
		g_free(data);
	}
}

static GList *txt_logger_list(GaimLogType type, const char *sn, GaimAccount *account)
{
	return log_lister_common(type, sn, account, ".txt", &txt_logger);
}

static GList *txt_logger_list_syslog(GaimAccount *account)
{
	return log_lister_common(GAIM_LOG_SYSTEM, ".system", account, ".txt", &txt_logger);
}

static char *txt_logger_read(GaimLog *log, GaimLogReadFlags *flags)
{
	char *read, *minus_header, *minus_header2;
	struct generic_logger_data *data = log->logger_data;
	*flags = 0;
	if (!data || !data->path)
		return g_strdup(_("<font color=\"red\"><b>Unable to find log path!</b></font>"));
	if (g_file_get_contents(data->path, &read, NULL, NULL)) {
		minus_header = strchr(read, '\n');
		if (!minus_header)
			minus_header = g_strdup(read);
		else
			minus_header = g_strdup(minus_header + 1);
		g_free(read);
		minus_header2 = gaim_escape_html(minus_header);
		g_free(minus_header);
		return minus_header2;
	}
	return g_strdup_printf(_("<font color=\"red\"><b>Could not read file: %s</b></font>"), data->path);
}

static GaimLogLogger txt_logger = {
	N_("Plain text"), "txt",
	NULL,
	txt_logger_write,
	txt_logger_finalize,
	txt_logger_list,
	txt_logger_read,
	log_sizer_common,
	NULL,
	txt_logger_list_syslog
};

/****************
 * OLD LOGGER ***
 ****************/

/* The old logger doesn't write logs, only reads them.  This is to include
 * old logs in the log viewer transparently.
 */

struct old_logger_data {
	GaimStringref *pathref;
	int offset;
	int length;
};

static GList *old_logger_list(GaimLogType type, const char *sn, GaimAccount *account)
{
	FILE *file;
	char buf[BUF_LONG];
	struct tm tm;
	char month[4];
	struct old_logger_data *data = NULL;
	char *logfile = g_strdup_printf("%s.log", gaim_normalize(account, sn));
	char *pathstr = g_build_filename(gaim_user_dir(), "logs", logfile, NULL);
	GaimStringref *pathref = gaim_stringref_new(pathstr);
	char *newlog;
	int logfound = 0;
	int lastoff = 0;
	int newlen;
	time_t lasttime = 0;

	GaimLog *log = NULL;
	GList *list = NULL;

	g_free(logfile);
	g_free(pathstr);

	if (!(file = g_fopen(gaim_stringref_value(pathref), "rb"))) {
		gaim_stringref_unref(pathref);
		return NULL;
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
					log = gaim_log_new(GAIM_LOG_IM, sn, account, -1);
					log->logger = &old_logger;
					log->time = lasttime;
					data = g_new0(struct old_logger_data, 1);
					data->offset = lastoff;
					data->length = newlen;
					data->pathref = gaim_stringref_ref(pathref);
					log->logger_data = data;
					list = g_list_append(list, log);
				}
			}

			logfound = 1;
			lastoff = offset;

			g_snprintf(convostart, length, "%s", temp);
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
			log = gaim_log_new(GAIM_LOG_IM, sn, account, -1);
			log->logger = &old_logger;
			log->time = lasttime;
			data = g_new0(struct old_logger_data, 1);
			data->offset = lastoff;
			data->length = newlen;
			data->pathref = gaim_stringref_ref(pathref);
			log->logger_data = data;
			list = g_list_append(list, log);
		}
	}

	gaim_stringref_unref(pathref);
	fclose(file);
	return list;
}

static int old_logger_total_size(GaimLogType type, const char *name, GaimAccount *account)
{
	char *logfile = g_strdup_printf("%s.log", gaim_normalize(account, name));
	char *pathstr = g_build_filename(gaim_user_dir(), "logs", logfile, NULL);
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

static char * old_logger_read (GaimLog *log, GaimLogReadFlags *flags)
{
	struct old_logger_data *data = log->logger_data;
	FILE *file = g_fopen(gaim_stringref_value(data->pathref), "rb");
	char *tmp, *read = g_malloc(data->length + 1);
	fseek(file, data->offset, SEEK_SET);
	fread(read, data->length, 1, file);
	fclose(file);
	read[data->length] = '\0';
	*flags = 0;
	if(strstr(read, "<BR>"))
		*flags |= GAIM_LOG_READ_NO_NEWLINE;
	else {
		tmp = g_markup_escape_text(read, -1);
		g_free(read);
		read = tmp;
	}
	return read;
}

static int old_logger_size (GaimLog *log)
{
	struct old_logger_data *data = log->logger_data;
	return data ? data->length : 0;
}

static void old_logger_finalize(GaimLog *log)
{
	struct old_logger_data *data = log->logger_data;
	gaim_stringref_unref(data->pathref);
	g_free(data);
}

static GaimLogLogger old_logger = {
	"old logger", "old",
	NULL, NULL,
	old_logger_finalize,
	old_logger_list,
	old_logger_read,
	old_logger_size,
	old_logger_total_size,
	NULL
};
