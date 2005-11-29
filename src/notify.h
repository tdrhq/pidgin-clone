/**
 * @file notify.h Notification API
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
#ifndef _GAIM_NOTIFY_H_
#define _GAIM_NOTIFY_H_

#include <stdlib.h>
#include <glib-object.h>
#include <glib.h>

#include "connection.h"

/**
 * Notification close callbacks.
 */
typedef void  (*GaimNotifyCloseCallback) (gpointer user_data);

/**
 * Notification types.
 */
typedef enum
{
	GAIM_NOTIFY_MESSAGE = 0,   /**< Message notification.         */
	GAIM_NOTIFY_EMAIL,         /**< Single e-mail notification.   */
	GAIM_NOTIFY_EMAILS,        /**< Multiple e-mail notification. */
	GAIM_NOTIFY_FORMATTED,     /**< Formatted text.               */
	GAIM_NOTIFY_SEARCHRESULTS, /**< Buddy search results.         */
	GAIM_NOTIFY_USERINFO,      /**< Formatted userinfo text.      */
	GAIM_NOTIFY_URI            /**< URI notification or display.  */

} GaimNotifyType;

/**
 * Notification message types.
 */
typedef enum
{
	GAIM_NOTIFY_MSG_ERROR   = 0, /**< Error notification.       */
	GAIM_NOTIFY_MSG_WARNING,     /**< Warning notification.     */
	GAIM_NOTIFY_MSG_INFO         /**< Information notification. */

} GaimNotifyMsgType;

/**
 * The types of buttons
 */
typedef enum
{
	GAIM_NOTIFY_BUTTON_CONTINUE = 0,
	GAIM_NOTIFY_BUTTON_ADD_BUDDY

} GaimNotifySearchButtonType;

/**
 * Search results object.
 */
typedef struct
{
	GList *columns;        /**< List of the search column objects. */
	GList *rows;           /**< List of rows in the result. */
	GList *buttons;        /**< List of buttons to display. */

} GaimNotifySearchResults;

/**
 * Single column of a search result.
 */
typedef struct
{
	char *title; /**< Title of the column. */

} GaimNotifySearchColumn;

typedef void (*GaimNotifySearchResultsCallback)(GaimConnection *, GList *);


/**
 * Definition of a button.
 */
typedef struct
{
	GaimNotifySearchButtonType type;
	GaimNotifySearchResultsCallback callback; /**< Function to be called when clicked. */

} GaimNotifySearchButton;

/**
 * Notification UI operations.
 */
typedef struct
{
	void *(*notify_message)(GaimNotifyMsgType type, const char *title,
							const char *primary, const char *secondary);
	void *(*notify_email)(const char *subject, const char *from,
						  const char *to, const char *url);
	void *(*notify_emails)(size_t count, gboolean detailed,
						   const char **subjects, const char **froms,
						   const char **tos, const char **urls);
	void *(*notify_formatted)(const char *title, const char *primary,
							  const char *secondary, const char *text);
	void *(*notify_searchresults)(GaimConnection *gc, const char *title,
								  const char *primary, const char *secondary,
								  GaimNotifySearchResults *results);
	void (*notify_searchresults_new_rows)(GaimConnection *gc,
										  GaimNotifySearchResults *results,
										  void *data, gpointer user_data);
	void *(*notify_userinfo)(GaimConnection *gc, const char *who,
							  const char *text);
	void *(*notify_uri)(const char *uri);

	void (*close_notify)(GaimNotifyType type, void *ui_handle);

} GaimNotifyUiOps;

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/
/** Search results notification API                                       */
/**************************************************************************/
/*@{*/

/**
 * Displays results from a buddy search.  This can be, for example,
 * a window with a list of all found buddies, where you are given the
 * option of adding buddies to your buddy list.
 *
 * @param gc        The GaimConnection handle associated with the information.
 * @param title     The title of the message.  If this is NULL, the title
 *                  will be "Search Results."
 * @param primary   The main point of the message.
 * @param secondary The secondary information.
 * @param results   The GaimNotifySearchResults instance.
 * @param cb        The callback to call when the user closes
 *                  the notification.
 * @param user_data The data to pass to the callback.
 *
 * @return A UI-specific handle.
 */
void *gaim_notify_searchresults(GaimConnection *gc, const char *title,
								const char *primary, const char *secondary,
								GaimNotifySearchResults *results, GaimNotifyCloseCallback cb,
								gpointer user_data);

void gaim_notify_searchresults_free(GaimNotifySearchResults *results);

/**
 * Replace old rows with the new. Reuse an existing window.
 *
 * @param gc        The GaimConnection structure.
 * @param results   The GaimNotifySearchResults structure.
 * @param data      Data returned by the gaim_notify_searchresults().
 * @param user_data User defined data.
 */
void gaim_notify_searchresults_new_rows(GaimConnection *gc,
										GaimNotifySearchResults *results,
										void *data, gpointer user_data);

/**
 * Adds a button that will be displayed in the search results dialog.
 *
 * @param results The search results object.
 * @param type    Type of the button. (TODO: Only one button of a given type can be displayed.)
 * @param cb      Function that will be called on the click event.
 */
void gaim_notify_searchresults_button_add(GaimNotifySearchResults *results,
										  GaimNotifySearchButtonType type,
										  GaimNotifySearchResultsCallback cb);

/**
 * Returns a newly created search results object.
 *
 * @return The new search results object.
 */
GaimNotifySearchResults *gaim_notify_searchresults_new();

/**
 * Returns a newly created search result column object.
 *
 * @param title Title of the column. NOTE: Title will get g_strdup()ed.
 * 
 * @return The new search column object.
 */
GaimNotifySearchColumn *gaim_notify_searchresults_column_new(const char *title);

/**
 * Adds a new column to the search result object.
 *
 * @param results The result object to which the column will be added.
 * @param column The column that will be added to the result object.
 */
void gaim_notify_searchresults_column_add(GaimNotifySearchResults *results,
										  GaimNotifySearchColumn *column);

/**
 * Adds a new row of the results to the search results object.
 *
 * @param results The search results object.
 * @param row     The row of the results.
 */
void gaim_notify_searchresults_row_add(GaimNotifySearchResults *results,
									   GList *row);

/**
 * Returns a number of the rows in the search results object.
 * 
 * @param results The search results object.
 *
 * @return Number of the result rows.
 */
guint gaim_notify_searchresults_get_rows_count(GaimNotifySearchResults *results);

/**
 * Returns a number of the columns in the search results object.
 *
 * @param results The search results object.
 *
 * @return Number of the columns.
 */
guint gaim_notify_searchresults_get_columns_count(GaimNotifySearchResults *results);

/**
 * Returns a row of the results from the search results object.
 *
 * @param results The search results object.
 * @param row_id  Index of the row to be returned.
 *
 * @return Row of the results.
 */
GList *gaim_notify_searchresults_row_get(GaimNotifySearchResults *results,
										 unsigned int row_id);

/**
 * Returns a title of the search results object's column.
 * 
 * @param results   The search results object.
 * @param column_id Index of the column.
 *
 * @return Title of the column.
 */
char *gaim_notify_searchresults_column_get_title(GaimNotifySearchResults *results,
												 unsigned int column_id);

/*@}*/

/**************************************************************************/
/** @name Notification API                                                */
/**************************************************************************/
/*@{*/

/**
 * Displays a notification message to the user.
 *
 * @param handle    The plugin or connection handle.
 * @param type      The notification type.
 * @param title     The title of the message.
 * @param primary   The main point of the message.
 * @param secondary The secondary information.
 * @param cb        The callback to call when the user closes
 *                  the notification.
 * @param user_data The data to pass to the callback.
 *
 * @return A UI-specific handle.
 */
void *gaim_notify_message(void *handle, GaimNotifyMsgType type,
						  const char *title, const char *primary,
						  const char *secondary, GaimNotifyCloseCallback cb,
						  gpointer user_data);

/**
 * Displays a single e-mail notification to the user.
 *
 * @param handle    The plugin or connection handle.
 * @param subject   The subject of the e-mail.
 * @param from      The from address.
 * @param to        The destination address.
 * @param url       The URL where the message can be read.
 * @param cb        The callback to call when the user closes
 *                  the notification.
 * @param user_data The data to pass to the callback.
 *
 * @return A UI-specific handle.
 */
void *gaim_notify_email(void *handle, const char *subject,
						const char *from, const char *to,
						const char *url, GaimNotifyCloseCallback cb,
						gpointer user_data);

/**
 * Displays a notification for multiple e-mails to the user.
 *
 * @param handle    The plugin or connection handle.
 * @param count     The number of e-mails.
 * @param detailed  @c TRUE if there is information for each e-mail in the
 *                  arrays.
 * @param subjects  The array of subjects.
 * @param froms     The array of from addresses.
 * @param tos       The array of destination addresses.
 * @param urls      The URLs where the messages can be read.
 * @param cb        The callback to call when the user closes
 *                  the notification.
 * @param user_data The data to pass to the callback.
 *
 * @return A UI-specific handle.
 */
void *gaim_notify_emails(void *handle, size_t count, gboolean detailed,
						 const char **subjects, const char **froms,
						 const char **tos, const char **urls,
						 GaimNotifyCloseCallback cb, gpointer user_data);

/**
 * Displays a notification with formatted text.
 *
 * The text is essentially a stripped-down format of HTML, the same that
 * IMs may send.
 *
 * @param handle    The plugin or connection handle.
 * @param title     The title of the message.
 * @param primary   The main point of the message.
 * @param secondary The secondary information.
 * @param text      The formatted text.
 * @param cb        The callback to call when the user closes
 *                  the notification.
 * @param user_data The data to pass to the callback.
 *
 * @return A UI-specific handle.
 */
void *gaim_notify_formatted(void *handle, const char *title,
							const char *primary, const char *secondary,
							const char *text, GaimNotifyCloseCallback cb, gpointer user_data);

/**
 * Displays user information with formatted text, passing information giving
 * the connection and username from which the user information came.
 *
 * The text is essentially a stripped-down format of HTML, the same that
 * IMs may send.
 *
 * @param gc	    The GaimConnection handle associated with the information.
 * @param who	    The username associated with the information.
 * @param text      The formatted text.
 * @param cb        The callback to call when the user closes
 *                  the notification.
 * @param user_data The data to pass to the callback.
 *
 * @return A UI-specific handle.
 */
void *gaim_notify_userinfo(GaimConnection *gc, const char *who,
						   const char *text, GaimNotifyCloseCallback cb,
						   gpointer user_data);

/**
 * Opens a URI or somehow presents it to the user.
 *
 * @param handle The plugin or connection handle.
 * @param uri    The URI to display or go to.
 *
 * @return A UI-specific handle, if any. This may only be presented if
 *         the UI code displays a dialog instead of a webpage, or something
 *         similar.
 */
void *gaim_notify_uri(void *handle, const char *uri);

/**
 * Closes a notification.
 *
 * This should be used only by the UI operation functions and part of the
 * core.
 *
 * @param type      The notification type.
 * @param ui_handle The notification UI handle.
 */
void gaim_notify_close(GaimNotifyType type, void *ui_handle);

/**
 * Closes all notifications registered with the specified handle.
 *
 * @param handle The handle.
 */
void gaim_notify_close_with_handle(void *handle);

/**
 * A wrapper for gaim_notify_message that displays an information message.
 */
#define gaim_notify_info(handle, title, primary, secondary) \
	gaim_notify_message((handle), GAIM_NOTIFY_MSG_INFO, (title), \
						(primary), (secondary), NULL, NULL)

/**
 * A wrapper for gaim_notify_message that displays a warning message.
 */
#define gaim_notify_warning(handle, title, primary, secondary) \
	gaim_notify_message((handle), GAIM_NOTIFY_MSG_WARNING, (title), \
						(primary), (secondary), NULL, NULL)

/**
 * A wrapper for gaim_notify_message that displays an error message.
 */
#define gaim_notify_error(handle, title, primary, secondary) \
	gaim_notify_message((handle), GAIM_NOTIFY_MSG_ERROR, (title), \
						(primary), (secondary), NULL, NULL)

/*@}*/

/**************************************************************************/
/** @name UI Registration Functions                                       */
/**************************************************************************/
/*@{*/

/**
 * Sets the UI operations structure to be used when displaying a
 * notification.
 *
 * @param ops The UI operations structure.
 */
void gaim_notify_set_ui_ops(GaimNotifyUiOps *ops);

/**
 * Returns the UI operations structure to be used when displaying a
 * notification.
 *
 * @return The UI operations structure.
 */
GaimNotifyUiOps *gaim_notify_get_ui_ops(void);

/*@}*/

/**************************************************************************/
/** @name Notify Subsystem                                         */
/**************************************************************************/
/*@{*/

/**
 * Returns the notify subsystem handle.
 *
 * @return The notify subsystem handle.
 */
void *gaim_notify_get_handle(void);

/**
 * Initializes the notify subsystem.
 */
void gaim_notify_init(void);

/**
 * Uninitializes the notify subsystem.
 */
void gaim_notify_uninit(void);

/*@}*/


#ifdef __cplusplus
}
#endif

#endif /* _GAIM_NOTIFY_H_ */
