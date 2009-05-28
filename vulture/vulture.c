/*
 * Vulture - Win32 libpurple client
 *
 * vulture.c: Entry point and utility functions.
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
#include "purplemain.h"
#include "purplequeue.h"
#include "cmdline.h"


HINSTANCE g_hInstance;
const TCHAR cg_szAppName[] = TEXT("Vulture");


/**
 * Entry point.
 *
 * @param	hinst		Instance handle.
 * @param	hinstPrev	Always NULL.
 * @param	szCmdLine	Complete command-line, in ANSI.
 * @param	iCmdShow	Initial window state. Cf. ShowWindow.
 *
 * @return Exit code.
 */
int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hinstPrev, LPSTR szCmdLine, int iCmdShow)
{
	MSG msg;
	HANDLE hthreadPurple;

	g_hInstance = hinst;

	VultureParseCommandLine();

	if(VultureCreateBuddyList(iCmdShow) != 0)
	{
		return 1;
	}

	VultureInitLibpurple(&hthreadPurple);
	if(hthreadPurple == (HANDLE)-1L)
	{
		return 2;
	}

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	/* UI has shut down; do the same to libpurple, waiting until it's
	 * complete.
	 */
	VultureEnqueueAsyncPurpleCall(PC_QUIT, NULL);
	WaitForSingleObject(hthreadPurple, INFINITE);
	CloseHandle(hthreadPurple);

	VultureShutDownLibpurple();

	VultureCommandLineCleanup();

	return msg.wParam;
}
