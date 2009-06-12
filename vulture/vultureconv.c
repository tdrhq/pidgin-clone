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
#include <glib.h>

#include "vulture.h"
#include "resource.h"
#include "vultureconv.h"
#include "purpleconv.h"
#include "vultureblist.h"
#include "purplemain.h"
#include "purplequeue.h"


#define CONVCONTAINERCLASS	TEXT("VULTURECONVCONTAINER")


typedef struct _CONVCONTAINERDATA
{
	HWND	hwndTabDlg;
} CONVCONTAINERDATA;


static LRESULT CALLBACK ConvContainerWndProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK ConvContTabDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
static void RecalcTabIndices(HWND hwndTabs);


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
		{
			RECT rcClient;

			lpccd = (CONVCONTAINERDATA*)ProcHeapAlloc(sizeof(CONVCONTAINERDATA));
			SetWindowLongPtr(hwnd, GWL_USERDATA, (LONG_PTR)lpccd);

			/* Create the tab control. */
			GetClientRect(hwnd, &rcClient);
			lpccd->hwndTabDlg = CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_CONVCONT), hwnd, ConvContTabDlgProc);
		}

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

		return 0;


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



#define CONV_CONT_TAB_MARGIN	4

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
			CONV_CONT_TAB_MARGIN,
			CONV_CONT_TAB_MARGIN,
			LOWORD(lParam) - 2 * CONV_CONT_TAB_MARGIN,
			HIWORD(lParam) - 2 * CONV_CONT_TAB_MARGIN,
			SWP_NOZORDER | SWP_NOACTIVATE);

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
