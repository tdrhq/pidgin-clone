/**
 * @file user.c User functions
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
#include "msn.h"
#include "user.h"

MsnUser *
msn_user_new(MsnSession *session, const char *passport, const char *name)
{
	MsnUser *user;

	user = msn_users_find_with_passport(session->users, passport);

	if (user == NULL) {
		user = g_new0(MsnUser, 1);

		user->session = session;

		msn_user_set_passport(user, passport);

		msn_users_add(session->users, user);
	}

	if (name != NULL)
		msn_user_set_name(user, name);

	msn_user_ref(user);

	return user;
}

void
msn_user_destroy(MsnUser *user)
{
	g_return_if_fail(user != NULL);

	if (user->ref_count > 0) {
		msn_user_unref(user);

		return;
	}

	if (user->session != NULL && user->session->users != NULL)
		msn_users_remove(user->session->users, user);

	if (user->clientcaps != NULL)
		g_hash_table_destroy(user->clientcaps);

	if (user->group_ids != NULL)
		g_list_free(user->group_ids);

	if (user->msnobj != NULL)
		msn_object_destroy(user->msnobj);

	if (user->passport != NULL) g_free(user->passport);
	if (user->name     != NULL) g_free(user->name);

	if (user->phone.home   != NULL) g_free(user->phone.home);
	if (user->phone.work   != NULL) g_free(user->phone.work);
	if (user->phone.mobile != NULL) g_free(user->phone.mobile);

	g_free(user);
}

MsnUser *
msn_user_ref(MsnUser *user)
{
	g_return_val_if_fail(user != NULL, NULL);

	user->ref_count++;

	return user;
}

MsnUser *
msn_user_unref(MsnUser *user)
{
	g_return_val_if_fail(user != NULL, NULL);

	if (user->ref_count <= 0)
		return NULL;

	user->ref_count--;

	if (user->ref_count == 0) {
		msn_user_destroy(user);

		return NULL;
	}

	return user;
}

void
msn_user_set_passport(MsnUser *user, const char *passport)
{
	g_return_if_fail(user != NULL);

	if (user->passport != NULL)
		g_free(user->passport);

	user->passport = g_strdup(passport);
}

void
msn_user_set_name(MsnUser *user, const char *name)
{
	g_return_if_fail(user != NULL);

	if (user->name != NULL)
		g_free(user->name);

	user->name = g_strdup(name);
}

void
msn_user_set_group_ids(MsnUser *user, GList *ids)
{
	g_return_if_fail(user != NULL);

	user->group_ids = ids;
}

void
msn_user_add_group_id(MsnUser *user, int id)
{
	g_return_if_fail(user != NULL);
	g_return_if_fail(id > -1);

	if (!g_list_find(user->group_ids, GINT_TO_POINTER(id)))
		user->group_ids = g_list_append(user->group_ids, GINT_TO_POINTER(id));
}

void
msn_user_remove_group_id(MsnUser *user, int id)
{
	g_return_if_fail(user != NULL);
	g_return_if_fail(id > -1);

	user->group_ids = g_list_remove(user->group_ids, GINT_TO_POINTER(id));
}
void
msn_user_set_home_phone(MsnUser *user, const char *number)
{
	g_return_if_fail(user != NULL);

	if (user->phone.home != NULL)
		g_free(user->phone.home);

	user->phone.home = (number == NULL ? NULL : g_strdup(number));
}

void
msn_user_set_work_phone(MsnUser *user, const char *number)
{
	g_return_if_fail(user != NULL);

	if (user->phone.work != NULL)
		g_free(user->phone.work);

	user->phone.work = (number == NULL ? NULL : g_strdup(number));
}

void
msn_user_set_mobile_phone(MsnUser *user, const char *number)
{
	g_return_if_fail(user != NULL);

	if (user->phone.mobile != NULL)
		g_free(user->phone.mobile);

	user->phone.mobile = (number == NULL ? NULL : g_strdup(number));
}

void
msn_user_set_object(MsnUser *user, MsnObject *obj)
{
	g_return_if_fail(user != NULL);

	if (user->msnobj != NULL)
		msn_object_destroy(user->msnobj);

	user->msnobj = obj;
}

void
msn_user_set_client_caps(MsnUser *user, GHashTable *info)
{
	g_return_if_fail(user != NULL);
	g_return_if_fail(info != NULL);

	if (user->clientcaps != NULL)
		g_hash_table_destroy(user->clientcaps);

	user->clientcaps = info;
}

const char *
msn_user_get_passport(const MsnUser *user)
{
	g_return_val_if_fail(user != NULL, NULL);

	return user->passport;
}

const char *
msn_user_get_name(const MsnUser *user)
{
	g_return_val_if_fail(user != NULL, NULL);

	return user->name;
}

GList *
msn_user_get_group_ids(const MsnUser *user)
{
	g_return_val_if_fail(user != NULL, NULL);

	return user->group_ids;
}

const char *
msn_user_get_home_phone(const MsnUser *user)
{
	g_return_val_if_fail(user != NULL, NULL);

	return user->phone.home;
}

const char *
msn_user_get_work_phone(const MsnUser *user)
{
	g_return_val_if_fail(user != NULL, NULL);

	return user->phone.work;
}

const char *
msn_user_get_mobile_phone(const MsnUser *user)
{
	g_return_val_if_fail(user != NULL, NULL);

	return user->phone.mobile;
}

MsnObject *
msn_user_get_object(const MsnUser *user)
{
	g_return_val_if_fail(user != NULL, NULL);

	return user->msnobj;
}

GHashTable *
msn_user_get_client_caps(const MsnUser *user)
{
	g_return_val_if_fail(user != NULL, NULL);

	return user->clientcaps;
}

MsnUsers *
msn_users_new(void)
{
	MsnUsers *users = g_new0(MsnUsers, 1);

	return users;
}

void
msn_users_destroy(MsnUsers *users)
{
	GList *l, *l_next = NULL;

	g_return_if_fail(users != NULL);

	for (l = users->users; l != NULL; l = l_next) {
		l_next = l->next;

		msn_user_destroy(l->data);

		users->users = g_list_remove(users->users, l->data);
	}

	/* See if we've leaked anybody. */
	while (users->users != NULL) {
		gaim_debug(GAIM_DEBUG_WARNING, "msn",
				   "Leaking user %s\n",
				   msn_user_get_passport(users->users->data));
	}

	g_free(users);
}

void
msn_users_add(MsnUsers *users, MsnUser *user)
{
	g_return_if_fail(users != NULL);
	g_return_if_fail(user != NULL);

	users->users = g_list_append(users->users, user);

	users->count++;
}

void
msn_users_remove(MsnUsers *users, MsnUser *user)
{
	g_return_if_fail(users != NULL);
	g_return_if_fail(user  != NULL);

	users->users = g_list_remove(users->users, user);

	users->count--;
}

size_t
msn_users_get_count(const MsnUsers *users)
{
	g_return_val_if_fail(users != NULL, 0);

	return users->count;
}

MsnUser *
msn_users_find_with_passport(MsnUsers *users, const char *passport)
{
	GList *l;

	g_return_val_if_fail(users != NULL, NULL);
	g_return_val_if_fail(passport != NULL, NULL);

	for (l = users->users; l != NULL; l = l->next) {
		MsnUser *user = (MsnUser *)l->data;

		if (user->passport != NULL &&
			!g_ascii_strcasecmp(passport, user->passport)) {

			return user;
		}
	}

	return NULL;
}
