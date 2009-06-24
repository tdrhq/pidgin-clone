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
	public class Connection
	{
		/*
		 * void purple_connection_new(PurpleAccount * account, gboolean regist, char * password)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_new(IntPtr account, bool regist, string password);

		public static void New(PurpleAccount account, bool regist, string password)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_connection_new_unregister(PurpleAccount * account, char * password, PurpleAccountUnregistrationCb cb, void * user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_new_unregister(IntPtr account, string password, UNKNOWN cb, IntPtr user_data);

		public static void NewUnregister(PurpleAccount account, string password, PurpleAccountUnregistrationCb cb, IntPtr user_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_connection_destroy(PurpleConnection * gc)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_destroy(IntPtr gc);

		public static void Destroy(PurpleConnection gc)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_connection_set_state(PurpleConnection * gc, PurpleConnectionState state)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_set_state(IntPtr gc, UNKNOWN state);

		public static void SetState(PurpleConnection gc, PurpleConnectionState state)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_connection_set_account(PurpleConnection * gc, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_set_account(IntPtr gc, IntPtr account);

		public static void SetAccount(PurpleConnection gc, PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_connection_set_display_name(PurpleConnection * gc, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_set_display_name(IntPtr gc, string name);

		public static void SetDisplayName(PurpleConnection gc, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_connection_set_protocol_data(PurpleConnection * connection, void * proto_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_set_protocol_data(IntPtr connection, IntPtr proto_data);

		public static void SetProtocolData(PurpleConnection connection, IntPtr proto_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConnectionState purple_connection_get_state(PurpleConnection * gc)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_connection_get_state(IntPtr gc);

		public static PurpleConnectionState GetState(PurpleConnection gc)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccount * purple_connection_get_account(PurpleConnection * gc)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_connection_get_account(IntPtr gc);

		public static PurpleAccount GetAccount(PurpleConnection gc)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_connection_get_password(PurpleConnection * gc)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_connection_get_password(IntPtr gc);

		public static string GetPassword(PurpleConnection gc)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_connection_get_display_name(PurpleConnection * gc)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_connection_get_display_name(IntPtr gc);

		public static string GetDisplayName(PurpleConnection gc)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_connection_get_protocol_data(PurpleConnection * connection)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_connection_get_protocol_data(IntPtr connection);

		public static IntPtr GetProtocolData(PurpleConnection connection)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_connection_update_progress(PurpleConnection * gc, char * text, size_t step, size_t count)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_update_progress(IntPtr gc, string text, UNKNOWN step, UNKNOWN count);

		public static void UpdateProgress(PurpleConnection gc, string text, size_t step, size_t count)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_connection_notice(PurpleConnection * gc, char * text)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_notice(IntPtr gc, string text);

		public static void Notice(PurpleConnection gc, string text)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_connection_error(PurpleConnection * gc, char * reason)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_error(IntPtr gc, string reason);

		public static void Error(PurpleConnection gc, string reason)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_connection_error_reason(PurpleConnection * gc, PurpleConnectionError reason, char * description)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_error_reason(IntPtr gc, UNKNOWN reason, string description);

		public static void ErrorReason(PurpleConnection gc, PurpleConnectionError reason, string description)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_connection_ssl_error(PurpleConnection * gc, PurpleSslErrorType ssl_error)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_ssl_error(IntPtr gc, UNKNOWN ssl_error);

		public static void SslError(PurpleConnection gc, PurpleSslErrorType ssl_error)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_connection_error_is_fatal(PurpleConnectionError reason)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_connection_error_is_fatal(UNKNOWN reason);

		public static bool ErrorIsFatal(PurpleConnectionError reason)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_connections_disconnect_all()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connections_disconnect_all();

		public static void ConnectionsDisconnectAll()
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_connections_get_all()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_connections_get_all();

		public static GList ConnectionsGetAll()
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_connections_get_connecting()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_connections_get_connecting();

		public static GList ConnectionsGetConnecting()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_connections_set_ui_ops(PurpleConnectionUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connections_set_ui_ops(IntPtr ops);

		public static void ConnectionsSetUiOps(PurpleConnectionUiOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConnectionUiOps * purple_connections_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_connections_get_ui_ops();

		public static PurpleConnectionUiOps ConnectionsGetUiOps()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_connections_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connections_init();

		public static void ConnectionsInit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_connections_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connections_uninit();

		public static void ConnectionsUninit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_connections_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_connections_get_handle();

		public static IntPtr ConnectionsGetHandle()
		{
			throw new NotImplementedException();
		}

	}
}

