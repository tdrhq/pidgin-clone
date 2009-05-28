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

#include "vulture.h"
#include "blist.h"


static LRESULT CALLBACK BuddyListProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);


static HWND g_hwndBuddyList = NULL;


/**
 * Creates the buddy list.
 *
 * @param	iCmdShow	Initial window state. Cf. ShowWindow.
 *
 * @return Zero on success; non-zero on error.
 */
int VultureCreateBuddyList(int iCmdShow)
{
	const TCHAR c_szClassName[] = TEXT("VULTUREBLIST");

	WNDCLASSEX wndclassex;

	if(g_hwndBuddyList)
		return 1;

	wndclassex.cbClsExtra = 0;
	wndclassex.cbSize = sizeof(wndclassex);
	wndclassex.cbWndExtra = 0;
	wndclassex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclassex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclassex.hIconSm = LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR | LR_SHARED);
	wndclassex.hInstance = g_hInstance;
	wndclassex.lpfnWndProc = BuddyListProc;
	wndclassex.lpszClassName = c_szClassName;
	wndclassex.lpszMenuName = NULL;
	wndclassex.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;

	if(!RegisterClassEx(&wndclassex))
		return 2;

	g_hwndBuddyList = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
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

	if(!g_hwndBuddyList)
		return 3;

	ShowWindow(g_hwndBuddyList, iCmdShow);

	return 0;
}


/**
 * Buddy-list window procedure.
 *
 * @param	hwnd	Buddy-list handle.
 * @param	uiMsg	Message ID.
 * @param	wParam	Message-specific.
 * @param	lParam	Message-specific.
 *
 * @return Message-specific.
 */
static LRESULT CALLBACK BuddyListProc(HWND hwnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uiMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(VEC_SUCCESS);
		return 0;
	}

	return DefWindowProc(hwnd, uiMsg, wParam, lParam);
}
