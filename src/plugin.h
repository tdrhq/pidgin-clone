/**
 * @file plugin.h Plugin API
 * @ingroup core
 *
 * gaim
 *
 * Gaim is the legal property of its developers, whose names are too numerous
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef _GAIM_PLUGIN_H_
#define _GAIM_PLUGIN_H_

#include <glib/glist.h>
#include <gmodule.h>
#include "signals.h"
#include "value.h"

typedef struct _GaimPlugin           GaimPlugin;
typedef struct _GaimPluginInfo       GaimPluginInfo;
typedef struct _GaimPluginUiInfo     GaimPluginUiInfo;
typedef struct _GaimPluginLoaderInfo GaimPluginLoaderInfo;

typedef int GaimPluginPriority; /**< Plugin priority. */

#include "pluginpref.h"

/**
 * Plugin types.
 */
typedef enum
{
	GAIM_PLUGIN_UNKNOWN  = -1,  /**< Unknown type.    */
	GAIM_PLUGIN_STANDARD = 0,   /**< Standard plugin. */
	GAIM_PLUGIN_LOADER,         /**< Loader plugin.   */
	GAIM_PLUGIN_PROTOCOL        /**< Protocol plugin. */

} GaimPluginType;

#define GAIM_PRIORITY_DEFAULT     0
#define GAIM_PRIORITY_HIGHEST  9999
#define GAIM_PRIORITY_LOWEST  -9999

#define GAIM_PLUGIN_FLAG_INVISIBLE 0x01

#define GAIM_PLUGIN_API_VERSION 4
#define GAIM_LOADER_API_VERSION 2

/**
 * Detailed information about a plugin.
 *
 * This is used in the version 2.0 API and up.
 */
struct _GaimPluginInfo
{
	unsigned int api_version;
	GaimPluginType type;
	char *ui_requirement;
	unsigned long flags;
	GList *dependencies;
	GaimPluginPriority priority;

	char *id;
	char *name;
	char *version;
	char *summary;
	char *description;
	char *author;
	char *homepage;

	gboolean (*load)(GaimPlugin *plugin);
	gboolean (*unload)(GaimPlugin *plugin);
	void (*destroy)(GaimPlugin *plugin);

	void *ui_info;
	void *extra_info;
	GaimPluginUiInfo *prefs_info;
	GList *(*actions)(GaimPlugin *plugin);
};

/**
 * Extra information for loader plugins.
 */
struct _GaimPluginLoaderInfo
{
	unsigned int api_version;

	GList *exts;

	gboolean (*probe)(GaimPlugin *plugin);
	gboolean (*load)(GaimPlugin *plugin);
	gboolean (*unload)(GaimPlugin *plugin);
	void     (*destroy)(GaimPlugin *plugin);
};

/**
 * A plugin handle.
 */
struct _GaimPlugin
{
	gboolean native_plugin;                /**< Native C plugin.          */
	gboolean loaded;                       /**< The loaded state.         */
	void *handle;                          /**< The module handle.        */
	char *path;                            /**< The path to the plugin.   */
	GaimPluginInfo *info;                  /**< The plugin information.   */
	char *error;
	void *ipc_data;                        /**< IPC data.                 */
	void *extra;                           /**< Plugin-specific data.     */
};

#define GAIM_PLUGIN_LOADER_INFO(plugin) \
	((GaimPluginLoaderInfo *)(plugin)->info->extra_info)

struct _GaimPluginUiInfo {
	GaimPluginPrefFrame *(*get_plugin_pref_frame)(GaimPlugin *plugin);

	void *iter;                                           /**< Reserved */
	GaimPluginPrefFrame *frame;                           /**< Reserved */
};

#define GAIM_PLUGIN_HAS_PREF_FRAME(plugin) \
	((plugin)->info != NULL && (plugin)->info->prefs_info != NULL)

#define GAIM_PLUGIN_UI_INFO(plugin) \
	((GaimPluginUiInfo*)(plugin)->info->prefs_info)

/**
 * Handles the initialization of modules.
 */
#if !defined(GAIM_PLUGINS) || defined(GAIM_STATIC_PRPL)
# define GAIM_INIT_PLUGIN(pluginname, initfunc, plugininfo) \
	gboolean gaim_init_##pluginname##_plugin(void) { \
		GaimPlugin *plugin = gaim_plugin_new(TRUE, NULL); \
		plugin->info = &(plugininfo); \
		initfunc((plugin)); \
		return gaim_plugin_register(plugin); \
	}
#else /* GAIM_PLUGINS  && !GAIM_STATIC_PRPL */
# define GAIM_INIT_PLUGIN(pluginname, initfunc, plugininfo) \
	G_MODULE_EXPORT gboolean gaim_init_plugin(GaimPlugin *plugin) { \
		plugin->info = &(plugininfo); \
		initfunc((plugin)); \
		return gaim_plugin_register(plugin); \
	}
#endif

#ifdef __cplusplus
extern "C" {
#endif


void *gaim_plugins_get_handle(void);


/**************************************************************************/
/** @name Plugin API                                                      */
/**************************************************************************/
/*@{*/

/**
 * Creates a new plugin structure.
 *
 * @param native Whether or not the plugin is native.
 * @param path   The path to the plugin, or @c NULL if statically compiled.
 *
 * @return A new GaimPlugin structure.
 */
GaimPlugin *gaim_plugin_new(gboolean native, const char *path);

/**
 * Probes a plugin, retrieving the information on it and adding it to the
 * list of available plugins.
 *
 * @param filename The plugin's filename.
 *
 * @return The plugin handle.
 *
 * @see gaim_plugin_load()
 * @see gaim_plugin_destroy()
 */
GaimPlugin *gaim_plugin_probe(const char *filename);

/**
 * Registers a plugin and prepares it for loading.
 *
 * This shouldn't be called by anything but the internal module code.
 *
 * @param plugin The plugin to register.
 */
gboolean gaim_plugin_register(GaimPlugin *plugin);

/**
 * Attempts to load a previously probed plugin.
 *
 * @param plugin The plugin to load.
 *
 * @return @c TRUE if successful, or @c FALSE otherwise.
 *
 * @see gaim_plugin_reload()
 * @see gaim_plugin_unload()
 */
gboolean gaim_plugin_load(GaimPlugin *plugin);

/**
 * Unloads the specified plugin.
 *
 * @param plugin The plugin handle.
 *
 * @return @c TRUE if successful, or @c FALSE otherwise.
 *
 * @see gaim_plugin_load()
 * @see gaim_plugin_reload()
 */
gboolean gaim_plugin_unload(GaimPlugin *plugin);

/**
 * Reloads a plugin.
 *
 * @param plugin The old plugin handle.
 *
 * @return @c TRUE if successful, or @c FALSE otherwise.
 *
 * @see gaim_plugin_load()
 * @see gaim_plugin_unload()
 */
gboolean gaim_plugin_reload(GaimPlugin *plugin);

/**
 * Unloads a plugin and destroys the structure from memory.
 *
 * @param plugin The plugin handle.
 */
void gaim_plugin_destroy(GaimPlugin *plugin);

/**
 * Returns whether or not a plugin is currently loaded.
 *
 * @param plugin The plugin.
 *
 * @return TRUE if loaded, or FALSE otherwise.
 */
gboolean gaim_plugin_is_loaded(const GaimPlugin *plugin);

/*@}*/

/**************************************************************************/
/** @name Plugin IPC API                                                  */
/**************************************************************************/
/*@{*/

/**
 * Registers an IPC command in a plugin.
 *
 * @param plugin     The plugin to register the command with.
 * @param command    The name of the command.
 * @param func       The function to execute.
 * @param marshal    The marshalling function.
 * @param ret_value  The return value type.
 * @param num_params The number of parameters.
 * @param ...        The parameter types.
 *
 * @return TRUE if the function was registered successfully, or
 *         FALSE otherwise.
 */
gboolean gaim_plugin_ipc_register(GaimPlugin *plugin, const char *command,
								  GaimCallback func,
								  GaimSignalMarshalFunc marshal,
								  GaimValue *ret_value, int num_params, ...);

/**
 * Unregisters an IPC command in a plugin.
 *
 * @param plugin  The plugin to unregister the command from.
 * @param command The name of the command.
 */
void gaim_plugin_ipc_unregister(GaimPlugin *plugin, const char *command);

/**
 * Unregisters all IPC commands in a plugin.
 *
 * @param plugin The plugin to unregister the commands from.
 */
void gaim_plugin_ipc_unregister_all(GaimPlugin *plugin);

/**
 * Returns a list of value types used for an IPC command.
 *
 * @param plugin     The plugin.
 * @param command    The name of the command.
 * @param ret_value  The returned return value.
 * @param num_params The returned number of parameters.
 * @param params     The returned list of parameters.
 *
 * @return TRUE if the command was found, or FALSE otherwise.
 */
gboolean gaim_plugin_ipc_get_params(GaimPlugin *plugin, const char *command,
									GaimValue **ret_value, int *num_params,
									GaimValue ***params);

/**
 * Executes an IPC command.
 *
 * @param plugin  The plugin to execute the command on.
 * @param command The name of the command.
 * @param ok      TRUE if the call was successful, or FALSE otherwise.
 * @param ...     The parameters to pass.
 *
 * @return The return value, which will be NULL if the command doesn't
 *         return a value.
 */
void *gaim_plugin_ipc_call(GaimPlugin *plugin, const char *command,
						   gboolean *ok, ...);

/*@}*/

/**************************************************************************/
/** @name Plugins API                                                     */
/**************************************************************************/
/*@{*/

/**
 * Sets the search paths for plugins.
 *
 * @param count The number of search paths.
 * @param paths The search paths.
 */
void gaim_plugins_set_search_paths(size_t count, char **paths);

/**
 * Unloads all loaded plugins.
 */
void gaim_plugins_unload_all(void);

/**
 * Destroys all registered plugins.
 */
void gaim_plugins_destroy_all(void);

/**
 * Attempts to load all the plugins in the specified preference key
 * that were loaded when gaim last quit.
 *
 * @param key The preference key containing the list of plugins.
 */
void gaim_plugins_load_saved(const char *key);

/**
 * Probes for plugins in the registered module paths.
 *
 * @param ext The extension type to probe for, or @c NULL for all.
 *
 * @see gaim_plugin_set_probe_path()
 */
void gaim_plugins_probe(const char *ext);

/**
 * Returns whether or not plugin support is enabled.
 *
 * @return TRUE if plugin support is enabled, or FALSE otherwise.
 */
gboolean gaim_plugins_enabled(void);

/**
 * Registers a function that will be called when probing is finished.
 *
 * @param func The callback function.
 * @param data Data to pass to the callback.
 */
void gaim_plugins_register_probe_notify_cb(void (*func)(void *), void *data);

/**
 * Unregisters a function that would be called when probing is finished.
 *
 * @param func The callback function.
 */
void gaim_plugins_unregister_probe_notify_cb(void (*func)(void *));

/**
 * Registers a function that will be called when a plugin is loaded.
 *
 * @param func The callback function.
 * @param data Data to pass to the callback.
 */
void gaim_plugins_register_load_notify_cb(void (*func)(GaimPlugin *, void *),
										  void *data);

/**
 * Unregisters a function that would be called when a plugin is loaded.
 *
 * @param func The callback function.
 */
void gaim_plugins_unregister_load_notify_cb(void (*func)(GaimPlugin *, void *));

/**
 * Registers a function that will be called when a plugin is unloaded.
 *
 * @param func The callback function.
 * @param data Data to pass to the callback.
 */
void gaim_plugins_register_unload_notify_cb(void (*func)(GaimPlugin *, void *),
											void *data);

/**
 * Unregisters a function that would be called when a plugin is unloaded.
 *
 * @param func The callback function.
 */
void gaim_plugins_unregister_unload_notify_cb(void (*func)(GaimPlugin *,
														   void *));

/**
 * Finds a plugin with the specified name.
 *
 * @param name The plugin name.
 *
 * @return The plugin if found, or @c NULL if not found.
 */
GaimPlugin *gaim_plugins_find_with_name(const char *name);

/**
 * Finds a plugin with the specified filename (filename with a path).
 *
 * @param filename The plugin filename.
 *
 * @return The plugin if found, or @c NULL if not found.
 */
GaimPlugin *gaim_plugins_find_with_filename(const char *filename);

/**
 * Finds a plugin with the specified basename (filename without a path).
 *
 * @param basename The plugin basename.
 *
 * @return The plugin if found, or @c NULL if not found.
 */
GaimPlugin *gaim_plugins_find_with_basename(const char *basename);

/**
 * Finds a plugin with the specified plugin ID.
 *
 * @param id The plugin ID.
 *
 * @return The plugin if found, or @c NULL if not found.
 */
GaimPlugin *gaim_plugins_find_with_id(const char *id);

/**
 * Returns a list of all loaded plugins.
 *
 * @return A list of all loaded plugins.
 */
GList *gaim_plugins_get_loaded(void);

/**
 * Returns a list of all protocol plugins.
 *
 * @return A list of all protocol plugins.
 */
GList *gaim_plugins_get_protocols(void);

/**
 * Returns a list of all plugins, whether loaded or not.
 *
 * @return A list of all plugins.
 */
GList *gaim_plugins_get_all(void);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif /* _GAIM_PLUGIN_H_ */
