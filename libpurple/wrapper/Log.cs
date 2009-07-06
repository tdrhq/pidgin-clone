/* PurpleWrapper - A .NET (CLR) wrapper for libpurple
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
 * clean interface between .NET/CLR and the unmanaged C library libpurple.
 *
 * This is the second major commit of the code.
 * Next things:
 *  - A few of the .h files have anonymous parameter names (eg: void cat(int, int).
 *    This program will need to assign these parameters names.
 *  - Function pointers inside structs aren't translated correctly into C#.
 *  - Two places there are specific-length arrays (eg: char hostname[256]). The parser
 *    does not detect them as an array.
 */

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace PurpleWrapper
{
	public class Log
	{
		public enum PurpleLogType
		{
			PURPLE_LOG_IM,
			PURPLE_LOG_CHAT,
			PURPLE_LOG_SYSTEM
		};

		public enum PurpleLogReadFlags
		{
			PURPLE_LOG_READ_NO_NEWLINE = 1
		};

		/*
		 * void purple_log_free(PurpleLog * log)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_free(IntPtr log);

		public static void Free(PurpleLog log)
		{
			purple_log_free(log.Reference);
		}

		/*
		 * char * purple_log_read(PurpleLog * log, PurpleLogReadFlags * flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_log_read(IntPtr log, Log.PurpleLogReadFlags flags);

		public static string Read(PurpleLog log, Log.PurpleLogReadFlags flags)
		{
			/* Unable to process flags, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_log_get_logs(PurpleLogType type, char * name, PurpleAccount * account)
		 * 
		 * Could not generate a wrapper for purple_log_get_logs in file "log.h".
		 * Message: The type could not be resolved (GList * purple_log_get_logs(PurpleLogType type, char * name, PurpleAccount * account)).
		 */

		/*
		 * GHashTable * purple_log_get_log_sets()
		 * 
		 * Could not generate a wrapper for purple_log_get_log_sets in file "log.h".
		 * Message: The type could not be resolved (GHashTable * purple_log_get_log_sets()).
		 */

		/*
		 * GList * purple_log_get_system_logs(PurpleAccount * account)
		 * 
		 * Could not generate a wrapper for purple_log_get_system_logs in file "log.h".
		 * Message: The type could not be resolved (GList * purple_log_get_system_logs(PurpleAccount * account)).
		 */

		/*
		 * int purple_log_get_size(PurpleLog * log)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_log_get_size(IntPtr log);

		public static int GetSize(PurpleLog log)
		{
			return purple_log_get_size(log.Reference);
		}

		/*
		 * int purple_log_get_total_size(PurpleLogType type, char * name, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_log_get_total_size(Log.PurpleLogType type, string name, IntPtr account);

		public static int GetTotalSize(Log.PurpleLogType type, string name, PurpleAccount account)
		{
			/* Unable to process type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * int purple_log_get_activity_score(PurpleLogType type, char * name, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_log_get_activity_score(Log.PurpleLogType type, string name, IntPtr account);

		public static int GetActivityScore(Log.PurpleLogType type, string name, PurpleAccount account)
		{
			/* Unable to process type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_log_is_deletable(PurpleLog * log)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_log_is_deletable(IntPtr log);

		public static bool IsDeletable(PurpleLog log)
		{
			return purple_log_is_deletable(log.Reference);
		}

		/*
		 * gboolean purple_log_delete(PurpleLog * log)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_log_delete(IntPtr log);

		public static bool Delete(PurpleLog log)
		{
			return purple_log_delete(log.Reference);
		}

		/*
		 * char * purple_log_get_log_dir(PurpleLogType type, char * name, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_log_get_log_dir(Log.PurpleLogType type, string name, IntPtr account);

		public static string GetLogDir(Log.PurpleLogType type, string name, PurpleAccount account)
		{
			/* Unable to process type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * gint purple_log_compare(gconstpointer y, gconstpointer z)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_log_compare(IntPtr y, IntPtr z);

		public static int Compare(IntPtr y, IntPtr z)
		{
			return purple_log_compare(y, z);
		}

		/*
		 * gint purple_log_set_compare(gconstpointer y, gconstpointer z)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_log_set_compare(IntPtr y, IntPtr z);

		public static int SetCompare(IntPtr y, IntPtr z)
		{
			return purple_log_set_compare(y, z);
		}

		/*
		 * void purple_log_set_free(PurpleLogSet * set)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_set_free(IntPtr set);

		public static void SetFree(PurpleLogSet set)
		{
			purple_log_set_free(set.Reference);
		}

		/*
		 * void purple_log_common_writer(PurpleLog * log, char * ext)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_common_writer(IntPtr log, string ext);

		public static void CommonWriter(PurpleLog log, string ext)
		{
			purple_log_common_writer(log.Reference, ext);
		}

		/*
		 * int purple_log_common_sizer(PurpleLog * log)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_log_common_sizer(IntPtr log);

		public static int CommonSizer(PurpleLog log)
		{
			return purple_log_common_sizer(log.Reference);
		}

		/*
		 * gboolean purple_log_common_deleter(PurpleLog * log)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_log_common_deleter(IntPtr log);

		public static bool CommonDeleter(PurpleLog log)
		{
			return purple_log_common_deleter(log.Reference);
		}

		/*
		 * gboolean purple_log_common_is_deletable(PurpleLog * log)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_log_common_is_deletable(IntPtr log);

		public static bool CommonIsDeletable(PurpleLog log)
		{
			return purple_log_common_is_deletable(log.Reference);
		}

		/*
		 * void purple_log_logger_free(PurpleLogLogger * logger)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_logger_free(IntPtr logger);

		public static void LoggerFree(PurpleLogLogger logger)
		{
			purple_log_logger_free(logger.Reference);
		}

		/*
		 * void purple_log_logger_add(PurpleLogLogger * logger)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_logger_add(IntPtr logger);

		public static void LoggerAdd(PurpleLogLogger logger)
		{
			purple_log_logger_add(logger.Reference);
		}

		/*
		 * void purple_log_logger_remove(PurpleLogLogger * logger)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_logger_remove(IntPtr logger);

		public static void LoggerRemove(PurpleLogLogger logger)
		{
			purple_log_logger_remove(logger.Reference);
		}

		/*
		 * void purple_log_logger_set(PurpleLogLogger * logger)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_logger_set(IntPtr logger);

		public static void LoggerSet(PurpleLogLogger logger)
		{
			purple_log_logger_set(logger.Reference);
		}

		/*
		 * PurpleLogLogger * purple_log_logger_get()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_log_logger_get();

		public static PurpleLogLogger LoggerGet()
		{
			return new PurpleLogLogger(purple_log_logger_get());
		}

		/*
		 * GList * purple_log_logger_get_options()
		 * 
		 * Could not generate a wrapper for purple_log_logger_get_options in file "log.h".
		 * Message: The type could not be resolved (GList * purple_log_logger_get_options()).
		 */

		/*
		 * void purple_log_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_init();

		public static void Init()
		{
			purple_log_init();
		}

		/*
		 * void * purple_log_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_log_get_handle();

		public static IntPtr GetHandle()
		{
			return purple_log_get_handle();
		}

		/*
		 * void purple_log_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_log_uninit();

		public static void Uninit()
		{
			purple_log_uninit();
		}

	}
}

