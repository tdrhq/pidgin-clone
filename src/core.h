/**
 * @file core.h Gaim Core API
 * @defgroup core Gaim Core
 *
 * gaim
 *
 * Copyright (C) 2003 Christian Hammond <chipx86@gnupdate.org>
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
#ifndef _GAIM_CORE_H_
#define _GAIM_CORE_H_

typedef struct GaimCore GaimCore;

typedef struct
{
	void (*ui_prefs_init)(void);
	void (*debug_ui_init)(void); /* Unfortunate necessity. */
	void (*ui_init)(void);
	void (*quit)(void);

} GaimCoreUiOps;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the core of gaim.
 *
 * This will setup preferences for all the core subsystems.
 *
 * @param ui The ID of the UI using the core. This should be a
 *           unique ID, registered with the gaim team.
 *
 * @return @c TRUE if successful, or @c FALSE otherwise.
 */
gboolean gaim_core_init(const char *ui);

/**
 * Quits the core of gaim, which, depending on the UI, may quit the
 * application using the gaim core.
 */
void gaim_core_quit(void);

/**
 * Returns the version of the core library.
 *
 * @return The version of the core library.
 */
const char *gaim_core_get_version(void);

/**
 * Returns whether or not there are any mainloop events pending.
 *
 * @return TRUE if there are mainloop events pending. FALSE otherwise.
 */
gboolean gaim_core_mainloop_events_pending(void);

/**
 * Iterates once through the gaim mainloop.
 *
 * This is in actuality a wrapper around glib's mainloop iteration
 * function, but provides a nice, healthy level of abstraction.
 *
 * All UIs not using glib must call this in a timer.
 */
void gaim_core_mainloop_iteration(void);

/**
 * Iterates through all remaining events in the mainloop.
 */
void gaim_core_mainloop_finish_events(void);

/**
 * Returns the ID of the UI that is using the core.
 *
 * @return The ID of the UI that is currently using the core.
 */
const char *gaim_core_get_ui(void);

/**
 * Returns a handle to the gaim core.
 *
 * This is used for such things as signals.
 */
GaimCore *gaim_get_core(void);

/**
 * Sets the UI ops for the core.
 *
 * @param A UI ops structure for the core.
 */
void gaim_core_set_ui_ops(GaimCoreUiOps *ops);

/**
 * Returns the UI ops for the core.
 *
 * @return The core's UI ops structure.
 */
GaimCoreUiOps *gaim_core_get_ui_ops(void);

#ifdef __cplusplus
}
#endif

#endif /* _GAIM_CORE_H_ */

/*

                                                  /===-
                                                `//"\\   """"`---.___.-""
             ______-==|                         | |  \\           _-"`
       __--"""  ,-/-==\\                        | |   `\        ,'
    _-"       /'    |  \\            ___         / /      \      /
  .'        /       |   \\         /"   "\    /' /        \   /'
 /  ____  /         |    \`\.__/-""  D O   \_/'  /          \/'
/-'"    """""---__  |     "-/"   O G     R   /'        _--"`
                  \_|      /   R    __--_  t ),   __--""
                    '""--_/  T   _-"_>--<_\ h '-" \
                   {\__--_/}    / \\__>--<__\ e B  \
                   /'   (_/  _-"  | |__>--<__|   U  |
                  |   _/) )-"     | |__>--<__|  R   |
                  / /" ,_/       / /__>---<__/ N    |
                 o-o _//        /-"_>---<__-" I    /
                 (^("          /"_>---<__-  N   _-"
                ,/|           /__>--<__/  A  _-"
             ,//('(          |__>--<__|  T  /                  .----_
            ( ( '))          |__>--<__|    |                 /' _---_"\
         `-)) )) (           |__>--<__|  O |               /'  /     "\`\
        ,/,'//( (             \__>--<__\  R \            /'  //        ||
      ,( ( ((, ))              "-__>--<_"-_  "--____---"' _/'/        /'
    `"/  )` ) ,/|                 "-_">--<_/-__       __-" _/
  ._-"//( )/ )) `                    ""-'_/_/ /"""""""__--"
   ;'( ')/ ,)(                              """"""""""
  ' ') '( (/
    '   '  `

*/
