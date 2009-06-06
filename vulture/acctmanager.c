/*
 * Vulture - Win32 libpurple client
 *
 * acctmanager.c: Account management, including UI.
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
#include "acctmanager.h"



typedef struct _ACCMGRDATA
{
	GList	*lpglistAccounts;
} ACCMGRDATA;


static INT_PTR CALLBACK AccountManagerDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);


#define CCH_ACCMGR_HEADER	64


/**
 * Show the account manager dialogue. The list of accounts is updated to
 * reflect the user's changes, but the caller needs to inform libpurple.
 *
 * @param		hwndParent		Parent window handle.
 * @param[in/out]	lpglistAccounts		List of VULTURE_ACCOUNTs.
 *
 * @return TRUE iff the dialogue was OK-ed. The list may have been modified
 * even if FALSE is returned.
 */
BOOL VultureAccountManagerDlg(HWND hwndParent, GList *lpglistAccounts)
{
	ACCMGRDATA amd = {lpglistAccounts};
	return DialogBoxParam(g_hInstance, MAKEINTRESOURCE(IDD_ACCOUNTS), hwndParent, AccountManagerDlgProc, (LPARAM)&amd);
}


/**
 * Dialogue procedure for account manager.
 *
 * @param	hwndDlg		Dialogue window handle.
 * @param	uiMsg		Message ID.
 * @param	wParam		Message-specific.
 * @param	lParam		Message-specific.
 *
 * @return Usually TRUE if message processed and FALSE otherwise. There are
 * some exceptions for particular messages.
 */
static INT_PTR CALLBACK AccountManagerDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{	
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		{
			ACCMGRDATA *lpamd = (ACCMGRDATA*)lParam;
			GList *lpglistAccounts;
			HWND hwndList = GetDlgItem(hwndDlg, IDC_LIST_ACCOUNTS);
			LVITEM lvitem;
			LVCOLUMN lvcol;
			TCHAR szHeader[CCH_ACCMGR_HEADER];

			/* Remember the parameters. */
			SetWindowLongPtr(hwndDlg, GWLP_USERDATA, (LONG)lpamd);

			/* Prepare the columns, taking titles from the string
			 * table.
			 */

			lvcol.mask = LVCF_TEXT | LVCF_SUBITEM;
			lvcol.iSubItem = 0;
			lvcol.pszText = szHeader;

			LoadString(g_hInstance, IDS_ACCMGR_ACCOUNT, szHeader, NUM_ELEMENTS(szHeader));
			ListView_InsertColumn(hwndList, lvcol.iSubItem, &lvcol);

			lvcol.iSubItem++;
			LoadString(g_hInstance, IDS_ACCMGR_PROTOCOL, szHeader, NUM_ELEMENTS(szHeader));
			ListView_InsertColumn(hwndList, lvcol.iSubItem, &lvcol);


			/* List-view extended styles have to be set explicitly,
			 * not just read from the template.
			 */
			ListView_SetExtendedListViewStyle(hwndList, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);


			/* Populate the list. */
			for(lvitem.iItem = 0, lpglistAccounts = lpamd->lpglistAccounts;
				lpglistAccounts;
				lvitem.iItem++, lpglistAccounts = lpglistAccounts->next)
			{
				lvitem.mask = LVIF_TEXT | LVIF_PARAM;
				lvitem.iSubItem = 0;
				lvitem.pszText = ((VULTURE_ACCOUNT*)lpglistAccounts->data)->szUsername;
				lvitem.lParam = (LPARAM)lpglistAccounts->data;
				ListView_InsertItem(hwndList, &lvitem);

				lvitem.mask = LVIF_TEXT;
				lvitem.iSubItem = 1;
				lvitem.pszText = ((VULTURE_ACCOUNT*)lpglistAccounts->data)->szProtocolID;
				ListView_SetItem(hwndList, &lvitem);

				ListView_SetCheckState(hwndList, lvitem.iItem, ((VULTURE_ACCOUNT*)lpglistAccounts->data)->bEnabled);
			}

			ListView_SetColumnWidth(hwndList, 0, LVSCW_AUTOSIZE);
			ListView_SetColumnWidth(hwndList, 1, LVSCW_AUTOSIZE_USEHEADER);
		}

		/* Let the system set the focus. */
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				/* Store the checkbox states for each account. */
				HWND hwndList = GetDlgItem(hwndDlg, IDC_LIST_ACCOUNTS);
				LVITEM lvitem;
				int iCount = ListView_GetItemCount(hwndList);

				lvitem.mask = LVIF_PARAM;

				for(lvitem.iItem = 0; lvitem.iItem < iCount; lvitem.iItem++)
				{
					ListView_GetItem(hwndList, &lvitem);
					((VULTURE_ACCOUNT*)lvitem.lParam)->bEnabled = ListView_GetCheckState(hwndList, lvitem.iItem);
				}

				EndDialog(hwndDlg, TRUE);
			}

			return TRUE;

		case IDCANCEL:
			EndDialog(hwndDlg, FALSE);
			return TRUE;
		}

		break;
	}

	return FALSE;
}
