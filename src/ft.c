/**
 * @file ft.c File Transfer API
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
 *
 */
#include "internal.h"
#include "ft.h"
#include "network.h"
#include "notify.h"
#include "prefs.h"
#include "proxy.h"
#include "request.h"
#include "util.h"

static GaimXferUiOps *xfer_ui_ops = NULL;

GaimXfer *
gaim_xfer_new(GaimAccount *account, GaimXferType type, const char *who)
{
	GaimXfer *xfer;
	GaimXferUiOps *ui_ops;

	g_return_val_if_fail(type    != GAIM_XFER_UNKNOWN, NULL);
	g_return_val_if_fail(account != NULL,              NULL);
	g_return_val_if_fail(who     != NULL,              NULL);

	xfer = g_new0(GaimXfer, 1);

	xfer->ref = 1;
	xfer->type    = type;
	xfer->account = account;
	xfer->who     = g_strdup(who);
	xfer->ui_ops  = gaim_xfers_get_ui_ops();

	ui_ops = gaim_xfer_get_ui_ops(xfer);

	if (ui_ops != NULL && ui_ops->new_xfer != NULL)
		ui_ops->new_xfer(xfer);

	return xfer;
}

static void
gaim_xfer_destroy(GaimXfer *xfer)
{
	GaimXferUiOps *ui_ops;

	g_return_if_fail(xfer != NULL);

	if (gaim_xfer_get_status(xfer) == GAIM_XFER_STATUS_STARTED)
		gaim_xfer_cancel_local(xfer);

	ui_ops = gaim_xfer_get_ui_ops(xfer);

	if (ui_ops != NULL && ui_ops->destroy != NULL)
		ui_ops->destroy(xfer);

	g_free(xfer->who);
	g_free(xfer->filename);

	if (xfer->remote_ip != NULL) g_free(xfer->remote_ip);

	if (xfer->local_filename != NULL)
		g_free(xfer->local_filename);

	g_free(xfer);
}

void
gaim_xfer_ref(GaimXfer *xfer)
{
	g_return_if_fail(xfer != NULL);

	xfer->ref++;
}

void
gaim_xfer_unref(GaimXfer *xfer)
{
	g_return_if_fail(xfer != NULL);

	xfer->ref--;

	if (xfer->ref == 0)
		gaim_xfer_destroy(xfer);
}

static void
gaim_xfer_choose_file_ok_cb(void *user_data, const char *filename)
{
	GaimXfer *xfer;
	struct stat st;

	xfer = (GaimXfer *)user_data;

	if (stat(filename, &st) != 0) {
		/* File not found. */
		if (gaim_xfer_get_type(xfer) == GAIM_XFER_RECEIVE) {
			gaim_xfer_request_accepted(xfer, filename);
		}
		else {
			gaim_notify_error(NULL, NULL,
							  _("That file does not exist."), NULL);

			gaim_xfer_request_denied(xfer);
		}
	}
	else if ((gaim_xfer_get_type(xfer) == GAIM_XFER_SEND) &&
			 (st.st_size == 0)) {

		gaim_notify_error(NULL, NULL,
						  _("Cannot send a file of 0 bytes."), NULL);

		gaim_xfer_request_denied(xfer);
	}
	else {
		if (S_ISDIR(st.st_mode)) {
			/* XXX */
			gaim_xfer_request_denied(xfer);
		}
		else if (gaim_xfer_get_type(xfer) == GAIM_XFER_RECEIVE) {
			gaim_xfer_request_accepted(xfer, filename);
		}
		else {
			gaim_xfer_request_accepted(xfer, filename);
		}
	}

	gaim_xfer_unref(xfer);
}

static void
gaim_xfer_choose_file_cancel_cb(void *user_data, const char *filename)
{
	GaimXfer *xfer = (GaimXfer *)user_data;

	gaim_xfer_request_denied(xfer);
}

static int
gaim_xfer_choose_file(GaimXfer *xfer)
{
	gaim_request_file(xfer, NULL, gaim_xfer_get_filename(xfer),
					  (gaim_xfer_get_type(xfer) == GAIM_XFER_RECEIVE),
					  G_CALLBACK(gaim_xfer_choose_file_ok_cb),
					  G_CALLBACK(gaim_xfer_choose_file_cancel_cb), xfer);

	return 0;
}

static int
cancel_recv_cb(GaimXfer *xfer)
{
	gaim_xfer_request_denied(xfer);
	gaim_xfer_unref(xfer);

	return 0;
}

static void
gaim_xfer_ask_recv(GaimXfer *xfer)
{
	char *buf, *size_buf;
	size_t size;

	/* If we have already accepted the request, ask the destination file
	   name directly */
	if (gaim_xfer_get_status(xfer) != GAIM_XFER_STATUS_ACCEPTED) {
		size = gaim_xfer_get_size(xfer);
		size_buf = gaim_str_size_to_units(size);

		buf = g_strdup_printf(_("%s wants to send you %s (%s)"),
				      xfer->who, gaim_xfer_get_filename(xfer),
				      size_buf);
		g_free(size_buf);

		gaim_request_accept_cancel(NULL, NULL, buf, NULL, 0, xfer,
					   G_CALLBACK(gaim_xfer_choose_file),
					   G_CALLBACK(cancel_recv_cb));
		g_free(buf);
	} else
		gaim_xfer_choose_file(xfer);
}

static int
ask_accept_ok(GaimXfer *xfer)
{
	gaim_xfer_request_accepted(xfer, NULL);
	gaim_xfer_unref(xfer);

	return 0;
}

static int
ask_accept_cancel(GaimXfer *xfer)
{
	gaim_xfer_request_denied(xfer);
	gaim_xfer_unref(xfer);

	return 0;
}

static void
gaim_xfer_ask_accept(GaimXfer *xfer)
{
	char *buf, *buf2 = NULL;

	buf = g_strdup_printf(_("Accept file transfer request from %s?"),
			      xfer->who);
	if (gaim_xfer_get_remote_ip(xfer) &&
	    gaim_xfer_get_remote_port(xfer))
		buf2 = g_strdup_printf(_("A file is available for download from:\n"
					 "Remote host: %s\nRemote port: %d"),
				       gaim_xfer_get_remote_ip(xfer),
				       gaim_xfer_get_remote_port(xfer));
	gaim_request_accept_cancel(NULL, NULL, buf, buf2, 0, xfer,
				   G_CALLBACK(ask_accept_ok),
				   G_CALLBACK(ask_accept_cancel));
	g_free(buf);
	g_free(buf2);
}

void
gaim_xfer_request(GaimXfer *xfer)
{
	g_return_if_fail(xfer != NULL);
	g_return_if_fail(xfer->ops.init != NULL);

	gaim_xfer_ref(xfer);

	if (gaim_xfer_get_type(xfer) == GAIM_XFER_RECEIVE) {
		if (gaim_xfer_get_filename(xfer) ||
		    gaim_xfer_get_status(xfer) == GAIM_XFER_STATUS_ACCEPTED)
			gaim_xfer_ask_recv(xfer);
		else
			gaim_xfer_ask_accept(xfer);
	} else
		gaim_xfer_choose_file(xfer);
}

void
gaim_xfer_request_accepted(GaimXfer *xfer, const char *filename)
{
	GaimXferType type;

	if (xfer == NULL)
		return;

	type = gaim_xfer_get_type(xfer);

	if (!filename && type == GAIM_XFER_RECEIVE) {
		xfer->status = GAIM_XFER_STATUS_ACCEPTED;
		xfer->ops.init(xfer);
		return;
	}

	if (type == GAIM_XFER_SEND) {
		struct stat sb;

		/* Check the filename. */
		if (g_strrstr(filename, "..")) {
			char *msg;

			msg = g_strdup_printf(_("%s is not a valid filename.\n"),
								  filename);

			gaim_xfer_error(type, xfer->who, msg);

			g_free(msg);
			gaim_xfer_unref(xfer);
			return;
		}

		if (stat(filename, &sb) == -1) {
			char *msg;

			msg = g_strdup_printf(_("%s was not found.\n"), filename);

			gaim_xfer_error(type, xfer->who, msg);

			g_free(msg);
			gaim_xfer_unref(xfer);
			return;
		}

		gaim_xfer_set_local_filename(xfer, filename);
		gaim_xfer_set_size(xfer, sb.st_size);
	}
	else {
		/* TODO: Sanity checks and file overwrite checks. */

		gaim_xfer_set_local_filename(xfer, filename);
	}


	xfer->ops.init(xfer);
}

void
gaim_xfer_request_denied(GaimXfer *xfer)
{
	g_return_if_fail(xfer != NULL);

	if (xfer->ops.request_denied != NULL)
		xfer->ops.request_denied(xfer);

	gaim_xfer_unref(xfer);
}

GaimXferType
gaim_xfer_get_type(const GaimXfer *xfer)
{
	g_return_val_if_fail(xfer != NULL, GAIM_XFER_UNKNOWN);

	return xfer->type;
}

GaimAccount *
gaim_xfer_get_account(const GaimXfer *xfer)
{
	g_return_val_if_fail(xfer != NULL, NULL);

	return xfer->account;
}

GaimXferStatusType
gaim_xfer_get_status(const GaimXfer *xfer)
{
	g_return_val_if_fail(xfer != NULL, GAIM_XFER_STATUS_UNKNOWN);

	return xfer->status;
}

gboolean
gaim_xfer_is_canceled(const GaimXfer *xfer)
{
	g_return_val_if_fail(xfer != NULL, TRUE);

	if ((gaim_xfer_get_status(xfer) == GAIM_XFER_STATUS_CANCEL_LOCAL) ||
	    (gaim_xfer_get_status(xfer) == GAIM_XFER_STATUS_CANCEL_REMOTE))
		return TRUE;
	else
		return FALSE;
}

gboolean
gaim_xfer_is_completed(const GaimXfer *xfer)
{
	g_return_val_if_fail(xfer != NULL, TRUE);

	return (gaim_xfer_get_status(xfer) == GAIM_XFER_STATUS_DONE);
}

const char *
gaim_xfer_get_filename(const GaimXfer *xfer)
{
	g_return_val_if_fail(xfer != NULL, NULL);

	return xfer->filename;
}

const char *
gaim_xfer_get_local_filename(const GaimXfer *xfer)
{
	g_return_val_if_fail(xfer != NULL, NULL);

	return xfer->local_filename;
}

size_t
gaim_xfer_get_bytes_sent(const GaimXfer *xfer)
{
	g_return_val_if_fail(xfer != NULL, 0);

	return xfer->bytes_sent;
}

size_t
gaim_xfer_get_bytes_remaining(const GaimXfer *xfer)
{
	g_return_val_if_fail(xfer != NULL, 0);

	return xfer->bytes_remaining;
}

size_t
gaim_xfer_get_size(const GaimXfer *xfer)
{
	g_return_val_if_fail(xfer != NULL, 0);

	return xfer->size;
}

double
gaim_xfer_get_progress(const GaimXfer *xfer)
{
	g_return_val_if_fail(xfer != NULL, 0.0);

	if (gaim_xfer_get_size(xfer) == 0)
		return 0.0;

	return ((double)gaim_xfer_get_bytes_sent(xfer) /
			(double)gaim_xfer_get_size(xfer));
}

unsigned int
gaim_xfer_get_local_port(const GaimXfer *xfer)
{
	g_return_val_if_fail(xfer != NULL, -1);

	return xfer->local_port;
}

const char *
gaim_xfer_get_remote_ip(const GaimXfer *xfer)
{
	g_return_val_if_fail(xfer != NULL, NULL);

	return xfer->remote_ip;
}

unsigned int
gaim_xfer_get_remote_port(const GaimXfer *xfer)
{
	g_return_val_if_fail(xfer != NULL, -1);

	return xfer->remote_port;
}

static void
gaim_xfer_set_status(GaimXfer *xfer, GaimXferStatusType status)
{
	g_return_if_fail(xfer != NULL);

	xfer->status = status;
}

void
gaim_xfer_set_completed(GaimXfer *xfer, gboolean completed)
{
	GaimXferUiOps *ui_ops;

	g_return_if_fail(xfer != NULL);

	if (completed == TRUE)
		gaim_xfer_set_status(xfer, GAIM_XFER_STATUS_DONE);

	ui_ops = gaim_xfer_get_ui_ops(xfer);

	if (ui_ops != NULL && ui_ops->update_progress != NULL)
		ui_ops->update_progress(xfer, gaim_xfer_get_progress(xfer));
}

void
gaim_xfer_set_filename(GaimXfer *xfer, const char *filename)
{
	g_return_if_fail(xfer != NULL);

	if (xfer->filename != NULL)
		g_free(xfer->filename);

	xfer->filename = (filename == NULL ? NULL : g_strdup(filename));
}

void
gaim_xfer_set_local_filename(GaimXfer *xfer, const char *filename)
{
	g_return_if_fail(xfer != NULL);

	if (xfer->local_filename != NULL)
		g_free(xfer->local_filename);

	xfer->local_filename = (filename == NULL ? NULL : g_strdup(filename));
}

void
gaim_xfer_set_size(GaimXfer *xfer, size_t size)
{
	g_return_if_fail(xfer != NULL);

	if (xfer->size == 0)
		xfer->bytes_remaining = size - xfer->bytes_sent;

	xfer->size = size;
}

GaimXferUiOps *
gaim_xfer_get_ui_ops(const GaimXfer *xfer)
{
	g_return_val_if_fail(xfer != NULL, NULL);

	return xfer->ui_ops;
}

void
gaim_xfer_set_init_fnc(GaimXfer *xfer, void (*fnc)(GaimXfer *))
{
	g_return_if_fail(xfer != NULL);

	xfer->ops.init = fnc;
}

void gaim_xfer_set_request_denied_fnc(GaimXfer *xfer, void (*fnc)(GaimXfer *))
{
	g_return_if_fail(xfer != NULL);

	xfer->ops.request_denied = fnc;
}

void
gaim_xfer_set_read_fnc(GaimXfer *xfer, ssize_t (*fnc)(char **, GaimXfer *))
{
	g_return_if_fail(xfer != NULL);

	xfer->ops.read = fnc;
}

void
gaim_xfer_set_write_fnc(GaimXfer *xfer,
						ssize_t (*fnc)(const char *, size_t, GaimXfer *))
{
	g_return_if_fail(xfer != NULL);

	xfer->ops.write = fnc;
}

void
gaim_xfer_set_ack_fnc(GaimXfer *xfer,
			  void (*fnc)(GaimXfer *, const char *, size_t))
{
	g_return_if_fail(xfer != NULL);

	xfer->ops.ack = fnc;
}

void
gaim_xfer_set_start_fnc(GaimXfer *xfer, void (*fnc)(GaimXfer *))
{
	g_return_if_fail(xfer != NULL);

	xfer->ops.start = fnc;
}

void
gaim_xfer_set_end_fnc(GaimXfer *xfer, void (*fnc)(GaimXfer *))
{
	g_return_if_fail(xfer != NULL);

	xfer->ops.end = fnc;
}

void
gaim_xfer_set_cancel_send_fnc(GaimXfer *xfer, void (*fnc)(GaimXfer *))
{
	g_return_if_fail(xfer != NULL);

	xfer->ops.cancel_send = fnc;
}

void
gaim_xfer_set_cancel_recv_fnc(GaimXfer *xfer, void (*fnc)(GaimXfer *))
{
	g_return_if_fail(xfer != NULL);

	xfer->ops.cancel_recv = fnc;
}

ssize_t
gaim_xfer_read(GaimXfer *xfer, char **buffer)
{
	ssize_t s, r;

	g_return_val_if_fail(xfer   != NULL, 0);
	g_return_val_if_fail(buffer != NULL, 0);

	if (gaim_xfer_get_size(xfer) == 0)
		s = 4096;
	else
		s = MIN(gaim_xfer_get_bytes_remaining(xfer), 4096);

	if (xfer->ops.read != NULL)
		r = (xfer->ops.read)(buffer, xfer);
	else {
		*buffer = g_malloc0(s);

		r = read(xfer->fd, *buffer, s);
		if ((gaim_xfer_get_size(xfer) > 0) &&
			((gaim_xfer_get_bytes_sent(xfer)+r) >= gaim_xfer_get_size(xfer)))
			gaim_xfer_set_completed(xfer, TRUE);
		else if(r <= 0)
			r = -1;
	}

	return r;
}

ssize_t
gaim_xfer_write(GaimXfer *xfer, const char *buffer, size_t size)
{
	ssize_t r, s;

	g_return_val_if_fail(xfer   != NULL, 0);
	g_return_val_if_fail(buffer != NULL, 0);
	g_return_val_if_fail(size   != 0,    0);

	s = MIN(gaim_xfer_get_bytes_remaining(xfer), size);

	if (xfer->ops.write != NULL) {
		r = (xfer->ops.write)(buffer, s, xfer);
	} else {
		r = write(xfer->fd, buffer, s);
		if ((gaim_xfer_get_bytes_sent(xfer)+r) >= gaim_xfer_get_size(xfer))
			gaim_xfer_set_completed(xfer, TRUE);
	}

	return r;
}

static void
transfer_cb(gpointer data, gint source, GaimInputCondition condition)
{
	GaimXferUiOps *ui_ops;
	GaimXfer *xfer = (GaimXfer *)data;
	char *buffer = NULL;
	ssize_t r;

	if (condition & GAIM_INPUT_READ) {
		r = gaim_xfer_read(xfer, &buffer);
		if (r > 0) {
			fwrite(buffer, 1, r, xfer->dest_fp);
		} else if(r < 0) {
			gaim_xfer_cancel_remote(xfer);
			return;
		}
	}
	else {
		size_t s = MIN(gaim_xfer_get_bytes_remaining(xfer), 4096);

		/* this is so the prpl can keep the connection open
		   if it needs to for some odd reason. */
		if (s == 0) {
			if(xfer->watcher) {
				gaim_input_remove(xfer->watcher);
				xfer->watcher = 0;
			}
			return;
		}

		buffer = g_malloc0(s);

		fread(buffer, 1, s, xfer->dest_fp);

		/* Write as much as we're allowed to. */
		r = gaim_xfer_write(xfer, buffer, s);

		if (r == -1) {
			gaim_xfer_cancel_remote(xfer);
			g_free(buffer);
			return;
		} else if (r < s) {
			/* We have to seek back in the file now. */
			fseek(xfer->dest_fp, r - s, SEEK_CUR);
		}
	}

	if (gaim_xfer_get_size(xfer) > 0)
		xfer->bytes_remaining -= r;

	xfer->bytes_sent += r;

	if (xfer->ops.ack != NULL)
		xfer->ops.ack(xfer, buffer, r);

	g_free(buffer);

	ui_ops = gaim_xfer_get_ui_ops(xfer);

	if (ui_ops != NULL && ui_ops->update_progress != NULL)
		ui_ops->update_progress(xfer, gaim_xfer_get_progress(xfer));

	if (gaim_xfer_is_completed(xfer))
		gaim_xfer_end(xfer);
}

static void
begin_transfer(GaimXfer *xfer, GaimInputCondition cond)
{
	GaimXferUiOps *ui_ops;
	GaimXferType type = gaim_xfer_get_type(xfer);

	ui_ops = gaim_xfer_get_ui_ops(xfer);

	/*
	 * We'll have already tried to open this earlier to make sure we can
	 * read/write here. Should be safe.
	 */
	xfer->dest_fp = fopen(gaim_xfer_get_local_filename(xfer),
						  type == GAIM_XFER_RECEIVE ? "wb" : "rb");

	/* Just in case, though. */
	if (xfer->dest_fp == NULL) {
		gaim_xfer_cancel_local(xfer); /* ? */
		return;
	}

	xfer->watcher = gaim_input_add(xfer->fd, cond, transfer_cb, xfer);

	if (ui_ops != NULL && ui_ops->add_xfer != NULL)
		ui_ops->add_xfer(xfer);

	if (xfer->ops.start != NULL)
		xfer->ops.start(xfer);
}

static void
connect_cb(gpointer data, gint source, GaimInputCondition condition)
{
	GaimXfer *xfer = (GaimXfer *)data;

	xfer->fd = source;

	begin_transfer(xfer, condition);
}

void
gaim_xfer_start(GaimXfer *xfer, int fd, const char *ip,
				unsigned int port)
{
	GaimInputCondition cond;
	GaimXferType type;

	g_return_if_fail(xfer != NULL);
	g_return_if_fail(gaim_xfer_get_type(xfer) != GAIM_XFER_UNKNOWN);

	type = gaim_xfer_get_type(xfer);

	xfer->bytes_remaining = gaim_xfer_get_size(xfer);
	xfer->bytes_sent      = 0;

	gaim_xfer_set_status(xfer, GAIM_XFER_STATUS_STARTED);

	if (type == GAIM_XFER_RECEIVE) {
		cond = GAIM_INPUT_READ;

		if (ip != NULL) {
			xfer->remote_ip   = g_strdup(ip);
			xfer->remote_port = port;

			/* Establish a file descriptor. */
			gaim_proxy_connect(xfer->account, xfer->remote_ip,
							   xfer->remote_port, connect_cb, xfer);

			return;
		}
		else {
			xfer->fd = fd;
		}
	}
	else {
		cond = GAIM_INPUT_WRITE;

		xfer->fd = fd;
	}

	begin_transfer(xfer, cond);
}

void
gaim_xfer_end(GaimXfer *xfer)
{
	g_return_if_fail(xfer != NULL);

	/* See if we are actually trying to cancel this. */
	if (gaim_xfer_get_status(xfer) != GAIM_XFER_STATUS_DONE) {
		gaim_xfer_cancel_local(xfer);
		return;
	}

	if (xfer->ops.end != NULL)
		xfer->ops.end(xfer);

	if (xfer->watcher != 0) {
		gaim_input_remove(xfer->watcher);
		xfer->watcher = 0;
	}

	if (xfer->fd != 0)
		close(xfer->fd);

	if (xfer->dest_fp != NULL) {
		fclose(xfer->dest_fp);
		xfer->dest_fp = NULL;
	}

	gaim_xfer_unref(xfer);
}

void
gaim_xfer_add(GaimXfer *xfer)
{
	GaimXferUiOps *ui_ops;

	g_return_if_fail(xfer != NULL);

	ui_ops = gaim_xfer_get_ui_ops(xfer);

	if (ui_ops != NULL && ui_ops->add_xfer != NULL)
		ui_ops->add_xfer(xfer);
}

void
gaim_xfer_cancel_local(GaimXfer *xfer)
{
	GaimXferUiOps *ui_ops;

	g_return_if_fail(xfer != NULL);

	gaim_xfer_set_status(xfer, GAIM_XFER_STATUS_CANCEL_LOCAL);

	if (gaim_xfer_get_type(xfer) == GAIM_XFER_SEND)
	{
		if (xfer->ops.cancel_send != NULL)
			xfer->ops.cancel_send(xfer);
	}
	else
	{
		if (xfer->ops.cancel_recv != NULL)
			xfer->ops.cancel_recv(xfer);
	}

	if (xfer->watcher != 0) {
		gaim_input_remove(xfer->watcher);
		xfer->watcher = 0;
	}

	if (xfer->fd != 0)
		close(xfer->fd);

	if (xfer->dest_fp != NULL) {
		fclose(xfer->dest_fp);
		xfer->dest_fp = NULL;
	}

	ui_ops = gaim_xfer_get_ui_ops(xfer);

	if (ui_ops != NULL && ui_ops->cancel_local != NULL)
		ui_ops->cancel_local(xfer);

	xfer->bytes_remaining = 0;

	gaim_xfer_unref(xfer);
}

void
gaim_xfer_cancel_remote(GaimXfer *xfer)
{
	GaimXferUiOps *ui_ops;

	g_return_if_fail(xfer != NULL);

	gaim_xfer_set_status(xfer, GAIM_XFER_STATUS_CANCEL_REMOTE);

	if (gaim_xfer_get_type(xfer) == GAIM_XFER_SEND)
	{
		if (xfer->ops.cancel_send != NULL)
			xfer->ops.cancel_send(xfer);
	}
	else
	{
		if (xfer->ops.cancel_recv != NULL)
			xfer->ops.cancel_recv(xfer);
	}

	if (xfer->watcher != 0) {
		gaim_input_remove(xfer->watcher);
		xfer->watcher = 0;
	}

	if (xfer->fd != 0)
		close(xfer->fd);

	if (xfer->dest_fp != NULL) {
		fclose(xfer->dest_fp);
		xfer->dest_fp = NULL;
	}

	ui_ops = gaim_xfer_get_ui_ops(xfer);

	if (ui_ops != NULL && ui_ops->cancel_remote != NULL)
		ui_ops->cancel_remote(xfer);

	xfer->bytes_remaining = 0;

	gaim_xfer_unref(xfer);
}

void
gaim_xfer_error(GaimXferType type, const char *who, const char *msg)
{
	char *title;

	g_return_if_fail(msg  != NULL);
	g_return_if_fail(type != GAIM_XFER_UNKNOWN);

	if (type == GAIM_XFER_SEND)
		title = g_strdup_printf(_("File transfer to %s aborted.\n"), who);
	else
		title = g_strdup_printf(_("File transfer from %s aborted.\n"), who);

	gaim_notify_error(NULL, NULL, title, msg);

	g_free(title);
}

void
gaim_xfer_update_progress(GaimXfer *xfer)
{
	GaimXferUiOps *ui_ops;

	g_return_if_fail(xfer != NULL);

	ui_ops = gaim_xfer_get_ui_ops(xfer);
	if (ui_ops != NULL && ui_ops->update_progress != NULL)
		ui_ops->update_progress(xfer, gaim_xfer_get_progress(xfer));
}


/**************************************************************************
 * File Transfer Subsystem API
 **************************************************************************/

void
gaim_xfers_set_ui_ops(GaimXferUiOps *ops)
{
	xfer_ui_ops = ops;
}

GaimXferUiOps *
gaim_xfers_get_ui_ops(void)
{
	return xfer_ui_ops;
}
