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
	public class ThemeManager
	{
		/*
		 * GType purple_theme_manager_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_theme_manager_get_type();

		public static /* libgobject */ IntPtr ThemeManagerGetType()
		{
			/* Unable to process purple_theme_manager_get_type, a GObjectObject. */
			
		}

		/*
		 * void purple_theme_manager_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_manager_init();

		public static void ThemeManagerInit()
		{
			purple_theme_manager_init();
		}

		/*
		 * void purple_theme_manager_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_manager_uninit();

		public static void ThemeManagerUninit()
		{
			purple_theme_manager_uninit();
		}

		/*
		 * void purple_theme_manager_refresh()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_manager_refresh();

		public static void ThemeManagerRefresh()
		{
			purple_theme_manager_refresh();
		}

		/*
		 * PurpleTheme * purple_theme_manager_find_theme(gchar * name, gchar * type)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_theme_manager_find_theme(string name, string type);

		public static PurpleTheme ThemeManagerFindTheme(string name, string type)
		{
			return new PurpleTheme(purple_theme_manager_find_theme(name, type));
		}

		/*
		 * void purple_theme_manager_add_theme(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_manager_add_theme(IntPtr theme);

		public static void ThemeManagerAddTheme(PurpleTheme theme)
		{
			purple_theme_manager_add_theme(theme.Reference);
		}

		/*
		 * void purple_theme_manager_remove_theme(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_manager_remove_theme(IntPtr theme);

		public static void ThemeManagerRemoveTheme(PurpleTheme theme)
		{
			purple_theme_manager_remove_theme(theme.Reference);
		}

		/*
		 * void purple_theme_manager_register_type(PurpleThemeLoader * loader)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_manager_register_type(IntPtr loader);

		public static void ThemeManagerRegisterType(PurpleThemeLoader loader)
		{
			purple_theme_manager_register_type(loader.Reference);
		}

		/*
		 * void purple_theme_manager_unregister_type(PurpleThemeLoader * loader)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_manager_unregister_type(IntPtr loader);

		public static void ThemeManagerUnregisterType(PurpleThemeLoader loader)
		{
			purple_theme_manager_unregister_type(loader.Reference);
		}

		/*
		 * void purple_theme_manager_for_each_theme(PTFunc func)
		 * 
		 * Could not generate a wrapper for purple_theme_manager_for_each_theme in file "theme-manager.h".
		 * Message: The type could not be resolved (PTFunc func).
		 */

	}
}

