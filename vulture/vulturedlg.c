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
#include "purpleblist.h"


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
static void AutoEnableJoinDlgOKButton(HWND hwndDlg, GList *lpglistFields);
static INT_PTR CALLBACK AddBuddyDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static void AutoEnableBuddyDlgOKButton(HWND hwndDlg);
static void PopulateGroupsCombo(HWND hwndCBEx, GList *lpglistGroups);


/**
 * Displays the "Join Chat" dialogue.
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
 * Dialogue procedure for "Join Chat" dialogue.
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

			AutoEnableJoinDlgOKButton(hwndDlg, s_lpglistFields);
		}

		/* Let the system set the focus. */
		return TRUE;

	case WM_COMMAND:
		/* Should really make sure this comes from an edit control, but
		 * no harm done.
		 */
		if(HIWORD(wParam) == EN_CHANGE)
			AutoEnableJoinDlgOKButton(hwndDlg, s_lpglistFields);

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
			{
				UpdateJoinChatFields(hwndDlg, &s_lpglistFields, &s_iMaxShowFields);
				AutoEnableJoinDlgOKButton(hwndDlg, s_lpglistFields);
			}

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
	LPTSTR szLabelTmp = VultureUTF8ToTCHAR(szLabelUTF8);
	LPTSTR szLabel = VultureAmpersandify(szLabelTmp);
	HWND hwndLabel;
	RECT rc;
	HFONT hfont;

	g_free(szLabelTmp);

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

	ProcHeapFree(szLabel);

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


/**
 * Enables or disables the OK button in the join-chat dialogue according to the
 * values of the fields.
 *
 * @param	hwndDlg		Join-chat dialogue.
 */
static void AutoEnableJoinDlgOKButton(HWND hwndDlg, GList *lpglistFields)
{
	HWND hwndOK = GetDlgItem(hwndDlg, IDOK);
	BOOL bEnable = TRUE;

	if(lpglistFields)
	{
		for(; lpglistFields; lpglistFields = lpglistFields->next)
		{
			JOIN_DLG_FIELD *lpjdf = lpglistFields->data;

			if(lpjdf->bRequired && GetWindowTextLength(lpjdf->hwndEdit) == 0)
				bEnable = FALSE;
		}
	}
	else bEnable = FALSE;

	if(!bEnable)
	{
		/* Don't leave the focus on a disabled control. */
		if(GetFocus() == hwndOK)
			SendMessage(hwndDlg, WM_NEXTDLGCTL, 0, 0);

		EnableWindow(hwndOK, FALSE);
	}
	else
		EnableWindow(hwndOK, TRUE);
}


/**
 * Displays either the "Add Buddy" or the "Send IM" dialogue.
 *
 * @param		hwndParent	Parent window handle.
 * @param[in/out]	lpvabd		Details of buddy to add are returned
 *					here, and bIMFieldsOnly determines
 *					which flavour of dialogue to show.
 *
 * @return TRUE iff OKed.
 */
BOOL VultureAddBuddyDlg(HWND hwndParent, VULTURE_ADD_BUDDY_DATA *lpvabd)
{
	return (BOOL)DialogBoxParam(g_hInstance, MAKEINTRESOURCE(lpvabd->bIMFieldsOnly ? IDD_IMBUDDY : IDD_ADDBUDDY), hwndParent, AddBuddyDlgProc, (LPARAM)lpvabd);
}


/**
 * Dialogue procedure for "Add Buddy" dialogue.
 *
 * @param	hwndDlg		Dialogue window handle.
 * @param	uiMsg		Message ID.
 * @param	wParam		Message-specific.
 * @param	lParam		Message-specific.
 *
 * @return Usually TRUE if message processed and FALSE otherwise. There are
 * some exceptions for particular messages.
 */
static INT_PTR CALLBACK AddBuddyDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	static GList *s_lpglistAccounts = NULL;
	static GList *s_lpglistGroups = NULL;
	static VULTURE_ADD_BUDDY_DATA *s_lpvabd = NULL;

	switch(uiMsg)
	{
	case WM_INITDIALOG:
		{
			VULTURE_GET_ACCOUNTS vgetaccounts;

			/* We return stuff here. */
			s_lpvabd = (VULTURE_ADD_BUDDY_DATA*)lParam;

			/* Get online accounts. */
			vgetaccounts.bOnlineOnly = TRUE;
			VultureSingleSyncPurpleCall(PC_GETACCOUNTS, &vgetaccounts);
			s_lpglistAccounts = vgetaccounts.lpglistAccounts;

			/* Populate combo and select first item. */
			PopulateAccountsCombo(GetDlgItem(hwndDlg, IDC_CBEX_ACCOUNTS), s_lpglistAccounts);
			if(SendDlgItemMessage(hwndDlg, IDC_CBEX_ACCOUNTS, CB_GETCOUNT, 0, 0) > 0)
				SendDlgItemMessage(hwndDlg, IDC_CBEX_ACCOUNTS, CB_SETCURSEL, 0, 0);

			if(!s_lpvabd->bIMFieldsOnly)
			{
				/* Get all groups. */
				VultureSingleSyncPurpleCall(PC_GETGROUPS, &s_lpglistGroups);

				/* Populate combo and select first item. */
				PopulateGroupsCombo(GetDlgItem(hwndDlg, IDC_CBEX_GROUP), s_lpglistGroups);
				if(SendDlgItemMessage(hwndDlg, IDC_CBEX_GROUP, CB_GETCOUNT, 0, 0) > 0)
					SendDlgItemMessage(hwndDlg, IDC_CBEX_GROUP, CB_SETCURSEL, 0, 0);
			}

			AutoEnableBuddyDlgOKButton(hwndDlg);
		}

		/* Let the system set the focus. */
		return TRUE;

	case WM_COMMAND:
		/* Should really make sure this comes from an edit control, but
		 * no harm done.
		 */
		if(HIWORD(wParam) == EN_CHANGE)
			AutoEnableBuddyDlgOKButton(hwndDlg);

		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				COMBOBOXEXITEM cbexitem;
				int cch;

				/* Get the selected account. */
				cbexitem.mask = CBEIF_LPARAM;
				cbexitem.iItem = SendDlgItemMessage(hwndDlg, IDC_CBEX_ACCOUNTS, CB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwndDlg, IDC_CBEX_ACCOUNTS, CBEM_GETITEM, 0, (LPARAM)&cbexitem);
				s_lpvabd->lppac = ((VULTURE_ACCOUNT*)cbexitem.lParam)->lppac;

				/* Get username. */
				cch = GetWindowTextLength(GetDlgItem(hwndDlg, IDC_EDIT_USERNAME)) + 1;
				s_lpvabd->szUsername = ProcHeapAlloc(cch * sizeof(TCHAR));
				GetDlgItemText(hwndDlg, IDC_EDIT_USERNAME, s_lpvabd->szUsername, cch);

				if(!s_lpvabd->bIMFieldsOnly)
				{
					/* Get the selected group. */
					cbexitem.mask = CBEIF_LPARAM;
					cbexitem.iItem = SendDlgItemMessage(hwndDlg, IDC_CBEX_GROUP, CB_GETCURSEL, 0, 0);
					SendDlgItemMessage(hwndDlg, IDC_CBEX_GROUP, CBEM_GETITEM, 0, (LPARAM)&cbexitem);
					s_lpvabd->lpvblistnodeGroup = (VULTURE_BLIST_NODE*)cbexitem.lParam;

					if(s_lpvabd->lpvblistnodeGroup)
						VultureBListNodeAddRef(s_lpvabd->lpvblistnodeGroup);

					cch = GetWindowTextLength(GetDlgItem(hwndDlg, IDC_EDIT_ALIAS)) + 1;
					if(cch > 1)
					{
						s_lpvabd->szAlias = ProcHeapAlloc(cch * sizeof(TCHAR));
						GetDlgItemText(hwndDlg, IDC_EDIT_ALIAS, s_lpvabd->szAlias, cch);
					}
					else
						s_lpvabd->szAlias = NULL;
				}

				EndDialog(hwndDlg, TRUE);
			}

			return TRUE;

		case IDCANCEL:
			EndDialog(hwndDlg, FALSE);
			return TRUE;
		}

		break;

	case WM_DESTROY:
		VultureFreeAccountList(s_lpglistAccounts);
		VultureFreeGroupList(s_lpglistGroups);
		return TRUE;
	}

	return FALSE;
}


/**
 * Enables or disables the OK button in the add-buddy or IM dialogues according
 * to the states of other controls.
 *
 * @param	hwndDlg		Add-buddy dialogue.
 */
static void AutoEnableBuddyDlgOKButton(HWND hwndDlg)
{
	HWND hwndOK = GetDlgItem(hwndDlg, IDOK);
	BOOL bEnable = SendDlgItemMessage(hwndDlg, IDC_CBEX_ACCOUNTS, CB_GETCURSEL, 0, 0) >= 0 && GetWindowTextLength(GetDlgItem(hwndDlg, IDC_EDIT_USERNAME)) > 0;

	if(!bEnable)
	{
		/* Don't leave the focus on a disabled control. */
		if(GetFocus() == hwndOK)
			SendMessage(hwndDlg, WM_NEXTDLGCTL, 0, 0);

		EnableWindow(hwndOK, FALSE);
	}
	else
		EnableWindow(hwndOK, TRUE);
}


/**
 * Populates a ComboBoxEx control with groups.
 *
 * @param	hwndCBEx		ComboBoxEx control window handle.
 * @param	lpglistGroups		Accounts to add.
 */
static void PopulateGroupsCombo(HWND hwndCBEx, GList *lpglistGroups)
{
	GList *lpglistRover;
	COMBOBOXEXITEM cbexitem;

	SendMessage(hwndCBEx, CB_RESETCONTENT, 0, 0);

	cbexitem.mask = CBEIF_TEXT | CBEIF_LPARAM;
	cbexitem.iItem = -1;

	/* Add each group. */
	for(lpglistRover = lpglistGroups; lpglistRover; lpglistRover = lpglistRover->next)
	{
		VULTURE_BLIST_NODE *lpvblistnode = (VULTURE_BLIST_NODE*)lpglistRover->data;

		if(lpvblistnode->szNodeText)
		{
			cbexitem.pszText = lpvblistnode->szNodeText;
			cbexitem.lParam = (LPARAM)lpvblistnode;
			SendMessage(hwndCBEx, CBEM_INSERTITEM, 0, (LPARAM)&cbexitem);
		}
	}
}
