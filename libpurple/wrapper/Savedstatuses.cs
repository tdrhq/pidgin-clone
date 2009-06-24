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
	public class Savedstatuses
	{
		/*
		 * PurpleSavedStatus * purple_savedstatus_new(char * title, PurpleStatusPrimitive type)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_new(string title, UNKNOWN type);

		public static PurpleSavedStatus SavedstatusNew(string title, PurpleStatusPrimitive type)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_savedstatus_set_title(PurpleSavedStatus * status, char * title)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatus_set_title(IntPtr status, string title);

		public static void SavedstatusSetTitle(PurpleSavedStatus status, string title)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_savedstatus_set_type(PurpleSavedStatus * status, PurpleStatusPrimitive type)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatus_set_type(IntPtr status, UNKNOWN type);

		public static void SavedstatusSetType(PurpleSavedStatus status, PurpleStatusPrimitive type)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_savedstatus_set_message(PurpleSavedStatus * status, char * message)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatus_set_message(IntPtr status, string message);

		public static void SavedstatusSetMessage(PurpleSavedStatus status, string message)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_savedstatus_set_substatus(PurpleSavedStatus * status, PurpleAccount * account, PurpleStatusType * type, char * message)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatus_set_substatus(IntPtr status, IntPtr account, IntPtr type, string message);

		public static void SavedstatusSetSubstatus(PurpleSavedStatus status, PurpleAccount account, PurpleStatusType type, string message)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_savedstatus_unset_substatus(PurpleSavedStatus * saved_status, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatus_unset_substatus(IntPtr saved_status, IntPtr account);

		public static void SavedstatusUnsetSubstatus(PurpleSavedStatus saved_status, PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_savedstatus_delete(char * title)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_savedstatus_delete(string title);

		public static bool SavedstatusDelete(string title)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_savedstatus_delete_by_status(PurpleSavedStatus * saved_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatus_delete_by_status(IntPtr saved_status);

		public static void SavedstatusDeleteByStatus(PurpleSavedStatus saved_status)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_savedstatuses_get_all()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatuses_get_all();

		public static GList GetAll()
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_savedstatuses_get_popular(unsigned int)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatuses_get_popular(UNKNOWN int);

		public static GList GetPopular(unsigned int)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleSavedStatus * purple_savedstatus_get_current()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_get_current();

		public static PurpleSavedStatus SavedstatusGetCurrent()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleSavedStatus * purple_savedstatus_get_default()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_get_default();

		public static PurpleSavedStatus SavedstatusGetDefault()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleSavedStatus * purple_savedstatus_get_idleaway()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_get_idleaway();

		public static PurpleSavedStatus SavedstatusGetIdleaway()
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_savedstatus_is_idleaway()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_savedstatus_is_idleaway();

		public static bool SavedstatusIsIdleaway()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_savedstatus_set_idleaway(gboolean idleaway)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatus_set_idleaway(bool idleaway);

		public static void SavedstatusSetIdleaway(bool idleaway)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleSavedStatus * purple_savedstatus_get_startup()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_get_startup();

		public static PurpleSavedStatus SavedstatusGetStartup()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleSavedStatus * purple_savedstatus_find(char * title)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_find(string title);

		public static PurpleSavedStatus SavedstatusFind(string title)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleSavedStatus * purple_savedstatus_find_by_creation_time(time_t creation_time)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_find_by_creation_time(UNKNOWN creation_time);

		public static PurpleSavedStatus SavedstatusFindByCreationTime(time_t creation_time)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleSavedStatus * purple_savedstatus_find_transient_by_type_and_message(PurpleStatusPrimitive type, char * message)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_find_transient_by_type_and_message(UNKNOWN type, string message);

		public static PurpleSavedStatus SavedstatusFindTransientByTypeAndMessage(PurpleStatusPrimitive type, string message)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_savedstatus_is_transient(PurpleSavedStatus * saved_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_savedstatus_is_transient(IntPtr saved_status);

		public static bool SavedstatusIsTransient(PurpleSavedStatus saved_status)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_savedstatus_get_title(PurpleSavedStatus * saved_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_savedstatus_get_title(IntPtr saved_status);

		public static string SavedstatusGetTitle(PurpleSavedStatus saved_status)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatusPrimitive purple_savedstatus_get_type(PurpleSavedStatus * saved_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_savedstatus_get_type(IntPtr saved_status);

		public static PurpleStatusPrimitive SavedstatusGetType(PurpleSavedStatus saved_status)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_savedstatus_get_message(PurpleSavedStatus * saved_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_savedstatus_get_message(IntPtr saved_status);

		public static string SavedstatusGetMessage(PurpleSavedStatus saved_status)
		{
			throw new NotImplementedException();
		}

		/*
		 * time_t purple_savedstatus_get_creation_time(PurpleSavedStatus * saved_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_savedstatus_get_creation_time(IntPtr saved_status);

		public static time_t SavedstatusGetCreationTime(PurpleSavedStatus saved_status)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_savedstatus_has_substatuses(PurpleSavedStatus * saved_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_savedstatus_has_substatuses(IntPtr saved_status);

		public static bool SavedstatusHasSubstatuses(PurpleSavedStatus saved_status)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleSavedStatusSub * purple_savedstatus_get_substatus(PurpleSavedStatus * saved_status, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_get_substatus(IntPtr saved_status, IntPtr account);

		public static PurpleSavedStatusSub SavedstatusGetSubstatus(PurpleSavedStatus saved_status, PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatusType * purple_savedstatus_substatus_get_type(PurpleSavedStatusSub * substatus)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatus_substatus_get_type(IntPtr substatus);

		public static PurpleStatusType SavedstatusSubstatusGetType(PurpleSavedStatusSub substatus)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_savedstatus_substatus_get_message(PurpleSavedStatusSub * substatus)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_savedstatus_substatus_get_message(IntPtr substatus);

		public static string SavedstatusSubstatusGetMessage(PurpleSavedStatusSub substatus)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_savedstatus_activate(PurpleSavedStatus * saved_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatus_activate(IntPtr saved_status);

		public static void SavedstatusActivate(PurpleSavedStatus saved_status)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_savedstatus_activate_for_account(PurpleSavedStatus * saved_status, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatus_activate_for_account(IntPtr saved_status, IntPtr account);

		public static void SavedstatusActivateForAccount(PurpleSavedStatus saved_status, PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_savedstatuses_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_savedstatuses_get_handle();

		public static IntPtr GetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_savedstatuses_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatuses_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_savedstatuses_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_savedstatuses_uninit();

		public static void Uninit()
		{
			throw new NotImplementedException();
		}

	}
}

