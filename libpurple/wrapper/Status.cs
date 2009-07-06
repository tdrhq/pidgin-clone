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
	public class Status
	{
		public enum PurplePresenceContext
		{
			PURPLE_PRESENCE_CONTEXT_UNSET = 0,
			PURPLE_PRESENCE_CONTEXT_ACCOUNT,
			PURPLE_PRESENCE_CONTEXT_CONV,
			PURPLE_PRESENCE_CONTEXT_BUDDY
		};

		public enum PurpleStatusPrimitive
		{
			PURPLE_STATUS_UNSET = 0,
			PURPLE_STATUS_OFFLINE,
			PURPLE_STATUS_AVAILABLE,
			PURPLE_STATUS_UNAVAILABLE,
			PURPLE_STATUS_INVISIBLE,
			PURPLE_STATUS_AWAY,
			PURPLE_STATUS_EXTENDED_AWAY,
			PURPLE_STATUS_MOBILE,
			PURPLE_STATUS_TUNE,
			PURPLE_STATUS_NUM_PRIMITIVES
		};

		/*
		 * char * purple_primitive_get_id_from_type(PurpleStatusPrimitive type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_primitive_get_id_from_type(Status.PurpleStatusPrimitive type);

		public static string PrimitiveGetIdFromType(Status.PurpleStatusPrimitive type)
		{
			/* Unable to process type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * char * purple_primitive_get_name_from_type(PurpleStatusPrimitive type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_primitive_get_name_from_type(Status.PurpleStatusPrimitive type);

		public static string PrimitiveGetNameFromType(Status.PurpleStatusPrimitive type)
		{
			/* Unable to process type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * PurpleStatusPrimitive purple_primitive_get_type_from_id(char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern Status.PurpleStatusPrimitive purple_primitive_get_type_from_id(string id);

		public static Status.PurpleStatusPrimitive PrimitiveGetTypeFromId(string id)
		{
			/* Unable to process purple_primitive_get_type_from_id, a KnownEnum. */
			
		}

		/*
		 * void purple_status_type_destroy(PurpleStatusType * status_type)
		 * 
		 * Could not generate a wrapper for purple_status_type_destroy in file "status.h".
		 * Message: The type could not be resolved (PurpleStatusType * status_type).
		 */

		/*
		 * char * purple_status_type_get_id(PurpleStatusType * status_type)
		 * 
		 * Could not generate a wrapper for purple_status_type_get_id in file "status.h".
		 * Message: The type could not be resolved (PurpleStatusType * status_type).
		 */

		/*
		 * char * purple_status_type_get_name(PurpleStatusType * status_type)
		 * 
		 * Could not generate a wrapper for purple_status_type_get_name in file "status.h".
		 * Message: The type could not be resolved (PurpleStatusType * status_type).
		 */

		/*
		 * gboolean purple_status_type_is_saveable(PurpleStatusType * status_type)
		 * 
		 * Could not generate a wrapper for purple_status_type_is_saveable in file "status.h".
		 * Message: The type could not be resolved (PurpleStatusType * status_type).
		 */

		/*
		 * gboolean purple_status_type_is_user_settable(PurpleStatusType * status_type)
		 * 
		 * Could not generate a wrapper for purple_status_type_is_user_settable in file "status.h".
		 * Message: The type could not be resolved (PurpleStatusType * status_type).
		 */

		/*
		 * gboolean purple_status_type_is_independent(PurpleStatusType * status_type)
		 * 
		 * Could not generate a wrapper for purple_status_type_is_independent in file "status.h".
		 * Message: The type could not be resolved (PurpleStatusType * status_type).
		 */

		/*
		 * gboolean purple_status_type_is_exclusive(PurpleStatusType * status_type)
		 * 
		 * Could not generate a wrapper for purple_status_type_is_exclusive in file "status.h".
		 * Message: The type could not be resolved (PurpleStatusType * status_type).
		 */

		/*
		 * gboolean purple_status_type_is_available(PurpleStatusType * status_type)
		 * 
		 * Could not generate a wrapper for purple_status_type_is_available in file "status.h".
		 * Message: The type could not be resolved (PurpleStatusType * status_type).
		 */

		/*
		 * char * purple_status_type_get_primary_attr(PurpleStatusType * type)
		 * 
		 * Could not generate a wrapper for purple_status_type_get_primary_attr in file "status.h".
		 * Message: The type could not be resolved (PurpleStatusType * type).
		 */

		/*
		 * GList * purple_status_type_get_attrs(PurpleStatusType * status_type)
		 * 
		 * Could not generate a wrapper for purple_status_type_get_attrs in file "status.h".
		 * Message: The type could not be resolved (GList * purple_status_type_get_attrs(PurpleStatusType * status_type)).
		 */

		/*
		 * void purple_status_attr_destroy(PurpleStatusAttr * attr)
		 * 
		 * Could not generate a wrapper for purple_status_attr_destroy in file "status.h".
		 * Message: The type could not be resolved (PurpleStatusAttr * attr).
		 */

		/*
		 * char * purple_status_attr_get_id(PurpleStatusAttr * attr)
		 * 
		 * Could not generate a wrapper for purple_status_attr_get_id in file "status.h".
		 * Message: The type could not be resolved (PurpleStatusAttr * attr).
		 */

		/*
		 * char * purple_status_attr_get_name(PurpleStatusAttr * attr)
		 * 
		 * Could not generate a wrapper for purple_status_attr_get_name in file "status.h".
		 * Message: The type could not be resolved (PurpleStatusAttr * attr).
		 */

		/*
		 * PurpleValue * purple_status_attr_get_value(PurpleStatusAttr * attr)
		 * 
		 * Could not generate a wrapper for purple_status_attr_get_value in file "status.h".
		 * Message: The type could not be resolved (PurpleValue * purple_status_attr_get_value(PurpleStatusAttr * attr)).
		 */

		/*
		 * void purple_status_destroy(PurpleStatus * status)
		 * 
		 * Could not generate a wrapper for purple_status_destroy in file "status.h".
		 * Message: The type could not be resolved (PurpleStatus * status).
		 */

		/*
		 * void purple_status_set_active(PurpleStatus * status, gboolean active)
		 * 
		 * Could not generate a wrapper for purple_status_set_active in file "status.h".
		 * Message: The type could not be resolved (PurpleStatus * status).
		 */

		/*
		 * PurpleStatusType * purple_status_get_type(PurpleStatus * status)
		 * 
		 * Could not generate a wrapper for purple_status_get_type in file "status.h".
		 * Message: The type could not be resolved (PurpleStatusType * purple_status_get_type(PurpleStatus * status)).
		 */

		/*
		 * PurplePresence * purple_status_get_presence(PurpleStatus * status)
		 * 
		 * Could not generate a wrapper for purple_status_get_presence in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * purple_status_get_presence(PurpleStatus * status)).
		 */

		/*
		 * char * purple_status_get_id(PurpleStatus * status)
		 * 
		 * Could not generate a wrapper for purple_status_get_id in file "status.h".
		 * Message: The type could not be resolved (PurpleStatus * status).
		 */

		/*
		 * char * purple_status_get_name(PurpleStatus * status)
		 * 
		 * Could not generate a wrapper for purple_status_get_name in file "status.h".
		 * Message: The type could not be resolved (PurpleStatus * status).
		 */

		/*
		 * gboolean purple_status_is_independent(PurpleStatus * status)
		 * 
		 * Could not generate a wrapper for purple_status_is_independent in file "status.h".
		 * Message: The type could not be resolved (PurpleStatus * status).
		 */

		/*
		 * gboolean purple_status_is_exclusive(PurpleStatus * status)
		 * 
		 * Could not generate a wrapper for purple_status_is_exclusive in file "status.h".
		 * Message: The type could not be resolved (PurpleStatus * status).
		 */

		/*
		 * gboolean purple_status_is_available(PurpleStatus * status)
		 * 
		 * Could not generate a wrapper for purple_status_is_available in file "status.h".
		 * Message: The type could not be resolved (PurpleStatus * status).
		 */

		/*
		 * gboolean purple_status_is_active(PurpleStatus * status)
		 * 
		 * Could not generate a wrapper for purple_status_is_active in file "status.h".
		 * Message: The type could not be resolved (PurpleStatus * status).
		 */

		/*
		 * gboolean purple_status_is_online(PurpleStatus * status)
		 * 
		 * Could not generate a wrapper for purple_status_is_online in file "status.h".
		 * Message: The type could not be resolved (PurpleStatus * status).
		 */

		/*
		 * int purple_status_get_attr_int(PurpleStatus * status, char * id)
		 * 
		 * Could not generate a wrapper for purple_status_get_attr_int in file "status.h".
		 * Message: The type could not be resolved (PurpleStatus * status).
		 */

		/*
		 * gint purple_status_compare(PurpleStatus * status1, PurpleStatus * status2)
		 * 
		 * Could not generate a wrapper for purple_status_compare in file "status.h".
		 * Message: The type could not be resolved (PurpleStatus * status1).
		 */

		/*
		 * PurplePresence * purple_presence_new(PurplePresenceContext context)
		 * 
		 * Could not generate a wrapper for purple_presence_new in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * purple_presence_new(PurplePresenceContext context)).
		 */

		/*
		 * PurplePresence * purple_presence_new_for_account(PurpleAccount * account)
		 * 
		 * Could not generate a wrapper for purple_presence_new_for_account in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * purple_presence_new_for_account(PurpleAccount * account)).
		 */

		/*
		 * PurplePresence * purple_presence_new_for_conv(PurpleConversation * conv)
		 * 
		 * Could not generate a wrapper for purple_presence_new_for_conv in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * purple_presence_new_for_conv(PurpleConversation * conv)).
		 */

		/*
		 * PurplePresence * purple_presence_new_for_buddy(PurpleBuddy * buddy)
		 * 
		 * Could not generate a wrapper for purple_presence_new_for_buddy in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * purple_presence_new_for_buddy(PurpleBuddy * buddy)).
		 */

		/*
		 * void purple_presence_destroy(PurplePresence * presence)
		 * 
		 * Could not generate a wrapper for purple_presence_destroy in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * presence).
		 */

		/*
		 * void purple_presence_add_status(PurplePresence * presence, PurpleStatus * status)
		 * 
		 * Could not generate a wrapper for purple_presence_add_status in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * presence).
		 */

		/*
		 * void purple_presence_add_list(PurplePresence * presence, GList * source_list)
		 * 
		 * Could not generate a wrapper for purple_presence_add_list in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * presence).
		 */

		/*
		 * void purple_presence_set_login_time(PurplePresence * presence, time_t login_time)
		 * 
		 * Could not generate a wrapper for purple_presence_set_login_time in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * presence).
		 */

		/*
		 * PurplePresenceContext purple_presence_get_context(PurplePresence * presence)
		 * 
		 * Could not generate a wrapper for purple_presence_get_context in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * presence).
		 */

		/*
		 * PurpleAccount * purple_presence_get_account(PurplePresence * presence)
		 * 
		 * Could not generate a wrapper for purple_presence_get_account in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * presence).
		 */

		/*
		 * PurpleConversation * purple_presence_get_conversation(PurplePresence * presence)
		 * 
		 * Could not generate a wrapper for purple_presence_get_conversation in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * presence).
		 */

		/*
		 * char * purple_presence_get_chat_user(PurplePresence * presence)
		 * 
		 * Could not generate a wrapper for purple_presence_get_chat_user in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * presence).
		 */

		/*
		 * PurpleBuddy * purple_presence_get_buddy(PurplePresence * presence)
		 * 
		 * Could not generate a wrapper for purple_presence_get_buddy in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * presence).
		 */

		/*
		 * GList * purple_presence_get_statuses(PurplePresence * presence)
		 * 
		 * Could not generate a wrapper for purple_presence_get_statuses in file "status.h".
		 * Message: The type could not be resolved (GList * purple_presence_get_statuses(PurplePresence * presence)).
		 */

		/*
		 * PurpleStatus * purple_presence_get_active_status(PurplePresence * presence)
		 * 
		 * Could not generate a wrapper for purple_presence_get_active_status in file "status.h".
		 * Message: The type could not be resolved (PurpleStatus * purple_presence_get_active_status(PurplePresence * presence)).
		 */

		/*
		 * gboolean purple_presence_is_available(PurplePresence * presence)
		 * 
		 * Could not generate a wrapper for purple_presence_is_available in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * presence).
		 */

		/*
		 * gboolean purple_presence_is_online(PurplePresence * presence)
		 * 
		 * Could not generate a wrapper for purple_presence_is_online in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * presence).
		 */

		/*
		 * gboolean purple_presence_is_idle(PurplePresence * presence)
		 * 
		 * Could not generate a wrapper for purple_presence_is_idle in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * presence).
		 */

		/*
		 * time_t purple_presence_get_idle_time(PurplePresence * presence)
		 * 
		 * Could not generate a wrapper for purple_presence_get_idle_time in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * presence).
		 */

		/*
		 * time_t purple_presence_get_login_time(PurplePresence * presence)
		 * 
		 * Could not generate a wrapper for purple_presence_get_login_time in file "status.h".
		 * Message: The type could not be resolved (PurplePresence * presence).
		 */

		/*
		 * void * purple_status_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_get_handle();

		public static IntPtr GetHandle()
		{
			return purple_status_get_handle();
		}

		/*
		 * void purple_status_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_init();

		public static void Init()
		{
			purple_status_init();
		}

		/*
		 * void purple_status_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_uninit();

		public static void Uninit()
		{
			purple_status_uninit();
		}

	}
}

