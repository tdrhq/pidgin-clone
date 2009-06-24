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
	public class PurpleXfer : UnmanagedWrapper<_PurpleXfer>
	{
		public PurpleXfer()
		{
		}

		public PurpleXfer(IntPtr refernece)
		{
			this.Reference = reference;
			this.Data = (_PurpleXfer)Marshal.PtrToStructure(this.Reference, typeof(_PurpleXfer));
		}

		public uint ref
		{
			get
			{
				return this.Data.ref;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.ref = value;
			}
		}

		public PurpleXferType type
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public PurpleAccount account
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public string who
		{
			get
			{
				return this.Data.who;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.who = value;
			}
		}

		public string message
		{
			get
			{
				return this.Data.message;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.message = value;
			}
		}

		public string filename
		{
			get
			{
				return this.Data.filename;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.filename = value;
			}
		}

		public string local_filename
		{
			get
			{
				return this.Data.local_filename;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.local_filename = value;
			}
		}

		public size_t size
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public FILE dest_fp
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public string remote_ip
		{
			get
			{
				return this.Data.remote_ip;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.remote_ip = value;
			}
		}

		public int local_port
		{
			get
			{
				return this.Data.local_port;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.local_port = value;
			}
		}

		public int remote_port
		{
			get
			{
				return this.Data.remote_port;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.remote_port = value;
			}
		}

		public int fd
		{
			get
			{
				return this.Data.fd;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.fd = value;
			}
		}

		public int watcher
		{
			get
			{
				return this.Data.watcher;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.watcher = value;
			}
		}

		public size_t bytes_sent
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public size_t bytes_remaining
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public time_t start_time
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public time_t end_time
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public size_t current_buffer_size
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public PurpleXferStatusType status
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public struct {
		void (*init)(PurpleXfer *xfer)
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public void request_denied
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public void start
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public void end
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public void cancel_send
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public void cancel_recv
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public gssize read
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public gssize write
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public void ack
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public PurpleXferUiOps ui_ops
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public IntPtr ui_data
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

		public IntPtr data
		{
			get
			{
				throw new NotImplementedException(); /* Non-native type. */
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				throw new NotImplementedException(); /* Non-native type. */
			}
		}

	}


	[StructLayout(LayoutKind.Sequential)]
	class _PurpleXfer
	{
		/*
		 * guint ref
		 */
		uint ref;

		/*
		 * PurpleXferType type
		 */
		UNKNOWN type;

		/*
		 * PurpleAccount * account
		 */
		IntPtr account;

		/*
		 * char * who
		 */
		string who;

		/*
		 * char * message
		 */
		string message;

		/*
		 * char * filename
		 */
		string filename;

		/*
		 * char * local_filename
		 */
		string local_filename;

		/*
		 * size_t size
		 */
		UNKNOWN size;

		/*
		 * FILE * dest_fp
		 */
		IntPtr dest_fp;

		/*
		 * char * remote_ip
		 */
		string remote_ip;

		/*
		 * int local_port
		 */
		int local_port;

		/*
		 * int remote_port
		 */
		int remote_port;

		/*
		 * int fd
		 */
		int fd;

		/*
		 * int watcher
		 */
		int watcher;

		/*
		 * size_t bytes_sent
		 */
		UNKNOWN bytes_sent;

		/*
		 * size_t bytes_remaining
		 */
		UNKNOWN bytes_remaining;

		/*
		 * time_t start_time
		 */
		UNKNOWN start_time;

		/*
		 * time_t end_time
		 */
		UNKNOWN end_time;

		/*
		 * size_t current_buffer_size
		 */
		UNKNOWN current_buffer_size;

		/*
		 * PurpleXferStatusType status
		 */
		UNKNOWN status;

		/*
		 * struct {
		void (*init)(PurpleXfer *xfer)
		 */
		UNKNOWN {
		void (*init)(PurpleXfer *xfer);

		/*
		 * void (*request_denied)(PurpleXfer * xfer)
		 */
		void request_denied;

		/*
		 * void (*start)(PurpleXfer * xfer)
		 */
		void start;

		/*
		 * void (*end)(PurpleXfer * xfer)
		 */
		void end;

		/*
		 * void (*cancel_send)(PurpleXfer * xfer)
		 */
		void cancel_send;

		/*
		 * void (*cancel_recv)(PurpleXfer * xfer)
		 */
		void cancel_recv;

		/*
		 * gssize (*read)(guchar ** buffer, PurpleXfer * xfer)
		 */
		UNKNOWN read;

		/*
		 * gssize (*write)(guchar * buffer, size_t size, PurpleXfer * xfer)
		 */
		UNKNOWN write;

		/*
		 * void (*ack)(PurpleXfer * xfer, guchar * buffer, size_t size)
		 */
		void ack;

		/*
		 * PurpleXferUiOps * ui_ops
		 */
		IntPtr ui_ops;

		/*
		 * void * ui_data
		 */
		IntPtr ui_data;

		/*
		 * void * data
		 */
		IntPtr data;

	}
}

