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



/**
 * Retrieves the buddy icon for a buddy.
 *
 * @param	lpbuddy		Buddy.
 *
 * @return Bitmap handle, or NULL on error.
 */
HBITMAP PurpleGetBuddyIcon(PurpleBuddy *lpbuddy)
{
	GdkPixbuf *lppixbuf;
	GdkPixbufLoader *lppbloader;
	PurpleBuddyIcon *lpbuddyicon;
	gconstpointer lpvBuddyIconData;
	size_t cbBuddyIconData;
	guchar *lpucPixbufPixels;
	int cbPixbufRowstride, cbDIBRowstride;
	int cx, cy;
	HDC hdc;
	HBITMAP hbitmap;
	BITMAPINFO bmi;
	BYTE *lpbyBits;
	int iRow;
	int iChannels, iBitsPerSample;

	lpbuddyicon = purple_buddy_icons_find(lpbuddy->account, lpbuddy->name);

	if(!lpbuddyicon)
		return NULL;

	lpvBuddyIconData = purple_buddy_icon_get_data(lpbuddyicon, &cbBuddyIconData);

	lppbloader = gdk_pixbuf_loader_new();
	gdk_pixbuf_loader_write(lppbloader, lpvBuddyIconData, cbBuddyIconData, NULL);
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
	cbDIBRowstride = (4 * cx * sizeof(DWORD) + 3) / 4;

	lpbyBits = ProcHeapAlloc(cbDIBRowstride * cy);

	/* Bottom-up DIB. */
	for(iRow = 0; iRow < cy; iRow++)
	{
		int cbDIBRowOffest = (cy - iRow) * cbDIBRowstride;
		int cbPixbufRowOffest = (cy - iRow) * cbDIBRowstride;
		int iCol;

		for(iCol = 0; iCol < cx; iCol++)
		{
			int iChan;

			/* DIB is 0x(AA)RRGGBB; pixbuf is 0x(AA)BBGGRR (both
			 * little-endian).
			 */

			/* Blue, green, red. */
			for(iChan = 0; iChan < 3; iChan++)
				lpbyBits[cbDIBRowOffest + iChan] = lpucPixbufPixels[cbPixbufRowOffest + (2 - iChan)];

			if(iChannels == 4)
				lpbyBits[cbDIBRowOffest + 3] = lpucPixbufPixels[cbPixbufRowOffest + 3];
		}
	}

	g_object_unref(lppixbuf);

	hdc = GetDC(NULL);
	hbitmap = CreateDIBitmap(hdc, &bmi.bmiHeader, CBM_INIT, lpbyBits, &bmi, DIB_RGB_COLORS);
	ReleaseDC(NULL, hdc);

	ProcHeapFree(lpbyBits);

	return hbitmap;
}
