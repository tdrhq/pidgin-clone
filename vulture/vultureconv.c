/*
 * Vulture - Win32 libpurple client
 *
 * vultureconv.c: Conversation UI.
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
#include <commctrl.h>
#include <richedit.h>
#include <glib.h>
#include <string.h>
#include <tchar.h>
#include <stdlib.h>

#include "vulture.h"
#include "resource.h"
#include "vultureconv.h"
#include "purpleconv.h"
#include "vultureblist.h"
#include "purplemain.h"
#include "purplequeue.h"
#include "vulturebicon.h"


#define CONVCONTAINERCLASS	TEXT("VULTURECONVCONTAINER")
#define CONV_DLG_MARGIN		4


typedef struct _CONVCONTAINERDATA
{
	HWND	hwndTabDlg;
	int	iSelectedTab;
} CONVCONTAINERDATA;


static int g_cyInput = 48;
static int g_cxNames = 128;


static LRESULT CALLBACK ConvContainerWndProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK ConvContTabDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static void RecalcTabIndices(HWND hwndTabs);
static INT_PTR CALLBACK IMDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK ChatDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK ConvCommonDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static void ResizeActiveConversationWindow(HWND hwndConvContainer, HWND hwndTabs);
static void RepositionConvControls(HWND hwndConvDlg, VULTURE_CONVERSATION *lpvconv);
static LRESULT CALLBACK InputBoxSubclassProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static void EnableAppropriateConvWindow(CONVCONTAINERDATA *lpccd);
static void SetConvTitle(VULTURE_CONVERSATION *lpvconv, HWND hwndTabs, LPTSTR szTitle);
static void UpdateIMStatusText(HWND hwndDlg, VULTURE_CONVERSATION *lpvconv);
static void FreeChatUser(void *lpvChatUser);
static int CALLBACK UserListComparator(LPARAM lParam1, LPARAM lParam2, LPARAM lParamUnused);
static INLINE void SortUserList(HWND hwndTVUsers);
static void UpdateIMIcon(HWND hwndDlg, VULTURE_CONVERSATION *lpvconv);


/**
 * Registers the window class for conversation containers.
 *
 * @return Zero on success; non-zero on error.
 */
int VultureRegisterConvContainerWindowClass(void)
{
	WNDCLASSEX wndclassex;

	wndclassex.cbClsExtra = 0;
	wndclassex.cbSize = sizeof(wndclassex);
	wndclassex.cbWndExtra = 0;
	wndclassex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclassex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclassex.hIconSm = LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR | LR_SHARED);
	wndclassex.hInstance = g_hInstance;
	wndclassex.lpfnWndProc = ConvContainerWndProc;
	wndclassex.lpszClassName = CONVCONTAINERCLASS;
	wndclassex.lpszMenuName = MAKEINTRESOURCE(IDM_CONV);
	wndclassex.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;

	if(!RegisterClassEx(&wndclassex))
		return 1;

	return 0;
}


/**
 * Creates a conversation-container window.
 *
 * @return Window handle, or NULL on error.
 */
HWND VultureCreateConvContainer(void)
{
	return CreateWindowEx(
		WS_EX_WINDOWEDGE,
		CONVCONTAINERCLASS,
		cg_szAppName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,

		/* TODO: Read geometry from preferences. */
		CW_USEDEFAULT, CW_USEDEFAULT,
		480, 640,

		NULL, 
		NULL,
		g_hInstance,
		NULL);
}


/**
 * Window procedure for conversation containers.
 *
 * @param	hwnd	Buddy-list handle.
 * @param	uiMsg	Message ID.
 * @param	wParam	Message-specific.
 * @param	lParam	Message-specific.
 *
 * @return Message-specific.
 */
static LRESULT CALLBACK ConvContainerWndProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	CONVCONTAINERDATA *lpccd;

	switch(uiMsg)
	{
	case WM_CREATE:
		lpccd = (CONVCONTAINERDATA*)ProcHeapAlloc(sizeof(CONVCONTAINERDATA));
		SetWindowLongPtr(hwnd, GWL_USERDATA, (LONG_PTR)lpccd);

		/* Create the tab control. */
		lpccd->hwndTabDlg = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_CONVCONT), hwnd, ConvContTabDlgProc);

		/* No tab selected initially. */
		lpccd->iSelectedTab = -1;

		return 0;

	case WM_PURPLEUIMSG:

		lpccd = (CONVCONTAINERDATA*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

		switch(wParam)
		{
		case VUIMSG_NEWCONVERSATION:
			{
				VULTURE_CONVERSATION *lpvconv = (VULTURE_CONVERSATION*)lParam;
				TCITEM tcitem;
				HWND hwndTabs = GetDlgItem(lpccd->hwndTabDlg, IDC_TAB_CONVERSATIONS);

				lpvconv->hwndContainer = hwnd;

				/* Create a new tab. */
				tcitem.mask = TCIF_PARAM;
				tcitem.lParam = lParam;

				lpvconv->iTabIndex = TabCtrl_InsertItem(hwndTabs, TabCtrl_GetItemCount(hwndTabs), &tcitem);

				/* Create conversation dialogue. It is
				 * initially disabled and hidden. The window
				 * handle is saved by the dialogue procedure.
				 */
				if(lpvconv->convtype == PURPLE_CONV_TYPE_IM)
					CreateDialogParam(g_hInstance, MAKEINTRESOURCE(IDD_IM), hwndTabs, IMDlgProc, (LPARAM)lpvconv);
				else
					CreateDialogParam(g_hInstance, MAKEINTRESOURCE(IDD_CHAT), hwndTabs, ChatDlgProc, (LPARAM)lpvconv);

				SetWindowPos(lpvconv->hwndConv, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOREPOSITION | SWP_NOSIZE);

				/* Only strictly necessary if we're the only
				 * tab.
				 */
				EnableAppropriateConvWindow(lpccd);

				ResizeActiveConversationWindow(hwnd, hwndTabs);
				RecalcTabIndices(hwndTabs);
			}

			break;

		case VUIMSG_DESTROYEDCONVERSATION:
			{
				/* Core is getting rid of conversation. Remove
				 * the UI.
				 */

				VULTURE_CONVERSATION *lpvconv = (VULTURE_CONVERSATION*)lParam;
				HWND hwndTabs = GetDlgItem(lpccd->hwndTabDlg, IDC_TAB_CONVERSATIONS);

				TabCtrl_DeleteItem(hwndTabs, lpvconv->iTabIndex);
				if(lpvconv->hwndConv) DestroyWindow(lpvconv->hwndConv);

				VultureFreeConversation(lpvconv);
				

				/* If we were the last conversation, take down
				 * the container, too.
				 */
				if(TabCtrl_GetItemCount(hwndTabs) == 0)
					DestroyWindow(hwnd);
				else
					RecalcTabIndices(hwndTabs);
			}

			break;

		case VUIMSG_CONVCHANGED:
			{
				VULTURE_CONV_CHANGED *lpvcchanged = (VULTURE_CONV_CHANGED*)lParam;
				HWND hwndTabs = GetDlgItem(lpccd->hwndTabDlg, IDC_TAB_CONVERSATIONS);

				switch(lpvcchanged->pcut)
				{
				case PURPLE_CONV_UPDATE_TITLE:
					{
						VULTURE_CONV_GET_STRING vcgetstring;

						vcgetstring.lpvconv = lpvcchanged->lpvconv;
						VultureSingleSyncPurpleCall(PC_CONVGETTITLE, &vcgetstring);
						SetConvTitle(lpvcchanged->lpvconv, hwndTabs, vcgetstring.sz ? vcgetstring.sz : TEXT(""));
						if(vcgetstring.sz) g_free(vcgetstring.sz);
					}

					break;

				default:
					/* If we don't know what to do with it,
					 * forward it to the conversastion
					 * window itself.
					 */
					SendMessage(lpvcchanged->lpvconv->hwndConv, uiMsg, wParam, lParam);
					break;
				}

				g_free(lpvcchanged);
			}

			break;
		}

		return 0;


	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDM_CONV_CONV_CLOSE:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;
		}

		break;


	case WM_NOTIFY:
		{
			LPNMHDR lpnmhdr = (LPNMHDR)lParam;

			if(lpnmhdr->idFrom == IDC_TAB_CONVERSATIONS && lpnmhdr->code == TCN_SELCHANGE)
			{
				EnableAppropriateConvWindow((CONVCONTAINERDATA*)GetWindowLongPtr(hwnd, GWLP_USERDATA));
				ResizeActiveConversationWindow(hwnd, lpnmhdr->hwndFrom);
				return 0;
			}
		}

		break;

	case WM_SIZE:
		lpccd = (CONVCONTAINERDATA*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		SetWindowPos(
			lpccd->hwndTabDlg,
			NULL,
			0,
			0,
			LOWORD(lParam),
			HIWORD(lParam),
			SWP_NOZORDER | SWP_NOACTIVATE);
		ResizeActiveConversationWindow(hwnd, GetDlgItem(lpccd->hwndTabDlg, IDC_TAB_CONVERSATIONS));

		return 0;


	case WM_ACTIVATE:
		if(LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE)
			FlashWindow(hwnd, FALSE);

		break;


	case WM_CLOSE:
		{
			HWND hwndTabs;
			int i, iCount;
			TCITEM tcitem;

			lpccd = (CONVCONTAINERDATA*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			hwndTabs = GetDlgItem(lpccd->hwndTabDlg, IDC_TAB_CONVERSATIONS);
			iCount = TabCtrl_GetItemCount(hwndTabs);

			tcitem.mask = TCIF_PARAM;

			for(i = 0; i < iCount; i++)
			{
				TabCtrl_GetItem(hwndTabs, i, &tcitem);
				VultureEnqueueAsyncPurpleCall(PC_DESTROYCONVERSATION, (void*)tcitem.lParam);
			}

			/* We're not going to be around much longer, but ignore
			 * the user in the meantime.
			 */
			EnableWindow(hwnd, FALSE);
		}
		return 0;


	case WM_NCDESTROY:

		lpccd = (CONVCONTAINERDATA*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

		g_lpglistConvContainers = g_list_remove(g_lpglistConvContainers, hwnd);

		ProcHeapFree(lpccd);

		return 0;
	}

	return DefWindowProc(hwnd, uiMsg, wParam, lParam);
}



/**
 * Dialogue procedure for conversation tabs.
 *
 * @param	hwndDlg		Dialogue window handle.
 * @param	uiMsg		Message ID.
 * @param	wParam		Message-specific.
 * @param	lParam		Message-specific.
 *
 * @return Usually TRUE if message processed and FALSE otherwise. There are
 * some exceptions for particular messages.
 */
static INT_PTR CALLBACK ConvContTabDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		/* Let the system set the focus. */
		return TRUE;

	case WM_SIZE:
		SetWindowPos(
			GetDlgItem(hwndDlg, IDC_TAB_CONVERSATIONS),
			NULL,
			CONV_DLG_MARGIN,
			CONV_DLG_MARGIN,
			LOWORD(lParam) - 2 * CONV_DLG_MARGIN,
			HIWORD(lParam) - 2 * CONV_DLG_MARGIN,
			SWP_NOZORDER | SWP_NOACTIVATE);

		return TRUE;

	case WM_NOTIFY:
		/* Forward to parent. */
		SendMessage(GetParent(hwndDlg), uiMsg, wParam, lParam);
		return TRUE;
	}

	return FALSE;
}


/**
 * Reclaculates tab indices cached in conversation structures.
 *
 * @param	hwndTabs	Tab control.
 */
static void RecalcTabIndices(HWND hwndTabs)
{
	int i, iCount = TabCtrl_GetItemCount(hwndTabs);
	TCITEM tcitem;

	tcitem.mask = TCIF_PARAM;

	for(i = 0; i < iCount; i++)
	{
		TabCtrl_GetItem(hwndTabs, i, &tcitem);
		((VULTURE_CONVERSATION*)tcitem.lParam)->iTabIndex = i;
	}
}


/**
 * Dialogue procedure for IM dialogues. Delegates processing common with chats
 * to ConvCommonDlgProc.
 *
 * @param	hwndDlg		Dialogue window handle.
 * @param	uiMsg		Message ID.
 * @param	wParam		Message-specific.
 * @param	lParam		Message-specific.
 *
 * @return Usually TRUE if message processed and FALSE otherwise. There are
 * some exceptions for particular messages.
 */
static INT_PTR CALLBACK IMDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		UpdateIMStatusText(hwndDlg, (VULTURE_CONVERSATION*)lParam);

		/* Get the icon control to the right size. */
		RepositionConvControls(hwndDlg, (VULTURE_CONVERSATION*)lParam);

		UpdateIMIcon(hwndDlg, (VULTURE_CONVERSATION*)lParam);

		break;

	case WM_PURPLEUIMSG:
		switch(wParam)
		{
		case VUIMSG_UPDATEIMSTATUSTEXT:
			UpdateIMStatusText(hwndDlg, (VULTURE_CONVERSATION*)lParam);
			break;

		case VUIMSG_CONVCHANGED:
			{
				VULTURE_CONV_CHANGED *lpvcchanged = (VULTURE_CONV_CHANGED*)lParam;

				if(lpvcchanged->pcut == PURPLE_CONV_UPDATE_ICON)
					UpdateIMIcon(hwndDlg, lpvcchanged->lpvconv);
			}

			break;
		}

		break;
	}

	return ConvCommonDlgProc(hwndDlg, uiMsg, wParam, lParam);
}


/**
 * Dialogue procedure for chat dialogues. Delegates processing common with IMs
 * to ConvCommonDlgProc.
 *
 * @param	hwndDlg		Dialogue window handle.
 * @param	uiMsg		Message ID.
 * @param	wParam		Message-specific.
 * @param	lParam		Message-specific.
 *
 * @return Usually TRUE if message processed and FALSE otherwise. There are
 * some exceptions for particular messages.
 */
static INT_PTR CALLBACK ChatDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	VULTURE_CONVERSATION_CHAT *lpvconvchat;

	switch(uiMsg)
	{
	case WM_INITDIALOG:
		lpvconvchat = (VULTURE_CONVERSATION_CHAT*)lParam;
		lpvconvchat->lpgtreePeople = g_tree_new_full(VultureCompareTCHARStrings, NULL, free, FreeChatUser);
		break;

	case WM_PURPLEUIMSG:

		lpvconvchat = (VULTURE_CONVERSATION_CHAT*)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);

		switch(wParam)
		{
		case VUIMSG_CHATADDUSERS:
			{
				VULTURE_CHAT_ADD_USERS *lpvchataddusers = (VULTURE_CHAT_ADD_USERS*)lParam;
				GList *lpglistRover;
				HWND hwndTVNames = GetDlgItem(hwndDlg, IDC_TREE_NAMES);

				/* Add the new users to the binary tree, and
				 * create tree-view entries for them.
				 */
				for(lpglistRover = lpvchataddusers->lpglistNewUsers; lpglistRover; lpglistRover = lpglistRover->next)
				{
					VULTURE_CHAT_USER *lpvcu = ProcHeapAlloc(sizeof(VULTURE_CHAT_USER));
					VULTURE_ADD_CHAT_USER *lpvaddchatuser = lpglistRover->data;
					TVINSERTSTRUCT tvis;

					lpvcu->szName = _tcsdup(lpvaddchatuser->szName);
					lpvcu->szAlias = lpvaddchatuser->szAlias ? _tcsdup(lpvaddchatuser->szAlias) : NULL;
					lpvcu->szAliasKey = lpvaddchatuser->szAliasKey ? _tcsdup(lpvaddchatuser->szAliasKey) : NULL;
					lpvcu->bIsBuddy = lpvaddchatuser->bIsBuddy;
					lpvcu->pccbflags = lpvaddchatuser->pccbflags;

					/* Add to tree-view. */
					tvis.hParent = TVI_ROOT;
					tvis.hInsertAfter = TVI_ROOT;
					tvis.item.mask = TVIF_PARAM | TVIF_TEXT;
					tvis.item.pszText = lpvaddchatuser->szName;
					tvis.item.lParam = (LPARAM)lpvcu;
					lpvcu->hti = TreeView_InsertItem(hwndTVNames, &tvis);

					/* Add to binary tree. */
					g_tree_insert(lpvconvchat->lpgtreePeople, _tcsdup(lpvaddchatuser->szName), lpvcu);
				}

				VultureFreeChatAddUsers(lpvchataddusers);

				SortUserList(hwndTVNames);
			}

			break;

		case VUIMSG_CHATRENAMEUSER:
			{
				VULTURE_CHAT_RENAME_USER *lpvchatrenameuser = (VULTURE_CHAT_RENAME_USER*)lParam;
				VULTURE_CHAT_USER *lpvchatuser = g_tree_lookup(lpvconvchat->lpgtreePeople, lpvchatrenameuser->szOldName);
				VULTURE_CHAT_USER *lpvchatuserNew;
				TVITEM tvitem;
				HWND hwndTVNames = GetDlgItem(hwndDlg, IDC_TREE_NAMES);

				tvitem.mask = TVIF_TEXT | TVIF_PARAM;
				tvitem.hItem = lpvchatuser->hti;
				tvitem.pszText = lpvchatrenameuser->szNewName;

				/* Create new entry for this user. */
				lpvchatuserNew = ProcHeapAlloc(sizeof(VULTURE_CHAT_USER));
				lpvchatuserNew->szAlias = lpvchatrenameuser->szNewAlias ? _tcsdup(lpvchatrenameuser->szNewAlias) : NULL;
				lpvchatuserNew->szAliasKey = lpvchatuser->szAliasKey ? _tcsdup(lpvchatuser->szAliasKey) : NULL;
				lpvchatuserNew->bIsBuddy = lpvchatuser->bIsBuddy;
				lpvchatuserNew->pccbflags = lpvchatuser->pccbflags;
				lpvchatuserNew->hti = lpvchatuser->hti;

				/* Remove old entry from binary tree. */
				g_tree_remove(lpvconvchat->lpgtreePeople, lpvchatrenameuser->szOldName);

				/* Add new entry. */
				g_tree_insert(lpvconvchat->lpgtreePeople, lpvchatrenameuser->szNewName, lpvchatuserNew);

				/* Update tree-view. */
				tvitem.lParam = (LPARAM)lpvchatuser;
				TreeView_SetItem(hwndTVNames, &tvitem);

				VultureFreeRenameUser(lpvchatrenameuser);

				SortUserList(hwndTVNames);
			}

			break;

		case VUIMSG_CHATREMOVEUSERS:
			{
				VULTURE_CHAT_REMOVE_USERS *lpvchatremoveusers = (VULTURE_CHAT_REMOVE_USERS*)lParam;
				GList *lpglistRover;
				HWND hwndTVNames = GetDlgItem(hwndDlg, IDC_TREE_NAMES);

				for(lpglistRover = lpvchatremoveusers->lpglistNames; lpglistRover; lpglistRover = lpglistRover->next)
				{
					VULTURE_CHAT_USER *lpvchatuser = g_tree_lookup(lpvconvchat->lpgtreePeople, lpglistRover->data);

					/* Remove from tree-view. */
					TreeView_DeleteItem(hwndTVNames, lpvchatuser->hti);

					/* Remove from binary tree. */
					g_tree_remove(lpvconvchat->lpgtreePeople, lpglistRover->data);
				}

				VultureFreeChatRemoveUsers(lpvchatremoveusers);

				SortUserList(hwndTVNames);
			}

			break;

		case VUIMSG_CONVCHANGED:
			{
				VULTURE_CONV_CHANGED *lpvcchanged = (VULTURE_CONV_CHANGED*)lParam;

				if(lpvcchanged->pcut == PURPLE_CONV_UPDATE_TOPIC)
				{
					VULTURE_CONV_GET_STRING vcgetstring;

					vcgetstring.lpvconv = lpvcchanged->lpvconv;
					VultureSingleSyncPurpleCall(PC_CHATGETTOPIC, &vcgetstring);
					SetDlgItemText(hwndDlg, IDC_STATIC_TOPIC, vcgetstring.sz ? vcgetstring.sz : TEXT(""));
					if(vcgetstring.sz) g_free(vcgetstring.sz);
				}
			}

			break;
		}

		break;

	case WM_DESTROY:
		lpvconvchat = (VULTURE_CONVERSATION_CHAT*)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);
		g_tree_destroy(lpvconvchat->lpgtreePeople);
		break;
	}

	return ConvCommonDlgProc(hwndDlg, uiMsg, wParam, lParam);
}


/**
 * Handles processing common to IM and chat windows.
 *
 * @param	hwndDlg		Dialogue window handle.
 * @param	uiMsg		Message ID.
 * @param	wParam		Message-specific.
 * @param	lParam		Message-specific.
 *
 * @return Usually TRUE if message processed and FALSE otherwise. There are
 * some exceptions for particular messages.
 */
static INT_PTR CALLBACK ConvCommonDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	VULTURE_CONVERSATION *lpvconv;

	switch(uiMsg)
	{
	case WM_INITDIALOG:
		{
			CONVCONTAINERDATA *lpccd;
			HWND hwndREInput = GetDlgItem(hwndDlg, IDC_RICHEDIT_INPUT);
			VULTURE_CONV_GET_STRING vcgetstring;
			HWND hwndTabs;

			/* Remember the conversation. */
			lpvconv = (VULTURE_CONVERSATION*)lParam;
			SetWindowLongPtr(hwndDlg, GWLP_USERDATA, lParam);

			lpvconv->hwndConv = hwndDlg;

			/* Parent data. */
			lpccd = (CONVCONTAINERDATA*)GetWindowLongPtr(lpvconv->hwndContainer, GWLP_USERDATA);
			hwndTabs = GetDlgItem(lpccd->hwndTabDlg, IDC_TAB_CONVERSATIONS);

			/* Subclass the input box. */
			lpvconv->wndprocInputOrig = (WNDPROC)GetWindowLongPtr(hwndREInput, GWLP_WNDPROC);
			SetWindowLongPtr(hwndREInput, GWLP_WNDPROC, (LONG)InputBoxSubclassProc);

			/* Set the title, both of the tab and in the
			 * conversation window itself.
			 */
			vcgetstring.lpvconv = lpvconv;
			VultureSingleSyncPurpleCall(PC_CONVGETTITLE, &vcgetstring);
			SetConvTitle(lpvconv, hwndTabs, vcgetstring.sz ? vcgetstring.sz : TEXT(""));
			if(vcgetstring.sz) g_free(vcgetstring.sz);
		}
		
		/* Let the system set the focus. */
		return TRUE;

	case WM_INPUTENTER:
		/* User pressed Enter in input box. Send message. */
		if(GetWindowTextLength((HWND)lParam) > 0)
		{
			VULTURE_CONV_SEND *lpvcsend = ProcHeapAlloc(sizeof(VULTURE_CONV_SEND));
			GETTEXTLENGTHEX gtlex;
			GETTEXTEX gettextex;
			int cchInput;

#ifdef UNICODE
			const UINT c_codepage = 1200;
#else
			const UINT c_codepage = CP_ACP;
#endif

			gtlex.flags = GTL_CLOSE | GTL_NUMCHARS;
			gtlex.codepage = c_codepage;
			cchInput = SendMessage((HWND)lParam, EM_GETTEXTLENGTHEX, (WPARAM)&gtlex, 0) + 1;
			lpvcsend->szMessage = ProcHeapAlloc(cchInput * sizeof(TCHAR));

			gettextex.cb = cchInput * sizeof(TCHAR);
			gettextex.codepage = c_codepage;
			gettextex.flags = GT_DEFAULT;
			gettextex.lpDefaultChar = NULL;
			gettextex.lpUsedDefChar = NULL;
			SendMessage((HWND)lParam, EM_GETTEXTEX, (WPARAM)&gettextex, (LPARAM)lpvcsend->szMessage);

			SetWindowText((HWND)lParam, TEXT(""));

			lpvcsend->lpvconv = (VULTURE_CONVERSATION*)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);
			VultureEnqueueAsyncPurpleCall(PC_CONVSEND, lpvcsend);
		}

		return TRUE;

	case WM_SIZE:
		RepositionConvControls(hwndDlg, (VULTURE_CONVERSATION*)GetWindowLongPtr(hwndDlg, GWLP_USERDATA));
		return TRUE;

	case WM_DESTROY:
		{
			/* Get handle to bitmap in icon control. */
			HBITMAP hbmPrev = (HBITMAP)SendDlgItemMessage(hwndDlg, IDC_STATIC_ICON, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);

			lpvconv = (VULTURE_CONVERSATION*)GetWindowLongPtr(hwndDlg, GWLP_USERDATA);
			
			/* On Windows >= XP, the static control sometimes makes
			 * a copy of the bitmap we send it, in which case we
			 * have *two* bitmaps to free now.
			 */
			if(hbmPrev != lpvconv->hbmIcon && hbmPrev)
				DeleteObject(hbmPrev);

			if(lpvconv->hbmIcon)
				DeleteObject(lpvconv->hbmIcon);
		}

		return TRUE;
	}

	return FALSE;
}



/**
 * Resizes active conversation dialogue to fit in the tab.
 *
 * @param	hwndConvContainer	Conversation container window.
 * @param	hwndTabs		Tab control.
 */
static void ResizeActiveConversationWindow(HWND hwndConvContainer, HWND hwndTabs)
{
	TCITEM tcitem;
	RECT rc;
	CONVCONTAINERDATA *lpccd = (CONVCONTAINERDATA*)GetWindowLongPtr(hwndConvContainer, GWLP_USERDATA);

	tcitem.mask = TCIF_PARAM;

	GetWindowRect(hwndTabs, &rc);
	TabCtrl_AdjustRect(hwndTabs, FALSE, &rc);
	MapWindowPoints(HWND_DESKTOP, hwndTabs, (LPPOINT)(void*)&rc, 2);

	if(lpccd->iSelectedTab >= 0)
	{
		TabCtrl_GetItem(hwndTabs, lpccd->iSelectedTab, &tcitem);
		SetWindowPos(
			((VULTURE_CONVERSATION*)tcitem.lParam)->hwndConv,
			NULL,
			rc.left,
			rc.top,
			rc.right - rc.left,
			rc.bottom - rc.top,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
}


#define CONV_TOP_MARGIN		48

/**
 * Repositions and resizes controls in a conversation window.
 *
 * @param	hwndConvDlg	Conversation window.
 * @param	lpvconv		Conversation.
 */
static void RepositionConvControls(HWND hwndConvDlg, VULTURE_CONVERSATION *lpvconv)
{
	RECT rcClient;
	HDWP hdwp = BeginDeferWindowPos(lpvconv->convtype == PURPLE_CONV_TYPE_CHAT ? 4 : 3);
	int cxLeft;

	GetClientRect(hwndConvDlg, &rcClient);

	/* Width of input and output controls. */
	cxLeft = rcClient.right - 2 * CONV_DLG_MARGIN - (lpvconv->convtype == PURPLE_CONV_TYPE_CHAT ? (CONV_DLG_MARGIN + g_cxNames) : 0);

	hdwp = DeferWindowPos(
		hdwp,
		GetDlgItem(hwndConvDlg, IDC_STATIC_ICON),
		NULL,
		CONV_DLG_MARGIN,
		CONV_DLG_MARGIN,
		CONV_TOP_MARGIN - 2 * CONV_DLG_MARGIN,
		CONV_TOP_MARGIN - 2 * CONV_DLG_MARGIN,
		SWP_NOACTIVATE | SWP_NOZORDER);

	hdwp = DeferWindowPos(
		hdwp,
		GetDlgItem(hwndConvDlg, IDC_RICHEDIT_CONV),
		NULL,
		CONV_DLG_MARGIN,
		CONV_DLG_MARGIN + CONV_TOP_MARGIN,
		cxLeft,
		rcClient.bottom - g_cyInput - 3 * CONV_DLG_MARGIN - CONV_TOP_MARGIN,
		SWP_NOACTIVATE | SWP_NOZORDER);

	hdwp = DeferWindowPos(
		hdwp,
		GetDlgItem(hwndConvDlg, IDC_RICHEDIT_INPUT),
		NULL,
		CONV_DLG_MARGIN,
		rcClient.bottom - g_cyInput - CONV_DLG_MARGIN,
		cxLeft,
		g_cyInput,
		SWP_NOACTIVATE | SWP_NOZORDER);

	if(lpvconv->convtype == PURPLE_CONV_TYPE_CHAT)
	{
		hdwp = DeferWindowPos(
			hdwp,
			GetDlgItem(hwndConvDlg, IDC_TREE_NAMES),
			NULL,
			2 * CONV_DLG_MARGIN + cxLeft,
			CONV_DLG_MARGIN + CONV_TOP_MARGIN,
			g_cxNames,
			rcClient.bottom - 2 * CONV_DLG_MARGIN - CONV_TOP_MARGIN,
			SWP_NOACTIVATE | SWP_NOZORDER);
	}

	EndDeferWindowPos(hdwp);
}


/**
 * Displays a received message in a conversation window, and handles
 * highlighting.
 *
 * @param	lpvcwrite	Conversation message data structure.
 */
void VultureWriteConversation(VULTURE_CONV_WRITE *lpvcwrite)
{
	CHARRANGE charrange;
	int cchTime;
	LPTSTR szTime;
	HWND hwndRichEdit = GetDlgItem(lpvcwrite->lpvconv->hwndConv, IDC_RICHEDIT_CONV);
	CONVCONTAINERDATA *lpccd;
	HWND hwndTabs;

	charrange.cpMin = charrange.cpMax = -1;
	SendMessage(hwndRichEdit, EM_EXSETSEL, 0, (LPARAM)&charrange);

	/* Build the timestamp. */
	cchTime = GetTimeFormat(LOCALE_USER_DEFAULT, 0, &lpvcwrite->systimeMsg, NULL, NULL, 0);
	szTime = ProcHeapAlloc(cchTime * sizeof(TCHAR));
	GetTimeFormat(LOCALE_USER_DEFAULT, 0, &lpvcwrite->systimeMsg, NULL, szTime, cchTime);
	SendMessage(hwndRichEdit, EM_REPLACESEL, 0, (LPARAM)szTime);
	ProcHeapFree(szTime);

	SendMessage(hwndRichEdit, EM_REPLACESEL, 0, (LPARAM)TEXT(" "));
	SendMessage(hwndRichEdit, EM_REPLACESEL, 0, (LPARAM)lpvcwrite->szName);
	SendMessage(hwndRichEdit, EM_REPLACESEL, 0, (LPARAM)TEXT(": "));
	SendMessage(hwndRichEdit, EM_REPLACESEL, 0, (LPARAM)lpvcwrite->szMessage);
	SendMessage(hwndRichEdit, EM_REPLACESEL, 0, (LPARAM)TEXT("\n"));

	if(GetForegroundWindow() != lpvcwrite->lpvconv->hwndContainer)
	{
#if (WINVER >= 0x0500)
		FLASHWINFO flashwinfo;

		SystemParametersInfo(SPI_GETFOREGROUNDFLASHCOUNT, 0, &flashwinfo.uCount, 0);
		flashwinfo.cbSize = sizeof(flashwinfo);
		flashwinfo.dwFlags = FLASHW_ALL;
		flashwinfo.dwTimeout = 0;
		flashwinfo.hwnd = lpvcwrite->lpvconv->hwndContainer;
		FlashWindowEx(&flashwinfo);
#else
		FlashWindow(lpvcwrite->lpvconv->hwndContainer, TRUE);
#endif
	}

	lpccd = (CONVCONTAINERDATA*)GetWindowLongPtr(lpvcwrite->lpvconv->hwndContainer, GWLP_USERDATA);
	hwndTabs = GetDlgItem(lpccd->hwndTabDlg, IDC_TAB_CONVERSATIONS);
	if(lpvcwrite->lpvconv->iTabIndex != TabCtrl_GetCurSel(hwndTabs))
		TabCtrl_HighlightItem(hwndTabs, lpvcwrite->lpvconv->iTabIndex, TRUE);
}


/**
 * Subclassing window procedure for input Rich Edit control in conversation
 * windows.
 *
 * @param	hwnd		Input box window handle.
 * @param	uiMsg		Message ID.
 * @param	wParam		Message-specific.
 * @param	lParam		Message-specific.
 *
 * @return Message-specific.
 */
static LRESULT CALLBACK InputBoxSubclassProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	VULTURE_CONVERSATION *lpvconv;
	HWND hwndParent = GetParent(hwnd);

	/* Intercept the Enter key. */
	if(uiMsg == WM_KEYDOWN && wParam == VK_RETURN && !(GetKeyState(VK_SHIFT) & 0x8000))
	{
		SendMessage(hwndParent, WM_INPUTENTER, 0, (LPARAM)hwnd);
		return 0;
	}

	lpvconv = (VULTURE_CONVERSATION*)GetWindowLongPtr(hwndParent, GWLP_USERDATA);
	return CallWindowProc(lpvconv->wndprocInputOrig, hwnd, uiMsg, wParam, lParam);
}


/**
 * Called by the core to free the data allocated when sending a message when
 * it's done with it.
 *
 * @param	lpvcsend	Data to free.
 */
void VultureFreeConvSend(VULTURE_CONV_SEND *lpvcsend)
{
	ProcHeapFree(lpvcsend->szMessage);
	ProcHeapFree(lpvcsend);
}


/**
 * Enables and shows the conversation window for the selected tab, giving the
 * focus to the input box.
 *
 * @param	lpccd	Container window data.
 */
static void EnableAppropriateConvWindow(CONVCONTAINERDATA *lpccd)
{
	TCITEM tcitem;
	HWND hwndTabs = GetDlgItem(lpccd->hwndTabDlg, IDC_TAB_CONVERSATIONS);
	HWND hwndConv;

	tcitem.mask = TCIF_PARAM;

	/* Disable old conversation. */
	if(lpccd->iSelectedTab >= 0)
	{
		TabCtrl_GetItem(hwndTabs, lpccd->iSelectedTab, &tcitem);
		hwndConv = ((VULTURE_CONVERSATION*)tcitem.lParam)->hwndConv;

		EnableWindow(hwndConv, FALSE);
		ShowWindow(hwndConv, SW_HIDE);
	}

	lpccd->iSelectedTab = TabCtrl_GetCurSel(hwndTabs);
	TabCtrl_GetItem(hwndTabs, lpccd->iSelectedTab, &tcitem);
	hwndConv = ((VULTURE_CONVERSATION*)tcitem.lParam)->hwndConv;

	EnableWindow(hwndConv, TRUE);
	SetWindowPos(hwndConv, HWND_TOP, 0, 0, 0, 0, SWP_NOREPOSITION | SWP_NOSIZE);
	ShowWindow(hwndConv, SW_SHOW);

	SetFocus(GetDlgItem(hwndConv, IDC_RICHEDIT_INPUT));
}


/**
 * Sets the title of a conversation in the UI, both of the tab and of the
 * conversation window itself.
 *
 * @param	lpvconv		Conversation.
 * @param	hwndTabs	Tab control.
 * @param	szTitle		New title.
 */
static void SetConvTitle(VULTURE_CONVERSATION *lpvconv, HWND hwndTabs, LPTSTR szTitle)
{
	TCITEM tcitem;

	tcitem.mask = TCIF_TEXT;
	tcitem.pszText = szTitle;
	TabCtrl_SetItem(hwndTabs, lpvconv->iTabIndex, &tcitem);
	SetDlgItemText(lpvconv->hwndConv, IDC_STATIC_NAME, szTitle);
}


/**
 * Refreshes the status text in an IM window.
 *
 * @param	hwndDlg		IM dialogue.
 * @param	lpvconv		Conversation.
 */
static void UpdateIMStatusText(HWND hwndDlg, VULTURE_CONVERSATION *lpvconv)
{
	VULTURE_CONV_GET_STRING vcgetstring;

	/* Set the status text. */
	vcgetstring.lpvconv = lpvconv;
	VultureSingleSyncPurpleCall(PC_IMGETSTATUSMSG, &vcgetstring);
	SetDlgItemText(hwndDlg, IDC_STATIC_STATUS, vcgetstring.sz ? vcgetstring.sz : TEXT(""));
	if(vcgetstring.sz) g_free(vcgetstring.sz);
}


/**
 * GDestroyNotify that frees a VULTURE_CHAT_USER structure and its contents.
 *
 * @param	lpvChatUser	Structure to free.
 */
static void FreeChatUser(void *lpvChatUser)
{
	VULTURE_CHAT_USER *lpvchatuser = (VULTURE_CHAT_USER*)lpvChatUser;

	free(lpvchatuser->szName);
	if(lpvchatuser->szAlias) free(lpvchatuser->szAlias);
	if(lpvchatuser->szAliasKey) free(lpvchatuser->szAliasKey);

	ProcHeapFree(lpvchatuser);
}


/**
 * Comparator for comparing two user-list entries in a chat. Intended for use
 * with TreeView_SortChildrenCB.
 *
 * @param	lParam1		Address of first entry's VULTURE_CHAT_USER.
 * @param	lParam2		Address of second entry's VULTURE_CHAT_USER.
 * @param	lParamUnused	Unused.
 *
 * @return Negative/zero/positive as the first entry should come resp. before/
 * at/after the second.
 */
static int CALLBACK UserListComparator(LPARAM lParam1, LPARAM lParam2, LPARAM lParamUnused)
{
	/* Ranks in decreasing order of seniority. */
	const int c_iRankedFlags[] =
	{
		PURPLE_CBFLAGS_FOUNDER,
		PURPLE_CBFLAGS_OP,
		PURPLE_CBFLAGS_HALFOP,
		PURPLE_CBFLAGS_VOICE
	};

	VULTURE_CHAT_USER *lpvchatuser1 = (VULTURE_CHAT_USER*)lParam1;
	VULTURE_CHAT_USER *lpvchatuser2 = (VULTURE_CHAT_USER*)lParam2;
	int iRank1 = 0, iRank2 = 0;
	int i;

	UNREFERENCED_PARAMETER(lParamUnused);

	/* Compute ranks based on flags. */
	for(i = 0; i < (int)NUM_ELEMENTS(c_iRankedFlags) && !(lpvchatuser1->pccbflags & c_iRankedFlags[i]); i++)
		iRank1++;

	for(i = 0; i < (int)NUM_ELEMENTS(c_iRankedFlags) && !(lpvchatuser2->pccbflags & c_iRankedFlags[i]); i++)
		iRank2++;

	/* If ranks are different, the lower comes first. */
	if(iRank1 != iRank2)
		return iRank1 - iRank2;

	/* If ranks are equal, compare the names. */
	return _tcscmp(lpvchatuser1->szName, lpvchatuser2->szName);
}


/**
 * Sorts the user-list in a chat.
 *
 * @param	hwndTVUsers	User-list window handle.
 */
static INLINE void SortUserList(HWND hwndTVUsers)
{
	TVSORTCB tvsortcb;

	tvsortcb.hParent = TVI_ROOT;
	tvsortcb.lParam = 0;
	tvsortcb.lpfnCompare = UserListComparator;

	TreeView_SortChildrenCB(hwndTVUsers, &tvsortcb, FALSE);
}


/**
 * Updates the icon in an IM window.
 *
 * @param	hwndDlg		IM dialogue window handle.
 * @param	lpvconv		Conversation.
 */
static void UpdateIMIcon(HWND hwndDlg, VULTURE_CONVERSATION *lpvconv)
{
	VULTURE_GET_IM_BUDDY_ICON vgetimbicon;
	HBITMAP hbmPrev;
	RECT rcIcon;

	GetClientRect(GetDlgItem(hwndDlg, IDC_STATIC_ICON), &rcIcon);
	vgetimbicon.cxMax = rcIcon.right - rcIcon.left;
	vgetimbicon.cyMax = rcIcon.bottom - rcIcon.top;

	vgetimbicon.lpvconv = lpvconv;
	VultureSingleSyncPurpleCall(PC_GETIMBUDDYICON, &vgetimbicon);

	/* Set new bitmap and retrieve handle to
	 * old one.
	 */
	hbmPrev = (HBITMAP)SendDlgItemMessage(hwndDlg, IDC_STATIC_ICON, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)vgetimbicon.hbmIcon);

	/* On Windows >= XP, the static control
	 * sometimes makes a copy of the bitmap
	 * we send it, in which case we have
	 * *two* bitmaps to free now.
	 */
	if(hbmPrev != lpvconv->hbmIcon && hbmPrev)
		DeleteObject(hbmPrev);

	if(lpvconv->hbmIcon)
		DeleteObject(lpvconv->hbmIcon);

	lpvconv->hbmIcon = vgetimbicon.hbmIcon;
}
