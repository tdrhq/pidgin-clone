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
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "vulture.h"
#include "purple.h"
#include "purplebicon.h"


static HBITMAP GetBuddyIcon(gconstpointer lpvBuddyIconData, size_t cbBuddyIconData, int cxMax, int cyMax);



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
	PurpleStoredImage *lpstoredimg;
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
