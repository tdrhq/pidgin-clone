/**
 * @file roomlist.h Room List API
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

#ifndef _GAIM_ROOMLIST_H_
#define _GAIM_ROOMLIST_H_

/**************************************************************************/
/** Data Structures                                                       */
/**************************************************************************/

typedef struct _GaimRoomlist GaimRoomlist;
typedef struct _GaimRoomlistRoom GaimRoomlistRoom;
typedef enum _GaimRoomlistRoomType GaimRoomlistRoomType;
typedef struct _GaimRoomlistField GaimRoomlistField;
typedef enum _GaimRoomlistFieldType GaimRoomlistFieldType;
typedef struct _GaimRoomlistUiOps GaimRoomlistUiOps;

/**
 * Represents a list of rooms for a given connection on a given protocol.
 */
struct _GaimRoomlist {
	GaimAccount *account; /**< The account this list belongs to. */
	GList *fields; /**< The fields. */
	GList *rooms; /**< The list of rooms. */
	gpointer ui_data; /**< UI private data. */
	gpointer proto_data; /** Prpl private data. */
	guint ref; /**< The reference count. */
};

/**
 * The types of rooms.
 *
 * These are ORable flags.
 */
enum _GaimRoomlistRoomType {
	GAIM_ROOMLIST_ROOMTYPE_CATAGORY = 0x01, /**< It's a catagory, but not a room you can join. */
	GAIM_ROOMLIST_ROOMTYPE_ROOM = 0x02,     /**< It's a room, like the kind you can join. */
};

/**
 * Represents a room.
 */
struct _GaimRoomlistRoom {
	GaimRoomlistRoomType type; /**< The type of room. */
	gchar *name; /**< The name of the room. */
	GList *fields; /**< Other fields. */
	GaimRoomlistRoom *parent; /**< The parent room, or NULL. */
	gboolean expanded_once; /**< A flag the UI uses to avoid multiple expand prpl cbs. */
};

/**
 * The types of fields.
 */
enum _GaimRoomlistFieldType {
	GAIM_ROOMLIST_FIELD_BOOL,
	GAIM_ROOMLIST_FIELD_INT,
	GAIM_ROOMLIST_FIELD_STRING, /**< We do a g_strdup on the passed value if it's this type. */
};

/**
 * A field a room might have.
 */
struct _GaimRoomlistField {
	GaimRoomlistFieldType type; /**< The type of field. */
	gchar *label; /**< The i18n user displayed name of the field. */
	gchar *name; /**< The internal name of the field. */
	gboolean hidden; /**< Hidden? */
};

/**
 * The room list ops to be filled out by the UI.
 */
struct _GaimRoomlistUiOps {
	void (*new)(GaimRoomlist *list); /**< A new list was created. */
	void (*set_fields)(GaimRoomlist *list, GList *fields); /**< Sets the columns. */
	void (*add_room)(GaimRoomlist *list, GaimRoomlistRoom *room); /**< Add a room to the list. */
	void (*in_progress)(GaimRoomlist *list, gboolean flag); /**< Are we fetching stuff still? */
	void (*destroy)(GaimRoomlist *list); /**< We're destroying list. */
};


#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/
/** @name Room List API                                                   */
/**************************************************************************/
/*@{*/

/**
 * Returns a newly created room list object.
 *
 * It has an inital reference count of 1.
 *
 * @param account The account that's listing rooms.
 * @return The new room list handle.
 */
GaimRoomlist *gaim_roomlist_new(GaimAccount *account);

/**
 * Increases the reference count on the room list.
 *
 * @param list The object to ref.
 */
void gaim_roomlist_ref(GaimRoomlist *list);

/**
 * Decreases the reference count on the room list.
 *
 * The room list will be destroyed when this reaches 0.
 *
 * @param list The room list object to unref and possibly
 *             destroy.
 */
void gaim_roomlist_unref(GaimRoomlist *list);

/**
 * Set the different field types and their names for this protocol.
 *
 * This must be called before gaim_roomlist_room_add().
 *
 * @param list The room list.
 * @param fields A GList of GaimRoomlistField's. UI's are encourged
 *               to default to displaying them in the order given.
 */
void gaim_roomlist_set_fields(GaimRoomlist *list, GList *fields);

/**
 * Set the "in progress" state of the room list.
 *
 * The UI is encourged to somehow hint to the user
 * whether or not we're busy downloading a room list or not.
 *
 * @param list The room list.
 * @param in_progress We're downloading it, or we're not.
 */
void gaim_roomlist_set_in_progress(GaimRoomlist *list, gboolean in_progress);

/**
 * Adds a room to the list of them.
 *
 * @param list The room list.
 * @param room The room to add to the list. The GList of fields must be in the same
               order as was given in gaim_roomlist_set_fields().
*/
void gaim_roomlist_room_add(GaimRoomlist *list, GaimRoomlistRoom *room);

/**
 * Do we support room listing?
 *
 * @param gc The GaimConnection we're asking.
 * @return @c TRUE if it's possible to get a room list.
 */
gboolean gaim_roomlist_is_possible(GaimConnection *gc);

/**
 * Returns a GaimRoomlist structure from the prpl, and
 * instructs the prpl to start fetching the list.
 *
 * @param gc The GaimConnection to have get a list.
 *
 * @return A GaimRoomlist* or @c NULL if the protocol
 *         doesn't support that.
 */
GaimRoomlist *gaim_roomlist_get_list(GaimConnection *gc);

/**
 * Tells the prpl to stop fetching the list.
 * If this is possible and done, the prpl will
 * call set_in_progress with @c FALSE and possibly
 * unref the list if it took a reference.
 *
 * @param list The room list to cancel a get_list on.
 */
void gaim_roomlist_cancel_get_list(GaimRoomlist *list);

/**
 * Tells the prpl that a catagory was expanded.
 *
 * On some protocols, the rooms in the catagory
 * won't be fetched until this is called.
 *
 * @param list The room list.
 * @param room The catagory that was expanded. The expression
 *             (catagory->type & GAIM_ROOMLIST_ROOMTYPE_CATAGORY)
 *             must be true.
 */
void gaim_roomlist_expand_catagory(GaimRoomlist *list, GaimRoomlistRoom *catagory);

/*@}*/

/**************************************************************************/
/** @name Room API                                                        */
/**************************************************************************/
/*@{*/

/**
 * Creates a new room, to be added to the list.
 *
 * @param type The type of room.
 * @param name The name of the room.
 * @param parent The room's parent, if any.
 *
 * @return A new room.
 */
GaimRoomlistRoom *gaim_roomlist_room_new(GaimRoomlistRoomType type, const gchar *name,
                                         GaimRoomlistRoom *parent);

/**
 * Adds a field to a room.
 *
 * @param list The room list the room belongs to.
 * @param room The room.
 * @param field The field to append. Strings get g_strdup'd internally.
 */
void gaim_roomlist_room_add_field(GaimRoomlist *list, GaimRoomlistRoom *room, gconstpointer field);

/*@}*/

/**************************************************************************/
/** @name Room Field API                                                  */
/**************************************************************************/
/*@{*/

/**
 * Creates a new field.
 *
 * @param type The type of the field.
 * @param label The i18n'ed, user displayable name.
 * @param name The internal name of the field.
 *
 * @return A new GaimRoomlistField, ready to be added to a GList and passed to
 *         gaim_roomlist_set_fields().
 */
GaimRoomlistField *gaim_roomlist_field_new(GaimRoomlistFieldType type,
                                           const gchar *label, const gchar *name,
                                           gboolean hidden);
/*@}*/

/**************************************************************************/
/** @name UI Registration Functions                                       */
/**************************************************************************/
/*@{*/

/**
 * Sets the UI operations structure to be used in all gaim room lists.
 *
 * @param ops The UI operations structure.
 */
void gaim_roomlist_set_ui_ops(GaimRoomlistUiOps *ops);

/**
 * Returns the gaim window UI operations structure to be used in
 * new windows.
 *
 * @return A filled-out GaimRoomlistUiOps structure.
 */
GaimRoomlistUiOps *gaim_roomlist_get_ui_ops(void);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif /* _GAIM_ROOMLIST_H_ */
