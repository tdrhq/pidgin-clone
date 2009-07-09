/*
 * Vulture - Win32 libpurple client
 *
 * vulture.h: Header to be included by all modules.
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

#ifndef _VULTURE_VULTURE_H_
#define _VULTURE_VULTURE_H_


#include <windows.h>
#include <glib.h>
#include <time.h>


#define INLINE			__inline
#define VULTURE_ID		"vulture"

#define NUM_ELEMENTS(arr)	(sizeof(arr)/sizeof(arr[0]))

extern HINSTANCE g_hInstance;
extern HANDLE g_hProcHeap;

extern const TCHAR cg_szAppName[];


enum VULTURE_EXIT_CODES
{
	VEC_SUCCESS = 0,
	VEC_ERROR_BLIST,
	VEC_ERROR_PURPLEINIT,
	VEC_ERROR_CONVCONTCLASS,
	VEC_ERROR_RICHEDIT,
};


enum VULTURE_WINDOW_MESSAGES
{
	WM_PURPLEUIMSG = WM_APP,
	WM_INPUTENTER,
};



int MessageBoxFromStringTable(HWND hwnd, WORD wResourceString, UINT uiType);
void VultureTimetToSystemTime(time_t t, LPSYSTEMTIME lpsystime);
LPTSTR VultureAmpersandify(LPCTSTR szUnderscored);
gint VultureCompareTCHARStrings(gconstpointer lpvStr1, gconstpointer lpvStr2, gpointer lpvUnused);



/** Wrappers for heap functions. */
static INLINE LPVOID ProcHeapAlloc(DWORD cb) { return HeapAlloc(g_hProcHeap, HEAP_GENERATE_EXCEPTIONS, cb); }
static INLINE LPVOID ProcHeapReAlloc(LPVOID lpv, DWORD cb) { return HeapReAlloc(g_hProcHeap, HEAP_GENERATE_EXCEPTIONS, lpv, cb); }
static INLINE void ProcHeapFree(LPVOID lpv) { HeapFree(g_hProcHeap, 0, lpv); }


/**
 * Converts a string of TCHARs to a string of UTF-8 characters.
 *
 * @param	sz	TCHAR string.
 *
 * @return UTF-8 string. Free with g_free.
 */
static INLINE gchar* VultureTCHARToUTF8(LPCTSTR sz)
{
#ifdef UNICODE
	return g_utf16_to_utf8(sz, -1, NULL, NULL, NULL);
#else
	gsize cchWritten;
	return g_locale_to_utf8(sz, -1, NULL, &cchWritten, NULL);
#endif
}


/**
 * Converts a string of UTF-8 characters to a string of TCHARs.
 *
 * @param	sz	UTF-8 string.
 *
 * @return TCHAR string. Free with g_free.
 */
static INLINE LPTSTR VultureUTF8ToTCHAR(const gchar* sz)
{
#ifdef UNICODE
	return g_utf8_to_utf16(sz, -1, NULL, NULL, NULL);
#else
	gsize cchWritten;
	return g_locale_from_utf8(sz, -1, NULL, &cchWritten, NULL);
#endif
}


#endif
