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
	public class Blist
	{
		/*
		 * PurpleBuddyList * purple_blist_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_new();

		public static PurpleBuddyList New()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_set_blist(PurpleBuddyList * blist)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_set_blist(IntPtr blist);

		public static void SetBlist(PurpleBuddyList blist)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBuddyList * purple_get_blist()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_get_blist();

		public static PurpleBuddyList GetBlist()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBlistNode * purple_blist_get_root()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_get_root();

		public static PurpleBlistNode GetRoot()
		{
			throw new NotImplementedException();
		}

		/*
		 * GSList * purple_blist_get_buddies()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_get_buddies();

		public static GSList GetBuddies()
		{
			throw new NotImplementedException();
		}

		/*
		 * gpointer purple_blist_get_ui_data()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_get_ui_data();

		public static IntPtr GetUiData()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_set_ui_data(gpointer ui_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_set_ui_data(IntPtr ui_data);

		public static void SetUiData(IntPtr ui_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBlistNode * purple_blist_node_next(PurpleBlistNode * node, gboolean offline)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_node_next(IntPtr node, bool offline);

		public static PurpleBlistNode NodeNext(PurpleBlistNode node, bool offline)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBlistNode * purple_blist_node_get_parent(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_node_get_parent(IntPtr node);

		public static PurpleBlistNode NodeGetParent(PurpleBlistNode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBlistNode * purple_blist_node_get_first_child(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_node_get_first_child(IntPtr node);

		public static PurpleBlistNode NodeGetFirstChild(PurpleBlistNode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBlistNode * purple_blist_node_get_sibling_next(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_node_get_sibling_next(IntPtr node);

		public static PurpleBlistNode NodeGetSiblingNext(PurpleBlistNode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBlistNode * purple_blist_node_get_sibling_prev(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_node_get_sibling_prev(IntPtr node);

		public static PurpleBlistNode NodeGetSiblingPrev(PurpleBlistNode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * gpointer purple_blist_node_get_ui_data(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_node_get_ui_data(IntPtr node);

		public static IntPtr NodeGetUiData(PurpleBlistNode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_node_set_ui_data(PurpleBlistNode * node, gpointer ui_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_node_set_ui_data(IntPtr node, IntPtr ui_data);

		public static void NodeSetUiData(PurpleBlistNode node, IntPtr ui_data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_show()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_show();

		public static void Show()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_destroy()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_destroy();

		public static void Destroy()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_set_visible(gboolean show)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_set_visible(bool show);

		public static void SetVisible(bool show)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_update_buddy_status(PurpleBuddy * buddy, PurpleStatus * old_status)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_update_buddy_status(IntPtr buddy, IntPtr old_status);

		public static void UpdateBuddyStatus(PurpleBuddy buddy, PurpleStatus old_status)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_update_node_icon(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_update_node_icon(IntPtr node);

		public static void UpdateNodeIcon(PurpleBlistNode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_update_buddy_icon(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_update_buddy_icon(IntPtr buddy);

		public static void UpdateBuddyIcon(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_rename_buddy(PurpleBuddy * buddy, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_rename_buddy(IntPtr buddy, string name);

		public static void RenameBuddy(PurpleBuddy buddy, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_alias_contact(PurpleContact * contact, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_alias_contact(IntPtr contact, string alias);

		public static void AliasContact(PurpleContact contact, string alias)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_alias_buddy(PurpleBuddy * buddy, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_alias_buddy(IntPtr buddy, string alias);

		public static void AliasBuddy(PurpleBuddy buddy, string alias)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_server_alias_buddy(PurpleBuddy * buddy, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_server_alias_buddy(IntPtr buddy, string alias);

		public static void ServerAliasBuddy(PurpleBuddy buddy, string alias)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_alias_chat(PurpleChat * chat, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_alias_chat(IntPtr chat, string alias);

		public static void AliasChat(PurpleChat chat, string alias)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_rename_group(PurpleGroup * group, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_rename_group(IntPtr group, string name);

		public static void RenameGroup(PurpleGroup group, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleChat * purple_chat_new(PurpleAccount * account, char * alias, GHashTable * components)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_chat_new(IntPtr account, string alias, IntPtr components);

		public static PurpleChat ChatNew(PurpleAccount account, string alias, GHashTable components)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_chat_destroy(PurpleChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_chat_destroy(IntPtr chat);

		public static void ChatDestroy(PurpleChat chat)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_add_chat(PurpleChat * chat, PurpleGroup * group, PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_add_chat(IntPtr chat, IntPtr group, IntPtr node);

		public static void AddChat(PurpleChat chat, PurpleGroup group, PurpleBlistNode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBuddy * purple_buddy_new(PurpleAccount * account, char * name, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_new(IntPtr account, string name, string alias);

		public static PurpleBuddy BuddyNew(PurpleAccount account, string name, string alias)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_buddy_destroy(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_destroy(IntPtr buddy);

		public static void BuddyDestroy(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_buddy_set_icon(PurpleBuddy * buddy, PurpleBuddyIcon * icon)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_set_icon(IntPtr buddy, IntPtr icon);

		public static void BuddySetIcon(PurpleBuddy buddy, PurpleBuddyIcon icon)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccount * purple_buddy_get_account(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_get_account(IntPtr buddy);

		public static PurpleAccount BuddyGetAccount(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_buddy_get_name(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_get_name(IntPtr buddy);

		public static string BuddyGetName(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBuddyIcon * purple_buddy_get_icon(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_get_icon(IntPtr buddy);

		public static PurpleBuddyIcon BuddyGetIcon(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * gpointer purple_buddy_get_protocol_data(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_get_protocol_data(IntPtr buddy);

		public static IntPtr BuddyGetProtocolData(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_buddy_set_protocol_data(PurpleBuddy * buddy, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_set_protocol_data(IntPtr buddy, IntPtr data);

		public static void BuddySetProtocolData(PurpleBuddy buddy, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleContact * purple_buddy_get_contact(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_get_contact(IntPtr buddy);

		public static PurpleContact BuddyGetContact(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePresence * purple_buddy_get_presence(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_get_presence(IntPtr buddy);

		public static PurplePresence BuddyGetPresence(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_add_buddy(PurpleBuddy * buddy, PurpleContact * contact, PurpleGroup * group, PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_add_buddy(IntPtr buddy, IntPtr contact, IntPtr group, IntPtr node);

		public static void AddBuddy(PurpleBuddy buddy, PurpleContact contact, PurpleGroup group, PurpleBlistNode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleGroup * purple_group_new(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_group_new(string name);

		public static PurpleGroup GroupNew(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_group_destroy(PurpleGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_group_destroy(IntPtr group);

		public static void GroupDestroy(PurpleGroup group)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_add_group(PurpleGroup * group, PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_add_group(IntPtr group, IntPtr node);

		public static void AddGroup(PurpleGroup group, PurpleBlistNode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleContact * purple_contact_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_contact_new();

		public static PurpleContact ContactNew()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_contact_destroy(PurpleContact * contact)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_contact_destroy(IntPtr contact);

		public static void ContactDestroy(PurpleContact contact)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_add_contact(PurpleContact * contact, PurpleGroup * group, PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_add_contact(IntPtr contact, IntPtr group, IntPtr node);

		public static void AddContact(PurpleContact contact, PurpleGroup group, PurpleBlistNode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_merge_contact(PurpleContact * source, PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_merge_contact(IntPtr source, IntPtr node);

		public static void MergeContact(PurpleContact source, PurpleBlistNode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBuddy * purple_contact_get_priority_buddy(PurpleContact * contact)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_contact_get_priority_buddy(IntPtr contact);

		public static PurpleBuddy ContactGetPriorityBuddy(PurpleContact contact)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_contact_set_alias(PurpleContact * contact, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_contact_set_alias(IntPtr contact, string alias);

		public static void ContactSetAlias(PurpleContact contact, string alias)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_contact_get_alias(PurpleContact * contact)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_contact_get_alias(IntPtr contact);

		public static string ContactGetAlias(PurpleContact contact)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_contact_on_account(PurpleContact * contact, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_contact_on_account(IntPtr contact, IntPtr account);

		public static bool ContactOnAccount(PurpleContact contact, PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_contact_invalidate_priority_buddy(PurpleContact * contact)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_contact_invalidate_priority_buddy(IntPtr contact);

		public static void ContactInvalidatePriorityBuddy(PurpleContact contact)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_remove_buddy(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_remove_buddy(IntPtr buddy);

		public static void RemoveBuddy(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_remove_contact(PurpleContact * contact)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_remove_contact(IntPtr contact);

		public static void RemoveContact(PurpleContact contact)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_remove_chat(PurpleChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_remove_chat(IntPtr chat);

		public static void RemoveChat(PurpleChat chat)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_remove_group(PurpleGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_remove_group(IntPtr group);

		public static void RemoveGroup(PurpleGroup group)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_buddy_get_alias_only(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_get_alias_only(IntPtr buddy);

		public static string BuddyGetAliasOnly(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_buddy_get_server_alias(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_get_server_alias(IntPtr buddy);

		public static string BuddyGetServerAlias(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_buddy_get_contact_alias(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_get_contact_alias(IntPtr buddy);

		public static string BuddyGetContactAlias(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_buddy_get_local_alias(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_get_local_alias(IntPtr buddy);

		public static string BuddyGetLocalAlias(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_buddy_get_alias(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_get_alias(IntPtr buddy);

		public static string BuddyGetAlias(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_buddy_get_local_buddy_alias(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_get_local_buddy_alias(IntPtr buddy);

		public static string BuddyGetLocalBuddyAlias(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_chat_get_name(PurpleChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_chat_get_name(IntPtr chat);

		public static string ChatGetName(PurpleChat chat)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBuddy * purple_find_buddy(PurpleAccount * account, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_find_buddy(IntPtr account, string name);

		public static PurpleBuddy FindBuddy(PurpleAccount account, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBuddy * purple_find_buddy_in_group(PurpleAccount * account, char * name, PurpleGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_find_buddy_in_group(IntPtr account, string name, IntPtr group);

		public static PurpleBuddy FindBuddyInGroup(PurpleAccount account, string name, PurpleGroup group)
		{
			throw new NotImplementedException();
		}

		/*
		 * GSList * purple_find_buddies(PurpleAccount * account, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_find_buddies(IntPtr account, string name);

		public static GSList FindBuddies(PurpleAccount account, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleGroup * purple_find_group(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_find_group(string name);

		public static PurpleGroup FindGroup(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleChat * purple_blist_find_chat(PurpleAccount * account, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_find_chat(IntPtr account, string name);

		public static PurpleChat FindChat(PurpleAccount account, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleGroup * purple_chat_get_group(PurpleChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_chat_get_group(IntPtr chat);

		public static PurpleGroup ChatGetGroup(PurpleChat chat)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccount * purple_chat_get_account(PurpleChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_chat_get_account(IntPtr chat);

		public static PurpleAccount ChatGetAccount(PurpleChat chat)
		{
			throw new NotImplementedException();
		}

		/*
		 * GHashTable * purple_chat_get_components(PurpleChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_chat_get_components(IntPtr chat);

		public static GHashTable ChatGetComponents(PurpleChat chat)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleGroup * purple_buddy_get_group(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_get_group(IntPtr buddy);

		public static PurpleGroup BuddyGetGroup(PurpleBuddy buddy)
		{
			throw new NotImplementedException();
		}

		/*
		 * GSList * purple_group_get_accounts(PurpleGroup * g)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_group_get_accounts(IntPtr g);

		public static GSList GroupGetAccounts(PurpleGroup g)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_group_on_account(PurpleGroup * g, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_group_on_account(IntPtr g, IntPtr account);

		public static bool GroupOnAccount(PurpleGroup g, PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_group_get_name(PurpleGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_group_get_name(IntPtr group);

		public static string GroupGetName(PurpleGroup group)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_add_account(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_add_account(IntPtr account);

		public static void AddAccount(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_remove_account(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_remove_account(IntPtr account);

		public static void RemoveAccount(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_blist_get_group_size(PurpleGroup * group, gboolean offline)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_blist_get_group_size(IntPtr group, bool offline);

		public static int GetGroupSize(PurpleGroup group, bool offline)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_blist_get_group_online_count(PurpleGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_blist_get_group_online_count(IntPtr group);

		public static int GetGroupOnlineCount(PurpleGroup group)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_load()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_load();

		public static void Load()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_schedule_save()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_schedule_save();

		public static void ScheduleSave()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_request_add_buddy(PurpleAccount * account, char * username, char * group, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_request_add_buddy(IntPtr account, string username, string group, string alias);

		public static void RequestAddBuddy(PurpleAccount account, string username, string group, string alias)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_request_add_chat(PurpleAccount * account, PurpleGroup * group, char * alias, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_request_add_chat(IntPtr account, IntPtr group, string alias, string name);

		public static void RequestAddChat(PurpleAccount account, PurpleGroup group, string alias, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_request_add_group()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_request_add_group();

		public static void RequestAddGroup()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_node_set_bool(PurpleBlistNode * node, char * key, gboolean value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_node_set_bool(IntPtr node, string key, bool value);

		public static void NodeSetBool(PurpleBlistNode node, string key, bool value)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_blist_node_get_bool(PurpleBlistNode * node, char * key)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_blist_node_get_bool(IntPtr node, string key);

		public static bool NodeGetBool(PurpleBlistNode node, string key)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_node_set_int(PurpleBlistNode * node, char * key, int value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_node_set_int(IntPtr node, string key, int value);

		public static void NodeSetInt(PurpleBlistNode node, string key, int value)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_blist_node_get_int(PurpleBlistNode * node, char * key)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_blist_node_get_int(IntPtr node, string key);

		public static int NodeGetInt(PurpleBlistNode node, string key)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_node_set_string(PurpleBlistNode * node, char * key, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_node_set_string(IntPtr node, string key, string value);

		public static void NodeSetString(PurpleBlistNode node, string key, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_blist_node_get_string(PurpleBlistNode * node, char * key)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_blist_node_get_string(IntPtr node, string key);

		public static string NodeGetString(PurpleBlistNode node, string key)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_node_remove_setting(PurpleBlistNode * node, char * key)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_node_remove_setting(IntPtr node, string key);

		public static void NodeRemoveSetting(PurpleBlistNode node, string key)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_node_set_flags(PurpleBlistNode * node, PurpleBlistNodeFlags flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_node_set_flags(IntPtr node, UNKNOWN flags);

		public static void NodeSetFlags(PurpleBlistNode node, PurpleBlistNodeFlags flags)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBlistNodeFlags purple_blist_node_get_flags(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_blist_node_get_flags(IntPtr node);

		public static PurpleBlistNodeFlags NodeGetFlags(PurpleBlistNode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBlistNodeType purple_blist_node_get_type(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_blist_node_get_type(IntPtr node);

		public static PurpleBlistNodeType NodeGetType(PurpleBlistNode node)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_blist_node_get_extended_menu(PurpleBlistNode * n)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_node_get_extended_menu(IntPtr n);

		public static GList NodeGetExtendedMenu(PurpleBlistNode n)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_set_ui_ops(PurpleBlistUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_set_ui_ops(IntPtr ops);

		public static void SetUiOps(PurpleBlistUiOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleBlistUiOps * purple_blist_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_get_ui_ops();

		public static PurpleBlistUiOps GetUiOps()
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_blist_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_get_handle();

		public static IntPtr GetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_blist_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_uninit();

		public static void Uninit()
		{
			throw new NotImplementedException();
		}

	}
}

