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
	 * File: server.h
	 */
	public class Server
	{

		/*
		 * Function Name: serv_send_typing
		 * Function Type: unsigned int
		 * Function RawType: int
		 * Function Category: Native
		 */
		/*
		 * unsigned int serv_send_typing(PurpleConnection * gc, char * name, PurpleTypingState state)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint serv_send_typing(IntPtr gc, string name, Conversation.PurpleTypingState state);

		public static uint ServSendTyping(PurpleConnection gc, string name, Conversation.PurpleTypingState state)
		{
			/* Unable to process state, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: serv_move_buddy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_move_buddy(PurpleBuddy * , PurpleGroup * , PurpleGroup * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_move_buddy(IntPtr _PurpleWrapper_arg0, IntPtr _PurpleWrapper_arg1, IntPtr _PurpleWrapper_arg2);

		public static void ServMoveBuddy(PurpleBuddy _PurpleWrapper_arg0, PurpleGroup _PurpleWrapper_arg1, PurpleGroup _PurpleWrapper_arg2)
		{
			serv_move_buddy(_PurpleWrapper_arg0.Reference, _PurpleWrapper_arg1.Reference, _PurpleWrapper_arg2.Reference);
		}

		/*
		 * Function Name: serv_send_im
		 * Function Type: int
		 * Function RawType: int
		 * Function Category: Native
		 */
		/*
		 * int serv_send_im(PurpleConnection * , char * , char * , PurpleMessageFlags flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern int serv_send_im(IntPtr _PurpleWrapper_arg0, string _PurpleWrapper_arg1, string _PurpleWrapper_arg2, Conversation.PurpleMessageFlags flags);

		public static int ServSendIm(PurpleConnection _PurpleWrapper_arg0, string _PurpleWrapper_arg1, string _PurpleWrapper_arg2, Conversation.PurpleMessageFlags flags)
		{
			/* Unable to process flags, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_get_attention_type_from_code
		 * Function Type: PurpleAttentionType *
		 * Function RawType: PurpleAttentionType
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleAttentionType * purple_get_attention_type_from_code(PurpleAccount * account, guint type_code)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_get_attention_type_from_code(IntPtr account, uint type_code);

		public static PurpleAttentionType GetAttentionTypeFromCode(PurpleAccount account, uint type_code)
		{
			return new PurpleAttentionType(purple_get_attention_type_from_code(account.Reference, type_code));
		}

		/*
		 * Function Name: serv_send_attention
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_send_attention(PurpleConnection * gc, char * who, guint type_code)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_send_attention(IntPtr gc, string who, uint type_code);

		public static void ServSendAttention(PurpleConnection gc, string who, uint type_code)
		{
			serv_send_attention(gc.Reference, who, type_code);
		}

		/*
		 * Function Name: serv_got_attention
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_got_attention(PurpleConnection * gc, char * who, guint type_code)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_got_attention(IntPtr gc, string who, uint type_code);

		public static void ServGotAttention(PurpleConnection gc, string who, uint type_code)
		{
			serv_got_attention(gc.Reference, who, type_code);
		}

		/*
		 * Function Name: serv_get_info
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_get_info(PurpleConnection * , char * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_get_info(IntPtr _PurpleWrapper_arg0, string _PurpleWrapper_arg1);

		public static void ServGetInfo(PurpleConnection _PurpleWrapper_arg0, string _PurpleWrapper_arg1)
		{
			serv_get_info(_PurpleWrapper_arg0.Reference, _PurpleWrapper_arg1);
		}

		/*
		 * Function Name: serv_set_info
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_set_info(PurpleConnection * , char * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_set_info(IntPtr _PurpleWrapper_arg0, string _PurpleWrapper_arg1);

		public static void ServSetInfo(PurpleConnection _PurpleWrapper_arg0, string _PurpleWrapper_arg1)
		{
			serv_set_info(_PurpleWrapper_arg0.Reference, _PurpleWrapper_arg1);
		}

		/*
		 * Function Name: serv_add_permit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_add_permit(PurpleConnection * , char * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_add_permit(IntPtr _PurpleWrapper_arg0, string _PurpleWrapper_arg1);

		public static void ServAddPermit(PurpleConnection _PurpleWrapper_arg0, string _PurpleWrapper_arg1)
		{
			serv_add_permit(_PurpleWrapper_arg0.Reference, _PurpleWrapper_arg1);
		}

		/*
		 * Function Name: serv_add_deny
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_add_deny(PurpleConnection * , char * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_add_deny(IntPtr _PurpleWrapper_arg0, string _PurpleWrapper_arg1);

		public static void ServAddDeny(PurpleConnection _PurpleWrapper_arg0, string _PurpleWrapper_arg1)
		{
			serv_add_deny(_PurpleWrapper_arg0.Reference, _PurpleWrapper_arg1);
		}

		/*
		 * Function Name: serv_rem_permit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_rem_permit(PurpleConnection * , char * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_rem_permit(IntPtr _PurpleWrapper_arg0, string _PurpleWrapper_arg1);

		public static void ServRemPermit(PurpleConnection _PurpleWrapper_arg0, string _PurpleWrapper_arg1)
		{
			serv_rem_permit(_PurpleWrapper_arg0.Reference, _PurpleWrapper_arg1);
		}

		/*
		 * Function Name: serv_rem_deny
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_rem_deny(PurpleConnection * , char * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_rem_deny(IntPtr _PurpleWrapper_arg0, string _PurpleWrapper_arg1);

		public static void ServRemDeny(PurpleConnection _PurpleWrapper_arg0, string _PurpleWrapper_arg1)
		{
			serv_rem_deny(_PurpleWrapper_arg0.Reference, _PurpleWrapper_arg1);
		}

		/*
		 * Function Name: serv_set_permit_deny
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_set_permit_deny(PurpleConnection * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_set_permit_deny(IntPtr _PurpleWrapper_arg0);

		public static void ServSetPermitDeny(PurpleConnection _PurpleWrapper_arg0)
		{
			serv_set_permit_deny(_PurpleWrapper_arg0.Reference);
		}

		/*
		 * Function Name: serv_chat_invite
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_chat_invite(PurpleConnection * ,  , char * , char * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_chat_invite(IntPtr _PurpleWrapper_arg0, /* Unresolved: [Category: Unknown], [Name:  ] */ IntPtr _PurpleWrapper_arg1, string _PurpleWrapper_arg2, string _PurpleWrapper_arg3);

		public static void ServChatInvite(PurpleConnection _PurpleWrapper_arg0, /* Unresolved: [Category: Unknown], [Name:  ] */ IntPtr _PurpleWrapper_arg1, string _PurpleWrapper_arg2, string _PurpleWrapper_arg3)
		{
			/* Unable to process , a Unknown. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: serv_chat_leave
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_chat_leave(PurpleConnection * ,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_chat_leave(IntPtr _PurpleWrapper_arg0, /* Unresolved: [Category: Unknown], [Name:  ] */ IntPtr _PurpleWrapper_arg1);

		public static void ServChatLeave(PurpleConnection _PurpleWrapper_arg0, /* Unresolved: [Category: Unknown], [Name:  ] */ IntPtr _PurpleWrapper_arg1)
		{
			/* Unable to process , a Unknown. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: serv_chat_whisper
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_chat_whisper(PurpleConnection * ,  , char * , char * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_chat_whisper(IntPtr _PurpleWrapper_arg0, /* Unresolved: [Category: Unknown], [Name:  ] */ IntPtr _PurpleWrapper_arg1, string _PurpleWrapper_arg2, string _PurpleWrapper_arg3);

		public static void ServChatWhisper(PurpleConnection _PurpleWrapper_arg0, /* Unresolved: [Category: Unknown], [Name:  ] */ IntPtr _PurpleWrapper_arg1, string _PurpleWrapper_arg2, string _PurpleWrapper_arg3)
		{
			/* Unable to process , a Unknown. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: serv_chat_send
		 * Function Type: int
		 * Function RawType: int
		 * Function Category: Native
		 */
		/*
		 * int serv_chat_send(PurpleConnection * ,  , char * , PurpleMessageFlags flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern int serv_chat_send(IntPtr _PurpleWrapper_arg0, /* Unresolved: [Category: Unknown], [Name:  ] */ IntPtr _PurpleWrapper_arg1, string _PurpleWrapper_arg2, Conversation.PurpleMessageFlags flags);

		public static int ServChatSend(PurpleConnection _PurpleWrapper_arg0, /* Unresolved: [Category: Unknown], [Name:  ] */ IntPtr _PurpleWrapper_arg1, string _PurpleWrapper_arg2, Conversation.PurpleMessageFlags flags)
		{
			/* Unable to process , a Unknown. */
			/* Unable to process flags, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: serv_alias_buddy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_alias_buddy(PurpleBuddy * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_alias_buddy(IntPtr _PurpleWrapper_arg0);

		public static void ServAliasBuddy(PurpleBuddy _PurpleWrapper_arg0)
		{
			serv_alias_buddy(_PurpleWrapper_arg0.Reference);
		}

		/*
		 * Function Name: serv_got_alias
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_got_alias(PurpleConnection * gc, char * who, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_got_alias(IntPtr gc, string who, string alias);

		public static void ServGotAlias(PurpleConnection gc, string who, string alias)
		{
			serv_got_alias(gc.Reference, who, alias);
		}

		/*
		 * Function Name: purple_serv_got_private_alias
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_serv_got_private_alias(PurpleConnection * gc, char * who, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_serv_got_private_alias(IntPtr gc, string who, string alias);

		public static void ServGotPrivateAlias(PurpleConnection gc, string who, string alias)
		{
			purple_serv_got_private_alias(gc.Reference, who, alias);
		}

		/*
		 * Function Name: serv_got_typing_stopped
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_got_typing_stopped(PurpleConnection * gc, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_got_typing_stopped(IntPtr gc, string name);

		public static void ServGotTypingStopped(PurpleConnection gc, string name)
		{
			serv_got_typing_stopped(gc.Reference, name);
		}

		/*
		 * Function Name: serv_join_chat
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_join_chat(PurpleConnection * , GHashTable * data)
		 * 
		 * Could not generate a wrapper for serv_join_chat in file "server.h".
		 * Message: Unable to provide a type for HashTable as no function information was provided in code.
		 */

		/*
		 * Function Name: serv_reject_chat
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_reject_chat(PurpleConnection * , GHashTable * data)
		 * 
		 * Could not generate a wrapper for serv_reject_chat in file "server.h".
		 * Message: Unable to provide a type for HashTable as no function information was provided in code.
		 */

		/*
		 * Function Name: purple_serv_got_join_chat_failed
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_serv_got_join_chat_failed(PurpleConnection * gc, GHashTable * data)
		 * 
		 * Could not generate a wrapper for purple_serv_got_join_chat_failed in file "server.h".
		 * Message: Unable to provide a type for HashTable as no function information was provided in code.
		 */

		/*
		 * Function Name: serv_got_chat_left
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_got_chat_left(PurpleConnection * g, int id)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_got_chat_left(IntPtr g, int id);

		public static void ServGotChatLeft(PurpleConnection g, int id)
		{
			serv_got_chat_left(g.Reference, id);
		}

		/*
		 * Function Name: serv_send_file
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void serv_send_file(PurpleConnection * gc, char * who, char * file)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_send_file(IntPtr gc, string who, string file);

		public static void ServSendFile(PurpleConnection gc, string who, string file)
		{
			serv_send_file(gc.Reference, who, file);
		}

	}
}

