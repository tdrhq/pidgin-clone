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
	public class Whiteboard
	{
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
		 * void purple_whiteboard_set_prpl_ops(PurpleWhiteboard * wb, PurpleWhiteboardPrplOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_whiteboard_set_prpl_ops(IntPtr wb, IntPtr ops);

		public static void SetPrplOps(PurpleWhiteboard wb, PurpleWhiteboardPrplOps ops)
		{
			purple_whiteboard_set_prpl_ops(wb.Reference, ops.Reference);
		}

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
		 * void purple_whiteboard_destroy(PurpleWhiteboard * wb)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_whiteboard_destroy(IntPtr wb);

		public static void Destroy(PurpleWhiteboard wb)
		{
			purple_whiteboard_destroy(wb.Reference);
		}

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
		 * PurpleWhiteboard * purple_whiteboard_get_session(PurpleAccount * account, char * who)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_whiteboard_get_session(IntPtr account, string who);

		public static PurpleWhiteboard GetSession(PurpleAccount account, string who)
		{
			return new PurpleWhiteboard(purple_whiteboard_get_session(account.Reference, who));
		}

		/*
		 * void purple_whiteboard_draw_list_destroy(GList * draw_list)
		 * 
		 * Could not generate a wrapper for purple_whiteboard_draw_list_destroy in file "whiteboard.h".
		 * Message: The type could not be resolved (GList * draw_list).
		 */

		/*
		 * gboolean purple_whiteboard_get_dimensions(PurpleWhiteboard * wb, int * width, int * height)
		 * 
		 * Could not generate a wrapper for purple_whiteboard_get_dimensions in file "whiteboard.h".
		 * Message: The type could not be resolved (int * width).
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
		 * void purple_whiteboard_draw_point(PurpleWhiteboard * wb, int x, int y, int color, int size)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_whiteboard_draw_point(IntPtr wb, int x, int y, int color, int size);

		public static void DrawPoint(PurpleWhiteboard wb, int x, int y, int color, int size)
		{
			purple_whiteboard_draw_point(wb.Reference, x, y, color, size);
		}

		/*
		 * void purple_whiteboard_send_draw_list(PurpleWhiteboard * wb, GList * list)
		 * 
		 * Could not generate a wrapper for purple_whiteboard_send_draw_list in file "whiteboard.h".
		 * Message: The type could not be resolved (GList * list).
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
		 * void purple_whiteboard_clear(PurpleWhiteboard * wb)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_whiteboard_clear(IntPtr wb);

		public static void Clear(PurpleWhiteboard wb)
		{
			purple_whiteboard_clear(wb.Reference);
		}

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
		 * void purple_whiteboard_send_brush(PurpleWhiteboard * wb, int size, int color)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_whiteboard_send_brush(IntPtr wb, int size, int color);

		public static void SendBrush(PurpleWhiteboard wb, int size, int color)
		{
			purple_whiteboard_send_brush(wb.Reference, size, color);
		}

		/*
		 * gboolean purple_whiteboard_get_brush(PurpleWhiteboard * wb, int * size, int * color)
		 * 
		 * Could not generate a wrapper for purple_whiteboard_get_brush in file "whiteboard.h".
		 * Message: The type could not be resolved (int * size).
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

