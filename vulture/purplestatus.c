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
#include "purplemain.h"


/* Number of populat saved statuses. */
#define CPSS_POPULAR	6


static void MakeVultureStatusFromPurple(PurpleSavedStatus *lppss, VULTURE_SAVED_STATUS *lpvss);


/**
 * Gets a list of VULTURE_SAVED_STATUS records representing those statuses we
 * want to show in the buddy-list window. Adapted from Finch.
 *
 * @param[out]	lplpglistStatuses	List to populate.
 */
void PurpleGetBoxSavedStatuses(GList **lplpglistStatuses)
{
	const PurpleStatusPrimitive c_rgpsprim[] =
	{
		PURPLE_STATUS_AVAILABLE, PURPLE_STATUS_AWAY,
		PURPLE_STATUS_INVISIBLE, PURPLE_STATUS_OFFLINE
	};

	GList *lpglistPurpleStatuses;
	int i;


	*lplpglistStatuses = NULL;

	/* Primitive statuses first. */
	for(i = 0; i < NUM_ELEMENTS(c_rgpsprim); i++)
	{
		VULTURE_SAVED_STATUS *lpvss = g_new(VULTURE_SAVED_STATUS, 1);

		const char *szTitle = purple_primitive_get_name_from_type(c_rgpsprim[i]);

		lpvss->psprim = c_rgpsprim[i];
		lpvss->vsstype = VSSTYPE_PRIMITIVE;
		lpvss->lppss = NULL;
		lpvss->szTitle = szTitle ? VultureUTF8ToTCHAR(szTitle) : NULL;
		lpvss->szMessage = NULL;

		*lplpglistStatuses = g_list_prepend(*lplpglistStatuses, lpvss);
	}


	/* Now do popular statuses. */
	for(lpglistPurpleStatuses = purple_savedstatuses_get_popular(CPSS_POPULAR);
		lpglistPurpleStatuses;
		lpglistPurpleStatuses = lpglistPurpleStatuses->next)
	{
		PurpleSavedStatus *lppss = lpglistPurpleStatuses->data;
		VULTURE_SAVED_STATUS *lpvss = g_new(VULTURE_SAVED_STATUS, 1);

		MakeVultureStatusFromPurple(lppss, lpvss);

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
		VultureFreeStatus((VULTURE_SAVED_STATUS*)lpglistRover->data);

	g_list_free(lpglistStatuses);
}


/**
 * Frees a VULTURE_SAVED_STATUS.
 *
 * @param	lpvss	Status to free.
 */
void VultureFreeStatus(VULTURE_SAVED_STATUS *lpvss)
{
	if(lpvss->szTitle) g_free(lpvss->szTitle);
	if(lpvss->szMessage) g_free(lpvss->szMessage);

	g_free(lpvss);
}


/**
 * Sets the active status.
 *
 * @param	lpvss	New status.
 */
void PurpleSetStatus(VULTURE_SAVED_STATUS *lpvss)
{
	char *szMessage = NULL;
	PurpleSavedStatus *lppss;

	switch(lpvss->vsstype)
	{
	case VSSTYPE_FIRM:
		/* This case is easy: we have a PurpleSavedStatus cached, so
		 * just activate it.
		 */
		purple_savedstatus_activate(lpvss->lppss);
		return;

	case VSSTYPE_TRANSIENT:
		if(lpvss->szMessage)
			szMessage = VultureTCHARToUTF8(lpvss->szMessage);
		break;

	default:
		break;
	}

	lppss = purple_savedstatus_find_transient_by_type_and_message(lpvss->psprim, szMessage);
	if(!lppss)
	{
		/* No matching transient status, so make one. */
		lppss = purple_savedstatus_new(NULL, lpvss->psprim);
		purple_savedstatus_set_message(lppss, szMessage);
	}

	purple_savedstatus_activate(lppss);

	if(szMessage)
		g_free(szMessage);
}


/**
 * savedstatus-changed signal-handler. Called when the status is changed.
 *
 * @param	lppssNew	New status.
 * @param	lppssOld	Old status.
 */
void PurpleStatusChanged(PurpleSavedStatus *lppssNew, PurpleSavedStatus *lppssOld)
{
	VULTURE_SAVED_STATUS *lpvss = g_new(VULTURE_SAVED_STATUS, 1);

	MakeVultureStatusFromPurple(lppssNew, lpvss);

	/* The UI will free the status we just allocated. */
	VulturePostUIMessage(g_hwndMain, VUIMSG_STATUSCHANGED, lpvss);
}


/**
 * Populate a VULTURE_SAVED_STATUS from a PurpleSavedStatus.
 *
 * @param	lppss	Source.
 * @param[out]	lpvss	Destination.
 */
static void MakeVultureStatusFromPurple(PurpleSavedStatus *lppss, VULTURE_SAVED_STATUS *lpvss)
{
	const char *szTitle = purple_savedstatus_get_title(lppss);
	const char *szMessage = purple_savedstatus_get_message(lppss);

	lpvss->psprim = purple_savedstatus_get_type(lppss);
	lpvss->vsstype = purple_savedstatus_is_transient(lppss) ? VSSTYPE_TRANSIENT : VSSTYPE_FIRM;
	lpvss->lppss = lppss;
	lpvss->szTitle = szTitle ? VultureUTF8ToTCHAR(szTitle) : NULL;
	lpvss->szMessage = szMessage ? VultureUTF8ToTCHAR(szMessage) : NULL;
}
