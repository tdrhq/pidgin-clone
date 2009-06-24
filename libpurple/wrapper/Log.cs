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

/*
 * This file was auto-generated from the libpurple header files to provide a
 * clean interface between .NET/CLR and the unmanaged C library, libpurple.
 *
 * This code isn't complete, but completely a work in progress. :)
 * Three major things left:
 *  - Resolve the remaining UNKNOWN types.
 *  - Handle translation between delegate and function pointers.
 *  - Fill in the translation between public .NET class calls and private DllImport[] calls.
 */

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace PurpleWrapper
{
	public class Log
	{
		/*
		 * PurpleLog * purple_log_new(PurpleLogType type, char * name, PurpleAccount * account, PurpleConversation * conv, time_t time, struct tm)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_log_new(UNKNOWN type, string name, IntPtr account, IntPtr conv, UNKNOWN time, UNKNOWN tm);

		public static PurpleLog New(PurpleLogType type, string name, PurpleAccount account, PurpleConversation conv, time_t time, struct tm)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_log_free(PurpleLog * log)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_free(IntPtr log);

		public static void Free(PurpleLog log)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_log_write(PurpleLog * log, PurpleMessageFlags type, char * from, time_t time, char * message)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_write(IntPtr log, UNKNOWN type, string from, UNKNOWN time, string message);

		public static void Write(PurpleLog log, PurpleMessageFlags type, string from, time_t time, string message)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_log_read(PurpleLog * log, PurpleLogReadFlags * flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_log_read(IntPtr log, IntPtr flags);

		public static string Read(PurpleLog log, PurpleLogReadFlags flags)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_log_get_logs(PurpleLogType type, char * name, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_log_get_logs(UNKNOWN type, string name, IntPtr account);

		public static GList GetLogs(PurpleLogType type, string name, PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * GHashTable * purple_log_get_log_sets()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_log_get_log_sets();

		public static GHashTable GetLogSets()
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_log_get_system_logs(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_log_get_system_logs(IntPtr account);

		public static GList GetSystemLogs(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_log_get_size(PurpleLog * log)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_log_get_size(IntPtr log);

		public static int GetSize(PurpleLog log)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_log_get_total_size(PurpleLogType type, char * name, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_log_get_total_size(UNKNOWN type, string name, IntPtr account);

		public static int GetTotalSize(PurpleLogType type, string name, PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_log_get_activity_score(PurpleLogType type, char * name, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_log_get_activity_score(UNKNOWN type, string name, IntPtr account);

		public static int GetActivityScore(PurpleLogType type, string name, PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_log_is_deletable(PurpleLog * log)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_log_is_deletable(IntPtr log);

		public static bool IsDeletable(PurpleLog log)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_log_delete(PurpleLog * log)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_log_delete(IntPtr log);

		public static bool Delete(PurpleLog log)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_log_get_log_dir(PurpleLogType type, char * name, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_log_get_log_dir(UNKNOWN type, string name, IntPtr account);

		public static string GetLogDir(PurpleLogType type, string name, PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * gint purple_log_compare(gconstpointer y, gconstpointer z)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_log_compare(UNKNOWN y, UNKNOWN z);

		public static int Compare(gconstpointer y, gconstpointer z)
		{
			throw new NotImplementedException();
		}

		/*
		 * gint purple_log_set_compare(gconstpointer y, gconstpointer z)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_log_set_compare(UNKNOWN y, UNKNOWN z);

		public static int SetCompare(gconstpointer y, gconstpointer z)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_log_set_free(PurpleLogSet * set)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_set_free(IntPtr set);

		public static void SetFree(PurpleLogSet set)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_log_common_writer(PurpleLog * log, char * ext)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_common_writer(IntPtr log, string ext);

		public static void CommonWriter(PurpleLog log, string ext)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_log_common_lister(PurpleLogType type, char * name, PurpleAccount * account, char * ext, PurpleLogLogger * logger)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_log_common_lister(UNKNOWN type, string name, IntPtr account, string ext, IntPtr logger);

		public static GList CommonLister(PurpleLogType type, string name, PurpleAccount account, string ext, PurpleLogLogger logger)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_log_common_total_sizer(PurpleLogType type, char * name, PurpleAccount * account, char * ext)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_log_common_total_sizer(UNKNOWN type, string name, IntPtr account, string ext);

		public static int CommonTotalSizer(PurpleLogType type, string name, PurpleAccount account, string ext)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_log_common_sizer(PurpleLog * log)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_log_common_sizer(IntPtr log);

		public static int CommonSizer(PurpleLog log)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_log_common_deleter(PurpleLog * log)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_log_common_deleter(IntPtr log);

		public static bool CommonDeleter(PurpleLog log)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_log_common_is_deletable(PurpleLog * log)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_log_common_is_deletable(IntPtr log);

		public static bool CommonIsDeletable(PurpleLog log)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleLogLogger * purple_log_logger_new(char * id, char * name, int functions, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_log_logger_new(string id, string name, int functions, ...);

		public static PurpleLogLogger LoggerNew(string id, string name, int functions, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_log_logger_free(PurpleLogLogger * logger)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_logger_free(IntPtr logger);

		public static void LoggerFree(PurpleLogLogger logger)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_log_logger_add(PurpleLogLogger * logger)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_logger_add(IntPtr logger);

		public static void LoggerAdd(PurpleLogLogger logger)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_log_logger_remove(PurpleLogLogger * logger)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_logger_remove(IntPtr logger);

		public static void LoggerRemove(PurpleLogLogger logger)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_log_logger_set(PurpleLogLogger * logger)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_logger_set(IntPtr logger);

		public static void LoggerSet(PurpleLogLogger logger)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleLogLogger * purple_log_logger_get()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_log_logger_get();

		public static PurpleLogLogger LoggerGet()
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_log_logger_get_options()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_log_logger_get_options();

		public static GList LoggerGetOptions()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_log_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_log_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_log_get_handle();

		public static IntPtr GetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_log_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_uninit();

		public static void Uninit()
		{
			throw new NotImplementedException();
		}

	}
}

