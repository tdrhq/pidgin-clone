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
#include <commctrl.h>
#include <glib.h>
#include <time.h>

#include "vulture.h"
#include "resource.h"
#include "vultureblist.h"
#include "purplemain.h"
#include "purplequeue.h"
#include "cmdline.h"
#include "vultureconv.h"


HINSTANCE g_hInstance;
HANDLE g_hProcHeap;

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
	INITCOMMONCONTROLSEX iccx;
	HACCEL haccel;
	HANDLE hlibRichEdit;

	g_hInstance = hinst;
	g_hProcHeap = GetProcessHeap();

	iccx.dwSize = sizeof(iccx);
	iccx.dwICC = ICC_WIN95_CLASSES | ICC_USEREX_CLASSES;
	InitCommonControlsEx(&iccx);

	/* Attempt to load RichEdit. This needs to be done before the dialogue
	 * manager tries to create any RichEdit controls.
	 */
	hlibRichEdit = LoadLibrary(TEXT("riched20.dll"));
	if(!hlibRichEdit)
	{
		MessageBoxFromStringTable(NULL, IDS_ERROR_RICHEDIT, MB_ICONERROR);
		return VEC_ERROR_RICHEDIT;
	}

	g_thread_init(NULL);

	VultureParseCommandLine();

	if(VultureRegisterConvContainerWindowClass() != 0)
	{
		MessageBoxFromStringTable(NULL, IDS_ERROR_CONVCONTCLASS, MB_ICONERROR);
		return VEC_ERROR_CONVCONTCLASS;
	}

	if(VultureCreateMainWindow(iCmdShow) != 0)
	{
		MessageBoxFromStringTable(NULL, IDS_ERROR_BLIST, MB_ICONERROR);
		return VEC_ERROR_BLIST;
	}

	VultureInitLibpurple(&hthreadPurple);
	if(hthreadPurple == (HANDLE)-1L)
	{
		MessageBoxFromStringTable(NULL, IDS_ERROR_PURPLEINIT, MB_ICONERROR);
		return VEC_ERROR_PURPLEINIT;
	}

	/* For keyboard shortcuts. */
	haccel = LoadAccelerators(hinst, MAKEINTRESOURCE(IDR_ACCEL_MAIN));

	while(GetMessage(&msg, NULL, 0, 0))
	{
		if(!TranslateAccelerator(g_hwndMain, haccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	/* UI has shut down; wait for libpurple, waiting until it's
	 * complete.
	 */
	WaitForSingleObject(hthreadPurple, INFINITE);
	CloseHandle(hthreadPurple);

	VultureShutDownLibpurple();

	VultureCommandLineCleanup();

	FreeLibrary(hlibRichEdit);

	return msg.wParam;
}


#define MBFST_INIT_BUFSIZE 128

/**
 * Displays a message box using a string from the table.
 *
 * @param	hwnd			Parent window.
 * @param	wResourceString		ID of string.
 * @param	uiType			As for MessageBox.
 *
 * @return As for MessageBox.
 */
int MessageBoxFromStringTable(HWND hwnd, WORD wResourceString, UINT uiType)
{
	MSGBOXPARAMS mbp;
	DWORD cchBuffer = MBFST_INIT_BUFSIZE;
	LPTSTR szBuffer = ProcHeapAlloc(cchBuffer * sizeof(TCHAR));
	int iRet;

	/* Load the string. We don't just pass the ID to MessageBoxIndirect, since
	 * it has an (undocumented?!) limit on the length of the string.
	 */
	while((DWORD)LoadString(g_hInstance, wResourceString, szBuffer, cchBuffer) >= cchBuffer - 1)
	{
		ProcHeapFree(szBuffer);
		cchBuffer <<= 1;
		szBuffer = ProcHeapAlloc(cchBuffer * sizeof(TCHAR));
	}


	mbp.cbSize = sizeof(mbp);
	mbp.dwContextHelpId = 0;
	mbp.dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
	mbp.dwStyle = uiType;
	mbp.hInstance = g_hInstance;
	mbp.hwndOwner = hwnd;
	mbp.lpfnMsgBoxCallback = NULL;
	mbp.lpszCaption = cg_szAppName;
	mbp.lpszIcon = NULL;
	mbp.lpszText = szBuffer;

	iRet = MessageBoxIndirect(&mbp);

	ProcHeapFree(szBuffer);

	return iRet;
}


/**
 * Converts a time_t to a SYSTEMTIME. Adapted from "Converting a time_t Value
 * to a File Time" from the Windows SDK.
 *
 * @param	t		Time to convert.
 * @param[out]	lpsystime	Converted time.
 */
void VultureTimetToSystemTime(time_t t, LPSYSTEMTIME lpsystime)
{
	FILETIME filetime;
	LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000LL;

	filetime.dwLowDateTime = (DWORD)ll;
	filetime.dwHighDateTime = ll >> 32;

	FileTimeToSystemTime(&filetime, lpsystime);
}
