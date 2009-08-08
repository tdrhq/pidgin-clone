/*
 * Vulture - Win32 libpurple client
 *
 * purplequeue.c: Event queue for the interface with libpurple.
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

#include "vulture.h"
#include "purplemain.h"
#include "purplequeue.h"
#include "purplestatus.h"
#include "purpleacct.h"
#include "vultureconv.h"
#include "purpleconv.h"
#include "purpleblist.h"
#include "vultureblist.h"
#include "vulturedlg.h"
#include "vulturebicon.h"
#include "purplebicon.h"


/** Queue node representing a libpurple call. */
typedef struct _PURPLE_CALL
{
	int	iCallID;	/** < Function identifier. */
	void	*lpvParam;	/** < Function-specific data. */
	HANDLE	heventSync;	/** < Event for synchronous calls. */
} PURPLE_CALL;


/** A GSource-derived structure used to poll libpurple call queues. */
typedef struct _QUEUE_SOURCE
{
	GSource			gsource;	/** < Opaque structure used internally by glib. */
	GPollFD			gpollfdQueue;	/** < Event that signals queue population. */
	GQueue			*lpgq;		/** < Queue. */
	LPCRITICAL_SECTION	lpcs;		/** < Critical section protecting queue. */
} QUEUE_SOURCE;


static INLINE PURPLE_CALL* NewPurpleCall(int iCallID, void *lpvParam, HANDLE heventSync);
static void DispatchPurpleCall(PURPLE_CALL *lppurplecall);
static void DequeueAndDispatchPurpleCall(GQueue *lpgq, LPCRITICAL_SECTION lpcs, HANDLE heventQueue);
static QUEUE_SOURCE* CreateQueueSource(GQueue *lpgq, LPCRITICAL_SECTION lpcs, HANDLE heventQueue, gint iPriority);
static gboolean QueuePrepare(GSource *lpgsource, int *lpiTimeout);
static gboolean QueueCheck(GSource *lpgsource);
static gboolean QueueDispatch(GSource *lpgsource, GSourceFunc gsfCallback, gpointer lpvData);


#define EFFECTIVE_BUDDY(lpblistnode) \
	((lpblistnode) && PURPLE_BLIST_NODE_IS_CONTACT(lpblistnode) ? \
				(PurpleBlistNode*)purple_contact_get_priority_buddy((PurpleContact*)(lpblistnode)) : \
				(lpblistnode))

#define ASYNC_QUEUE_PRIORITY	G_PRIORITY_DEFAULT
#define SYNC_QUEUE_PRIORITY	(G_PRIORITY_DEFAULT - 1)


/* Events that indicate whether there are libpurple calls waiting in each
 * queue.
 */
static HANDLE g_heventSyncQueue = NULL, g_heventAsyncQueue = NULL;

static CRITICAL_SECTION g_csSyncQueue, g_csAsyncQueue;
static GQueue *g_lpgqSyncQueue = NULL, *g_lpgqAsyncQueue = NULL;


/** Initialises the queues of calls to libpurple from outside the thread. */
void VulturePurpleInitQueue(void)
{
	g_heventSyncQueue = CreateEvent(NULL, TRUE, FALSE, NULL);
	g_heventAsyncQueue = CreateEvent(NULL, TRUE, FALSE, NULL);

	InitializeCriticalSection(&g_csSyncQueue);
	InitializeCriticalSection(&g_csAsyncQueue);

	g_lpgqSyncQueue = g_queue_new();
	g_lpgqAsyncQueue = g_queue_new();
}


/** Destroys the queues of calls to libpurple from outside the thread. */
void VulturePurpleDestroyQueue(void)
{
	while(!g_queue_is_empty(g_lpgqAsyncQueue))
		g_free(g_queue_pop_head(g_lpgqAsyncQueue));

	while(!g_queue_is_empty(g_lpgqSyncQueue))
		g_free(g_queue_pop_head(g_lpgqSyncQueue));

	g_queue_free(g_lpgqAsyncQueue);
	g_queue_free(g_lpgqSyncQueue);

	DeleteCriticalSection(&g_csAsyncQueue);
	DeleteCriticalSection(&g_csSyncQueue);

	CloseHandle(g_heventAsyncQueue);
	CloseHandle(g_heventSyncQueue);
}


/**
 * Creates a PURPLE_CALL queue node.
 *
 * @param	iCallID		ID of the operation to perform.
 * @param	lpvParam	Function-specific data.
 * @param	heventSync	Event handle. Should be NULL iff async.
 *
 * @return New queue node. Free with g_free.
 */
static INLINE PURPLE_CALL* NewPurpleCall(int iCallID, void *lpvParam, HANDLE heventSync)
{
	PURPLE_CALL *lppurplecall = g_malloc(sizeof(PURPLE_CALL));

	lppurplecall->iCallID = iCallID;
	lppurplecall->lpvParam = lpvParam;
	lppurplecall->heventSync = heventSync;

	return lppurplecall;
}


/**
 * Enqueues an asynchronous call to libpurple.
 *
 * @param	iCallID		ID of the operation to perform.
 * @param	lpvParam	Function-specific data.
 */
void VultureEnqueueAsyncPurpleCall(int iCallID, void *lpvParam)
{
	PURPLE_CALL *lppurplecall = NewPurpleCall(iCallID, lpvParam, NULL);

	EnterCriticalSection(&g_csAsyncQueue);
		g_queue_push_tail(g_lpgqAsyncQueue, lppurplecall);
		SetEvent(g_heventAsyncQueue);
	LeaveCriticalSection(&g_csAsyncQueue);
}


/**
 * Enqueues a synchronous call to libpurple.
 *
 * @param	iCallID		ID of the operation to perform.
 * @param	lpvParam	Function-specific data.
 * @param	heventSync	Event handle for synchronisation.
 */
void VultureEnqueueSyncPurpleCall(int iCallID, void *lpvParam, HANDLE heventSync)
{
	PURPLE_CALL *lppurplecall = NewPurpleCall(iCallID, lpvParam, heventSync);

	EnterCriticalSection(&g_csSyncQueue);
		g_queue_push_tail(g_lpgqSyncQueue, lppurplecall);
		SetEvent(g_heventSyncQueue);
	LeaveCriticalSection(&g_csSyncQueue);
}


/**
 * Dispatches and frees a call to libpurple.
 *
 * @param	lppurplecall	Call info.
 */
static void DispatchPurpleCall(PURPLE_CALL *lppurplecall)
{
	switch(lppurplecall->iCallID)
	{
	case PC_GETALLSAVEDSTATUSES:
		PurpleGetBoxSavedStatuses((GList**)lppurplecall->lpvParam);
		break;

	case PC_GETACCOUNTS:
		{
			VULTURE_GET_ACCOUNTS *lpvgetaccounts = (VULTURE_GET_ACCOUNTS*)lppurplecall->lpvParam;
			PurpleGetAccounts(lpvgetaccounts->bOnlineOnly, &lpvgetaccounts->lpglistAccounts);
		}

		break;

	case PC_UPDATEPURPLEACCOUNT:
		PurpleApplyVultureAccount((VULTURE_ACCOUNT*)lppurplecall->lpvParam);
		break;

	case PC_SETSAVEDSTATUS:
		PurpleSetStatus((VULTURE_SAVED_STATUS*)lppurplecall->lpvParam);
		break;

	case PC_DESTROYCONVERSATION:
		purple_conversation_destroy(((VULTURE_CONVERSATION*)lppurplecall->lpvParam)->lpconv);
		break;

	case PC_CONVSEND:
		PurpleConversationSend((VULTURE_CONV_SEND*)lppurplecall->lpvParam);
		VultureFreeConvSend((VULTURE_CONV_SEND*)lppurplecall->lpvParam);
		break;

	case PC_GETSTATUSMSG:
		{
			/* Caller should g_free returned buffer if non-NULL. */
			PurpleSavedStatus *lppss = purple_savedstatus_get_current();
			const char *szMessage = purple_savedstatus_get_message(lppss);
			*((LPTSTR*)lppurplecall->lpvParam) = szMessage ? VultureUTF8ToTCHAR(szMessage) : NULL;
		}

		break;

	case PC_BLISTNODEACTIVATED:
		PurpleBListNodeActivated((VULTURE_BLIST_NODE*)lppurplecall->lpvParam);
		break;

	case PC_CONVGETTITLE:
		{
			VULTURE_CONV_GET_STRING *lpvcgetstr = (VULTURE_CONV_GET_STRING*)lppurplecall->lpvParam;
			lpvcgetstr->sz = PurpleConvGetTitle(lpvcgetstr->lpvconv->lpconv);
		}
		
		break;

	case PC_IMGETSTATUSMSG:
		{
			VULTURE_CONV_GET_STRING *lpvcgetstr = (VULTURE_CONV_GET_STRING*)lppurplecall->lpvParam;
			lpvcgetstr->sz = PurpleIMGetStatusText(lpvcgetstr->lpvconv->lpconv);
		}
		
		break;

	case PC_GETCHATFIELDS:
		{
			VULTURE_GET_CHAT_FIELDS *lpgetchatfields = lppurplecall->lpvParam;
			lpgetchatfields->lpglistFields = PurpleGetChatFields(lpgetchatfields->lpvac->lppac);
		}
		
		break;

	case PC_JOINCHAT:
		{
			VULTURE_JOIN_CHAT_DATA *lpvjcd = lppurplecall->lpvParam;
			PurpleJoinAdHocChat(lpvjcd->lppac, lpvjcd->lphashParameters);
		}
		
		break;

	case PC_CHATGETTOPIC:
		{
			VULTURE_CONV_GET_STRING *lpvcgetstr = (VULTURE_CONV_GET_STRING*)lppurplecall->lpvParam;
			lpvcgetstr->sz = PurpleChatGetTopic(lpvcgetstr->lpvconv->lpconv);
		}
		
		break;

	case PC_GETIMBUDDYICON:
		{
			VULTURE_GET_IM_BUDDY_ICON *lpvgetimbicon = lppurplecall->lpvParam;
			lpvgetimbicon->hbmIcon = PurpleGetIMBuddyIcon(lpvgetimbicon->lpvconv->lpconv, lpvgetimbicon->cxMax, lpvgetimbicon->cyMax);
		}
		
		break;

	case PC_MAKECONTEXTMENU:
		{
			VULTURE_MAKE_CONTEXT_MENU *lpvmcm = lppurplecall->lpvParam;
			
			EnterCriticalSection(&lpvmcm->lpvblistnode->cs);
				if(lpvmcm->lpvblistnode->lpblistnode)
				{
					if(lpvmcm->bExtraItems)
					{
						if(PURPLE_BLIST_NODE_IS_BUDDY(lpvmcm->lpvblistnode->lpblistnode) || PURPLE_BLIST_NODE_IS_CONTACT(lpvmcm->lpvblistnode->lpblistnode))
							PurpleMakeBuddyMenu(lpvmcm->hmenu, EFFECTIVE_BUDDY(lpvmcm->lpvblistnode->lpblistnode), lpvmcm->lplpglistVMA);
						else if(PURPLE_BLIST_NODE_IS_CHAT(lpvmcm->lpvblistnode->lpblistnode))
							PurpleMakeChatMenu(lpvmcm->hmenu, lpvmcm->lpvblistnode->lpblistnode, lpvmcm->lplpglistVMA);
					}

					PurpleCommonMakeMenu(lpvmcm->hmenu, lpvmcm->lpvblistnode->lpblistnode);
				}
			LeaveCriticalSection(&lpvmcm->lpvblistnode->cs);
		}

		break;


	case PC_PERFORMMENUACTION:
		{
			VULTURE_MENU_ACTION *lpvma = lppurplecall->lpvParam;
			lpvma->lpfnCallback(lpvma->lpvObject, lpvma->lpvData);
		}

		break;

	case PC_TOGGLESHOWOFFLINE:
		{
			PurpleBlistNode *lpblistnodeBuddy = EFFECTIVE_BUDDY(((VULTURE_BLIST_NODE*)lppurplecall->lpvParam)->lpblistnode);

			if(lpblistnodeBuddy)
			{
				purple_blist_node_set_bool(lpblistnodeBuddy, "show_offline", !purple_blist_node_get_bool(lpblistnodeBuddy, "show_offline"));
				PurpleBlistUpdateNode(NULL, lpblistnodeBuddy);
			}
		}

		break;

	case PC_ALIASNODE:
		{
			VULTURE_ALIAS_NODE *lpvaliasnode = lppurplecall->lpvParam;
			PurpleBlistAliasNode(lpvaliasnode->lpvblistnode->lpblistnode, lpvaliasnode->szAlias);
		}

		break;

	case PC_TOGGLEAUTOJOIN:
		{
			if(((VULTURE_BLIST_NODE*)lppurplecall->lpvParam)->lpblistnode)
			{
				purple_blist_node_set_bool(
					((VULTURE_BLIST_NODE*)lppurplecall->lpvParam)->lpblistnode,
					"vulture-autojoin",
					!purple_blist_node_get_bool(
						((VULTURE_BLIST_NODE*)lppurplecall->lpvParam)->lpblistnode,
						"vulture-autojoin"
						)
					);
			}
		}

		break;

	case PC_REMOVEBLISTNODE:
		PurpleDeleteBlistNode(((VULTURE_BLIST_NODE*)lppurplecall->lpvParam)->lpblistnode);
		break;

	case PC_BLISTNODEHASCHILDREN:
		{
			VULTURE_BLIST_NODE_GET_BOOL *lpvbngetbool = lppurplecall->lpvParam;
			lpvbngetbool->bReturn = lpvbngetbool->lpvblistnode->lpblistnode && lpvbngetbool->lpvblistnode->lpblistnode->child;
		}

		break;

	case PC_SETCUSTOMICON:
		{
			VULTURE_BLIST_NODE_STRING_PAIR *lpvblnstringpair = lppurplecall->lpvParam;

			EnterCriticalSection(&lpvblnstringpair->lpvblistnode->cs);
				if(lpvblnstringpair->lpvblistnode->lpblistnode)
				{
					if(lpvblnstringpair->sz)
					{
						gchar *szFilename = VultureTCHARToUTF8(lpvblnstringpair->sz);

						purple_buddy_icons_node_set_custom_icon_from_file(lpvblnstringpair->lpvblistnode->lpblistnode, szFilename);

						g_free(szFilename);
					}
					else
					{
						purple_buddy_icons_node_set_custom_icon(lpvblnstringpair->lpvblistnode->lpblistnode, NULL, 0);
					}
				}
			LeaveCriticalSection(&lpvblnstringpair->lpvblistnode->cs);

			VultureBListNodeAddRef(lpvblnstringpair->lpvblistnode);
			VulturePostUIMessage(VUIMSG_UPDATEBLISTNODE, lpvblnstringpair->lpvblistnode);
		}

		break;

	case PC_UPDATEBLISTCHILDREN:
		{
			PurpleBlistNode *lpblistnode = ((VULTURE_BLIST_NODE*)lppurplecall->lpvParam)->lpblistnode;
			
			if(lpblistnode)
			{
				PurpleBlistNode *lpblistnodeChild;

				for(lpblistnodeChild = purple_blist_node_get_first_child(lpblistnode);
					lpblistnodeChild;
					lpblistnodeChild = purple_blist_node_get_sibling_next(lpblistnodeChild))
				{
					PurpleBlistUpdateNode(purple_get_blist(), lpblistnodeChild);
				}
			}
		}

		break;

	case PC_GETBLISTNODEICON:
		{
			VULTURE_GET_BLIST_NODE_ICON *lpvgblnicon = lppurplecall->lpvParam;
			lpvgblnicon->hbmIcon = lpvgblnicon->lpvblistnode ?
				PurpleGetBlistNodeIcon(lpvgblnicon->lpvblistnode->lpblistnode, -1, -1) :
				NULL;
		}
		
		break;

	case PC_GETGROUPS:
		*((GList**)lppurplecall->lpvParam) = PurpleGetGroups();
		break;

	case PC_ADDBUDDY:
		PurpleAddBuddy(lppurplecall->lpvParam);
		break;

	case PC_REFRESHBLIST:
		PurpleBlistUpdateLeaves(purple_get_blist()->root);
		break;

	case PC_STARTIM:
		{
			VULTURE_ADD_BUDDY_DATA *lpvabd = lppurplecall->lpvParam;
			gchar *szUsernameUTF8 = VultureTCHARToUTF8(lpvabd->szUsername);
			purple_conversation_new(PURPLE_CONV_TYPE_IM, lpvabd->lppac, szUsernameUTF8);
			g_free(szUsernameUTF8);
		}

		break;

	case PC_ADDCHAT:
		PurpleAddChat(lppurplecall->lpvParam);
		break;

	case PC_ADDGROUP:
		PurpleAddGroup(lppurplecall->lpvParam);
		break;

	case PC_QUIT:
		purple_core_quit();
		g_main_loop_quit(g_lpgmainloop);
		break;
	}

	/* Let the caller know we're done if they care. */
	if(lppurplecall->heventSync)
		SetEvent(lppurplecall->heventSync);

	g_free(lppurplecall);
}


/**
 * Dequeues the call at the head of a queue, dispatches and frees it. The queue
 * must not be empty. The population event is cleared if the last event in the
 * queue is dispatched.
 *
 * @param	lpgq		Queue of calls.
 * @param	lpcs		Critical section for access to the queue.
 * @param	heventQueue	Population event for the queue.
 */
static void DequeueAndDispatchPurpleCall(GQueue *lpgq, LPCRITICAL_SECTION lpcs, HANDLE heventQueue)
{
	PURPLE_CALL *lppurplecall;

	EnterCriticalSection(lpcs);
		lppurplecall = g_queue_pop_head(lpgq);
	LeaveCriticalSection(lpcs);

	DispatchPurpleCall(lppurplecall);

	EnterCriticalSection(lpcs);
		if(g_queue_is_empty(lpgq))
			ResetEvent(heventQueue);
	LeaveCriticalSection(lpcs);
}


/**
 * Creates a new GSource-derived object for polling call-queues.
 *
 * @param	lpgq		Queue of calls.
 * @param	lpcs		Critical section for access to the queue.
 * @param	heventQueue	Handle signalling queue population.
 * @param	iPriority	Priority to assign to the source.
 *
 * @return Source object. Release a reference using g_source_unref when the
 * caller is finished with it.
 */
static QUEUE_SOURCE* CreateQueueSource(GQueue *lpgq, LPCRITICAL_SECTION lpcs, HANDLE heventQueue, gint iPriority)
{
	static GSourceFuncs gsfQueue = {QueuePrepare, QueueCheck, QueueDispatch, NULL, NULL, NULL};
	QUEUE_SOURCE *lpqsource = (QUEUE_SOURCE*)g_source_new(&gsfQueue, sizeof(QUEUE_SOURCE));

	lpqsource->gpollfdQueue.events = G_IO_IN;
	lpqsource->gpollfdQueue.fd = (gint)heventQueue;

	lpqsource->lpgq = lpgq;
	lpqsource->lpcs = lpcs;

	g_source_add_poll(&lpqsource->gsource, &lpqsource->gpollfdQueue);

	g_source_set_priority(&lpqsource->gsource, iPriority);

	return lpqsource;
}


/**
 * prepare() function for a QUEUE_SOURCE. We always want to poll, so we just
 * return FALSE after specifying an infinite timeout.
 *
 * @param	lpgsource	Ignored.
 * @param[out]	lpiTimeout	Set to -1.
 *
 * @return Always FALSE.
 */
static gboolean QueuePrepare(GSource *lpgsource, int *lpiTimeout)
{
	UNREFERENCED_PARAMETER(lpgsource);
	*lpiTimeout = -1;
	return FALSE;
}


/**
 * check() function for a QUEUE_SOURCE. Determines whether there are any calls
 * enqueued (which trivially there will be, as we are only polling one handle
 * in each source and *one* handle in the source is set).
 *
 * @param	lpgsource	QUEUE_SOURCE.
 *
 * @return Whether there are messages waiting.
 */
static gboolean QueueCheck(GSource *lpgsource)
{
	/* Check whether the event is signalled. */
	return (WaitForSingleObject((HANDLE)((QUEUE_SOURCE*)lpgsource)->gpollfdQueue.fd, 0) == WAIT_OBJECT_0);
}


/**
 * dispatch() function for a QUEUE_SOURCE. Dispatches a message from the front
 * of the queue.
 *
 * @param	lpgsource	QUEUE_SOURCE.
 * @param	gsfCallback	Ignored.
 * @param	lpvData		Ignored.
 *
 * @return Always TRUE.
 */
static gboolean QueueDispatch(GSource *lpgsource, GSourceFunc gsfCallback, gpointer lpvData)
{
	QUEUE_SOURCE *lpqsource = (QUEUE_SOURCE*)lpgsource;

	UNREFERENCED_PARAMETER(gsfCallback);
	UNREFERENCED_PARAMETER(lpvData);

	EnterCriticalSection(lpqsource->lpcs);

		/* Sanity check: make sure we have a call to dispatch. */
		if(WaitForSingleObject((HANDLE)lpqsource->gpollfdQueue.fd, 0) == WAIT_OBJECT_0)
		{
			DequeueAndDispatchPurpleCall(lpqsource->lpgq, lpqsource->lpcs, (HANDLE)lpqsource->gpollfdQueue.fd);
		}

	LeaveCriticalSection(lpqsource->lpcs);

	return TRUE;
}


/**
 * Creates a new GSource-derived object for polling the async queue.
 *
 * @return Source object. Release a reference using g_source_unref when the
 * caller is finished with it.
 */
GSource* VultureCreateAsyncQueueSource(void)
{
	return &CreateQueueSource(g_lpgqAsyncQueue, &g_csAsyncQueue, g_heventAsyncQueue, ASYNC_QUEUE_PRIORITY)->gsource;
}


/**
 * Creates a new GSource-derived object for polling the sync queue.
 *
 * @return Source object. Release a reference using g_source_unref when the
 * caller is finished with it.
 */
GSource* VultureCreateSyncQueueSource(void)
{
	return &CreateQueueSource(g_lpgqSyncQueue, &g_csSyncQueue, g_heventSyncQueue, SYNC_QUEUE_PRIORITY)->gsource;
}


/**
 * Waits for libpurple calls in a given context to complete, and then destroys
 * the context.
 *
 * @param	lpgarrayWaitContext	Wait context.
 */
void VulturePurpleWait(GArray *lpgarrayWaitContext)
{
	int i;

	if(lpgarrayWaitContext->len > 0)
		WaitForMultipleObjects(lpgarrayWaitContext->len, (HANDLE*)lpgarrayWaitContext->data, TRUE, INFINITE);
	
	for(i = 0; i < lpgarrayWaitContext->len; i++)
		CloseHandle(g_array_index(lpgarrayWaitContext, HANDLE, i));

	g_array_free(lpgarrayWaitContext, TRUE);
}


/**
 * Enqueues a synchronous call to libpurple, creating a handle for
 * synchornisation and adding it to the given wait context.
 *
 * @param	iCallID			ID of the operation to perform.
 * @param	lpvParam		Function-specific data.
 * @param	lpgarrayWaitContext	Wait context for synchronisation.
 */
void VultureEnqueueMultiSyncPurpleCall(int iCallID, void *lpvParam, GArray *lpgarrayWaitContext)
{
	HANDLE hevent = CreateEvent(NULL, TRUE, FALSE, NULL);
	g_array_append_val(lpgarrayWaitContext, hevent);
	VultureEnqueueSyncPurpleCall(iCallID, lpvParam, hevent);
}


/**
 * Makes and waits for a single synchronous call to libpurple.
 *
 * @param	iCallID			ID of the operation to perform.
 * @param	lpvParam		Function-specific data.
 */
void VultureSingleSyncPurpleCall(int iCallID, void *lpvParam)
{
	HANDLE hevent = CreateEvent(NULL, TRUE, FALSE, NULL);
	VultureEnqueueSyncPurpleCall(iCallID, lpvParam, hevent);
	WaitForSingleObject(hevent, INFINITE);
	CloseHandle(hevent);
}
