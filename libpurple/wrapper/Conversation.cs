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
	 * File: conversation.h
	 */
	public class Conversation
	{
		public enum PurpleConversationType
		{
			PURPLE_CONV_TYPE_UNKNOWN = 0,
			PURPLE_CONV_TYPE_IM,
			PURPLE_CONV_TYPE_CHAT,
			PURPLE_CONV_TYPE_MISC,
			PURPLE_CONV_TYPE_ANY
		};

		public enum PurpleConvUpdateType
		{
			PURPLE_CONV_UPDATE_ADD = 0,
			PURPLE_CONV_UPDATE_REMOVE,
			PURPLE_CONV_UPDATE_ACCOUNT,
			PURPLE_CONV_UPDATE_TYPING,
			PURPLE_CONV_UPDATE_UNSEEN,
			PURPLE_CONV_UPDATE_LOGGING,
			PURPLE_CONV_UPDATE_TOPIC,
			PURPLE_CONV_ACCOUNT_ONLINE,
			PURPLE_CONV_ACCOUNT_OFFLINE,
			PURPLE_CONV_UPDATE_AWAY,
			PURPLE_CONV_UPDATE_ICON,
			PURPLE_CONV_UPDATE_TITLE,
			PURPLE_CONV_UPDATE_CHATLEFT,
			PURPLE_CONV_UPDATE_FEATURES
		};

		public enum PurpleTypingState
		{
			PURPLE_NOT_TYPING = 0,
			PURPLE_TYPING,
			PURPLE_TYPED
		};

		public enum PurpleMessageFlags
		{
			PURPLE_MESSAGE_SEND = 0x0001,
			PURPLE_MESSAGE_RECV = 0x0002,
			PURPLE_MESSAGE_SYSTEM = 0x0004,
			PURPLE_MESSAGE_AUTO_RESP = 0x0008,
			PURPLE_MESSAGE_ACTIVE_ONLY = 0x0010,
			PURPLE_MESSAGE_NICK = 0x0020,
			PURPLE_MESSAGE_NO_LOG = 0x0040,
			PURPLE_MESSAGE_WHISPER = 0x0080,
			PURPLE_MESSAGE_ERROR = 0x0200,
			PURPLE_MESSAGE_DELAYED = 0x0400,
			PURPLE_MESSAGE_RAW = 0x0800,
			PURPLE_MESSAGE_IMAGES = 0x1000,
			PURPLE_MESSAGE_NOTIFY = 0x2000,
			PURPLE_MESSAGE_NO_LINKIFY = 0x4000,
			PURPLE_MESSAGE_INVISIBLE = 0x8000
		};

		public enum PurpleConvChatBuddyFlags
		{
			PURPLE_CBFLAGS_NONE = 0x0000,
			PURPLE_CBFLAGS_VOICE = 0x0001,
			PURPLE_CBFLAGS_HALFOP = 0x0002,
			PURPLE_CBFLAGS_OP = 0x0004,
			PURPLE_CBFLAGS_FOUNDER = 0x0008,
			PURPLE_CBFLAGS_TYPING = 0x0010
		};


		/*
		 * Function Name: purple_conversation_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conversation_destroy(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_destroy(IntPtr conv);

		public static void Destroy(PurpleConversation conv)
		{
			purple_conversation_destroy(conv.Reference);
		}

		/*
		 * Function Name: purple_conversation_present
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conversation_present(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_present(IntPtr conv);

		public static void Present(PurpleConversation conv)
		{
			purple_conversation_present(conv.Reference);
		}

		/*
		 * Function Name: purple_conversation_get_type
		 * Function Type: PurpleConversationType
		 * Function RawType: PurpleConversationType
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleConversationType purple_conversation_get_type(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern Conversation.PurpleConversationType purple_conversation_get_type(IntPtr conv);

		public static Conversation.PurpleConversationType GetType(PurpleConversation conv)
		{
			/* Unable to process the return value of purple_conversation_get_type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_conversations_set_ui_ops
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conversations_set_ui_ops(PurpleConversationUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversations_set_ui_ops(IntPtr ops);

		public static void ConversationsSetUiOps(PurpleConversationUiOps ops)
		{
			purple_conversations_set_ui_ops(ops.Reference);
		}

		/*
		 * Function Name: purple_conversation_get_account
		 * Function Type: PurpleAccount *
		 * Function RawType: PurpleAccount
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleAccount * purple_conversation_get_account(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conversation_get_account(IntPtr conv);

		public static PurpleAccount GetAccount(PurpleConversation conv)
		{
			return new PurpleAccount(purple_conversation_get_account(conv.Reference));
		}

		/*
		 * Function Name: purple_conversation_get_gc
		 * Function Type: PurpleConnection *
		 * Function RawType: PurpleConnection
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleConnection * purple_conversation_get_gc(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conversation_get_gc(IntPtr conv);

		public static PurpleConnection GetGc(PurpleConversation conv)
		{
			return new PurpleConnection(purple_conversation_get_gc(conv.Reference));
		}

		/*
		 * Function Name: purple_conversation_set_title
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conversation_set_title(PurpleConversation * conv, char * title)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_set_title(IntPtr conv, string title);

		public static void SetTitle(PurpleConversation conv, string title)
		{
			purple_conversation_set_title(conv.Reference, title);
		}

		/*
		 * Function Name: purple_conversation_get_title
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_conversation_get_title(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_conversation_get_title(IntPtr conv);

		public static string GetTitle(PurpleConversation conv)
		{
			return purple_conversation_get_title(conv.Reference);
		}

		/*
		 * Function Name: purple_conversation_autoset_title
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conversation_autoset_title(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_autoset_title(IntPtr conv);

		public static void AutosetTitle(PurpleConversation conv)
		{
			purple_conversation_autoset_title(conv.Reference);
		}

		/*
		 * Function Name: purple_conversation_set_name
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conversation_set_name(PurpleConversation * conv, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_set_name(IntPtr conv, string name);

		public static void SetName(PurpleConversation conv, string name)
		{
			purple_conversation_set_name(conv.Reference, name);
		}

		/*
		 * Function Name: purple_conversation_get_name
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_conversation_get_name(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_conversation_get_name(IntPtr conv);

		public static string GetName(PurpleConversation conv)
		{
			return purple_conversation_get_name(conv.Reference);
		}

		/*
		 * Function Name: purple_conversation_set_logging
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conversation_set_logging(PurpleConversation * conv, gboolean log)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_set_logging(IntPtr conv, bool log);

		public static void SetLogging(PurpleConversation conv, bool log)
		{
			purple_conversation_set_logging(conv.Reference, log);
		}

		/*
		 * Function Name: purple_conversation_is_logging
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_conversation_is_logging(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_conversation_is_logging(IntPtr conv);

		public static bool IsLogging(PurpleConversation conv)
		{
			return purple_conversation_is_logging(conv.Reference);
		}

		/*
		 * Function Name: purple_conversation_close_logs
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conversation_close_logs(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_close_logs(IntPtr conv);

		public static void CloseLogs(PurpleConversation conv)
		{
			purple_conversation_close_logs(conv.Reference);
		}

		/*
		 * Function Name: purple_conversation_get_im_data
		 * Function Type: PurpleConvIm *
		 * Function RawType: PurpleConvIm
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleConvIm * purple_conversation_get_im_data(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conversation_get_im_data(IntPtr conv);

		public static PurpleConvIm GetImData(PurpleConversation conv)
		{
			return new PurpleConvIm(purple_conversation_get_im_data(conv.Reference));
		}

		/*
		 * Function Name: purple_conversation_get_chat_data
		 * Function Type: PurpleConvChat *
		 * Function RawType: PurpleConvChat
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleConvChat * purple_conversation_get_chat_data(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conversation_get_chat_data(IntPtr conv);

		public static PurpleConvChat GetChatData(PurpleConversation conv)
		{
			return new PurpleConvChat(purple_conversation_get_chat_data(conv.Reference));
		}

		/*
		 * Function Name: purple_conversation_get_data
		 * Function Type: gpointer
		 * Function RawType: gpointer
		 * Function Category: VoidPointer
		 */
		/*
		 * gpointer purple_conversation_get_data(PurpleConversation * conv, char * key)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conversation_get_data(IntPtr conv, string key);

		public static IntPtr GetData(PurpleConversation conv, string key)
		{
			return purple_conversation_get_data(conv.Reference, key);
		}

		/*
		 * Function Name: purple_get_conversations
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_get_conversations()
		 * 
		 * Could not generate a wrapper for purple_get_conversations in file "conversation.h".
		 * Message: Unknown inner-type of argument 0 of purple_get_conversations
		 */

		/*
		 * Function Name: purple_get_ims
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_get_ims()
		 * 
		 * Could not generate a wrapper for purple_get_ims in file "conversation.h".
		 * Message: Unknown inner-type of argument 0 of purple_get_ims
		 */

		/*
		 * Function Name: purple_get_chats
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_get_chats()
		 * 
		 * Could not generate a wrapper for purple_get_chats in file "conversation.h".
		 * Message: Unknown inner-type of argument 0 of purple_get_chats
		 */

		/*
		 * Function Name: purple_conversation_get_features
		 * Function Type: PurpleConnectionFlags
		 * Function RawType: PurpleConnectionFlags
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleConnectionFlags purple_conversation_get_features(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern Connection.PurpleConnectionFlags purple_conversation_get_features(IntPtr conv);

		public static Connection.PurpleConnectionFlags GetFeatures(PurpleConversation conv)
		{
			/* Unable to process the return value of purple_conversation_get_features, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_conversation_has_focus
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_conversation_has_focus(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_conversation_has_focus(IntPtr conv);

		public static bool HasFocus(PurpleConversation conv)
		{
			return purple_conversation_has_focus(conv.Reference);
		}

		/*
		 * Function Name: purple_conversation_update
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conversation_update(PurpleConversation * conv, PurpleConvUpdateType type)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_update(IntPtr conv, Conversation.PurpleConvUpdateType type);

		public static void Update(PurpleConversation conv, Conversation.PurpleConvUpdateType type)
		{
			/* Unable to process type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_conversation_get_message_history
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_conversation_get_message_history(PurpleConversation * conv)
		 * 
		 * Could not generate a wrapper for purple_conversation_get_message_history in file "conversation.h".
		 * Message: Unknown inner-type of argument 0 of purple_conversation_get_message_history
		 */

		/*
		 * Function Name: purple_conversation_clear_message_history
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conversation_clear_message_history(PurpleConversation * conv)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversation_clear_message_history(IntPtr conv);

		public static void ClearMessageHistory(PurpleConversation conv)
		{
			purple_conversation_clear_message_history(conv.Reference);
		}

		/*
		 * Function Name: purple_conversation_message_get_sender
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_conversation_message_get_sender(PurpleConvMessage * msg)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_conversation_message_get_sender(IntPtr msg);

		public static string MessageGetSender(PurpleConvMessage msg)
		{
			return purple_conversation_message_get_sender(msg.Reference);
		}

		/*
		 * Function Name: purple_conversation_message_get_message
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_conversation_message_get_message(PurpleConvMessage * msg)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_conversation_message_get_message(IntPtr msg);

		public static string MessageGetMessage(PurpleConvMessage msg)
		{
			return purple_conversation_message_get_message(msg.Reference);
		}

		/*
		 * Function Name: purple_conversation_message_get_flags
		 * Function Type: PurpleMessageFlags
		 * Function RawType: PurpleMessageFlags
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleMessageFlags purple_conversation_message_get_flags(PurpleConvMessage * msg)
		 */
		[DllImport("libpurple.dll")]
		private static extern Conversation.PurpleMessageFlags purple_conversation_message_get_flags(IntPtr msg);

		public static Conversation.PurpleMessageFlags MessageGetFlags(PurpleConvMessage msg)
		{
			/* Unable to process the return value of purple_conversation_message_get_flags, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_conversation_message_get_timestamp
		 * Function Type: time_t
		 * Function RawType: time_t
		 * Function Category: DateTime
		 */
		/*
		 * time_t purple_conversation_message_get_timestamp(PurpleConvMessage * msg)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_conversation_message_get_timestamp(IntPtr msg);

		public static DateTime MessageGetTimestamp(PurpleConvMessage msg)
		{
			/* Unable to process the return value of purple_conversation_message_get_timestamp, a DateTime. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_conv_im_get_conversation
		 * Function Type: PurpleConversation *
		 * Function RawType: PurpleConversation
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleConversation * purple_conv_im_get_conversation(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conv_im_get_conversation(IntPtr im);

		public static PurpleConversation ConvImGetConversation(PurpleConvIm im)
		{
			return new PurpleConversation(purple_conv_im_get_conversation(im.Reference));
		}

		/*
		 * Function Name: purple_conv_im_set_icon
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_im_set_icon(PurpleConvIm * im, PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_set_icon(IntPtr im, IntPtr icon);

		public static void ConvImSetIcon(PurpleConvIm im, /* Resolved as PointerToUnknownStruct to PurpleBuddyIcon */ IntPtr icon)
		{
			/* Unable to process icon, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_conv_im_get_icon
		 * Function Type: PurpleBuddyIcon *
		 * Function RawType: PurpleBuddyIcon
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleBuddyIcon * purple_conv_im_get_icon(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conv_im_get_icon(IntPtr im);

		public static /* Resolved as PointerToUnknownStruct to PurpleBuddyIcon */ IntPtr ConvImGetIcon(PurpleConvIm im)
		{
			/* Unable to process the return value of purple_conv_im_get_icon, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_conv_im_set_typing_state
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_im_set_typing_state(PurpleConvIm * im, PurpleTypingState state)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_set_typing_state(IntPtr im, Conversation.PurpleTypingState state);

		public static void ConvImSetTypingState(PurpleConvIm im, Conversation.PurpleTypingState state)
		{
			/* Unable to process state, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_conv_im_get_typing_state
		 * Function Type: PurpleTypingState
		 * Function RawType: PurpleTypingState
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleTypingState purple_conv_im_get_typing_state(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern Conversation.PurpleTypingState purple_conv_im_get_typing_state(IntPtr im);

		public static Conversation.PurpleTypingState ConvImGetTypingState(PurpleConvIm im)
		{
			/* Unable to process the return value of purple_conv_im_get_typing_state, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_conv_im_start_typing_timeout
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_im_start_typing_timeout(PurpleConvIm * im, int timeout)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_start_typing_timeout(IntPtr im, int timeout);

		public static void ConvImStartTypingTimeout(PurpleConvIm im, int timeout)
		{
			purple_conv_im_start_typing_timeout(im.Reference, timeout);
		}

		/*
		 * Function Name: purple_conv_im_stop_typing_timeout
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_im_stop_typing_timeout(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_stop_typing_timeout(IntPtr im);

		public static void ConvImStopTypingTimeout(PurpleConvIm im)
		{
			purple_conv_im_stop_typing_timeout(im.Reference);
		}

		/*
		 * Function Name: purple_conv_im_get_typing_timeout
		 * Function Type: guint
		 * Function RawType: guint
		 * Function Category: Native
		 */
		/*
		 * guint purple_conv_im_get_typing_timeout(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_conv_im_get_typing_timeout(IntPtr im);

		public static uint ConvImGetTypingTimeout(PurpleConvIm im)
		{
			return purple_conv_im_get_typing_timeout(im.Reference);
		}

		/*
		 * Function Name: purple_conv_im_set_type_again
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_im_set_type_again(PurpleConvIm * im, unsigned int val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_set_type_again(IntPtr im, uint val);

		public static void ConvImSetTypeAgain(PurpleConvIm im, uint val)
		{
			purple_conv_im_set_type_again(im.Reference, val);
		}

		/*
		 * Function Name: purple_conv_im_get_type_again
		 * Function Type: time_t
		 * Function RawType: time_t
		 * Function Category: DateTime
		 */
		/*
		 * time_t purple_conv_im_get_type_again(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_conv_im_get_type_again(IntPtr im);

		public static DateTime ConvImGetTypeAgain(PurpleConvIm im)
		{
			/* Unable to process the return value of purple_conv_im_get_type_again, a DateTime. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_conv_im_start_send_typed_timeout
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_im_start_send_typed_timeout(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_start_send_typed_timeout(IntPtr im);

		public static void ConvImStartSendTypedTimeout(PurpleConvIm im)
		{
			purple_conv_im_start_send_typed_timeout(im.Reference);
		}

		/*
		 * Function Name: purple_conv_im_stop_send_typed_timeout
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_im_stop_send_typed_timeout(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_stop_send_typed_timeout(IntPtr im);

		public static void ConvImStopSendTypedTimeout(PurpleConvIm im)
		{
			purple_conv_im_stop_send_typed_timeout(im.Reference);
		}

		/*
		 * Function Name: purple_conv_im_get_send_typed_timeout
		 * Function Type: guint
		 * Function RawType: guint
		 * Function Category: Native
		 */
		/*
		 * guint purple_conv_im_get_send_typed_timeout(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_conv_im_get_send_typed_timeout(IntPtr im);

		public static uint ConvImGetSendTypedTimeout(PurpleConvIm im)
		{
			return purple_conv_im_get_send_typed_timeout(im.Reference);
		}

		/*
		 * Function Name: purple_conv_im_update_typing
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_im_update_typing(PurpleConvIm * im)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_update_typing(IntPtr im);

		public static void ConvImUpdateTyping(PurpleConvIm im)
		{
			purple_conv_im_update_typing(im.Reference);
		}

		/*
		 * Function Name: purple_conv_present_error
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_conv_present_error(char * who, PurpleAccount * account, char * what)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_conv_present_error(string who, IntPtr account, string what);

		public static bool ConvPresentError(string who, PurpleAccount account, string what)
		{
			return purple_conv_present_error(who, account.Reference, what);
		}

		/*
		 * Function Name: purple_conv_im_send
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_im_send(PurpleConvIm * im, char * message)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_send(IntPtr im, string message);

		public static void ConvImSend(PurpleConvIm im, string message)
		{
			purple_conv_im_send(im.Reference, message);
		}

		/*
		 * Function Name: purple_conv_send_confirm
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_send_confirm(PurpleConversation * conv, char * message)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_send_confirm(IntPtr conv, string message);

		public static void ConvSendConfirm(PurpleConversation conv, string message)
		{
			purple_conv_send_confirm(conv.Reference, message);
		}

		/*
		 * Function Name: purple_conv_im_send_with_flags
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_im_send_with_flags(PurpleConvIm * im, char * message, PurpleMessageFlags flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_im_send_with_flags(IntPtr im, string message, Conversation.PurpleMessageFlags flags);

		public static void ConvImSendWithFlags(PurpleConvIm im, string message, Conversation.PurpleMessageFlags flags)
		{
			/* Unable to process flags, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_conv_custom_smiley_close
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_custom_smiley_close(PurpleConversation * conv, char * smile)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_custom_smiley_close(IntPtr conv, string smile);

		public static void ConvCustomSmileyClose(PurpleConversation conv, string smile)
		{
			purple_conv_custom_smiley_close(conv.Reference, smile);
		}

		/*
		 * Function Name: purple_conv_chat_get_conversation
		 * Function Type: PurpleConversation *
		 * Function RawType: PurpleConversation
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleConversation * purple_conv_chat_get_conversation(PurpleConvChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conv_chat_get_conversation(IntPtr chat);

		public static PurpleConversation ConvChatGetConversation(PurpleConvChat chat)
		{
			return new PurpleConversation(purple_conv_chat_get_conversation(chat.Reference));
		}

		/*
		 * Function Name: purple_conv_chat_set_users
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_conv_chat_set_users(PurpleConvChat * chat, GList * users)
		 * 
		 * Could not generate a wrapper for purple_conv_chat_set_users in file "conversation.h".
		 * Message: Unknown inner-type of argument 0 of purple_conv_chat_set_users
		 */

		/*
		 * Function Name: purple_conv_chat_get_users
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_conv_chat_get_users(PurpleConvChat * chat)
		 * 
		 * Could not generate a wrapper for purple_conv_chat_get_users in file "conversation.h".
		 * Message: Unknown inner-type of argument 0 of purple_conv_chat_get_users
		 */

		/*
		 * Function Name: purple_conv_chat_ignore
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_chat_ignore(PurpleConvChat * chat, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_ignore(IntPtr chat, string name);

		public static void ConvChatIgnore(PurpleConvChat chat, string name)
		{
			purple_conv_chat_ignore(chat.Reference, name);
		}

		/*
		 * Function Name: purple_conv_chat_unignore
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_chat_unignore(PurpleConvChat * chat, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_unignore(IntPtr chat, string name);

		public static void ConvChatUnignore(PurpleConvChat chat, string name)
		{
			purple_conv_chat_unignore(chat.Reference, name);
		}

		/*
		 * Function Name: purple_conv_chat_set_ignored
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_conv_chat_set_ignored(PurpleConvChat * chat, GList * ignored)
		 * 
		 * Could not generate a wrapper for purple_conv_chat_set_ignored in file "conversation.h".
		 * Message: Unknown inner-type of argument 0 of purple_conv_chat_set_ignored
		 */

		/*
		 * Function Name: purple_conv_chat_get_ignored
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_conv_chat_get_ignored(PurpleConvChat * chat)
		 * 
		 * Could not generate a wrapper for purple_conv_chat_get_ignored in file "conversation.h".
		 * Message: Unknown inner-type of argument 0 of purple_conv_chat_get_ignored
		 */

		/*
		 * Function Name: purple_conv_chat_get_topic
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_conv_chat_get_topic(PurpleConvChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_conv_chat_get_topic(IntPtr chat);

		public static string ConvChatGetTopic(PurpleConvChat chat)
		{
			return purple_conv_chat_get_topic(chat.Reference);
		}

		/*
		 * Function Name: purple_conv_chat_set_id
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_chat_set_id(PurpleConvChat * chat, int id)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_set_id(IntPtr chat, int id);

		public static void ConvChatSetId(PurpleConvChat chat, int id)
		{
			purple_conv_chat_set_id(chat.Reference, id);
		}

		/*
		 * Function Name: purple_conv_chat_get_id
		 * Function Type: int
		 * Function RawType: int
		 * Function Category: Native
		 */
		/*
		 * int purple_conv_chat_get_id(PurpleConvChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_conv_chat_get_id(IntPtr chat);

		public static int ConvChatGetId(PurpleConvChat chat)
		{
			return purple_conv_chat_get_id(chat.Reference);
		}

		/*
		 * Function Name: purple_conv_chat_send
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_chat_send(PurpleConvChat * chat, char * message)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_send(IntPtr chat, string message);

		public static void ConvChatSend(PurpleConvChat chat, string message)
		{
			purple_conv_chat_send(chat.Reference, message);
		}

		/*
		 * Function Name: purple_conv_chat_send_with_flags
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_chat_send_with_flags(PurpleConvChat * chat, char * message, PurpleMessageFlags flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_send_with_flags(IntPtr chat, string message, Conversation.PurpleMessageFlags flags);

		public static void ConvChatSendWithFlags(PurpleConvChat chat, string message, Conversation.PurpleMessageFlags flags)
		{
			/* Unable to process flags, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_conv_chat_find_user
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_conv_chat_find_user(PurpleConvChat * chat, char * user)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_conv_chat_find_user(IntPtr chat, string user);

		public static bool ConvChatFindUser(PurpleConvChat chat, string user)
		{
			return purple_conv_chat_find_user(chat.Reference, user);
		}

		/*
		 * Function Name: purple_conv_chat_clear_users
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_chat_clear_users(PurpleConvChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_clear_users(IntPtr chat);

		public static void ConvChatClearUsers(PurpleConvChat chat)
		{
			purple_conv_chat_clear_users(chat.Reference);
		}

		/*
		 * Function Name: purple_conv_chat_set_nick
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_chat_set_nick(PurpleConvChat * chat, char * nick)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_set_nick(IntPtr chat, string nick);

		public static void ConvChatSetNick(PurpleConvChat chat, string nick)
		{
			purple_conv_chat_set_nick(chat.Reference, nick);
		}

		/*
		 * Function Name: purple_conv_chat_get_nick
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_conv_chat_get_nick(PurpleConvChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_conv_chat_get_nick(IntPtr chat);

		public static string ConvChatGetNick(PurpleConvChat chat)
		{
			return purple_conv_chat_get_nick(chat.Reference);
		}

		/*
		 * Function Name: purple_find_chat
		 * Function Type: PurpleConversation *
		 * Function RawType: PurpleConversation
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleConversation * purple_find_chat(PurpleConnection * gc, int id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_find_chat(IntPtr gc, int id);

		public static PurpleConversation FindChat(PurpleConnection gc, int id)
		{
			return new PurpleConversation(purple_find_chat(gc.Reference, id));
		}

		/*
		 * Function Name: purple_conv_chat_left
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_chat_left(PurpleConvChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_left(IntPtr chat);

		public static void ConvChatLeft(PurpleConvChat chat)
		{
			purple_conv_chat_left(chat.Reference);
		}

		/*
		 * Function Name: purple_conv_chat_has_left
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_conv_chat_has_left(PurpleConvChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_conv_chat_has_left(IntPtr chat);

		public static bool ConvChatHasLeft(PurpleConvChat chat)
		{
			return purple_conv_chat_has_left(chat.Reference);
		}

		/*
		 * Function Name: purple_conv_chat_cb_find
		 * Function Type: PurpleConvChatBuddy *
		 * Function RawType: PurpleConvChatBuddy
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleConvChatBuddy * purple_conv_chat_cb_find(PurpleConvChat * chat, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conv_chat_cb_find(IntPtr chat, string name);

		public static PurpleConvChatBuddy ConvChatCbFind(PurpleConvChat chat, string name)
		{
			return new PurpleConvChatBuddy(purple_conv_chat_cb_find(chat.Reference, name));
		}

		/*
		 * Function Name: purple_conv_chat_cb_get_name
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_conv_chat_cb_get_name(PurpleConvChatBuddy * cb)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_conv_chat_cb_get_name(IntPtr cb);

		public static string ConvChatCbGetName(PurpleConvChatBuddy cb)
		{
			return purple_conv_chat_cb_get_name(cb.Reference);
		}

		/*
		 * Function Name: purple_conv_chat_cb_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conv_chat_cb_destroy(PurpleConvChatBuddy * cb)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conv_chat_cb_destroy(IntPtr cb);

		public static void ConvChatCbDestroy(PurpleConvChatBuddy cb)
		{
			purple_conv_chat_cb_destroy(cb.Reference);
		}

		/*
		 * Function Name: purple_conversation_get_extended_menu
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_conversation_get_extended_menu(PurpleConversation * conv)
		 * 
		 * Could not generate a wrapper for purple_conversation_get_extended_menu in file "conversation.h".
		 * Message: Unknown inner-type of argument 0 of purple_conversation_get_extended_menu
		 */

		/*
		 * Function Name: purple_conversation_do_command
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_conversation_do_command(PurpleConversation * conv, gchar * cmdline, gchar * markup, gchar ** error)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_conversation_do_command(IntPtr conv, string cmdline, string markup, IntPtr error);

		public static bool DoCommand(PurpleConversation conv, string cmdline, string markup, /* Resolved as PointerToUnknownStruct to gchar */ IntPtr error)
		{
			/* Unable to process error, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_conversations_get_handle
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
		 */
		/*
		 * void * purple_conversations_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_conversations_get_handle();

		public static IntPtr ConversationsGetHandle()
		{
			return purple_conversations_get_handle();
		}

		/*
		 * Function Name: purple_conversations_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conversations_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversations_init();

		public static void ConversationsInit()
		{
			purple_conversations_init();
		}

		/*
		 * Function Name: purple_conversations_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_conversations_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_conversations_uninit();

		public static void ConversationsUninit()
		{
			purple_conversations_uninit();
		}

	}
}

