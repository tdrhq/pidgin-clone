/* PurpleWrapper - A .NET (CLR) wrapper for libpurple
 *
 * Purple is the legal property of its developers, whose names are too numerous
 * to list here.  Please refer to the COPYRIGHT file distributed with this
 * source distribution.
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

/*
 * This file was auto-generated from the libpurple header files to provide a
 * clean interface between .NET/CLR and the unmanaged C library libpurple.
 *
 * This is the second major commit of the code.
 * Next things:
 *  - A few of the .h files have anonymous parameter names (eg: void cat(int, int).
 *    This program will need to assign these parameters names.
 *  - Function pointers inside structs aren't translated correctly into C#.
 *  - Two places there are specific-length arrays (eg: char hostname[256]). The parser
 *    does not detect them as an array.
 */

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace PurpleWrapper
{

	/*
	 * File: whiteboard.h
	 */
	public class Whiteboard
	{

		/*
		 * Function Name: purple_whiteboard_set_ui_ops
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_whiteboard_set_ui_ops(PurpleWhiteboardUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_whiteboard_set_ui_ops(IntPtr ops);

		public static void SetUiOps(PurpleWhiteboardUiOps ops)
		{
			purple_whiteboard_set_ui_ops(ops.Reference);
		}

		/*
		 * Function Name: purple_whiteboard_set_prpl_ops
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_whiteboard_set_prpl_ops(PurpleWhiteboard * wb, PurpleWhiteboardPrplOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_whiteboard_set_prpl_ops(IntPtr wb, IntPtr ops);

		public static void SetPrplOps(PurpleWhiteboard wb, PurpleWhiteboardPrplOps ops)
		{
			purple_whiteboard_set_prpl_ops(wb.Reference, ops.Reference);
		}

		/*
		 * Function Name: purple_whiteboard_create
		 * Function Type: PurpleWhiteboard *
		 * Function RawType: PurpleWhiteboard
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleWhiteboard * purple_whiteboard_create(PurpleAccount * account, char * who, int state)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_whiteboard_create(IntPtr account, string who, int state);

		public static PurpleWhiteboard Create(PurpleAccount account, string who, int state)
		{
			return new PurpleWhiteboard(purple_whiteboard_create(account.Reference, who, state));
		}

		/*
		 * Function Name: purple_whiteboard_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_whiteboard_destroy(PurpleWhiteboard * wb)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_whiteboard_destroy(IntPtr wb);

		public static void Destroy(PurpleWhiteboard wb)
		{
			purple_whiteboard_destroy(wb.Reference);
		}

		/*
		 * Function Name: purple_whiteboard_start
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_whiteboard_start(PurpleWhiteboard * wb)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_whiteboard_start(IntPtr wb);

		public static void Start(PurpleWhiteboard wb)
		{
			purple_whiteboard_start(wb.Reference);
		}

		/*
		 * Function Name: purple_whiteboard_get_session
		 * Function Type: PurpleWhiteboard *
		 * Function RawType: PurpleWhiteboard
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleWhiteboard * purple_whiteboard_get_session(PurpleAccount * account, char * who)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_whiteboard_get_session(IntPtr account, string who);

		public static PurpleWhiteboard GetSession(PurpleAccount account, string who)
		{
			return new PurpleWhiteboard(purple_whiteboard_get_session(account.Reference, who));
		}

		/*
		 * Function Name: purple_whiteboard_draw_list_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_whiteboard_draw_list_destroy(GList * draw_list)
		 * 
		 * Could not generate a wrapper for purple_whiteboard_draw_list_destroy in file "whiteboard.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Function Name: purple_whiteboard_get_dimensions
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_whiteboard_get_dimensions(PurpleWhiteboard * wb, int * width, int * height)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_whiteboard_get_dimensions(IntPtr wb, IntPtr width, IntPtr height);

		public static bool GetDimensions(PurpleWhiteboard wb, /* Resolved as PointerToUnknownStruct to int */ IntPtr width, /* Resolved as PointerToUnknownStruct to int */ IntPtr height)
		{
			/* Unable to process width, a PointerToUnknownStruct. */
			/* Unable to process height, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_whiteboard_set_dimensions
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_whiteboard_set_dimensions(PurpleWhiteboard * wb, int width, int height)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_whiteboard_set_dimensions(IntPtr wb, int width, int height);

		public static void SetDimensions(PurpleWhiteboard wb, int width, int height)
		{
			purple_whiteboard_set_dimensions(wb.Reference, width, height);
		}

		/*
		 * Function Name: purple_whiteboard_draw_point
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_whiteboard_draw_point(PurpleWhiteboard * wb, int x, int y, int color, int size)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_whiteboard_draw_point(IntPtr wb, int x, int y, int color, int size);

		public static void DrawPoint(PurpleWhiteboard wb, int x, int y, int color, int size)
		{
			purple_whiteboard_draw_point(wb.Reference, x, y, color, size);
		}

		/*
		 * Function Name: purple_whiteboard_send_draw_list
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_whiteboard_send_draw_list(PurpleWhiteboard * wb, GList * list)
		 * 
		 * Could not generate a wrapper for purple_whiteboard_send_draw_list in file "whiteboard.h".
		 * Message: Unable to provide a type for GList as no function information was provided in code.
		 */

		/*
		 * Function Name: purple_whiteboard_draw_line
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_whiteboard_draw_line(PurpleWhiteboard * wb, int x1, int y1, int x2, int y2, int color, int size)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_whiteboard_draw_line(IntPtr wb, int x1, int y1, int x2, int y2, int color, int size);

		public static void DrawLine(PurpleWhiteboard wb, int x1, int y1, int x2, int y2, int color, int size)
		{
			purple_whiteboard_draw_line(wb.Reference, x1, y1, x2, y2, color, size);
		}

		/*
		 * Function Name: purple_whiteboard_clear
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_whiteboard_clear(PurpleWhiteboard * wb)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_whiteboard_clear(IntPtr wb);

		public static void Clear(PurpleWhiteboard wb)
		{
			purple_whiteboard_clear(wb.Reference);
		}

		/*
		 * Function Name: purple_whiteboard_send_clear
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_whiteboard_send_clear(PurpleWhiteboard * wb)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_whiteboard_send_clear(IntPtr wb);

		public static void SendClear(PurpleWhiteboard wb)
		{
			purple_whiteboard_send_clear(wb.Reference);
		}

		/*
		 * Function Name: purple_whiteboard_send_brush
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_whiteboard_send_brush(PurpleWhiteboard * wb, int size, int color)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_whiteboard_send_brush(IntPtr wb, int size, int color);

		public static void SendBrush(PurpleWhiteboard wb, int size, int color)
		{
			purple_whiteboard_send_brush(wb.Reference, size, color);
		}

		/*
		 * Function Name: purple_whiteboard_get_brush
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_whiteboard_get_brush(PurpleWhiteboard * wb, int * size, int * color)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_whiteboard_get_brush(IntPtr wb, IntPtr size, IntPtr color);

		public static bool GetBrush(PurpleWhiteboard wb, /* Resolved as PointerToUnknownStruct to int */ IntPtr size, /* Resolved as PointerToUnknownStruct to int */ IntPtr color)
		{
			/* Unable to process size, a PointerToUnknownStruct. */
			/* Unable to process color, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_whiteboard_set_brush
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_whiteboard_set_brush(PurpleWhiteboard * wb, int size, int color)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_whiteboard_set_brush(IntPtr wb, int size, int color);

		public static void SetBrush(PurpleWhiteboard wb, int size, int color)
		{
			purple_whiteboard_set_brush(wb.Reference, size, color);
		}

	}
}

