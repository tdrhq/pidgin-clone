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
	public class Cmds
	{
		/*
		 * PurpleCmdId purple_cmd_register(gchar * cmd, gchar * args, PurpleCmdPriority p, PurpleCmdFlag f, gchar * prpl_id, PurpleCmdFunc func, gchar * helpstr, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_cmd_register(string cmd, string args, UNKNOWN p, UNKNOWN f, string prpl_id, UNKNOWN func, string helpstr, IntPtr data);

		public static PurpleCmdId CmdRegister(string cmd, string args, PurpleCmdPriority p, PurpleCmdFlag f, string prpl_id, PurpleCmdFunc func, string helpstr, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_cmd_unregister(PurpleCmdId id)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cmd_unregister(UNKNOWN id);

		public static void CmdUnregister(PurpleCmdId id)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleCmdStatus purple_cmd_do_command(PurpleConversation * conv, gchar * cmdline, gchar * markup, gchar ** errormsg)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_cmd_do_command(IntPtr conv, string cmdline, string markup, IntPtr errormsg);

		public static PurpleCmdStatus CmdDoCommand(PurpleConversation conv, string cmdline, string markup, gchar errormsg)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_cmd_list(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_cmd_list(IntPtr conv);

		public static GList CmdList(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_cmd_help(PurpleConversation * conv, gchar * cmd)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_cmd_help(IntPtr conv, string cmd);

		public static GList CmdHelp(PurpleConversation conv, string cmd)
		{
			throw new NotImplementedException();
		}

		/*
		 * gpointer purple_cmds_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_cmds_get_handle();

		public static IntPtr GetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_cmds_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cmds_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_cmds_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cmds_uninit();

		public static void Uninit()
		{
			throw new NotImplementedException();
		}

	}
}

