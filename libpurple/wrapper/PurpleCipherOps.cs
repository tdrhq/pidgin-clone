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
	 * File: cipher.h
	 * Structure: PurpleCipherOps
	 */
	public class PurpleCipherOps : UnmanagedWrapper<_PurpleCipherOps>
	{
		public PurpleCipherOps()
		{
		}

		public PurpleCipherOps(IntPtr reference)
		{
			this.Reference = reference;
			this.Data = (_PurpleCipherOps)Marshal.PtrToStructure(this.Reference, typeof(_PurpleCipherOps));
		}

		/*
		 * Argument Name: set_option
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr set_option
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
		 * Argument Name: get_option
		 * Argument Type: void *
		 * Argument Category: VoidPointer
		 */
		public IntPtr get_option
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
		 * Argument Name: init
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr init
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
		 * Argument Name: reset
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr reset
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
		 * Argument Name: uninit
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr uninit
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
		 * Argument Name: set_iv
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr set_iv
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
		 * Argument Name: append
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr append
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
		 * Argument Name: digest
		 * Argument Type: gboolean
		 * Argument Category: Native
		 */
		public IntPtr digest
		{
			get
			{
				return this.Data.digest;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.digest = value;
			}
		}

		/*
		 * Argument Name: encrypt
		 * Argument Type: int
		 * Argument Category: Native
		 */
		public IntPtr encrypt
		{
			get
			{
				return this.Data.encrypt;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.encrypt = value;
			}
		}

		/*
		 * Argument Name: decrypt
		 * Argument Type: int
		 * Argument Category: Native
		 */
		public IntPtr decrypt
		{
			get
			{
				return this.Data.decrypt;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.decrypt = value;
			}
		}

		/*
		 * Argument Name: set_salt
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr set_salt
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
		 * Argument Name: get_salt_size
		 * Argument Type: size_t
		 * Argument Category: Native
		 */
		public IntPtr get_salt_size
		{
			get
			{
				return this.Data.get_salt_size;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.get_salt_size = value;
			}
		}

		/*
		 * Argument Name: set_key
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr set_key
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
		 * Argument Name: get_key_size
		 * Argument Type: size_t
		 * Argument Category: Native
		 */
		public IntPtr get_key_size
		{
			get
			{
				return this.Data.get_key_size;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.get_key_size = value;
			}
		}

		/*
		 * Argument Name: set_batch_mode
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr set_batch_mode
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
		 * Argument Name: get_batch_mode
		 * Argument Type: PurpleCipherBatchMode
		 * Argument Category: KnownEnum
		 */
		public IntPtr get_batch_mode
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
		 * Argument Name: get_block_size
		 * Argument Type: size_t
		 * Argument Category: Native
		 */
		public IntPtr get_block_size
		{
			get
			{
				return this.Data.get_block_size;
			}
			set
			{
				if (this.Reference != IntPtr.Zero)
					this.Reference = IntPtr.Zero;

				this.Data.get_block_size = value;
			}
		}

		/*
		 * Argument Name: set_key_with_len
		 * Argument Type: void
		 * Argument Category: Void
		 */
		public IntPtr set_key_with_len
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
	public class _PurpleCipherOps
	{
		/*
		 * void (*set_option)(PurpleCipherContext * context, gchar * name, void * value)
		 */
		public IntPtr set_option;

		/*
		 * void * (*get_option)(PurpleCipherContext * context, gchar * name)
		 */
		public IntPtr get_option;

		/*
		 * void (*init)(PurpleCipherContext * context, void * extra)
		 */
		public IntPtr init;

		/*
		 * void (*reset)(PurpleCipherContext * context, void * extra)
		 */
		public IntPtr reset;

		/*
		 * void (*uninit)(PurpleCipherContext * context)
		 */
		public IntPtr uninit;

		/*
		 * void (*set_iv)(PurpleCipherContext * context, guchar * iv, size_t len)
		 */
		public IntPtr set_iv;

		/*
		 * void (*append)(PurpleCipherContext * context, guchar * data, size_t len)
		 */
		public IntPtr append;

		/*
		 * gboolean (*digest)(PurpleCipherContext * context, size_t in_len, guchar digest[], size_t * out_len)
		 */
		public IntPtr digest;

		/*
		 * int (*encrypt)(PurpleCipherContext * context, guchar data[], size_t len, guchar output[], size_t * outlen)
		 */
		public IntPtr encrypt;

		/*
		 * int (*decrypt)(PurpleCipherContext * context, guchar data[], size_t len, guchar output[], size_t * outlen)
		 */
		public IntPtr decrypt;

		/*
		 * void (*set_salt)(PurpleCipherContext * context, guchar * salt)
		 */
		public IntPtr set_salt;

		/*
		 * size_t (*get_salt_size)(PurpleCipherContext * context)
		 */
		public IntPtr get_salt_size;

		/*
		 * void (*set_key)(PurpleCipherContext * context, guchar * key)
		 */
		public IntPtr set_key;

		/*
		 * size_t (*get_key_size)(PurpleCipherContext * context)
		 */
		public IntPtr get_key_size;

		/*
		 * void (*set_batch_mode)(PurpleCipherContext * context, PurpleCipherBatchMode mode)
		 */
		public IntPtr set_batch_mode;

		/*
		 * PurpleCipherBatchMode (*get_batch_mode)(PurpleCipherContext * context)
		 */
		public IntPtr get_batch_mode;

		/*
		 * size_t (*get_block_size)(PurpleCipherContext * context)
		 */
		public IntPtr get_block_size;

		/*
		 * void (*set_key_with_len)(PurpleCipherContext * context, guchar * key, size_t len)
		 */
		public IntPtr set_key_with_len;

	}

}
