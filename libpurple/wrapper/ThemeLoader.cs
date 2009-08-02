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
	 * File: theme-loader.h
	 */
	public class ThemeLoader
	{

		/*
		 * Function Name: purple_theme_loader_get_type
		 * Function Type: GType
		 * Function RawType: GType
		 * Function Category: GObjectObject
		 */
		/*
		 * GType purple_theme_loader_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_theme_loader_get_type();

		public static /* libobject */ IntPtr ThemeLoaderGetType()
		{
			/* Unable to process the return value of purple_theme_loader_get_type, a GObjectObject. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_theme_loader_get_type_string
		 * Function Type: gchar *
		 * Function RawType: gchar
		 * Function Category: Native
		 */
		/*
		 * gchar * purple_theme_loader_get_type_string(PurpleThemeLoader * self)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_theme_loader_get_type_string(IntPtr self);

		public static string ThemeLoaderGetTypeString(PurpleThemeLoader self)
		{
			return purple_theme_loader_get_type_string(self.Reference);
		}

		/*
		 * Function Name: purple_theme_loader_build
		 * Function Type: PurpleTheme *
		 * Function RawType: PurpleTheme
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleTheme * purple_theme_loader_build(PurpleThemeLoader * loader, gchar * dir)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_theme_loader_build(IntPtr loader, string dir);

		public static PurpleTheme ThemeLoaderBuild(PurpleThemeLoader loader, string dir)
		{
			return new PurpleTheme(purple_theme_loader_build(loader.Reference, dir));
		}

	}
}

