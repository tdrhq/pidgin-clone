/*
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
 *
 */
#include "gtkinternal.h"

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#endif

#ifdef USE_AO
# include <ao/ao.h>
# include <audiofile.h>
#endif /* USE_AO */

#ifdef USE_NAS_AUDIO
# include <audio/audiolib.h>
# include <audio/soundlib.h>
#endif /* USE_NAS_AUDIO */

#include "debug.h"
#include "notify.h"
#include "prefs.h"
#include "sound.h"
#include "util.h"

#include "gtksound.h"


struct gaim_sound_event {
	char *label;
	char *pref;
	char *def;
};



static gboolean mute_login_sounds = FALSE;
static gboolean mute_sounds = FALSE;
static gboolean sound_initialized = FALSE;

static struct gaim_sound_event sounds[GAIM_NUM_SOUNDS] = {
	{N_("Buddy logs in"), "login", "arrive.wav"},
	{N_("Buddy logs out"), "logout", "leave.wav"},
	{N_("Message received"), "im_recv", "receive.wav"},
	{N_("Message received begins conversation"), "first_im_recv", "receive.wav"},
	{N_("Message sent"), "send_im", "send.wav"},
	{N_("Person enters chat"), "join_chat", "arrive.wav"},
	{N_("Person leaves chat"), "left_chat", "leave.wav"},
	{N_("You talk in chat"), "send_chat_msg", "send.wav"},
	{N_("Others talk in chat"), "chat_msg_recv", "receive.wav"},
	/* this isn't a terminator, it's the buddy pounce default sound event ;-) */
	{NULL, "pounce_default", "redalert.wav"},
	{N_("Someone says your name in chat"), "nick_said", "redalert.wav"}
};

#ifdef USE_AO
static int ao_driver = -1;
#endif /* USE_AO */

static void _pref_sound_method_changed(const char *name, GaimPrefType type,
		gpointer val, gpointer data);

static void gaim_gtk_sound_init(void)
{
	gaim_prefs_add_none("/gaim/gtk/sound");
	gaim_prefs_add_none("/gaim/gtk/sound/enabled");
	gaim_prefs_add_none("/gaim/gtk/sound/file");
	gaim_prefs_add_bool("/gaim/gtk/sound/enabled/login", TRUE);
	gaim_prefs_add_string("/gaim/gtk/sound/file/login", "");
	gaim_prefs_add_bool("/gaim/gtk/sound/enabled/logout", TRUE);
	gaim_prefs_add_string("/gaim/gtk/sound/file/logout", "");
	gaim_prefs_add_bool("/gaim/gtk/sound/enabled/im_recv", TRUE);
	gaim_prefs_add_string("/gaim/gtk/sound/file/im_recv", "");
	gaim_prefs_add_bool("/gaim/gtk/sound/enabled/first_im_recv", FALSE);
	gaim_prefs_add_string("/gaim/gtk/sound/file/first_im_recv", "");
	gaim_prefs_add_bool("/gaim/gtk/sound/enabled/send_im", TRUE);
	gaim_prefs_add_string("/gaim/gtk/sound/file/send_im", "");
	gaim_prefs_add_bool("/gaim/gtk/sound/enabled/join_chat", FALSE);
	gaim_prefs_add_string("/gaim/gtk/sound/file/join_chat", "");
	gaim_prefs_add_bool("/gaim/gtk/sound/enabled/left_chat", FALSE);
	gaim_prefs_add_string("/gaim/gtk/sound/file/left_chat", "");
	gaim_prefs_add_bool("/gaim/gtk/sound/enabled/send_chat_msg", FALSE);
	gaim_prefs_add_string("/gaim/gtk/sound/file/send_chat_msg", "");
	gaim_prefs_add_bool("/gaim/gtk/sound/enabled/chat_msg_recv", FALSE);
	gaim_prefs_add_string("/gaim/gtk/sound/file/chat_msg_recv", "");
	gaim_prefs_add_bool("/gaim/gtk/sound/enabled/nick_said", FALSE);
	gaim_prefs_add_string("/gaim/gtk/sound/file/nick_said", "");
	gaim_prefs_add_bool("/gaim/gtk/sound/enabled/pounce_default", TRUE);
	gaim_prefs_add_string("/gaim/gtk/sound/file/pounce_default", "");
	gaim_prefs_add_bool("/gaim/gtk/sound/conv_focus", TRUE);
	gaim_prefs_add_string("/gaim/gtk/sound/command", "");
	gaim_prefs_add_string("/gaim/gtk/sound/method", "automatic");

#ifdef USE_AO
	gaim_debug(GAIM_DEBUG_INFO, "sound",
			"Initializing sound output drivers.\n");
	ao_initialize();
#endif /* USE_AO */

	gaim_prefs_connect_callback("/gaim/gtk/sound/method",
			_pref_sound_method_changed, NULL);
}


static void gaim_gtk_sound_shutdown(void)
{
#ifdef USE_AO
	ao_shutdown();
#endif
	sound_initialized = FALSE;
}

static void gaim_gtk_sound_play_file(const char *filename)
{
	const char *method;
#if defined(USE_NAS_AUDIO) || defined(USE_AO)
	pid_t pid;
#ifdef USE_AO
	AFfilehandle file;
#endif
#endif

	if (!sound_initialized)
		gaim_prefs_trigger_callback("/gaim/gtk/sound/method");

	if (mute_sounds)
		return;

	method = gaim_prefs_get_string("/gaim/gtk/sound/method");

	if (!strcmp(method, "beep")) {
		gdk_beep();
		return;
	}

	if (!g_file_test(filename, G_FILE_TEST_EXISTS)) {
		char *tmp = g_strdup_printf(_("Unable to play sound because the chosen file (%s) does not exist."), filename);
		gaim_notify_error(NULL, NULL, tmp, NULL);
		g_free(tmp);
		return;
	}

#ifndef _WIN32
	if (!strcmp(method, "custom")) {
		const char *sound_cmd;
		char *command;
		GError *error = NULL;

		sound_cmd = gaim_prefs_get_string("/gaim/gtk/sound/command");

		if (!sound_cmd || *sound_cmd == '\0') {
			gaim_notify_error(NULL, NULL,
							  _("Unable to play sound because the "
								"'Command' sound method has been chosen, "
								"but no command has been set."), NULL);
			return;
		}

		if(strstr(sound_cmd, "%s"))
			command = gaim_strreplace(sound_cmd, "%s", filename);
		else
			command = g_strdup_printf("%s %s", sound_cmd, filename);

		if(!g_spawn_command_line_async(command, &error)) {
			char *tmp = g_strdup_printf(_("Unable to play sound because the configured sound command could not be launched: %s"), error->message);
			gaim_notify_error(NULL, NULL, tmp, NULL);
			g_free(tmp);
			g_error_free(error);
		}

		g_free(command);
		return;
	}
#if defined(USE_NAS_AUDIO) || defined(USE_AO)
	pid = fork();
	if (pid < 0)
		return;
	else if (pid == 0) {
#ifdef USE_NAS_AUDIO
		if (!strcmp(method, "nas")) {
			if (play_file_nas(filename))
				_exit(0);
		}
#endif /* USE_NAS_AUDIO */

#ifdef USE_AO
		file = afOpenFile(filename, "rb", NULL);
		if(file) {
			ao_device *device;
			ao_sample_format format;
			int in_fmt;
			int bytes_per_frame;

			format.rate = afGetRate(file, AF_DEFAULT_TRACK);
			format.channels = afGetChannels(file, AF_DEFAULT_TRACK);
			afGetSampleFormat(file, AF_DEFAULT_TRACK, &in_fmt,
					&format.bits);

			/* XXX: libao doesn't seem to like 8-bit sounds, so we'll
			 * let libaudiofile make them a bit better for us */
			if(format.bits == 8)
				format.bits = 16;

			afSetVirtualSampleFormat(file, AF_DEFAULT_TRACK,
					AF_SAMPFMT_TWOSCOMP, format.bits);

#if __BYTE_ORDER == __BIG_ENDIAN
			format.byte_format = AO_FMT_BIG;
			afSetVirtualByteOrder(file, AF_DEFAULT_TRACK,
					AF_BYTEORDER_BIGENDIAN);
#elif __BYTE_ORDER == __LITTLE_ENDIAN
			format.byte_format = AO_FMT_LITTLE;
			afSetVirtualByteOrder(file, AF_DEFAULT_TRACK,
					AF_BYTEORDER_LITTLEENDIAN);
#endif

			bytes_per_frame = format.bits * format.channels / 8;

			device = ao_open_live(ao_driver, &format, NULL);

			if(device) {
				int frames_read;
				char buf[4096];
				int buf_frames = sizeof(buf) / bytes_per_frame;

				while((frames_read = afReadFrames(file, AF_DEFAULT_TRACK,
								buf, buf_frames))) {
					if(!ao_play(device, buf, frames_read * bytes_per_frame))
						break;
				}
				ao_close(device);
			}
			afCloseFile(file);
		}
		ao_shutdown();
#endif /* USE_AO */
		_exit(0);
	}
#else /* USE_NAS_AUDIO || USE_AO */
	gdk_beep();
	return;
#endif /* USE_NAS_AUDIO || USE_AO */
#else /* _WIN32 */
	gaim_debug(GAIM_DEBUG_INFO, "sound", "Playing %s\n", filename);

	if (!PlaySound(filename, 0, SND_ASYNC | SND_FILENAME))
		gaim_debug(GAIM_DEBUG_ERROR, "sound", "Error playing sound.\n");
#endif /* _WIN32 */
}

static void gaim_gtk_sound_play_event(GaimSoundEventID event)
{
	char *enable_pref;
	char *file_pref;

	if ((event == GAIM_SOUND_BUDDY_ARRIVE) && mute_login_sounds)
		return;

	if (event >= GAIM_NUM_SOUNDS) {
		gaim_debug(GAIM_DEBUG_MISC, "sound",
				   "got request for unknown sound: %d\n", event);
		return;
	}

	enable_pref = g_strdup_printf("/gaim/gtk/sound/enabled/%s",
			sounds[event].pref);
	file_pref = g_strdup_printf("/gaim/gtk/sound/file/%s", sounds[event].pref);

	/* check NULL for sounds that don't have an option, ie buddy pounce */
	if (gaim_prefs_get_bool(enable_pref)) {
		char *filename = g_strdup(gaim_prefs_get_string(file_pref));
		if(!filename || !strlen(filename)) {
			if(filename) g_free(filename);
			filename = g_build_filename(DATADIR, "sounds", "gaim", sounds[event].def, NULL);
		}

		gaim_sound_play_file(filename);
		g_free(filename);
	}

	g_free(enable_pref);
	g_free(file_pref);
}

static GaimSoundUiOps sound_ui_ops =
{
	gaim_gtk_sound_init,
	gaim_gtk_sound_shutdown,
	gaim_gtk_sound_play_file,
	gaim_gtk_sound_play_event
};

GaimSoundUiOps *
gaim_gtk_sound_get_ui_ops(void)
{
	return &sound_ui_ops;
}


static void _pref_sound_method_changed(const char *name, GaimPrefType type,
		gpointer val, gpointer data) {
	if(type != GAIM_PREF_STRING || strcmp(name, "/gaim/gtk/sound/method"))
		return;

	sound_initialized = TRUE;

#ifdef USE_AO
	ao_driver = -1;

	if(!strcmp(val, "esd"))
		ao_driver = ao_driver_id("esd");
	else if(!strcmp(val, "arts"))
		ao_driver = ao_driver_id("arts");
	else if(!strcmp(val, "automatic"))
		ao_driver = ao_default_driver_id();

	if(ao_driver != -1) {
		ao_info *info = ao_driver_info(ao_driver);
		gaim_debug(GAIM_DEBUG_INFO, "sound",
				   "Sound output driver loaded: %s\n", info->name);
	}
#endif /* USE_AO */
#ifdef USE_NAS
	if (!strcmp(val, "nas"))
		gaim_debug(GAIM_DEBUG_INFO, "sound",
				   "Sound output driver loaded: NAS output\n");
#endif /* USE_NAS */
}

#ifdef USE_NAS_AUDIO
static gboolean play_file_nas(const char *filename)
{
	AuServer *nas_serv;
	gboolean ret = FALSE;

	if((nas_serv = AuOpenServer(NULL, 0, NULL, 0, NULL, NULL))) {
		ret = AuSoundPlaySynchronousFromFile(nas_serv, filename, 100);
		AuCloseServer(nas_serv);
	}

	return ret;
}

#endif /* USE_NAS_AUDIO */

void gaim_gtk_sound_set_mute(gboolean mute)
{
	mute_sounds = mute;
}

gboolean gaim_gtk_sound_get_mute()
{
	return mute_sounds;
}

void gaim_gtk_sound_set_login_mute(gboolean mute)
{
	mute_login_sounds = mute;
}

const char *gaim_gtk_sound_get_event_option(GaimSoundEventID event)
{
	if(event >= GAIM_NUM_SOUNDS)
		return 0;

	return sounds[event].pref;
}

char *gaim_gtk_sound_get_event_label(GaimSoundEventID event)
{
	if(event >= GAIM_NUM_SOUNDS)
		return NULL;

	return sounds[event].label;
}

