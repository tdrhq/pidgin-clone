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
	 * File: pounce.h
	 */
	public class Pounce
	{
		public enum PurplePounceEvent
		{
			PURPLE_POUNCE_NONE = 0x000,
			PURPLE_POUNCE_SIGNON = 0x001,
			PURPLE_POUNCE_SIGNOFF = 0x002,
			PURPLE_POUNCE_AWAY = 0x004,
			PURPLE_POUNCE_AWAY_RETURN = 0x008,
			PURPLE_POUNCE_IDLE = 0x010,
			PURPLE_POUNCE_IDLE_RETURN = 0x020,
			PURPLE_POUNCE_TYPING = 0x040,
			PURPLE_POUNCE_TYPED = 0x080,
			PURPLE_POUNCE_TYPING_STOPPED = 0x100,
			PURPLE_POUNCE_MESSAGE_RECEIVED = 0x200
		};

		public enum PurplePounceOption
		{
			PURPLE_POUNCE_OPTION_NONE = 0x00,
			PURPLE_POUNCE_OPTION_AWAY = 0x01
		};

		public delegate void PurplePounceCb(IntPtr _PurpleWrapper_arg0, /* Unresolved: [Category: Unknown], [Name:  ] */ IntPtr _PurpleWrapper_arg1, IntPtr _PurpleWrapper_arg2);

		/*
		 * Function Name: purple_pounce_destroy
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_pounce_destroy(PurplePounce * pounce)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_destroy(IntPtr pounce);

		public static void Destroy(PurplePounce pounce)
		{
			purple_pounce_destroy(pounce.Reference);
		}

		/*
		 * Function Name: purple_pounce_destroy_all_by_account
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_pounce_destroy_all_by_account(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_destroy_all_by_account(IntPtr account);

		public static void DestroyAllByAccount(PurpleAccount account)
		{
			purple_pounce_destroy_all_by_account(account.Reference);
		}

		/*
		 * Function Name: purple_pounce_set_events
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_pounce_set_events(PurplePounce * pounce, PurplePounceEvent events)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_set_events(IntPtr pounce, Pounce.PurplePounceEvent events);

		public static void SetEvents(PurplePounce pounce, Pounce.PurplePounceEvent events)
		{
			/* Unable to process events, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_pounce_set_options
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_pounce_set_options(PurplePounce * pounce, PurplePounceOption options)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_set_options(IntPtr pounce, Pounce.PurplePounceOption options);

		public static void SetOptions(PurplePounce pounce, Pounce.PurplePounceOption options)
		{
			/* Unable to process options, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_pounce_set_pouncer
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_pounce_set_pouncer(PurplePounce * pounce, PurpleAccount * pouncer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_set_pouncer(IntPtr pounce, IntPtr pouncer);

		public static void SetPouncer(PurplePounce pounce, PurpleAccount pouncer)
		{
			purple_pounce_set_pouncer(pounce.Reference, pouncer.Reference);
		}

		/*
		 * Function Name: purple_pounce_set_pouncee
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_pounce_set_pouncee(PurplePounce * pounce, char * pouncee)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_set_pouncee(IntPtr pounce, string pouncee);

		public static void SetPouncee(PurplePounce pounce, string pouncee)
		{
			purple_pounce_set_pouncee(pounce.Reference, pouncee);
		}

		/*
		 * Function Name: purple_pounce_set_save
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_pounce_set_save(PurplePounce * pounce, gboolean save)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_set_save(IntPtr pounce, bool save);

		public static void SetSave(PurplePounce pounce, bool save)
		{
			purple_pounce_set_save(pounce.Reference, save);
		}

		/*
		 * Function Name: purple_pounce_action_register
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_pounce_action_register(PurplePounce * pounce, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_action_register(IntPtr pounce, string name);

		public static void ActionRegister(PurplePounce pounce, string name)
		{
			purple_pounce_action_register(pounce.Reference, name);
		}

		/*
		 * Function Name: purple_pounce_set_data
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_pounce_set_data(PurplePounce * pounce, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_set_data(IntPtr pounce, IntPtr data);

		public static void SetData(PurplePounce pounce, IntPtr data)
		{
			purple_pounce_set_data(pounce.Reference, data);
		}

		/*
		 * Function Name: purple_pounce_get_events
		 * Function Type: PurplePounceEvent
		 * Function RawType: PurplePounceEvent
		 * Function Category: KnownEnum
		 */
		/*
		 * PurplePounceEvent purple_pounce_get_events(PurplePounce * pounce)
		 */
		[DllImport("libpurple.dll")]
		private static extern Pounce.PurplePounceEvent purple_pounce_get_events(IntPtr pounce);

		public static Pounce.PurplePounceEvent GetEvents(PurplePounce pounce)
		{
			/* Unable to process the return value of purple_pounce_get_events, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_pounce_get_options
		 * Function Type: PurplePounceOption
		 * Function RawType: PurplePounceOption
		 * Function Category: KnownEnum
		 */
		/*
		 * PurplePounceOption purple_pounce_get_options(PurplePounce * pounce)
		 */
		[DllImport("libpurple.dll")]
		private static extern Pounce.PurplePounceOption purple_pounce_get_options(IntPtr pounce);

		public static Pounce.PurplePounceOption GetOptions(PurplePounce pounce)
		{
			/* Unable to process the return value of purple_pounce_get_options, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * Function Name: purple_pounce_get_pouncer
		 * Function Type: PurpleAccount *
		 * Function RawType: PurpleAccount
		 * Function Category: PointerToKnownStruct
		 */
		/*
		 * PurpleAccount * purple_pounce_get_pouncer(PurplePounce * pounce)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_pounce_get_pouncer(IntPtr pounce);

		public static PurpleAccount GetPouncer(PurplePounce pounce)
		{
			return new PurpleAccount(purple_pounce_get_pouncer(pounce.Reference));
		}

		/*
		 * Function Name: purple_pounce_get_pouncee
		 * Function Type: char *
		 * Function RawType: char
		 * Function Category: Native
		 */
		/*
		 * char * purple_pounce_get_pouncee(PurplePounce * pounce)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_pounce_get_pouncee(IntPtr pounce);

		public static string GetPouncee(PurplePounce pounce)
		{
			return purple_pounce_get_pouncee(pounce.Reference);
		}

		/*
		 * Function Name: purple_pounce_get_save
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_pounce_get_save(PurplePounce * pounce)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_pounce_get_save(IntPtr pounce);

		public static bool GetSave(PurplePounce pounce)
		{
			return purple_pounce_get_save(pounce.Reference);
		}

		/*
		 * Function Name: purple_pounce_get_data
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
		 */
		/*
		 * void * purple_pounce_get_data(PurplePounce * pounce)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_pounce_get_data(IntPtr pounce);

		public static IntPtr GetData(PurplePounce pounce)
		{
			return purple_pounce_get_data(pounce.Reference);
		}

		/*
		 * Function Name: purple_pounces_load
		 * Function Type: gboolean
		 * Function RawType: gboolean
		 * Function Category: Native
		 */
		/*
		 * gboolean purple_pounces_load()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_pounces_load();

		public static bool PouncesLoad()
		{
			return purple_pounces_load();
		}

		/*
		 * Function Name: purple_pounces_unregister_handler
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_pounces_unregister_handler(char * ui)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounces_unregister_handler(string ui);

		public static void PouncesUnregisterHandler(string ui)
		{
			purple_pounces_unregister_handler(ui);
		}

		/*
		 * Function Name: purple_pounces_get_all
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_pounces_get_all()
		 * 
		 * Could not generate a wrapper for purple_pounces_get_all in file "pounce.h".
		 * Message: Unknown inner-type of argument 0 of purple_pounces_get_all
		 */

		/*
		 * Function Name: purple_pounces_get_all_for_ui
		 * Function Type: GList *
		 * Function RawType: GList
		 * Function Category: PointerToGList
		 */
		/*
		 * GList * purple_pounces_get_all_for_ui(char * ui)
		 * 
		 * Could not generate a wrapper for purple_pounces_get_all_for_ui in file "pounce.h".
		 * Message: Unknown inner-type of argument 0 of purple_pounces_get_all_for_ui
		 */

		/*
		 * Function Name: purple_pounces_get_handle
		 * Function Type: void *
		 * Function RawType: void
		 * Function Category: VoidPointer
		 */
		/*
		 * void * purple_pounces_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_pounces_get_handle();

		public static IntPtr PouncesGetHandle()
		{
			return purple_pounces_get_handle();
		}

		/*
		 * Function Name: purple_pounces_init
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_pounces_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounces_init();

		public static void PouncesInit()
		{
			purple_pounces_init();
		}

		/*
		 * Function Name: purple_pounces_uninit
		 * Function Type: void
		 * Function RawType: void
		 * Function Category: Void
		 */
		/*
		 * void purple_pounces_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounces_uninit();

		public static void PouncesUninit()
		{
			purple_pounces_uninit();
		}

	}
}

