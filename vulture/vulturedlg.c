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
#include <glib.h>

#include "vulture.h"
#include "vulturedlg.h"
#include "resource.h"


static INT_PTR CALLBACK JoinChatDlgProc(HWND hwndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);


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
	switch(uiMsg)
	{
	case WM_INITDIALOG:
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
	}

	return FALSE;
}
