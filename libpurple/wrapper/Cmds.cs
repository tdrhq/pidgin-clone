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
	 * File: cmds.h
	 */
	public class Cmds
	{
		public enum PurpleCmdStatus
		{
			PURPLE_CMD_STATUS_OK,
			PURPLE_CMD_STATUS_FAILED,
			PURPLE_CMD_STATUS_NOT_FOUND,
			PURPLE_CMD_STATUS_WRONG_ARGS,
			PURPLE_CMD_STATUS_WRONG_PRPL,
			PURPLE_CMD_STATUS_WRONG_TYPE
		};

		public enum PurpleCmdRet
		{
			PURPLE_CMD_RET_OK,
			PURPLE_CMD_RET_FAILED,
			PURPLE_CMD_RET_CONTINUE
		};

		public enum PurpleCmdPriority
		{
			PURPLE_CMD_P_VERY_LOW = -1000,
			PURPLE_CMD_P_LOW = 0,
			PURPLE_CMD_P_DEFAULT = 1000,
			PURPLE_CMD_P_PRPL = 2000,
			PURPLE_CMD_P_PLUGIN = 3000,
			PURPLE_CMD_P_ALIAS = 4000,
			PURPLE_CMD_P_HIGH = 5000,
			PURPLE_CMD_P_VERY_HIGH = 6000
		};

		public enum PurpleCmdFlag
		{
			PURPLE_CMD_FLAG_IM = 0x01,
			PURPLE_CMD_FLAG_CHAT = 0x02,
			PURPLE_CMD_FLAG_PRPL_ONLY = 0x04,
			PURPLE_CMD_FLAG_ALLOW_WRONG_ARGS = 0x08
		};

		public delegate Cmds.PurpleCmdRet PurpleCmdFunc(IntPtr _PurpleWrapper_arg0, string cmd, IntPtr args, IntPtr error, IntPtr data);

		/*
		 * Function Name: purple_cmd_unregister
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_cmd_unregister(PurpleCmdId id)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cmd_unregister(uint id);

		public static void CmdUnregister(uint id)
		{
			purple_cmd_unregister(id);
		}

		/*
		 * Function Name: purple_cmd_list
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_cmd_list(PurpleConversation * conv)
		 * 
		 * Could not generate a wrapper for purple_cmd_list in file "cmds.h".
		 * Message: Unknown inner-type of argument 0 of purple_cmd_list
		 */

		/*
		 * Function Name: purple_cmd_help
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_cmd_help(PurpleConversation * conv, gchar * cmd)
		 * 
		 * Could not generate a wrapper for purple_cmd_help in file "cmds.h".
		 * Message: Unknown inner-type of argument 0 of purple_cmd_help
		 */

		/*
		 * Function Name: purple_cmds_get_handle
		 * Function Type: gpointer
		 * Function RawType: gpointer
		 * Function Category: VoidPointer
		 */
		/*
		 * gpointer purple_cmds_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_cmds_get_handle();

		public static IntPtr GetHandle()
		{
			return purple_cmds_get_handle();
		}

		/*
		 * Function Name: purple_cmds_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_cmds_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cmds_init();

		public static void Init()
		{
			purple_cmds_init();
		}

		/*
		 * Function Name: purple_cmds_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_cmds_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_cmds_uninit();

		public static void Uninit()
		{
			purple_cmds_uninit();
		}

	}
}

