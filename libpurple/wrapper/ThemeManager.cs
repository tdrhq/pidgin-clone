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
	public class ThemeManager
	{
		/*
		 * GType purple_theme_manager_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_theme_manager_get_type();

		public static GType ThemeManagerGetType()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_theme_manager_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_manager_init();

		public static void ThemeManagerInit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_theme_manager_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_manager_uninit();

		public static void ThemeManagerUninit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_theme_manager_refresh()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_manager_refresh();

		public static void ThemeManagerRefresh()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleTheme * purple_theme_manager_find_theme(gchar * name, gchar * type)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_theme_manager_find_theme(string name, string type);

		public static PurpleTheme ThemeManagerFindTheme(string name, string type)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_theme_manager_add_theme(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_manager_add_theme(IntPtr theme);

		public static void ThemeManagerAddTheme(PurpleTheme theme)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_theme_manager_remove_theme(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_manager_remove_theme(IntPtr theme);

		public static void ThemeManagerRemoveTheme(PurpleTheme theme)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_theme_manager_register_type(PurpleThemeLoader * loader)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_manager_register_type(IntPtr loader);

		public static void ThemeManagerRegisterType(PurpleThemeLoader loader)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_theme_manager_unregister_type(PurpleThemeLoader * loader)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_manager_unregister_type(IntPtr loader);

		public static void ThemeManagerUnregisterType(PurpleThemeLoader loader)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_theme_manager_for_each_theme(PTFunc func)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_manager_for_each_theme(UNKNOWN func);

		public static void ThemeManagerForEachTheme(PTFunc func)
		{
			throw new NotImplementedException();
		}

	}
}

