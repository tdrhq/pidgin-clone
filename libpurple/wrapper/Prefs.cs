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
	public class Prefs
	{
		/*
		 * void * purple_prefs_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_prefs_get_handle();

		public static IntPtr GetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_uninit();

		public static void Uninit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_add_none(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_add_none(string name);

		public static void AddNone(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_add_bool(char * name, gboolean value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_add_bool(string name, bool value);

		public static void AddBool(string name, bool value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_add_int(char * name, int value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_add_int(string name, int value);

		public static void AddInt(string name, int value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_add_string(char * name, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_add_string(string name, string value);

		public static void AddString(string name, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_add_string_list(char * name, GList * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_add_string_list(string name, IntPtr value);

		public static void AddStringList(string name, GList value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_add_path(char * name, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_add_path(string name, string value);

		public static void AddPath(string name, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_add_path_list(char * name, GList * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_add_path_list(string name, IntPtr value);

		public static void AddPathList(string name, GList value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_remove(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_remove(string name);

		public static void Remove(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_rename(char * oldname, char * newname)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_rename(string oldname, string newname);

		public static void Rename(string oldname, string newname)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_rename_boolean_toggle(char * oldname, char * newname)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_rename_boolean_toggle(string oldname, string newname);

		public static void RenameBooleanToggle(string oldname, string newname)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_destroy()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_destroy();

		public static void Destroy()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_set_generic(char * name, gpointer value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_set_generic(string name, IntPtr value);

		public static void SetGeneric(string name, IntPtr value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_set_bool(char * name, gboolean value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_set_bool(string name, bool value);

		public static void SetBool(string name, bool value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_set_int(char * name, int value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_set_int(string name, int value);

		public static void SetInt(string name, int value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_set_string(char * name, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_set_string(string name, string value);

		public static void SetString(string name, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_set_string_list(char * name, GList * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_set_string_list(string name, IntPtr value);

		public static void SetStringList(string name, GList value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_set_path(char * name, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_set_path(string name, string value);

		public static void SetPath(string name, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_set_path_list(char * name, GList * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_set_path_list(string name, IntPtr value);

		public static void SetPathList(string name, GList value)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_prefs_exists(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_prefs_exists(string name);

		public static bool Exists(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePrefType purple_prefs_get_type(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_prefs_get_type(string name);

		public static PurplePrefType GetType(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_prefs_get_bool(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_prefs_get_bool(string name);

		public static bool GetBool(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_prefs_get_int(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_prefs_get_int(string name);

		public static int GetInt(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_prefs_get_string(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_prefs_get_string(string name);

		public static string GetString(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_prefs_get_string_list(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_prefs_get_string_list(string name);

		public static GList GetStringList(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_prefs_get_path(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_prefs_get_path(string name);

		public static string GetPath(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_prefs_get_path_list(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_prefs_get_path_list(string name);

		public static GList GetPathList(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_prefs_get_children_names(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_prefs_get_children_names(string name);

		public static GList GetChildrenNames(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint purple_prefs_connect_callback(void * handle, char * name, PurplePrefCallback cb, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_prefs_connect_callback(IntPtr handle, string name, UNKNOWN cb, IntPtr data);

		public static uint ConnectCallback(IntPtr handle, string name, PurplePrefCallback cb, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_disconnect_callback(guint callback_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_disconnect_callback(uint callback_id);

		public static void DisconnectCallback(uint callback_id)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_disconnect_by_handle(void * handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_disconnect_by_handle(IntPtr handle);

		public static void DisconnectByHandle(IntPtr handle)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_trigger_callback(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_trigger_callback(string name);

		public static void TriggerCallback(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_prefs_load()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_prefs_load();

		public static bool Load()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prefs_update_old()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prefs_update_old();

		public static void UpdateOld()
		{
			throw new NotImplementedException();
		}

	}
}

