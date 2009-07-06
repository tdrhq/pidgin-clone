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
	public class Prpl
	{
		public enum PurpleIconScaleRules
		{
			PURPLE_ICON_SCALE_DISPLAY = 0x01,
			PURPLE_ICON_SCALE_SEND = 0x02
		};

		public enum PurpleProtocolOptions
		{
			OPT_PROTO_UNIQUE_CHATNAME = 0x00000004,
			OPT_PROTO_CHAT_TOPIC = 0x00000008,
			OPT_PROTO_NO_PASSWORD = 0x00000010,
			OPT_PROTO_MAIL_CHECK = 0x00000020,
			OPT_PROTO_IM_IMAGE = 0x00000040,
			OPT_PROTO_PASSWORD_OPTIONAL = 0x00000080,
			OPT_PROTO_USE_POINTSIZE = 0x00000100,
			OPT_PROTO_REGISTER_NOSCREENNAME = 0x00000200,
			OPT_PROTO_SLASH_COMMANDS_NATIVE = 0x00000400
		};

		/*
		 * unsigned int( )
		 * 
		 * Could not generate a wrapper for int in file "prpl.h".
		 * Message: The type could not be resolved (unsigned int( )).
		 */

		/*
		 * void purple_attention_type_set_name(PurpleAttentionType * type, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_attention_type_set_name(IntPtr type, string name);

		public static void AttentionTypeSetName(PurpleAttentionType type, string name)
		{
			purple_attention_type_set_name(type.Reference, name);
		}

		/*
		 * void purple_attention_type_set_incoming_desc(PurpleAttentionType * type, char * desc)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_attention_type_set_incoming_desc(IntPtr type, string desc);

		public static void AttentionTypeSetIncomingDesc(PurpleAttentionType type, string desc)
		{
			purple_attention_type_set_incoming_desc(type.Reference, desc);
		}

		/*
		 * void purple_attention_type_set_outgoing_desc(PurpleAttentionType * type, char * desc)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_attention_type_set_outgoing_desc(IntPtr type, string desc);

		public static void AttentionTypeSetOutgoingDesc(PurpleAttentionType type, string desc)
		{
			purple_attention_type_set_outgoing_desc(type.Reference, desc);
		}

		/*
		 * void purple_attention_type_set_icon_name(PurpleAttentionType * type, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_attention_type_set_icon_name(IntPtr type, string name);

		public static void AttentionTypeSetIconName(PurpleAttentionType type, string name)
		{
			purple_attention_type_set_icon_name(type.Reference, name);
		}

		/*
		 * void purple_attention_type_set_unlocalized_name(PurpleAttentionType * type, char * ulname)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_attention_type_set_unlocalized_name(IntPtr type, string ulname);

		public static void AttentionTypeSetUnlocalizedName(PurpleAttentionType type, string ulname)
		{
			purple_attention_type_set_unlocalized_name(type.Reference, ulname);
		}

		/*
		 * char * purple_attention_type_get_name(PurpleAttentionType * type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_attention_type_get_name(IntPtr type);

		public static string AttentionTypeGetName(PurpleAttentionType type)
		{
			return purple_attention_type_get_name(type.Reference);
		}

		/*
		 * char * purple_attention_type_get_incoming_desc(PurpleAttentionType * type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_attention_type_get_incoming_desc(IntPtr type);

		public static string AttentionTypeGetIncomingDesc(PurpleAttentionType type)
		{
			return purple_attention_type_get_incoming_desc(type.Reference);
		}

		/*
		 * char * purple_attention_type_get_outgoing_desc(PurpleAttentionType * type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_attention_type_get_outgoing_desc(IntPtr type);

		public static string AttentionTypeGetOutgoingDesc(PurpleAttentionType type)
		{
			return purple_attention_type_get_outgoing_desc(type.Reference);
		}

		/*
		 * char * purple_attention_type_get_icon_name(PurpleAttentionType * type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_attention_type_get_icon_name(IntPtr type);

		public static string AttentionTypeGetIconName(PurpleAttentionType type)
		{
			return purple_attention_type_get_icon_name(type.Reference);
		}

		/*
		 * char * purple_attention_type_get_unlocalized_name(PurpleAttentionType * type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_attention_type_get_unlocalized_name(IntPtr type);

		public static string AttentionTypeGetUnlocalizedName(PurpleAttentionType type)
		{
			return purple_attention_type_get_unlocalized_name(type.Reference);
		}

		/*
		 * void purple_prpl_got_account_login_time(PurpleAccount * account, time_t login_time)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_got_account_login_time(IntPtr account, ulong login_time);

		public static void GotAccountLoginTime(PurpleAccount account, DateTime login_time)
		{
			ulong _PurpleWrapper_arg1 = (ulong)(login_time - new DateTime(1970, 1, 1)).TotalSeconds;
			purple_prpl_got_account_login_time(account.Reference, _PurpleWrapper_arg1);
		}

		/*
		 * void purple_prpl_got_account_actions(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_got_account_actions(IntPtr account);

		public static void GotAccountActions(PurpleAccount account)
		{
			purple_prpl_got_account_actions(account.Reference);
		}

		/*
		 * GList * purple_prpl_get_statuses(PurpleAccount * account, PurplePresence * presence)
		 * 
		 * Could not generate a wrapper for purple_prpl_get_statuses in file "prpl.h".
		 * Message: The type could not be resolved (GList * purple_prpl_get_statuses(PurpleAccount * account, PurplePresence * presence)).
		 */

		/*
		 * void purple_prpl_send_attention(PurpleConnection * gc, char * who, guint type_code)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_send_attention(IntPtr gc, string who, uint type_code);

		public static void SendAttention(PurpleConnection gc, string who, uint type_code)
		{
			purple_prpl_send_attention(gc.Reference, who, type_code);
		}

		/*
		 * void purple_prpl_got_attention(PurpleConnection * gc, char * who, guint type_code)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_got_attention(IntPtr gc, string who, uint type_code);

		public static void GotAttention(PurpleConnection gc, string who, uint type_code)
		{
			purple_prpl_got_attention(gc.Reference, who, type_code);
		}

		/*
		 * void purple_prpl_got_attention_in_chat(PurpleConnection * gc, int id, char * who, guint type_code)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_got_attention_in_chat(IntPtr gc, int id, string who, uint type_code);

		public static void GotAttentionInChat(PurpleConnection gc, int id, string who, uint type_code)
		{
			purple_prpl_got_attention_in_chat(gc.Reference, id, who, type_code);
		}

		/*
		 * PurplePlugin * purple_find_prpl(char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_find_prpl(string id);

		public static PurplePlugin FindPrpl(string id)
		{
			return new PurplePlugin(purple_find_prpl(id));
		}

	}
}

