/**
 * @file signals.h Signal API
 * @ingroup core
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
#ifndef _GAIM_SIGNAL_H_
#define _GAIM_SIGNAL_H_

#include <glib.h>

#define GAIM_CALLBACK(func) ((GaimCallback)func)

typedef void (*GaimCallback)(void);
typedef void (*GaimSignalMarshalFunc)(GaimCallback cb, va_list args,
									  void *data, void **return_val);

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/
/** @name Signal API                                                      */
/**************************************************************************/
/*@{*/

/**
 * Registers a signal in an instance.
 *
 * @param instance The instance to register the signal for.
 * @param signal   The signal name.
 * @param marshal  The marshal function.
 *
 * @return The signal ID local to that instance, or 0 if the signal
 *         couldn't be registered.
 */
gulong gaim_signal_register(void *instance, const char *signal,
							GaimSignalMarshalFunc marshal);

/**
 * Unregisters a signal in an instance.
 *
 * @param instance The instance to unregister the signal for.
 * @param signal   The signal name.
 */
void gaim_signal_unregister(void *instance, const char *signal);

/**
 * Unregisters all signals in an instance.
 *
 * @param instance The instance to unregister the signal for.
 */
void gaim_signals_unregister_by_instance(void *instance);

/**
 * Connects a signal handler to a signal for a particular object.
 *
 * Take care not to register a handler function twice. Gaim will
 * not correct any mistakes for you in this area.
 *
 * @param instance The instance to connect to.
 * @param signal   The name of the signal to connect.
 * @param handle   The handle of the receiver.
 * @param func     The callback function.
 * @param data     The data to pass to the callback function.
 *
 * @return The signal handler ID.
 *
 * @see gaim_signal_disconnect()
 */
gulong gaim_signal_connect(void *instance, const char *signal,
						   void *handle, GaimCallback func, void *data);

/**
 * Connects a signal handler to a signal for a particular object.
 *
 * The signal handler will take a va_args of arguments, instead of
 * individual arguments.
 *
 * Take care not to register a handler function twice. Gaim will
 * not correct any mistakes for you in this area.
 *
 * @param instance The instance to connect to.
 * @param signal   The name of the signal to connect.
 * @param handle   The handle of the receiver.
 * @param func     The callback function.
 * @param data     The data to pass to the callback function.
 *
 * @return The signal handler ID.
 *
 * @see gaim_signal_disconnect()
 */
gulong gaim_signal_connect_vargs(void *instance, const char *signal,
								 void *handle, GaimCallback func, void *data);

/**
 * Disconnects a signal handler from a signal on an object.
 *
 * @param instance The instance to disconnect from.
 * @param signal   The name of the signal to disconnect.
 * @param handle   The handle of the receiver.
 * @param func     The registered function to disconnect.
 *
 * @see gaim_signal_connect()
 */
void gaim_signal_disconnect(void *instance, const char *signal,
							void *handle, GaimCallback func);

/**
 * Removes all callbacks associated with a receiver handle.
 *
 * @param handle The receiver handle.
 */
void gaim_signals_disconnect_by_handle(void *handle);

/**
 * Emits a signal.
 *
 * @param instance The instance emitting the signal.
 * @param signal   The signal being emitted.
 *
 * @see gaim_signal_connect()
 * @see gaim_signal_disconnect()
 */
void gaim_signal_emit(void *instance, const char *signal, ...);

/**
 * Emits a signal, using a va_list of arguments.
 *
 * @param instance The instance emitting the signal.
 * @param signal   The signal being emitted.
 * @param args     The arguments list.
 *
 * @see gaim_signal_connect()
 * @see gaim_signal_disconnect()
 */
void gaim_signal_emit_vargs(void *instance, const char *signal, va_list args);

/**
 * Emits a signal and returns the return value from the last handler.
 *
 * @param instance The instance emitting the signal.
 * @param signal   The signal being emitted.
 *
 * @return The return value from the last handler.
 */
void *gaim_signal_emit_return_1(void *instance, const char *signal, ...);

/**
 * Emits a signal and returns the return value from the last handler.
 *
 * @param instance The instance emitting the signal.
 * @param signal   The signal being emitted.
 * @param args     The arguments list.
 *
 * @return The return value from the last handler.
 */
void *gaim_signal_emit_vargs_return_1(void *instance, const char *signal,
									  va_list args);

/**
 * Initializes the signals subsystem.
 */
void gaim_signals_init();

/**
 * Uninitializes the signals subsystem.
 */
void gaim_signals_uninit();

/*@}*/

/**************************************************************************/
/** @name Marshal Functions                                               */
/**************************************************************************/
/*@{*/

void gaim_marshal_VOID(
		GaimCallback cb, va_list args, void *data, void **return_val);
void gaim_marshal_VOID__POINTER(
		GaimCallback cb, va_list args, void *data, void **return_val);
void gaim_marshal_VOID__POINTER_POINTER(
		GaimCallback cb, va_list args, void *data, void **return_val);
void gaim_marshal_VOID__POINTER_POINTER_UINT(
		GaimCallback cb, va_list args, void *data, void **return_val);
void gaim_marshal_VOID__POINTER_POINTER_POINTER(
		GaimCallback cb, va_list args, void *data, void **return_val);
void gaim_marshal_VOID__POINTER_POINTER_POINTER_POINTER(
		GaimCallback cb, va_list args, void *data, void **return_val);
void gaim_marshal_VOID__POINTER_POINTER_POINTER_UINT(
		GaimCallback cb, va_list args, void *data, void **return_val);
void gaim_marshal_VOID__POINTER_POINTER_POINTER_UINT_UINT(
		GaimCallback cb, va_list args, void *data, void **return_val);

void gaim_marshal_BOOLEAN__POINTER(
		GaimCallback cb, va_list args, void *data, void **return_val);
void gaim_marshal_BOOLEAN__POINTER_POINTER(
		GaimCallback cb, va_list args, void *data, void **return_val);
void gaim_marshal_BOOLEAN__POINTER_POINTER_POINTER(
		GaimCallback cb, va_list args, void *data, void **return_val);
void gaim_marshal_BOOLEAN__POINTER_POINTER_UINT(
		GaimCallback cb, va_list args, void *data, void **return_val);
void gaim_marshal_BOOLEAN__POINTER_POINTER_POINTER_UINT(
		GaimCallback cb, va_list args, void *data, void **return_val);
void gaim_marshal_BOOLEAN__POINTER_POINTER_POINTER_POINTER(
		GaimCallback cb, va_list args, void *data, void **return_val);
void gaim_marshal_BOOLEAN__POINTER_POINTER_POINTER_POINTER_POINTER(
		GaimCallback cb, va_list args, void *data, void **return_val);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif /* _GAIM_SIGNAL_H_ */
