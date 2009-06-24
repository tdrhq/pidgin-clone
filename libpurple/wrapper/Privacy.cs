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
	public class Privacy
	{
		/*
		 * gboolean purple_privacy_permit_add(PurpleAccount * account, char * name, gboolean local_only)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_privacy_permit_add(IntPtr account, string name, bool local_only);

		public static bool PermitAdd(PurpleAccount account, string name, bool local_only)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_privacy_permit_remove(PurpleAccount * account, char * name, gboolean local_only)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_privacy_permit_remove(IntPtr account, string name, bool local_only);

		public static bool PermitRemove(PurpleAccount account, string name, bool local_only)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_privacy_deny_add(PurpleAccount * account, char * name, gboolean local_only)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_privacy_deny_add(IntPtr account, string name, bool local_only);

		public static bool DenyAdd(PurpleAccount account, string name, bool local_only)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_privacy_deny_remove(PurpleAccount * account, char * name, gboolean local_only)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_privacy_deny_remove(IntPtr account, string name, bool local_only);

		public static bool DenyRemove(PurpleAccount account, string name, bool local_only)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_privacy_allow(PurpleAccount * account, char * who, gboolean local, gboolean restore)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_privacy_allow(IntPtr account, string who, bool local, bool restore);

		public static void Allow(PurpleAccount account, string who, bool local, bool restore)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_privacy_deny(PurpleAccount * account, char * who, gboolean local, gboolean restore)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_privacy_deny(IntPtr account, string who, bool local, bool restore);

		public static void Deny(PurpleAccount account, string who, bool local, bool restore)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_privacy_check(PurpleAccount * account, char * who)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_privacy_check(IntPtr account, string who);

		public static bool Check(PurpleAccount account, string who)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_privacy_set_ui_ops(PurplePrivacyUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_privacy_set_ui_ops(IntPtr ops);

		public static void SetUiOps(PurplePrivacyUiOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePrivacyUiOps * purple_privacy_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_privacy_get_ui_ops();

		public static PurplePrivacyUiOps GetUiOps()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_privacy_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_privacy_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

	}
}

