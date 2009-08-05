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
#include "vulturebicon.h"


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
static void RunBuddyMenuCmd(HWND hwndBuddies, VULTURE_BLIST_NODE *lpvblistnode, HMENU hmenu, int iCmd);
static BOOL RunCommonMenuCmd(HWND hwndBuddies, VULTURE_BLIST_NODE *lpvblistnode, HMENU hmenu, int iCmd);
static void RunChatMenuCmd(HWND hwndBuddies, VULTURE_BLIST_NODE *lpvblistnode, HMENU hmenu, int iCmd);
static void RemoveNodeRequest(HWND hwndBuddies, VULTURE_BLIST_NODE *lpvblistnode);
static void UpdateBListNode(HWND hwndBlistTree, VULTURE_BLIST_NODE *lpvbn);
static void DrawBListNodeExtra(LPNMTVCUSTOMDRAW lpnmtvcdraw);
static HBITMAP GetBListNodeIcon(VULTURE_BLIST_NODE *lpvblistnode);
static void InvalidateBListNodeIconCache(VULTURE_BLIST_NODE *lpvblistnode);


#define BLIST_MARGIN		6
#define CX_STATUSICON		16
#define CY_STATUSICON		16
#define CX_BLISTNODEINTSPACER	8

enum CONTEXT_MENU_INDICES
{
	CMI_BUDDY = 0,
	CMI_CONTACT_COMPOSITE,
	CMI_CONTACT_BASIC,
	CMI_CHAT,
};


HWND g_hwndMain = NULL;
GList *g_lpglistConvContainers = NULL;

static GList *g_lpglistStatuses = NULL;
static HWND g_hwndStatusDlg = NULL, g_hwndBListDlg = NULL;
static HIMAGELIST g_himlStatusIcons = NULL;



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

			/* Load and initialise image lists. */
			g_himlStatusIcons = ImageList_LoadImage(g_hInstance, MAKEINTRESOURCE(IDB_STATUS_ICONS), CX_STATUSICON, 0, 0, IMAGE_BITMAP, LR_CREATEDIBSECTION);

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

		case IDM_BLIST_BUDDIES_ADDBUDDY:
			{
				VULTURE_ADD_BUDDY_DATA vabd;

				if(VultureAddBuddyDlg(hwnd, &vabd))
				{
					VultureSingleSyncPurpleCall(PC_ADDBUDDY, &vabd);

					if(vabd.lpvblistnodeGroup)
						VultureBListNodeRelease(vabd.lpvblistnodeGroup);

					ProcHeapFree(vabd.szUsername);
					if(vabd.szAlias)
						ProcHeapFree(vabd.szAlias);
				}
			}

			return 0;

		case IDM_BLIST_BUDDIES_CLOSE:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			return 0;

		case IDM_BLIST_VIEW_SHOWOFFLINE:
			g_vflags.bShowOffline = !g_vflags.bShowOffline;
			VultureEnqueueAsyncPurpleCall(PC_REFRESHBLIST, NULL);
			return 0;

		case IDM_BLIST_ACCOUNTS_MANAGE:
			ManageAccounts(hwnd);
			return 0;
		}

		break;

	case WM_INITMENUPOPUP:
		if(!HIWORD(lParam))
		{
			CheckMenuItem((HMENU)wParam, IDM_BLIST_VIEW_SHOWOFFLINE, g_vflags.bShowOffline ? MF_CHECKED : MF_UNCHECKED);
		}

		return 0;

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
				UpdateBListNode(GetDlgItem(g_hwndBListDlg, IDC_TREE_BLIST), (VULTURE_BLIST_NODE*)lParam);

				/* Release the reference for this call. */
				VultureBListNodeRelease((VULTURE_BLIST_NODE*)lParam);

				break;

			case VUIMSG_REMOVEBLISTNODE:
				RemoveBListNode(GetDlgItem(g_hwndBListDlg, IDC_TREE_BLIST), (VULTURE_BLIST_NODE*)lParam);
				VultureBListNodeRelease((VULTURE_BLIST_NODE*)lParam);
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

			case VUIMSG_CHATADDUSERS:
				hwndForward = ((VULTURE_CHAT_ADD_USERS*)lParam)->lpvconvChat->hwndConv;
				break;

			case VUIMSG_CHATRENAMEUSER:
				hwndForward = ((VULTURE_CHAT_RENAME_USER*)lParam)->lpvconvChat->hwndConv;
				break;

			case VUIMSG_CHATREMOVEUSERS:
				hwndForward = ((VULTURE_CHAT_REMOVE_USERS*)lParam)->lpvconvChat->hwndConv;
				break;

			case VUIMSG_INVALIDATEICONCACHE:
				{
					VULTURE_BLIST_NODE *lpvblistnode = (VULTURE_BLIST_NODE*)lParam;

					InvalidateBListNodeIconCache(lpvblistnode);

					/* Release the reference for this call. */
					VultureBListNodeRelease(lpvblistnode);
				}

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

		return 0;

	case WM_NCDESTROY:
		ImageList_Destroy(g_himlStatusIcons);
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

			/* Set the combo's image-lists, and tell it to
			 * recalculate its size.
			 */
			SendDlgItemMessage(hwndDlg, IDC_CBEX_STATUS, CBEM_SETIMAGELIST, 0, (LPARAM)g_himlStatusIcons);
			SetWindowPos(GetDlgItem(hwndDlg, IDC_CBEX_STATUS), NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
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
							
							VultureEnqueueAsyncPurpleCall(PC_BLISTNODEACTIVATED, (VULTURE_BLIST_NODE*)tvitem.lParam);

							return TRUE;
						}
					}

					break;

				case NM_RCLICK:
					{
						TVITEM tvitem;

						tvitem.hItem = TreeView_GetDropHilight(lpnmhdr->hwndFrom);
						if(!tvitem.hItem) tvitem.hItem = TreeView_GetSelection(lpnmhdr->hwndFrom);

						if(tvitem.hItem)
						{
							VULTURE_BLIST_NODE *lpvblistnode;
							HMENU hmenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDM_BLIST_CONTEXT));
							HMENU hmenuSubmenu = NULL;
							GList *lpglistVMA = NULL;
							VULTURE_MAKE_CONTEXT_MENU vmcm;

							/* Really select this node. */
							TreeView_SelectItem(lpnmhdr->hwndFrom, tvitem.hItem);

							tvitem.mask = TVIF_PARAM;
							TreeView_GetItem(lpnmhdr->hwndFrom, &tvitem);

							lpvblistnode = (VULTURE_BLIST_NODE*)tvitem.lParam;
							VultureBListNodeAddRef(lpvblistnode);

							/* Assume we need to ask the core for extra items. */
							vmcm.bExtraItems = TRUE;

							EnterCriticalSection(&lpvblistnode->cs);
								switch(lpvblistnode->nodetype)
								{
								case PURPLE_BLIST_BUDDY_NODE:
									hmenuSubmenu = GetSubMenu(hmenu, CMI_BUDDY);
									break;

								case PURPLE_BLIST_CONTACT_NODE:
									if(lpvblistnode->bExpanded)
									{
										hmenuSubmenu = GetSubMenu(hmenu, CMI_CONTACT_BASIC);
										vmcm.bExtraItems = FALSE;
									}
									else
										hmenuSubmenu = GetSubMenu(hmenu, CMI_CONTACT_COMPOSITE);

									break;

								case PURPLE_BLIST_CHAT_NODE:
									hmenuSubmenu = GetSubMenu(hmenu, CMI_CHAT);
									break;

								default:
									vmcm.bExtraItems = FALSE;
									break;
								}
							LeaveCriticalSection(&lpvblistnode->cs);

							vmcm.hmenu = hmenuSubmenu;
							vmcm.lpvblistnode = lpvblistnode;
							vmcm.lplpglistVMA = &lpglistVMA;

							VultureSingleSyncPurpleCall(PC_MAKECONTEXTMENU, &vmcm);

							if(hmenuSubmenu)
							{
								POINT ptMouse;
								int iCmd;

								GetCursorPos(&ptMouse);
								iCmd = TrackPopupMenu(hmenuSubmenu, TPM_RIGHTBUTTON | TPM_RETURNCMD, ptMouse.x, ptMouse.y, 0, hwndDlg, NULL);

								if(iCmd != 0)
								{
									switch(lpvblistnode->nodetype)
									{
									case PURPLE_BLIST_BUDDY_NODE:
									case PURPLE_BLIST_CONTACT_NODE:
										RunBuddyMenuCmd(lpnmhdr->hwndFrom, lpvblistnode, hmenuSubmenu, iCmd);
										break;
									case PURPLE_BLIST_CHAT_NODE:
										RunChatMenuCmd(lpnmhdr->hwndFrom, lpvblistnode, hmenuSubmenu, iCmd);
										break;
									default:
										break;
									}
								}
							}

							VultureBListNodeRelease(lpvblistnode);

							/* Destroy menu. This will also destroy our modifications. */
							DestroyMenu(hmenu);

							/* Clean up any extra data we might have as a result of
							 * having modified the menu.
							 */
							g_list_foreach(lpglistVMA, (GFunc)g_free, NULL);
							g_list_free(lpglistVMA);

							/* Prevent spurious right-click messages being sent
							 * elsewhere.
							 */
							SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, TRUE);

							return TRUE;
						}
					}

					break;

				case TVN_ENDLABELEDIT:
					{
						/* Label-editing in the buddy-
						 * list has finished. Alias the
						 * node if appropriate.
						 */

						VULTURE_ALIAS_NODE valiasnode;
						LPNMTVDISPINFO lpnmtvdispinfo = (LPNMTVDISPINFO)lParam;
						VULTURE_BLIST_NODE *lpvblistnode = (VULTURE_BLIST_NODE*)lpnmtvdispinfo->item.lParam;

						/* Make sure editing wasn't
						 * cancelled.
						 */
						if(lpnmtvdispinfo->item.pszText)
						{
							valiasnode.lpvblistnode = lpvblistnode;
							valiasnode.szAlias = lpnmtvdispinfo->item.pszText;
							VultureSingleSyncPurpleCall(PC_ALIASNODE, &valiasnode);
						}

						lpvblistnode->ui.editnodestate = VENS_NOEDIT;
					}

					return TRUE;

				case TVN_BEGINLABELEDIT:
					{
						VULTURE_BLIST_NODE *lpvblistnode = (VULTURE_BLIST_NODE*)((LPNMTVDISPINFO)lParam)->item.lParam;

						if(lpvblistnode->ui.editnodestate == VENS_PREEDIT)
						{
							SetWindowText(TreeView_GetEditControl(lpnmhdr->hwndFrom), lpvblistnode->szNodeText);
							lpvblistnode->ui.editnodestate = VENS_EDIT;
							SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, FALSE);
						}
						else
						{
							/* No secret handshake, no label edit! */
							SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, TRUE);
						}
					}

					return TRUE;

				case NM_CUSTOMDRAW:
					{
						LPNMTVCUSTOMDRAW lpnmtvcdraw = (LPNMTVCUSTOMDRAW)lParam;

						switch(lpnmtvcdraw->nmcd.dwDrawStage)
						{
						case CDDS_PREPAINT:
							/* Ask for notifications for each item. */
							SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, CDRF_NOTIFYITEMDRAW);
							break;

						case CDDS_ITEMPREPAINT:
							/* Tell me when you've finished painting. */
							SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, CDRF_NOTIFYPOSTPAINT);
							break;

						case CDDS_ITEMPOSTPAINT:
							DrawBListNodeExtra(lpnmtvcdraw);
							SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, CDRF_DODEFAULT);
							break;
						}
					}

					return TRUE;
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
	static GHashTable *s_lpghashStatusIcons = NULL;

	GList *lpglistStatusRover;
	GArray *lpgarrayWaitContext;

	/* Initialise the lookup table of status icons the first time. */
	if(!s_lpghashStatusIcons)
	{
		s_lpghashStatusIcons = g_hash_table_new(NULL, NULL);
		g_hash_table_insert(s_lpghashStatusIcons, (gpointer)PURPLE_STATUS_AVAILABLE, (gpointer)SICON_AVAILABLE);
		g_hash_table_insert(s_lpghashStatusIcons, (gpointer)PURPLE_STATUS_AWAY, (gpointer)SICON_AWAY);
		g_hash_table_insert(s_lpghashStatusIcons, (gpointer)PURPLE_STATUS_UNAVAILABLE, (gpointer)SICON_BUSY);
		g_hash_table_insert(s_lpghashStatusIcons, (gpointer)PURPLE_STATUS_INVISIBLE, (gpointer)SICON_INVISIBLE);
		g_hash_table_insert(s_lpghashStatusIcons, (gpointer)PURPLE_STATUS_OFFLINE, (gpointer)SICON_OFFLINE);
		g_hash_table_insert(s_lpghashStatusIcons, (gpointer)PURPLE_STATUS_EXTENDED_AWAY, (gpointer)SICON_EXTAWAY);
	}

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

		if(g_hash_table_lookup_extended(s_lpghashStatusIcons, (gpointer)lpvss->psprim, NULL, (gpointer*)&cbexitem.iImage))
		{
			cbexitem.iSelectedImage = cbexitem.iImage;
			cbexitem.mask |= CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
		}

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
 * @param	hwndBlistTree	Buddy-list tree-view handle.
 * @param	lpvbn		Node to delete.
 */
static void RemoveBListNode(HWND hwndBlistTree, VULTURE_BLIST_NODE *lpvbn)
{
	if(lpvbn->hti)
	{
		EnterCriticalSection(&lpvbn->cs);
			TreeView_DeleteItem(hwndBlistTree, lpvbn->hti);
			lpvbn->hti = NULL;
		LeaveCriticalSection(&lpvbn->cs);

		/* Release the reference belonging to the pointer
		 * cached in the tree-item.
		 */
		VultureBListNodeRelease(lpvbn);
	}
}


/**
 * Executes a menu command from the context menu for a buddy or contact node in
 * the buddy list.
 *
 * @param	hwndBuddies	Buddy-list tree-view.
 * @param	lpvblistnode	List node to which the context menu relates.
 * @param	hmenu		Context menu.
 * @param	iCmd		Command ID.
 */
static void RunBuddyMenuCmd(HWND hwndBuddies, VULTURE_BLIST_NODE *lpvblistnode, HMENU hmenu, int iCmd)
{
	if(RunCommonMenuCmd(hwndBuddies, lpvblistnode, hmenu, iCmd))
		return;

	switch(iCmd)
	{
	case IDM_BLIST_CONTEXT_SHOWOFFLINE:
		VultureEnqueueAsyncPurpleCall(PC_TOGGLESHOWOFFLINE, lpvblistnode);
		break;

	case IDM_BLIST_CONTEXT_COLLAPSE:
		EnterCriticalSection(&lpvblistnode->cs);
		{
			HTREEITEM hti;

			lpvblistnode->bExpanded = FALSE;

			while((hti = TreeView_GetChild(hwndBuddies, lpvblistnode->hti)))
			{
				TVITEM tvi;

				tvi.hItem = hti;
				tvi.mask = TVIF_PARAM;
				TreeView_GetItem(hwndBuddies, &tvi);

				RemoveBListNode(hwndBuddies, (VULTURE_BLIST_NODE*)tvi.lParam);
			}
		}
		LeaveCriticalSection(&lpvblistnode->cs);

		/* Update tree node. */
		UpdateBListNode(hwndBuddies, lpvblistnode);

		break;

	case IDM_BLIST_CONTEXT_EXPAND:
		lpvblistnode->bExpanded = TRUE;
		VultureSingleSyncPurpleCall(PC_UPDATEBLISTCHILDREN, lpvblistnode);
		PostMessage(hwndBuddies, TVM_EXPAND, TVE_EXPAND, (LPARAM)lpvblistnode->hti);

		/* Update tree node. */
		UpdateBListNode(hwndBuddies, lpvblistnode);

		break;
	}
}


/**
 * Determines whether a menu command is one of those common to various sorts of
 * context menu, and if so, executes it.
 *
 * @param	hwndBuddies	Buddy-list tree-view.
 * @param	lpvblistnode	List node to which the context menu relates.
 * @param	hmenu		Context menu.
 * @param	iCmd		Command ID.
 *
 * @return TRUE iff we processed the command.
 */
static BOOL RunCommonMenuCmd(HWND hwndBuddies, VULTURE_BLIST_NODE *lpvblistnode, HMENU hmenu, int iCmd)
{
	UNREFERENCED_PARAMETER(hmenu);

	switch(iCmd)
	{
	case IDM_BLIST_CONTEXT_ACTIVATE:
		VultureEnqueueAsyncPurpleCall(PC_BLISTNODEACTIVATED, lpvblistnode);
		return TRUE;

	case IDM_BLIST_CONTEXT_ALIAS:
		lpvblistnode->ui.editnodestate = VENS_PREEDIT;
		SendMessage(hwndBuddies, TVM_EDITLABEL, 0, (LPARAM)lpvblistnode->hti);
		return TRUE;

	case IDM_BLIST_CONTEXT_REMOVE:
		RemoveNodeRequest(hwndBuddies, lpvblistnode);
		return TRUE;

	case IDM_BLIST_CONTEXT_CUSTOMICON:
		{
			TCHAR szFilename[MAX_PATH];
			TCHAR szFilter[256];
			TCHAR szTitle[256];

			VultureLoadAndFormatFilterString(IDS_BUDDYICON_FILTER, szFilter, NUM_ELEMENTS(szFilter));
			LoadString(g_hInstance, IDS_BUDDYICON_TITLE, szTitle, NUM_ELEMENTS(szTitle));

			if(VultureCommDlgOpen(g_hwndMain, szFilename, NUM_ELEMENTS(szFilename), szTitle, szFilter, TEXT("png"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY))
			{
				VULTURE_BLIST_NODE_STRING_PAIR vblnstringpairNewIcon;

				InvalidateBListNodeIconCache(lpvblistnode);

				vblnstringpairNewIcon.lpvblistnode = lpvblistnode;
				vblnstringpairNewIcon.sz = szFilename;

				VultureSingleSyncPurpleCall(PC_SETCUSTOMICON, &vblnstringpairNewIcon);
			}
		}

		return TRUE;

	case IDM_BLIST_CONTEXT_REMOVEICON:
		{
			VULTURE_BLIST_NODE_STRING_PAIR vblnstringpairNewIcon;

			InvalidateBListNodeIconCache(lpvblistnode);

			vblnstringpairNewIcon.lpvblistnode = lpvblistnode;
			vblnstringpairNewIcon.sz = NULL;

			VultureSingleSyncPurpleCall(PC_SETCUSTOMICON, &vblnstringpairNewIcon);
		}

	default:
		/* Not a static command that we recongise; might be a dynamic
		 * command.
		 */
		if(iCmd >= IDM_DYNAMIC_FIRST)
		{
			MENUITEMINFO mii;

			mii.cbSize = sizeof(mii);
			mii.fMask = MIIM_DATA;
			GetMenuItemInfo(hmenu, iCmd, FALSE, &mii);

			VultureSingleSyncPurpleCall(PC_PERFORMMENUACTION, (VULTURE_MENU_ACTION*)mii.dwItemData);

			return TRUE;
		}

		break;
	}

	return FALSE;
}


/**
 * Executes a menu command from the context menu for a chat node in the buddy
 * list.
 *
 * @param	hwndBuddies	Buddy-list tree-view.
 * @param	lpvblistnode	List node to which the context menu relates.
 * @param	hmenu		Context menu.
 * @param	iCmd		Command ID.
 */
static void RunChatMenuCmd(HWND hwndBuddies, VULTURE_BLIST_NODE *lpvblistnode, HMENU hmenu, int iCmd)
{
	if(RunCommonMenuCmd(hwndBuddies, lpvblistnode, hmenu, iCmd))
		return;

	switch(iCmd)
	{
	case IDM_BLIST_CONTEXT_AUTOJOIN:
		VultureEnqueueAsyncPurpleCall(PC_TOGGLEAUTOJOIN, lpvblistnode);
		break;
	}
}


/**
 * Deletes a buddy-list node, prompting the user first if necessary.
 *
 * @param	hwndBuddies	Buddy-list tree-view.
 * @param	lpvblistnode	List node to delete.
 */
static void RemoveNodeRequest(HWND hwndBuddies, VULTURE_BLIST_NODE *lpvblistnode)
{
	VULTURE_BLIST_NODE_GET_BOOL vblngetbool;
	BOOL bDelete = TRUE;

	vblngetbool.lpvblistnode = lpvblistnode;
	VultureSingleSyncPurpleCall(PC_BLISTNODEHASCHILDREN, &vblngetbool);

	/* If we have any children, prompt before deleting. */
	if(vblngetbool.bReturn)
	{
		EnterCriticalSection(&lpvblistnode->cs);
			if(lpvblistnode->nodetype == PURPLE_BLIST_GROUP_NODE)
				bDelete = MessageBoxFromStringTable(g_hwndMain, IDS_QUERY_DELGROUP, MB_ICONEXCLAMATION | MB_YESNO);
			else if(lpvblistnode->nodetype == PURPLE_BLIST_CONTACT_NODE)
				bDelete = MessageBoxFromStringTable(g_hwndMain, IDS_QUERY_DELCONTACT, MB_ICONEXCLAMATION | MB_YESNO);
		LeaveCriticalSection(&lpvblistnode->cs);
	}

	if(bDelete)
		VultureSingleSyncPurpleCall(PC_REMOVEBLISTNODE, lpvblistnode);
}


/**
 * Updates a buddy-list node in the tree, according to its data.
 *
 * @param	hwndBlistTree	Buddy-list tree-view handle.
 * @param	lpvbn		Node to update.
 */
static void UpdateBListNode(HWND hwndBlistTree, VULTURE_BLIST_NODE *lpvbn)
{
	TVITEMEX tvitemex;

	if(lpvbn->hti)
	{
		HTREEITEM htiParent = TreeView_GetParent(hwndBlistTree, lpvbn->hti);

		/* If the parent doesn't match, we need
		 * to recreate.
		 */
		EnterCriticalSection(&lpvbn->cs);
		{
			if((lpvbn->lpvbnParent && lpvbn->lpvbnParent->hti != htiParent) ||
				(!lpvbn->lpvbnParent && htiParent))
			{
				RemoveBListNode(hwndBlistTree, lpvbn);
			}
		}
		LeaveCriticalSection(&lpvbn->cs);
	}


	/* New node? */
	if(!lpvbn->hti)
	{
		TVINSERTSTRUCT tvis;

		/* We cache this in the tree-view. */
		VultureBListNodeAddRef(lpvbn);

		EnterCriticalSection(&lpvbn->cs);
		{
			tvis.hParent = lpvbn->lpvbnParent ? lpvbn->lpvbnParent->hti : TVI_ROOT;
			tvis.hInsertAfter = TVI_SORT;
			tvis.itemex.mask = TVIF_PARAM;
			tvis.itemex.lParam = (LPARAM)lpvbn;
		}
		LeaveCriticalSection(&lpvbn->cs);

		lpvbn->hti = TreeView_InsertItem(hwndBlistTree, &tvis);
	}

	/* Set height. */
	EnterCriticalSection(&lpvbn->cs);
	{
		tvitemex.mask = TVIF_HANDLE | TVIF_INTEGRAL;
		tvitemex.hItem = lpvbn->hti;
		tvitemex.iIntegral =
			((lpvbn->nodetype == PURPLE_BLIST_CONTACT_NODE && lpvbn->bExpanded) || lpvbn->nodetype == PURPLE_BLIST_GROUP_NODE) ?
			1 :
			2;
	}
	LeaveCriticalSection(&lpvbn->cs);

	TreeView_SetItem(hwndBlistTree, &tvitemex);
}


/**
 * Draws custom stuff for a buddy-list node.
 *
 * @param	lpnmtvcdraw	Custom-draw data for node.
 */
static void DrawBListNodeExtra(LPNMTVCUSTOMDRAW lpnmtvcdraw)
{
	RECT rcText, rcClient;
	COLORREF crOldFG, crOldBG;
	VULTURE_BLIST_NODE *lpvblistnode = (VULTURE_BLIST_NODE*)lpnmtvcdraw->nmcd.lItemlParam;
	HBITMAP hbmIcon;

	GetClientRect(lpnmtvcdraw->nmcd.hdr.hwndFrom, &rcClient);

	/* Get the rectangle in which the control would render its text if it
	 * were doing so itself, and extend it to the right width.
	 */
	TreeView_GetItemRect(lpnmtvcdraw->nmcd.hdr.hwndFrom, (HTREEITEM)lpnmtvcdraw->nmcd.dwItemSpec, &rcText, TRUE);
	rcText.right = rcClient.right;
	InflateRect(&rcText, -1, -1);

	crOldBG = SetBkColor(lpnmtvcdraw->nmcd.hdc, lpnmtvcdraw->clrTextBk);
	crOldFG = SetTextColor(lpnmtvcdraw->nmcd.hdc, lpnmtvcdraw->clrText);

	/* This call must be made outside the CS. */
	hbmIcon = GetBListNodeIcon(lpvblistnode);

	EnterCriticalSection(&lpvblistnode->cs);
	{
		/* Draw status icon. */
		if(lpvblistnode->nodetype != PURPLE_BLIST_GROUP_NODE && lpvblistnode->ui.editnodestate != VENS_EDIT)
		{
			int xIcon = rcText.left;
			int yIcon = (rcText.bottom + rcText.top - CY_STATUSICON) / 2;
			int iIndex = -1;

			/* Adjust text rectangle for icon. */
			rcText.left += CX_STATUSICON + CX_BLISTNODEINTSPACER;

			switch(lpvblistnode->nodetype)
			{
			case PURPLE_BLIST_CONTACT_NODE:
				if(lpvblistnode->bExpanded)
					iIndex = SICON_PERSON;
				else
					/* Fall through. */
			case PURPLE_BLIST_BUDDY_NODE:
					iIndex = lpvblistnode->iStatusIcon;
				break;

			case PURPLE_BLIST_CHAT_NODE:
				iIndex = SICON_CHAT;
				break;

			default:
				break;
			}

			if(iIndex >= 0)
				ImageList_Draw(g_himlStatusIcons, iIndex, lpnmtvcdraw->nmcd.hdc, xIcon, yIcon, ILD_NORMAL);
		}

		/* Draw main icon. */
		if(hbmIcon)
		{
			BITMAP bitmap;
			HDC hdcMem;
			HBITMAP hbmOld;
			int iEdge = rcText.bottom - rcText.top;
			int xScaled, yScaled, cxScaled, cyScaled;
			int iOldStretchMode;

			GetObject(hbmIcon, sizeof(bitmap), &bitmap);

			/* Adjust text rectangle at the right-hand edge. */
			rcText.right -= iEdge + CX_BLISTNODEINTSPACER;

			/* Calculate dimensions. */
			if(bitmap.bmWidth <= iEdge && bitmap.bmHeight <= iEdge)
			{
				/* We fit entirely in the room available. */
				cxScaled = bitmap.bmWidth;
				cyScaled = bitmap.bmHeight;
			}
			else if(bitmap.bmWidth > bitmap.bmHeight)
			{
				/* Scale to fit width. */
				cxScaled = iEdge;
				cyScaled = MulDiv(bitmap.bmHeight, iEdge, bitmap.bmWidth);
			}
			else
			{
				/* Scale to fit height. */
				cxScaled = MulDiv(bitmap.bmWidth, iEdge, bitmap.bmHeight);
				cyScaled = iEdge;
			}

			xScaled = rcText.right + CX_BLISTNODEINTSPACER + (iEdge - cxScaled) / 2;
			yScaled = rcText.top + (iEdge - cyScaled) / 2;

			hdcMem = CreateCompatibleDC(lpnmtvcdraw->nmcd.hdc);
			hbmOld = SelectObject(hdcMem, hbmIcon);
			iOldStretchMode = SetStretchBltMode(lpnmtvcdraw->nmcd.hdc, COLORONCOLOR);
			StretchBlt(lpnmtvcdraw->nmcd.hdc, xScaled, yScaled, cxScaled, cyScaled, hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
			SetStretchBltMode(lpnmtvcdraw->nmcd.hdc, iOldStretchMode);
			SelectObject(hdcMem, hbmOld);
			DeleteDC(hdcMem);
		}

		if(lpvblistnode->ui.editnodestate != VENS_EDIT)
		{
			if(lpvblistnode->szStatusText &&
				((lpvblistnode->nodetype == PURPLE_BLIST_CONTACT_NODE && !lpvblistnode->bExpanded) ||
				lpvblistnode->nodetype == PURPLE_BLIST_BUDDY_NODE))
			{
				HTREEITEM htiSel, htiDrop;

				/* Render main text. */
				if(lpvblistnode->szNodeText)
					DrawText(lpnmtvcdraw->nmcd.hdc, lpvblistnode->szNodeText, -1, &rcText, DT_END_ELLIPSIS | DT_SINGLELINE);

				/* Render secondary text. */
				htiSel = TreeView_GetSelection(lpnmtvcdraw->nmcd.hdr.hwndFrom);
				htiDrop = TreeView_GetDropHilight(lpnmtvcdraw->nmcd.hdr.hwndFrom);
				if(htiDrop != (HTREEITEM)lpnmtvcdraw->nmcd.dwItemSpec &&
					 (htiSel != (HTREEITEM)lpnmtvcdraw->nmcd.dwItemSpec || htiDrop != NULL))
					SetTextColor(lpnmtvcdraw->nmcd.hdc, 0x808080);
				DrawText(lpnmtvcdraw->nmcd.hdc, lpvblistnode->szStatusText, -1, &rcText, DT_BOTTOM | DT_END_ELLIPSIS | DT_SINGLELINE);
			}
			else
			{
				/* No secondary text; render main text only. */
				if(lpvblistnode->szNodeText)
					DrawText(lpnmtvcdraw->nmcd.hdc, lpvblistnode->szNodeText, -1, &rcText, DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE);
			}
		}
	}
	LeaveCriticalSection(&lpvblistnode->cs);

	SetTextColor(lpnmtvcdraw->nmcd.hdc, crOldFG);
	SetBkColor(lpnmtvcdraw->nmcd.hdc, crOldBG);
}


/**
 * Retrieves the icon to display for a buddy-list node. This may make a call to
 * the core, so beware of deadlocks.
 *
 * @param	lpvblistnode	Buddy-list node.
 */
static HBITMAP GetBListNodeIcon(VULTURE_BLIST_NODE *lpvblistnode)
{
	if(!lpvblistnode->ui.bIconCacheValid)
	{
		VULTURE_GET_BLIST_NODE_ICON vgblnicon;

		vgblnicon.lpvblistnode = lpvblistnode;
		VultureSingleSyncPurpleCall(PC_GETBLISTNODEICON, &vgblnicon);
		lpvblistnode->ui.hbmIconCache = vgblnicon.hbmIcon;
		lpvblistnode->ui.bIconCacheValid = TRUE;
	}

	return lpvblistnode->ui.hbmIconCache;
}


/**
 * Invalidates the icon cache for a buddy-list node, freeing the cached bitmap.
 *
 * @param	lpvblistnode	Buddy-list node.
 */
static void InvalidateBListNodeIconCache(VULTURE_BLIST_NODE *lpvblistnode)
{
	if(lpvblistnode->ui.bIconCacheValid)
	{
		if(lpvblistnode->ui.hbmIconCache)
			DeleteObject(lpvblistnode->ui.hbmIconCache);

		lpvblistnode->ui.bIconCacheValid = FALSE;
	}
}
