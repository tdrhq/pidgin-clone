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
	public class Connection
	{
		public enum PurpleConnectionFlags
		{
			PURPLE_CONNECTION_HTML = 0x0001,
			PURPLE_CONNECTION_NO_BGCOLOR = 0x0002,
			PURPLE_CONNECTION_AUTO_RESP = 0x0004,
			PURPLE_CONNECTION_FORMATTING_WBFO = 0x0008,
			PURPLE_CONNECTION_NO_NEWLINES = 0x0010,
			PURPLE_CONNECTION_NO_FONTSIZE = 0x0020,
			PURPLE_CONNECTION_NO_URLDESC = 0x0040,
			PURPLE_CONNECTION_NO_IMAGES = 0x0080,
			PURPLE_CONNECTION_ALLOW_CUSTOM_SMILEY = 0x0100
		};

		public enum PurpleConnectionState
		{
			PURPLE_DISCONNECTED = 0,
			PURPLE_CONNECTED,
			PURPLE_CONNECTING
		};

		public enum PurpleConnectionError
		{
			PURPLE_CONNECTION_ERROR_NETWORK_ERROR = 0,
			PURPLE_CONNECTION_ERROR_INVALID_USERNAME = 1,
			PURPLE_CONNECTION_ERROR_AUTHENTICATION_FAILED = 2,
			PURPLE_CONNECTION_ERROR_AUTHENTICATION_IMPOSSIBLE = 3,
			PURPLE_CONNECTION_ERROR_NO_SSL_SUPPORT = 4,
			PURPLE_CONNECTION_ERROR_ENCRYPTION_ERROR = 5,
			PURPLE_CONNECTION_ERROR_NAME_IN_USE = 6,
			PURPLE_CONNECTION_ERROR_INVALID_SETTINGS = 7,
			PURPLE_CONNECTION_ERROR_CERT_NOT_PROVIDED = 8,
			PURPLE_CONNECTION_ERROR_CERT_UNTRUSTED = 9,
			PURPLE_CONNECTION_ERROR_CERT_EXPIRED = 10,
			PURPLE_CONNECTION_ERROR_CERT_NOT_ACTIVATED = 11,
			PURPLE_CONNECTION_ERROR_CERT_HOSTNAME_MISMATCH = 12,
			PURPLE_CONNECTION_ERROR_CERT_FINGERPRINT_MISMATCH = 13,
			PURPLE_CONNECTION_ERROR_CERT_SELF_SIGNED = 14,
			PURPLE_CONNECTION_ERROR_CERT_OTHER_ERROR = 15,
			PURPLE_CONNECTION_ERROR_OTHER_ERROR = 16
		};

		/*
		 * void purple_connection_new_unregister(PurpleAccount * account, char * password, PurpleAccountUnregistrationCb cb, void * user_data)
		 * 
		 * Could not generate a wrapper for purple_connection_new_unregister in file "connection.h".
		 * Message: The type could not be resolved (PurpleAccountUnregistrationCb cb).
		 */

		/*
		 * void purple_connection_destroy(PurpleConnection * gc)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_destroy(IntPtr gc);

		public static void Destroy(PurpleConnection gc)
		{
			purple_connection_destroy(gc.Reference);
		}

		/*
		 * void purple_connection_set_state(PurpleConnection * gc, PurpleConnectionState state)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_set_state(IntPtr gc, Connection.PurpleConnectionState state);

		public static void SetState(PurpleConnection gc, Connection.PurpleConnectionState state)
		{
			/* Unable to process state, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * void purple_connection_set_account(PurpleConnection * gc, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_set_account(IntPtr gc, IntPtr account);

		public static void SetAccount(PurpleConnection gc, PurpleAccount account)
		{
			purple_connection_set_account(gc.Reference, account.Reference);
		}

		/*
		 * void purple_connection_set_display_name(PurpleConnection * gc, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_set_display_name(IntPtr gc, string name);

		public static void SetDisplayName(PurpleConnection gc, string name)
		{
			purple_connection_set_display_name(gc.Reference, name);
		}

		/*
		 * void purple_connection_set_protocol_data(PurpleConnection * connection, void * proto_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_set_protocol_data(IntPtr connection, IntPtr proto_data);

		public static void SetProtocolData(PurpleConnection connection, IntPtr proto_data)
		{
			purple_connection_set_protocol_data(connection.Reference, proto_data);
		}

		/*
		 * PurpleConnectionState purple_connection_get_state(PurpleConnection * gc)
		 */
		[DllImport("libpurple.dll")]
		private static extern Connection.PurpleConnectionState purple_connection_get_state(IntPtr gc);

		public static Connection.PurpleConnectionState GetState(PurpleConnection gc)
		{
			/* Unable to process purple_connection_get_state, a KnownEnum. */
			
		}

		/*
		 * PurpleAccount * purple_connection_get_account(PurpleConnection * gc)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_connection_get_account(IntPtr gc);

		public static PurpleAccount GetAccount(PurpleConnection gc)
		{
			return new PurpleAccount(purple_connection_get_account(gc.Reference));
		}

		/*
		 * PurplePlugin * purple_connection_get_prpl(PurpleConnection * gc)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_connection_get_prpl(IntPtr gc);

		public static PurplePlugin GetPrpl(PurpleConnection gc)
		{
			return new PurplePlugin(purple_connection_get_prpl(gc.Reference));
		}

		/*
		 * char * purple_connection_get_password(PurpleConnection * gc)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_connection_get_password(IntPtr gc);

		public static string GetPassword(PurpleConnection gc)
		{
			return purple_connection_get_password(gc.Reference);
		}

		/*
		 * char * purple_connection_get_display_name(PurpleConnection * gc)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_connection_get_display_name(IntPtr gc);

		public static string GetDisplayName(PurpleConnection gc)
		{
			return purple_connection_get_display_name(gc.Reference);
		}

		/*
		 * void * purple_connection_get_protocol_data(PurpleConnection * connection)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_connection_get_protocol_data(IntPtr connection);

		public static IntPtr GetProtocolData(PurpleConnection connection)
		{
			return purple_connection_get_protocol_data(connection.Reference);
		}

		/*
		 * void purple_connection_notice(PurpleConnection * gc, char * text)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_notice(IntPtr gc, string text);

		public static void Notice(PurpleConnection gc, string text)
		{
			purple_connection_notice(gc.Reference, text);
		}

		/*
		 * void purple_connection_error(PurpleConnection * gc, char * reason)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connection_error(IntPtr gc, string reason);

		public static void Error(PurpleConnection gc, string reason)
		{
			purple_connection_error(gc.Reference, reason);
		}

		/*
		 * gboolean purple_connection_error_is_fatal(PurpleConnectionError reason)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_connection_error_is_fatal(Connection.PurpleConnectionError reason);

		public static bool ErrorIsFatal(Connection.PurpleConnectionError reason)
		{
			/* Unable to process reason, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * void purple_connections_disconnect_all()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connections_disconnect_all();

		public static void ConnectionsDisconnectAll()
		{
			purple_connections_disconnect_all();
		}

		/*
		 * GList * purple_connections_get_all()
		 * 
		 * Could not generate a wrapper for purple_connections_get_all in file "connection.h".
		 * Message: The type could not be resolved (GList * purple_connections_get_all()).
		 */

		/*
		 * GList * purple_connections_get_connecting()
		 * 
		 * Could not generate a wrapper for purple_connections_get_connecting in file "connection.h".
		 * Message: The type could not be resolved (GList * purple_connections_get_connecting()).
		 */

		/*
		 * void purple_connections_set_ui_ops(PurpleConnectionUiOps * ops)
		 * 
		 * Could not generate a wrapper for purple_connections_set_ui_ops in file "connection.h".
		 * Message: The type could not be resolved (PurpleConnectionUiOps * ops).
		 */

		/*
		 * PurpleConnectionUiOps * purple_connections_get_ui_ops()
		 * 
		 * Could not generate a wrapper for purple_connections_get_ui_ops in file "connection.h".
		 * Message: The type could not be resolved (PurpleConnectionUiOps * purple_connections_get_ui_ops()).
		 */

		/*
		 * void purple_connections_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connections_init();

		public static void ConnectionsInit()
		{
			purple_connections_init();
		}

		/*
		 * void purple_connections_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_connections_uninit();

		public static void ConnectionsUninit()
		{
			purple_connections_uninit();
		}

		/*
		 * void * purple_connections_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_connections_get_handle();

		public static IntPtr ConnectionsGetHandle()
		{
			return purple_connections_get_handle();
		}

	}
}

