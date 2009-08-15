/**
 * @file privacy.h Privacy API
 * @ingroup core
 */

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
#ifndef _PURPLE_PRIVACY_H_
#define _PURPLE_PRIVACY_H_

#define PURPLE_PRIVACY_GROUP	"_Privacy"

#include "account.h"

/**
 * Privacy data types.
 */
typedef enum _PurplePrivacySetting
{
	PURPLE_PRIVACY_ALLOW_ALL = 1,
	PURPLE_PRIVACY_BLOCK_MSG_NONBUDDY,
	PURPLE_PRIVACY_ALLOW_BUDDYLIST,
	PURPLE_PRIVACY_CUSTOM
} PurplePrivacySetting;

typedef enum _NativePrivacySetting
{
	PURPLE_PRIVACY_NATIVE_ALLOW_ALL = 0x0004,
	PURPLE_PRIVACY_NATIVE_DENY_ALL = 0x0008,	/* Both messages and presence blocked, add extra states if for a protocol only messages blocked */
	PURPLE_PRIVACY_NATIVE_ALLOW_USERS = 0x0010,
	PURPLE_PRIVACY_NATIVE_DENY_USERS = 0x0020,	/* Both messages and presence blocked, add extra states if for a protocol only messages blocked */
	PURPLE_PRIVACY_NATIVE_ALLOW_BUDDYLIST = 0x0040
} NativePrivacySetting;

typedef enum _PurplePrivacyListType
{
	PURPLE_PRIVACY_ALLOW_LIST = 0x0004,
	PURPLE_PRIVACY_BLOCK_MESSAGE_LIST = 0x0008,
	PURPLE_PRIVACY_BLOCK_BOTH_LIST = 0x0010,
	PURPLE_PRIVACY_VISIBLE_LIST = 0x0020,
	PURPLE_PRIVACY_INVISIBLE_LIST = 0x0040,
	PURPLE_PRIVACY_BUDDY_LIST = 0x0080
} PurplePrivacyListType;

typedef struct _PurpleAccountPrivacySpec
{
	NativePrivacySetting supported_privacy_states;	/* Stores the privacy states natively supported */
	PurplePrivacyListType supported_privacy_lists;	/* Stores the privacy lists types natively supported */
	gboolean visible_list_transient;	/* True for Yahoo visible lists, as these are lost across sessions/status changes */
	gboolean invisible_to_all_supported;	/* whether invisible to all status is supported */
	gboolean can_deny_buddy;		/* can a buddy be added to the deny list - FALSE for yahoo! */
} PurplePrivacySpec;

typedef enum _PurplePrivacyContext
{
	PURPLE_PRIVACY_BLOCK_ALL = 0x0004,
	PURPLE_PRIVACY_BLOCK_MESSAGE = 0x0008,
	PURPLE_PRIVACY_BLOCK_PRESENCE = 0x0010,
	PURPLE_PRIVACY_BLOCK_FT = 0x0020,
	PURPLE_PRIVACY_BLOCK_CONF = 0x0040
} PurplePrivacyContext;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Privacy core/UI operations.
 */
typedef struct
{
	void (*permit_added)(PurpleAccount *account, const char *name);
	void (*permit_removed)(PurpleAccount *account, const char *name);
	void (*deny_added)(PurpleAccount *account, const char *name);
	void (*deny_removed)(PurpleAccount *account, const char *name);

	void (*_purple_reserved1)(void);
	void (*_purple_reserved2)(void);
	void (*_purple_reserved3)(void);
	void (*_purple_reserved4)(void);
} PurplePrivacyUiOps;


/**
 * Sets the UI operations structure for the privacy subsystem.
 *
 * @param ops The UI operations structure.
 */
void purple_privacy_set_ui_ops(PurplePrivacyUiOps *ops);

/**
 * Returns the UI operations structure for the privacy subsystem.
 *
 * @return The UI operations structure.
 */
PurplePrivacyUiOps *purple_privacy_get_ui_ops(void);

/**
 * Initializes the privacy subsystem.
 */
void purple_privacy_init(void);

/* Returns account specific privacy lists */
GSList *purple_privacy_list_get_members_by_account(PurpleAccount *account, PurplePrivacyListType type);

/* called by prpls with all the privacy lists + buddy list. Synchronizes the local master list (blist) */
gboolean purple_privacy_sync_lists(PurpleAccount *account, GSList *buddy_l, GSList *allow_l, GSList *block_msg_l, GSList *block_both_l, GSList *visible_l, GSList *invisible_l);

/* returns TRUE if account supports "Invisible" status, otherwise FALSE */ 
gboolean purple_privacy_account_supports_invisible_status (PurpleAccount *account);

/* returns TRUE if account's current status is "Invisible", otherwise FALSE */ 
gboolean purple_privacy_account_status_invisible(PurpleAccount *account);

/* Sets account's status to "Invisible", returns TRUE if successful, "FALSE" if not */ 
gboolean purple_privacy_set_account_status_invisible(PurpleAccount *account);

/* Sets account's status visible , returns TRUE if successful, "FALSE" if not */
/* Following order is used to test for a suitable visible status:
	* Current active saved status
	* Default saved status
	* Available status
*/
gboolean purple_privacy_set_account_status_visible(PurpleAccount *account);

/* Returns the gloabl privacy state */
PurplePrivacySetting purple_privacy_obtain_global_state(void);
PurplePrivacySetting purple_privacy_obtain_account_state(PurpleAccount *account);
NativePrivacySetting purple_privacy_obtain_native_state(PurpleAccount *account);
void purple_privacy_set_account_state(PurpleAccount *account, PurplePrivacySetting state);
void purple_privacy_set_global_state(PurplePrivacySetting state);
void *purple_privacy_get_handle(void);
void purple_privacy_uninit(void);

gboolean purple_privacy_set_blocking_context(PurpleAccount *account, const char *who, gboolean local_only, PurplePrivacyContext context);
PurplePrivacyContext purple_privacy_get_blocking_context(PurpleAccount *account, const char *who);
gboolean purple_privacy_set_block_all(PurpleAccount *account, const char *who, gboolean local, gboolean server);
gboolean purple_privacy_set_block_presence(PurpleAccount *account, const char *who, gboolean local, gboolean server);
gboolean purple_privacy_set_block_message(PurpleAccount *account, const char *who);
gboolean purple_privacy_set_block_ft(PurpleAccount *account, const char *who);
gboolean purple_privacy_set_block_conf(PurpleAccount *account, const char *who);
gboolean purple_privacy_unset_block_message(PurpleAccount *account, const char *who);
gboolean purple_privacy_unset_block_conf(PurpleAccount *account, const char *who);
gboolean purple_privacy_unset_block_ft(PurpleAccount *account, const char *who);
gboolean purple_privacy_unset_block_all(PurpleAccount *account, const char *who, gboolean local, gboolean server);
gboolean purple_privacy_unset_block_presence(PurpleAccount *account, const char *who, gboolean local, gboolean server);
gboolean purple_privacy_check(PurpleAccount *account, const char *who, PurplePrivacyContext context);
gboolean purple_privacy_check_list_support(PurpleAccount *account, PurplePrivacyListType type);

#ifdef __cplusplus
}
#endif

#endif /* _PURPLE_PRIVACY_H_ */
