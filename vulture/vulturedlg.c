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


typedef struct _JOIN_DLG_FIELD
{
	LPCSTR	szID;
	HWND	hwndLabel, hwndEdit;
	BOOL	bIsInt, bRequired;
} JOIN_DLG_FIELD;


/* Dialogue co-ordinates for the join-chat fields. */
#define X_JC_LABEL		15
#define X_JC_EDIT		75
#define CX_JC_LABEL		55
#define CY_JC_LABEL		10
#define CX_JC_EDIT		120
#define CY_JC_EDIT		12
#define Y_JC_LABEL_FIRST	62
#define Y_JC_EDIT_FIRST		60
#define CY_JC_INCREMENT		15

/* First ID for dynamic join-chat controls. */
#define IDC_JC_DYNAMIC		2000


static INT_PTR CALLBACK JoinChatDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static void PopulateAccountsCombo(HWND hwndCBEx, GList *lpglistAccounts);
static HWND CreateJoinDlgLabel(HWND hwndDlg, int iFieldNum, LPCSTR szLabelUTF8);
static HWND CreateJoinDlgEdit(HWND hwndDlg, int iFieldNum, BOOL bNumber, BOOL bSecret);
static void UpdateJoinChatFields(HWND hwndDlg, GList **lplpglistFields, int *lpiMaxShowFields);


/**
 * Displays the "Join a Chat" dialogue.
 *
 * @param	hwndParent	Parent window handle.
 * @param[out]	lpvjcd		Details of chat to join are returned here.
 *
 * @return TRUE iff OKed.
 */
BOOL VultureJoinChatDlg(HWND hwndParent, VULTURE_JOIN_CHAT_DATA *lpvjcd)
{
	return (BOOL)DialogBoxParam(g_hInstance, MAKEINTRESOURCE(IDD_JOINCHAT), hwndParent, JoinChatDlgProc, (LPARAM)lpvjcd);
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
	static VULTURE_JOIN_CHAT_DATA *s_lpvjcd = NULL;
	static GList *s_lpglistFields = NULL;
	static int s_iMaxShowFields;

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

			/* Remember metrics. */
			GetClientRect(hwndDlg, &rcDlg);
			GetWindowRect(GetDlgItem(hwndDlg, IDC_STATIC_DETAILS), &rcGroup);
			GetWindowRect(GetDlgItem(hwndDlg, IDOK), &rcButton);

			MapWindowPoints(HWND_DESKTOP, hwndDlg, (LPPOINT)(LPVOID)&rcGroup, 2);
			MapWindowPoints(HWND_DESKTOP, hwndDlg, (LPPOINT)(LPVOID)&rcButton, 2);

			s_cyNonGroup = rcDlg.bottom - rcGroup.bottom + rcGroup.top;
			s_cyButtonMargin = rcDlg.bottom - rcButton.top;

			/* We return stuff here. */
			s_lpvjcd = (VULTURE_JOIN_CHAT_DATA*)lParam;

			/* No fields to start with. */
			s_lpglistFields = NULL;

			/* The dialogue is initially big enough to show three
			 * fields.
			 */
			s_iMaxShowFields = 3;

			/* Populate combo and select first item. */
			PopulateAccountsCombo(GetDlgItem(hwndDlg, IDC_CBEX_ACCOUNTS), s_lpglistAccounts);
			if(SendDlgItemMessage(hwndDlg, IDC_CBEX_ACCOUNTS, CB_GETCOUNT, 0, 0) > 0)
			{
				SendDlgItemMessage(hwndDlg, IDC_CBEX_ACCOUNTS, CB_SETCURSEL, 0, 0);

				/* Show fields. */
				UpdateJoinChatFields(hwndDlg, &s_lpglistFields, &s_iMaxShowFields);
			}
		}

		/* Let the system set the focus. */
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				GList *lpglistRover;
				COMBOBOXEXITEM cbexitem;

				/* Get the selected account. */
				cbexitem.mask = CBEIF_LPARAM;
				cbexitem.iItem = SendDlgItemMessage(hwndDlg, IDC_CBEX_ACCOUNTS, CB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwndDlg, IDC_CBEX_ACCOUNTS, CBEM_GETITEM, 0, (LPARAM)&cbexitem);
				s_lpvjcd->lppac = ((VULTURE_ACCOUNT*)cbexitem.lParam)->lppac;

				/* Build hash table. */
				s_lpvjcd->lphashParameters = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

				for(lpglistRover = s_lpglistFields; lpglistRover; lpglistRover = lpglistRover->next)
				{
					JOIN_DLG_FIELD *lpjdf = lpglistRover->data;
					int cchField = GetWindowTextLength(lpjdf->hwndEdit) + 1;

					if(cchField > 1)
					{
						LPTSTR szField = ProcHeapAlloc(cchField * sizeof(TCHAR));
						gchar *szFieldUTF8;

						GetWindowText(lpjdf->hwndEdit, szField, cchField);
						szFieldUTF8 = VultureTCHARToUTF8(szField);
						ProcHeapFree(szField);

						g_hash_table_replace(s_lpvjcd->lphashParameters, g_strdup(lpjdf->szID), szFieldUTF8);
					}
				}

				EndDialog(hwndDlg, TRUE);
			}

			return TRUE;

		case IDCANCEL:
			EndDialog(hwndDlg, FALSE);
			return TRUE;

		case IDC_CBEX_ACCOUNTS:
			if(HIWORD(wParam) == CBN_SELCHANGE)
				UpdateJoinChatFields(hwndDlg, &s_lpglistFields, &s_iMaxShowFields);

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
			GetWindowRect(GetDlgItem(hwndDlg, IDC_STATIC_DETAILS), &rcGroup);
			GetWindowRect(GetDlgItem(hwndDlg, IDOK), &rcOK);
			GetWindowRect(GetDlgItem(hwndDlg, IDCANCEL), &rcCancel);

			MapWindowPoints(HWND_DESKTOP, hwndDlg, (LPPOINT)(LPVOID)&rcGroup, 2);
			MapWindowPoints(HWND_DESKTOP, hwndDlg, (LPPOINT)(LPVOID)&rcOK, 2);
			MapWindowPoints(HWND_DESKTOP, hwndDlg, (LPPOINT)(LPVOID)&rcCancel, 2);

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
		{
			GList *lpglistRover;

			VultureFreeAccountList(s_lpglistAccounts);

			for(lpglistRover = s_lpglistFields; lpglistRover; lpglistRover = lpglistRover->next)
				ProcHeapFree(lpglistRover->data);

			g_list_free(s_lpglistFields);
		}

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
	cbexitem.iItem = -1;

	/* Add each account. */
	for(lpglistRover = lpglistAccounts; lpglistRover; lpglistRover = lpglistRover->next)
	{
		VULTURE_ACCOUNT *lpvac = (VULTURE_ACCOUNT*)lpglistRover->data;

		cbexitem.pszText = lpvac->szUsername;
		cbexitem.lParam = (LPARAM)lpvac;
		SendMessage(hwndCBEx, CBEM_INSERTITEM, 0, (LPARAM)&cbexitem);
	}
}


/**
 * Creates a label for a chat parameter in the join-chat dialogue.
 *
 * @param	hwndDlg		Join-chat dialogue.
 * @param	iFieldNum	Zero-based field number.
 * @param	szLabelUTF8	Label text, in UTF-8.
 *
 * @return Window handle for label.
 */
static HWND CreateJoinDlgLabel(HWND hwndDlg, int iFieldNum, LPCSTR szLabelUTF8)
{
	LPTSTR szLabel = VultureUTF8ToTCHAR(szLabelUTF8);
	HWND hwndLabel;
	RECT rc;
	HFONT hfont;

	/* We cheat slightly and use right, bottom for width, height resp. */
	rc.left = X_JC_LABEL;
	rc.top =  Y_JC_LABEL_FIRST + iFieldNum * CY_JC_INCREMENT;
	rc.right = CX_JC_LABEL;
	rc.bottom = CY_JC_LABEL;
	MapDialogRect(hwndDlg, &rc);
	
	hwndLabel = CreateWindowEx(
		0,
		TEXT("STATIC"),
		szLabel,
		WS_CHILD | WS_VISIBLE,
		rc.left, rc.top,
		rc.right, rc.bottom,
		hwndDlg,
		(HMENU)(IDC_JC_DYNAMIC + iFieldNum * 2),
		g_hInstance,
		NULL);

	g_free(szLabel);

	/* Set the font, by querying the dialogue first. */
	hfont = (HFONT)SendMessage(hwndDlg, WM_GETFONT, 0, 0);
	SendMessage(hwndLabel, WM_SETFONT, (WPARAM)hfont, FALSE);

	return hwndLabel;
}


/**
 * Creates an edit box for a chat parameter in the join-chat dialogue.
 *
 * @param	hwndDlg		Join-chat dialogue.
 * @param	iFieldNum	Zero-based field number.
 * @param	bNumber		Whether the field should accept only numbers.
 * @param	bSecret		Whether the field is password-esque.
 *
 * @return Window handle for edit box.
 */
static HWND CreateJoinDlgEdit(HWND hwndDlg, int iFieldNum, BOOL bNumber, BOOL bSecret)
{
	HWND hwndEdit;
	RECT rc;
	HFONT hfont;

	/* We cheat slightly and use right, bottom for width, height resp. */
	rc.left = X_JC_EDIT;
	rc.top =  Y_JC_EDIT_FIRST + iFieldNum * CY_JC_INCREMENT;
	rc.right = CX_JC_EDIT;
	rc.bottom = CY_JC_EDIT;
	MapDialogRect(hwndDlg, &rc);
	
	hwndEdit = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		TEXT("EDIT"),
		TEXT(""),
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL | (bNumber ? ES_NUMBER : 0) | (bSecret ? ES_PASSWORD : 0),
		rc.left, rc.top,
		rc.right, rc.bottom,
		hwndDlg,
		(HMENU)(IDC_JC_DYNAMIC + iFieldNum * 2 + 1),
		g_hInstance,
		NULL);

	/* Set the font, by querying the dialogue first. */
	hfont = (HFONT)SendMessage(hwndDlg, WM_GETFONT, 0, 0);
	SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hfont, FALSE);

	return hwndEdit;
}


/**
 * Creates an edit box for a chat parameter in the join-chat dialogue.
 *
 * @param		hwndDlg			Join-chat dialogue.
 * @param[in,out]	lplpglistFields		Current list of fields, to be
 *						replaced with new ones.
 * @param[in,out]	lpiMaxShowFields	Room for fields in dialogue.
 */
static void UpdateJoinChatFields(HWND hwndDlg, GList **lplpglistFields, int *lpiMaxShowFields)
{
	GList *lpglistRover;
	VULTURE_GET_CHAT_FIELDS getchatfields;
	COMBOBOXEXITEM cbexitem;
	int iFieldNum;

	/* Destroy existing fields. */
	for(lpglistRover = *lplpglistFields; lpglistRover; lpglistRover = lpglistRover->next)
	{
		JOIN_DLG_FIELD *lpjdf = lpglistRover->data;
		DestroyWindow(lpjdf->hwndLabel);
		DestroyWindow(lpjdf->hwndEdit);

		ProcHeapFree(lpjdf);
	}

	g_list_free(*lplpglistFields);

	/* Get the selected account. */
	cbexitem.mask = CBEIF_LPARAM;
	cbexitem.iItem = SendDlgItemMessage(hwndDlg, IDC_CBEX_ACCOUNTS, CB_GETCURSEL, 0, 0);
	SendDlgItemMessage(hwndDlg, IDC_CBEX_ACCOUNTS, CBEM_GETITEM, 0, (LPARAM)&cbexitem);

	/* Find out what we need to specify for chats
	 * on this account.
	 */
	getchatfields.lpvac = (VULTURE_ACCOUNT*)cbexitem.lParam;
	VultureSingleSyncPurpleCall(PC_GETCHATFIELDS, &getchatfields);
	
	*lplpglistFields = NULL;

	/* Create new fields. */
	for(lpglistRover = getchatfields.lpglistFields, iFieldNum = 0;
		lpglistRover;
		lpglistRover = lpglistRover->next, iFieldNum++)
	{
		struct proto_chat_entry *lppce = lpglistRover->data;
		JOIN_DLG_FIELD *lpjdf = ProcHeapAlloc(sizeof(JOIN_DLG_FIELD));

		lpjdf->bIsInt = lppce->is_int;
		lpjdf->bRequired = lppce->required;
		lpjdf->szID = lppce->identifier;

		lpjdf->hwndLabel = CreateJoinDlgLabel(hwndDlg, iFieldNum, lppce->label);
		lpjdf->hwndEdit = CreateJoinDlgEdit(hwndDlg, iFieldNum, lppce->is_int && (lppce->min >= 0), lppce->secret);

		*lplpglistFields = g_list_prepend(*lplpglistFields, lpjdf);
	}

	if(iFieldNum > *lpiMaxShowFields)
	{
		RECT rcAdjust, rcCurrent;

		/* How much extra height? */
		SetRect(&rcAdjust, 0, 0, 0, (iFieldNum - *lpiMaxShowFields) * CY_JC_INCREMENT);
		MapDialogRect(hwndDlg, &rcAdjust);

		/* Get current dimensions and adjust. */
		GetWindowRect(hwndDlg, &rcCurrent);
		rcCurrent.bottom += rcAdjust.bottom;

		SetWindowPos(hwndDlg, NULL, 0, 0, rcCurrent.right - rcCurrent.left, rcCurrent.bottom - rcCurrent.top, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

		*lpiMaxShowFields = iFieldNum;
	}

	*lplpglistFields = g_list_reverse(*lplpglistFields);

	g_list_free(getchatfields.lpglistFields);
}
