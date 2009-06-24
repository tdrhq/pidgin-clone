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
	public class Status
	{
		/*
		 * char * purple_primitive_get_id_from_type(PurpleStatusPrimitive type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_primitive_get_id_from_type(UNKNOWN type);

		public static string PrimitiveGetIdFromType(PurpleStatusPrimitive type)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_primitive_get_name_from_type(PurpleStatusPrimitive type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_primitive_get_name_from_type(UNKNOWN type);

		public static string PrimitiveGetNameFromType(PurpleStatusPrimitive type)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatusPrimitive purple_primitive_get_type_from_id(char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_primitive_get_type_from_id(string id);

		public static PurpleStatusPrimitive PrimitiveGetTypeFromId(string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatusType * purple_status_type_new_full(PurpleStatusPrimitive primitive, char * id, char * name, gboolean saveable, gboolean user_settable, gboolean independent)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_type_new_full(UNKNOWN primitive, string id, string name, bool saveable, bool user_settable, bool independent);

		public static PurpleStatusType TypeNewFull(PurpleStatusPrimitive primitive, string id, string name, bool saveable, bool user_settable, bool independent)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatusType * purple_status_type_new(PurpleStatusPrimitive primitive, char * id, char * name, gboolean user_settable)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_type_new(UNKNOWN primitive, string id, string name, bool user_settable);

		public static PurpleStatusType TypeNew(PurpleStatusPrimitive primitive, string id, string name, bool user_settable)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatusType * purple_status_type_new_with_attrs(PurpleStatusPrimitive primitive, char * id, char * name, gboolean saveable, gboolean user_settable, gboolean independent, char * attr_id, char * attr_name, PurpleValue * attr_value, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_type_new_with_attrs(UNKNOWN primitive, string id, string name, bool saveable, bool user_settable, bool independent, string attr_id, string attr_name, IntPtr attr_value, ...);

		public static PurpleStatusType TypeNewWithAttrs(PurpleStatusPrimitive primitive, string id, string name, bool saveable, bool user_settable, bool independent, string attr_id, string attr_name, PurpleValue attr_value, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_status_type_destroy(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_type_destroy(IntPtr status_type);

		public static void TypeDestroy(PurpleStatusType status_type)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_status_type_set_primary_attr(PurpleStatusType * status_type, char * attr_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_type_set_primary_attr(IntPtr status_type, string attr_id);

		public static void TypeSetPrimaryAttr(PurpleStatusType status_type, string attr_id)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_status_type_add_attr(PurpleStatusType * status_type, char * id, char * name, PurpleValue * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_type_add_attr(IntPtr status_type, string id, string name, IntPtr value);

		public static void TypeAddAttr(PurpleStatusType status_type, string id, string name, PurpleValue value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_status_type_add_attrs(PurpleStatusType * status_type, char * id, char * name, PurpleValue * value, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_type_add_attrs(IntPtr status_type, string id, string name, IntPtr value, ...);

		public static void TypeAddAttrs(PurpleStatusType status_type, string id, string name, PurpleValue value, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_status_type_add_attrs_vargs(PurpleStatusType * status_type, va_list args)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_type_add_attrs_vargs(IntPtr status_type, UNKNOWN args);

		public static void TypeAddAttrsVargs(PurpleStatusType status_type, va_list args)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatusPrimitive purple_status_type_get_primitive(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_status_type_get_primitive(IntPtr status_type);

		public static PurpleStatusPrimitive TypeGetPrimitive(PurpleStatusType status_type)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_status_type_get_id(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_status_type_get_id(IntPtr status_type);

		public static string TypeGetId(PurpleStatusType status_type)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_status_type_get_name(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_status_type_get_name(IntPtr status_type);

		public static string TypeGetName(PurpleStatusType status_type)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_status_type_is_saveable(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_type_is_saveable(IntPtr status_type);

		public static bool TypeIsSaveable(PurpleStatusType status_type)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_status_type_is_user_settable(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_type_is_user_settable(IntPtr status_type);

		public static bool TypeIsUserSettable(PurpleStatusType status_type)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_status_type_is_independent(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_type_is_independent(IntPtr status_type);

		public static bool TypeIsIndependent(PurpleStatusType status_type)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_status_type_is_exclusive(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_type_is_exclusive(IntPtr status_type);

		public static bool TypeIsExclusive(PurpleStatusType status_type)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_status_type_is_available(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_type_is_available(IntPtr status_type);

		public static bool TypeIsAvailable(PurpleStatusType status_type)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_status_type_get_primary_attr(PurpleStatusType * type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_status_type_get_primary_attr(IntPtr type);

		public static string TypeGetPrimaryAttr(PurpleStatusType type)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatusAttr * purple_status_type_get_attr(PurpleStatusType * status_type, char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_type_get_attr(IntPtr status_type, string id);

		public static PurpleStatusAttr TypeGetAttr(PurpleStatusType status_type, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_status_type_get_attrs(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_type_get_attrs(IntPtr status_type);

		public static GList TypeGetAttrs(PurpleStatusType status_type)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatusType * purple_status_type_find_with_id(GList * status_types, char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_type_find_with_id(IntPtr status_types, string id);

		public static PurpleStatusType TypeFindWithId(GList status_types, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatusAttr * purple_status_attr_new(char * id, char * name, PurpleValue * value_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_attr_new(string id, string name, IntPtr value_type);

		public static PurpleStatusAttr AttrNew(string id, string name, PurpleValue value_type)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_status_attr_destroy(PurpleStatusAttr * attr)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_attr_destroy(IntPtr attr);

		public static void AttrDestroy(PurpleStatusAttr attr)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_status_attr_get_id(PurpleStatusAttr * attr)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_status_attr_get_id(IntPtr attr);

		public static string AttrGetId(PurpleStatusAttr attr)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_status_attr_get_name(PurpleStatusAttr * attr)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_status_attr_get_name(IntPtr attr);

		public static string AttrGetName(PurpleStatusAttr attr)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleValue * purple_status_attr_get_value(PurpleStatusAttr * attr)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_attr_get_value(IntPtr attr);

		public static PurpleValue AttrGetValue(PurpleStatusAttr attr)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatus * purple_status_new(PurpleStatusType * status_type, PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_new(IntPtr status_type, IntPtr presence);

		public static PurpleStatus New(PurpleStatusType status_type, PurplePresence presence)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_status_destroy(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_destroy(IntPtr status);

		public static void Destroy(PurpleStatus status)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_status_set_active(PurpleStatus * status, gboolean active)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_set_active(IntPtr status, bool active);

		public static void SetActive(PurpleStatus status, bool active)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_status_set_active_with_attrs(PurpleStatus * status, gboolean active, va_list args)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_set_active_with_attrs(IntPtr status, bool active, UNKNOWN args);

		public static void SetActiveWithAttrs(PurpleStatus status, bool active, va_list args)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_status_set_active_with_attrs_list(PurpleStatus * status, gboolean active, GList * attrs)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_set_active_with_attrs_list(IntPtr status, bool active, IntPtr attrs);

		public static void SetActiveWithAttrsList(PurpleStatus status, bool active, GList attrs)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_status_set_attr_boolean(PurpleStatus * status, char * id, gboolean value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_set_attr_boolean(IntPtr status, string id, bool value);

		public static void SetAttrBoolean(PurpleStatus status, string id, bool value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_status_set_attr_int(PurpleStatus * status, char * id, int value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_set_attr_int(IntPtr status, string id, int value);

		public static void SetAttrInt(PurpleStatus status, string id, int value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_status_set_attr_string(PurpleStatus * status, char * id, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_set_attr_string(IntPtr status, string id, string value);

		public static void SetAttrString(PurpleStatus status, string id, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatusType * purple_status_get_type(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_get_type(IntPtr status);

		public static PurpleStatusType GetType(PurpleStatus status)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePresence * purple_status_get_presence(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_get_presence(IntPtr status);

		public static PurplePresence GetPresence(PurpleStatus status)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_status_get_id(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_status_get_id(IntPtr status);

		public static string GetId(PurpleStatus status)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_status_get_name(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_status_get_name(IntPtr status);

		public static string GetName(PurpleStatus status)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_status_is_independent(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_is_independent(IntPtr status);

		public static bool IsIndependent(PurpleStatus status)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_status_is_exclusive(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_is_exclusive(IntPtr status);

		public static bool IsExclusive(PurpleStatus status)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_status_is_available(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_is_available(IntPtr status);

		public static bool IsAvailable(PurpleStatus status)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_status_is_active(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_is_active(IntPtr status);

		public static bool IsActive(PurpleStatus status)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_status_is_online(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_is_online(IntPtr status);

		public static bool IsOnline(PurpleStatus status)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleValue * purple_status_get_attr_value(PurpleStatus * status, char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_get_attr_value(IntPtr status, string id);

		public static PurpleValue GetAttrValue(PurpleStatus status, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_status_get_attr_boolean(PurpleStatus * status, char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_get_attr_boolean(IntPtr status, string id);

		public static bool GetAttrBoolean(PurpleStatus status, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_status_get_attr_int(PurpleStatus * status, char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_status_get_attr_int(IntPtr status, string id);

		public static int GetAttrInt(PurpleStatus status, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_status_get_attr_string(PurpleStatus * status, char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_status_get_attr_string(IntPtr status, string id);

		public static string GetAttrString(PurpleStatus status, string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * gint purple_status_compare(PurpleStatus * status1, PurpleStatus * status2)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_status_compare(IntPtr status1, IntPtr status2);

		public static int Compare(PurpleStatus status1, PurpleStatus status2)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePresence * purple_presence_new(PurplePresenceContext context)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_new(UNKNOWN context);

		public static PurplePresence PresenceNew(PurplePresenceContext context)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePresence * purple_presence_new_for_account(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_new_for_account(IntPtr account);

		public static PurplePresence PresenceNewForAccount(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePresence * purple_presence_new_for_conv(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_new_for_conv(IntPtr conv);

		public static PurplePresence PresenceNewForConv(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePresence * purple_presence_new_for_buddy(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_new_for_buddy(IntPtr buddy);

		public static PurplePresence PresenceNewForBuddy(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_presence_destroy(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_presence_destroy(IntPtr presence);

		public static void PresenceDestroy(PurplePresence presence)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_presence_add_status(PurplePresence * presence, PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_presence_add_status(IntPtr presence, IntPtr status);

		public static void PresenceAddStatus(PurplePresence presence, PurpleStatus status)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_presence_add_list(PurplePresence * presence, GList * source_list)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_presence_add_list(IntPtr presence, IntPtr source_list);

		public static void PresenceAddList(PurplePresence presence, GList source_list)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_presence_set_status_active(PurplePresence * presence, char * status_id, gboolean active)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_presence_set_status_active(IntPtr presence, string status_id, bool active);

		public static void PresenceSetStatusActive(PurplePresence presence, string status_id, bool active)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_presence_switch_status(PurplePresence * presence, char * status_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_presence_switch_status(IntPtr presence, string status_id);

		public static void PresenceSwitchStatus(PurplePresence presence, string status_id)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_presence_set_idle(PurplePresence * presence, gboolean idle, time_t idle_time)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_presence_set_idle(IntPtr presence, bool idle, UNKNOWN idle_time);

		public static void PresenceSetIdle(PurplePresence presence, bool idle, time_t idle_time)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_presence_set_login_time(PurplePresence * presence, time_t login_time)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_presence_set_login_time(IntPtr presence, UNKNOWN login_time);

		public static void PresenceSetLoginTime(PurplePresence presence, time_t login_time)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePresenceContext purple_presence_get_context(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_presence_get_context(IntPtr presence);

		public static PurplePresenceContext PresenceGetContext(PurplePresence presence)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccount * purple_presence_get_account(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_get_account(IntPtr presence);

		public static PurpleAccount PresenceGetAccount(PurplePresence presence)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConversation * purple_presence_get_conversation(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_get_conversation(IntPtr presence);

		public static PurpleConversation PresenceGetConversation(PurplePresence presence)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_presence_get_chat_user(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_presence_get_chat_user(IntPtr presence);

		public static string PresenceGetChatUser(PurplePresence presence)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBuddy * purple_presence_get_buddy(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_get_buddy(IntPtr presence);

		public static PurpleBuddy PresenceGetBuddy(PurplePresence presence)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_presence_get_statuses(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_get_statuses(IntPtr presence);

		public static GList PresenceGetStatuses(PurplePresence presence)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatus * purple_presence_get_status(PurplePresence * presence, char * status_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_get_status(IntPtr presence, string status_id);

		public static PurpleStatus PresenceGetStatus(PurplePresence presence, string status_id)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatus * purple_presence_get_active_status(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_get_active_status(IntPtr presence);

		public static PurpleStatus PresenceGetActiveStatus(PurplePresence presence)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_presence_is_available(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_presence_is_available(IntPtr presence);

		public static bool PresenceIsAvailable(PurplePresence presence)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_presence_is_online(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_presence_is_online(IntPtr presence);

		public static bool PresenceIsOnline(PurplePresence presence)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_presence_is_status_active(PurplePresence * presence, char * status_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_presence_is_status_active(IntPtr presence, string status_id);

		public static bool PresenceIsStatusActive(PurplePresence presence, string status_id)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_presence_is_status_primitive_active(PurplePresence * presence, PurpleStatusPrimitive primitive)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_presence_is_status_primitive_active(IntPtr presence, UNKNOWN primitive);

		public static bool PresenceIsStatusPrimitiveActive(PurplePresence presence, PurpleStatusPrimitive primitive)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_presence_is_idle(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_presence_is_idle(IntPtr presence);

		public static bool PresenceIsIdle(PurplePresence presence)
		{
			throw new NotImplementedException();
		}

		/*
		 * time_t purple_presence_get_idle_time(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_presence_get_idle_time(IntPtr presence);

		public static time_t PresenceGetIdleTime(PurplePresence presence)
		{
			throw new NotImplementedException();
		}

		/*
		 * time_t purple_presence_get_login_time(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_presence_get_login_time(IntPtr presence);

		public static time_t PresenceGetLoginTime(PurplePresence presence)
		{
			throw new NotImplementedException();
		}

		/*
		 * gint purple_presence_compare(PurplePresence * presence1, PurplePresence * presence2)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_presence_compare(IntPtr presence1, IntPtr presence2);

		public static int PresenceCompare(PurplePresence presence1, PurplePresence presence2)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_status_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_get_handle();

		public static IntPtr GetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_status_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_status_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_uninit();

		public static void Uninit()
		{
			throw new NotImplementedException();
		}

	}
}

