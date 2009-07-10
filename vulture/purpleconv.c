/*
 * Vulture - Win32 libpurple client
 *
 * purpleconv.c: Conversation back-end.
 *
 * Copyright (C) 2009, Gregor Dick <gdick@soc.pidgin.im>
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

#include <windows.h>
#include <glib.h>
#include <time.h>

#include "vulture.h"
#include "purple.h"
#include "purpleconv.h"
#include "vultureconv.h"
#include "purplemain.h"
#include "purpleblist.h"



/**
 * PurpleConversationUiOps::create_conversation callback. Called when a new
 * conversation is created.
 *
 * @param	lpconv	New conversation.
 */
void PurpleNewConversation(PurpleConversation *lpconv)
{
	/* The UI will eventually free this. */
	VULTURE_CONVERSATION *lpvconv;
	
	if(lpconv->type == PURPLE_CONV_TYPE_IM)
		lpvconv = (VULTURE_CONVERSATION*)g_new(VULTURE_CONVERSATION_IM, 1);
	else
		lpvconv = (VULTURE_CONVERSATION*)g_new(VULTURE_CONVERSATION_CHAT, 1);

	lpconv->ui_data = lpvconv;

	lpvconv->lpconv = lpconv;
	lpvconv->hwndContainer = lpvconv->hwndConv = NULL;

	/* Cache this, since the UI should not mess with the PurpleConversation
	 * directly.
	 */
	lpvconv->convtype = lpconv->type;

	VulturePostUIMessage(VUIMSG_NEWCONVERSATION, lpvconv);
}


/**
 * Called by the UI to free the data allocated by PurpleNewConversation when
 * the UI is done with it.
 *
 * @param	lpvconv		Data to free.
 */
void VultureFreeConversation(VULTURE_CONVERSATION *lpvconv)
{
	g_free(lpvconv);
}


/**
 * PurpleConversationUiOps::destroy_conversation callback. Called when a
 * conversation is being destroyed by the core. This should inform the
 * front-end to dismantle its UI. The sequence for an interactive request to
 * close a conversation is therefore: UI requests core to destroy convo, core
 * tells UI to destroy itself.
 *
 * @param	lpconv	Dying conversation.
 */
void PurpleDestroyConversation(PurpleConversation *lpconv)
{
	/* This pointer is about to become invalid. */
	((VULTURE_CONVERSATION*)lpconv->ui_data)->lpconv = NULL;

	VulturePostUIMessage(VUIMSG_DESTROYEDCONVERSATION, (VULTURE_CONVERSATION*)lpconv->ui_data);
}



/**
 * PurpleConversationUiOps::write_conv callback. Called when a message is to be
 * displayed in a conversation.
 *
 * @param	lpconv		Conversation.
 * @param	szName		Username.
 * @param	szAlias		Alias.
 * @param	szMessage	Message.
 * @param	pmflags		Message flags.
 * @param	timeMsg		Time of message.
 */
void PurpleWriteConversation(PurpleConversation *lpconv, const char *szName, const char *szAlias, const char *szMessage, PurpleMessageFlags pmflags, time_t timeMsg)
{
	VULTURE_CONV_WRITE *lpvcwrite = g_new(VULTURE_CONV_WRITE, 1);

	lpvcwrite->lpvconv = lpconv->ui_data;
	lpvcwrite->szName = VultureUTF8ToTCHAR(szAlias ? szAlias : szName);
	lpvcwrite->szMessage = VultureUTF8ToTCHAR(szMessage);
	VultureTimetToSystemTime(timeMsg, &lpvcwrite->systimeMsg);

	VulturePostUIMessage(VUIMSG_WRITECONVERSATION, lpvcwrite);
} 


/**
 * Called by the UI to free the data allocated by PurpleWriteConversation when
 * the UI is done with it.
 *
 * @param	lpvcwrite	Data to free.
 */
void VultureFreeConvWrite(VULTURE_CONV_WRITE *lpvcwrite)
{
	g_free(lpvcwrite->szMessage);
	g_free(lpvcwrite->szName);
	g_free(lpvcwrite);
}


/**
 * Sends a message.
 *
 * @param	lpvcsend	Details of message to send.
 */
void PurpleConversationSend(VULTURE_CONV_SEND *lpvcsend)
{
	char *szMessage = VultureTCHARToUTF8(lpvcsend->szMessage);

	/* Send to IM or chat, as appropriate. */
	if(lpvcsend->lpvconv->lpconv->type == PURPLE_CONV_TYPE_IM)
		purple_conv_im_send(lpvcsend->lpvconv->lpconv->u.im, szMessage);
	else
		purple_conv_chat_send(lpvcsend->lpvconv->lpconv->u.chat, szMessage);

	g_free(szMessage);
}


/**
 * Called in response to the conversation-updated signal.
 *
 * @param	lpconv	Conversation.
 * @param	pcut	Type of change.
 */
void PurpleConvChanged(PurpleConversation *lpconv, PurpleConvUpdateType pcut)
{
	VULTURE_CONVERSATION *lpvconv = lpconv->ui_data;
	VULTURE_CONV_CHANGED *lpvcchanged;

	if(!lpvconv)
		return;

	/* Forward the message to the UI. */

	lpvcchanged = g_new(VULTURE_CONV_CHANGED, 1);

	lpvcchanged->lpvconv = lpvconv;
	lpvcchanged->pcut = pcut;

	VulturePostUIMessage(VUIMSG_CONVCHANGED, lpvcchanged);
}


/**
 * Gets the title of a conversation.
 *
 * @param	lpconv	Conversation.
 */
LPTSTR PurpleConvGetTitle(PurpleConversation *lpconv)
{
	const char *szTitle;

	if(!lpconv)
		return NULL;

	szTitle = purple_conversation_get_title(lpconv);
	return szTitle ? VultureUTF8ToTCHAR(szTitle) : NULL;
}


/**
 * Gets the status message of our interlocutor in an IM window.
 *
 * @param	lpconv	Conversation.
 */
LPTSTR PurpleIMGetStatusText(PurpleConversation *lpconv)
{
	if(!lpconv || lpconv->type != PURPLE_CONV_TYPE_IM)
		return NULL;

	return PurpleBuddyGetStatusText(purple_find_buddy(lpconv->account, lpconv->name));
}


/**
 * Gets the parameters required to specify a chat for a given account.
 *
 * @param	lppac	Account.
 */
GList* PurpleGetChatFields(PurpleAccount *lppac)
{
	PurpleConnection *lppconn;
	GList* (*lpfnChatInfo)(PurpleConnection*);

	if(!lppac)
		return NULL;

	lppconn = purple_account_get_connection(lppac);
	lpfnChatInfo = PURPLE_PLUGIN_PROTOCOL_INFO(lppconn->prpl)->chat_info;

	if(lpfnChatInfo)
		return lpfnChatInfo(lppconn);

	return NULL;
}


/**
 * Joins a chat specified by a set of parameters.
 *
 * @param	lppac			Account.
 * @param	lphashParameters	Parameters specifying the chat.
 */
void PurpleJoinAdHocChat(PurpleAccount *lppac, GHashTable *lphashParameters)
{
	/* Make a new chat, join it and then delete it immediately. */
	PurpleChat *lpchat;

	if(!lppac || !lphashParameters)
		return;
	
	lpchat = purple_chat_new(lppac, NULL, lphashParameters);
	PurpleJoinChat(lpchat);
	purple_blist_remove_chat(lpchat);
}


/**
 * Joins a chat.
 *
 * @param	lpchat	Chat to join.
 */
void PurpleJoinChat(PurpleChat *lpchat)
{
	/* TODO: Check whether there's already a conversation for this chat.
	 * See Pidgin's gtk_blist_join_chat.
	 */

	serv_join_chat(lpchat->account->gc, lpchat->components);
}


/**
 * Called when users join a chat.
 *
 * @param	lpconv			Conversation.
 * @param	lpglistChatBuddies	List of PurpleConvChatBuddy structures
 *					describing new users.
 * @param	bNewArrivals		Whether entrance messages are shown.
 */
void PurpleChatAddUsers(PurpleConversation *lpconv, GList *lpglistChatBuddies, gboolean bNewArrivals)
{
	VULTURE_CHAT_ADD_USERS *lpvchataddusers = g_new(VULTURE_CHAT_ADD_USERS, 1);
	VULTURE_ADD_CHAT_USER *lpvaddchatuser;

	UNREFERENCED_PARAMETER(bNewArrivals);

	lpvchataddusers->lpvconvChat = lpconv->ui_data;
	lpvchataddusers->lpglistNewUsers = NULL;

	/* We make a new list for the UI's consumption. */
	for(; lpglistChatBuddies; lpglistChatBuddies = lpglistChatBuddies->next)
	{
		PurpleConvChatBuddy *lppccbuddy = lpglistChatBuddies->data;

		lpvaddchatuser = g_new(VULTURE_ADD_CHAT_USER, 1);
		lpvaddchatuser->szAlias = lppccbuddy->alias ? VultureUTF8ToTCHAR(lppccbuddy->alias) : NULL;
		lpvaddchatuser->szAliasKey = lppccbuddy->alias_key ? VultureUTF8ToTCHAR(lppccbuddy->alias_key) : NULL;
		lpvaddchatuser->szName = VultureUTF8ToTCHAR(lppccbuddy->name);
		lpvaddchatuser->bIsBuddy = lppccbuddy->buddy;
		lpvaddchatuser->pccbflags = lppccbuddy->flags;

		lpvchataddusers->lpglistNewUsers = g_list_prepend(lpvchataddusers->lpglistNewUsers, lpvaddchatuser);
	}

	lpvchataddusers->lpglistNewUsers = g_list_reverse(lpvchataddusers->lpglistNewUsers);

	VulturePostUIMessage(VUIMSG_CHATADDUSERS, lpvchataddusers);
}


/**
 * Frees a VULTURE_CHAT_ADD_USERS structure once the UI is done with it.
 *
 * @param	lpvchataddusers		Structure to free.
 */
void VultureFreeChatAddUsers(VULTURE_CHAT_ADD_USERS *lpvchataddusers)
{
	GList *lpglistRover;

	for(lpglistRover = lpvchataddusers->lpglistNewUsers; lpglistRover; lpglistRover = lpglistRover->next)
	{
		VULTURE_ADD_CHAT_USER *lpvaddchatuser = lpglistRover->data;

		if(lpvaddchatuser->szAlias) g_free(lpvaddchatuser->szAlias);
		if(lpvaddchatuser->szAliasKey) g_free(lpvaddchatuser->szAliasKey);
		if(lpvaddchatuser->szName) g_free(lpvaddchatuser->szName);

		g_free(lpvaddchatuser);
	}

	g_list_free(lpvchataddusers->lpglistNewUsers);

	g_free(lpvchataddusers);
}


/**
 * Called when a user is renamed in a chat.
 *
 * @param	lpconv		Conversation.
 * @param	szOldName	Old name.
 * @param	szNewName	New name.
 * @param	szNewAlias	New alias. Possibly NULL.
 */
void PurpleChatRenameUser(PurpleConversation *lpconv, const char *szOldName, const char *szNewName, const char *szNewAlias)
{
	VULTURE_CHAT_RENAME_USER *lpvchatrenameuser = g_new(VULTURE_CHAT_RENAME_USER, 1);

	lpvchatrenameuser->lpvconvChat = lpconv->ui_data;
	lpvchatrenameuser->szOldName = VultureUTF8ToTCHAR(szOldName);
	lpvchatrenameuser->szNewName = VultureUTF8ToTCHAR(szNewName);
	lpvchatrenameuser->szNewAlias = szNewAlias ? VultureUTF8ToTCHAR(szNewAlias) : NULL;

	VulturePostUIMessage(VUIMSG_CHATRENAMEUSER, lpvchatrenameuser);
}


/**
 * Frees a VULTURE_CHAT_RENAME_USER structure once the UI is done with it.
 *
 * @param	lpvchatrenameuser		Structure to free.
 */
void VultureFreeRenameUser(VULTURE_CHAT_RENAME_USER *lpvchatrenameuser)
{
	g_free(lpvchatrenameuser->szOldName);
	g_free(lpvchatrenameuser->szNewName);
	if(lpvchatrenameuser->szNewAlias) g_free(lpvchatrenameuser->szNewAlias);

	g_free(lpvchatrenameuser);
}


/**
 * Called when users leave a chat.
 *
 * @param	lpconv		Conversation.
 * @param	lpglistUsers	List of names of departing users.
 */
void PurpleChatRemoveUsers(PurpleConversation *lpconv, GList *lpglistUsers)
{
	VULTURE_CHAT_REMOVE_USERS *lpvchatremoveusers = g_new(VULTURE_CHAT_REMOVE_USERS, 1);

	lpvchatremoveusers->lpvconvChat = lpconv->ui_data;
	lpvchatremoveusers->lpglistNames = NULL;

	for(; lpglistUsers; lpglistUsers = lpglistUsers->next)
		lpvchatremoveusers->lpglistNames = g_list_prepend(lpvchatremoveusers->lpglistNames, VultureUTF8ToTCHAR(lpglistUsers->data));

	lpvchatremoveusers->lpglistNames = g_list_reverse(lpvchatremoveusers->lpglistNames);

	VulturePostUIMessage(VUIMSG_CHATREMOVEUSERS, lpvchatremoveusers);
}


/**
 * Frees a VULTURE_CHAT_ADD_USERS structure once the UI is done with it.
 *
 * @param	lpvchataddusers		Structure to free.
 */
void VultureFreeChatRemoveUsers(VULTURE_CHAT_REMOVE_USERS *lpvchatremoveusers)
{
	GList *lpglistRover;

	for(lpglistRover = lpvchatremoveusers->lpglistNames; lpglistRover; lpglistRover = lpglistRover->next)
		g_free(lpglistRover->data);

	g_list_free(lpvchatremoveusers->lpglistNames);

	g_free(lpvchatremoveusers);
}
