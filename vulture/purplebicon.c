/*
 * Vulture - Win32 libpurple client
 *
 * purplebicon.c: Buddy-icon management.
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
#include <glib/gstdio.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string.h>

#include "vulture.h"
#include "purple.h"
#include "purplebicon.h"
#include "purplemain.h"


static HBITMAP GetBuddyIcon(gconstpointer lpvBuddyIconData, size_t cbBuddyIconData, int cxMax, int cyMax);
static gpointer ConvertAndScaleBuddyIcon(const gchar *szFilename, PurplePlugin *lpplugin, gsize *lpcbImage);
static void SetGlobalBuddyIcon(const gchar *szFilename);



/**
 * Retrieves the buddy icon for a buddy. It is scaled, with its aspect ratio
 * maintained, so as not to exceed the specified dimensions, unless either of
 * these is non-positive, in which case no scaling is performed.
 *
 * @param	lpbuddy		Buddy.
 * @param	cxMax		Maximum width.
 * @param	cyMax		Maximum height.
 *
 * @return Bitmap handle, or NULL on error.
 */
static HBITMAP GetBuddyIcon(gconstpointer lpvBuddyIconData, size_t cbBuddyIconData, int cxMax, int cyMax)
{
	GdkPixbuf *lppixbuf;
	GdkPixbufLoader *lppbloader;
	guchar *lpucPixbufPixels;
	int cbPixbufRowstride, cbDIBRowstride;
	int cx, cy, cxScaled, cyScaled;
	HDC hdc, hdcScreen;
	HBITMAP hbitmap, hbmOld;
	BITMAPINFO bmi;
	BYTE *lpbyBits;
	int iRow;
	int iChannels, iBitsPerSample;
	GError *lpgerror = NULL;

	lppbloader = gdk_pixbuf_loader_new();
	if(!gdk_pixbuf_loader_write(lppbloader, lpvBuddyIconData, cbBuddyIconData, &lpgerror))
	gdk_pixbuf_loader_close(lppbloader, NULL);

	lppixbuf = gdk_pixbuf_loader_get_pixbuf(lppbloader);

	g_object_unref(lppbloader);

	lpucPixbufPixels = gdk_pixbuf_get_pixels(lppixbuf);

	cx = gdk_pixbuf_get_width(lppixbuf);
	cy = gdk_pixbuf_get_height(lppixbuf);
	cbPixbufRowstride = gdk_pixbuf_get_rowstride(lppixbuf);
	iChannels = gdk_pixbuf_get_n_channels(lppixbuf);
	iBitsPerSample = gdk_pixbuf_get_bits_per_sample(lppixbuf);

	/* This assertion will always pass at the time of writing, but we do
	 * rely on these properties.
	 */
	if((iChannels != 3 && iChannels != 4) || iBitsPerSample != 8)
	{
		g_object_unref(lppixbuf);
		return NULL;
	}


	/* Convert the pixbuf's pixels into a DIB. */

	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biBitCount = iChannels * iBitsPerSample;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biWidth = cx;
	bmi.bmiHeader.biHeight = cy;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biSizeImage = 0;
	bmi.bmiHeader.biClrImportant = 0;
	bmi.bmiHeader.biClrUsed = 0;
	bmi.bmiHeader.biXPelsPerMeter = 0;
	bmi.bmiHeader.biYPelsPerMeter = 0;

	/* Scanlines must start on DWORD boundaries. */
	cbDIBRowstride = 4 * ((cx * iChannels + 3) / 4);

	lpbyBits = ProcHeapAlloc(cbDIBRowstride * cy);

	/* Bottom-up DIB. */
	for(iRow = 0; iRow < cy; iRow++)
	{
		int cbDIBRowOffest = (cy - iRow - 1) * cbDIBRowstride;
		int cbPixbufRowOffest = iRow * cbPixbufRowstride;
		int iCol;

		for(iCol = 0; iCol < cx; iCol++)
		{
			int iChan;

			/* DIB is 0x(AA)RRGGBB; pixbuf is 0x(AA)BBGGRR (both
			 * little-endian).
			 */

			/* Blue, green, red. */
			for(iChan = 0; iChan < 3; iChan++)
				lpbyBits[cbDIBRowOffest + iCol * iChannels + iChan] = lpucPixbufPixels[cbPixbufRowOffest + iCol * iChannels + (2 - iChan)];

			if(iChannels == 4)
				lpbyBits[cbDIBRowOffest + iCol * iChannels + 3] = lpucPixbufPixels[cbPixbufRowOffest + iCol * iChannels + 3];
		}
	}

	g_object_unref(lppixbuf);

	/* Scale if necessary. */
	if(cxMax > 0 && cyMax > 0 && (cx > cxMax || cy > cyMax))
	{
		if(cx * cyMax > cy * cxMax)
		{
			/* Scale to fit width. */
			cxScaled = cxMax;
			cyScaled = MulDiv(cy, cxMax, cx);
		}
		else
		{
			/* Scaled to fit height. */
			cxScaled = MulDiv(cx, cyMax, cy);
			cyScaled = cyMax;
		}
	}
	else
	{
		cxScaled = cx;
		cyScaled = cy;
	}

	hdcScreen = GetDC(NULL);
	hdc = CreateCompatibleDC(hdcScreen);

	hbitmap = CreateCompatibleBitmap(hdcScreen, cxScaled, cyScaled);
	hbmOld = SelectObject(hdc, hbitmap);
	SetStretchBltMode(hdc, COLORONCOLOR);
	StretchDIBits(hdc, 0, 0, cxScaled, cyScaled, 0, 0, cx, cy, lpbyBits, &bmi, DIB_RGB_COLORS, SRCCOPY);
	SelectObject(hdc, hbmOld);
	
	DeleteDC(hdc);
	ReleaseDC(NULL, hdcScreen);

	ProcHeapFree(lpbyBits);

	return hbitmap;
}


/**
 * Retrieves the buddy icon to be shown for an IM. It is scaled, with its
 * aspect ratio maintained, so as not to exceed the specified dimensions,
 * unless either of these is non-positive, in which case no scaling is
 * performed.
 *
 * @param	lpconv	IM conversation.
 * @param	cxMax	Maximum width.
 * @param	cyMax	Maximum height.
 *
 * @return Bitmap handle, or NULL on error.
 */
HBITMAP PurpleGetIMBuddyIcon(PurpleConversation *lpconv, int cxMax, int cyMax)
{
	PurpleBuddy *lpbuddy;
	PurpleContact *lpcontact;
	PurpleStoredImage *lpstoredimg = NULL;
	gconstpointer lpvBuddyIconData = NULL;
	size_t cbBuddyIconData;
	PurpleBuddyIcon *lpbuddyicon = NULL;
	HBITMAP hbitmap = NULL;

	if(!lpconv || lpconv->type != PURPLE_CONV_TYPE_IM)
		return NULL;

	/* First we attempt to load the custom icon for the buddy's contact. */
	if((lpbuddy = purple_find_buddy(lpconv->account, purple_conversation_get_name(lpconv))) &&
		(lpcontact = purple_buddy_get_contact(lpbuddy)) &&
		(lpstoredimg = purple_buddy_icons_node_find_custom_icon((PurpleBlistNode*)lpcontact)))
	{
		lpvBuddyIconData = purple_imgstore_get_data(lpstoredimg);
		cbBuddyIconData = purple_imgstore_get_size(lpstoredimg);
	}

	if(!lpvBuddyIconData &&
		(lpbuddyicon = purple_conv_im_get_icon(PURPLE_CONV_IM(lpconv))))
	{
		lpvBuddyIconData = purple_buddy_icon_get_data(lpbuddyicon, &cbBuddyIconData);
	}


	if(lpvBuddyIconData)
		hbitmap = GetBuddyIcon(lpvBuddyIconData, cbBuddyIconData, cxMax, cyMax);

	/* These are safe even when their arguments are NULL. */
	purple_imgstore_unref(lpstoredimg);
	purple_buddy_icon_unref(lpbuddyicon);

	return hbitmap;
}


/**
 * Retrieves the buddy icon to be shown for a buddy-list node. It is scaled,
 * with its aspect ratio maintained, so as not to exceed the specified
 * dimensions, unless either of these is non-positive, in which case no scaling
 * is performed.
 *
 * @param	lpconv	IM conversation.
 * @param	cxMax	Maximum width.
 * @param	cyMax	Maximum height.
 *
 * @return Bitmap handle, or NULL on error.
 */
HBITMAP PurpleGetBlistNodeIcon(PurpleBlistNode *lpblistnode, int cxMax, int cyMax)
{
	PurpleStoredImage *lpstoredimg;
	gconstpointer lpvBuddyIconData = NULL;
	size_t cbBuddyIconData;
	PurpleBuddy *lpbuddy = NULL;
	HBITMAP hbitmap = NULL;
	PurpleBuddyIcon *lpbuddyicon = NULL;

	if(!lpblistnode)
		return NULL;

	/* First we attempt to load a custom icon for the node. */
	if((lpstoredimg = purple_buddy_icons_node_find_custom_icon(lpblistnode)))
	{
		lpvBuddyIconData = purple_imgstore_get_data(lpstoredimg);
		cbBuddyIconData = purple_imgstore_get_size(lpstoredimg);
	}

	if(!lpvBuddyIconData)
	{
		/* If that failed, fall back to a buddy if we can. */
		if(PURPLE_BLIST_NODE_IS_BUDDY(lpblistnode))
			lpbuddy = (PurpleBuddy*)lpblistnode;
		else if(PURPLE_BLIST_NODE_IS_CONTACT(lpblistnode))
			lpbuddy = purple_contact_get_priority_buddy((PurpleContact*)lpblistnode);

		if(lpbuddy && (lpbuddyicon = purple_buddy_icons_find(lpbuddy->account, lpbuddy->name)))
			lpvBuddyIconData = purple_buddy_icon_get_data(lpbuddyicon, &cbBuddyIconData);
	}

	if(lpvBuddyIconData)
		hbitmap = GetBuddyIcon(lpvBuddyIconData, cbBuddyIconData, cxMax, cyMax);

	/* These are safe even when their arguments are NULL. */
	purple_imgstore_unref(lpstoredimg);
	purple_buddy_icon_unref(lpbuddyicon);

	return hbitmap;
}


/**
 * Retrieves contents of an image file, scaled and converted into a format
 * suitable for a buddy icon. Based very closely on Pidgin's
 * pidgin_convert_buddy_icon.
 *
 * @param	szFilename	Filename of buddy icon.
 * @param	lpplugin	Prpl.
 * @param[out]	lpcbImage	Returns the length of the data. May be
 *				clobbered even on error.
 *
 * @return Image data, or NULL on error.
 */
static gpointer ConvertAndScaleBuddyIcon(const gchar *szFilename, PurplePlugin *lpplugin, gsize *lpcbImage)
{
	gchar *szTempFile = NULL;
	PurplePluginProtocolInfo *lpprplinfo;
	int cx, cy;
	int i, j;
	gchar **rgszPixbufFormats, **rgszPrplFormats;
	BOOL bFormatSupported, bNeedScale;
	GdkPixbufFormat *lppixbufformat;
	gchar *lpcImage;

	lpprplinfo = PURPLE_PLUGIN_PROTOCOL_INFO(lpplugin);
	if(!lpprplinfo->icon_spec.format)
		return NULL;

	lppixbufformat = gdk_pixbuf_get_file_info(szFilename, &cx, &cy);
	if(!lppixbufformat)
		return NULL;

	/* Which formats does the prpl support? */
	rgszPrplFormats = g_strsplit(lpprplinfo->icon_spec.format, ",", 0);

	/* Attempt to match supported formats against the format of the file we
	 * were given.
	 */

	rgszPixbufFormats = gdk_pixbuf_format_get_extensions(lppixbufformat);

	bFormatSupported = FALSE;
	for(i = 0; rgszPixbufFormats[i] && !bFormatSupported; i++)
		for(j = 0; rgszPrplFormats[j] && !bFormatSupported; j++)
			if(!g_ascii_strcasecmp(rgszPixbufFormats[i], rgszPrplFormats[j]))
				bFormatSupported = TRUE;

	/* We need to scale iff the prpl requires it and we're out of range. */
	bNeedScale = (lpprplinfo->icon_spec.scale_rules & PURPLE_ICON_SCALE_SEND) &&
		  (cx < lpprplinfo->icon_spec.min_width ||
		   cx > lpprplinfo->icon_spec.max_width ||
		   cy < lpprplinfo->icon_spec.min_height ||
		   cy > lpprplinfo->icon_spec.max_height);

	/* Mangle the image, unless it's already very nice. */
	if(!bFormatSupported || bNeedScale)
	{
		GdkPixbuf *lppixbuf, *lppixbufScaled;
		GError *lpgerror = NULL;

		lppixbuf = gdk_pixbuf_new_from_file(szFilename, &lpgerror);

		if(lppixbuf)
		{
			int cxScaled = cx, cyScaled = cy;
			BOOL bHaveCompression = gdk_pixbuf_major_version > 2 || (gdk_pixbuf_major_version == 2 && gdk_pixbuf_minor_version >= 8);

			/* Get aspect-correct scaled size. */
			purple_buddy_icon_get_scale_size(&lpprplinfo->icon_spec, &cxScaled, &cyScaled);

			lppixbufScaled = gdk_pixbuf_scale_simple(lppixbuf, cxScaled, cyScaled, GDK_INTERP_HYPER);
			g_object_unref(lppixbuf);

			szFilename = NULL;

			/* Attempt each of the supported formats. */
			for(i = 0; rgszPrplFormats[i]; i++)
			{
				FILE *lpfile;

				/* libpurple's routine is less fiddly than
				 * GetTempFileName here.
				 */
				if(!(lpfile = purple_mkstemp(&szTempFile, TRUE)))
					break;
				fclose(lpfile);

				if(bHaveCompression && strcmp(rgszPrplFormats[i], "png") == 0)
				{
					if (gdk_pixbuf_save(lppixbufScaled, szTempFile, rgszPrplFormats[i], &lpgerror, "compression", "9", NULL))
						break;
				}
				else if(gdk_pixbuf_save(lppixbufScaled, szTempFile, rgszPrplFormats[i], &lpgerror, NULL))
					break;

				/* If we get here, we failed. */
				if(lpgerror)
				{
					g_error_free(lpgerror);
					lpgerror = NULL;
				}

				g_unlink(szTempFile);
				g_free(szTempFile);
				szTempFile = NULL;
			}
		}
		else
		{
			g_error_free(lpgerror);
			szFilename = NULL;
		}
	}

	g_strfreev(rgszPixbufFormats);
	g_strfreev(rgszPrplFormats);

	if(szFilename || (szFilename = szTempFile))
		g_file_get_contents(szFilename, &lpcImage, lpcbImage, NULL);

	if(szTempFile)
	{
		g_unlink(szTempFile);
		g_free(szTempFile);
	}

	/* Make sure we're not too big. */
	if((lpprplinfo->icon_spec.max_filesize != 0) && (*lpcbImage > lpprplinfo->icon_spec.max_filesize))
	{
		g_free(lpcImage);
		return NULL;
	}

	return lpcImage;
}


/**
 * Sets the buddy icons for accounts using the global icon, and instructs the
 * UI to show the new icon. Called by the preference hook.
 *
 * @param	szFilename	Filename of buddy icon.
 */
static void SetGlobalBuddyIcon(const gchar *szFilename)
{
	GList *lpglistAccounts;

	for(lpglistAccounts = purple_accounts_get_all(); lpglistAccounts; lpglistAccounts = lpglistAccounts->next)
	{
		PurpleAccount *lpaccount = lpglistAccounts->data;
		PurplePlugin *lpplugin = purple_find_prpl(purple_account_get_protocol_id(lpaccount));

		if(lpplugin)
		{
			PurplePluginProtocolInfo *lpprplinfo = PURPLE_PLUGIN_PROTOCOL_INFO(lpplugin);

			/* Set icon for accounts that don't override the global
			 * setting.
			 */
			if(lpprplinfo && purple_account_get_bool(lpaccount, "use-global-buddyicon", TRUE) && lpprplinfo->icon_spec.format)
			{
				if(szFilename)
				{
					gsize cbImage;
					gpointer lpvImage = ConvertAndScaleBuddyIcon(szFilename, lpplugin, &cbImage);

					purple_buddy_icons_set_account_icon(lpaccount, lpvImage, cbImage);
				}
				else
					purple_buddy_icons_set_account_icon(lpaccount, NULL, 0);

				purple_account_set_buddy_icon_path(lpaccount, szFilename);
			}
		}
	}

	PurpleRefreshBuddyIcon(szFilename);
}


/**
 * Called when the global buddy-icon preference is set.
 *
 * @param	szName		Unused.
 * @param	preftype	Unused.
 * @param	lpvValue	Filename of buddy icon.
 * @param	lpvData		Unused.
 */
void PurpleGlobalBuddyIconPrefChanged(const char *szName, PurplePrefType preftype, gconstpointer lpvValue, gpointer lpvData)
{
	UNREFERENCED_PARAMETER(szName);
	UNREFERENCED_PARAMETER(preftype);
	UNREFERENCED_PARAMETER(lpvData);
	SetGlobalBuddyIcon(lpvValue);
}


/**
 * Posts a new HBITMAP to the UI to display as the buddy icon.
 *
 * @param	szFilename	Filename of buddy icon. May be NULL.
 */
void PurpleRefreshBuddyIcon(const gchar *szFilename)
{
	PurpleStoredImage *lpstoredimg;

	if(szFilename && (lpstoredimg = purple_imgstore_new_from_file(szFilename)))
	{
		/* Make an HBITMAP and send it back to the UI for display in
		 * the main window.
		 */
		HBITMAP hbmIcon = GetBuddyIcon(purple_imgstore_get_data(lpstoredimg), purple_imgstore_get_size(lpstoredimg), 0, 0);
		VulturePostUIMessage(VUIMSG_NEWGLOBALBICON, hbmIcon);
		purple_imgstore_unref(lpstoredimg);
	}
	else
		VulturePostUIMessage(VUIMSG_NEWGLOBALBICON, NULL);
}
