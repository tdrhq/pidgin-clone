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
	public class Theme
	{
		/*
		 * GType purple_theme_get_type()
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_theme_get_type();

		public static GType GetType()
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_theme_get_name(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_theme_get_name(IntPtr theme);

		public static string GetName(PurpleTheme theme)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_theme_set_name(PurpleTheme * theme, gchar * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_set_name(IntPtr theme, string name);

		public static void SetName(PurpleTheme theme, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_theme_get_description(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_theme_get_description(IntPtr theme);

		public static string GetDescription(PurpleTheme theme)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_theme_set_description(PurpleTheme * theme, gchar * description)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_set_description(IntPtr theme, string description);

		public static void SetDescription(PurpleTheme theme, string description)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_theme_get_author(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_theme_get_author(IntPtr theme);

		public static string GetAuthor(PurpleTheme theme)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_theme_set_author(PurpleTheme * theme, gchar * author)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_set_author(IntPtr theme, string author);

		public static void SetAuthor(PurpleTheme theme, string author)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_theme_get_type_string(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_theme_get_type_string(IntPtr theme);

		public static string GetTypeString(PurpleTheme theme)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_theme_get_dir(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_theme_get_dir(IntPtr theme);

		public static string GetDir(PurpleTheme theme)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_theme_set_dir(PurpleTheme * theme, gchar * dir)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_set_dir(IntPtr theme, string dir);

		public static void SetDir(PurpleTheme theme, string dir)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_theme_get_image(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_theme_get_image(IntPtr theme);

		public static string GetImage(PurpleTheme theme)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_theme_get_image_full(PurpleTheme * theme)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_theme_get_image_full(IntPtr theme);

		public static string GetImageFull(PurpleTheme theme)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_theme_set_image(PurpleTheme * theme, gchar * img)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_theme_set_image(IntPtr theme, string img);

		public static void SetImage(PurpleTheme theme, string img)
		{
			throw new NotImplementedException();
		}

	}
}

