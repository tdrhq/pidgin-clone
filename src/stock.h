/**
 * @file stock.h GTK+ Stock resources
 * @ingroup gtkui
 *
 * gaim
 *
 * Copyright (C) 2002-2003, Christian Hammond <chipx86@gnupdate.org>
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
#ifndef _STOCK_H_
#define _STOCK_H_

/**************************************************************************/
/** @name Stock images                                                    */
/**************************************************************************/
/*@{*/
#define GAIM_STOCK_ABOUT           "gaim-about"
#define GAIM_STOCK_ACCOUNTS        "gaim-accounts"
#define GAIM_STOCK_AWAY            "gaim-away"
#define GAIM_STOCK_BGCOLOR         "gaim-bgcolor"
#define GAIM_STOCK_BLOCK           "gaim-block"
#define GAIM_STOCK_CHAT            "gaim-chat"
#define GAIM_STOCK_DIALOG_AUTH     "gaim-dialog-auth"
#define GAIM_STOCK_DIALOG_COOL     "gaim-dialog-cool"
#define GAIM_STOCK_DIALOG_ERROR    "gaim-dialog-error"
#define GAIM_STOCK_DIALOG_INFO     "gaim-dialog-info"
#define GAIM_STOCK_DIALOG_QUESTION "gaim-dialog-question"
#define GAIM_STOCK_DIALOG_WARNING  "gaim-dialog-warning"
#define GAIM_STOCK_DOWNLOAD        "gaim-download"
#define GAIM_STOCK_EDIT            "gaim-edit"
#define GAIM_STOCK_FGCOLOR         "gaim-fgcolor"
#define GAIM_STOCK_FILE_CANCELED   "gaim-file-canceled"
#define GAIM_STOCK_FILE_DONE       "gaim-file-done"
#define GAIM_STOCK_FILE_TRANSFER   "gaim-file-transfer"
#define GAIM_STOCK_ICON_AWAY       "gaim-icon-away"
#define GAIM_STOCK_ICON_AWAY_MSG   "gaim-icon-away-msg"
#define GAIM_STOCK_ICON_CONNECT    "gaim-icon-away-connect"
#define GAIM_STOCK_ICON_OFFLINE    "gaim-icon-offline"
#define GAIM_STOCK_ICON_ONLINE     "gaim-icon-online"
#define GAIM_STOCK_ICON_ONLINE_MSG "gaim-icon-online-msg"
#define GAIM_STOCK_IGNORE          "gaim-ignore"
#define GAIM_STOCK_IM              "gaim-im"
#define GAIM_STOCK_IMAGE           "gaim-image"
#define GAIM_STOCK_INFO            "gaim-info"
#define GAIM_STOCK_INVITE          "gaim-invite"
#define GAIM_STOCK_LINK            "gaim-link"
#define GAIM_STOCK_LOGO            "gaim-logo"
#define GAIM_STOCK_MODIFY          "gaim-modify"
#define GAIM_STOCK_PRIVACY         "gaim-privacy"
#define GAIM_STOCK_SEND            "gaim-send"
#define GAIM_STOCK_SIGN_OFF        "gaim-sign-off"
#define GAIM_STOCK_SIGN_ON         "gaim-sign-on"
#define GAIM_STOCK_SMILEY          "gaim-smiley"
#define GAIM_STOCK_OPEN_MAIL       "gaim-stock-open-mail"
#define GAIM_STOCK_TEXT_BIGGER     "gaim-text-bigger"
#define GAIM_STOCK_TEXT_NORMAL     "gaim-text-normal"
#define GAIM_STOCK_TEXT_SMALLER    "gaim-text-smaller"
#define GAIM_STOCK_TYPED           "gaim-typed"
#define GAIM_STOCK_TYPING          "gaim-typing"
#define GAIM_STOCK_UPLOAD          "gaim-upload"
#define GAIM_STOCK_WARN            "gaim-warn"
/*@}*/

/**
 * For getting the icon size for the logo
 */
#define GAIM_ICON_SIZE_LOGO        "gaim-icon-size-logo"

/**
 * Sets up the gaim stock repository.
 */
void setup_stock(void);

#endif /* _STOCK_H_ */
