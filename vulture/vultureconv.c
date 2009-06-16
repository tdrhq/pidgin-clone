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

#include "vulture.h"
#include "resource.h"
#include "vultureconv.h"
#include "purpleconv.h"
#include "vultureblist.h"
#include "purplemain.h"
#include "purplequeue.h"


#define CONVCONTAINERCLASS	TEXT("VULTURECONVCONTAINER")
#define CONV_DLG_MARGIN		4


typedef struct _CONVCONTAINERDATA
{
	HWND	hwndTabDlg;
	int	iSelectedTab;
} CONVCONTAINERDATA;


static int g_cyInput = 48;


static LRESULT CALLBACK ConvContainerWndProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK ConvContTabDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static void RecalcTabIndices(HWND hwndTabs);
static INT_PTR CALLBACK IMDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static void ResizeActiveConversationWindow(HWND hwndConvContainer, HWND hwndTabs);
static void RepositionConvControls(HWND hwndConvDlg);
static LRESULT CALLBACK InputBoxSubclassProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static void EnableAppropriateConvWindow(CONVCONTAINERDATA *lpccd);


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
				tcitem.mask = TCIF_TEXT | TCIF_PARAM;
				tcitem.lParam = lParam;

				EnterCriticalSection(&lpvconv->sync.cs);
					tcitem.pszText = lpvconv->sync.szTitle;
					lpvconv->iTabIndex = TabCtrl_InsertItem(hwndTabs, TabCtrl_GetItemCount(hwndTabs), &tcitem);
				LeaveCriticalSection(&lpvconv->sync.cs);

				/* Create conversation dialogue. It is
				 * initially disabled and hidden.
				 */
				lpvconv->hwndConv = CreateDialogParam(g_hInstance, MAKEINTRESOURCE(IDD_IM), hwndTabs, IMDlgProc, (LPARAM)lpvconv);
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


	case WM_DESTROY:

		lpccd = (CONVCONTAINERDATA*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

		if(lpccd->hwndTabDlg) DestroyWindow(lpccd->hwndTabDlg);

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
	VULTURE_CONVERSATION *lpvconv;

	switch(uiMsg)
	{
	case WM_INITDIALOG:
		{
			HWND hwndREInput = GetDlgItem(hwndDlg, IDC_RICHEDIT_INPUT);

			/* Remember the conversation. */
			lpvconv = (VULTURE_CONVERSATION*)lParam;
			SetWindowLongPtr(hwndDlg, GWLP_USERDATA, lParam);

			/* Subclass the input box. */
			lpvconv->wndprocInputOrig = (WNDPROC)GetWindowLongPtr(hwndREInput, GWLP_WNDPROC);
			SetWindowLongPtr(hwndREInput, GWLP_WNDPROC, (LONG)InputBoxSubclassProc);
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
		RepositionConvControls(hwndDlg);
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
 */
static void RepositionConvControls(HWND hwndConvDlg)
{
	RECT rcClient;
	HDWP hdwp = BeginDeferWindowPos(2);

	GetClientRect(hwndConvDlg, &rcClient);

	hdwp = DeferWindowPos(
		hdwp,
		GetDlgItem(hwndConvDlg, IDC_RICHEDIT_CONV),
		NULL,
		CONV_DLG_MARGIN,
		CONV_DLG_MARGIN + CONV_TOP_MARGIN,
		rcClient.right - 2 * CONV_DLG_MARGIN,
		rcClient.bottom - g_cyInput - 3 * CONV_DLG_MARGIN - CONV_TOP_MARGIN,
		SWP_NOACTIVATE | SWP_NOZORDER);

	hdwp = DeferWindowPos(
		hdwp,
		GetDlgItem(hwndConvDlg, IDC_RICHEDIT_INPUT),
		NULL,
		CONV_DLG_MARGIN,
		rcClient.bottom - g_cyInput - CONV_DLG_MARGIN,
		rcClient.right - 2 * CONV_DLG_MARGIN,
		g_cyInput,
		SWP_NOACTIVATE | SWP_NOZORDER);

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
