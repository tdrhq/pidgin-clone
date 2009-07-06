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
	public class Plugin
	{
		public enum PurplePluginType
		{
			PURPLE_PLUGIN_UNKNOWN = -1,
			PURPLE_PLUGIN_STANDARD = 0,
			PURPLE_PLUGIN_LOADER,
			PURPLE_PLUGIN_PROTOCOL
		};

		/*
		 * PurplePlugin * purple_plugin_new(gboolean native, char * path)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_new(bool native, string path);

		public static PurplePlugin New(bool native, string path)
		{
			return new PurplePlugin(purple_plugin_new(native, path));
		}

		/*
		 * PurplePlugin * purple_plugin_probe(char * filename)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugin_probe(string filename);

		public static PurplePlugin Probe(string filename)
		{
			return new PurplePlugin(purple_plugin_probe(filename));
		}

		/*
		 * gboolean purple_plugin_register(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugin_register(IntPtr plugin);

		public static bool Register(PurplePlugin plugin)
		{
			return purple_plugin_register(plugin.Reference);
		}

		/*
		 * gboolean purple_plugin_load(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugin_load(IntPtr plugin);

		public static bool Load(PurplePlugin plugin)
		{
			return purple_plugin_load(plugin.Reference);
		}

		/*
		 * gboolean purple_plugin_unload(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugin_unload(IntPtr plugin);

		public static bool Unload(PurplePlugin plugin)
		{
			return purple_plugin_unload(plugin.Reference);
		}

		/*
		 * void purple_plugin_disable(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_disable(IntPtr plugin);

		public static void Disable(PurplePlugin plugin)
		{
			purple_plugin_disable(plugin.Reference);
		}

		/*
		 * gboolean purple_plugin_reload(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugin_reload(IntPtr plugin);

		public static bool Reload(PurplePlugin plugin)
		{
			return purple_plugin_reload(plugin.Reference);
		}

		/*
		 * void purple_plugin_destroy(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_destroy(IntPtr plugin);

		public static void Destroy(PurplePlugin plugin)
		{
			purple_plugin_destroy(plugin.Reference);
		}

		/*
		 * gboolean purple_plugin_is_loaded(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugin_is_loaded(IntPtr plugin);

		public static bool IsLoaded(PurplePlugin plugin)
		{
			return purple_plugin_is_loaded(plugin.Reference);
		}

		/*
		 * gboolean purple_plugin_is_unloadable(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugin_is_unloadable(IntPtr plugin);

		public static bool IsUnloadable(PurplePlugin plugin)
		{
			return purple_plugin_is_unloadable(plugin.Reference);
		}

		/*
		 * gchar * purple_plugin_get_id(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_get_id(IntPtr plugin);

		public static string GetId(PurplePlugin plugin)
		{
			return purple_plugin_get_id(plugin.Reference);
		}

		/*
		 * gchar * purple_plugin_get_name(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_get_name(IntPtr plugin);

		public static string GetName(PurplePlugin plugin)
		{
			return purple_plugin_get_name(plugin.Reference);
		}

		/*
		 * gchar * purple_plugin_get_version(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_get_version(IntPtr plugin);

		public static string GetVersion(PurplePlugin plugin)
		{
			return purple_plugin_get_version(plugin.Reference);
		}

		/*
		 * gchar * purple_plugin_get_summary(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_get_summary(IntPtr plugin);

		public static string GetSummary(PurplePlugin plugin)
		{
			return purple_plugin_get_summary(plugin.Reference);
		}

		/*
		 * gchar * purple_plugin_get_description(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_get_description(IntPtr plugin);

		public static string GetDescription(PurplePlugin plugin)
		{
			return purple_plugin_get_description(plugin.Reference);
		}

		/*
		 * gchar * purple_plugin_get_author(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_get_author(IntPtr plugin);

		public static string GetAuthor(PurplePlugin plugin)
		{
			return purple_plugin_get_author(plugin.Reference);
		}

		/*
		 * gchar * purple_plugin_get_homepage(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern string purple_plugin_get_homepage(IntPtr plugin);

		public static string GetHomepage(PurplePlugin plugin)
		{
			return purple_plugin_get_homepage(plugin.Reference);
		}

		/*
		 * void purple_plugin_ipc_unregister(PurplePlugin * plugin, char * command)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_ipc_unregister(IntPtr plugin, string command);

		public static void IpcUnregister(PurplePlugin plugin, string command)
		{
			purple_plugin_ipc_unregister(plugin.Reference, command);
		}

		/*
		 * void purple_plugin_ipc_unregister_all(PurplePlugin * plugin)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_ipc_unregister_all(IntPtr plugin);

		public static void IpcUnregisterAll(PurplePlugin plugin)
		{
			purple_plugin_ipc_unregister_all(plugin.Reference);
		}

		/*
		 * void purple_plugins_add_search_path(char * path)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_add_search_path(string path);

		public static void PluginsAddSearchPath(string path)
		{
			purple_plugins_add_search_path(path);
		}

		/*
		 * void purple_plugins_unload_all()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_unload_all();

		public static void PluginsUnloadAll()
		{
			purple_plugins_unload_all();
		}

		/*
		 * void purple_plugins_unload(PurplePluginType type)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_unload(Plugin.PurplePluginType type);

		public static void PluginsUnload(Plugin.PurplePluginType type)
		{
			/* Unable to process type, a KnownEnum. */
			throw new NotImplementedException();
		}

		/*
		 * void purple_plugins_destroy_all()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_destroy_all();

		public static void PluginsDestroyAll()
		{
			purple_plugins_destroy_all();
		}

		/*
		 * void purple_plugins_save_loaded(char * key)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_save_loaded(string key);

		public static void PluginsSaveLoaded(string key)
		{
			purple_plugins_save_loaded(key);
		}

		/*
		 * void purple_plugins_load_saved(char * key)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_load_saved(string key);

		public static void PluginsLoadSaved(string key)
		{
			purple_plugins_load_saved(key);
		}

		/*
		 * void purple_plugins_probe(char * ext)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_probe(string ext);

		public static void PluginsProbe(string ext)
		{
			purple_plugins_probe(ext);
		}

		/*
		 * gboolean purple_plugins_enabled()
		 */
		[DllImport("libpurple.dll")]
		private static extern bool purple_plugins_enabled();

		public static bool PluginsEnabled()
		{
			return purple_plugins_enabled();
		}

		/*
		 * PurplePlugin * purple_plugins_find_with_name(char * name)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugins_find_with_name(string name);

		public static PurplePlugin PluginsFindWithName(string name)
		{
			return new PurplePlugin(purple_plugins_find_with_name(name));
		}

		/*
		 * PurplePlugin * purple_plugins_find_with_filename(char * filename)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugins_find_with_filename(string filename);

		public static PurplePlugin PluginsFindWithFilename(string filename)
		{
			return new PurplePlugin(purple_plugins_find_with_filename(filename));
		}

		/*
		 * PurplePlugin * purple_plugins_find_with_basename(char * basename)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugins_find_with_basename(string basename);

		public static PurplePlugin PluginsFindWithBasename(string basename)
		{
			return new PurplePlugin(purple_plugins_find_with_basename(basename));
		}

		/*
		 * PurplePlugin * purple_plugins_find_with_id(char * id)
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugins_find_with_id(string id);

		public static PurplePlugin PluginsFindWithId(string id)
		{
			return new PurplePlugin(purple_plugins_find_with_id(id));
		}

		/*
		 * GList * purple_plugins_get_loaded()
		 * 
		 * Could not generate a wrapper for purple_plugins_get_loaded in file "plugin.h".
		 * Message: The type could not be resolved (GList * purple_plugins_get_loaded()).
		 */

		/*
		 * GList * purple_plugins_get_protocols()
		 * 
		 * Could not generate a wrapper for purple_plugins_get_protocols in file "plugin.h".
		 * Message: The type could not be resolved (GList * purple_plugins_get_protocols()).
		 */

		/*
		 * GList * purple_plugins_get_all()
		 * 
		 * Could not generate a wrapper for purple_plugins_get_all in file "plugin.h".
		 * Message: The type could not be resolved (GList * purple_plugins_get_all()).
		 */

		/*
		 * void * purple_plugins_get_handle()
		 */
		[DllImport("libpurple.dll")]
		private static extern IntPtr purple_plugins_get_handle();

		public static IntPtr PluginsGetHandle()
		{
			return purple_plugins_get_handle();
		}

		/*
		 * void purple_plugins_init()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_init();

		public static void PluginsInit()
		{
			purple_plugins_init();
		}

		/*
		 * void purple_plugins_uninit()
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugins_uninit();

		public static void PluginsUninit()
		{
			purple_plugins_uninit();
		}

		/*
		 * void purple_plugin_action_free(PurplePluginAction * action)
		 */
		[DllImport("libpurple.dll")]
		private static extern void purple_plugin_action_free(IntPtr action);

		public static void ActionFree(PurplePluginAction action)
		{
			purple_plugin_action_free(action.Reference);
		}

	}
}

