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
	 * File: signals.h
	 */
	public class Signals
	{
		public delegate void PurpleCallback();
		public delegate void PurpleSignalMarshalFunc(IntPtr cb, /* Unresolved: [Category: Unknown], [Name: va_list args] */ IntPtr args, IntPtr data, IntPtr return_val);

		/*
		 * Function Name: purple_signal_unregister
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_signal_unregister(void * instance, char * signal)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_signal_unregister(IntPtr instance, string signal);

		public static void SignalUnregister(IntPtr instance, string signal)
		{
			purple_signal_unregister(instance, signal);
		}

		/*
		 * Function Name: purple_signals_unregister_by_instance
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_signals_unregister_by_instance(void * instance)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_signals_unregister_by_instance(IntPtr instance);

		public static void UnregisterByInstance(IntPtr instance)
		{
			purple_signals_unregister_by_instance(instance);
		}

		/*
		 * Function Name: purple_signals_disconnect_by_handle
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_signals_disconnect_by_handle(void * handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_signals_disconnect_by_handle(IntPtr handle);

		public static void DisconnectByHandle(IntPtr handle)
		{
			purple_signals_disconnect_by_handle(handle);
		}

		/*
		 * Function Name: purple_signal_emit_vargs
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_signal_emit_vargs(void * instance, char * signal, va_list args)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_signal_emit_vargs(IntPtr instance, string signal, /* Unresolved: [Category: Unknown], [Name: va_list args] */ IntPtr args);

		public static void SignalEmitVargs(IntPtr instance, string signal, /* Unresolved: [Category: Unknown], [Name: va_list args] */ IntPtr args)
		{
			/* Unable to process args, a Unknown. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_signals_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_signals_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_signals_init();

		public static void Init()
		{
			purple_signals_init();
		}

		/*
		 * Function Name: purple_signals_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_signals_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_signals_uninit();

		public static void Uninit()
		{
			purple_signals_uninit();
		}

	}
}

