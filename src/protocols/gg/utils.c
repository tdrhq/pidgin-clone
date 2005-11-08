/**
 * @file utils.c
 *
 * gaim
 *
 * Copyright (C) 2005  Bartosz Oler <bartosz@bzimage.us>
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


#include "utils.h"


/* static uin_t ggp_str_to_uin(const char *text) {{{ */
uin_t ggp_str_to_uin(const char *text)
{
	char *tmp;
	long num;

	if (!text)
		return 0;

	errno = 0;
	num = strtol(text, &tmp, 0);

	if (*text == '\0' || *tmp != '\0')
		return 0;

	if ((errno == ERANGE || (num == LONG_MAX || num == LONG_MIN))
	    || num > UINT_MAX || num < 0)
		return 0;

	return (uin_t) num;
}
/* }}} */

/* char *charset_convert(const gchar *locstr, const char *encsrc, const char *encdst) {{{ */
char *charset_convert(const gchar *locstr, const char *encsrc, const char *encdst)
{
	gchar *msg;
	GError *err = NULL;

	if (locstr == NULL)
		return NULL;

	msg = g_convert_with_fallback(locstr, strlen(locstr), encdst, encsrc,
				      "?", NULL, NULL, &err);
	if (err != NULL) {
		gaim_debug_error("gg", "Error converting from %s to %s: %s\n",
				 encsrc, encdst, err->message);
		g_error_free(err);
	}

	/* Just in case? */
	if (msg == NULL)
		msg = g_strdup(locstr);

	return msg;
}
/* }}} */

/* ggp_get_uin(GaimAccount *account) {{{ */
uin_t ggp_get_uin(GaimAccount *account)
{
	return ggp_str_to_uin(gaim_account_get_username(account));
}
/* }}} */

/* const *char ggp_buddy_get_name(GaimConnection *gc, const uin_t uin) {{{ */
const char *ggp_buddy_get_name(GaimConnection *gc, const uin_t uin)
{
	GaimBuddy *buddy;
	gchar *str_uin;

	str_uin = g_strdup_printf("%lu", (unsigned long int)uin);

	buddy = gaim_find_buddy(gaim_connection_get_account(gc), str_uin);
	if (buddy != NULL) {
		g_free(str_uin);
		return gaim_buddy_get_alias(buddy);
	} else {
		return str_uin;
	}
}
/* }}} */


/* vim: set ts=8 sts=0 sw=8 noet: */
