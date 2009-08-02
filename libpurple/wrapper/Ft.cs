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
	 * File: ft.h
	 */
	public class Ft
	{
		public enum PurpleXferType
		{
			PURPLE_XFER_UNKNOWN = 0,
			PURPLE_XFER_SEND,
			PURPLE_XFER_RECEIVE
		};

		public enum PurpleXferStatusType
		{
			PURPLE_XFER_STATUS_UNKNOWN = 0,
			PURPLE_XFER_STATUS_NOT_STARTED,
			PURPLE_XFER_STATUS_ACCEPTED,
			PURPLE_XFER_STATUS_STARTED,
			PURPLE_XFER_STATUS_DONE,
			PURPLE_XFER_STATUS_CANCEL_LOCAL,
			PURPLE_XFER_STATUS_CANCEL_REMOTE
		};


		/*
		 * Function Name: purple_xfers_get_all
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_xfers_get_all()
		 * 
		 * Could not generate a wrapper for purple_xfers_get_all in file "ft.h".
		 * Message: Unknown inner-type of argument 0 of purple_xfers_get_all
		 */

		/*
		 * Function Name: purple_xfer_ref
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_ref(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_ref(IntPtr xfer);

		public static void XferRef(PurpleXfer xfer)
		{
			purple_xfer_ref(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_unref
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_unref(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_unref(IntPtr xfer);

		public static void XferUnref(PurpleXfer xfer)
		{
			purple_xfer_unref(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_request
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_request(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_request(IntPtr xfer);

		public static void XferRequest(PurpleXfer xfer)
		{
			purple_xfer_request(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_request_accepted
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_request_accepted(PurpleXfer * xfer, char * filename)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_request_accepted(IntPtr xfer, string filename);

		public static void XferRequestAccepted(PurpleXfer xfer, string filename)
		{
			purple_xfer_request_accepted(xfer.Reference, filename);
		}

		/*
		 * Function Name: purple_xfer_request_denied
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_request_denied(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_request_denied(IntPtr xfer);

		public static void XferRequestDenied(PurpleXfer xfer)
		{
			purple_xfer_request_denied(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_get_type
		 * Function Type: PurpleXferType
		 * Function RawType: PurpleXferType
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleXferType purple_xfer_get_type(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern Ft.PurpleXferType purple_xfer_get_type(IntPtr xfer);

		public static Ft.PurpleXferType XferGetType(PurpleXfer xfer)
		{
			/* Unable to process the return value of purple_xfer_get_type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_xfer_get_account
		 * Function Type: PurpleAccount *
		 * Function RawType: PurpleAccount
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleAccount * purple_xfer_get_account(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_xfer_get_account(IntPtr xfer);

		public static PurpleAccount XferGetAccount(PurpleXfer xfer)
		{
			return new PurpleAccount(purple_xfer_get_account(xfer.Reference));
		}

		/*
		 * Function Name: purple_xfer_get_remote_user
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_xfer_get_remote_user(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_xfer_get_remote_user(IntPtr xfer);

		public static string XferGetRemoteUser(PurpleXfer xfer)
		{
			return purple_xfer_get_remote_user(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_get_status
		 * Function Type: PurpleXferStatusType
		 * Function RawType: PurpleXferStatusType
		 * Function Category: KnownEnum
		 */
		/*
		 * PurpleXferStatusType purple_xfer_get_status(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern Ft.PurpleXferStatusType purple_xfer_get_status(IntPtr xfer);

		public static Ft.PurpleXferStatusType XferGetStatus(PurpleXfer xfer)
		{
			/* Unable to process the return value of purple_xfer_get_status, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_xfer_is_canceled
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_xfer_is_canceled(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_xfer_is_canceled(IntPtr xfer);

		public static bool XferIsCanceled(PurpleXfer xfer)
		{
			return purple_xfer_is_canceled(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_is_completed
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_xfer_is_completed(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_xfer_is_completed(IntPtr xfer);

		public static bool XferIsCompleted(PurpleXfer xfer)
		{
			return purple_xfer_is_completed(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_get_filename
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_xfer_get_filename(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_xfer_get_filename(IntPtr xfer);

		public static string XferGetFilename(PurpleXfer xfer)
		{
			return purple_xfer_get_filename(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_get_local_filename
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_xfer_get_local_filename(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_xfer_get_local_filename(IntPtr xfer);

		public static string XferGetLocalFilename(PurpleXfer xfer)
		{
			return purple_xfer_get_local_filename(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_get_bytes_sent
		 * Function Type: size_t
		 * Function RawType: size_t
		 * Function Category: Native
		 */
		/*
		 * size_t purple_xfer_get_bytes_sent(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_xfer_get_bytes_sent(IntPtr xfer);

		public static ulong XferGetBytesSent(PurpleXfer xfer)
		{
			return purple_xfer_get_bytes_sent(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_get_bytes_remaining
		 * Function Type: size_t
		 * Function RawType: size_t
		 * Function Category: Native
		 */
		/*
		 * size_t purple_xfer_get_bytes_remaining(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_xfer_get_bytes_remaining(IntPtr xfer);

		public static ulong XferGetBytesRemaining(PurpleXfer xfer)
		{
			return purple_xfer_get_bytes_remaining(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_get_size
		 * Function Type: size_t
		 * Function RawType: size_t
		 * Function Category: Native
		 */
		/*
		 * size_t purple_xfer_get_size(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_xfer_get_size(IntPtr xfer);

		public static ulong XferGetSize(PurpleXfer xfer)
		{
			return purple_xfer_get_size(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_get_progress
		 * Function Type: double
		 * Function RawType: double
		 * Function Category: Native
		 */
		/*
		 * double purple_xfer_get_progress(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern double purple_xfer_get_progress(IntPtr xfer);

		public static double XferGetProgress(PurpleXfer xfer)
		{
			return purple_xfer_get_progress(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_get_local_port
		 * Function Type: unsigned int
		 * Function RawType: int
		 * Function Category: Native
		 */
		/*
		 * unsigned int purple_xfer_get_local_port(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_xfer_get_local_port(IntPtr xfer);

		public static uint XferGetLocalPort(PurpleXfer xfer)
		{
			return purple_xfer_get_local_port(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_get_remote_ip
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_xfer_get_remote_ip(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_xfer_get_remote_ip(IntPtr xfer);

		public static string XferGetRemoteIp(PurpleXfer xfer)
		{
			return purple_xfer_get_remote_ip(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_get_remote_port
		 * Function Type: unsigned int
		 * Function RawType: int
		 * Function Category: Native
		 */
		/*
		 * unsigned int purple_xfer_get_remote_port(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern uint purple_xfer_get_remote_port(IntPtr xfer);

		public static uint XferGetRemotePort(PurpleXfer xfer)
		{
			return purple_xfer_get_remote_port(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_get_start_time
		 * Function Type: time_t
		 * Function RawType: time_t
		 * Function Category: DateTime
		 */
		/*
		 * time_t purple_xfer_get_start_time(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_xfer_get_start_time(IntPtr xfer);

		public static DateTime XferGetStartTime(PurpleXfer xfer)
		{
			/* Unable to process the return value of purple_xfer_get_start_time, a DateTime. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_xfer_get_end_time
		 * Function Type: time_t
		 * Function RawType: time_t
		 * Function Category: DateTime
		 */
		/*
		 * time_t purple_xfer_get_end_time(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern ulong purple_xfer_get_end_time(IntPtr xfer);

		public static DateTime XferGetEndTime(PurpleXfer xfer)
		{
			/* Unable to process the return value of purple_xfer_get_end_time, a DateTime. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_xfer_set_completed
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_set_completed(PurpleXfer * xfer, gboolean completed)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_completed(IntPtr xfer, bool completed);

		public static void XferSetCompleted(PurpleXfer xfer, bool completed)
		{
			purple_xfer_set_completed(xfer.Reference, completed);
		}

		/*
		 * Function Name: purple_xfer_set_message
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_set_message(PurpleXfer * xfer, char * message)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_message(IntPtr xfer, string message);

		public static void XferSetMessage(PurpleXfer xfer, string message)
		{
			purple_xfer_set_message(xfer.Reference, message);
		}

		/*
		 * Function Name: purple_xfer_set_filename
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_set_filename(PurpleXfer * xfer, char * filename)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_filename(IntPtr xfer, string filename);

		public static void XferSetFilename(PurpleXfer xfer, string filename)
		{
			purple_xfer_set_filename(xfer.Reference, filename);
		}

		/*
		 * Function Name: purple_xfer_set_local_filename
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_set_local_filename(PurpleXfer * xfer, char * filename)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_local_filename(IntPtr xfer, string filename);

		public static void XferSetLocalFilename(PurpleXfer xfer, string filename)
		{
			purple_xfer_set_local_filename(xfer.Reference, filename);
		}

		/*
		 * Function Name: purple_xfer_set_size
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_set_size(PurpleXfer * xfer, size_t size)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_size(IntPtr xfer, ulong size);

		public static void XferSetSize(PurpleXfer xfer, ulong size)
		{
			purple_xfer_set_size(xfer.Reference, size);
		}

		/*
		 * Function Name: purple_xfer_set_bytes_sent
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_set_bytes_sent(PurpleXfer * xfer, size_t bytes_sent)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_bytes_sent(IntPtr xfer, ulong bytes_sent);

		public static void XferSetBytesSent(PurpleXfer xfer, ulong bytes_sent)
		{
			purple_xfer_set_bytes_sent(xfer.Reference, bytes_sent);
		}

		/*
		 * Function Name: purple_xfer_get_ui_ops
		 * Function Type: PurpleXferUiOps *
		 * Function RawType: PurpleXferUiOps
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleXferUiOps * purple_xfer_get_ui_ops(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_xfer_get_ui_ops(IntPtr xfer);

		public static /* Resolved as PointerToUnknownStruct to PurpleXferUiOps */ IntPtr XferGetUiOps(PurpleXfer xfer)
		{
			/* Unable to process the return value of purple_xfer_get_ui_ops, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_xfer_read
		 * Function Type: gssize
		 * Function RawType: gssize
		 * Function Category: Native
		 */
		/*
		 * gssize purple_xfer_read(PurpleXfer * xfer, guchar ** buffer)
		 */
		[DllImport("libpurple.dll")]
		private static extern long purple_xfer_read(IntPtr xfer, IntPtr buffer);

		public static long XferRead(PurpleXfer xfer, /* Resolved as PointerToUnknownStruct to guchar */ IntPtr buffer)
		{
			/* Unable to process buffer, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_xfer_write
		 * Function Type: gssize
		 * Function RawType: gssize
		 * Function Category: Native
		 */
		/*
		 * gssize purple_xfer_write(PurpleXfer * xfer, guchar * buffer, gsize size)
		 */
		[DllImport("libpurple.dll")]
		private static extern long purple_xfer_write(IntPtr xfer, IntPtr buffer, ulong size);

		public static long XferWrite(PurpleXfer xfer, /* Resolved as PointerToUnknownStruct to guchar */ IntPtr buffer, ulong size)
		{
			/* Unable to process buffer, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_xfer_end
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_end(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_end(IntPtr xfer);

		public static void XferEnd(PurpleXfer xfer)
		{
			purple_xfer_end(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_add
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_add(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_add(IntPtr xfer);

		public static void XferAdd(PurpleXfer xfer)
		{
			purple_xfer_add(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_cancel_local
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_cancel_local(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_cancel_local(IntPtr xfer);

		public static void XferCancelLocal(PurpleXfer xfer)
		{
			purple_xfer_cancel_local(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_cancel_remote
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_cancel_remote(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_cancel_remote(IntPtr xfer);

		public static void XferCancelRemote(PurpleXfer xfer)
		{
			purple_xfer_cancel_remote(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_error
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_error(PurpleXferType type, PurpleAccount * account, char * who, char * msg)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_error(Ft.PurpleXferType type, IntPtr account, string who, string msg);

		public static void XferError(Ft.PurpleXferType type, PurpleAccount account, string who, string msg)
		{
			/* Unable to process type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_xfer_update_progress
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_update_progress(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_update_progress(IntPtr xfer);

		public static void XferUpdateProgress(PurpleXfer xfer)
		{
			purple_xfer_update_progress(xfer.Reference);
		}

		/*
		 * Function Name: purple_xfer_conversation_write
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfer_conversation_write(PurpleXfer * xfer, char * message, gboolean is_error)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_conversation_write(IntPtr xfer, string message, bool is_error);

		public static void XferConversationWrite(PurpleXfer xfer, string message, bool is_error)
		{
			purple_xfer_conversation_write(xfer.Reference, message, is_error);
		}

		/*
		 * Function Name: purple_xfers_get_handle
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
		 */
		/*
		 * void * purple_xfers_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_xfers_get_handle();

		public static IntPtr XfersGetHandle()
		{
			return purple_xfers_get_handle();
		}

		/*
		 * Function Name: purple_xfers_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfers_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfers_init();

		public static void XfersInit()
		{
			purple_xfers_init();
		}

		/*
		 * Function Name: purple_xfers_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfers_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfers_uninit();

		public static void XfersUninit()
		{
			purple_xfers_uninit();
		}

		/*
		 * Function Name: purple_xfers_set_ui_ops
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_xfers_set_ui_ops(PurpleXferUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfers_set_ui_ops(IntPtr ops);

		public static void XfersSetUiOps(/* Resolved as PointerToUnknownStruct to PurpleXferUiOps */ IntPtr ops)
		{
			/* Unable to process ops, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_xfers_get_ui_ops
		 * Function Type: PurpleXferUiOps *
		 * Function RawType: PurpleXferUiOps
		 * Function Category: PointerToUnknownStruct
		 */
		/*
		 * PurpleXferUiOps * purple_xfers_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_xfers_get_ui_ops();

		public static /* Resolved as PointerToUnknownStruct to PurpleXferUiOps */ IntPtr XfersGetUiOps()
		{
			/* Unable to process the return value of purple_xfers_get_ui_ops, a PointerToUnknownStruct. */
			throw new NotImplementedException();
		}

	}
}

