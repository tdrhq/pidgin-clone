/* purple
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
 * clean interface between .NET/CLR and the unmanaged C library, libpurple.
 *
 * This code isn't complete, but completely a work in progress. :)
 * Three major things left:
 *  - Resolve the remaining UNKNOWN types.
 *  - Handle translation between delegate and function pointers.
 *  - Fill in the translation between public .NET class calls and private DllImport[] calls.
 */

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace PurpleWrapper
{
	public class Signals
	{
		/*
		 * gulong purple_signal_register(void * instance, char * signal, PurpleSignalMarshalFunc marshal, PurpleValue * ret_value, int num_values, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_signal_register(IntPtr instance, string signal, UNKNOWN marshal, IntPtr ret_value, int num_values, ...);

		public static ulong SignalRegister(IntPtr instance, string signal, PurpleSignalMarshalFunc marshal, PurpleValue ret_value, int num_values, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_signal_unregister(void * instance, char * signal)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_signal_unregister(IntPtr instance, string signal);

		public static void SignalUnregister(IntPtr instance, string signal)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_signals_unregister_by_instance(void * instance)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_signals_unregister_by_instance(IntPtr instance);

		public static void UnregisterByInstance(IntPtr instance)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_signal_get_values(void * instance, char * signal, PurpleValue ** ret_value, int * num_values, PurpleValue *** values)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_signal_get_values(IntPtr instance, string signal, IntPtr ret_value, IntPtr num_values, IntPtr values);

		public static void SignalGetValues(IntPtr instance, string signal, PurpleValue ret_value, int num_values, PurpleValue values)
		{
			throw new NotImplementedException();
		}

		/*
		 * gulong purple_signal_connect_priority(void * instance, char * signal, void * handle, PurpleCallback func, void * data, int priority)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_signal_connect_priority(IntPtr instance, string signal, IntPtr handle, UNKNOWN func, IntPtr data, int priority);

		public static ulong SignalConnectPriority(IntPtr instance, string signal, IntPtr handle, PurpleCallback func, IntPtr data, int priority)
		{
			throw new NotImplementedException();
		}

		/*
		 * gulong purple_signal_connect(void * instance, char * signal, void * handle, PurpleCallback func, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_signal_connect(IntPtr instance, string signal, IntPtr handle, UNKNOWN func, IntPtr data);

		public static ulong SignalConnect(IntPtr instance, string signal, IntPtr handle, PurpleCallback func, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * gulong purple_signal_connect_priority_vargs(void * instance, char * signal, void * handle, PurpleCallback func, void * data, int priority)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_signal_connect_priority_vargs(IntPtr instance, string signal, IntPtr handle, UNKNOWN func, IntPtr data, int priority);

		public static ulong SignalConnectPriorityVargs(IntPtr instance, string signal, IntPtr handle, PurpleCallback func, IntPtr data, int priority)
		{
			throw new NotImplementedException();
		}

		/*
		 * gulong purple_signal_connect_vargs(void * instance, char * signal, void * handle, PurpleCallback func, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_signal_connect_vargs(IntPtr instance, string signal, IntPtr handle, UNKNOWN func, IntPtr data);

		public static ulong SignalConnectVargs(IntPtr instance, string signal, IntPtr handle, PurpleCallback func, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_signal_disconnect(void * instance, char * signal, void * handle, PurpleCallback func)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_signal_disconnect(IntPtr instance, string signal, IntPtr handle, UNKNOWN func);

		public static void SignalDisconnect(IntPtr instance, string signal, IntPtr handle, PurpleCallback func)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_signals_disconnect_by_handle(void * handle)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_signals_disconnect_by_handle(IntPtr handle);

		public static void DisconnectByHandle(IntPtr handle)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_signal_emit(void * instance, char * signal, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_signal_emit(IntPtr instance, string signal, ...);

		public static void SignalEmit(IntPtr instance, string signal, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_signal_emit_vargs(void * instance, char * signal, va_list args)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_signal_emit_vargs(IntPtr instance, string signal, UNKNOWN args);

		public static void SignalEmitVargs(IntPtr instance, string signal, va_list args)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_signal_emit_return_1(void * instance, char * signal, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_signal_emit_return_1(IntPtr instance, string signal, ...);

		public static IntPtr SignalEmitReturn1(IntPtr instance, string signal, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_signal_emit_vargs_return_1(void * instance, char * signal, va_list args)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_signal_emit_vargs_return_1(IntPtr instance, string signal, UNKNOWN args);

		public static IntPtr SignalEmitVargsReturn1(IntPtr instance, string signal, va_list args)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_signals_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_signals_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_signals_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_signals_uninit();

		public static void Uninit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_VOID(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_VOID(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalVoid(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_VOID__INT(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_VOID__INT(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalVoid_int(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_VOID__INT_INT(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_VOID__INT_INT(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalVoid_intInt(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_VOID__POINTER(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_VOID__POINTER(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalVoid_pointer(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_VOID__POINTER_UINT(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_VOID__POINTER_UINT(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalVoid_pointerUint(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_VOID__POINTER_INT_INT(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_VOID__POINTER_INT_INT(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalVoid_pointerIntInt(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_VOID__POINTER_INT_POINTER(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_VOID__POINTER_INT_POINTER(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalVoid_pointerIntPointer(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_VOID__POINTER_POINTER(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_VOID__POINTER_POINTER(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalVoid_pointerPointer(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_VOID__POINTER_POINTER_UINT(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_VOID__POINTER_POINTER_UINT(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalVoid_pointerPointerUint(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_VOID__POINTER_POINTER_UINT_UINT(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_VOID__POINTER_POINTER_UINT_UINT(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalVoid_pointerPointerUintUint(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_VOID__POINTER_POINTER_POINTER(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_VOID__POINTER_POINTER_POINTER(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalVoid_pointerPointerPointer(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_VOID__POINTER_POINTER_POINTER_POINTER(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_VOID__POINTER_POINTER_POINTER_POINTER(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalVoid_pointerPointerPointerPointer(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_VOID__POINTER_POINTER_POINTER_POINTER_POINTER(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_VOID__POINTER_POINTER_POINTER_POINTER_POINTER(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalVoid_pointerPointerPointerPointerPointer(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_VOID__POINTER_POINTER_POINTER_UINT(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_VOID__POINTER_POINTER_POINTER_UINT(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalVoid_pointerPointerPointerUint(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_VOID__POINTER_POINTER_POINTER_POINTER_UINT(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_VOID__POINTER_POINTER_POINTER_POINTER_UINT(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalVoid_pointerPointerPointerPointerUint(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_VOID__POINTER_POINTER_POINTER_UINT_UINT(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_VOID__POINTER_POINTER_POINTER_UINT_UINT(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalVoid_pointerPointerPointerUintUint(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_INT__INT(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_INT__INT(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalInt_int(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_INT__INT_INT(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_INT__INT_INT(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalInt_intInt(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_INT__POINTER_POINTER(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_INT__POINTER_POINTER(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalInt_pointerPointer(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_INT__POINTER_POINTER_POINTER_POINTER_POINTER(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_INT__POINTER_POINTER_POINTER_POINTER_POINTER(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalInt_pointerPointerPointerPointerPointer(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_BOOLEAN__POINTER(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_BOOLEAN__POINTER(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalBoolean_pointer(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_BOOLEAN__POINTER_POINTER(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_BOOLEAN__POINTER_POINTER(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalBoolean_pointerPointer(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_BOOLEAN__POINTER_POINTER_POINTER(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_BOOLEAN__POINTER_POINTER_POINTER(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalBoolean_pointerPointerPointer(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_BOOLEAN__POINTER_POINTER_UINT(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_BOOLEAN__POINTER_POINTER_UINT(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalBoolean_pointerPointerUint(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_BOOLEAN__POINTER_POINTER_POINTER_UINT(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_BOOLEAN__POINTER_POINTER_POINTER_UINT(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalBoolean_pointerPointerPointerUint(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_BOOLEAN__POINTER_POINTER_POINTER_POINTER(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_BOOLEAN__POINTER_POINTER_POINTER_POINTER(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalBoolean_pointerPointerPointerPointer(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_BOOLEAN__POINTER_POINTER_POINTER_POINTER_POINTER(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_BOOLEAN__POINTER_POINTER_POINTER_POINTER_POINTER(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalBoolean_pointerPointerPointerPointerPointer(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_BOOLEAN__POINTER_POINTER_POINTER_POINTER_UINT(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_BOOLEAN__POINTER_POINTER_POINTER_POINTER_UINT(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalBoolean_pointerPointerPointerPointerUint(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_BOOLEAN__POINTER_POINTER_POINTER_POINTER_POINTER_POINTER(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_BOOLEAN__POINTER_POINTER_POINTER_POINTER_POINTER_POINTER(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalBoolean_pointerPointerPointerPointerPointerPointer(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_BOOLEAN__INT_POINTER(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_BOOLEAN__INT_POINTER(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalBoolean_intPointer(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_POINTER__POINTER_INT(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_POINTER__POINTER_INT(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalPointer_pointerInt(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_POINTER__POINTER_INT64(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_POINTER__POINTER_INT64(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalPointer_pointerInt64(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_POINTER__POINTER_INT_BOOLEAN(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_POINTER__POINTER_INT_BOOLEAN(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalPointer_pointerIntBoolean(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_POINTER__POINTER_INT64_BOOLEAN(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_POINTER__POINTER_INT64_BOOLEAN(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalPointer_pointerInt64Boolean(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_marshal_POINTER__POINTER_POINTER(PurpleCallback cb, va_list args, void * data, void ** return_val)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_marshal_POINTER__POINTER_POINTER(UNKNOWN cb, UNKNOWN args, IntPtr data, IntPtr return_val);

		public static void MarshalPointer_pointerPointer(PurpleCallback cb, va_list args, IntPtr data, void return_val)
		{
			throw new NotImplementedException();
		}

	}
}

