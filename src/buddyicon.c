/**
 * @file icon.c Buddy Icon API
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
#include "internal.h"
#include "buddyicon.h"
#include "conversation.h"
#include "debug.h"
#include "util.h"

static GHashTable *account_cache = NULL;
static char       *cache_dir     = NULL;
static gboolean    icon_caching  = TRUE;

static GaimBuddyIcon *
gaim_buddy_icon_create(GaimAccount *account, const char *username)
{
	GaimBuddyIcon *icon;
	GHashTable *icon_cache;

	icon = g_new0(GaimBuddyIcon, 1);

	gaim_buddy_icon_set_account(icon,  account);
	gaim_buddy_icon_set_username(icon, username);

	icon_cache = g_hash_table_lookup(account_cache, account);

	if (icon_cache == NULL)
	{
		icon_cache = g_hash_table_new(g_str_hash, g_str_equal);

		g_hash_table_insert(account_cache, account, icon_cache);
	}

	g_hash_table_insert(icon_cache,
	                   (char *)gaim_buddy_icon_get_username(icon), icon);
	return icon;
}

GaimBuddyIcon *
gaim_buddy_icon_new(GaimAccount *account, const char *username,
					void *icon_data, size_t icon_len)
{
	GaimBuddyIcon *icon;

	g_return_val_if_fail(account   != NULL, NULL);
	g_return_val_if_fail(username  != NULL, NULL);
	g_return_val_if_fail(icon_data != NULL, NULL);
	g_return_val_if_fail(icon_len  > 0,    NULL);

	icon = gaim_buddy_icons_find(account, username);

	if (icon == NULL)
		icon = gaim_buddy_icon_create(account, username);

	gaim_buddy_icon_ref(icon);
	gaim_buddy_icon_set_data(icon, icon_data, icon_len);
	gaim_buddy_icon_unref(icon);

	/* We don't take a reference here. gaim_buddy_icon_set_data() makes blist.c or
	   conversation.c, or both, do that for us.
	 */
	return icon;
}

void
gaim_buddy_icon_destroy(GaimBuddyIcon *icon)
{
	GaimConversation *conv;
	GaimAccount *account;
	GHashTable *icon_cache;
	const char *username;
	GSList *sl, *list;

	g_return_if_fail(icon != NULL);

	if (icon->ref_count > 0)
	{
		/* If the ref count is greater than 0, then we weren't called from
		 * gaim_buddy_icon_unref(). So we go through and ask everyone to
		 * unref us. Then we return, since we know somewhere along the
		 * line we got called recursively by one of the unrefs, and the
		 * icon is already destroyed.
		 */
		account  = gaim_buddy_icon_get_account(icon);
		username = gaim_buddy_icon_get_username(icon);

		conv = gaim_find_conversation_with_account(username, account);
		if (conv != NULL && gaim_conversation_get_type(conv) == GAIM_CONV_IM)
			gaim_conv_im_set_icon(GAIM_CONV_IM(conv), NULL);

		for (list = sl = gaim_find_buddies(account, username); sl != NULL;
			 sl = sl->next)
		{
			GaimBuddy *buddy = (GaimBuddy *)sl->data;

			gaim_buddy_set_icon(buddy, NULL);
		}

		g_slist_free(list);

		return;
	}

	icon_cache = g_hash_table_lookup(account_cache,
									 gaim_buddy_icon_get_account(icon));

	if (icon_cache != NULL)
		g_hash_table_remove(icon_cache, gaim_buddy_icon_get_username(icon));

	if (icon->username != NULL)
		g_free(icon->username);

	if (icon->data != NULL)
		g_free(icon->data);

	g_free(icon);
}

GaimBuddyIcon *
gaim_buddy_icon_ref(GaimBuddyIcon *icon)
{
	g_return_val_if_fail(icon != NULL, NULL);

	icon->ref_count++;

	return icon;
}

GaimBuddyIcon *
gaim_buddy_icon_unref(GaimBuddyIcon *icon)
{
	g_return_val_if_fail(icon != NULL, NULL);

	if (icon->ref_count <= 0)
		return NULL;

	icon->ref_count--;

	if (icon->ref_count == 0)
	{
		gaim_buddy_icon_destroy(icon);

		return NULL;
	}

	return icon;
}

void
gaim_buddy_icon_update(GaimBuddyIcon *icon)
{
	GaimConversation *conv;
	GaimAccount *account;
	const char *username;
	GSList *sl, *list;

	g_return_if_fail(icon != NULL);

	account  = gaim_buddy_icon_get_account(icon);
	username = gaim_buddy_icon_get_username(icon);

	for (list = sl = gaim_find_buddies(account, username); sl != NULL;
		 sl = sl->next)
	{
		GaimBuddy *buddy = (GaimBuddy *)sl->data;

		gaim_buddy_set_icon(buddy, icon);
	}

	g_slist_free(list);

	conv = gaim_find_conversation_with_account(username, account);

	if (conv != NULL && gaim_conversation_get_type(conv) == GAIM_CONV_IM)
		gaim_conv_im_set_icon(GAIM_CONV_IM(conv), icon);
}

void
gaim_buddy_icon_cache(GaimBuddyIcon *icon, GaimBuddy *buddy)
{
	const void *data;
	const char *dirname;
	char *random;
	char *filename;
	const char *old_icon;
	size_t len;
	struct stat st;
	FILE *file = NULL;

	g_return_if_fail(icon  != NULL);
	g_return_if_fail(buddy != NULL);

	if (!gaim_buddy_icons_is_caching())
		return;

	data = gaim_buddy_icon_get_data(icon, &len);

	random   = g_strdup_printf("%x", g_random_int());
	dirname  = gaim_buddy_icons_get_cache_dir();
	filename = g_build_filename(dirname, random, NULL);
	old_icon = gaim_blist_node_get_string((GaimBlistNode*)buddy, "buddy_icon");

	if (!g_file_test(dirname, G_FILE_TEST_IS_DIR))
	{
		gaim_debug_info("buddy icons", "Creating icon cache directory.\n");

		if (mkdir(dirname, S_IRUSR | S_IWUSR | S_IXUSR) < 0)
		{
			gaim_debug_error("buddy icons",
							 "Unable to create directory %s: %s\n",
							 dirname, strerror(errno));
		}
	}

	if ((file = fopen(filename, "wb")) != NULL)
	{
		fwrite(data, 1, len, file);
		fclose(file);
	}

	g_free(filename);

	if (old_icon != NULL)
	{
		if(!stat(old_icon, &st))
			unlink(old_icon);
		else {
			filename = g_build_filename(dirname, random, NULL);
			if(!stat(filename, &st))
				unlink(filename);
			g_free(filename);
		}
	}

	gaim_blist_node_set_string((GaimBlistNode *)buddy, "buddy_icon", random);

	g_free(random);
}

void
gaim_buddy_icon_set_account(GaimBuddyIcon *icon, GaimAccount *account)
{
	g_return_if_fail(icon    != NULL);
	g_return_if_fail(account != NULL);

	icon->account = account;
}

void
gaim_buddy_icon_set_username(GaimBuddyIcon *icon, const char *username)
{
	g_return_if_fail(icon     != NULL);
	g_return_if_fail(username != NULL);

	if (icon->username != NULL)
		g_free(icon->username);

	icon->username = g_strdup(username);
}

void
gaim_buddy_icon_set_data(GaimBuddyIcon *icon, void *data, size_t len)
{
	g_return_if_fail(icon != NULL);

	if (icon->data != NULL)
		g_free(icon->data);

	if (data != NULL && len > 0)
	{
		icon->data = g_memdup(data, len);
		icon->len  = len;
	}
	else
	{
		icon->data = NULL;
		icon->len  = 0;
	}

	gaim_buddy_icon_update(icon);
}

GaimAccount *
gaim_buddy_icon_get_account(const GaimBuddyIcon *icon)
{
	g_return_val_if_fail(icon != NULL, NULL);

	return icon->account;
}

const char *
gaim_buddy_icon_get_username(const GaimBuddyIcon *icon)
{
	g_return_val_if_fail(icon != NULL, NULL);

	return icon->username;
}

const void *
gaim_buddy_icon_get_data(const GaimBuddyIcon *icon, size_t *len)
{
	g_return_val_if_fail(icon != NULL, NULL);

	if (len != NULL)
		*len = icon->len;

	return icon->data;
}

void
gaim_buddy_icons_set_for_user(GaimAccount *account, const char *username,
							  void *icon_data, size_t icon_len)
{
	g_return_if_fail(account  != NULL);
	g_return_if_fail(username != NULL);

	if (icon_data == NULL || icon_len == 0)
	{
		GaimBuddyIcon *buddy_icon;

		buddy_icon = gaim_buddy_icons_find(account, username);

		if (buddy_icon != NULL)
			gaim_buddy_icon_destroy(buddy_icon);
	}
	else
	{
		gaim_buddy_icon_new(account, username, icon_data, icon_len);
	}
}

GaimBuddyIcon *
gaim_buddy_icons_find(GaimAccount *account, const char *username)
{
	GHashTable *icon_cache;
	GaimBuddyIcon *ret = NULL;
	char *filename = NULL;

	g_return_val_if_fail(account  != NULL, NULL);
	g_return_val_if_fail(username != NULL, NULL);

	icon_cache = g_hash_table_lookup(account_cache, account);

	if ((icon_cache == NULL) || ((ret = g_hash_table_lookup(icon_cache, username)) == NULL)) {
		const char *file;
		struct stat st;
		GaimBuddy *b = gaim_find_buddy(account, username);

		if (!b)
			return NULL;

		if ((file = gaim_blist_node_get_string((GaimBlistNode*)b, "buddy_icon")) == NULL)
			return NULL;

		if (!stat(file, &st))
			filename = g_strdup(file);
		else
			filename = g_build_filename(gaim_buddy_icons_get_cache_dir(), file, NULL);

		if (!stat(filename, &st)) {
			FILE *f = fopen(filename, "rb");
			if (f) {
				char *data = g_malloc(st.st_size);
				fread(data, 1, st.st_size, f);
				fclose(f);
				ret = gaim_buddy_icon_create(account, username);
				gaim_buddy_icon_ref(ret);
				gaim_buddy_icon_set_data(ret, data, st.st_size);
				gaim_buddy_icon_unref(ret);
				g_free(data);
				g_free(filename);
				return ret;
			}
		}
		g_free(filename);
	}

	return ret;
}

void
gaim_buddy_icons_set_caching(gboolean caching)
{
	icon_caching = caching;
}

gboolean
gaim_buddy_icons_is_caching(void)
{
	return icon_caching;
}

void
gaim_buddy_icons_set_cache_dir(const char *dir)
{
	g_return_if_fail(cache_dir != NULL);

	if (cache_dir != NULL)
		g_free(cache_dir);

	cache_dir = g_strdup(dir);
}

const char *
gaim_buddy_icons_get_cache_dir(void)
{
	return cache_dir;
}

void *
gaim_buddy_icons_get_handle()
{
	static int handle;

	return &handle;
}

void
gaim_buddy_icons_init()
{
	account_cache = g_hash_table_new_full(
		g_direct_hash, g_direct_equal,
		NULL, (GFreeFunc)g_hash_table_destroy);

	cache_dir = g_build_filename(gaim_user_dir(), "icons", NULL);
}

void
gaim_buddy_icons_uninit()
{
	g_hash_table_destroy(account_cache);
}
