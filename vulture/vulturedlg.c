/*
 * Vulture - Win32 libpurple client
 *
 * vulturedlg.c: Sundry dialogues.
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
#include "vulturedlg.h"
#include "resource.h"
#include "acctmanager.h"
#include "purplequeue.h"
#include "purpleacct.h"


static INT_PTR CALLBACK JoinChatDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static void PopulateAccountsCombo(HWND hwndCBEx, GList *lpglistAccounts);


/**
 * Displays the "Join a Chat" dialogue.
 *
 * @param	hwndParent	Parent window handle.
 *
 * @return Hash table containing details of chat to join, and which caller is
 * responsible for freeing; or NULL if cancelled.
 */
GHashTable* VultureJoinChatDlg(HWND hwndParent)
{
	return (GHashTable*)DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_JOINCHAT), hwndParent, JoinChatDlgProc);
}


/**
 * Dialogue procedure for "Join a Chat" dialogue.
 *
 * @param	hwndDlg		Dialogue window handle.
 * @param	uiMsg		Message ID.
 * @param	wParam		Message-specific.
 * @param	lParam		Message-specific.
 *
 * @return Usually TRUE if message processed and FALSE otherwise. There are
 * some exceptions for particular messages.
 */
static INT_PTR CALLBACK JoinChatDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	static int s_cyNonGroup = 0, s_cyButtonMargin = 0;
	static GList *s_lpglistAccounts = NULL;

	switch(uiMsg)
	{
	case WM_INITDIALOG:
		{
			VULTURE_GET_ACCOUNTS vgetaccounts;
			RECT rcGroup, rcDlg, rcButton;

			/* Get online accounts. */
			vgetaccounts.bOnlineOnly = TRUE;
			VultureSingleSyncPurpleCall(PC_GETACCOUNTS, &vgetaccounts);
			s_lpglistAccounts = vgetaccounts.lpglistAccounts;

			/* Populate combo and select first item. */
			PopulateAccountsCombo(GetDlgItem(hwndDlg, IDC_CBEX_ACCOUNTS), s_lpglistAccounts);
			if(SendDlgItemMessage(hwndDlg, IDC_CBEX_ACCOUNTS, CB_GETCOUNT, 0, 0) > 0)
				SendDlgItemMessage(hwndDlg, IDC_CBEX_ACCOUNTS, CB_SETCURSEL, 0, 0);

			/* Remember metrics. */
			GetClientRect(hwndDlg, &rcDlg);
			GetClientRect(GetDlgItem(hwndDlg, IDC_STATIC_DETAILS), &rcGroup);
			GetClientRect(GetDlgItem(hwndDlg, IDOK), &rcButton);

			s_cyNonGroup = rcDlg.bottom - rcGroup.bottom + rcGroup.top;
			s_cyButtonMargin = rcDlg.bottom - rcButton.top;
		}

		/* Let the system set the focus. */
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			/* TODO: return hash table. */
			EndDialog(hwndDlg, (INT_PTR)NULL);
			return TRUE;

		case IDCANCEL:
			EndDialog(hwndDlg, (INT_PTR)NULL);
			return TRUE;
		}

		break;

	case WM_SIZE:
		{
			RECT rcGroup, rcOK, rcCancel;
			HDWP hdwp;
			HWND hwndGroup = GetDlgItem(hwndDlg, IDC_STATIC_DETAILS);
			HWND hwndOK = GetDlgItem(hwndDlg, IDOK);
			HWND hwndCancel = GetDlgItem(hwndDlg, IDCANCEL);

			GetClientRect(hwndGroup, &rcGroup);
			GetClientRect(hwndOK, &rcOK);
			GetClientRect(hwndCancel, &rcCancel);

			/* Adjust group and buttons so that they fill the
			 * dialogue.
			 */
			hdwp = BeginDeferWindowPos(3);
			hdwp = DeferWindowPos(hdwp, hwndGroup, NULL, 0, 0, rcGroup.right - rcGroup.left, HIWORD(lParam) - s_cyNonGroup, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			hdwp = DeferWindowPos(hdwp, hwndOK, NULL, rcOK.left, HIWORD(lParam) - s_cyButtonMargin, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			hdwp = DeferWindowPos(hdwp, hwndCancel, NULL, rcCancel.left, HIWORD(lParam) - s_cyButtonMargin, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			EndDeferWindowPos(hdwp);
		}

		return TRUE;

	case WM_DESTROY:
		VultureFreeAccountList(s_lpglistAccounts);
		return TRUE;
	}

	return FALSE;
}


/**
 * Populates a ComboBoxEx control with accounts.
 *
 * @param	hwndCBEx		ComboBoxEx control window handle.
 * @param	lpglistAccounts		Accounts to add.
 */
static void PopulateAccountsCombo(HWND hwndCBEx, GList *lpglistAccounts)
{
	GList *lpglistRover;
	COMBOBOXEXITEM cbexitem;

	SendMessage(hwndCBEx, CB_RESETCONTENT, 0, 0);

	cbexitem.mask = CBEIF_TEXT | CBEIF_LPARAM;

	/* Add each account. */
	for(lpglistRover = lpglistAccounts; lpglistRover; lpglistRover = lpglistRover->next)
	{
		VULTURE_ACCOUNT *lpvac = (VULTURE_ACCOUNT*)lpglistRover->data;

		cbexitem.pszText = lpvac->szUsername;
		cbexitem.lParam = (LPARAM)lpvac;
		SendMessage(hwndCBEx, CBEM_INSERTITEM, 0, (LPARAM)&cbexitem);
	}
}
