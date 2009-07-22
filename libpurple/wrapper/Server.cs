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
	public class Server
	{
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
		 * void serv_move_buddy(PurpleBuddy * , PurpleGroup * , PurpleGroup * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_move_buddy(IntPtr _PurpleWrapper_arg0, IntPtr _PurpleWrapper_arg1, IntPtr _PurpleWrapper_arg2);

		public static void ServMoveBuddy(PurpleBuddy _PurpleWrapper_arg0, PurpleGroup _PurpleWrapper_arg1, PurpleGroup _PurpleWrapper_arg2)
		{
			serv_move_buddy(.Reference, .Reference, .Reference);
		}

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
		 * PurpleAttentionType * purple_get_attention_type_from_code(PurpleAccount * account, guint type_code)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_get_attention_type_from_code(IntPtr account, uint type_code);

		public static PurpleAttentionType GetAttentionTypeFromCode(PurpleAccount account, uint type_code)
		{
			return new PurpleAttentionType(purple_get_attention_type_from_code(account.Reference, type_code));
		}

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
		 * void serv_got_attention(PurpleConnection * gc, char * who, guint type_code)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_got_attention(IntPtr gc, string who, uint type_code);

		public static void ServGotAttention(PurpleConnection gc, string who, uint type_code)
		{
			serv_got_attention(gc.Reference, who, type_code);
		}

		/*
		 * void serv_get_info(PurpleConnection * , char * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_get_info(IntPtr _PurpleWrapper_arg0, string _PurpleWrapper_arg1);

		public static void ServGetInfo(PurpleConnection _PurpleWrapper_arg0, string _PurpleWrapper_arg1)
		{
			serv_get_info(.Reference, );
		}

		/*
		 * void serv_set_info(PurpleConnection * , char * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_set_info(IntPtr _PurpleWrapper_arg0, string _PurpleWrapper_arg1);

		public static void ServSetInfo(PurpleConnection _PurpleWrapper_arg0, string _PurpleWrapper_arg1)
		{
			serv_set_info(.Reference, );
		}

		/*
		 * void serv_add_permit(PurpleConnection * , char * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_add_permit(IntPtr _PurpleWrapper_arg0, string _PurpleWrapper_arg1);

		public static void ServAddPermit(PurpleConnection _PurpleWrapper_arg0, string _PurpleWrapper_arg1)
		{
			serv_add_permit(.Reference, );
		}

		/*
		 * void serv_add_deny(PurpleConnection * , char * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_add_deny(IntPtr _PurpleWrapper_arg0, string _PurpleWrapper_arg1);

		public static void ServAddDeny(PurpleConnection _PurpleWrapper_arg0, string _PurpleWrapper_arg1)
		{
			serv_add_deny(.Reference, );
		}

		/*
		 * void serv_rem_permit(PurpleConnection * , char * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_rem_permit(IntPtr _PurpleWrapper_arg0, string _PurpleWrapper_arg1);

		public static void ServRemPermit(PurpleConnection _PurpleWrapper_arg0, string _PurpleWrapper_arg1)
		{
			serv_rem_permit(.Reference, );
		}

		/*
		 * void serv_rem_deny(PurpleConnection * , char * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_rem_deny(IntPtr _PurpleWrapper_arg0, string _PurpleWrapper_arg1);

		public static void ServRemDeny(PurpleConnection _PurpleWrapper_arg0, string _PurpleWrapper_arg1)
		{
			serv_rem_deny(.Reference, );
		}

		/*
		 * void serv_set_permit_deny(PurpleConnection * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_set_permit_deny(IntPtr _PurpleWrapper_arg0);

		public static void ServSetPermitDeny(PurpleConnection _PurpleWrapper_arg0)
		{
			serv_set_permit_deny(.Reference);
		}

		/*
		 * void serv_chat_invite(PurpleConnection * ,  , char * , char * )
		 * 
		 * Could not generate a wrapper for serv_chat_invite in file "server.h".
		 * Message: The type could not be resolved ( ).
		 */

		/*
		 * void serv_chat_leave(PurpleConnection * ,  )
		 * 
		 * Could not generate a wrapper for serv_chat_leave in file "server.h".
		 * Message: The type could not be resolved ( ).
		 */

		/*
		 * void serv_chat_whisper(PurpleConnection * ,  , char * , char * )
		 * 
		 * Could not generate a wrapper for serv_chat_whisper in file "server.h".
		 * Message: The type could not be resolved ( ).
		 */

		/*
		 * int serv_chat_send(PurpleConnection * ,  , char * , PurpleMessageFlags flags)
		 * 
		 * Could not generate a wrapper for serv_chat_send in file "server.h".
		 * Message: The type could not be resolved ( ).
		 */

		/*
		 * void serv_alias_buddy(PurpleBuddy * )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_alias_buddy(IntPtr _PurpleWrapper_arg0);

		public static void ServAliasBuddy(PurpleBuddy _PurpleWrapper_arg0)
		{
			serv_alias_buddy(.Reference);
		}

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
		 * void purple_serv_got_private_alias(PurpleConnection * gc, char * who, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_serv_got_private_alias(IntPtr gc, string who, string alias);

		public static void ServGotPrivateAlias(PurpleConnection gc, string who, string alias)
		{
			purple_serv_got_private_alias(gc.Reference, who, alias);
		}

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
		 * void serv_join_chat(PurpleConnection * , GHashTable * data)
		 * 
		 * Could not generate a wrapper for serv_join_chat in file "server.h".
		 * Message: The type could not be resolved (GHashTable * data).
		 */

		/*
		 * void serv_reject_chat(PurpleConnection * , GHashTable * data)
		 * 
		 * Could not generate a wrapper for serv_reject_chat in file "server.h".
		 * Message: The type could not be resolved (GHashTable * data).
		 */

		/*
		 * void purple_serv_got_join_chat_failed(PurpleConnection * gc, GHashTable * data)
		 * 
		 * Could not generate a wrapper for purple_serv_got_join_chat_failed in file "server.h".
		 * Message: The type could not be resolved (GHashTable * data).
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

