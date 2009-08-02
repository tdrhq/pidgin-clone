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

	/*
	 * File: savedstatuses.h
	 */
	public class Savedstatuses
	{

		/*
		 * Function Name: purple_savedstatus_delete
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
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
		 * Function Name: purple_savedstatus_delete_by_status
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_savedstatus_delete_by_status(PurpleSavedStatus * saved_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatus_delete_by_status(IntPtr saved_status);

		public static void SavedstatusDeleteByStatus(/* Resolved as PointerToUnknownStruct to PurpleSavedStatus */ IntPtr saved_status)
		{
			/* Unable to process saved_status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatuses_get_all
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_savedstatuses_get_all()
		 * 
		 * Could not generate a wrapper for purple_savedstatuses_get_all in file "savedstatuses.h".
		 * Message: Unknown inner-type of argument 0 of purple_savedstatuses_get_all
		 */

		/*
		 * Function Name: purple_savedstatuses_get_popular
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_savedstatuses_get_popular(unsigned int how_many)
		 * 
		 * Could not generate a wrapper for purple_savedstatuses_get_popular in file "savedstatuses.h".
		 * Message: Unknown inner-type of argument 0 of purple_savedstatuses_get_popular
		 */

		/*
		 * Function Name: purple_savedstatus_get_current
		 * Function Type: PurpleSavedStatus *
		 * Function RawType: PurpleSavedStatus
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleSavedStatus * purple_savedstatus_get_current()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_get_current();

		public static /* Resolved as PointerToUnknownStruct to PurpleSavedStatus */ IntPtr SavedstatusGetCurrent()
		{
			/* Unable to process the return value of purple_savedstatus_get_current, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatus_get_default
		 * Function Type: PurpleSavedStatus *
		 * Function RawType: PurpleSavedStatus
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleSavedStatus * purple_savedstatus_get_default()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_get_default();

		public static /* Resolved as PointerToUnknownStruct to PurpleSavedStatus */ IntPtr SavedstatusGetDefault()
		{
			/* Unable to process the return value of purple_savedstatus_get_default, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatus_get_idleaway
		 * Function Type: PurpleSavedStatus *
		 * Function RawType: PurpleSavedStatus
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleSavedStatus * purple_savedstatus_get_idleaway()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_get_idleaway();

		public static /* Resolved as PointerToUnknownStruct to PurpleSavedStatus */ IntPtr SavedstatusGetIdleaway()
		{
			/* Unable to process the return value of purple_savedstatus_get_idleaway, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatus_is_idleaway
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
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
		 * Function Name: purple_savedstatus_set_idleaway
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_savedstatus_get_startup
		 * Function Type: PurpleSavedStatus *
		 * Function RawType: PurpleSavedStatus
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleSavedStatus * purple_savedstatus_get_startup()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_get_startup();

		public static /* Resolved as PointerToUnknownStruct to PurpleSavedStatus */ IntPtr SavedstatusGetStartup()
		{
			/* Unable to process the return value of purple_savedstatus_get_startup, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatus_find
		 * Function Type: PurpleSavedStatus *
		 * Function RawType: PurpleSavedStatus
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleSavedStatus * purple_savedstatus_find(char * title)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_find(string title);

		public static /* Resolved as PointerToUnknownStruct to PurpleSavedStatus */ IntPtr SavedstatusFind(string title)
		{
			/* Unable to process the return value of purple_savedstatus_find, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatus_find_by_creation_time
		 * Function Type: PurpleSavedStatus *
		 * Function RawType: PurpleSavedStatus
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleSavedStatus * purple_savedstatus_find_by_creation_time(time_t creation_time)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_find_by_creation_time(ulong creation_time);

		public static /* Resolved as PointerToUnknownStruct to PurpleSavedStatus */ IntPtr SavedstatusFindByCreationTime(DateTime creation_time)
		{
			ulong _PurpleWrapper_param0 = (ulong)(creation_time - new DateTime(1970, 1, 1)).TotalSeconds;
			/* Unable to process the return value of purple_savedstatus_find_by_creation_time, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatus_find_transient_by_type_and_message
		 * Function Type: PurpleSavedStatus *
		 * Function RawType: PurpleSavedStatus
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleSavedStatus * purple_savedstatus_find_transient_by_type_and_message(PurpleStatusPrimitive type, char * message)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_find_transient_by_type_and_message(Status.PurpleStatusPrimitive type, string message);

		public static /* Resolved as PointerToUnknownStruct to PurpleSavedStatus */ IntPtr SavedstatusFindTransientByTypeAndMessage(Status.PurpleStatusPrimitive type, string message)
		{
			/* Unable to process type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatus_is_transient
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_savedstatus_is_transient(PurpleSavedStatus * saved_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_savedstatus_is_transient(IntPtr saved_status);

		public static bool SavedstatusIsTransient(/* Resolved as PointerToUnknownStruct to PurpleSavedStatus */ IntPtr saved_status)
		{
			/* Unable to process saved_status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatus_get_title
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_savedstatus_get_title(PurpleSavedStatus * saved_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_savedstatus_get_title(IntPtr saved_status);

		public static string SavedstatusGetTitle(/* Resolved as PointerToUnknownStruct to PurpleSavedStatus */ IntPtr saved_status)
		{
			/* Unable to process saved_status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatus_get_type
		 * Function Type: PurpleStatusPrimitive
		 * Function RawType: PurpleStatusPrimitive
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleStatusPrimitive purple_savedstatus_get_type(PurpleSavedStatus * saved_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern Status.PurpleStatusPrimitive purple_savedstatus_get_type(IntPtr saved_status);

		public static Status.PurpleStatusPrimitive SavedstatusGetType(/* Resolved as PointerToUnknownStruct to PurpleSavedStatus */ IntPtr saved_status)
		{
			/* Unable to process saved_status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatus_get_message
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_savedstatus_get_message(PurpleSavedStatus * saved_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_savedstatus_get_message(IntPtr saved_status);

		public static string SavedstatusGetMessage(/* Resolved as PointerToUnknownStruct to PurpleSavedStatus */ IntPtr saved_status)
		{
			/* Unable to process saved_status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatus_get_creation_time
		 * Function Type: time_t
		 * Function RawType: time_t
		 * Function Category: DateTime
		 */
		/*
		 * time_t purple_savedstatus_get_creation_time(PurpleSavedStatus * saved_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_savedstatus_get_creation_time(IntPtr saved_status);

		public static DateTime SavedstatusGetCreationTime(/* Resolved as PointerToUnknownStruct to PurpleSavedStatus */ IntPtr saved_status)
		{
			/* Unable to process saved_status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatus_has_substatuses
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_savedstatus_has_substatuses(PurpleSavedStatus * saved_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_savedstatus_has_substatuses(IntPtr saved_status);

		public static bool SavedstatusHasSubstatuses(/* Resolved as PointerToUnknownStruct to PurpleSavedStatus */ IntPtr saved_status)
		{
			/* Unable to process saved_status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatus_substatus_get_type
		 * Function Type: PurpleStatusType *
		 * Function RawType: PurpleStatusType
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleStatusType * purple_savedstatus_substatus_get_type(PurpleSavedStatusSub * substatus)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_substatus_get_type(IntPtr substatus);

		public static /* Resolved as PointerToUnknownStruct to PurpleStatusType */ IntPtr SavedstatusSubstatusGetType(/* Resolved as PointerToUnknownStruct to PurpleSavedStatusSub */ IntPtr substatus)
		{
			/* Unable to process substatus, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatus_substatus_get_message
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_savedstatus_substatus_get_message(PurpleSavedStatusSub * substatus)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_savedstatus_substatus_get_message(IntPtr substatus);

		public static string SavedstatusSubstatusGetMessage(/* Resolved as PointerToUnknownStruct to PurpleSavedStatusSub */ IntPtr substatus)
		{
			/* Unable to process substatus, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatus_activate
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_savedstatus_activate(PurpleSavedStatus * saved_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatus_activate(IntPtr saved_status);

		public static void SavedstatusActivate(/* Resolved as PointerToUnknownStruct to PurpleSavedStatus */ IntPtr saved_status)
		{
			/* Unable to process saved_status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatus_activate_for_account
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_savedstatus_activate_for_account(PurpleSavedStatus * saved_status, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatus_activate_for_account(IntPtr saved_status, IntPtr account);

		public static void SavedstatusActivateForAccount(/* Resolved as PointerToUnknownStruct to PurpleSavedStatus */ IntPtr saved_status, PurpleAccount account)
		{
			/* Unable to process saved_status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_savedstatuses_get_handle
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
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
		 * Function Name: purple_savedstatuses_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_savedstatuses_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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

