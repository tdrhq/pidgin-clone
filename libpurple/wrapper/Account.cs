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
	 * File: account.h
	 */
	public class Account
	{
		public enum PurpleAccountRequestType
		{
			PURPLE_ACCOUNT_REQUEST_AUTHORIZATION = 0
		};

		public delegate bool PurpleFilterAccountFunc(IntPtr account);
		public delegate void PurpleAccountRequestAuthorizationCb(IntPtr _PurpleWrapper_arg0);
		public delegate void PurpleAccountRegistrationCb(IntPtr account, bool succeeded, IntPtr user_data);
		public delegate void PurpleAccountUnregistrationCb(IntPtr account, bool succeeded, IntPtr user_data);

		/*
		 * Function Name: purple_account_new
		 * Function Type: PurpleAccount *
		 * Function RawType: PurpleAccount
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleAccount * purple_account_new(char * username, char * protocol_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_new(string username, string protocol_id);

		public static PurpleAccount New(string username, string protocol_id)
		{
			return new PurpleAccount(purple_account_new(username, protocol_id));
		}

		/*
		 * Function Name: purple_account_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_destroy(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_destroy(IntPtr account);

		public static void Destroy(PurpleAccount account)
		{
			purple_account_destroy(account.Reference);
		}

		/*
		 * Function Name: purple_account_connect
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_connect(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_connect(IntPtr account);

		public static void Connect(PurpleAccount account)
		{
			purple_account_connect(account.Reference);
		}

		/*
		 * Function Name: purple_account_set_register_callback
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_set_register_callback(PurpleAccount * account, PurpleAccountRegistrationCb cb, void * user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_register_callback(IntPtr account, IntPtr cb, IntPtr user_data);

		public static void SetRegisterCallback(PurpleAccount account, Account.PurpleAccountRegistrationCb cb, IntPtr user_data)
		{
			/* Unable to process cb, a KnownFunctionPointer. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_register
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_register(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_register(IntPtr account);

		public static void Register(PurpleAccount account)
		{
			purple_account_register(account.Reference);
		}

		/*
		 * Function Name: purple_account_unregister
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_unregister(PurpleAccount * account, PurpleAccountUnregistrationCb cb, void * user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_unregister(IntPtr account, IntPtr cb, IntPtr user_data);

		public static void Unregister(PurpleAccount account, Account.PurpleAccountUnregistrationCb cb, IntPtr user_data)
		{
			/* Unable to process cb, a KnownFunctionPointer. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_disconnect
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_disconnect(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_disconnect(IntPtr account);

		public static void Disconnect(PurpleAccount account)
		{
			purple_account_disconnect(account.Reference);
		}

		/*
		 * Function Name: purple_account_request_close_with_account
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_request_close_with_account(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_request_close_with_account(IntPtr account);

		public static void RequestCloseWithAccount(PurpleAccount account)
		{
			purple_account_request_close_with_account(account.Reference);
		}

		/*
		 * Function Name: purple_account_request_close
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_request_close(void * ui_handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_request_close(IntPtr ui_handle);

		public static void RequestClose(IntPtr ui_handle)
		{
			purple_account_request_close(ui_handle);
		}

		/*
		 * Function Name: purple_account_request_change_password
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_request_change_password(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_request_change_password(IntPtr account);

		public static void RequestChangePassword(PurpleAccount account)
		{
			purple_account_request_change_password(account.Reference);
		}

		/*
		 * Function Name: purple_account_request_change_user_info
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_request_change_user_info(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_request_change_user_info(IntPtr account);

		public static void RequestChangeUserInfo(PurpleAccount account)
		{
			purple_account_request_change_user_info(account.Reference);
		}

		/*
		 * Function Name: purple_account_set_username
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_set_username(PurpleAccount * account, char * username)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_username(IntPtr account, string username);

		public static void SetUsername(PurpleAccount account, string username)
		{
			purple_account_set_username(account.Reference, username);
		}

		/*
		 * Function Name: purple_account_set_password
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_set_password(PurpleAccount * account, char * password)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_password(IntPtr account, string password);

		public static void SetPassword(PurpleAccount account, string password)
		{
			purple_account_set_password(account.Reference, password);
		}

		/*
		 * Function Name: purple_account_set_alias
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_set_alias(PurpleAccount * account, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_alias(IntPtr account, string alias);

		public static void SetAlias(PurpleAccount account, string alias)
		{
			purple_account_set_alias(account.Reference, alias);
		}

		/*
		 * Function Name: purple_account_set_user_info
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_set_user_info(PurpleAccount * account, char * user_info)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_user_info(IntPtr account, string user_info);

		public static void SetUserInfo(PurpleAccount account, string user_info)
		{
			purple_account_set_user_info(account.Reference, user_info);
		}

		/*
		 * Function Name: purple_account_set_buddy_icon_path
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_set_buddy_icon_path(PurpleAccount * account, char * path)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_buddy_icon_path(IntPtr account, string path);

		public static void SetBuddyIconPath(PurpleAccount account, string path)
		{
			purple_account_set_buddy_icon_path(account.Reference, path);
		}

		/*
		 * Function Name: purple_account_set_connection
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_set_connection(PurpleAccount * account, PurpleConnection * gc)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_connection(IntPtr account, IntPtr gc);

		public static void SetConnection(PurpleAccount account, PurpleConnection gc)
		{
			purple_account_set_connection(account.Reference, gc.Reference);
		}

		/*
		 * Function Name: purple_account_set_remember_password
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_set_remember_password(PurpleAccount * account, gboolean value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_remember_password(IntPtr account, bool value);

		public static void SetRememberPassword(PurpleAccount account, bool value)
		{
			purple_account_set_remember_password(account.Reference, value);
		}

		/*
		 * Function Name: purple_account_set_check_mail
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_set_check_mail(PurpleAccount * account, gboolean value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_check_mail(IntPtr account, bool value);

		public static void SetCheckMail(PurpleAccount account, bool value)
		{
			purple_account_set_check_mail(account.Reference, value);
		}

		/*
		 * Function Name: purple_account_set_proxy_info
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_set_proxy_info(PurpleAccount * account, PurpleProxyInfo * info)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_proxy_info(IntPtr account, IntPtr info);

		public static void SetProxyInfo(PurpleAccount account, /* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr info)
		{
			/* Unable to process info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_set_status_types
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_set_status_types(PurpleAccount * account, GList * status_types)
		 * 
		 * Could not generate a wrapper for purple_account_set_status_types in file "account.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Function Name: purple_account_clear_settings
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_clear_settings(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_clear_settings(IntPtr account);

		public static void ClearSettings(PurpleAccount account)
		{
			purple_account_clear_settings(account.Reference);
		}

		/*
		 * Function Name: purple_account_set_int
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_set_int(PurpleAccount * account, char * name, int value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_int(IntPtr account, string name, int value);

		public static void SetInt(PurpleAccount account, string name, int value)
		{
			purple_account_set_int(account.Reference, name, value);
		}

		/*
		 * Function Name: purple_account_is_connected
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_account_is_connected(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_is_connected(IntPtr account);

		public static bool IsConnected(PurpleAccount account)
		{
			return purple_account_is_connected(account.Reference);
		}

		/*
		 * Function Name: purple_account_is_connecting
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_account_is_connecting(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_is_connecting(IntPtr account);

		public static bool IsConnecting(PurpleAccount account)
		{
			return purple_account_is_connecting(account.Reference);
		}

		/*
		 * Function Name: purple_account_is_disconnected
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_account_is_disconnected(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_is_disconnected(IntPtr account);

		public static bool IsDisconnected(PurpleAccount account)
		{
			return purple_account_is_disconnected(account.Reference);
		}

		/*
		 * Function Name: purple_account_get_username
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_account_get_username(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_get_username(IntPtr account);

		public static string GetUsername(PurpleAccount account)
		{
			return purple_account_get_username(account.Reference);
		}

		/*
		 * Function Name: purple_account_get_password
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_account_get_password(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_get_password(IntPtr account);

		public static string GetPassword(PurpleAccount account)
		{
			return purple_account_get_password(account.Reference);
		}

		/*
		 * Function Name: purple_account_get_alias
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_account_get_alias(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_get_alias(IntPtr account);

		public static string GetAlias(PurpleAccount account)
		{
			return purple_account_get_alias(account.Reference);
		}

		/*
		 * Function Name: purple_account_get_user_info
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_account_get_user_info(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_get_user_info(IntPtr account);

		public static string GetUserInfo(PurpleAccount account)
		{
			return purple_account_get_user_info(account.Reference);
		}

		/*
		 * Function Name: purple_account_get_buddy_icon_path
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_account_get_buddy_icon_path(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_get_buddy_icon_path(IntPtr account);

		public static string GetBuddyIconPath(PurpleAccount account)
		{
			return purple_account_get_buddy_icon_path(account.Reference);
		}

		/*
		 * Function Name: purple_account_get_protocol_id
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_account_get_protocol_id(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_get_protocol_id(IntPtr account);

		public static string GetProtocolId(PurpleAccount account)
		{
			return purple_account_get_protocol_id(account.Reference);
		}

		/*
		 * Function Name: purple_account_get_protocol_name
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_account_get_protocol_name(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_get_protocol_name(IntPtr account);

		public static string GetProtocolName(PurpleAccount account)
		{
			return purple_account_get_protocol_name(account.Reference);
		}

		/*
		 * Function Name: purple_account_get_connection
		 * Function Type: PurpleConnection *
		 * Function RawType: PurpleConnection
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleConnection * purple_account_get_connection(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_connection(IntPtr account);

		public static PurpleConnection GetConnection(PurpleAccount account)
		{
			return new PurpleConnection(purple_account_get_connection(account.Reference));
		}

		/*
		 * Function Name: purple_account_get_remember_password
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_account_get_remember_password(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_get_remember_password(IntPtr account);

		public static bool GetRememberPassword(PurpleAccount account)
		{
			return purple_account_get_remember_password(account.Reference);
		}

		/*
		 * Function Name: purple_account_get_check_mail
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_account_get_check_mail(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_get_check_mail(IntPtr account);

		public static bool GetCheckMail(PurpleAccount account)
		{
			return purple_account_get_check_mail(account.Reference);
		}

		/*
		 * Function Name: purple_account_get_proxy_info
		 * Function Type: PurpleProxyInfo *
		 * Function RawType: PurpleProxyInfo
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleProxyInfo * purple_account_get_proxy_info(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_proxy_info(IntPtr account);

		public static /* Resolved as PointerToUnknownStruct to PurpleProxyInfo */ IntPtr GetProxyInfo(PurpleAccount account)
		{
			/* Unable to process the return value of purple_account_get_proxy_info, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_get_active_status
		 * Function Type: PurpleStatus *
		 * Function RawType: PurpleStatus
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleStatus * purple_account_get_active_status(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_active_status(IntPtr account);

		public static /* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr GetActiveStatus(PurpleAccount account)
		{
			/* Unable to process the return value of purple_account_get_active_status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_get_presence
		 * Function Type: PurplePresence *
		 * Function RawType: PurplePresence
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurplePresence * purple_account_get_presence(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_presence(IntPtr account);

		public static /* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr GetPresence(PurpleAccount account)
		{
			/* Unable to process the return value of purple_account_get_presence, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_get_status_types
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_account_get_status_types(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_status_types(IntPtr account);

		public static IList<string> GetStatusTypes(PurpleAccount account)
		{
			/* Unable to process the return value of purple_account_get_status_types, a PointerToGList. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_get_log
		 * Function Type: PurpleLog *
		 * Function RawType: PurpleLog
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleLog * purple_account_get_log(PurpleAccount * account, gboolean create)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_log(IntPtr account, bool create);

		public static PurpleLog GetLog(PurpleAccount account, bool create)
		{
			return new PurpleLog(purple_account_get_log(account.Reference, create));
		}

		/*
		 * Function Name: purple_account_destroy_log
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_destroy_log(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_destroy_log(IntPtr account);

		public static void DestroyLog(PurpleAccount account)
		{
			purple_account_destroy_log(account.Reference);
		}

		/*
		 * Function Name: purple_account_add_buddy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_add_buddy(PurpleAccount * account, PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_add_buddy(IntPtr account, IntPtr buddy);

		public static void AddBuddy(PurpleAccount account, PurpleBuddy buddy)
		{
			purple_account_add_buddy(account.Reference, buddy.Reference);
		}

		/*
		 * Function Name: purple_account_add_buddies
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_add_buddies(PurpleAccount * account, GList * buddies)
		 * 
		 * Could not generate a wrapper for purple_account_add_buddies in file "account.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Function Name: purple_account_remove_group
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_remove_group(PurpleAccount * account, PurpleGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_remove_group(IntPtr account, IntPtr group);

		public static void RemoveGroup(PurpleAccount account, PurpleGroup group)
		{
			purple_account_remove_group(account.Reference, group.Reference);
		}

		/*
		 * Function Name: purple_account_supports_offline_message
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_account_supports_offline_message(PurpleAccount * account, PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_supports_offline_message(IntPtr account, IntPtr buddy);

		public static bool SupportsOfflineMessage(PurpleAccount account, PurpleBuddy buddy)
		{
			return purple_account_supports_offline_message(account.Reference, buddy.Reference);
		}

		/*
		 * Function Name: purple_account_get_current_error
		 * Function Type: PurpleConnectionErrorInfo *
		 * Function RawType: PurpleConnectionErrorInfo
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleConnectionErrorInfo * purple_account_get_current_error(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_current_error(IntPtr account);

		public static /* Resolved as PointerToUnknownStruct to PurpleConnectionErrorInfo */ IntPtr GetCurrentError(PurpleAccount account)
		{
			/* Unable to process the return value of purple_account_get_current_error, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_account_clear_current_error
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_account_clear_current_error(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_clear_current_error(IntPtr account);

		public static void ClearCurrentError(PurpleAccount account)
		{
			purple_account_clear_current_error(account.Reference);
		}

		/*
		 * Function Name: purple_accounts_add
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_accounts_add(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_accounts_add(IntPtr account);

		public static void AccountsAdd(PurpleAccount account)
		{
			purple_accounts_add(account.Reference);
		}

		/*
		 * Function Name: purple_accounts_remove
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_accounts_remove(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_accounts_remove(IntPtr account);

		public static void AccountsRemove(PurpleAccount account)
		{
			purple_accounts_remove(account.Reference);
		}

		/*
		 * Function Name: purple_accounts_delete
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_accounts_delete(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_accounts_delete(IntPtr account);

		public static void AccountsDelete(PurpleAccount account)
		{
			purple_accounts_delete(account.Reference);
		}

		/*
		 * Function Name: purple_accounts_reorder
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_accounts_reorder(PurpleAccount * account, gint new_index)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_accounts_reorder(IntPtr account, int new_index);

		public static void AccountsReorder(PurpleAccount account, int new_index)
		{
			purple_accounts_reorder(account.Reference, new_index);
		}

		/*
		 * Function Name: purple_accounts_get_all
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_accounts_get_all()
		 * 
		 * Could not generate a wrapper for purple_accounts_get_all in file "account.h".
		 * Message: Unknown inner-type of argument 0 of purple_accounts_get_all
		 */

		/*
		 * Function Name: purple_accounts_get_all_active
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_accounts_get_all_active()
		 * 
		 * Could not generate a wrapper for purple_accounts_get_all_active in file "account.h".
		 * Message: Unknown inner-type of argument 0 of purple_accounts_get_all_active
		 */

		/*
		 * Function Name: purple_accounts_find
		 * Function Type: PurpleAccount *
		 * Function RawType: PurpleAccount
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleAccount * purple_accounts_find(char * name, char * protocol)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_accounts_find(string name, string protocol);

		public static PurpleAccount AccountsFind(string name, string protocol)
		{
			return new PurpleAccount(purple_accounts_find(name, protocol));
		}

		/*
		 * Function Name: purple_accounts_restore_current_statuses
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_accounts_restore_current_statuses()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_accounts_restore_current_statuses();

		public static void AccountsRestoreCurrentStatuses()
		{
			purple_accounts_restore_current_statuses();
		}

		/*
		 * Function Name: purple_accounts_set_ui_ops
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_accounts_set_ui_ops(PurpleAccountUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_accounts_set_ui_ops(IntPtr ops);

		public static void AccountsSetUiOps(PurpleAccountUiOps ops)
		{
			purple_accounts_set_ui_ops(ops.Reference);
		}

		/*
		 * Function Name: purple_accounts_get_ui_ops
		 * Function Type: PurpleAccountUiOps *
		 * Function RawType: PurpleAccountUiOps
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleAccountUiOps * purple_accounts_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_accounts_get_ui_ops();

		public static PurpleAccountUiOps AccountsGetUiOps()
		{
			return new PurpleAccountUiOps(purple_accounts_get_ui_ops());
		}

		/*
		 * Function Name: purple_accounts_get_handle
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
		 */
		/*
		 * void * purple_accounts_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_accounts_get_handle();

		public static IntPtr AccountsGetHandle()
		{
			return purple_accounts_get_handle();
		}

		/*
		 * Function Name: purple_accounts_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_accounts_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_accounts_init();

		public static void AccountsInit()
		{
			purple_accounts_init();
		}

		/*
		 * Function Name: purple_accounts_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_accounts_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_accounts_uninit();

		public static void AccountsUninit()
		{
			purple_accounts_uninit();
		}

	}
}

