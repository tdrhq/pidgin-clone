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
	public class Conversation
	{
		/*
		 * PurpleConversation * purple_conversation_new(PurpleConversationType type, PurpleAccount * account, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conversation_new(UNKNOWN type, IntPtr account, string name);

		public static PurpleConversation New(PurpleConversationType type, PurpleAccount account, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversation_destroy(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_destroy(IntPtr conv);

		public static void Destroy(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversation_present(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_present(IntPtr conv);

		public static void Present(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConversationType purple_conversation_get_type(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_conversation_get_type(IntPtr conv);

		public static PurpleConversationType GetType(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversation_set_ui_ops(PurpleConversation * conv, PurpleConversationUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_set_ui_ops(IntPtr conv, IntPtr ops);

		public static void SetUiOps(PurpleConversation conv, PurpleConversationUiOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversations_set_ui_ops(PurpleConversationUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversations_set_ui_ops(IntPtr ops);

		public static void ConversationsSetUiOps(PurpleConversationUiOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConversationUiOps * purple_conversation_get_ui_ops(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conversation_get_ui_ops(IntPtr conv);

		public static PurpleConversationUiOps GetUiOps(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversation_set_account(PurpleConversation * conv, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_set_account(IntPtr conv, IntPtr account);

		public static void SetAccount(PurpleConversation conv, PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccount * purple_conversation_get_account(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conversation_get_account(IntPtr conv);

		public static PurpleAccount GetAccount(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConnection * purple_conversation_get_gc(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conversation_get_gc(IntPtr conv);

		public static PurpleConnection GetGc(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversation_set_title(PurpleConversation * conv, char * title)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_set_title(IntPtr conv, string title);

		public static void SetTitle(PurpleConversation conv, string title)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_conversation_get_title(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_conversation_get_title(IntPtr conv);

		public static string GetTitle(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversation_autoset_title(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_autoset_title(IntPtr conv);

		public static void AutosetTitle(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversation_set_name(PurpleConversation * conv, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_set_name(IntPtr conv, string name);

		public static void SetName(PurpleConversation conv, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_conversation_get_name(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_conversation_get_name(IntPtr conv);

		public static string GetName(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversation_set_logging(PurpleConversation * conv, gboolean log)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_set_logging(IntPtr conv, bool log);

		public static void SetLogging(PurpleConversation conv, bool log)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_conversation_is_logging(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_conversation_is_logging(IntPtr conv);

		public static bool IsLogging(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversation_close_logs(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_close_logs(IntPtr conv);

		public static void CloseLogs(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConvIm * purple_conversation_get_im_data(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conversation_get_im_data(IntPtr conv);

		public static PurpleConvIm GetImData(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConvChat * purple_conversation_get_chat_data(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conversation_get_chat_data(IntPtr conv);

		public static PurpleConvChat GetChatData(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversation_set_data(PurpleConversation * conv, char * key, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_set_data(IntPtr conv, string key, IntPtr data);

		public static void SetData(PurpleConversation conv, string key, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * gpointer purple_conversation_get_data(PurpleConversation * conv, char * key)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conversation_get_data(IntPtr conv, string key);

		public static IntPtr GetData(PurpleConversation conv, string key)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_get_conversations()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_get_conversations();

		public static GList GetConversations()
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_get_ims()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_get_ims();

		public static GList GetIms()
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_get_chats()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_get_chats();

		public static GList GetChats()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConversation * purple_find_conversation_with_account(PurpleConversationType type, char * name, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_find_conversation_with_account(UNKNOWN type, string name, IntPtr account);

		public static PurpleConversation FindConversationWithAccount(PurpleConversationType type, string name, PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversation_write(PurpleConversation * conv, char * who, char * message, PurpleMessageFlags flags, time_t mtime)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_write(IntPtr conv, string who, string message, UNKNOWN flags, UNKNOWN mtime);

		public static void Write(PurpleConversation conv, string who, string message, PurpleMessageFlags flags, time_t mtime)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversation_set_features(PurpleConversation * conv, PurpleConnectionFlags features)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_set_features(IntPtr conv, UNKNOWN features);

		public static void SetFeatures(PurpleConversation conv, PurpleConnectionFlags features)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConnectionFlags purple_conversation_get_features(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_conversation_get_features(IntPtr conv);

		public static PurpleConnectionFlags GetFeatures(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_conversation_has_focus(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_conversation_has_focus(IntPtr conv);

		public static bool HasFocus(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversation_update(PurpleConversation * conv, PurpleConvUpdateType type)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_update(IntPtr conv, UNKNOWN type);

		public static void Update(PurpleConversation conv, PurpleConvUpdateType type)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversation_foreach( )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_foreach(UNKNOWN );

		public static void Foreach( )
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_conversation_get_message_history(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conversation_get_message_history(IntPtr conv);

		public static GList GetMessageHistory(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversation_clear_message_history(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_clear_message_history(IntPtr conv);

		public static void ClearMessageHistory(PurpleConversation conv)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_conversation_message_get_sender(PurpleConvMessage * msg)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_conversation_message_get_sender(IntPtr msg);

		public static string MessageGetSender(PurpleConvMessage msg)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_conversation_message_get_message(PurpleConvMessage * msg)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_conversation_message_get_message(IntPtr msg);

		public static string MessageGetMessage(PurpleConvMessage msg)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleMessageFlags purple_conversation_message_get_flags(PurpleConvMessage * msg)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_conversation_message_get_flags(IntPtr msg);

		public static PurpleMessageFlags MessageGetFlags(PurpleConvMessage msg)
		{
			throw new NotImplementedException();
		}

		/*
		 * time_t purple_conversation_message_get_timestamp(PurpleConvMessage * msg)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_conversation_message_get_timestamp(IntPtr msg);

		public static time_t MessageGetTimestamp(PurpleConvMessage msg)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConversation * purple_conv_im_get_conversation(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conv_im_get_conversation(IntPtr im);

		public static PurpleConversation ConvImGetConversation(PurpleConvIm im)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_im_set_icon(PurpleConvIm * im, PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_set_icon(IntPtr im, IntPtr icon);

		public static void ConvImSetIcon(PurpleConvIm im, PurpleBuddyIcon icon)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBuddyIcon * purple_conv_im_get_icon(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conv_im_get_icon(IntPtr im);

		public static PurpleBuddyIcon ConvImGetIcon(PurpleConvIm im)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_im_set_typing_state(PurpleConvIm * im, PurpleTypingState state)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_set_typing_state(IntPtr im, UNKNOWN state);

		public static void ConvImSetTypingState(PurpleConvIm im, PurpleTypingState state)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleTypingState purple_conv_im_get_typing_state(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_conv_im_get_typing_state(IntPtr im);

		public static PurpleTypingState ConvImGetTypingState(PurpleConvIm im)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_im_start_typing_timeout(PurpleConvIm * im, int timeout)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_start_typing_timeout(IntPtr im, int timeout);

		public static void ConvImStartTypingTimeout(PurpleConvIm im, int timeout)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_im_stop_typing_timeout(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_stop_typing_timeout(IntPtr im);

		public static void ConvImStopTypingTimeout(PurpleConvIm im)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint purple_conv_im_get_typing_timeout(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_conv_im_get_typing_timeout(IntPtr im);

		public static uint ConvImGetTypingTimeout(PurpleConvIm im)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_im_set_type_again(PurpleConvIm * im, unsigned int)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_set_type_again(IntPtr im, UNKNOWN int);

		public static void ConvImSetTypeAgain(PurpleConvIm im, unsigned int)
		{
			throw new NotImplementedException();
		}

		/*
		 * time_t purple_conv_im_get_type_again(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_conv_im_get_type_again(IntPtr im);

		public static time_t ConvImGetTypeAgain(PurpleConvIm im)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_im_start_send_typed_timeout(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_start_send_typed_timeout(IntPtr im);

		public static void ConvImStartSendTypedTimeout(PurpleConvIm im)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_im_stop_send_typed_timeout(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_stop_send_typed_timeout(IntPtr im);

		public static void ConvImStopSendTypedTimeout(PurpleConvIm im)
		{
			throw new NotImplementedException();
		}

		/*
		 * guint purple_conv_im_get_send_typed_timeout(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_conv_im_get_send_typed_timeout(IntPtr im);

		public static uint ConvImGetSendTypedTimeout(PurpleConvIm im)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_im_update_typing(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_update_typing(IntPtr im);

		public static void ConvImUpdateTyping(PurpleConvIm im)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_im_write(PurpleConvIm * im, char * who, char * message, PurpleMessageFlags flags, time_t mtime)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_write(IntPtr im, string who, string message, UNKNOWN flags, UNKNOWN mtime);

		public static void ConvImWrite(PurpleConvIm im, string who, string message, PurpleMessageFlags flags, time_t mtime)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_conv_present_error(char * who, PurpleAccount * account, char * what)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_conv_present_error(string who, IntPtr account, string what);

		public static bool ConvPresentError(string who, PurpleAccount account, string what)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_im_send(PurpleConvIm * im, char * message)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_send(IntPtr im, string message);

		public static void ConvImSend(PurpleConvIm im, string message)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_send_confirm(PurpleConversation * conv, char * message)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_send_confirm(IntPtr conv, string message);

		public static void ConvSendConfirm(PurpleConversation conv, string message)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_im_send_with_flags(PurpleConvIm * im, char * message, PurpleMessageFlags flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_send_with_flags(IntPtr im, string message, UNKNOWN flags);

		public static void ConvImSendWithFlags(PurpleConvIm im, string message, PurpleMessageFlags flags)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_conv_custom_smiley_add(PurpleConversation * conv, char * smile, char * cksum_type, char * chksum, gboolean remote)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_conv_custom_smiley_add(IntPtr conv, string smile, string cksum_type, string chksum, bool remote);

		public static bool ConvCustomSmileyAdd(PurpleConversation conv, string smile, string cksum_type, string chksum, bool remote)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_custom_smiley_write(PurpleConversation * conv, char * smile, guchar * data, gsize size)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_custom_smiley_write(IntPtr conv, string smile, IntPtr data, UNKNOWN size);

		public static void ConvCustomSmileyWrite(PurpleConversation conv, string smile, guchar data, gsize size)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_custom_smiley_close(PurpleConversation * conv, char * smile)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_custom_smiley_close(IntPtr conv, string smile);

		public static void ConvCustomSmileyClose(PurpleConversation conv, string smile)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConversation * purple_conv_chat_get_conversation(PurpleConvChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conv_chat_get_conversation(IntPtr chat);

		public static PurpleConversation ConvChatGetConversation(PurpleConvChat chat)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_conv_chat_set_users(PurpleConvChat * chat, GList * users)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conv_chat_set_users(IntPtr chat, IntPtr users);

		public static GList ConvChatSetUsers(PurpleConvChat chat, GList users)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_conv_chat_get_users(PurpleConvChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conv_chat_get_users(IntPtr chat);

		public static GList ConvChatGetUsers(PurpleConvChat chat)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_ignore(PurpleConvChat * chat, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_ignore(IntPtr chat, string name);

		public static void ConvChatIgnore(PurpleConvChat chat, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_unignore(PurpleConvChat * chat, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_unignore(IntPtr chat, string name);

		public static void ConvChatUnignore(PurpleConvChat chat, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_conv_chat_set_ignored(PurpleConvChat * chat, GList * ignored)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conv_chat_set_ignored(IntPtr chat, IntPtr ignored);

		public static GList ConvChatSetIgnored(PurpleConvChat chat, GList ignored)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_conv_chat_get_ignored(PurpleConvChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conv_chat_get_ignored(IntPtr chat);

		public static GList ConvChatGetIgnored(PurpleConvChat chat)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_conv_chat_get_ignored_user(PurpleConvChat * chat, char * user)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_conv_chat_get_ignored_user(IntPtr chat, string user);

		public static string ConvChatGetIgnoredUser(PurpleConvChat chat, string user)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_conv_chat_is_user_ignored(PurpleConvChat * chat, char * user)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_conv_chat_is_user_ignored(IntPtr chat, string user);

		public static bool ConvChatIsUserIgnored(PurpleConvChat chat, string user)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_set_topic(PurpleConvChat * chat, char * who, char * topic)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_set_topic(IntPtr chat, string who, string topic);

		public static void ConvChatSetTopic(PurpleConvChat chat, string who, string topic)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_conv_chat_get_topic(PurpleConvChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_conv_chat_get_topic(IntPtr chat);

		public static string ConvChatGetTopic(PurpleConvChat chat)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_set_id(PurpleConvChat * chat, int id)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_set_id(IntPtr chat, int id);

		public static void ConvChatSetId(PurpleConvChat chat, int id)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_conv_chat_get_id(PurpleConvChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_conv_chat_get_id(IntPtr chat);

		public static int ConvChatGetId(PurpleConvChat chat)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_write(PurpleConvChat * chat, char * who, char * message, PurpleMessageFlags flags, time_t mtime)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_write(IntPtr chat, string who, string message, UNKNOWN flags, UNKNOWN mtime);

		public static void ConvChatWrite(PurpleConvChat chat, string who, string message, PurpleMessageFlags flags, time_t mtime)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_send(PurpleConvChat * chat, char * message)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_send(IntPtr chat, string message);

		public static void ConvChatSend(PurpleConvChat chat, string message)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_send_with_flags(PurpleConvChat * chat, char * message, PurpleMessageFlags flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_send_with_flags(IntPtr chat, string message, UNKNOWN flags);

		public static void ConvChatSendWithFlags(PurpleConvChat chat, string message, PurpleMessageFlags flags)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_add_user(PurpleConvChat * chat, char * user, char * extra_msg, PurpleConvChatBuddyFlags flags, gboolean new_arrival)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_add_user(IntPtr chat, string user, string extra_msg, UNKNOWN flags, bool new_arrival);

		public static void ConvChatAddUser(PurpleConvChat chat, string user, string extra_msg, PurpleConvChatBuddyFlags flags, bool new_arrival)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_add_users(PurpleConvChat * chat, GList * users, GList * extra_msgs, GList * flags, gboolean new_arrivals)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_add_users(IntPtr chat, IntPtr users, IntPtr extra_msgs, IntPtr flags, bool new_arrivals);

		public static void ConvChatAddUsers(PurpleConvChat chat, GList users, GList extra_msgs, GList flags, bool new_arrivals)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_rename_user(PurpleConvChat * chat, char * old_user, char * new_user)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_rename_user(IntPtr chat, string old_user, string new_user);

		public static void ConvChatRenameUser(PurpleConvChat chat, string old_user, string new_user)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_remove_user(PurpleConvChat * chat, char * user, char * reason)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_remove_user(IntPtr chat, string user, string reason);

		public static void ConvChatRemoveUser(PurpleConvChat chat, string user, string reason)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_remove_users(PurpleConvChat * chat, GList * users, char * reason)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_remove_users(IntPtr chat, IntPtr users, string reason);

		public static void ConvChatRemoveUsers(PurpleConvChat chat, GList users, string reason)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_conv_chat_find_user(PurpleConvChat * chat, char * user)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_conv_chat_find_user(IntPtr chat, string user);

		public static bool ConvChatFindUser(PurpleConvChat chat, string user)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_user_set_flags(PurpleConvChat * chat, char * user, PurpleConvChatBuddyFlags flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_user_set_flags(IntPtr chat, string user, UNKNOWN flags);

		public static void ConvChatUserSetFlags(PurpleConvChat chat, string user, PurpleConvChatBuddyFlags flags)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConvChatBuddyFlags purple_conv_chat_user_get_flags(PurpleConvChat * chat, char * user)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_conv_chat_user_get_flags(IntPtr chat, string user);

		public static PurpleConvChatBuddyFlags ConvChatUserGetFlags(PurpleConvChat chat, string user)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_clear_users(PurpleConvChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_clear_users(IntPtr chat);

		public static void ConvChatClearUsers(PurpleConvChat chat)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_set_nick(PurpleConvChat * chat, char * nick)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_set_nick(IntPtr chat, string nick);

		public static void ConvChatSetNick(PurpleConvChat chat, string nick)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_conv_chat_get_nick(PurpleConvChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_conv_chat_get_nick(IntPtr chat);

		public static string ConvChatGetNick(PurpleConvChat chat)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConversation * purple_find_chat(PurpleConnection * gc, int id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_find_chat(IntPtr gc, int id);

		public static PurpleConversation FindChat(PurpleConnection gc, int id)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_left(PurpleConvChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_left(IntPtr chat);

		public static void ConvChatLeft(PurpleConvChat chat)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_invite_user(PurpleConvChat * chat, char * user, char * message, gboolean confirm)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_invite_user(IntPtr chat, string user, string message, bool confirm);

		public static void ConvChatInviteUser(PurpleConvChat chat, string user, string message, bool confirm)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_conv_chat_has_left(PurpleConvChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_conv_chat_has_left(IntPtr chat);

		public static bool ConvChatHasLeft(PurpleConvChat chat)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConvChatBuddy * purple_conv_chat_cb_new(char * name, char * alias, PurpleConvChatBuddyFlags flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conv_chat_cb_new(string name, string alias, UNKNOWN flags);

		public static PurpleConvChatBuddy ConvChatCbNew(string name, string alias, PurpleConvChatBuddyFlags flags)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleConvChatBuddy * purple_conv_chat_cb_find(PurpleConvChat * chat, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conv_chat_cb_find(IntPtr chat, string name);

		public static PurpleConvChatBuddy ConvChatCbFind(PurpleConvChat chat, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_conv_chat_cb_get_name(PurpleConvChatBuddy * cb)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_conv_chat_cb_get_name(IntPtr cb);

		public static string ConvChatCbGetName(PurpleConvChatBuddy cb)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conv_chat_cb_destroy(PurpleConvChatBuddy * cb)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_cb_destroy(IntPtr cb);

		public static void ConvChatCbDestroy(PurpleConvChatBuddy cb)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_conversation_do_command(PurpleConversation * conv, gchar * cmdline, gchar * markup, gchar ** error)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_conversation_do_command(IntPtr conv, string cmdline, string markup, IntPtr error);

		public static bool DoCommand(PurpleConversation conv, string cmdline, string markup, gchar error)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_conversations_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conversations_get_handle();

		public static IntPtr ConversationsGetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversations_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversations_init();

		public static void ConversationsInit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_conversations_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversations_uninit();

		public static void ConversationsUninit()
		{
			throw new NotImplementedException();
		}

	}
}

