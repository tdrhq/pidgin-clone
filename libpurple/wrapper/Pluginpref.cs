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
	public class Pluginpref
	{
		/*
		 * PurplePluginPrefFrame * purple_plugin_pref_frame_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_pref_frame_new();

		public static PurplePluginPrefFrame PluginPrefFrameNew()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_pref_frame_destroy(PurplePluginPrefFrame * frame)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_frame_destroy(IntPtr frame);

		public static void PluginPrefFrameDestroy(PurplePluginPrefFrame frame)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_pref_frame_add(PurplePluginPrefFrame * frame, PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_frame_add(IntPtr frame, IntPtr pref);

		public static void PluginPrefFrameAdd(PurplePluginPrefFrame frame, PurplePluginPref pref)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_plugin_pref_frame_get_prefs(PurplePluginPrefFrame * frame)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_pref_frame_get_prefs(IntPtr frame);

		public static GList PluginPrefFrameGetPrefs(PurplePluginPrefFrame frame)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePluginPref * purple_plugin_pref_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_pref_new();

		public static PurplePluginPref PluginPrefNew()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePluginPref * purple_plugin_pref_new_with_name(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_pref_new_with_name(string name);

		public static PurplePluginPref PluginPrefNewWithName(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePluginPref * purple_plugin_pref_new_with_label(char * label)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_pref_new_with_label(string label);

		public static PurplePluginPref PluginPrefNewWithLabel(string label)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePluginPref * purple_plugin_pref_new_with_name_and_label(char * name, char * label)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_pref_new_with_name_and_label(string name, string label);

		public static PurplePluginPref PluginPrefNewWithNameAndLabel(string name, string label)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_pref_destroy(PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_destroy(IntPtr pref);

		public static void PluginPrefDestroy(PurplePluginPref pref)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_pref_set_name(PurplePluginPref * pref, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_set_name(IntPtr pref, string name);

		public static void PluginPrefSetName(PurplePluginPref pref, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_plugin_pref_get_name(PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_pref_get_name(IntPtr pref);

		public static string PluginPrefGetName(PurplePluginPref pref)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_pref_set_label(PurplePluginPref * pref, char * label)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_set_label(IntPtr pref, string label);

		public static void PluginPrefSetLabel(PurplePluginPref pref, string label)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_plugin_pref_get_label(PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_pref_get_label(IntPtr pref);

		public static string PluginPrefGetLabel(PurplePluginPref pref)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_pref_set_bounds(PurplePluginPref * pref, int min, int max)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_set_bounds(IntPtr pref, int min, int max);

		public static void PluginPrefSetBounds(PurplePluginPref pref, int min, int max)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_pref_get_bounds(PurplePluginPref * pref, int * min, int * max)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_get_bounds(IntPtr pref, IntPtr min, IntPtr max);

		public static void PluginPrefGetBounds(PurplePluginPref pref, int min, int max)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_pref_set_type(PurplePluginPref * pref, PurplePluginPrefType type)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_set_type(IntPtr pref, UNKNOWN type);

		public static void PluginPrefSetType(PurplePluginPref pref, PurplePluginPrefType type)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePluginPrefType purple_plugin_pref_get_type(PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_plugin_pref_get_type(IntPtr pref);

		public static PurplePluginPrefType PluginPrefGetType(PurplePluginPref pref)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_pref_add_choice(PurplePluginPref * pref, char * label, gpointer choice)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_add_choice(IntPtr pref, string label, IntPtr choice);

		public static void PluginPrefAddChoice(PurplePluginPref pref, string label, IntPtr choice)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_plugin_pref_get_choices(PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_pref_get_choices(IntPtr pref);

		public static GList PluginPrefGetChoices(PurplePluginPref pref)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_pref_set_max_length(PurplePluginPref * pref, unsigned int)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_set_max_length(IntPtr pref, UNKNOWN int);

		public static void PluginPrefSetMaxLength(PurplePluginPref pref, unsigned int)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_plugin_pref_get_max_length(PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_plugin_pref_get_max_length(IntPtr pref);

		public static int PluginPrefGetMaxLength(PurplePluginPref pref)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_pref_set_masked(PurplePluginPref * pref, gboolean mask)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_set_masked(IntPtr pref, bool mask);

		public static void PluginPrefSetMasked(PurplePluginPref pref, bool mask)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_plugin_pref_get_masked(PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugin_pref_get_masked(IntPtr pref);

		public static bool PluginPrefGetMasked(PurplePluginPref pref)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_pref_set_format_type(PurplePluginPref * pref, PurpleStringFormatType format)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_pref_set_format_type(IntPtr pref, UNKNOWN format);

		public static void PluginPrefSetFormatType(PurplePluginPref pref, PurpleStringFormatType format)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStringFormatType purple_plugin_pref_get_format_type(PurplePluginPref * pref)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_plugin_pref_get_format_type(IntPtr pref);

		public static PurpleStringFormatType PluginPrefGetFormatType(PurplePluginPref pref)
		{
			throw new NotImplementedException();
		}

	}
}

