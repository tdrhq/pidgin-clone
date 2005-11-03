/*
 * gaim - WinGaim Options Plugin
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
 *
 */
#include <gtk/gtk.h>
#include <gdk/gdkwin32.h>

#include "internal.h"

#include "core.h"
#include "debug.h"
#include "prefs.h"
#include "signals.h"
#include "version.h"

#include "gtkappbar.h"
#include "gtkblist.h"
#include "gtkconv.h"
#include "gtkplugin.h"
#include "gtkprefs.h"
#include "gtkutils.h"

/*
 *  MACROS & DEFINES
 */
#define WINPREFS_PLUGIN_ID               "gtk-win-prefs"

/*
 *  LOCALS
 */
static const char *PREF_DBLIST_DOCKABLE = "/plugins/gtk/win32/winprefs/dblist_dockable";
static const char *PREF_DBLIST_DOCKED =   "/plugins/gtk/win32/winprefs/dblist_docked";
static const char *PREF_DBLIST_HEIGHT =   "/plugins/gtk/win32/winprefs/dblist_height";
static const char *PREF_DBLIST_SIDE =     "/plugins/gtk/win32/winprefs/dblist_side";
static const char *PREF_BLIST_ON_TOP =    "/plugins/gtk/win32/winprefs/blist_on_top";
static const char *PREF_IM_BLINK =        "/plugins/gtk/win32/winprefs/im_blink";
/* Deprecated */
static const char *PREF_DBLIST_ON_TOP =   "/plugins/gtk/win32/winprefs/dblist_on_top";

static GaimPlugin *handle = NULL;
static GtkAppBar *blist_ab = NULL;
static GtkWidget *blist = NULL;

/* flash info */

struct _WGAIM_FLASH_INFO {
	guint t_handle;
	guint sig_handler;
};

enum {
	BLIST_TOP_NEVER = 0,
	BLIST_TOP_ALWAYS,
	BLIST_TOP_DOCKED,
};

typedef struct _WGAIM_FLASH_INFO WGAIM_FLASH_INFO;

typedef BOOL (CALLBACK* LPFNFLASHWINDOWEX)(PFLASHWINFO);

static LPFNFLASHWINDOWEX MyFlashWindowEx = NULL;

/*
 *  CODE
 */

/* BLIST DOCKING */

static void blist_save_state() {
	if(blist_ab) {
		if(gaim_prefs_get_bool(PREF_DBLIST_DOCKABLE) && blist_ab->docked) {
			gaim_prefs_set_int(PREF_DBLIST_HEIGHT, blist_ab->undocked_height);
			gaim_prefs_set_int(PREF_DBLIST_SIDE, blist_ab->side);
			gaim_prefs_set_bool(PREF_DBLIST_DOCKED, blist_ab->docked);
		} else
			gaim_prefs_set_bool(PREF_DBLIST_DOCKED, FALSE);
	}
}

static void blist_set_ontop(gboolean val) {
	if (!blist)
		return;

	if (val == TRUE)
		SetWindowPos(GDK_WINDOW_HWND(blist->window), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	else
		SetWindowPos(GDK_WINDOW_HWND(blist->window), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

}

static void blist_dock_cb(gboolean val) {
	if (val == TRUE) {
		gaim_debug_info(WINPREFS_PLUGIN_ID, "Blist Docking..\n");
		if (gaim_prefs_get_int(PREF_BLIST_ON_TOP) != BLIST_TOP_NEVER)
			blist_set_ontop(TRUE);
	} else {
		gaim_debug_info(WINPREFS_PLUGIN_ID, "Blist Undocking..\n");
		if (gaim_prefs_get_int(PREF_BLIST_ON_TOP) == BLIST_TOP_ALWAYS)
			blist_set_ontop(TRUE);
		else
			blist_set_ontop(FALSE);
	}
}

static void blist_set_dockable(gboolean val) {
	if (val == TRUE) {
		if (blist_ab == NULL && blist != NULL) {
			blist_ab = gtk_appbar_add(blist);
			gtk_appbar_add_dock_cb(blist_ab, blist_dock_cb);
		}
	} else {
		if (blist_ab != NULL) {
			gtk_appbar_remove(blist_ab);
			blist_ab = NULL;
		}

		if (gaim_prefs_get_int(PREF_BLIST_ON_TOP) == BLIST_TOP_ALWAYS)
			blist_set_ontop(TRUE);
		else
			blist_set_ontop(FALSE);
	}
}

/* PLUGIN CALLBACKS */

/* We need this because the blist destroy cb won't be called before the
   plugin is unloaded, when quitting */
static void gaim_quit_cb() {
	gaim_debug_info(WINPREFS_PLUGIN_ID, "gaim_quit_cb: removing appbar\n");
	blist_save_state();
	blist_set_dockable(FALSE);
}

static void blist_create_cb(GaimBuddyList *gaim_blist, void *data) {
	gaim_debug_info(WINPREFS_PLUGIN_ID, "buddy list created\n");

	blist = GAIM_GTK_BLIST(gaim_blist)->window;

	if (gaim_prefs_get_bool(PREF_DBLIST_DOCKABLE)) {
		blist_set_dockable(TRUE);
		if (gaim_prefs_get_bool(PREF_DBLIST_DOCKED)) {
			blist_ab->undocked_height = gaim_prefs_get_int(PREF_DBLIST_HEIGHT);
			gtk_appbar_dock(blist_ab,
							gaim_prefs_get_int(PREF_DBLIST_SIDE));
			if (gaim_prefs_get_int(PREF_BLIST_ON_TOP) == BLIST_TOP_DOCKED)
				blist_set_ontop(TRUE);
		}
	}

	if (gaim_prefs_get_int(PREF_BLIST_ON_TOP) == BLIST_TOP_ALWAYS)
		blist_set_ontop(TRUE);

}

/* AUTOSTART */

static int open_run_key(PHKEY phKey, REGSAM samDesired) {
	/* First try current user key (for WinNT & Win2k +), fall back to local machine */
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER,
					"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
					0,  samDesired,  phKey));
	else if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
					0,  samDesired,  phKey));
	else {
		gaim_debug_error(WINPREFS_PLUGIN_ID, "open_run_key: Could not open key for writing value\n");
		return 0;
	}
	return 1;
}

/* WIN PREFS GENERAL */

static void
winprefs_set_autostart(GtkWidget *w)
{
	HKEY hKey;

	if (!open_run_key(&hKey, KEY_SET_VALUE))
		return;
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w))) {
		char buffer[1024];
		DWORD size;

		if ((size = GetModuleFileName(wgaim_hinstance(),
									  (LPBYTE)buffer, sizeof(buffer)))==0) {
			gaim_debug_error(WINPREFS_PLUGIN_ID, "GetModuleFileName Error.. Could not set Gaim autostart.\n");
			RegCloseKey(hKey);
			return;
		}

		/* Now set value of new key */
		if (ERROR_SUCCESS != RegSetValueEx(hKey, "Gaim", 0, REG_SZ, buffer, size))
			gaim_debug_error(WINPREFS_PLUGIN_ID, "Could not set registry key value\n");
	} else {
		if (ERROR_SUCCESS != RegDeleteValue(hKey, "Gaim"))
			gaim_debug_error(WINPREFS_PLUGIN_ID, "Could not delete registry key value\n");
	}
	RegCloseKey(hKey);
}

static void
winprefs_set_blist_dockable (const char *pref, GaimPrefType type,
		gpointer value, gpointer user_data)
{
	blist_set_dockable(GPOINTER_TO_INT(value));
}

static void
winprefs_set_blist_ontop (const char *pref, GaimPrefType type,
		gpointer value, gpointer user_data)
{
	gint setting = gaim_prefs_get_int(PREF_BLIST_ON_TOP);
	if ((setting == BLIST_TOP_DOCKED && blist_ab && blist_ab->docked)
		|| setting == BLIST_TOP_ALWAYS)
		blist_set_ontop(TRUE);
	else
		blist_set_ontop(FALSE);
}

static void load_winver_specific_procs (void) {
	/* Used for Win98+ and WinNT5+ */
	MyFlashWindowEx = (LPFNFLASHWINDOWEX)wgaim_find_and_loadproc("user32.dll", "FlashWindowEx" );
}

/* Window flasher */
static gboolean flash_window_cb (gpointer data) {
	FlashWindow((HWND)data, TRUE);
	return TRUE;
}

static int
halt_flash_filter (GtkWidget *widget, GdkEventFocus *event, gpointer data)
{
	if (MyFlashWindowEx) {
		HWND hWnd = data;
		FLASHWINFO info;

		if (!IsWindow(hWnd))
			return 0;

		memset(&info, 0, sizeof(FLASHWINFO));
		info.cbSize = sizeof(FLASHWINFO);
		info.hwnd = hWnd;
		info.dwFlags = FLASHW_STOP;
		info.dwTimeout = 0;
		MyFlashWindowEx(&info);

	} else {
		WGAIM_FLASH_INFO *finfo = data;
		/* Stop flashing and remove filter */
		gaim_debug(GAIM_DEBUG_INFO, "wgaim", "Removing timeout\n");
		gaim_timeout_remove(finfo->t_handle);
		gaim_debug(GAIM_DEBUG_INFO, "wgaim", "Disconnecting signal handler\n");
		g_signal_handler_disconnect(G_OBJECT(widget),finfo->sig_handler);
		gaim_debug(GAIM_DEBUG_INFO, "wgaim", "done\n");
		g_free(finfo);
	}
	return 0;
}

/* FlashWindowEx is only supported by Win98+ and WinNT5+. If it's
   not supported we do it our own way */
static void
wgaim_conv_im_blink (GaimAccount *account, char *sender, char *message, int flags)
{
	GaimConversation *conv;
	GaimGtkWindow *win;
	GtkWidget *window;
	if (gaim_prefs_get_bool(PREF_IM_BLINK) == FALSE)
		return;

	conv = gaim_find_conversation_with_account(GAIM_CONV_TYPE_IM, sender, account);
	if (conv == NULL) {
	  gaim_debug_info("winprefs", "gar!\n");
	  return;
	}
	win = gaim_gtkconv_get_window(GAIM_GTK_CONVERSATION(conv));
	if (win == NULL) {
	  gaim_debug_info("winprefs", "gar2!\n");
	  return;
	}
	window = win->window;

	if (MyFlashWindowEx) {
		FLASHWINFO info;
		if (GetForegroundWindow() == GDK_WINDOW_HWND(window->window))
			return;

		memset(&info, 0, sizeof(FLASHWINFO));
		info.cbSize = sizeof(FLASHWINFO);
		info.hwnd = GDK_WINDOW_HWND(window->window);
		info.dwFlags = FLASHW_ALL;
		info.dwTimeout = 0;
		MyFlashWindowEx(&info);
		/* Stop flashing when window receives focus */
		g_signal_connect(G_OBJECT(window), "focus-in-event",
						 G_CALLBACK(halt_flash_filter), info.hwnd);
	} else {
		WGAIM_FLASH_INFO *finfo = g_new0(WGAIM_FLASH_INFO, 1);

		/* Start Flashing window */
		finfo->t_handle = gaim_timeout_add(1000, flash_window_cb,
										   GDK_WINDOW_HWND(window->window));
		finfo->sig_handler = g_signal_connect(G_OBJECT(window),
									"focus-in-event",
									G_CALLBACK(halt_flash_filter), finfo);
	}
}


/*
 *  EXPORTED FUNCTIONS
 */

gboolean plugin_load(GaimPlugin *plugin) {
	/* Find out how to go blinky */
	load_winver_specific_procs();

	handle = plugin;

	/* blist docking init */
	if (gaim_get_blist() && GAIM_GTK_BLIST(gaim_get_blist()) && GAIM_GTK_BLIST(gaim_get_blist())->window) {
		blist_create_cb(gaim_get_blist(), NULL);
	}

	/* This really shouldn't happen anymore generally, but if for some strange
	   reason, the blist is recreated, we need to set it up again. */
	gaim_signal_connect(gaim_gtk_blist_get_handle(), "gtkblist-created", plugin, GAIM_CALLBACK(blist_create_cb), NULL);

	gaim_signal_connect(gaim_conversations_get_handle(), "received-im-msg", plugin, GAIM_CALLBACK(wgaim_conv_im_blink), NULL);

	gaim_signal_connect((void*)gaim_get_core(), "quitting", plugin, GAIM_CALLBACK(gaim_quit_cb), NULL);

	gaim_prefs_connect_callback(handle, PREF_BLIST_ON_TOP,
								winprefs_set_blist_ontop, NULL);
	gaim_prefs_connect_callback(handle, PREF_DBLIST_DOCKABLE,
								winprefs_set_blist_dockable, NULL);

	return TRUE;
}

gboolean plugin_unload(GaimPlugin *plugin) {
	blist_set_dockable(FALSE);
	blist_set_ontop(FALSE);

	return TRUE;
}

static GtkWidget* get_config_frame(GaimPlugin *plugin) {
	GtkWidget *ret;
	GtkWidget *vbox;
	GtkWidget *button;
	char* gtk_version = NULL;
	HKEY hKey = HKEY_CURRENT_USER;

	ret = gtk_vbox_new(FALSE, 18);
	gtk_container_set_border_width (GTK_CONTAINER (ret), 12);

	gtk_version = g_strdup_printf("GTK+ %u.%u.%u\nGlib %u.%u.%u",
		gtk_major_version, gtk_minor_version, gtk_micro_version,
		glib_major_version, glib_minor_version, glib_micro_version);

	/* Display Installed GTK+ Runtime Version */
	if(gtk_version) {
		GtkWidget *label;
		vbox = gaim_gtk_make_frame(ret, _("GTK+ Runtime Version"));
		label = gtk_label_new(gtk_version);
		gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
		gtk_widget_show(label);
		g_free(gtk_version);
	}

	/* Autostart */
	vbox = gaim_gtk_make_frame (ret, _("Startup"));
	button = gtk_check_button_new_with_mnemonic(_("_Start Gaim on Windows startup"));
	gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
	if(open_run_key(&hKey, KEY_QUERY_VALUE)) {
		if(ERROR_SUCCESS == RegQueryValueEx(hKey, "Gaim", 0, NULL, NULL, NULL)) {
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
		}
	}
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(winprefs_set_autostart), NULL);
	gtk_widget_show(button);

	/* Buddy List */
	vbox = gaim_gtk_make_frame (ret, _("Buddy List"));
	gaim_gtk_prefs_checkbox(_("_Dockable Buddy List"),
							PREF_DBLIST_DOCKABLE, vbox);

	/* Blist On Top */
	gaim_gtk_prefs_dropdown(vbox, _("_Keep Buddy List window on top:"),
							GAIM_PREF_INT, PREF_BLIST_ON_TOP,
							_("Never"),             BLIST_TOP_NEVER,
							_("Always"),            BLIST_TOP_ALWAYS,
							/* XXX: Did this ever work? */
							_("Only when docked"),  BLIST_TOP_DOCKED,
							NULL);

	/* Conversations */
	vbox = gaim_gtk_make_frame (ret, _("Conversations"));
	gaim_gtk_prefs_checkbox(_("_Flash window when messages are received"),
							PREF_IM_BLINK, vbox);

	gtk_widget_show_all(ret);
	return ret;
}

static GaimGtkPluginUiInfo ui_info =
{
	get_config_frame
};

static GaimPluginInfo info =
{
	GAIM_PLUGIN_MAGIC,
	GAIM_MAJOR_VERSION,
	GAIM_MINOR_VERSION,
	GAIM_PLUGIN_STANDARD,
	GAIM_GTK_PLUGIN_TYPE,
	0,
	NULL,
	GAIM_PRIORITY_DEFAULT,
	WINPREFS_PLUGIN_ID,
	N_("WinGaim Options"),
	VERSION,
	N_("Options specific to Windows Gaim."),
	N_("Provides options specific to Windows Gaim, such as buddy list docking and conversation flashing."),
	"Herman Bloggs <hermanator12002@yahoo.com>",
	GAIM_WEBSITE,
	plugin_load,
	plugin_unload,
	NULL,
	&ui_info,
	NULL,
	NULL,
	NULL
};

static void
init_plugin(GaimPlugin *plugin)
{
        gaim_prefs_add_none("/plugins/gtk");
        gaim_prefs_add_none("/plugins/gtk/win32");
        gaim_prefs_add_none("/plugins/gtk/win32/winprefs");
        gaim_prefs_add_bool(PREF_DBLIST_DOCKABLE, FALSE);
        gaim_prefs_add_bool(PREF_DBLIST_DOCKED, FALSE);
        gaim_prefs_add_int(PREF_DBLIST_HEIGHT, 0);
        gaim_prefs_add_int(PREF_DBLIST_SIDE, 0);
        gaim_prefs_add_bool(PREF_IM_BLINK, TRUE);

		/* Convert old preferences */
		if (gaim_prefs_exists(PREF_DBLIST_ON_TOP)) {
			gint blist_top = BLIST_TOP_NEVER;
			if (gaim_prefs_get_bool(PREF_BLIST_ON_TOP))
				blist_top = BLIST_TOP_ALWAYS;
			else if (gaim_prefs_get_bool(PREF_DBLIST_ON_TOP))
				blist_top = BLIST_TOP_DOCKED;
			gaim_prefs_remove(PREF_BLIST_ON_TOP);
			gaim_prefs_remove(PREF_DBLIST_ON_TOP);
			gaim_prefs_add_int(PREF_BLIST_ON_TOP, blist_top);
		} else
			gaim_prefs_add_int(PREF_BLIST_ON_TOP, BLIST_TOP_NEVER);
}

GAIM_INIT_PLUGIN(winprefs, init_plugin, info)
