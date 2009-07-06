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
	public class Savedstatuses
	{
		/*
		 * gboolean purple_savedstatus_delete(char * title)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_savedstatus_delete(string title);

		public static bool SavedstatusDelete(string title)
		{
			return purple_savedstatus_delete(title);
		}

		/*
		 * void purple_savedstatus_delete_by_status(PurpleSavedStatus * saved_status)
		 * 
		 * Could not generate a wrapper for purple_savedstatus_delete_by_status in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatus * saved_status).
		 */

		/*
		 * GList * purple_savedstatuses_get_all()
		 * 
		 * Could not generate a wrapper for purple_savedstatuses_get_all in file "savedstatuses.h".
		 * Message: The type could not be resolved (GList * purple_savedstatuses_get_all()).
		 */

		/*
		 * GList * purple_savedstatuses_get_popular(unsigned int how_many)
		 * 
		 * Could not generate a wrapper for purple_savedstatuses_get_popular in file "savedstatuses.h".
		 * Message: The type could not be resolved (GList * purple_savedstatuses_get_popular(unsigned int how_many)).
		 */

		/*
		 * PurpleSavedStatus * purple_savedstatus_get_current()
		 * 
		 * Could not generate a wrapper for purple_savedstatus_get_current in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatus * purple_savedstatus_get_current()).
		 */

		/*
		 * PurpleSavedStatus * purple_savedstatus_get_default()
		 * 
		 * Could not generate a wrapper for purple_savedstatus_get_default in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatus * purple_savedstatus_get_default()).
		 */

		/*
		 * PurpleSavedStatus * purple_savedstatus_get_idleaway()
		 * 
		 * Could not generate a wrapper for purple_savedstatus_get_idleaway in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatus * purple_savedstatus_get_idleaway()).
		 */

		/*
		 * gboolean purple_savedstatus_is_idleaway()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_savedstatus_is_idleaway();

		public static bool SavedstatusIsIdleaway()
		{
			return purple_savedstatus_is_idleaway();
		}

		/*
		 * void purple_savedstatus_set_idleaway(gboolean idleaway)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatus_set_idleaway(bool idleaway);

		public static void SavedstatusSetIdleaway(bool idleaway)
		{
			purple_savedstatus_set_idleaway(idleaway);
		}

		/*
		 * PurpleSavedStatus * purple_savedstatus_get_startup()
		 * 
		 * Could not generate a wrapper for purple_savedstatus_get_startup in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatus * purple_savedstatus_get_startup()).
		 */

		/*
		 * PurpleSavedStatus * purple_savedstatus_find(char * title)
		 * 
		 * Could not generate a wrapper for purple_savedstatus_find in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatus * purple_savedstatus_find(char * title)).
		 */

		/*
		 * PurpleSavedStatus * purple_savedstatus_find_by_creation_time(time_t creation_time)
		 * 
		 * Could not generate a wrapper for purple_savedstatus_find_by_creation_time in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatus * purple_savedstatus_find_by_creation_time(time_t creation_time)).
		 */

		/*
		 * PurpleSavedStatus * purple_savedstatus_find_transient_by_type_and_message(PurpleStatusPrimitive type, char * message)
		 * 
		 * Could not generate a wrapper for purple_savedstatus_find_transient_by_type_and_message in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatus * purple_savedstatus_find_transient_by_type_and_message(PurpleStatusPrimitive type, char * message)).
		 */

		/*
		 * gboolean purple_savedstatus_is_transient(PurpleSavedStatus * saved_status)
		 * 
		 * Could not generate a wrapper for purple_savedstatus_is_transient in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatus * saved_status).
		 */

		/*
		 * char * purple_savedstatus_get_title(PurpleSavedStatus * saved_status)
		 * 
		 * Could not generate a wrapper for purple_savedstatus_get_title in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatus * saved_status).
		 */

		/*
		 * PurpleStatusPrimitive purple_savedstatus_get_type(PurpleSavedStatus * saved_status)
		 * 
		 * Could not generate a wrapper for purple_savedstatus_get_type in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatus * saved_status).
		 */

		/*
		 * char * purple_savedstatus_get_message(PurpleSavedStatus * saved_status)
		 * 
		 * Could not generate a wrapper for purple_savedstatus_get_message in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatus * saved_status).
		 */

		/*
		 * time_t purple_savedstatus_get_creation_time(PurpleSavedStatus * saved_status)
		 * 
		 * Could not generate a wrapper for purple_savedstatus_get_creation_time in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatus * saved_status).
		 */

		/*
		 * gboolean purple_savedstatus_has_substatuses(PurpleSavedStatus * saved_status)
		 * 
		 * Could not generate a wrapper for purple_savedstatus_has_substatuses in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatus * saved_status).
		 */

		/*
		 * PurpleStatusType * purple_savedstatus_substatus_get_type(PurpleSavedStatusSub * substatus)
		 * 
		 * Could not generate a wrapper for purple_savedstatus_substatus_get_type in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleStatusType * purple_savedstatus_substatus_get_type(PurpleSavedStatusSub * substatus)).
		 */

		/*
		 * char * purple_savedstatus_substatus_get_message(PurpleSavedStatusSub * substatus)
		 * 
		 * Could not generate a wrapper for purple_savedstatus_substatus_get_message in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatusSub * substatus).
		 */

		/*
		 * void purple_savedstatus_activate(PurpleSavedStatus * saved_status)
		 * 
		 * Could not generate a wrapper for purple_savedstatus_activate in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatus * saved_status).
		 */

		/*
		 * void purple_savedstatus_activate_for_account(PurpleSavedStatus * saved_status, PurpleAccount * account)
		 * 
		 * Could not generate a wrapper for purple_savedstatus_activate_for_account in file "savedstatuses.h".
		 * Message: The type could not be resolved (PurpleSavedStatus * saved_status).
		 */

		/*
		 * void * purple_savedstatuses_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatuses_get_handle();

		public static IntPtr GetHandle()
		{
			return purple_savedstatuses_get_handle();
		}

		/*
		 * void purple_savedstatuses_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatuses_init();

		public static void Init()
		{
			purple_savedstatuses_init();
		}

		/*
		 * void purple_savedstatuses_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatuses_uninit();

		public static void Uninit()
		{
			purple_savedstatuses_uninit();
		}

	}
}

