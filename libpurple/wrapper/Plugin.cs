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
	public class Plugin
	{
		/*
		 * gboolean _FUNC_NAME( )
		 */
		[DllImport("libpurple.dll")]
		private static extern bool _FUNC_NAME(UNKNOWN );

		public static bool _funcName( )
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean _FUNC_NAME( )
		 */
		[DllImport("libpurple.dll")]
		private static extern bool _FUNC_NAME(UNKNOWN );

		public static bool _funcName( )
		{
			throw new NotImplementedException();
		}

		/*
		 * return purple_plugin_register( )
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_plugin_register(UNKNOWN );

		public static return Register( )
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_init_plugin(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_init_plugin(IntPtr plugin);

		public static bool InitPlugin(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_init_plugin(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_init_plugin(IntPtr plugin);

		public static bool InitPlugin(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * return purple_plugin_register( )
		 */
		[DllImport("libpurple.dll")]
		private static extern UNKNOWN purple_plugin_register(UNKNOWN );

		public static return Register( )
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePlugin * purple_plugin_new(gboolean native, char * path)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_new(bool native, string path);

		public static PurplePlugin New(bool native, string path)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePlugin * purple_plugin_probe(char * filename)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_probe(string filename);

		public static PurplePlugin Probe(string filename)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_plugin_register(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugin_register(IntPtr plugin);

		public static bool Register(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_plugin_load(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugin_load(IntPtr plugin);

		public static bool Load(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_plugin_unload(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugin_unload(IntPtr plugin);

		public static bool Unload(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_disable(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_disable(IntPtr plugin);

		public static void Disable(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_plugin_reload(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugin_reload(IntPtr plugin);

		public static bool Reload(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_destroy(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_destroy(IntPtr plugin);

		public static void Destroy(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_plugin_is_loaded(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugin_is_loaded(IntPtr plugin);

		public static bool IsLoaded(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_plugin_is_unloadable(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugin_is_unloadable(IntPtr plugin);

		public static bool IsUnloadable(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_plugin_get_id(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_get_id(IntPtr plugin);

		public static string GetId(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_plugin_get_name(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_get_name(IntPtr plugin);

		public static string GetName(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_plugin_get_version(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_get_version(IntPtr plugin);

		public static string GetVersion(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_plugin_get_summary(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_get_summary(IntPtr plugin);

		public static string GetSummary(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_plugin_get_description(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_get_description(IntPtr plugin);

		public static string GetDescription(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_plugin_get_author(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_get_author(IntPtr plugin);

		public static string GetAuthor(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * gchar * purple_plugin_get_homepage(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_get_homepage(IntPtr plugin);

		public static string GetHomepage(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_plugin_ipc_register(PurplePlugin * plugin, char * command, PurpleCallback func, PurpleSignalMarshalFunc marshal, PurpleValue * ret_value, int num_params, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugin_ipc_register(IntPtr plugin, string command, UNKNOWN func, UNKNOWN marshal, IntPtr ret_value, int num_params, ...);

		public static bool IpcRegister(PurplePlugin plugin, string command, PurpleCallback func, PurpleSignalMarshalFunc marshal, PurpleValue ret_value, int num_params, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_ipc_unregister(PurplePlugin * plugin, char * command)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_ipc_unregister(IntPtr plugin, string command);

		public static void IpcUnregister(PurplePlugin plugin, string command)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_ipc_unregister_all(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_ipc_unregister_all(IntPtr plugin);

		public static void IpcUnregisterAll(PurplePlugin plugin)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_plugin_ipc_get_params(PurplePlugin * plugin, char * command, PurpleValue ** ret_value, int * num_params, PurpleValue *** params)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugin_ipc_get_params(IntPtr plugin, string command, IntPtr ret_value, IntPtr num_params, IntPtr params);

		public static bool IpcGetParams(PurplePlugin plugin, string command, PurpleValue ret_value, int num_params, PurpleValue params)
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_plugin_ipc_call(PurplePlugin * plugin, char * command, gboolean * ok, ...)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_ipc_call(IntPtr plugin, string command, IntPtr ok, ...);

		public static IntPtr IpcCall(PurplePlugin plugin, string command, gboolean ok, ...)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugins_add_search_path(char * path)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_add_search_path(string path);

		public static void PluginsAddSearchPath(string path)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugins_unload_all()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_unload_all();

		public static void PluginsUnloadAll()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugins_unload(PurplePluginType type)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_unload(UNKNOWN type);

		public static void PluginsUnload(PurplePluginType type)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugins_destroy_all()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_destroy_all();

		public static void PluginsDestroyAll()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugins_save_loaded(char * key)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_save_loaded(string key);

		public static void PluginsSaveLoaded(string key)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugins_load_saved(char * key)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_load_saved(string key);

		public static void PluginsLoadSaved(string key)
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugins_probe(char * ext)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_probe(string ext);

		public static void PluginsProbe(string ext)
		{
			throw new NotImplementedException();
		}

		/*
		 * gboolean purple_plugins_enabled()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugins_enabled();

		public static bool PluginsEnabled()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugins_register_probe_notify_cb( )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_register_probe_notify_cb(UNKNOWN );

		public static void PluginsRegisterProbeNotifyCb( )
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugins_unregister_probe_notify_cb( )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_unregister_probe_notify_cb(UNKNOWN );

		public static void PluginsUnregisterProbeNotifyCb( )
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugins_register_load_notify_cb( )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_register_load_notify_cb(UNKNOWN );

		public static void PluginsRegisterLoadNotifyCb( )
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugins_unregister_load_notify_cb( )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_unregister_load_notify_cb(UNKNOWN );

		public static void PluginsUnregisterLoadNotifyCb( )
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugins_register_unload_notify_cb( )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_register_unload_notify_cb(UNKNOWN );

		public static void PluginsRegisterUnloadNotifyCb( )
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugins_unregister_unload_notify_cb( )
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_unregister_unload_notify_cb(UNKNOWN );

		public static void PluginsUnregisterUnloadNotifyCb( )
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePlugin * purple_plugins_find_with_name(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugins_find_with_name(string name);

		public static PurplePlugin PluginsFindWithName(string name)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePlugin * purple_plugins_find_with_filename(char * filename)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugins_find_with_filename(string filename);

		public static PurplePlugin PluginsFindWithFilename(string filename)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePlugin * purple_plugins_find_with_basename(char * basename)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugins_find_with_basename(string basename);

		public static PurplePlugin PluginsFindWithBasename(string basename)
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePlugin * purple_plugins_find_with_id(char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugins_find_with_id(string id);

		public static PurplePlugin PluginsFindWithId(string id)
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_plugins_get_loaded()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugins_get_loaded();

		public static GList PluginsGetLoaded()
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_plugins_get_protocols()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugins_get_protocols();

		public static GList PluginsGetProtocols()
		{
			throw new NotImplementedException();
		}

		/*
		 * GList * purple_plugins_get_all()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugins_get_all();

		public static GList PluginsGetAll()
		{
			throw new NotImplementedException();
		}

		/*
		 * void * purple_plugins_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugins_get_handle();

		public static IntPtr PluginsGetHandle()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugins_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_init();

		public static void PluginsInit()
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugins_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_uninit();

		public static void PluginsUninit()
		{
			throw new NotImplementedException();
		}

		/*
		 * PurplePluginAction * purple_plugin_action_new( ,  )
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_action_new(UNKNOWN , UNKNOWN );

		public static PurplePluginAction ActionNew( ,  )
		{
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugin_action_free(PurplePluginAction * action)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_action_free(IntPtr action);

		public static void ActionFree(PurplePluginAction action)
		{
			throw new NotImplementedException();
		}

	}
}

