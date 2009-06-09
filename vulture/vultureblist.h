/*
 * Vulture - Win32 libpurple client
 *
 * vultureblist.h: Buddy list.
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

#ifndef _VULTURE_VULTUREBLIST_H_
#define _VULTURE_VULTUREBLIST_H_


#include <windows.h>
#include <commctrl.h>


typedef struct _VULTURE_BLIST_NODE
{
	LPTSTR				szNodeText;
	HTREEITEM			hti;
	struct _VULTURE_BLIST_NODE	*lpvbnParent;
	CRITICAL_SECTION		cs;
} VULTURE_BLIST_NODE;


#define WM_PURPLEUIMSG	WM_APP


extern HWND g_hwndMain;


int VultureCreateMainWindow(int iCmdShow);


#endif
