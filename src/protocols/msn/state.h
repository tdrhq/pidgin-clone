/**
 * @file state.h State functinos and definitions
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
#ifndef _MSN_STATE_H_
#define _MSN_STATE_H_

/**
 * Away types.
 */
typedef enum
{
	MSN_ONLINE  = 1,
	MSN_BUSY    = 2,
	MSN_IDLE    = 3,
	MSN_BRB     = 4,
	MSN_AWAY    = 5,
	MSN_PHONE   = 6,
	MSN_LUNCH   = 7,
	MSN_OFFLINE = 8,
	MSN_HIDDEN  = 9

} MsnAwayType;

#define MSN_AWAY_TYPE(x)   (((x) >> 1) & 0x0F)

/**
 * Returns the string representation of an away type.
 *
 * @param type The away type.
 *
 * @return The string representation of the away type.
 */
const char *msn_away_get_text(MsnAwayType type);

#endif /* _MSN_STATE_H_ */
