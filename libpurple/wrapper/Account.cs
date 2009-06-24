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
	public class Account
	{
		/*
		 * PurpleAccount * purple_account_new(char * username, char * protocol_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_new(string username, string protocol_id);

		public static PurpleAccount New(string username, string protocol_id)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_destroy(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_destroy(IntPtr account);

		public static void Destroy(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_connect(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_connect(IntPtr account);

		public static void Connect(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_register_callback(PurpleAccount * account, PurpleAccountRegistrationCb cb, void * user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_register_callback(IntPtr account, UNKNOWN cb, IntPtr user_data);

		public static void SetRegisterCallback(PurpleAccount account, PurpleAccountRegistrationCb cb, IntPtr user_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_register(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_register(IntPtr account);

		public static void Register(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_unregister(PurpleAccount * account, PurpleAccountUnregistrationCb cb, void * user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_unregister(IntPtr account, UNKNOWN cb, IntPtr user_data);

		public static void Unregister(PurpleAccount account, PurpleAccountUnregistrationCb cb, IntPtr user_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_disconnect(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_disconnect(IntPtr account);

		public static void Disconnect(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_notify_added(PurpleAccount * account, char * remote_user, char * id, char * alias, char * message)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_notify_added(IntPtr account, string remote_user, string id, string alias, string message);

		public static void NotifyAdded(PurpleAccount account, string remote_user, string id, string alias, string message)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_request_add(PurpleAccount * account, char * remote_user, char * id, char * alias, char * message)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_request_add(IntPtr account, string remote_user, string id, string alias, string message);

		public static void RequestAdd(PurpleAccount account, string remote_user, string id, string alias, string message)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_account_request_authorization(PurpleAccount * account, char * remote_user, char * id, char * alias, char * message, gboolean on_list, PurpleAccountRequestAuthorizationCb auth_cb, PurpleAccountRequestAuthorizationCb deny_cb, void * user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_request_authorization(IntPtr account, string remote_user, string id, string alias, string message, bool on_list, UNKNOWN auth_cb, UNKNOWN deny_cb, IntPtr user_data);

		public static IntPtr RequestAuthorization(PurpleAccount account, string remote_user, string id, string alias, string message, bool on_list, PurpleAccountRequestAuthorizationCb auth_cb, PurpleAccountRequestAuthorizationCb deny_cb, IntPtr user_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_request_close_with_account(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_request_close_with_account(IntPtr account);

		public static void RequestCloseWithAccount(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_request_close(void * ui_handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_request_close(IntPtr ui_handle);

		public static void RequestClose(IntPtr ui_handle)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_request_password(PurpleAccount * account, GCallback ok_cb, GCallback cancel_cb, void * user_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_request_password(IntPtr account, UNKNOWN ok_cb, UNKNOWN cancel_cb, IntPtr user_data);

		public static void RequestPassword(PurpleAccount account, GCallback ok_cb, GCallback cancel_cb, IntPtr user_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_request_change_password(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_request_change_password(IntPtr account);

		public static void RequestChangePassword(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_request_change_user_info(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_request_change_user_info(IntPtr account);

		public static void RequestChangeUserInfo(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_username(PurpleAccount * account, char * username)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_username(IntPtr account, string username);

		public static void SetUsername(PurpleAccount account, string username)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_password(PurpleAccount * account, char * password)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_password(IntPtr account, string password);

		public static void SetPassword(PurpleAccount account, string password)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_alias(PurpleAccount * account, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_alias(IntPtr account, string alias);

		public static void SetAlias(PurpleAccount account, string alias)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_user_info(PurpleAccount * account, char * user_info)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_user_info(IntPtr account, string user_info);

		public static void SetUserInfo(PurpleAccount account, string user_info)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_buddy_icon_path(PurpleAccount * account, char * path)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_buddy_icon_path(IntPtr account, string path);

		public static void SetBuddyIconPath(PurpleAccount account, string path)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_protocol_id(PurpleAccount * account, char * protocol_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_protocol_id(IntPtr account, string protocol_id);

		public static void SetProtocolId(PurpleAccount account, string protocol_id)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_connection(PurpleAccount * account, PurpleConnection * gc)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_connection(IntPtr account, IntPtr gc);

		public static void SetConnection(PurpleAccount account, PurpleConnection gc)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_remember_password(PurpleAccount * account, gboolean value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_remember_password(IntPtr account, bool value);

		public static void SetRememberPassword(PurpleAccount account, bool value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_check_mail(PurpleAccount * account, gboolean value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_check_mail(IntPtr account, bool value);

		public static void SetCheckMail(PurpleAccount account, bool value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_enabled(PurpleAccount * account, char * ui, gboolean value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_enabled(IntPtr account, string ui, bool value);

		public static void SetEnabled(PurpleAccount account, string ui, bool value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_proxy_info(PurpleAccount * account, PurpleProxyInfo * info)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_proxy_info(IntPtr account, IntPtr info);

		public static void SetProxyInfo(PurpleAccount account, PurpleProxyInfo info)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_status_types(PurpleAccount * account, GList * status_types)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_status_types(IntPtr account, IntPtr status_types);

		public static void SetStatusTypes(PurpleAccount account, GList status_types)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_status(PurpleAccount * account, char * status_id, gboolean active, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_status(IntPtr account, string status_id, bool active, ...);

		public static void SetStatus(PurpleAccount account, string status_id, bool active, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_status_list(PurpleAccount * account, char * status_id, gboolean active, GList * attrs)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_status_list(IntPtr account, string status_id, bool active, IntPtr attrs);

		public static void SetStatusList(PurpleAccount account, string status_id, bool active, GList attrs)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_clear_settings(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_clear_settings(IntPtr account);

		public static void ClearSettings(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_int(PurpleAccount * account, char * name, int value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_int(IntPtr account, string name, int value);

		public static void SetInt(PurpleAccount account, string name, int value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_string(PurpleAccount * account, char * name, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_string(IntPtr account, string name, string value);

		public static void SetString(PurpleAccount account, string name, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_bool(PurpleAccount * account, char * name, gboolean value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_bool(IntPtr account, string name, bool value);

		public static void SetBool(PurpleAccount account, string name, bool value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_ui_int(PurpleAccount * account, char * ui, char * name, int value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_ui_int(IntPtr account, string ui, string name, int value);

		public static void SetUiInt(PurpleAccount account, string ui, string name, int value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_ui_string(PurpleAccount * account, char * ui, char * name, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_ui_string(IntPtr account, string ui, string name, string value);

		public static void SetUiString(PurpleAccount account, string ui, string name, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_set_ui_bool(PurpleAccount * account, char * ui, char * name, gboolean value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_set_ui_bool(IntPtr account, string ui, string name, bool value);

		public static void SetUiBool(PurpleAccount account, string ui, string name, bool value)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_account_is_connected(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_is_connected(IntPtr account);

		public static bool IsConnected(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_account_is_connecting(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_is_connecting(IntPtr account);

		public static bool IsConnecting(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_account_is_disconnected(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_is_disconnected(IntPtr account);

		public static bool IsDisconnected(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_account_get_username(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_get_username(IntPtr account);

		public static string GetUsername(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_account_get_password(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_get_password(IntPtr account);

		public static string GetPassword(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_account_get_alias(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_get_alias(IntPtr account);

		public static string GetAlias(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_account_get_user_info(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_get_user_info(IntPtr account);

		public static string GetUserInfo(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_account_get_buddy_icon_path(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_get_buddy_icon_path(IntPtr account);

		public static string GetBuddyIconPath(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_account_get_protocol_id(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_get_protocol_id(IntPtr account);

		public static string GetProtocolId(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_account_get_protocol_name(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_get_protocol_name(IntPtr account);

		public static string GetProtocolName(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConnection * purple_account_get_connection(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_connection(IntPtr account);

		public static PurpleConnection GetConnection(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_account_get_remember_password(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_get_remember_password(IntPtr account);

		public static bool GetRememberPassword(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_account_get_check_mail(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_get_check_mail(IntPtr account);

		public static bool GetCheckMail(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_account_get_enabled(PurpleAccount * account, char * ui)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_get_enabled(IntPtr account, string ui);

		public static bool GetEnabled(PurpleAccount account, string ui)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleProxyInfo * purple_account_get_proxy_info(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_proxy_info(IntPtr account);

		public static PurpleProxyInfo GetProxyInfo(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatus * purple_account_get_active_status(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_active_status(IntPtr account);

		public static PurpleStatus GetActiveStatus(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatus * purple_account_get_status(PurpleAccount * account, char * status_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_status(IntPtr account, string status_id);

		public static PurpleStatus GetStatus(PurpleAccount account, string status_id)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatusType * purple_account_get_status_type(PurpleAccount * account, char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_status_type(IntPtr account, string id);

		public static PurpleStatusType GetStatusType(PurpleAccount account, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatusType * purple_account_get_status_type_with_primitive(PurpleAccount * account, PurpleStatusPrimitive primitive)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_status_type_with_primitive(IntPtr account, UNKNOWN primitive);

		public static PurpleStatusType GetStatusTypeWithPrimitive(PurpleAccount account, PurpleStatusPrimitive primitive)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePresence * purple_account_get_presence(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_presence(IntPtr account);

		public static PurplePresence GetPresence(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_account_is_status_active(PurpleAccount * account, char * status_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_is_status_active(IntPtr account, string status_id);

		public static bool IsStatusActive(PurpleAccount account, string status_id)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_account_get_status_types(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_status_types(IntPtr account);

		public static GList GetStatusTypes(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_account_get_int(PurpleAccount * account, char * name, int default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_account_get_int(IntPtr account, string name, int default_value);

		public static int GetInt(PurpleAccount account, string name, int default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_account_get_string(PurpleAccount * account, char * name, char * default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_get_string(IntPtr account, string name, string default_value);

		public static string GetString(PurpleAccount account, string name, string default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_account_get_bool(PurpleAccount * account, char * name, gboolean default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_get_bool(IntPtr account, string name, bool default_value);

		public static bool GetBool(PurpleAccount account, string name, bool default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_account_get_ui_int(PurpleAccount * account, char * ui, char * name, int default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_account_get_ui_int(IntPtr account, string ui, string name, int default_value);

		public static int GetUiInt(PurpleAccount account, string ui, string name, int default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_account_get_ui_string(PurpleAccount * account, char * ui, char * name, char * default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_account_get_ui_string(IntPtr account, string ui, string name, string default_value);

		public static string GetUiString(PurpleAccount account, string ui, string name, string default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_account_get_ui_bool(PurpleAccount * account, char * ui, char * name, gboolean default_value)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_get_ui_bool(IntPtr account, string ui, string name, bool default_value);

		public static bool GetUiBool(PurpleAccount account, string ui, string name, bool default_value)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleLog * purple_account_get_log(PurpleAccount * account, gboolean create)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_log(IntPtr account, bool create);

		public static PurpleLog GetLog(PurpleAccount account, bool create)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_destroy_log(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_destroy_log(IntPtr account);

		public static void DestroyLog(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_add_buddy(PurpleAccount * account, PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_add_buddy(IntPtr account, IntPtr buddy);

		public static void AddBuddy(PurpleAccount account, PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_add_buddies(PurpleAccount * account, GList * buddies)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_add_buddies(IntPtr account, IntPtr buddies);

		public static void AddBuddies(PurpleAccount account, GList buddies)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_remove_buddy(PurpleAccount * account, PurpleBuddy * buddy, PurpleGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_remove_buddy(IntPtr account, IntPtr buddy, IntPtr group);

		public static void RemoveBuddy(PurpleAccount account, PurpleBuddy buddy, PurpleGroup group)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_remove_buddies(PurpleAccount * account, GList * buddies, GList * groups)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_remove_buddies(IntPtr account, IntPtr buddies, IntPtr groups);

		public static void RemoveBuddies(PurpleAccount account, GList buddies, GList groups)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_remove_group(PurpleAccount * account, PurpleGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_remove_group(IntPtr account, IntPtr group);

		public static void RemoveGroup(PurpleAccount account, PurpleGroup group)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_change_password(PurpleAccount * account, char * orig_pw, char * new_pw)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_change_password(IntPtr account, string orig_pw, string new_pw);

		public static void ChangePassword(PurpleAccount account, string orig_pw, string new_pw)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_account_supports_offline_message(PurpleAccount * account, PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_account_supports_offline_message(IntPtr account, IntPtr buddy);

		public static bool SupportsOfflineMessage(PurpleAccount account, PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConnectionErrorInfo * purple_account_get_current_error(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_account_get_current_error(IntPtr account);

		public static PurpleConnectionErrorInfo GetCurrentError(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_account_clear_current_error(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_account_clear_current_error(IntPtr account);

		public static void ClearCurrentError(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_accounts_add(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_accounts_add(IntPtr account);

		public static void AccountsAdd(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_accounts_remove(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_accounts_remove(IntPtr account);

		public static void AccountsRemove(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_accounts_delete(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_accounts_delete(IntPtr account);

		public static void AccountsDelete(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_accounts_reorder(PurpleAccount * account, gint new_index)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_accounts_reorder(IntPtr account, int new_index);

		public static void AccountsReorder(PurpleAccount account, int new_index)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_accounts_get_all()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_accounts_get_all();

		public static GList AccountsGetAll()
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_accounts_get_all_active()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_accounts_get_all_active();

		public static GList AccountsGetAllActive()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccount * purple_accounts_find(char * name, char * protocol)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_accounts_find(string name, string protocol);

		public static PurpleAccount AccountsFind(string name, string protocol)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_accounts_restore_current_statuses()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_accounts_restore_current_statuses();

		public static void AccountsRestoreCurrentStatuses()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_accounts_set_ui_ops(PurpleAccountUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_accounts_set_ui_ops(IntPtr ops);

		public static void AccountsSetUiOps(PurpleAccountUiOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccountUiOps * purple_accounts_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_accounts_get_ui_ops();

		public static PurpleAccountUiOps AccountsGetUiOps()
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_accounts_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_accounts_get_handle();

		public static IntPtr AccountsGetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_accounts_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_accounts_init();

		public static void AccountsInit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_accounts_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_accounts_uninit();

		public static void AccountsUninit()
		{
			throw new NotImplementedException();
		}

	}
}

