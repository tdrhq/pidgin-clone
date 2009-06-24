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
	public class Ft
	{
		/*
		 * PurpleXfer * purple_xfer_new(PurpleAccount * account, PurpleXferType type, char * who)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_xfer_new(IntPtr account, UNKNOWN type, string who);

		public static PurpleXfer XferNew(PurpleAccount account, PurpleXferType type, string who)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_xfers_get_all()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_xfers_get_all();

		public static GList XfersGetAll()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_ref(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_ref(IntPtr xfer);

		public static void XferRef(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_unref(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_unref(IntPtr xfer);

		public static void XferUnref(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_request(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_request(IntPtr xfer);

		public static void XferRequest(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_request_accepted(PurpleXfer * xfer, char * filename)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_request_accepted(IntPtr xfer, string filename);

		public static void XferRequestAccepted(PurpleXfer xfer, string filename)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_request_denied(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_request_denied(IntPtr xfer);

		public static void XferRequestDenied(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleXferType purple_xfer_get_type(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_xfer_get_type(IntPtr xfer);

		public static PurpleXferType XferGetType(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccount * purple_xfer_get_account(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_xfer_get_account(IntPtr xfer);

		public static PurpleAccount XferGetAccount(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_xfer_get_remote_user(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_xfer_get_remote_user(IntPtr xfer);

		public static string XferGetRemoteUser(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleXferStatusType purple_xfer_get_status(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_xfer_get_status(IntPtr xfer);

		public static PurpleXferStatusType XferGetStatus(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_xfer_is_canceled(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_xfer_is_canceled(IntPtr xfer);

		public static bool XferIsCanceled(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_xfer_is_completed(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_xfer_is_completed(IntPtr xfer);

		public static bool XferIsCompleted(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_xfer_get_filename(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_xfer_get_filename(IntPtr xfer);

		public static string XferGetFilename(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_xfer_get_local_filename(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_xfer_get_local_filename(IntPtr xfer);

		public static string XferGetLocalFilename(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * size_t purple_xfer_get_bytes_sent(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_xfer_get_bytes_sent(IntPtr xfer);

		public static size_t XferGetBytesSent(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * size_t purple_xfer_get_bytes_remaining(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_xfer_get_bytes_remaining(IntPtr xfer);

		public static size_t XferGetBytesRemaining(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * size_t purple_xfer_get_size(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_xfer_get_size(IntPtr xfer);

		public static size_t XferGetSize(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * double purple_xfer_get_progress(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern double purple_xfer_get_progress(IntPtr xfer);

		public static double XferGetProgress(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_xfer_get_local_port(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_xfer_get_local_port(IntPtr xfer);

		public static int XferGetLocalPort(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_xfer_get_remote_ip(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_xfer_get_remote_ip(IntPtr xfer);

		public static string XferGetRemoteIp(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * int purple_xfer_get_remote_port(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern int purple_xfer_get_remote_port(IntPtr xfer);

		public static int XferGetRemotePort(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * time_t purple_xfer_get_start_time(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_xfer_get_start_time(IntPtr xfer);

		public static time_t XferGetStartTime(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * time_t purple_xfer_get_end_time(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_xfer_get_end_time(IntPtr xfer);

		public static time_t XferGetEndTime(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_set_completed(PurpleXfer * xfer, gboolean completed)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_completed(IntPtr xfer, bool completed);

		public static void XferSetCompleted(PurpleXfer xfer, bool completed)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_set_message(PurpleXfer * xfer, char * message)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_message(IntPtr xfer, string message);

		public static void XferSetMessage(PurpleXfer xfer, string message)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_set_filename(PurpleXfer * xfer, char * filename)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_filename(IntPtr xfer, string filename);

		public static void XferSetFilename(PurpleXfer xfer, string filename)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_set_local_filename(PurpleXfer * xfer, char * filename)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_local_filename(IntPtr xfer, string filename);

		public static void XferSetLocalFilename(PurpleXfer xfer, string filename)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_set_size(PurpleXfer * xfer, size_t size)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_size(IntPtr xfer, UNKNOWN size);

		public static void XferSetSize(PurpleXfer xfer, size_t size)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_set_bytes_sent(PurpleXfer * xfer, size_t bytes_sent)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_bytes_sent(IntPtr xfer, UNKNOWN bytes_sent);

		public static void XferSetBytesSent(PurpleXfer xfer, size_t bytes_sent)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleXferUiOps * purple_xfer_get_ui_ops(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_xfer_get_ui_ops(IntPtr xfer);

		public static PurpleXferUiOps XferGetUiOps(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_set_read_fnc(PurpleXfer * xfer,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_read_fnc(IntPtr xfer, UNKNOWN );

		public static void XferSetReadFnc(PurpleXfer xfer,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_set_write_fnc(PurpleXfer * xfer,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_write_fnc(IntPtr xfer, UNKNOWN );

		public static void XferSetWriteFnc(PurpleXfer xfer,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_set_ack_fnc(PurpleXfer * xfer,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_ack_fnc(IntPtr xfer, UNKNOWN );

		public static void XferSetAckFnc(PurpleXfer xfer,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_set_request_denied_fnc(PurpleXfer * xfer,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_request_denied_fnc(IntPtr xfer, UNKNOWN );

		public static void XferSetRequestDeniedFnc(PurpleXfer xfer,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_set_init_fnc(PurpleXfer * xfer,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_init_fnc(IntPtr xfer, UNKNOWN );

		public static void XferSetInitFnc(PurpleXfer xfer,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_set_start_fnc(PurpleXfer * xfer,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_start_fnc(IntPtr xfer, UNKNOWN );

		public static void XferSetStartFnc(PurpleXfer xfer,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_set_end_fnc(PurpleXfer * xfer,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_end_fnc(IntPtr xfer, UNKNOWN );

		public static void XferSetEndFnc(PurpleXfer xfer,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_set_cancel_send_fnc(PurpleXfer * xfer,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_cancel_send_fnc(IntPtr xfer, UNKNOWN );

		public static void XferSetCancelSendFnc(PurpleXfer xfer,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_set_cancel_recv_fnc(PurpleXfer * xfer,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_set_cancel_recv_fnc(IntPtr xfer, UNKNOWN );

		public static void XferSetCancelRecvFnc(PurpleXfer xfer,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * gssize purple_xfer_read(PurpleXfer * xfer, guchar ** buffer)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_xfer_read(IntPtr xfer, IntPtr buffer);

		public static gssize XferRead(PurpleXfer xfer, guchar buffer)
		{
			throw new NotImplementedException();
		}

		/*
		 * gssize purple_xfer_write(PurpleXfer * xfer, guchar * buffer, gsize size)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_xfer_write(IntPtr xfer, IntPtr buffer, UNKNOWN size);

		public static gssize XferWrite(PurpleXfer xfer, guchar buffer, gsize size)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_start(PurpleXfer * xfer, int fd, char * ip, unsigned int)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_start(IntPtr xfer, int fd, string ip, UNKNOWN int);

		public static void XferStart(PurpleXfer xfer, int fd, string ip, unsigned int)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_end(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_end(IntPtr xfer);

		public static void XferEnd(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_add(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_add(IntPtr xfer);

		public static void XferAdd(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_cancel_local(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_cancel_local(IntPtr xfer);

		public static void XferCancelLocal(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_cancel_remote(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_cancel_remote(IntPtr xfer);

		public static void XferCancelRemote(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_error(PurpleXferType type, PurpleAccount * account, char * who, char * msg)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_error(UNKNOWN type, IntPtr account, string who, string msg);

		public static void XferError(PurpleXferType type, PurpleAccount account, string who, string msg)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_update_progress(PurpleXfer * xfer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_update_progress(IntPtr xfer);

		public static void XferUpdateProgress(PurpleXfer xfer)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfer_conversation_write(PurpleXfer * xfer, char * message, gboolean is_error)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfer_conversation_write(IntPtr xfer, string message, bool is_error);

		public static void XferConversationWrite(PurpleXfer xfer, string message, bool is_error)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_xfers_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_xfers_get_handle();

		public static IntPtr XfersGetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfers_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfers_init();

		public static void XfersInit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfers_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfers_uninit();

		public static void XfersUninit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_xfers_set_ui_ops(PurpleXferUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_xfers_set_ui_ops(IntPtr ops);

		public static void XfersSetUiOps(PurpleXferUiOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleXferUiOps * purple_xfers_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_xfers_get_ui_ops();

		public static PurpleXferUiOps XfersGetUiOps()
		{
			throw new NotImplementedException();
		}

	}
}

