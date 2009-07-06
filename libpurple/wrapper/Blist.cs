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
	public class Blist
	{
		public enum PurpleBlistNodeType
		{
			PURPLE_BLIST_GROUP_NODE,
			PURPLE_BLIST_CONTACT_NODE,
			PURPLE_BLIST_BUDDY_NODE,
			PURPLE_BLIST_CHAT_NODE,
			PURPLE_BLIST_OTHER_NODE
		};

		public enum PurpleBlistNodeFlags
		{
			PURPLE_BLIST_NODE_FLAG_NO_SAVE = 1 << 0
		};

		/*
		 * PurpleBuddyList * purple_blist_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_new();

		public static PurpleBuddyList New()
		{
			return new PurpleBuddyList(purple_blist_new());
		}

		/*
		 * void purple_set_blist(PurpleBuddyList * blist)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_set_blist(IntPtr blist);

		public static void SetBlist(PurpleBuddyList blist)
		{
			purple_set_blist(blist.Reference);
		}

		/*
		 * PurpleBuddyList * purple_get_blist()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_get_blist();

		public static PurpleBuddyList GetBlist()
		{
			return new PurpleBuddyList(purple_get_blist());
		}

		/*
		 * PurpleBlistNode * purple_blist_get_root()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_get_root();

		public static PurpleBlistNode GetRoot()
		{
			return new PurpleBlistNode(purple_blist_get_root());
		}

		/*
		 * GSList * purple_blist_get_buddies()
		 * 
		 * Could not generate a wrapper for purple_blist_get_buddies in file "blist.h".
		 * Message: The type could not be resolved (GSList * purple_blist_get_buddies()).
		 */

		/*
		 * gpointer purple_blist_get_ui_data()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_get_ui_data();

		public static IntPtr GetUiData()
		{
			return purple_blist_get_ui_data();
		}

		/*
		 * void purple_blist_set_ui_data(gpointer ui_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_set_ui_data(IntPtr ui_data);

		public static void SetUiData(IntPtr ui_data)
		{
			purple_blist_set_ui_data(ui_data);
		}

		/*
		 * PurpleBlistNode * purple_blist_node_next(PurpleBlistNode * node, gboolean offline)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_node_next(IntPtr node, bool offline);

		public static PurpleBlistNode NodeNext(PurpleBlistNode node, bool offline)
		{
			return new PurpleBlistNode(purple_blist_node_next(node.Reference, offline));
		}

		/*
		 * PurpleBlistNode * purple_blist_node_get_parent(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_node_get_parent(IntPtr node);

		public static PurpleBlistNode NodeGetParent(PurpleBlistNode node)
		{
			return new PurpleBlistNode(purple_blist_node_get_parent(node.Reference));
		}

		/*
		 * PurpleBlistNode * purple_blist_node_get_first_child(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_node_get_first_child(IntPtr node);

		public static PurpleBlistNode NodeGetFirstChild(PurpleBlistNode node)
		{
			return new PurpleBlistNode(purple_blist_node_get_first_child(node.Reference));
		}

		/*
		 * PurpleBlistNode * purple_blist_node_get_sibling_next(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_node_get_sibling_next(IntPtr node);

		public static PurpleBlistNode NodeGetSiblingNext(PurpleBlistNode node)
		{
			return new PurpleBlistNode(purple_blist_node_get_sibling_next(node.Reference));
		}

		/*
		 * PurpleBlistNode * purple_blist_node_get_sibling_prev(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_node_get_sibling_prev(IntPtr node);

		public static PurpleBlistNode NodeGetSiblingPrev(PurpleBlistNode node)
		{
			return new PurpleBlistNode(purple_blist_node_get_sibling_prev(node.Reference));
		}

		/*
		 * gpointer purple_blist_node_get_ui_data(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_node_get_ui_data(IntPtr node);

		public static IntPtr NodeGetUiData(PurpleBlistNode node)
		{
			return purple_blist_node_get_ui_data(node.Reference);
		}

		/*
		 * void purple_blist_node_set_ui_data(PurpleBlistNode * node, gpointer ui_data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_node_set_ui_data(IntPtr node, IntPtr ui_data);

		public static void NodeSetUiData(PurpleBlistNode node, IntPtr ui_data)
		{
			purple_blist_node_set_ui_data(node.Reference, ui_data);
		}

		/*
		 * void purple_blist_show()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_show();

		public static void Show()
		{
			purple_blist_show();
		}

		/*
		 * void purple_blist_destroy()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_destroy();

		public static void Destroy()
		{
			purple_blist_destroy();
		}

		/*
		 * void purple_blist_set_visible(gboolean show)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_set_visible(bool show);

		public static void SetVisible(bool show)
		{
			purple_blist_set_visible(show);
		}

		/*
		 * void purple_blist_update_buddy_status(PurpleBuddy * buddy, PurpleStatus * old_status)
		 * 
		 * Could not generate a wrapper for purple_blist_update_buddy_status in file "blist.h".
		 * Message: The type could not be resolved (PurpleStatus * old_status).
		 */

		/*
		 * void purple_blist_update_node_icon(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_update_node_icon(IntPtr node);

		public static void UpdateNodeIcon(PurpleBlistNode node)
		{
			purple_blist_update_node_icon(node.Reference);
		}

		/*
		 * void purple_blist_update_buddy_icon(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_update_buddy_icon(IntPtr buddy);

		public static void UpdateBuddyIcon(PurpleBuddy buddy)
		{
			purple_blist_update_buddy_icon(buddy.Reference);
		}

		/*
		 * void purple_blist_rename_buddy(PurpleBuddy * buddy, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_rename_buddy(IntPtr buddy, string name);

		public static void RenameBuddy(PurpleBuddy buddy, string name)
		{
			purple_blist_rename_buddy(buddy.Reference, name);
		}

		/*
		 * void purple_blist_alias_contact(PurpleContact * contact, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_alias_contact(IntPtr contact, string alias);

		public static void AliasContact(PurpleContact contact, string alias)
		{
			purple_blist_alias_contact(contact.Reference, alias);
		}

		/*
		 * void purple_blist_alias_buddy(PurpleBuddy * buddy, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_alias_buddy(IntPtr buddy, string alias);

		public static void AliasBuddy(PurpleBuddy buddy, string alias)
		{
			purple_blist_alias_buddy(buddy.Reference, alias);
		}

		/*
		 * void purple_blist_server_alias_buddy(PurpleBuddy * buddy, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_server_alias_buddy(IntPtr buddy, string alias);

		public static void ServerAliasBuddy(PurpleBuddy buddy, string alias)
		{
			purple_blist_server_alias_buddy(buddy.Reference, alias);
		}

		/*
		 * void purple_blist_alias_chat(PurpleChat * chat, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_alias_chat(IntPtr chat, string alias);

		public static void AliasChat(PurpleChat chat, string alias)
		{
			purple_blist_alias_chat(chat.Reference, alias);
		}

		/*
		 * void purple_blist_rename_group(PurpleGroup * group, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_rename_group(IntPtr group, string name);

		public static void RenameGroup(PurpleGroup group, string name)
		{
			purple_blist_rename_group(group.Reference, name);
		}

		/*
		 * PurpleChat * purple_chat_new(PurpleAccount * account, char * alias, GHashTable * components)
		 * 
		 * Could not generate a wrapper for purple_chat_new in file "blist.h".
		 * Message: The type could not be resolved (GHashTable * components).
		 */

		/*
		 * void purple_chat_destroy(PurpleChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_chat_destroy(IntPtr chat);

		public static void ChatDestroy(PurpleChat chat)
		{
			purple_chat_destroy(chat.Reference);
		}

		/*
		 * void purple_blist_add_chat(PurpleChat * chat, PurpleGroup * group, PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_add_chat(IntPtr chat, IntPtr group, IntPtr node);

		public static void AddChat(PurpleChat chat, PurpleGroup group, PurpleBlistNode node)
		{
			purple_blist_add_chat(chat.Reference, group.Reference, node.Reference);
		}

		/*
		 * PurpleBuddy * purple_buddy_new(PurpleAccount * account, char * name, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_new(IntPtr account, string name, string alias);

		public static PurpleBuddy BuddyNew(PurpleAccount account, string name, string alias)
		{
			return new PurpleBuddy(purple_buddy_new(account.Reference, name, alias));
		}

		/*
		 * void purple_buddy_destroy(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_destroy(IntPtr buddy);

		public static void BuddyDestroy(PurpleBuddy buddy)
		{
			purple_buddy_destroy(buddy.Reference);
		}

		/*
		 * void purple_buddy_set_icon(PurpleBuddy * buddy, PurpleBuddyIcon * icon)
		 * 
		 * Could not generate a wrapper for purple_buddy_set_icon in file "blist.h".
		 * Message: The type could not be resolved (PurpleBuddyIcon * icon).
		 */

		/*
		 * PurpleAccount * purple_buddy_get_account(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_get_account(IntPtr buddy);

		public static PurpleAccount BuddyGetAccount(PurpleBuddy buddy)
		{
			return new PurpleAccount(purple_buddy_get_account(buddy.Reference));
		}

		/*
		 * char * purple_buddy_get_name(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_get_name(IntPtr buddy);

		public static string BuddyGetName(PurpleBuddy buddy)
		{
			return purple_buddy_get_name(buddy.Reference);
		}

		/*
		 * PurpleBuddyIcon * purple_buddy_get_icon(PurpleBuddy * buddy)
		 * 
		 * Could not generate a wrapper for purple_buddy_get_icon in file "blist.h".
		 * Message: The type could not be resolved (PurpleBuddyIcon * purple_buddy_get_icon(PurpleBuddy * buddy)).
		 */

		/*
		 * gpointer purple_buddy_get_protocol_data(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_get_protocol_data(IntPtr buddy);

		public static IntPtr BuddyGetProtocolData(PurpleBuddy buddy)
		{
			return purple_buddy_get_protocol_data(buddy.Reference);
		}

		/*
		 * void purple_buddy_set_protocol_data(PurpleBuddy * buddy, gpointer data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_buddy_set_protocol_data(IntPtr buddy, IntPtr data);

		public static void BuddySetProtocolData(PurpleBuddy buddy, IntPtr data)
		{
			purple_buddy_set_protocol_data(buddy.Reference, data);
		}

		/*
		 * PurpleContact * purple_buddy_get_contact(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_get_contact(IntPtr buddy);

		public static PurpleContact BuddyGetContact(PurpleBuddy buddy)
		{
			return new PurpleContact(purple_buddy_get_contact(buddy.Reference));
		}

		/*
		 * PurplePresence * purple_buddy_get_presence(PurpleBuddy * buddy)
		 * 
		 * Could not generate a wrapper for purple_buddy_get_presence in file "blist.h".
		 * Message: The type could not be resolved (PurplePresence * purple_buddy_get_presence(PurpleBuddy * buddy)).
		 */

		/*
		 * void purple_blist_add_buddy(PurpleBuddy * buddy, PurpleContact * contact, PurpleGroup * group, PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_add_buddy(IntPtr buddy, IntPtr contact, IntPtr group, IntPtr node);

		public static void AddBuddy(PurpleBuddy buddy, PurpleContact contact, PurpleGroup group, PurpleBlistNode node)
		{
			purple_blist_add_buddy(buddy.Reference, contact.Reference, group.Reference, node.Reference);
		}

		/*
		 * PurpleGroup * purple_group_new(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_group_new(string name);

		public static PurpleGroup GroupNew(string name)
		{
			return new PurpleGroup(purple_group_new(name));
		}

		/*
		 * void purple_group_destroy(PurpleGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_group_destroy(IntPtr group);

		public static void GroupDestroy(PurpleGroup group)
		{
			purple_group_destroy(group.Reference);
		}

		/*
		 * void purple_blist_add_group(PurpleGroup * group, PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_add_group(IntPtr group, IntPtr node);

		public static void AddGroup(PurpleGroup group, PurpleBlistNode node)
		{
			purple_blist_add_group(group.Reference, node.Reference);
		}

		/*
		 * PurpleContact * purple_contact_new()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_contact_new();

		public static PurpleContact ContactNew()
		{
			return new PurpleContact(purple_contact_new());
		}

		/*
		 * void purple_contact_destroy(PurpleContact * contact)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_contact_destroy(IntPtr contact);

		public static void ContactDestroy(PurpleContact contact)
		{
			purple_contact_destroy(contact.Reference);
		}

		/*
		 * void purple_blist_add_contact(PurpleContact * contact, PurpleGroup * group, PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_add_contact(IntPtr contact, IntPtr group, IntPtr node);

		public static void AddContact(PurpleContact contact, PurpleGroup group, PurpleBlistNode node)
		{
			purple_blist_add_contact(contact.Reference, group.Reference, node.Reference);
		}

		/*
		 * void purple_blist_merge_contact(PurpleContact * source, PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_merge_contact(IntPtr source, IntPtr node);

		public static void MergeContact(PurpleContact source, PurpleBlistNode node)
		{
			purple_blist_merge_contact(source.Reference, node.Reference);
		}

		/*
		 * PurpleBuddy * purple_contact_get_priority_buddy(PurpleContact * contact)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_contact_get_priority_buddy(IntPtr contact);

		public static PurpleBuddy ContactGetPriorityBuddy(PurpleContact contact)
		{
			return new PurpleBuddy(purple_contact_get_priority_buddy(contact.Reference));
		}

		/*
		 * void purple_contact_set_alias(PurpleContact * contact, char * alias)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_contact_set_alias(IntPtr contact, string alias);

		public static void ContactSetAlias(PurpleContact contact, string alias)
		{
			purple_contact_set_alias(contact.Reference, alias);
		}

		/*
		 * char * purple_contact_get_alias(PurpleContact * contact)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_contact_get_alias(IntPtr contact);

		public static string ContactGetAlias(PurpleContact contact)
		{
			return purple_contact_get_alias(contact.Reference);
		}

		/*
		 * gboolean purple_contact_on_account(PurpleContact * contact, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_contact_on_account(IntPtr contact, IntPtr account);

		public static bool ContactOnAccount(PurpleContact contact, PurpleAccount account)
		{
			return purple_contact_on_account(contact.Reference, account.Reference);
		}

		/*
		 * void purple_contact_invalidate_priority_buddy(PurpleContact * contact)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_contact_invalidate_priority_buddy(IntPtr contact);

		public static void ContactInvalidatePriorityBuddy(PurpleContact contact)
		{
			purple_contact_invalidate_priority_buddy(contact.Reference);
		}

		/*
		 * void purple_blist_remove_buddy(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_remove_buddy(IntPtr buddy);

		public static void RemoveBuddy(PurpleBuddy buddy)
		{
			purple_blist_remove_buddy(buddy.Reference);
		}

		/*
		 * void purple_blist_remove_contact(PurpleContact * contact)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_remove_contact(IntPtr contact);

		public static void RemoveContact(PurpleContact contact)
		{
			purple_blist_remove_contact(contact.Reference);
		}

		/*
		 * void purple_blist_remove_chat(PurpleChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_remove_chat(IntPtr chat);

		public static void RemoveChat(PurpleChat chat)
		{
			purple_blist_remove_chat(chat.Reference);
		}

		/*
		 * void purple_blist_remove_group(PurpleGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_remove_group(IntPtr group);

		public static void RemoveGroup(PurpleGroup group)
		{
			purple_blist_remove_group(group.Reference);
		}

		/*
		 * char * purple_buddy_get_alias_only(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_get_alias_only(IntPtr buddy);

		public static string BuddyGetAliasOnly(PurpleBuddy buddy)
		{
			return purple_buddy_get_alias_only(buddy.Reference);
		}

		/*
		 * char * purple_buddy_get_server_alias(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_get_server_alias(IntPtr buddy);

		public static string BuddyGetServerAlias(PurpleBuddy buddy)
		{
			return purple_buddy_get_server_alias(buddy.Reference);
		}

		/*
		 * char * purple_buddy_get_contact_alias(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_get_contact_alias(IntPtr buddy);

		public static string BuddyGetContactAlias(PurpleBuddy buddy)
		{
			return purple_buddy_get_contact_alias(buddy.Reference);
		}

		/*
		 * char * purple_buddy_get_local_alias(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_get_local_alias(IntPtr buddy);

		public static string BuddyGetLocalAlias(PurpleBuddy buddy)
		{
			return purple_buddy_get_local_alias(buddy.Reference);
		}

		/*
		 * char * purple_buddy_get_alias(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_get_alias(IntPtr buddy);

		public static string BuddyGetAlias(PurpleBuddy buddy)
		{
			return purple_buddy_get_alias(buddy.Reference);
		}

		/*
		 * char * purple_buddy_get_local_buddy_alias(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_buddy_get_local_buddy_alias(IntPtr buddy);

		public static string BuddyGetLocalBuddyAlias(PurpleBuddy buddy)
		{
			return purple_buddy_get_local_buddy_alias(buddy.Reference);
		}

		/*
		 * char * purple_chat_get_name(PurpleChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_chat_get_name(IntPtr chat);

		public static string ChatGetName(PurpleChat chat)
		{
			return purple_chat_get_name(chat.Reference);
		}

		/*
		 * PurpleBuddy * purple_find_buddy(PurpleAccount * account, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_find_buddy(IntPtr account, string name);

		public static PurpleBuddy FindBuddy(PurpleAccount account, string name)
		{
			return new PurpleBuddy(purple_find_buddy(account.Reference, name));
		}

		/*
		 * GSList * purple_find_buddies(PurpleAccount * account, char * name)
		 * 
		 * Could not generate a wrapper for purple_find_buddies in file "blist.h".
		 * Message: The type could not be resolved (GSList * purple_find_buddies(PurpleAccount * account, char * name)).
		 */

		/*
		 * PurpleGroup * purple_find_group(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_find_group(string name);

		public static PurpleGroup FindGroup(string name)
		{
			return new PurpleGroup(purple_find_group(name));
		}

		/*
		 * PurpleChat * purple_blist_find_chat(PurpleAccount * account, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_find_chat(IntPtr account, string name);

		public static PurpleChat FindChat(PurpleAccount account, string name)
		{
			return new PurpleChat(purple_blist_find_chat(account.Reference, name));
		}

		/*
		 * PurpleGroup * purple_chat_get_group(PurpleChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_chat_get_group(IntPtr chat);

		public static PurpleGroup ChatGetGroup(PurpleChat chat)
		{
			return new PurpleGroup(purple_chat_get_group(chat.Reference));
		}

		/*
		 * PurpleAccount * purple_chat_get_account(PurpleChat * chat)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_chat_get_account(IntPtr chat);

		public static PurpleAccount ChatGetAccount(PurpleChat chat)
		{
			return new PurpleAccount(purple_chat_get_account(chat.Reference));
		}

		/*
		 * GHashTable * purple_chat_get_components(PurpleChat * chat)
		 * 
		 * Could not generate a wrapper for purple_chat_get_components in file "blist.h".
		 * Message: The type could not be resolved (GHashTable * purple_chat_get_components(PurpleChat * chat)).
		 */

		/*
		 * PurpleGroup * purple_buddy_get_group(PurpleBuddy * buddy)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_buddy_get_group(IntPtr buddy);

		public static PurpleGroup BuddyGetGroup(PurpleBuddy buddy)
		{
			return new PurpleGroup(purple_buddy_get_group(buddy.Reference));
		}

		/*
		 * GSList * purple_group_get_accounts(PurpleGroup * g)
		 * 
		 * Could not generate a wrapper for purple_group_get_accounts in file "blist.h".
		 * Message: The type could not be resolved (GSList * purple_group_get_accounts(PurpleGroup * g)).
		 */

		/*
		 * gboolean purple_group_on_account(PurpleGroup * g, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_group_on_account(IntPtr g, IntPtr account);

		public static bool GroupOnAccount(PurpleGroup g, PurpleAccount account)
		{
			return purple_group_on_account(g.Reference, account.Reference);
		}

		/*
		 * char * purple_group_get_name(PurpleGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_group_get_name(IntPtr group);

		public static string GroupGetName(PurpleGroup group)
		{
			return purple_group_get_name(group.Reference);
		}

		/*
		 * void purple_blist_add_account(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_add_account(IntPtr account);

		public static void AddAccount(PurpleAccount account)
		{
			purple_blist_add_account(account.Reference);
		}

		/*
		 * void purple_blist_remove_account(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_remove_account(IntPtr account);

		public static void RemoveAccount(PurpleAccount account)
		{
			purple_blist_remove_account(account.Reference);
		}

		/*
		 * int purple_blist_get_group_size(PurpleGroup * group, gboolean offline)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_blist_get_group_size(IntPtr group, bool offline);

		public static int GetGroupSize(PurpleGroup group, bool offline)
		{
			return purple_blist_get_group_size(group.Reference, offline);
		}

		/*
		 * int purple_blist_get_group_online_count(PurpleGroup * group)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_blist_get_group_online_count(IntPtr group);

		public static int GetGroupOnlineCount(PurpleGroup group)
		{
			return purple_blist_get_group_online_count(group.Reference);
		}

		/*
		 * void purple_blist_load()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_load();

		public static void Load()
		{
			purple_blist_load();
		}

		/*
		 * void purple_blist_schedule_save()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_schedule_save();

		public static void ScheduleSave()
		{
			purple_blist_schedule_save();
		}

		/*
		 * void purple_blist_request_add_group()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_request_add_group();

		public static void RequestAddGroup()
		{
			purple_blist_request_add_group();
		}

		/*
		 * void purple_blist_node_set_bool(PurpleBlistNode * node, char * key, gboolean value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_node_set_bool(IntPtr node, string key, bool value);

		public static void NodeSetBool(PurpleBlistNode node, string key, bool value)
		{
			purple_blist_node_set_bool(node.Reference, key, value);
		}

		/*
		 * gboolean purple_blist_node_get_bool(PurpleBlistNode * node, char * key)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_blist_node_get_bool(IntPtr node, string key);

		public static bool NodeGetBool(PurpleBlistNode node, string key)
		{
			return purple_blist_node_get_bool(node.Reference, key);
		}

		/*
		 * void purple_blist_node_set_int(PurpleBlistNode * node, char * key, int value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_node_set_int(IntPtr node, string key, int value);

		public static void NodeSetInt(PurpleBlistNode node, string key, int value)
		{
			purple_blist_node_set_int(node.Reference, key, value);
		}

		/*
		 * int purple_blist_node_get_int(PurpleBlistNode * node, char * key)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_blist_node_get_int(IntPtr node, string key);

		public static int NodeGetInt(PurpleBlistNode node, string key)
		{
			return purple_blist_node_get_int(node.Reference, key);
		}

		/*
		 * char * purple_blist_node_get_string(PurpleBlistNode * node, char * key)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_blist_node_get_string(IntPtr node, string key);

		public static string NodeGetString(PurpleBlistNode node, string key)
		{
			return purple_blist_node_get_string(node.Reference, key);
		}

		/*
		 * void purple_blist_node_remove_setting(PurpleBlistNode * node, char * key)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_node_remove_setting(IntPtr node, string key);

		public static void NodeRemoveSetting(PurpleBlistNode node, string key)
		{
			purple_blist_node_remove_setting(node.Reference, key);
		}

		/*
		 * void purple_blist_node_set_flags(PurpleBlistNode * node, PurpleBlistNodeFlags flags)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_node_set_flags(IntPtr node, Blist.PurpleBlistNodeFlags flags);

		public static void NodeSetFlags(PurpleBlistNode node, Blist.PurpleBlistNodeFlags flags)
		{
			/* Unable to process flags, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * PurpleBlistNodeFlags purple_blist_node_get_flags(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern Blist.PurpleBlistNodeFlags purple_blist_node_get_flags(IntPtr node);

		public static Blist.PurpleBlistNodeFlags NodeGetFlags(PurpleBlistNode node)
		{
			/* Unable to process purple_blist_node_get_flags, a KnownEnum. */
			
		}

		/*
		 * PurpleBlistNodeType purple_blist_node_get_type(PurpleBlistNode * node)
		 */
		[DllImport("libpurple.dll")]
		private static extern Blist.PurpleBlistNodeType purple_blist_node_get_type(IntPtr node);

		public static Blist.PurpleBlistNodeType NodeGetType(PurpleBlistNode node)
		{
			/* Unable to process purple_blist_node_get_type, a KnownEnum. */
			
		}

		/*
		 * GList * purple_blist_node_get_extended_menu(PurpleBlistNode * n)
		 * 
		 * Could not generate a wrapper for purple_blist_node_get_extended_menu in file "blist.h".
		 * Message: The type could not be resolved (GList * purple_blist_node_get_extended_menu(PurpleBlistNode * n)).
		 */

		/*
		 * void purple_blist_set_ui_ops(PurpleBlistUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_set_ui_ops(IntPtr ops);

		public static void SetUiOps(PurpleBlistUiOps ops)
		{
			purple_blist_set_ui_ops(ops.Reference);
		}

		/*
		 * PurpleBlistUiOps * purple_blist_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_get_ui_ops();

		public static PurpleBlistUiOps GetUiOps()
		{
			return new PurpleBlistUiOps(purple_blist_get_ui_ops());
		}

		/*
		 * void * purple_blist_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_blist_get_handle();

		public static IntPtr GetHandle()
		{
			return purple_blist_get_handle();
		}

		/*
		 * void purple_blist_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_init();

		public static void Init()
		{
			purple_blist_init();
		}

		/*
		 * void purple_blist_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_blist_uninit();

		public static void Uninit()
		{
			purple_blist_uninit();
		}

	}
}

