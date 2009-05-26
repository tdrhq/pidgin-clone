/*
 * Vulture - Win32 libpurple client
 *
 * purplequeue.h: Event queue for the interface with libpurple.
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


#ifndef _VULTURE_PURPLEQUEUE_H_
#define _VULTURE_PURPLEQUEUE_H_


#include <windows.h>
#include <glib.h>


/** IDs of calls to libpurple. Used as values for iCallID member of
 * PURPLE_CALL.
 */
enum PURPLE_CALL_ID
{
	PC_QUIT,
};


void VulturePurpleInitQueue(void);
void VulturePurpleDestroyQueue(void);
void VultureEnqueueAsyncPurpleCall(int iCallID, void *lpvParam);
void VultureEnqueueSyncPurpleCall(int iCallID, void *lpvParam, HANDLE heventSync);
GSource* VultureCreateAsyncQueueSource(void);
GSource* VultureCreateSyncQueueSource(void);


#endif
