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
	 * File: status.h
	 */
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
		 * Function Name: purple_primitive_get_id_from_type
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
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
		 * Function Name: purple_primitive_get_name_from_type
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
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
		 * Function Name: purple_primitive_get_type_from_id
		 * Function Type: PurpleStatusPrimitive
		 * Function RawType: PurpleStatusPrimitive
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleStatusPrimitive purple_primitive_get_type_from_id(char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern Status.PurpleStatusPrimitive purple_primitive_get_type_from_id(string id);

		public static Status.PurpleStatusPrimitive PrimitiveGetTypeFromId(string id)
		{
			/* Unable to process the return value of purple_primitive_get_type_from_id, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_type_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_status_type_destroy(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_type_destroy(IntPtr status_type);

		public static void TypeDestroy(/* Resolved as PointerToUnknownStruct to PurpleStatusType */ IntPtr status_type)
		{
			/* Unable to process status_type, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_type_get_id
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_status_type_get_id(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_status_type_get_id(IntPtr status_type);

		public static string TypeGetId(/* Resolved as PointerToUnknownStruct to PurpleStatusType */ IntPtr status_type)
		{
			/* Unable to process status_type, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_type_get_name
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_status_type_get_name(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_status_type_get_name(IntPtr status_type);

		public static string TypeGetName(/* Resolved as PointerToUnknownStruct to PurpleStatusType */ IntPtr status_type)
		{
			/* Unable to process status_type, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_type_is_saveable
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_status_type_is_saveable(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_type_is_saveable(IntPtr status_type);

		public static bool TypeIsSaveable(/* Resolved as PointerToUnknownStruct to PurpleStatusType */ IntPtr status_type)
		{
			/* Unable to process status_type, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_type_is_user_settable
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_status_type_is_user_settable(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_type_is_user_settable(IntPtr status_type);

		public static bool TypeIsUserSettable(/* Resolved as PointerToUnknownStruct to PurpleStatusType */ IntPtr status_type)
		{
			/* Unable to process status_type, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_type_is_independent
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_status_type_is_independent(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_type_is_independent(IntPtr status_type);

		public static bool TypeIsIndependent(/* Resolved as PointerToUnknownStruct to PurpleStatusType */ IntPtr status_type)
		{
			/* Unable to process status_type, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_type_is_exclusive
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_status_type_is_exclusive(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_type_is_exclusive(IntPtr status_type);

		public static bool TypeIsExclusive(/* Resolved as PointerToUnknownStruct to PurpleStatusType */ IntPtr status_type)
		{
			/* Unable to process status_type, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_type_is_available
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_status_type_is_available(PurpleStatusType * status_type)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_type_is_available(IntPtr status_type);

		public static bool TypeIsAvailable(/* Resolved as PointerToUnknownStruct to PurpleStatusType */ IntPtr status_type)
		{
			/* Unable to process status_type, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_type_get_primary_attr
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_status_type_get_primary_attr(PurpleStatusType * type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_status_type_get_primary_attr(IntPtr type);

		public static string TypeGetPrimaryAttr(/* Resolved as PointerToUnknownStruct to PurpleStatusType */ IntPtr type)
		{
			/* Unable to process type, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_type_get_attrs
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_status_type_get_attrs(PurpleStatusType * status_type)
		 * 
		 * Could not generate a wrapper for purple_status_type_get_attrs in file "status.h".
		 * Message: Unknown inner-type of argument 0 of purple_status_type_get_attrs
		 */

		/*
		 * Function Name: purple_status_attr_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_status_attr_destroy(PurpleStatusAttr * attr)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_attr_destroy(IntPtr attr);

		public static void AttrDestroy(/* Resolved as PointerToUnknownStruct to PurpleStatusAttr */ IntPtr attr)
		{
			/* Unable to process attr, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_attr_get_id
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_status_attr_get_id(PurpleStatusAttr * attr)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_status_attr_get_id(IntPtr attr);

		public static string AttrGetId(/* Resolved as PointerToUnknownStruct to PurpleStatusAttr */ IntPtr attr)
		{
			/* Unable to process attr, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_attr_get_name
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_status_attr_get_name(PurpleStatusAttr * attr)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_status_attr_get_name(IntPtr attr);

		public static string AttrGetName(/* Resolved as PointerToUnknownStruct to PurpleStatusAttr */ IntPtr attr)
		{
			/* Unable to process attr, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_attr_get_value
		 * Function Type: PurpleValue *
		 * Function RawType: PurpleValue
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleValue * purple_status_attr_get_value(PurpleStatusAttr * attr)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_attr_get_value(IntPtr attr);

		public static /* Resolved as PointerToUnknownStruct to PurpleValue */ IntPtr AttrGetValue(/* Resolved as PointerToUnknownStruct to PurpleStatusAttr */ IntPtr attr)
		{
			/* Unable to process attr, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_status_destroy(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_destroy(IntPtr status);

		public static void Destroy(/* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr status)
		{
			/* Unable to process status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_set_active
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_status_set_active(PurpleStatus * status, gboolean active)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_status_set_active(IntPtr status, bool active);

		public static void SetActive(/* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr status, bool active)
		{
			/* Unable to process status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_get_type
		 * Function Type: PurpleStatusType *
		 * Function RawType: PurpleStatusType
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleStatusType * purple_status_get_type(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_get_type(IntPtr status);

		public static /* Resolved as PointerToUnknownStruct to PurpleStatusType */ IntPtr GetType(/* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr status)
		{
			/* Unable to process status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_get_presence
		 * Function Type: PurplePresence *
		 * Function RawType: PurplePresence
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurplePresence * purple_status_get_presence(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_status_get_presence(IntPtr status);

		public static /* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr GetPresence(/* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr status)
		{
			/* Unable to process status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_get_id
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_status_get_id(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_status_get_id(IntPtr status);

		public static string GetId(/* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr status)
		{
			/* Unable to process status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_get_name
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_status_get_name(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_status_get_name(IntPtr status);

		public static string GetName(/* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr status)
		{
			/* Unable to process status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_is_independent
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_status_is_independent(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_is_independent(IntPtr status);

		public static bool IsIndependent(/* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr status)
		{
			/* Unable to process status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_is_exclusive
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_status_is_exclusive(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_is_exclusive(IntPtr status);

		public static bool IsExclusive(/* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr status)
		{
			/* Unable to process status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_is_available
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_status_is_available(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_is_available(IntPtr status);

		public static bool IsAvailable(/* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr status)
		{
			/* Unable to process status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_is_active
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_status_is_active(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_is_active(IntPtr status);

		public static bool IsActive(/* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr status)
		{
			/* Unable to process status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_is_online
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_status_is_online(PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_status_is_online(IntPtr status);

		public static bool IsOnline(/* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr status)
		{
			/* Unable to process status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_get_attr_int
		 * Function Type: int
		 * Function RawType: int
		 * Function Category: Native
		 */
		/*
		 * int purple_status_get_attr_int(PurpleStatus * status, char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_status_get_attr_int(IntPtr status, string id);

		public static int GetAttrInt(/* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr status, string id)
		{
			/* Unable to process status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_compare
		 * Function Type: gint
		 * Function RawType: gint
		 * Function Category: Native
		 */
		/*
		 * gint purple_status_compare(PurpleStatus * status1, PurpleStatus * status2)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_status_compare(IntPtr status1, IntPtr status2);

		public static int Compare(/* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr status1, /* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr status2)
		{
			/* Unable to process status1, a PointerToUnknownStruct. */
			/* Unable to process status2, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_new
		 * Function Type: PurplePresence *
		 * Function RawType: PurplePresence
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurplePresence * purple_presence_new(PurplePresenceContext context)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_new(Status.PurplePresenceContext context);

		public static /* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr PresenceNew(Status.PurplePresenceContext context)
		{
			/* Unable to process context, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_new_for_account
		 * Function Type: PurplePresence *
		 * Function RawType: PurplePresence
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurplePresence * purple_presence_new_for_account(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_new_for_account(IntPtr account);

		public static /* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr PresenceNewForAccount(PurpleAccount account)
		{
			/* Unable to process the return value of purple_presence_new_for_account, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_new_for_conv
		 * Function Type: PurplePresence *
		 * Function RawType: PurplePresence
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurplePresence * purple_presence_new_for_conv(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_new_for_conv(IntPtr conv);

		public static /* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr PresenceNewForConv(PurpleConversation conv)
		{
			/* Unable to process the return value of purple_presence_new_for_conv, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_new_for_buddy
		 * Function Type: PurplePresence *
		 * Function RawType: PurplePresence
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurplePresence * purple_presence_new_for_buddy(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_new_for_buddy(IntPtr buddy);

		public static /* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr PresenceNewForBuddy(PurpleBuddy buddy)
		{
			/* Unable to process the return value of purple_presence_new_for_buddy, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_presence_destroy(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_presence_destroy(IntPtr presence);

		public static void PresenceDestroy(/* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr presence)
		{
			/* Unable to process presence, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_add_status
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_presence_add_status(PurplePresence * presence, PurpleStatus * status)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_presence_add_status(IntPtr presence, IntPtr status);

		public static void PresenceAddStatus(/* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr presence, /* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr status)
		{
			/* Unable to process presence, a PointerToUnknownStruct. */
			/* Unable to process status, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_add_list
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_presence_add_list(PurplePresence * presence, GList * source_list)
		 * 
		 * Could not generate a wrapper for purple_presence_add_list in file "status.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Function Name: purple_presence_set_login_time
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_presence_set_login_time(PurplePresence * presence, time_t login_time)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_presence_set_login_time(IntPtr presence, ulong login_time);

		public static void PresenceSetLoginTime(/* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr presence, DateTime login_time)
		{
			/* Unable to process presence, a PointerToUnknownStruct. */
			ulong _PurpleWrapper_param1 = (ulong)(login_time - new DateTime(1970, 1, 1)).TotalSeconds;
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_get_context
		 * Function Type: PurplePresenceContext
		 * Function RawType: PurplePresenceContext
		 * Function Category: KnownEnum
		 */
		/*
		 * PurplePresenceContext purple_presence_get_context(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern Status.PurplePresenceContext purple_presence_get_context(IntPtr presence);

		public static Status.PurplePresenceContext PresenceGetContext(/* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr presence)
		{
			/* Unable to process presence, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_get_account
		 * Function Type: PurpleAccount *
		 * Function RawType: PurpleAccount
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleAccount * purple_presence_get_account(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_get_account(IntPtr presence);

		public static PurpleAccount PresenceGetAccount(/* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr presence)
		{
			/* Unable to process presence, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_get_conversation
		 * Function Type: PurpleConversation *
		 * Function RawType: PurpleConversation
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleConversation * purple_presence_get_conversation(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_get_conversation(IntPtr presence);

		public static PurpleConversation PresenceGetConversation(/* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr presence)
		{
			/* Unable to process presence, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_get_chat_user
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_presence_get_chat_user(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_presence_get_chat_user(IntPtr presence);

		public static string PresenceGetChatUser(/* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr presence)
		{
			/* Unable to process presence, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_get_buddy
		 * Function Type: PurpleBuddy *
		 * Function RawType: PurpleBuddy
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleBuddy * purple_presence_get_buddy(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_get_buddy(IntPtr presence);

		public static PurpleBuddy PresenceGetBuddy(/* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr presence)
		{
			/* Unable to process presence, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_get_statuses
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_presence_get_statuses(PurplePresence * presence)
		 * 
		 * Could not generate a wrapper for purple_presence_get_statuses in file "status.h".
		 * Message: Unknown inner-type of argument 0 of purple_presence_get_statuses
		 */

		/*
		 * Function Name: purple_presence_get_active_status
		 * Function Type: PurpleStatus *
		 * Function RawType: PurpleStatus
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleStatus * purple_presence_get_active_status(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_presence_get_active_status(IntPtr presence);

		public static /* Resolved as PointerToUnknownStruct to PurpleStatus */ IntPtr PresenceGetActiveStatus(/* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr presence)
		{
			/* Unable to process presence, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_is_available
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_presence_is_available(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_presence_is_available(IntPtr presence);

		public static bool PresenceIsAvailable(/* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr presence)
		{
			/* Unable to process presence, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_is_online
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_presence_is_online(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_presence_is_online(IntPtr presence);

		public static bool PresenceIsOnline(/* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr presence)
		{
			/* Unable to process presence, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_is_idle
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_presence_is_idle(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_presence_is_idle(IntPtr presence);

		public static bool PresenceIsIdle(/* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr presence)
		{
			/* Unable to process presence, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_get_idle_time
		 * Function Type: time_t
		 * Function RawType: time_t
		 * Function Category: DateTime
		 */
		/*
		 * time_t purple_presence_get_idle_time(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_presence_get_idle_time(IntPtr presence);

		public static DateTime PresenceGetIdleTime(/* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr presence)
		{
			/* Unable to process presence, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_presence_get_login_time
		 * Function Type: time_t
		 * Function RawType: time_t
		 * Function Category: DateTime
		 */
		/*
		 * time_t purple_presence_get_login_time(PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_presence_get_login_time(IntPtr presence);

		public static DateTime PresenceGetLoginTime(/* Resolved as PointerToUnknownStruct to PurplePresence */ IntPtr presence)
		{
			/* Unable to process presence, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_status_get_handle
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
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
		 * Function Name: purple_status_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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
		 * Function Name: purple_status_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
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

