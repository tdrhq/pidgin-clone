#include "internal.h"

#include "blist.h"
#include "conversation.h"
#include "debug.h"
#include "signals.h"
#include "version.h"

#include "plugin.h"
#include "pluginpref.h"
#include "prefs.h"

#define STATENOTIFY_PLUGIN_ID "core-statenotify"

static void
write_status(GaimBuddy *buddy, const char *message)
{
	GaimConversation *conv;
	const char *who;
	char buf[256];

	conv = gaim_find_conversation_with_account(buddy->name, buddy->account);

	if (conv == NULL)
		return;

	who = gaim_buddy_get_alias(buddy);

	g_snprintf(buf, sizeof(buf), message, who);

	gaim_conversation_write(conv, NULL, buf, GAIM_MESSAGE_SYSTEM, time(NULL));
}

static void
buddy_away_cb(GaimBuddy *buddy, void *data)
{
	if (gaim_prefs_get_bool("/plugins/core/statenotify/notify_away"))
		write_status(buddy, _("%s has gone away."));
}

static void
buddy_unaway_cb(GaimBuddy *buddy, void *data)
{
	if (gaim_prefs_get_bool("/plugins/core/statenotify/notify_away"))
		write_status(buddy, _("%s is no longer away."));
}

static void
buddy_idle_cb(GaimBuddy *buddy, void *data)
{
	if (gaim_prefs_get_bool("/plugins/core/statenotify/notify_idle"))
		write_status(buddy, _("%s has become idle."));
}

static void
buddy_unidle_cb(GaimBuddy *buddy, void *data)
{
	if (gaim_prefs_get_bool("/plugins/core/statenotify/notify_idle"))
		write_status(buddy, _("%s is no longer idle."));
}

static GaimPluginPrefFrame *
get_plugin_pref_frame(GaimPlugin *plugin)
{
	GaimPluginPrefFrame *frame;
	GaimPluginPref *ppref;

	frame = gaim_plugin_pref_frame_new();

	ppref = gaim_plugin_pref_new_with_label(_("Notify When"));
	gaim_plugin_pref_frame_add(frame, ppref);

	ppref = gaim_plugin_pref_new_with_name_and_label("/plugins/core/statenotify/notify_away", _("Buddy Goes _Away"));
	gaim_plugin_pref_frame_add(frame, ppref);
	
	ppref = gaim_plugin_pref_new_with_name_and_label("/plugins/core/statenotify/notify_idle", _("Buddy Goes _Idle"));
	gaim_plugin_pref_frame_add(frame, ppref);
	
	return frame;
}

static gboolean
plugin_load(GaimPlugin *plugin)
{
	void *blist_handle = gaim_blist_get_handle();

	gaim_signal_connect(blist_handle, "buddy-away",
						plugin, GAIM_CALLBACK(buddy_away_cb), NULL);
	gaim_signal_connect(blist_handle, "buddy-back",
						plugin, GAIM_CALLBACK(buddy_unaway_cb), NULL);
	gaim_signal_connect(blist_handle, "buddy-idle",
						plugin, GAIM_CALLBACK(buddy_idle_cb), NULL);
	gaim_signal_connect(blist_handle, "buddy-unidle",
						plugin, GAIM_CALLBACK(buddy_unidle_cb), NULL);

	return TRUE;
}

static GaimPluginUiInfo prefs_info =
{
	get_plugin_pref_frame
};

static GaimPluginInfo info =
{
	GAIM_PLUGIN_MAGIC,
	GAIM_MAJOR_VERSION,
	GAIM_MINOR_VERSION,
	GAIM_PLUGIN_STANDARD,                             /**< type           */
	NULL,                                             /**< ui_requirement */
	0,                                                /**< flags          */
	NULL,                                             /**< dependencies   */
	GAIM_PRIORITY_DEFAULT,                            /**< priority       */

	STATENOTIFY_PLUGIN_ID,                            /**< id             */
	N_("Buddy State Notification"),                   /**< name           */
	VERSION,                                          /**< version        */
	                                                  /**  summary        */
	N_("Notifies in a conversation window when a buddy goes or returns from "
	   "away or idle."),
	                                                  /**  description    */
	N_("Notifies in a conversation window when a buddy goes or returns from "
	   "away or idle."),
	"Christian Hammond <chipx86@gnupdate.org>",       /**< author         */
	GAIM_WEBSITE,                                     /**< homepage       */

	plugin_load,                                      /**< load           */
	NULL,                                             /**< unload         */
	NULL,                                             /**< destroy        */

	NULL,                                             /**< ui_info        */
	NULL,                                             /**< extra_info     */
	&prefs_info,                                      /**< prefs_info     */
	NULL
};

static void
init_plugin(GaimPlugin *plugin)
{
	gaim_prefs_add_none("/plugins/core/statenotify");
	gaim_prefs_add_bool("/plugins/core/statenotify/notify_away", TRUE);
	gaim_prefs_add_bool("/plugins/core/statenotify/notify_idle", TRUE);
}

GAIM_INIT_PLUGIN(statenotify, init_plugin, info)
