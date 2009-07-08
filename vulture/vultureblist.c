/*
 * Vulture - Win32 libpurple client
 *
 * vultureblist.c: Buddy list.
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
#include <glib.h>
#include <tchar.h>
#include <string.h>

#include "vulture.h"
#include "resource.h"
#include "purple.h"
#include "vultureblist.h"
#include "purpleblist.h"
#include "purplequeue.h"
#include "purplestatus.h"
#include "acctmanager.h"
#include "purpleacct.h"
#include "purplemain.h"
#include "vultureconv.h"
#include "purpleconv.h"
#include "vulturedlg.h"


typedef struct _STATUSDLGDATA
{
	WNDPROC	wndprocStatusMsgOrig;
} STATUSDLGDATA;


static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK StatusDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK BuddyListDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static void ManageAccounts(HWND hwndParent);
static void PopulateStatusList(HWND hwndComboStatus);
static void UpdateStatusUI(VULTURE_SAVED_STATUS *lpvss, HWND hwndStatusDlg);
static LRESULT CALLBACK StatusMsgBoxSubclassProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static void SetStatusMsg(HWND hwndStatusDlg);
static void RemoveBListNode(HWND hwndBlistTree, VULTURE_BLIST_NODE *lpvbn);


#define BLIST_MARGIN 6


HWND g_hwndMain = NULL;
GList *g_lpglistConvContainers = NULL;

static GList *g_lpglistStatuses = NULL;
static HWND g_hwndStatusDlg = NULL, g_hwndBListDlg = NULL;



/**
 * Creates the main window, incorporating the buddy list.
 *
 * @param	iCmdShow	Initial window state. Cf. ShowWindow.
 *
 * @return Zero on success; non-zero on error.
 */
int VultureCreateMainWindow(int iCmdShow)
{
	const TCHAR c_szClassName[] = TEXT("VULTUREMAIN");

	WNDCLASSEX wndclassex;

	if(g_hwndMain)
		return 1;

	wndclassex.cbClsExtra = 0;
	wndclassex.cbSize = sizeof(wndclassex);
	wndclassex.cbWndExtra = 0;
	wndclassex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclassex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclassex.hIconSm = LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR | LR_SHARED);
	wndclassex.hInstance = g_hInstance;
	wndclassex.lpfnWndProc = MainWndProc;
	wndclassex.lpszClassName = c_szClassName;
	wndclassex.lpszMenuName = MAKEINTRESOURCE(IDM_BLIST);
	wndclassex.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;

	if(!RegisterClassEx(&wndclassex))
		return 2;

	g_hwndMain = CreateWindowEx(
		WS_EX_WINDOWEDGE,
		c_szClassName,
		cg_szAppName,
		WS_OVERLAPPEDWINDOW,

		/* TODO: Read geometry from preferences. */
		CW_USEDEFAULT, CW_USEDEFAULT,
		280, 500,

		NULL, 
		NULL,
		g_hInstance,
		NULL);

	if(!g_hwndMain)
		return 3;

	ShowWindow(g_hwndMain, iCmdShow);

	return 0;
}


/**
 * Main window procedure.
 *
 * @param	hwnd	Buddy-list handle.
 * @param	uiMsg	Message ID.
 * @param	wParam	Message-specific.
 * @param	lParam	Message-specific.
 *
 * @return Message-specific.
 */
static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	static HCURSOR s_hCursor;

	switch(uiMsg)
	{
	case WM_CREATE:
		{
			int i, iCount;
			HMENU hmenu = GetMenu(hwnd);

			g_hwndStatusDlg = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_STATUS), hwnd, StatusDlgProc);
			g_hwndBListDlg = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_BLIST), hwnd, BuddyListDlgProc);

			EnableWindow(g_hwndStatusDlg, FALSE);
			EnableWindow(g_hwndBListDlg, FALSE);

			s_hCursor = LoadCursor(NULL, IDC_WAIT);

			iCount = GetMenuItemCount(hmenu);
			for(i = 0; i < iCount; i++)
				EnableMenuItem(hmenu, i, MF_BYPOSITION | MF_DISABLED);
			DrawMenuBar(hwnd);
		}

		return 0;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDM_BLIST_BUDDIES_JOINCHAT:
			{
				VULTURE_JOIN_CHAT_DATA vjcd;

				if(VultureJoinChatDlg(hwnd, &vjcd))
					VultureSingleSyncPurpleCall(PC_JOINCHAT, &vjcd);
			}

			return 0;

		case IDM_BLIST_BUDDIES_CLOSE:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;

		case IDM_BLIST_ACCOUNTS_MANAGE:
			ManageAccounts(hwnd);
			return 0;
		}

		break;

	case WM_SIZE:
		{
			HDWP hdwp;
			RECT rcClient, rcStatus;

			GetClientRect(hwnd, &rcClient);
			GetWindowRect(g_hwndStatusDlg, &rcStatus);

			hdwp = BeginDeferWindowPos(2);
			hdwp = DeferWindowPos(hdwp, g_hwndStatusDlg, NULL, 0, 0, rcClient.right, rcStatus.bottom - rcStatus.top, SWP_NOACTIVATE | SWP_NOZORDER);
			rcClient.top = rcStatus.bottom - rcStatus.top;
			hdwp = DeferWindowPos(hdwp, g_hwndBListDlg, NULL, 0, rcClient.top, rcClient.right, rcClient.bottom - rcClient.top, SWP_NOACTIVATE | SWP_NOZORDER);
			EndDeferWindowPos(hdwp);
		}

		return 0;

	case WM_ERASEBKGND:
		/* Don't erase background: we never see it. Reduces flicker. */
		return 0;

	case WM_SETCURSOR:
		if(s_hCursor && LOWORD(lParam) == HTCLIENT)
		{
			SetCursor(s_hCursor);

			/* Stop processing. */
			return TRUE;
		}

		break;

	case WM_PURPLEUIMSG:
		{
			HWND hwndForward = NULL;

			switch(wParam)
			{
			case VUIMSG_PURPLEINITCOMPLETE:
				{
					int i, iCount;
					HMENU hmenu = GetMenu(hwnd);

					PopulateStatusList(GetDlgItem(g_hwndStatusDlg, IDC_CBEX_STATUS));

					EnableWindow(g_hwndStatusDlg, TRUE);
					EnableWindow(g_hwndBListDlg, TRUE);

					iCount = GetMenuItemCount(hmenu);
					for(i = 0; i < iCount; i++)
						EnableMenuItem(hmenu, i, MF_BYPOSITION | MF_ENABLED);
					DrawMenuBar(hwnd);

					s_hCursor = NULL;
				}

				break;

			case VUIMSG_UPDATEBLISTNODE:
				{
					HWND hwndBlistTree = GetDlgItem(g_hwndBListDlg, IDC_TREE_BLIST);
					VULTURE_BLIST_NODE *lpvbn = (VULTURE_BLIST_NODE*)lParam;

					EnterCriticalSection(&lpvbn->cs);
					{
						TVITEM tvitem;

						if(lpvbn->hti)
						{
							HTREEITEM htiParent = TreeView_GetParent(hwndBlistTree, lpvbn->hti);

							/* If the parent doesn't match, we need
							 * to recreate.
							 */
							if((lpvbn->lpvbnParent && lpvbn->lpvbnParent->hti != htiParent) ||
								(!lpvbn->lpvbnParent && htiParent))
							{
								RemoveBListNode(hwndBlistTree, lpvbn);
							}
						}


						/* New node? */
						if(!lpvbn->hti)
						{
							TVINSERTSTRUCT tvis;

							/* We cache this in the tree-view. */
							VultureBListNodeAddRef(lpvbn);

							tvis.hParent = lpvbn->lpvbnParent ? lpvbn->lpvbnParent->hti : TVI_ROOT;
							tvis.hInsertAfter = TVI_SORT;
							tvis.itemex.mask = TVIF_PARAM;
							tvis.itemex.lParam = (LPARAM)lpvbn;

							lpvbn->hti = TreeView_InsertItem(hwndBlistTree, &tvis);
						}

						/* Set text. */
						tvitem.mask = TVIF_TEXT | TVIF_HANDLE;
						tvitem.hItem = lpvbn->hti;
						tvitem.pszText = lpvbn->szNodeText;
						TreeView_SetItem(hwndBlistTree, &tvitem);
					}
					LeaveCriticalSection(&lpvbn->cs);

					/* Release the reference for this call. */
					VultureBListNodeRelease(lpvbn);
				}

				break;

			case VUIMSG_REMOVEBLISTNODE:
				RemoveBListNode(GetDlgItem(g_hwndBListDlg, IDC_TREE_BLIST), (VULTURE_BLIST_NODE*)lParam);
				break;

			case VUIMSG_NEWCONVERSATION:
				if(!g_lpglistConvContainers)
					g_lpglistConvContainers = g_list_prepend(g_lpglistConvContainers, VultureCreateConvContainer());

				/* Fall through. */
				
			case VUIMSG_DESTROYEDCONVERSATION:
				/* Forward the message to the first container. */
				hwndForward = (HWND)g_lpglistConvContainers->data;
				break;

			case VUIMSG_WRITECONVERSATION:
				VultureWriteConversation((VULTURE_CONV_WRITE*)lParam);
				VultureFreeConvWrite((VULTURE_CONV_WRITE*)lParam);
				break;

			case VUIMSG_STATUSCHANGED:
				UpdateStatusUI((VULTURE_SAVED_STATUS*)lParam, g_hwndStatusDlg);
				VultureFreeStatus((VULTURE_SAVED_STATUS*)lParam);
				break;

			case VUIMSG_CONVCHANGED:
				hwndForward = ((VULTURE_CONV_CHANGED*)lParam)->lpvconv->hwndContainer;
				break;

			case VUIMSG_UPDATEIMSTATUSTEXT:
				hwndForward = ((VULTURE_CONVERSATION*)lParam)->hwndConv;
				break;

			case VUIMSG_QUIT:
				DestroyWindow(hwnd);
				break;
			}

			/* If we specified a window, forward the message. */
			if(hwndForward)
				SendMessage(hwndForward, uiMsg, wParam, lParam);
		}

		return 0;


	case WM_CLOSE:
		{
			GList *lpglistRover;

			for(lpglistRover = g_lpglistConvContainers; lpglistRover; lpglistRover = lpglistRover->next)
				SendMessage((HWND)lpglistRover->data, WM_CLOSE, 0, 0);

			VultureEnqueueAsyncPurpleCall(PC_QUIT, NULL);

			/* Ignore the user for the rest of our life. */
			EnableWindow(hwnd, FALSE);
		}

		/* Don't destroy the window. The core will signal this thread
		 * to do so later.
		 */
		return 0;


	case WM_DESTROY:
		
		DestroyWindow(g_hwndBListDlg);
		DestroyWindow(g_hwndStatusDlg);

		if(g_lpglistConvContainers)
			g_list_free(g_lpglistConvContainers);

		PostQuitMessage(VEC_SUCCESS);

		return 0;
	}

	return DefWindowProc(hwnd, uiMsg, wParam, lParam);
}


/**
 * Dialogue procedure for status pane.
 *
 * @param	hwndDlg		Dialogue window handle.
 * @param	uiMsg		Message ID.
 * @param	wParam		Message-specific.
 * @param	lParam		Message-specific.
 *
 * @return Usually TRUE if message processed and FALSE otherwise. There are
 * some exceptions for particular messages.
 */
static INT_PTR CALLBACK StatusDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		{
			RECT rcIcon;
			POINT ptIcon;
			HWND hwndStatusMsg = GetDlgItem(hwndDlg, IDC_EDIT_STATUSMSG);
			STATUSDLGDATA *lpsdd;

			GetWindowRect(GetDlgItem(hwndDlg, IDC_BUDDY_ICON), &rcIcon);
			ptIcon.x = rcIcon.left;
			ptIcon.y = rcIcon.top;
			ScreenToClient(hwndDlg, &ptIcon);

			/* Move BLIST_MARGIN from left edge, and set width to
			 * height.
			 */
			SetWindowPos(GetDlgItem(hwndDlg, IDC_BUDDY_ICON), NULL, BLIST_MARGIN, ptIcon.y, rcIcon.bottom - rcIcon.top, rcIcon.bottom - rcIcon.top, SWP_NOACTIVATE | SWP_NOZORDER);

			/* Subclass status message box. */
			lpsdd = ProcHeapAlloc(sizeof(STATUSDLGDATA));
			lpsdd->wndprocStatusMsgOrig = (WNDPROC)GetWindowLongPtr(hwndStatusMsg, GWLP_WNDPROC);
			SetWindowLongPtr(hwndDlg, GWLP_USERDATA, (LONG)lpsdd);
			SetWindowLongPtr(hwndStatusMsg, GWLP_WNDPROC, (LONG)StatusMsgBoxSubclassProc);
		}

		/* Let the system set the focus. */
		return TRUE;

	case WM_SIZE:
		{
			HDWP hdwp;
			RECT rcClient, rcCombo, rcEdit;
			HWND hwndCombo = GetDlgItem(hwndDlg, IDC_CBEX_STATUS);
			HWND hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT_STATUSMSG);
			POINT ptCombo;
			int cxNew;

			GetClientRect(hwndDlg, &rcClient);

			/* Resize status combo and edit boxes. */

			GetWindowRect(hwndCombo, &rcCombo);
			GetWindowRect(hwndEdit, &rcEdit);

			ptCombo.x = rcCombo.left;
			ptCombo.y = rcCombo.top;
			ScreenToClient(hwndDlg, &ptCombo);

			/* Width is same for both. */
			cxNew = rcClient.right - BLIST_MARGIN - ptCombo.x;

			hdwp = BeginDeferWindowPos(2);
			hdwp = DeferWindowPos(hdwp, hwndCombo, NULL, 0, 0, cxNew, rcCombo.bottom - rcCombo.top, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
			hdwp = DeferWindowPos(hdwp, hwndEdit, NULL, 0, 0, cxNew, rcEdit.bottom - rcEdit.top, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
			EndDeferWindowPos(hdwp);
		}

		return TRUE;


	case WM_COMMAND:

		switch(LOWORD(wParam))
		{
		case IDC_CBEX_STATUS:
			if(HIWORD(wParam) == CBN_SELCHANGE)
			{
				/* Inform libpurple of the change in status. */

				int iSel = SendDlgItemMessage(hwndDlg, IDC_CBEX_STATUS, CB_GETCURSEL, 0, 0);

				if(iSel >= 0)
				{
					VULTURE_SAVED_STATUS *lpvssList = (VULTURE_SAVED_STATUS*)SendDlgItemMessage(hwndDlg, IDC_CBEX_STATUS, CB_GETITEMDATA, iSel, 0);
					VultureSingleSyncPurpleCall(PC_SETSAVEDSTATUS, lpvssList);
				}

				return TRUE;
			}

			break;

		case IDC_EDIT_STATUSMSG:
			if(HIWORD(wParam) == EN_KILLFOCUS)
			{
				SetStatusMsg(hwndDlg);
				return TRUE;
			}

			break;
		}

		break;


	case WM_INPUTENTER:
		SetStatusMsg(hwndDlg);
		SendMessage(g_hwndBListDlg, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(g_hwndBListDlg, IDC_TREE_BLIST), MAKELPARAM(TRUE, 0));
		return TRUE;


	case WM_NCDESTROY:

		if(g_lpglistStatuses)
			VulturePurpleFreeStatusList(g_lpglistStatuses);

		ProcHeapFree((STATUSDLGDATA*)GetWindowLongPtr(hwndDlg, GWLP_USERDATA));

		return TRUE;
	}

	return FALSE;
}


/**
 * Dialogue procedure for buddy list.
 *
 * @param	hwndDlg		Dialogue window handle.
 * @param	uiMsg		Message ID.
 * @param	wParam		Message-specific.
 * @param	lParam		Message-specific.
 *
 * @return Usually TRUE if message processed and FALSE otherwise. There are
 * some exceptions for particular messages.
 */
static INT_PTR CALLBACK BuddyListDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		/* Let the system set the focus. */
		return TRUE;

	case WM_SIZE:
		{
			RECT rcClient;

			GetClientRect(hwndDlg, &rcClient);
			SetWindowPos(GetDlgItem(hwndDlg, IDC_TREE_BLIST), NULL, BLIST_MARGIN, 0, rcClient.right - 2 * BLIST_MARGIN, rcClient.bottom - BLIST_MARGIN, SWP_NOACTIVATE | SWP_NOZORDER);
		}

		return TRUE;

	case WM_NOTIFY:
		{
			LPNMHDR lpnmhdr = (LPNMHDR)lParam;

			if(lpnmhdr->idFrom == IDC_TREE_BLIST)
			{
				switch(lpnmhdr->code)
				{
				case NM_RETURN:
				case NM_DBLCLK:
					{
						TVITEM tvitem;

						/* We double-clicked or pressed
						 * enter. If we have children,
						 * don't do anything special.
						 */
						if((tvitem.hItem = TreeView_GetSelection(lpnmhdr->hwndFrom)) &&
							!TreeView_GetChild(lpnmhdr->hwndFrom, tvitem.hItem))
						{
							tvitem.mask = TVIF_PARAM;
							TreeView_GetItem(lpnmhdr->hwndFrom, &tvitem);
							
							VultureEnqueueAsyncPurpleCall(PC_BLISTNODEDBLCLKED, (VULTURE_BLIST_NODE*)tvitem.lParam);

							return TRUE;
						}
					}

					break;
				}
			}
		}

		break;
	}

	return FALSE;
}


/**
 * Shows the account manager dialogue, and updates accounts if user OKs.
 *
 * @param	hwndParent	Parent window handle.
 */
static void ManageAccounts(HWND hwndParent)
{
	VULTURE_GET_ACCOUNTS vgetaccounts;

	vgetaccounts.bOnlineOnly = FALSE;
	VultureSingleSyncPurpleCall(PC_GETACCOUNTS, &vgetaccounts);

	/* Show the dialogue and check whether the user OKs. */
	if(VultureAccountManagerDlg(hwndParent, vgetaccounts.lpglistAccounts))
	{
		GList *lpglistRover;
		GArray *lpgarrayWaitContext = VultureAllocPurpleWaitContext();

		for(lpglistRover = vgetaccounts.lpglistAccounts; lpglistRover; lpglistRover = lpglistRover->next)
			VultureEnqueueMultiSyncPurpleCall(PC_UPDATEPURPLEACCOUNT, lpglistRover->data, lpgarrayWaitContext);

		VulturePurpleWait(lpgarrayWaitContext);		
	}

	VultureFreeAccountList(vgetaccounts.lpglistAccounts);
}


/**
 * Populates a combo-box with all saved statuses.
 *
 * @param	hwndComboStatus		Combo-box handle.
 */
static void PopulateStatusList(HWND hwndComboStatus)
{
	GList *lpglistStatusRover;
	GArray *lpgarrayWaitContext;

	if(g_lpglistStatuses)
		VulturePurpleFreeStatusList(g_lpglistStatuses);

	SendMessage(hwndComboStatus, CB_RESETCONTENT, 0, 0);

	/* Get all statuses. */
	lpgarrayWaitContext = VultureAllocPurpleWaitContext();
	VultureEnqueueMultiSyncPurpleCall(PC_GETALLSAVEDSTATUSES, (void*)&g_lpglistStatuses, lpgarrayWaitContext);
	VulturePurpleWait(lpgarrayWaitContext);

	/* Populate control. */
	for(lpglistStatusRover = g_lpglistStatuses; lpglistStatusRover; lpglistStatusRover = lpglistStatusRover->next)
	{
		VULTURE_SAVED_STATUS *lpvss = lpglistStatusRover->data;
		COMBOBOXEXITEM cbexitem;

		cbexitem.mask = CBEIF_TEXT | CBEIF_LPARAM;
		cbexitem.pszText = lpvss->szTitle ? lpvss->szTitle : TEXT("");
		cbexitem.lParam = (LPARAM)lpvss;

		/* Add at end of list. */
		cbexitem.iItem = -1;

		SendMessage(hwndComboStatus, CBEM_INSERTITEM, 0, (LPARAM)&cbexitem);
	}
}


/**
 * Updates the status combo-box and edit-box to reflect the given status.
 *
 * @param	lpvss		Status.
 * @param	hwndStatusDlg	Status dialogue handle.
 */
static void UpdateStatusUI(VULTURE_SAVED_STATUS *lpvss, HWND hwndStatusDlg)
{
	HWND hwndCombo = GetDlgItem(hwndStatusDlg, IDC_CBEX_STATUS);
	int i;
	int iCount = SendMessage(hwndCombo, CB_GETCOUNT, 0, 0);
	int iMatch = -1;

	PopulateStatusList(hwndCombo);

	if(lpvss->vsstype == VSSTYPE_FIRM)
	{
		/* Search for matching PurpleSavedStatus. */
		for(i = 0; i < iCount && iMatch < 0; i++)
		{
			VULTURE_SAVED_STATUS *lpvssList = (VULTURE_SAVED_STATUS*)SendMessage(hwndCombo, CB_GETITEMDATA, i, 0);

			if(lpvssList->vsstype == VSSTYPE_FIRM && lpvssList->lppss == lpvss->lppss)
				iMatch = i;
		}
	}

	/* Search for matching primitive. */
	for(i = 0; i < iCount && iMatch < 0; i++)
	{
		VULTURE_SAVED_STATUS *lpvssList = (VULTURE_SAVED_STATUS*)SendMessage(hwndCombo, CB_GETITEMDATA, i, 0);

		if(lpvssList->vsstype == VSSTYPE_PRIMITIVE && lpvssList->psprim == lpvss->psprim)
			iMatch = i;
	}

	if(iMatch >= 0)
	{
		SendMessage(hwndCombo, CB_SETCURSEL, iMatch, 0);
		SetDlgItemText(hwndStatusDlg, IDC_EDIT_STATUSMSG, lpvss->szMessage);
	}
}


/**
 * Subclassing window procedure for status message edit control.
 *
 * @param	hwnd		Input box window handle.
 * @param	uiMsg		Message ID.
 * @param	wParam		Message-specific.
 * @param	lParam		Message-specific.
 *
 * @return Message-specific.
 */
static LRESULT CALLBACK StatusMsgBoxSubclassProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	STATUSDLGDATA *lpsdd;
	HWND hwndParent = GetParent(hwnd);

	/* Intercept the Enter key. */
	if(uiMsg == WM_KEYDOWN && wParam == VK_RETURN)
	{
		SendMessage(hwndParent, WM_INPUTENTER, 0, (LPARAM)hwnd);
		return 0;
	}
	/* Don't beep. */
	else if(uiMsg == WM_CHAR && wParam == VK_RETURN)
		return 0;

	lpsdd = (STATUSDLGDATA*)GetWindowLongPtr(hwndParent, GWLP_USERDATA);
	return CallWindowProc(lpsdd->wndprocStatusMsgOrig, hwnd, uiMsg, wParam, lParam);
}


/**
 * Sets the current status message using the values of the UI controls.
 *
 * @param	hwndStatusDlg	Status dialogue window handle.
 */
static void SetStatusMsg(HWND hwndStatusDlg)
{
	HWND hwndEdit = GetDlgItem(hwndStatusDlg, IDC_EDIT_STATUSMSG);
	VULTURE_SAVED_STATUS vss;
	LPTSTR szOldMessage;

	int cchStatus = GetWindowTextLength(hwndEdit) + 1;

	if(cchStatus > 1)
	{
		vss.szMessage = ProcHeapAlloc(cchStatus * sizeof(TCHAR));
		GetWindowText(hwndEdit, vss.szMessage, cchStatus);
	}
	else vss.szMessage =  NULL;

	VultureSingleSyncPurpleCall(PC_GETSTATUSMSG, &szOldMessage);

	/* Only do anything if the message has actually changed. */
	if((vss.szMessage && !szOldMessage) ||
		(!vss.szMessage && szOldMessage) ||
		(vss.szMessage && szOldMessage && _tcscmp(vss.szMessage, szOldMessage) != 0))
	{
		int iSel = SendDlgItemMessage(hwndStatusDlg, IDC_CBEX_STATUS, CB_GETCURSEL, 0, 0);

		if(iSel >= 0)
		{
			VULTURE_SAVED_STATUS *lpvssList = (VULTURE_SAVED_STATUS*)SendDlgItemMessage(hwndStatusDlg, IDC_CBEX_STATUS, CB_GETITEMDATA, iSel, 0);

			vss.psprim = lpvssList->psprim;
			vss.szTitle = lpvssList->szTitle;
			vss.vsstype = VSSTYPE_TRANSIENT;

			VultureSingleSyncPurpleCall(PC_SETSAVEDSTATUS, &vss);
		}
	}

	if(szOldMessage) g_free(szOldMessage);
	if(vss.szMessage) ProcHeapFree(vss.szMessage);
}


/**
 * Removes a buddy-list node, doing the requisite housekeeping.
 *
 * @param	hwndStatusDlg	Buddy-list tree-view handle.
 * @param	lpvbn		Node to delete.
 */
static void RemoveBListNode(HWND hwndBlistTree, VULTURE_BLIST_NODE *lpvbn)
{
	TreeView_DeleteItem(hwndBlistTree, lpvbn->hti);
	lpvbn->hti = NULL;

	/* Release the reference belonging to the pointer
	 * cached in the tree-item.
	 */
	VultureBListNodeRelease(lpvbn);
}
