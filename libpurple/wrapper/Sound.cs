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
	public class Sound
	{
		/*
		 * void purple_sound_play_file(char * filename, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_sound_play_file(string filename, IntPtr account);

		public static void PlayFile(string filename, PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_sound_play_event(PurpleSoundEventID event, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_sound_play_event(UNKNOWN event, IntPtr account);

		public static void PlayEvent(PurpleSoundEventID event, PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_sound_set_ui_ops(PurpleSoundUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_sound_set_ui_ops(IntPtr ops);

		public static void SetUiOps(PurpleSoundUiOps ops)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleSoundUiOps * purple_sound_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_sound_get_ui_ops();

		public static PurpleSoundUiOps GetUiOps()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_sound_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_sound_init();

		public static void Init()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_sound_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_sound_uninit();

		public static void Uninit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_sounds_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_sounds_get_handle();

		public static IntPtr SoundsGetHandle()
		{
			throw new NotImplementedException();
		}

	}
}

