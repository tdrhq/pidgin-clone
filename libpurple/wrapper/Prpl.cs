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
	public class Prpl
	{
		/*
		 * unsigned int( )
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN int(UNKNOWN );

		public static unsigned Int( )
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAttentionType * purple_attention_type_new(char * ulname, char * name, char * inc_desc, char * out_desc)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_attention_type_new(string ulname, string name, string inc_desc, string out_desc);

		public static PurpleAttentionType AttentionTypeNew(string ulname, string name, string inc_desc, string out_desc)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_attention_type_set_name(PurpleAttentionType * type, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_attention_type_set_name(IntPtr type, string name);

		public static void AttentionTypeSetName(PurpleAttentionType type, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_attention_type_set_incoming_desc(PurpleAttentionType * type, char * desc)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_attention_type_set_incoming_desc(IntPtr type, string desc);

		public static void AttentionTypeSetIncomingDesc(PurpleAttentionType type, string desc)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_attention_type_set_outgoing_desc(PurpleAttentionType * type, char * desc)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_attention_type_set_outgoing_desc(IntPtr type, string desc);

		public static void AttentionTypeSetOutgoingDesc(PurpleAttentionType type, string desc)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_attention_type_set_icon_name(PurpleAttentionType * type, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_attention_type_set_icon_name(IntPtr type, string name);

		public static void AttentionTypeSetIconName(PurpleAttentionType type, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_attention_type_set_unlocalized_name(PurpleAttentionType * type, char * ulname)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_attention_type_set_unlocalized_name(IntPtr type, string ulname);

		public static void AttentionTypeSetUnlocalizedName(PurpleAttentionType type, string ulname)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_attention_type_get_name(PurpleAttentionType * type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_attention_type_get_name(IntPtr type);

		public static string AttentionTypeGetName(PurpleAttentionType type)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_attention_type_get_incoming_desc(PurpleAttentionType * type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_attention_type_get_incoming_desc(IntPtr type);

		public static string AttentionTypeGetIncomingDesc(PurpleAttentionType type)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_attention_type_get_outgoing_desc(PurpleAttentionType * type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_attention_type_get_outgoing_desc(IntPtr type);

		public static string AttentionTypeGetOutgoingDesc(PurpleAttentionType type)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_attention_type_get_icon_name(PurpleAttentionType * type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_attention_type_get_icon_name(IntPtr type);

		public static string AttentionTypeGetIconName(PurpleAttentionType type)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_attention_type_get_unlocalized_name(PurpleAttentionType * type)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_attention_type_get_unlocalized_name(IntPtr type);

		public static string AttentionTypeGetUnlocalizedName(PurpleAttentionType type)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prpl_got_account_idle(PurpleAccount * account, gboolean idle, time_t idle_time)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_got_account_idle(IntPtr account, bool idle, UNKNOWN idle_time);

		public static void GotAccountIdle(PurpleAccount account, bool idle, time_t idle_time)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prpl_got_account_login_time(PurpleAccount * account, time_t login_time)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_got_account_login_time(IntPtr account, UNKNOWN login_time);

		public static void GotAccountLoginTime(PurpleAccount account, time_t login_time)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prpl_got_account_status(PurpleAccount * account, char * status_id, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_got_account_status(IntPtr account, string status_id, ...);

		public static void GotAccountStatus(PurpleAccount account, string status_id, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prpl_got_account_actions(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_got_account_actions(IntPtr account);

		public static void GotAccountActions(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prpl_got_user_idle(PurpleAccount * account, char * name, gboolean idle, time_t idle_time)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_got_user_idle(IntPtr account, string name, bool idle, UNKNOWN idle_time);

		public static void GotUserIdle(PurpleAccount account, string name, bool idle, time_t idle_time)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prpl_got_user_login_time(PurpleAccount * account, char * name, time_t login_time)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_got_user_login_time(IntPtr account, string name, UNKNOWN login_time);

		public static void GotUserLoginTime(PurpleAccount account, string name, time_t login_time)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prpl_got_user_status(PurpleAccount * account, char * name, char * status_id, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_got_user_status(IntPtr account, string name, string status_id, ...);

		public static void GotUserStatus(PurpleAccount account, string name, string status_id, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prpl_got_user_status_deactive(PurpleAccount * account, char * name, char * status_id)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_got_user_status_deactive(IntPtr account, string name, string status_id);

		public static void GotUserStatusDeactive(PurpleAccount account, string name, string status_id)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prpl_change_account_status(PurpleAccount * account, PurpleStatus * old_status, PurpleStatus * new_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_change_account_status(IntPtr account, IntPtr old_status, IntPtr new_status);

		public static void ChangeAccountStatus(PurpleAccount account, PurpleStatus old_status, PurpleStatus new_status)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_prpl_get_statuses(PurpleAccount * account, PurplePresence * presence)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_prpl_get_statuses(IntPtr account, IntPtr presence);

		public static GList GetStatuses(PurpleAccount account, PurplePresence presence)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prpl_send_attention(PurpleConnection * gc, char * who, guint type_code)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_send_attention(IntPtr gc, string who, uint type_code);

		public static void SendAttention(PurpleConnection gc, string who, uint type_code)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prpl_got_attention(PurpleConnection * gc, char * who, guint type_code)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_got_attention(IntPtr gc, string who, uint type_code);

		public static void GotAttention(PurpleConnection gc, string who, uint type_code)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_prpl_got_attention_in_chat(PurpleConnection * gc, int id, char * who, guint type_code)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_prpl_got_attention_in_chat(IntPtr gc, int id, string who, uint type_code);

		public static void GotAttentionInChat(PurpleConnection gc, int id, string who, uint type_code)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleMediaCaps purple_prpl_get_media_caps(PurpleAccount * account, char * who)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_prpl_get_media_caps(IntPtr account, string who);

		public static PurpleMediaCaps GetMediaCaps(PurpleAccount account, string who)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_prpl_initiate_media(PurpleAccount * account, char * who, PurpleMediaSessionType type)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_prpl_initiate_media(IntPtr account, string who, UNKNOWN type);

		public static bool InitiateMedia(PurpleAccount account, string who, PurpleMediaSessionType type)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePlugin * purple_find_prpl(char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_find_prpl(string id);

		public static PurplePlugin FindPrpl(string id)
		{
			throw new NotImplementedException();
		}

	}
}

