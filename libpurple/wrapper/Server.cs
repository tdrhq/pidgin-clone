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
	public class Server
	{
		/*
		 * int serv_send_typing(PurpleConnection * gc, char * name, PurpleTypingState state)
		 */
		[DllImport("libpurple.dll")]
		private static extern int serv_send_typing(IntPtr gc, string name, UNKNOWN state);

		public static int ServSendTyping(PurpleConnection gc, string name, PurpleTypingState state)
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_move_buddy( ,  ,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_move_buddy(UNKNOWN , UNKNOWN , UNKNOWN );

		public static void ServMoveBuddy( ,  ,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * int serv_send_im( ,  ,  , PurpleMessageFlags flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern int serv_send_im(UNKNOWN , UNKNOWN , UNKNOWN , UNKNOWN flags);

		public static int ServSendIm( ,  ,  , PurpleMessageFlags flags)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAttentionType * purple_get_attention_type_from_code(PurpleAccount * account, guint type_code)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_get_attention_type_from_code(IntPtr account, uint type_code);

		public static PurpleAttentionType GetAttentionTypeFromCode(PurpleAccount account, uint type_code)
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_send_attention(PurpleConnection * gc, char * who, guint type_code)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_send_attention(IntPtr gc, string who, uint type_code);

		public static void ServSendAttention(PurpleConnection gc, string who, uint type_code)
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_got_attention(PurpleConnection * gc, char * who, guint type_code)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_got_attention(IntPtr gc, string who, uint type_code);

		public static void ServGotAttention(PurpleConnection gc, string who, uint type_code)
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_get_info( ,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_get_info(UNKNOWN , UNKNOWN );

		public static void ServGetInfo( ,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_set_info( ,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_set_info(UNKNOWN , UNKNOWN );

		public static void ServSetInfo( ,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_add_permit( ,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_add_permit(UNKNOWN , UNKNOWN );

		public static void ServAddPermit( ,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_add_deny( ,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_add_deny(UNKNOWN , UNKNOWN );

		public static void ServAddDeny( ,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_rem_permit( ,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_rem_permit(UNKNOWN , UNKNOWN );

		public static void ServRemPermit( ,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_rem_deny( ,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_rem_deny(UNKNOWN , UNKNOWN );

		public static void ServRemDeny( ,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_set_permit_deny( )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_set_permit_deny(UNKNOWN );

		public static void ServSetPermitDeny( )
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_chat_invite( ,  ,  ,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_chat_invite(UNKNOWN , UNKNOWN , UNKNOWN , UNKNOWN );

		public static void ServChatInvite( ,  ,  ,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_chat_leave( ,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_chat_leave(UNKNOWN , UNKNOWN );

		public static void ServChatLeave( ,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_chat_whisper( ,  ,  ,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_chat_whisper(UNKNOWN , UNKNOWN , UNKNOWN , UNKNOWN );

		public static void ServChatWhisper( ,  ,  ,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * int serv_chat_send( ,  ,  , PurpleMessageFlags flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern int serv_chat_send(UNKNOWN , UNKNOWN , UNKNOWN , UNKNOWN flags);

		public static int ServChatSend( ,  ,  , PurpleMessageFlags flags)
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_alias_buddy( )
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_alias_buddy(UNKNOWN );

		public static void ServAliasBuddy( )
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_got_alias(PurpleConnection * gc, char * who, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_got_alias(IntPtr gc, string who, string alias);

		public static void ServGotAlias(PurpleConnection gc, string who, string alias)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_serv_got_private_alias(PurpleConnection * gc, char * who, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_serv_got_private_alias(IntPtr gc, string who, string alias);

		public static void ServGotPrivateAlias(PurpleConnection gc, string who, string alias)
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_got_typing(PurpleConnection * gc, char * name, int timeout, PurpleTypingState state)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_got_typing(IntPtr gc, string name, int timeout, UNKNOWN state);

		public static void ServGotTyping(PurpleConnection gc, string name, int timeout, PurpleTypingState state)
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_got_typing_stopped(PurpleConnection * gc, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_got_typing_stopped(IntPtr gc, string name);

		public static void ServGotTypingStopped(PurpleConnection gc, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_got_im(PurpleConnection * gc, char * who, char * msg, PurpleMessageFlags flags, time_t mtime)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_got_im(IntPtr gc, string who, string msg, UNKNOWN flags, UNKNOWN mtime);

		public static void ServGotIm(PurpleConnection gc, string who, string msg, PurpleMessageFlags flags, time_t mtime)
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_join_chat( , GHashTable * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_join_chat(UNKNOWN , IntPtr data);

		public static void ServJoinChat( , GHashTable data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_reject_chat( , GHashTable * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_reject_chat(UNKNOWN , IntPtr data);

		public static void ServRejectChat( , GHashTable data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_got_chat_invite(PurpleConnection * gc, char * name, char * who, char * message, GHashTable * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_got_chat_invite(IntPtr gc, string name, string who, string message, IntPtr data);

		public static void ServGotChatInvite(PurpleConnection gc, string name, string who, string message, GHashTable data)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConversation * serv_got_joined_chat(PurpleConnection * gc, int id, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr serv_got_joined_chat(IntPtr gc, int id, string name);

		public static PurpleConversation ServGotJoinedChat(PurpleConnection gc, int id, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_serv_got_join_chat_failed(PurpleConnection * gc, GHashTable * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_serv_got_join_chat_failed(IntPtr gc, IntPtr data);

		public static void ServGotJoinChatFailed(PurpleConnection gc, GHashTable data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_got_chat_left(PurpleConnection * g, int id)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_got_chat_left(IntPtr g, int id);

		public static void ServGotChatLeft(PurpleConnection g, int id)
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_got_chat_in(PurpleConnection * g, int id, char * who, PurpleMessageFlags flags, char * message, time_t mtime)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_got_chat_in(IntPtr g, int id, string who, UNKNOWN flags, string message, UNKNOWN mtime);

		public static void ServGotChatIn(PurpleConnection g, int id, string who, PurpleMessageFlags flags, string message, time_t mtime)
		{
			throw new NotImplementedException();
		}

		/*
		 * void serv_send_file(PurpleConnection * gc, char * who, char * file)
		 */
		[DllImport("libpurple.dll")]
		private static extern void serv_send_file(IntPtr gc, string who, string file);

		public static void ServSendFile(PurpleConnection gc, string who, string file)
		{
			throw new NotImplementedException();
		}

	}
}

