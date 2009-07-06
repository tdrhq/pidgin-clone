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
	public class Sound
	{
		public enum PurpleSoundEventID
		{
			PURPLE_SOUND_BUDDY_ARRIVE = 0,
			PURPLE_SOUND_BUDDY_LEAVE,
			PURPLE_SOUND_RECEIVE,
			PURPLE_SOUND_FIRST_RECEIVE,
			PURPLE_SOUND_SEND,
			PURPLE_SOUND_CHAT_JOIN,
			PURPLE_SOUND_CHAT_LEAVE,
			PURPLE_SOUND_CHAT_YOU_SAY,
			PURPLE_SOUND_CHAT_SAY,
			PURPLE_SOUND_POUNCE_DEFAULT,
			PURPLE_SOUND_CHAT_NICK,
			PURPLE_NUM_SOUNDS
		};

		/*
		 * void purple_sound_play_file(char * filename, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_sound_play_file(string filename, IntPtr account);

		public static void PlayFile(string filename, PurpleAccount account)
		{
			purple_sound_play_file(filename, account.Reference);
		}

		/*
		 * void purple_sound_play_event(PurpleSoundEventID event, PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_sound_play_event(Sound.PurpleSoundEventID event_, IntPtr account);

		public static void PlayEvent(Sound.PurpleSoundEventID event_, PurpleAccount account)
		{
			/* Unable to process event_, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * void purple_sound_set_ui_ops(PurpleSoundUiOps * ops)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_sound_set_ui_ops(IntPtr ops);

		public static void SetUiOps(PurpleSoundUiOps ops)
		{
			purple_sound_set_ui_ops(ops.Reference);
		}

		/*
		 * PurpleSoundUiOps * purple_sound_get_ui_ops()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_sound_get_ui_ops();

		public static PurpleSoundUiOps GetUiOps()
		{
			return new PurpleSoundUiOps(purple_sound_get_ui_ops());
		}

		/*
		 * void purple_sound_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_sound_init();

		public static void Init()
		{
			purple_sound_init();
		}

		/*
		 * void purple_sound_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_sound_uninit();

		public static void Uninit()
		{
			purple_sound_uninit();
		}

		/*
		 * void * purple_sounds_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_sounds_get_handle();

		public static IntPtr SoundsGetHandle()
		{
			return purple_sounds_get_handle();
		}

	}
}

