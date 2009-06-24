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
	public class Pounce
	{
		/*
		 * PurplePounce * purple_pounce_new(char * ui_type, PurpleAccount * pouncer, char * pouncee, PurplePounceEvent event, PurplePounceOption option)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_pounce_new(string ui_type, IntPtr pouncer, string pouncee, UNKNOWN event, UNKNOWN option);

		public static PurplePounce New(string ui_type, PurpleAccount pouncer, string pouncee, PurplePounceEvent event, PurplePounceOption option)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_pounce_destroy(PurplePounce * pounce)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_destroy(IntPtr pounce);

		public static void Destroy(PurplePounce pounce)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_pounce_destroy_all_by_account(PurpleAccount * account)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_destroy_all_by_account(IntPtr account);

		public static void DestroyAllByAccount(PurpleAccount account)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_pounce_set_events(PurplePounce * pounce, PurplePounceEvent events)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_set_events(IntPtr pounce, UNKNOWN events);

		public static void SetEvents(PurplePounce pounce, PurplePounceEvent events)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_pounce_set_options(PurplePounce * pounce, PurplePounceOption options)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_set_options(IntPtr pounce, UNKNOWN options);

		public static void SetOptions(PurplePounce pounce, PurplePounceOption options)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_pounce_set_pouncer(PurplePounce * pounce, PurpleAccount * pouncer)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_set_pouncer(IntPtr pounce, IntPtr pouncer);

		public static void SetPouncer(PurplePounce pounce, PurpleAccount pouncer)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_pounce_set_pouncee(PurplePounce * pounce, char * pouncee)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_set_pouncee(IntPtr pounce, string pouncee);

		public static void SetPouncee(PurplePounce pounce, string pouncee)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_pounce_set_save(PurplePounce * pounce, gboolean save)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_set_save(IntPtr pounce, bool save);

		public static void SetSave(PurplePounce pounce, bool save)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_pounce_action_register(PurplePounce * pounce, char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_action_register(IntPtr pounce, string name);

		public static void ActionRegister(PurplePounce pounce, string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_pounce_action_set_enabled(PurplePounce * pounce, char * action, gboolean enabled)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_action_set_enabled(IntPtr pounce, string action, bool enabled);

		public static void ActionSetEnabled(PurplePounce pounce, string action, bool enabled)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_pounce_action_set_attribute(PurplePounce * pounce, char * action, char * attr, char * value)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_action_set_attribute(IntPtr pounce, string action, string attr, string value);

		public static void ActionSetAttribute(PurplePounce pounce, string action, string attr, string value)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_pounce_set_data(PurplePounce * pounce, void * data)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_set_data(IntPtr pounce, IntPtr data);

		public static void SetData(PurplePounce pounce, IntPtr data)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePounceEvent purple_pounce_get_events(PurplePounce * pounce)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_pounce_get_events(IntPtr pounce);

		public static PurplePounceEvent GetEvents(PurplePounce pounce)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePounceOption purple_pounce_get_options(PurplePounce * pounce)
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_pounce_get_options(IntPtr pounce);

		public static PurplePounceOption GetOptions(PurplePounce pounce)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurpleAccount * purple_pounce_get_pouncer(PurplePounce * pounce)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_pounce_get_pouncer(IntPtr pounce);

		public static PurpleAccount GetPouncer(PurplePounce pounce)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_pounce_get_pouncee(PurplePounce * pounce)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_pounce_get_pouncee(IntPtr pounce);

		public static string GetPouncee(PurplePounce pounce)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_pounce_get_save(PurplePounce * pounce)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_pounce_get_save(IntPtr pounce);

		public static bool GetSave(PurplePounce pounce)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_pounce_action_is_enabled(PurplePounce * pounce, char * action)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_pounce_action_is_enabled(IntPtr pounce, string action);

		public static bool ActionIsEnabled(PurplePounce pounce, string action)
		{
			throw new NotImplementedException();
		}

		/*
		 * char * purple_pounce_action_get_attribute(PurplePounce * pounce, char * action, char * attr)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_pounce_action_get_attribute(IntPtr pounce, string action, string attr);

		public static string ActionGetAttribute(PurplePounce pounce, string action, string attr)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_pounce_get_data(PurplePounce * pounce)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_pounce_get_data(IntPtr pounce);

		public static IntPtr GetData(PurplePounce pounce)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_pounce_execute(PurpleAccount * pouncer, char * pouncee, PurplePounceEvent events)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounce_execute(IntPtr pouncer, string pouncee, UNKNOWN events);

		public static void Execute(PurpleAccount pouncer, string pouncee, PurplePounceEvent events)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePounce * purple_find_pounce(PurpleAccount * pouncer, char * pouncee, PurplePounceEvent events)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_find_pounce(IntPtr pouncer, string pouncee, UNKNOWN events);

		public static PurplePounce FindPounce(PurpleAccount pouncer, string pouncee, PurplePounceEvent events)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_pounces_load()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_pounces_load();

		public static bool PouncesLoad()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_pounces_register_handler(char * ui, PurplePounceCb cb,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounces_register_handler(string ui, UNKNOWN cb, UNKNOWN );

		public static void PouncesRegisterHandler(string ui, PurplePounceCb cb,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_pounces_unregister_handler(char * ui)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounces_unregister_handler(string ui);

		public static void PouncesUnregisterHandler(string ui)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_pounces_get_all()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_pounces_get_all();

		public static GList PouncesGetAll()
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_pounces_get_all_for_ui(char * ui)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_pounces_get_all_for_ui(string ui);

		public static GList PouncesGetAllForUi(string ui)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_pounces_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_pounces_get_handle();

		public static IntPtr PouncesGetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_pounces_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounces_init();

		public static void PouncesInit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_pounces_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_pounces_uninit();

		public static void PouncesUninit()
		{
			throw new NotImplementedException();
		}

	}
}

