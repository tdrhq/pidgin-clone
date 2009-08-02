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
	 * File: prefs.h
	 */
	public class Prefs
	{
		public enum PurplePrefType
		{
			PURPLE_PREF_NONE,
			PURPLE_PREF_BOOLEAN,
			PURPLE_PREF_INT,
			PURPLE_PREF_STRING,
			PURPLE_PREF_STRING_LIST,
			PURPLE_PREF_PATH,
			PURPLE_PREF_PATH_LIST
		};

		public delegate void PurplePrefCallback(string name, Prefs.PurplePrefType type, IntPtr val, IntPtr data);

		/*
		 * Function Name: purple_prefs_get_handle
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
		 */
		/*
		 * void * purple_prefs_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_prefs_get_handle();

		public static IntPtr GetHandle()
		{
			return purple_prefs_get_handle();
		}

		/*
		 * Function Name: purple_prefs_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_init();

		public static void Init()
		{
			purple_prefs_init();
		}

		/*
		 * Function Name: purple_prefs_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_uninit();

		public static void Uninit()
		{
			purple_prefs_uninit();
		}

		/*
		 * Function Name: purple_prefs_add_none
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_add_none(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_add_none(string name);

		public static void AddNone(string name)
		{
			purple_prefs_add_none(name);
		}

		/*
		 * Function Name: purple_prefs_add_bool
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_add_bool(char * name, gboolean value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_add_bool(string name, bool value);

		public static void AddBool(string name, bool value)
		{
			purple_prefs_add_bool(name, value);
		}

		/*
		 * Function Name: purple_prefs_add_int
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_add_int(char * name, int value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_add_int(string name, int value);

		public static void AddInt(string name, int value)
		{
			purple_prefs_add_int(name, value);
		}

		/*
		 * Function Name: purple_prefs_add_string
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_add_string(char * name, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_add_string(string name, string value);

		public static void AddString(string name, string value)
		{
			purple_prefs_add_string(name, value);
		}

		/*
		 * Function Name: purple_prefs_add_string_list
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_add_string_list(char * name, GList * value)
		 * 
		 * Could not generate a wrapper for purple_prefs_add_string_list in file "prefs.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Function Name: purple_prefs_add_path
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_add_path(char * name, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_add_path(string name, string value);

		public static void AddPath(string name, string value)
		{
			purple_prefs_add_path(name, value);
		}

		/*
		 * Function Name: purple_prefs_add_path_list
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_add_path_list(char * name, GList * value)
		 * 
		 * Could not generate a wrapper for purple_prefs_add_path_list in file "prefs.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Function Name: purple_prefs_remove
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_remove(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_remove(string name);

		public static void Remove(string name)
		{
			purple_prefs_remove(name);
		}

		/*
		 * Function Name: purple_prefs_rename
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_rename(char * oldname, char * newname)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_rename(string oldname, string newname);

		public static void Rename(string oldname, string newname)
		{
			purple_prefs_rename(oldname, newname);
		}

		/*
		 * Function Name: purple_prefs_rename_boolean_toggle
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_rename_boolean_toggle(char * oldname, char * newname)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_rename_boolean_toggle(string oldname, string newname);

		public static void RenameBooleanToggle(string oldname, string newname)
		{
			purple_prefs_rename_boolean_toggle(oldname, newname);
		}

		/*
		 * Function Name: purple_prefs_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_destroy()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_destroy();

		public static void Destroy()
		{
			purple_prefs_destroy();
		}

		/*
		 * Function Name: purple_prefs_set_generic
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_set_generic(char * name, gpointer value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_set_generic(string name, IntPtr value);

		public static void SetGeneric(string name, IntPtr value)
		{
			purple_prefs_set_generic(name, value);
		}

		/*
		 * Function Name: purple_prefs_set_bool
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_set_bool(char * name, gboolean value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_set_bool(string name, bool value);

		public static void SetBool(string name, bool value)
		{
			purple_prefs_set_bool(name, value);
		}

		/*
		 * Function Name: purple_prefs_set_int
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_set_int(char * name, int value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_set_int(string name, int value);

		public static void SetInt(string name, int value)
		{
			purple_prefs_set_int(name, value);
		}

		/*
		 * Function Name: purple_prefs_set_string
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_set_string(char * name, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_set_string(string name, string value);

		public static void SetString(string name, string value)
		{
			purple_prefs_set_string(name, value);
		}

		/*
		 * Function Name: purple_prefs_set_string_list
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_set_string_list(char * name, GList * value)
		 * 
		 * Could not generate a wrapper for purple_prefs_set_string_list in file "prefs.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Function Name: purple_prefs_set_path
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_set_path(char * name, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_set_path(string name, string value);

		public static void SetPath(string name, string value)
		{
			purple_prefs_set_path(name, value);
		}

		/*
		 * Function Name: purple_prefs_set_path_list
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_set_path_list(char * name, GList * value)
		 * 
		 * Could not generate a wrapper for purple_prefs_set_path_list in file "prefs.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Function Name: purple_prefs_exists
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_prefs_exists(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_prefs_exists(string name);

		public static bool Exists(string name)
		{
			return purple_prefs_exists(name);
		}

		/*
		 * Function Name: purple_prefs_get_type
		 * Function Type: PurplePrefType
		 * Function RawType: PurplePrefType
		 * Function Category: KnownEnum
		 */
		/*
		 * PurplePrefType purple_prefs_get_type(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern Prefs.PurplePrefType purple_prefs_get_type(string name);

		public static Prefs.PurplePrefType GetType(string name)
		{
			/* Unable to process the return value of purple_prefs_get_type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_prefs_get_bool
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_prefs_get_bool(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_prefs_get_bool(string name);

		public static bool GetBool(string name)
		{
			return purple_prefs_get_bool(name);
		}

		/*
		 * Function Name: purple_prefs_get_int
		 * Function Type: int
		 * Function RawType: int
		 * Function Category: Native
		 */
		/*
		 * int purple_prefs_get_int(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_prefs_get_int(string name);

		public static int GetInt(string name)
		{
			return purple_prefs_get_int(name);
		}

		/*
		 * Function Name: purple_prefs_get_string
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_prefs_get_string(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_prefs_get_string(string name);

		public static string GetString(string name)
		{
			return purple_prefs_get_string(name);
		}

		/*
		 * Function Name: purple_prefs_get_string_list
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_prefs_get_string_list(char * name)
		 * 
		 * Could not generate a wrapper for purple_prefs_get_string_list in file "prefs.h".
		 * Message: Unknown inner-type of argument 0 of purple_prefs_get_string_list
		 */

		/*
		 * Function Name: purple_prefs_get_path
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_prefs_get_path(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_prefs_get_path(string name);

		public static string GetPath(string name)
		{
			return purple_prefs_get_path(name);
		}

		/*
		 * Function Name: purple_prefs_get_path_list
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_prefs_get_path_list(char * name)
		 * 
		 * Could not generate a wrapper for purple_prefs_get_path_list in file "prefs.h".
		 * Message: Unknown inner-type of argument 0 of purple_prefs_get_path_list
		 */

		/*
		 * Function Name: purple_prefs_get_children_names
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_prefs_get_children_names(char * name)
		 * 
		 * Could not generate a wrapper for purple_prefs_get_children_names in file "prefs.h".
		 * Message: Unknown inner-type of argument 0 of purple_prefs_get_children_names
		 */

		/*
		 * Function Name: purple_prefs_disconnect_callback
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_disconnect_callback(guint callback_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_disconnect_callback(uint callback_id);

		public static void DisconnectCallback(uint callback_id)
		{
			purple_prefs_disconnect_callback(callback_id);
		}

		/*
		 * Function Name: purple_prefs_disconnect_by_handle
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_disconnect_by_handle(void * handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_disconnect_by_handle(IntPtr handle);

		public static void DisconnectByHandle(IntPtr handle)
		{
			purple_prefs_disconnect_by_handle(handle);
		}

		/*
		 * Function Name: purple_prefs_trigger_callback
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_trigger_callback(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_trigger_callback(string name);

		public static void TriggerCallback(string name)
		{
			purple_prefs_trigger_callback(name);
		}

		/*
		 * Function Name: purple_prefs_load
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_prefs_load()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_prefs_load();

		public static bool Load()
		{
			return purple_prefs_load();
		}

		/*
		 * Function Name: purple_prefs_update_old
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_prefs_update_old()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_update_old();

		public static void UpdateOld()
		{
			purple_prefs_update_old();
		}

	}
}

