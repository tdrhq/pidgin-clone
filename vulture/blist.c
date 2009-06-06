/*
 * Vulture - Win32 libpurple client
 *
 * blist.c: Buddy list.
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

#include "vulture.h"
#include "resource.h"
#include "blist.h"
#include "purplequeue.h"
#include "purplestatus.h"
#include "acctmanager.h"
#include "purpleacct.h"


static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK StatusDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK BuddyListDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static void ManageAccounts(HWND hwndParent);


#define BLIST_MARGIN 6


HWND g_hwndMain = NULL;



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
	static HWND s_hwndStatusDlg = NULL, s_hwndBListDlg = NULL;

	switch(uiMsg)
	{
	case WM_CREATE:

		s_hwndStatusDlg = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_STATUS), hwnd, StatusDlgProc);
		s_hwndBListDlg = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_BLIST), hwnd, BuddyListDlgProc);

		return 0;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
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
			GetWindowRect(s_hwndStatusDlg, &rcStatus);

			hdwp = BeginDeferWindowPos(2);
			hdwp = DeferWindowPos(hdwp, s_hwndStatusDlg, NULL, 0, 0, rcClient.right, rcStatus.bottom - rcStatus.top, SWP_NOACTIVATE | SWP_NOZORDER);
			rcClient.top = rcStatus.bottom - rcStatus.top;
			hdwp = DeferWindowPos(hdwp, s_hwndBListDlg, NULL, 0, rcClient.top, rcClient.right, rcClient.bottom - rcClient.top, SWP_NOACTIVATE | SWP_NOZORDER);
			EndDeferWindowPos(hdwp);
		}

		return 0;

	case WM_ERASEBKGND:
		/* Don't erase background: we never see it. Reduces flicker. */
		return 0;

	case WM_DESTROY:
		DestroyWindow(s_hwndBListDlg);
		DestroyWindow(s_hwndStatusDlg);
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
	static GList *s_lpglistStatuses = NULL;

	switch(uiMsg)
	{
	case WM_INITDIALOG:
		{
			RECT rcIcon;
			POINT ptIcon;
			GList *lpglistStatusRover;
			GArray *lpgarrayWaitContext;
			HWND hwndComboStatus = GetDlgItem(hwndDlg, IDC_CBEX_STATUS);

			GetWindowRect(GetDlgItem(hwndDlg, IDC_BUDDY_ICON), &rcIcon);
			ptIcon.x = rcIcon.left;
			ptIcon.y = rcIcon.top;
			ScreenToClient(hwndDlg, &ptIcon);

			/* Move BLIST_MARGIN from left edge, and set width to
			 * height.
			 */
			SetWindowPos(GetDlgItem(hwndDlg, IDC_BUDDY_ICON), NULL, BLIST_MARGIN, ptIcon.y, rcIcon.bottom - rcIcon.top, rcIcon.bottom - rcIcon.top, SWP_NOACTIVATE | SWP_NOZORDER);

			/* Populate list of statuses. */
			lpgarrayWaitContext = VultureAllocPurpleWaitContext();
			VultureEnqueueMultiSyncPurpleCall(PC_GETALLSAVEDSTATUSES, (void*)&s_lpglistStatuses, lpgarrayWaitContext);
			VulturePurpleWait(lpgarrayWaitContext);

			for(lpglistStatusRover = s_lpglistStatuses; lpglistStatusRover; lpglistStatusRover = lpglistStatusRover->next)
			{
				VULTURE_SAVED_STATUS *lpvss = lpglistStatusRover->data;
				COMBOBOXEXITEM cbexitem;

				cbexitem.mask = CBEIF_TEXT | CBEIF_LPARAM;
				cbexitem.pszText = lpvss->szTitle;
				cbexitem.lParam = (LPARAM)lpvss;

				/* Add at end of list. */
				cbexitem.iItem = -1;

				SendMessage(hwndComboStatus, CBEM_INSERTITEM, 0, (LPARAM)&cbexitem);
			}
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

	case WM_DESTROY:
		if(s_lpglistStatuses)
				VulturePurpleFreeStatusList(s_lpglistStatuses);
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
	GList *lpglistAccounts;

	VultureSingleSyncPurpleCall(PC_GETALLACCOUNTS, &lpglistAccounts);

	/* Show the dialogue and check whether the user OKs. */
	if(VultureAccountManagerDlg(hwndParent, lpglistAccounts))
	{
		GList *lpglistRover;
		GArray *lpgarrayWaitContext = VultureAllocPurpleWaitContext();

		for(lpglistRover = lpglistAccounts; lpglistRover; lpglistRover = lpglistRover->next)
			VultureEnqueueMultiSyncPurpleCall(PC_UPDATEPURPLEACCOUNT, lpglistRover->data, lpgarrayWaitContext);

		VulturePurpleWait(lpgarrayWaitContext);		
	}

	VultureFreeAccountList(lpglistAccounts);
}
