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
	 * Structure: PurpleXfer
	 */
	public class PurpleXfer : UnmanagedWrapper<_PurpleXfer>
	{
		public PurpleXfer()
		{
		}

		public PurpleXfer(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleXfer)Marshal.PtrToStructure(this.Reference, typeof(_PurpleXfer));
		}

		/*
		 * Argument Name: ref
		 * Argument Type: guint
		 * Argument Category: Native
		 */
		public uint ref_
		{
			get
			{
				return this.Data.ref_;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.ref_ = value;
			}
		}

		/*
		 * Argument Name: type
		 * Argument Type: PurpleXferType
		 * Argument Category: KnownEnum
		 */
		public Ft.PurpleXferType type
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

		/*
		 * Argument Name: account
		 * Argument Type: PurpleAccount *
		 * Argument Category: PointerToKnownStruct
		 */
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

		/*
		 * Argument Name: who
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: message
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: filename
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: local_filename
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: size
		 * Argument Type: size_t
		 * Argument Category: Native
		 */
		public ulong size
		{
			get
			{
				return this.Data.size;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.size = value;
			}
		}

		/*
		 * Argument Name: dest_fp
		 * Argument Type: FILE *
		 * Argument Category: PointerToUnknownStruct
		 */
		public /* Resolved as PointerToUnknownStruct to FILE */ IntPtr dest_fp
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

		/*
		 * Argument Name: remote_ip
		 * Argument Type: char *
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: local_port
		 * Argument Type: int
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: remote_port
		 * Argument Type: int
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: fd
		 * Argument Type: int
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: watcher
		 * Argument Type: int
		 * Argument Category: Native
		 */
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

		/*
		 * Argument Name: bytes_sent
		 * Argument Type: size_t
		 * Argument Category: Native
		 */
		public ulong bytes_sent
		{
			get
			{
				return this.Data.bytes_sent;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.bytes_sent = value;
			}
		}

		/*
		 * Argument Name: bytes_remaining
		 * Argument Type: size_t
		 * Argument Category: Native
		 */
		public ulong bytes_remaining
		{
			get
			{
				return this.Data.bytes_remaining;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.bytes_remaining = value;
			}
		}

		/*
		 * Argument Name: start_time
		 * Argument Type: time_t
		 * Argument Category: DateTime
		 */
		public DateTime start_time
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

		/*
		 * Argument Name: end_time
		 * Argument Type: time_t
		 * Argument Category: DateTime
		 */
		public DateTime end_time
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

		/*
		 * Argument Name: current_buffer_size
		 * Argument Type: size_t
		 * Argument Category: Native
		 */
		public ulong current_buffer_size
		{
			get
			{
				return this.Data.current_buffer_size;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.current_buffer_size = value;
			}
		}

		/*
		 * Argument Name: status
		 * Argument Type: PurpleXferStatusType
		 * Argument Category: KnownEnum
		 */
		public Ft.PurpleXferStatusType status
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

		/*
		 * Argument Name: {
		void (*init)(PurpleXfer *xfer)
		 * Argument Type: struct
		 * Argument Category: InnerStruct
		 */
		/*
		 * Could not generate a wrapper for PurpleXfer.{
		void (*init)(PurpleXfer *xfer) in file "ft.h".
		 * Message: The struct contains an inner-struct or inner-union and cannot be automatically wrapped.
		 */

		/*
		 * Argument Name: request_denied
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr request_denied
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

		/*
		 * Argument Name: start
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr start
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

		/*
		 * Argument Name: end
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr end
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

		/*
		 * Argument Name: cancel_send
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr cancel_send
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

		/*
		 * Argument Name: cancel_recv
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr cancel_recv
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

		/*
		 * Argument Name: read
		 * Argument Type: gssize
		 * Argument Category: Native
		 */
		public IntPtr read
		{
			get
			{
				return this.Data.read;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.read = value;
			}
		}

		/*
		 * Argument Name: write
		 * Argument Type: gssize
		 * Argument Category: Native
		 */
		public IntPtr write
		{
			get
			{
				return this.Data.write;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.write = value;
			}
		}

		/*
		 * Argument Name: ack
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr ack
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
	public class _PurpleXfer
	{
		/*
		 * guint ref
		 */
		public uint ref_;

		/*
		 * PurpleXferType type
		 */
		public Ft.PurpleXferType type;

		/*
		 * PurpleAccount * account
		 */
		public IntPtr account;

		/*
		 * char * who
		 */
		public string who;

		/*
		 * char * message
		 */
		public string message;

		/*
		 * char * filename
		 */
		public string filename;

		/*
		 * char * local_filename
		 */
		public string local_filename;

		/*
		 * size_t size
		 */
		public ulong size;

		/*
		 * FILE * dest_fp
		 */
		public IntPtr dest_fp;

		/*
		 * char * remote_ip
		 */
		public string remote_ip;

		/*
		 * int local_port
		 */
		public int local_port;

		/*
		 * int remote_port
		 */
		public int remote_port;

		/*
		 * int fd
		 */
		public int fd;

		/*
		 * int watcher
		 */
		public int watcher;

		/*
		 * size_t bytes_sent
		 */
		public ulong bytes_sent;

		/*
		 * size_t bytes_remaining
		 */
		public ulong bytes_remaining;

		/*
		 * time_t start_time
		 */
		public ulong start_time;

		/*
		 * time_t end_time
		 */
		public ulong end_time;

		/*
		 * size_t current_buffer_size
		 */
		public ulong current_buffer_size;

		/*
		 * PurpleXferStatusType status
		 */
		public Ft.PurpleXferStatusType status;

		/*
		 * struct {
		void (*init)(PurpleXfer *xfer)
		 */
		/*
		 * Could not generate a wrapper for PurpleXfer.{
		void (*init)(PurpleXfer *xfer) in file "ft.h".
		 * Message: The struct contains an inner-struct or inner-union and cannot be automatically wrapped.
		 */

		/*
		 * void (*request_denied)(PurpleXfer * xfer)
		 */
		public IntPtr request_denied;

		/*
		 * void (*start)(PurpleXfer * xfer)
		 */
		public IntPtr start;

		/*
		 * void (*end)(PurpleXfer * xfer)
		 */
		public IntPtr end;

		/*
		 * void (*cancel_send)(PurpleXfer * xfer)
		 */
		public IntPtr cancel_send;

		/*
		 * void (*cancel_recv)(PurpleXfer * xfer)
		 */
		public IntPtr cancel_recv;

		/*
		 * gssize (*read)(guchar ** buffer, PurpleXfer * xfer)
		 */
		public IntPtr read;

		/*
		 * gssize (*write)(guchar * buffer, size_t size, PurpleXfer * xfer)
		 */
		public IntPtr write;

		/*
		 * void (*ack)(PurpleXfer * xfer, guchar * buffer, size_t size)
		 */
		public IntPtr ack;

	}

}

