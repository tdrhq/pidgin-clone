/**
* The QQ2003C protocol plugin
 *
 * for gaim
 *
 * Copyright (C) 2004 Puzzlebird
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

// START OF FILE
/*****************************************************************************/
#include <glib.h>		// GList
#include "conversation.h"	// GaimConversation

#include "utils.h"		// uid_to_gaim_name
#include "buddy_status.h"	// is_online
#include "group_conv.h"
#include "qq.h"			// qq_buddy

/*****************************************************************************/
// show group conversation window
void qq_group_conv_show_window(GaimConnection * gc, qq_group * group)
{
	GaimConversation *conv;
	qq_data *qd;

	g_return_if_fail(gc != NULL && gc->proto_data != NULL && group != NULL);
	qd = (qq_data *) gc->proto_data;

	conv = gaim_find_conversation_with_account(GAIM_CONV_TYPE_CHAT, /* add by gfhuang */group->group_name_utf8, gaim_connection_get_account(gc));
	if (conv == NULL)	// show only one window per group
		serv_got_joined_chat(gc, qd->channel++, group->group_name_utf8);
}				// qq_group_conv_show_window

/*****************************************************************************/
// refresh online member in group conversation window
void qq_group_conv_refresh_online_member(GaimConnection * gc, qq_group * group)
{
	GList *names, *list, *flags;
	qq_buddy *member;
	gchar *member_name;
	GaimConversation *conv;
	gint flag;
	g_return_if_fail(gc != NULL && group != NULL);

	names = NULL;
	flags = NULL;
	conv = gaim_find_conversation_with_account(GAIM_CONV_TYPE_CHAT /*gfhuang*/, group->group_name_utf8, gaim_connection_get_account(gc));
	if (conv != NULL && group->members != NULL) {
		list = group->members;
		while (list != NULL) {
			member = (qq_buddy *) list->data;
			//always put it even offline, by gfhuang
			names = g_list_append(names,
					      (member->nickname !=
					      NULL) ?
					      g_strdup(member->nickname) : uid_to_gaim_name(member->uid));
		
			flag = 0;
			if (is_online(member->status)) flag |= (GAIM_CBFLAGS_TYPING | GAIM_CBFLAGS_VOICE);		// TYPING to put online above OP and FOUNDER
			if(1 == (member->role & 1)) flag |= GAIM_CBFLAGS_OP;
			//if(4 == (member->role & 4)) flag |= GAIM_CBFLAGS_VOICE;    //active, no use
			if(member->uid == group->creator_uid) flag |= GAIM_CBFLAGS_FOUNDER;
			flags = g_list_append(flags, GINT_TO_POINTER(flag));
			list = list->next;
		}		// while list
		
		gaim_conv_chat_clear_users(GAIM_CONV_CHAT(conv));
		gaim_conv_chat_add_users(GAIM_CONV_CHAT(conv), names, NULL /*gfhuang*/, flags, FALSE /*gfhuang*/);
	}			// if conv
	// clean up names
	while (names != NULL) {
		member_name = (gchar *) names->data;
		names = g_list_remove(names, member_name);
		g_free(member_name);
	}			// while name
	// clean up flags, NOOOOOOOOOOOOOOOOOOOOOOO!!!!!! bugs, flags is not name! got by gfhuang
	/*
	while (flags != NULL) {
		member_name = (gchar *) flags->data;
		flags = g_list_remove(flags, member_name);
		g_free(member_name);
	}
	*/
	g_list_free(flags);	
}				// qq_group_conv_show_window

/*****************************************************************************/
// END OF FILE
