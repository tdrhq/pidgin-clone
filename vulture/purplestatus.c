/*
 * Vulture - Win32 libpurple client
 *
 * purplestatus.c: Saved-status management.
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
#include "purplestatus.h"


/**
 * Gets a list of VULTURE_SAVED_STATUS records representing all saved statuses.
 * Adapted from Finch.
 *
 * @param[out]	lplpglistStatuses	List to populate.
 */
void PurpleGetAllSavedStatuses(GList **lplpglistStatuses)
{
	GList *lpglistPurpleStatuses;

	*lplpglistStatuses = NULL;

	for(lpglistPurpleStatuses = purple_savedstatuses_get_all(); lpglistPurpleStatuses; lpglistPurpleStatuses = lpglistPurpleStatuses->next)
	{
		PurpleSavedStatus *lppss = lpglistPurpleStatuses->data;
		VULTURE_SAVED_STATUS *lpvss;
		const char *szTitle, *szType, *szMessage;

		lpvss = g_new(VULTURE_SAVED_STATUS, 1);

		szTitle = purple_savedstatus_get_title(lppss);
		szType = purple_primitive_get_name_from_type(purple_savedstatus_get_type(lppss));
		szMessage = purple_savedstatus_get_message(lppss);

		lpvss->lppss = lppss;
		lpvss->szTitle = szTitle ? VultureUTF8ToTCHAR(szTitle) : NULL;
		lpvss->szType = szType ? VultureUTF8ToTCHAR(szType) : NULL;
		lpvss->szMessage = szMessage ? VultureUTF8ToTCHAR(szMessage) : NULL;

		*lplpglistStatuses = g_list_prepend(*lplpglistStatuses, lpvss);
	}

	/* We built the list backwards for efficiency. Fix it. */
	*lplpglistStatuses = g_list_reverse(*lplpglistStatuses);
}


/**
 * Frees a list of statuses returned by PurpleGetAllSavedStatuses.
 *
 * @param	lpglistStatuses		List to free.
 */
void VulturePurpleFreeStatusList(GList *lpglistStatuses)
{
	GList *lpglistRover;

	for(lpglistRover = lpglistStatuses; lpglistRover; lpglistRover = lpglistRover->next)
	{
		VULTURE_SAVED_STATUS *lpvss = lpglistRover->data;
		
		if(lpvss->szTitle) g_free(lpvss->szTitle);
		if(lpvss->szType) g_free(lpvss->szType);
		if(lpvss->szMessage) g_free(lpvss->szMessage);

		g_free(lpvss);
	}

	g_list_free(lpglistStatuses);
}
