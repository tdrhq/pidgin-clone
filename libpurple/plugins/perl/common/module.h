

typedef struct group *Purple__Group;

#define group perl_group

#include <glib.h>
#ifdef _WIN32
#undef pipe
#endif
#include <EXTERN.h>
#include <perl.h>
#include <XSUB.h>

#undef group

#include "../perl-common.h"

#include "account.h"
#include "accountopt.h"
#include "blist.h"
#include "buddyicon.h"
#include "cipher.h"
#include "cmds.h"
#include "connection.h"
#include "conversation.h"
#include "core.h"
#include "debug.h"
#include "desktopitem.h"
#include "eventloop.h"
#include "ft.h"
#ifdef PURPLE_GTKPERL
#include "gtkaccount.h"
#include "gtkblist.h"
#include "gtkconn.h"
#include "gtkconv.h"
#include "gtkutils.h"
#endif
#include "imgstore.h"
#include "network.h"
#include "notify.h"
#include "plugin.h"
#include "pluginpref.h"
#include "pounce.h"
#include "prefs.h"
#include "privacy.h"
#include "prpl.h"
#include "proxy.h"
#include "request.h"
#include "roomlist.h"
#include "savedstatuses.h"
#include "server.h"
#include "signals.h"
#include "sound.h"
#include "sslconn.h"
#include "status.h"
#include "stringref.h"
/* Ewww. perl has it's own util.h which is in the include path :( */
#include "libpurple/util.h"
#include "value.h"
#include "xmlnode.h"

/* account.h */
typedef PurpleAccount *			Purple__Account;
typedef PurpleAccountOption *		Purple__Account__Option;
typedef PurpleAccountUiOps *		Purple__Account__UiOps;
typedef PurpleAccountUserSplit *		Purple__Account__UserSplit;

/* blist.h */
typedef PurpleBlistNode *			Purple__BuddyList__Node;
typedef PurpleBlistNodeFlags		Purple__BuddyList__NodeFlags;
typedef PurpleBlistUiOps *		Purple__BuddyList__UiOps;
typedef PurpleBuddyList *			Purple__BuddyList;
typedef PurpleBuddy *			Purple__BuddyList__Buddy;
typedef PurpleChat *			Purple__BuddyList__Chat;
typedef PurpleContact *			Purple__BuddyList__Contact;
typedef PurpleGroup *			Purple__BuddyList__Group;

/* buddyicon.h */
typedef PurpleBuddyIcon *			Purple__Buddy__Icon;

/* cipher.h */
typedef PurpleCipher *			Purple__Cipher;
typedef PurpleCipherCaps			Purple__CipherCaps;
typedef PurpleCipherContext *		Purple__Cipher__Context;
typedef PurpleCipherOps *			Purple__Cipher__Ops;

/* cmds.h */
typedef PurpleCmdFlag			Purple__Cmd__Flag;
typedef PurpleCmdId			Purple__Cmd__Id;
typedef PurpleCmdPriority			Purple__Cmd__Priority;
typedef PurpleCmdRet			Purple__Cmd__Ret;

/* connection.h */
typedef PurpleConnection *		Purple__Connection;
typedef PurpleConnectionFlags		Purple__ConnectionFlags;
typedef PurpleConnectionState		Purple__ConnectionState;
typedef PurpleConnectionUiOps *		Purple__Connection__UiOps;

/* conversation.h */
typedef PurpleConversationType		Purple__ConversationType;
typedef PurpleConvUpdateType		Purple__ConvUpdateType;
typedef PurpleTypingState			Purple__TypingState;
typedef PurpleMessageFlags		Purple__MessageFlags;
typedef PurpleConvChatBuddyFlags		Purple__ConvChatBuddyFlags;
typedef PurpleConversation *		Purple__Conversation;
typedef PurpleConversationUiOps *		Purple__Conversation__UiOps;
typedef PurpleConvIm *			Purple__Conversation__IM;
typedef PurpleConvChat *			Purple__Conversation__Chat;
typedef PurpleConvChatBuddy *		Purple__Conversation__ChatBuddy;

/* core.h */

typedef PurpleCore *			Purple__Core;
typedef PurpleCoreUiOps *		Purple__Core__UiOps;

/* debug.h */
typedef PurpleDebugLevel			Purple__DebugLevel;

/* desktopitem.h */
typedef PurpleDesktopItem *		Purple__DesktopItem;
typedef PurpleDesktopItemType		Purple__DesktopItemType;

/* eventloop.h */
typedef PurpleInputCondition *		Purple__InputCondition;
typedef PurpleEventLoopUiOps *		Purple__EventLoopUiOps;

/* ft.h */
typedef PurpleXfer *			Purple__Xfer;
typedef PurpleXferType			Purple__XferType;
typedef PurpleXferStatusType		Purple__XferStatusType;
typedef PurpleXferUiOps *			Purple__XferUiOps;

#ifdef PURPLE_GTKPERL
/* gtkblish.h */
typedef PurpleGtkBuddyList *		Purple__GTK__BuddyList;
typedef PurpleStatusIconSize		Purple__StatusIconSize;

/* gtkutils.h */
typedef PurpleButtonOrientation		Purple__ButtonOrientation;
typedef PurpleButtonStyle			Purple__ButtonStyle;
#ifndef _WIN32
typedef PurpleBrowserPlace		Purple__BrowserPlace;
#endif /* _WIN32 */

/* gtkconv.h */
typedef PurpleUnseenState			Purple__UnseenState;
typedef PurpleGtkConversation *		Purple__GTK__Conversation;
typedef GdkPixbuf *			Purple__GDK__Pixbuf;
typedef GtkWidget *			Purple__GTK__Widget;

/* gtkutils.h */
typedef GtkFileSelection *		Purple__GTK__FileSelection;
typedef GtkSelectionData *		Purple__GTK__SelectionData;
typedef GtkTextView *			Purple__GTK__TextView;

/* gtkconn.h */
#endif

/* imgstore.h */
typedef PurpleStoredImage *		Purple__StoredImage;

/* log.h */
typedef PurpleLog *			Purple__Log;
typedef PurpleLogCommonLoggerData *	Purple__LogCommonLoggerData;
typedef PurpleLogLogger *			Purple__Log__Logger;
typedef PurpleLogReadFlags *		Purple__Log__ReadFlags;
typedef PurpleLogSet *			Purple__LogSet;
typedef PurpleLogType			Purple__LogType;

/* network.h */
typedef PurpleNetworkListenData *		Purple__NetworkListenData;
typedef PurpleNetworkListenCallback	Purple__NetworkListenCallback;

/* notify.h */
typedef PurpleNotifyCloseCallback		Purple__NotifyCloseCallback;
typedef PurpleNotifyMsgType		Purple__NotifyMsgType;
typedef PurpleNotifySearchButtonType	Purple__NotifySearchButtonType;
typedef PurpleNotifySearchResults *	Purple__NotifySearchResults;
typedef PurpleNotifySearchColumn *	Purple__NotifySearchColumn;
typedef PurpleNotifySearchButton *	Purple__NotifySearchButton;
typedef PurpleNotifyType			Purple__NotifyType;
typedef PurpleNotifyUiOps *		Purple__NotifyUiOps;
typedef PurpleNotifyUserInfo *	Purple__NotifyUserInfo;
typedef PurpleNotifyUserInfoEntry *	Purple__NotifyUserInfoEntry;

/* plugin.h */
typedef PurplePlugin *			Purple__Plugin;
typedef PurplePluginAction *		Purple__Plugin__Action;
typedef PurplePluginInfo *		Purple__PluginInfo;
typedef PurplePluginLoaderInfo *		Purple__PluginLoaderInfo;
typedef PurplePluginType			Purple__PluginType;
typedef PurplePluginUiInfo *		Purple__PluginUiInfo;

/* pluginpref.h */
typedef PurplePluginPref *		Purple__PluginPref;
typedef PurplePluginPrefFrame *		Purple__PluginPref__Frame;
typedef PurplePluginPrefType		Purple__PluginPrefType;

/* pounce.h */
typedef PurplePounce *			Purple__Pounce;
typedef PurplePounceEvent			Purple__PounceEvent;

/* prefs.h */
typedef PurplePrefType			Purple__PrefType;

/* privacy.h */
typedef PurplePrivacyType			Purple__PrivacyType;
typedef PurplePrivacyUiOps *		Purple__Privacy__UiOps;

/* proxy.h */
typedef PurpleProxyInfo *			Purple__ProxyInfo;
typedef PurpleProxyType			Purple__ProxyType;

/* prpl.h */
typedef PurpleBuddyIconSpec *		Purple__Buddy__Icon__Spec;
typedef PurpleIconScaleRules		Purple__IconScaleRules;
typedef PurplePluginProtocolInfo *	Purple__PluginProtocolInfo;
typedef PurpleProtocolOptions		Purple__ProtocolOptions;

/* request.h */
typedef PurpleRequestField *		Purple__Request__Field;
typedef PurpleRequestFields *		Purple__Request__Fields;
typedef PurpleRequestFieldGroup *		Purple__Request__Field__Group;
typedef PurpleRequestFieldType		Purple__RequestFieldType;
typedef PurpleRequestType			Purple__RequestType;
typedef PurpleRequestUiOps *		Purple__Request__UiOps;

/* roomlist.h */
typedef PurpleRoomlist *			Purple__Roomlist;
typedef PurpleRoomlistField *		Purple__Roomlist__Field;
typedef PurpleRoomlistFieldType		Purple__RoomlistFieldType;
typedef PurpleRoomlistRoom *		Purple__Roomlist__Room;
typedef PurpleRoomlistRoomType		Purple__RoomlistRoomType;
typedef PurpleRoomlistUiOps *		Purple__Roomlist__UiOps;

/* savedstatuses.h */
typedef PurpleSavedStatus *		Purple__SavedStatus;
typedef PurpleSavedStatusSub *		Purple__SavedStatusSub;

/* sound.h */
typedef PurpleSoundEventID		Purple__SoundEventID;
typedef PurpleSoundUiOps *		Purple__Sound__UiOps;

/* sslconn.h */
typedef PurpleInputCondition *		Purple__Input__Condition;
typedef PurpleSslConnection *		Purple__Ssl__Connection;
typedef PurpleSslErrorType		Purple__SslErrorType;
typedef PurpleSslOps *			Purple__Ssl__Ops;

/* status.h */
typedef PurplePresence *			Purple__Presence;
typedef PurplePresenceContext		Purple__PresenceContext;
typedef PurpleStatus *			Purple__Status;
typedef PurpleStatusAttr *		Purple__StatusAttr;
typedef PurpleStatusPrimitive		Purple__StatusPrimitive;
typedef PurpleStatusType *		Purple__StatusType;

/* stringref.h */
typedef PurpleStringref *			Purple__Stringref;

/* util.h */
typedef PurpleInfoFieldFormatCallback	Purple__Util__InfoFieldFormatCallback;
typedef PurpleUtilFetchUrlData	Purple__Util__FetchUrlData;
typedef PurpleMenuAction *		Purple__Menu__Action;

/* value.h */
typedef PurpleValue *			Purple__Value;

/* xmlnode.h */
typedef xmlnode *			Purple__XMLNode;
typedef XMLNodeType			XMLNode__Type;

/* other */
typedef void *				Purple__Handle;

typedef gchar gchar_own;
