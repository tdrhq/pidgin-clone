#include "module.h"

typedef struct {
	char *cb;
} GaimPerlUrlData;

static void gaim_perl_util_url_cb(void *data, const char *url_data, size_t size) {
	GaimPerlUrlData *gpr = (GaimPerlUrlData *)data;
	dSP;
	ENTER;
	SAVETMPS;
	PUSHMARK(sp);

	XPUSHs(sv_2mortal(newSVpv(url_data, 0)));
	PUTBACK;

	call_pv(gpr->cb, G_EVAL | G_SCALAR);
	SPAGAIN;

	PUTBACK;
	FREETMPS;
	LEAVE;
}

MODULE = Gaim::Util  PACKAGE = Gaim::Util  PREFIX = gaim_
PROTOTYPES: ENABLE

void
gaim_url_fetch(handle, url, full, user_agent, http11, cb)
	Gaim::Plugin handle
	const char *url
	gboolean full
	const char *user_agent
	gboolean http11
	SV * cb
CODE:
	GaimPerlUrlData *gpr;
	STRLEN len;
	char *basename, *package;

	basename = g_path_get_basename(handle->path);
	gaim_perl_normalize_script_name(basename);
	package = g_strdup_printf("Gaim::Script::%s", basename);
	gpr = g_new(GaimPerlUrlData, 1);

	gpr->cb = g_strdup_printf("%s::%s", package, SvPV(cb, len));
	gaim_url_fetch(url, full, user_agent, http11, gaim_perl_util_url_cb, gpr);

int
gaim_build_dir(path, mode)
	const char *path
	int mode

gboolean
gaim_email_is_valid(address)
	const char *address

const char *
gaim_escape_filename(str)
	const char *str

char *
gaim_fd_get_ip(fd)
	int fd

const gchar *
gaim_home_dir()

gboolean
gaim_markup_extract_info_field(str, len, dest, start_token, skip, end_token, check_value, no_value_token, display_name, is_link, link_prefix)
	const char *str
	int len
	GString *dest
	const char *start_token
	int skip
	const char *end_token
	char check_value
	const char *no_value_token
	const char *display_name
	gboolean is_link
	const char *link_prefix

gboolean
gaim_markup_find_tag(needle, haystack, start, end, attributes)
	const char *needle
	const char *haystack
	const char **start
	const char **end
	GData **attributes

char *
gaim_markup_get_tag_name(tag)
	const char *tag

void
gaim_markup_html_to_xhtml(html, dest_xhtml, dest_plain)
	const char *html
	char **dest_xhtml
	char **dest_plain

char *
gaim_markup_linkify(str)
	const char *str

char *
gaim_markup_slice(str, x, y)
	const char *str
	guint x
	guint y

char *
gaim_markup_strip_html(str)
	const char *str

gboolean
gaim_message_meify(message, len)
	char *message
	size_t len

FILE *
gaim_mkstemp(path, binary)
	char **path
	gboolean binary

const char *
gaim_normalize(account, str)
	Gaim::Account account
	const char *str

gboolean
gaim_program_is_valid(program)
	const char *program

char *
gaim_str_add_cr(str)
	const char *str

char *
gaim_str_binary_to_ascii(binary, len)
	const unsigned char *binary
	guint len

gboolean
gaim_str_has_prefix(s, p)
	const char *s
	const char *p

gboolean
gaim_str_has_suffix(s, x)
	const char *s
	const char *x

char *
gaim_str_seconds_to_string(sec)
	guint sec

char *
gaim_str_size_to_units(size)
	size_t size

void
gaim_str_strip_char(str, thechar)
	char *str
	char thechar

gchar *
gaim_str_sub_away_formatters(str, name)
	const char *str
	const char *name

time_t
gaim_str_to_time(timestamp, utc)
	const char *timestamp
	gboolean utc

gchar *
gaim_strcasereplace(string, delimiter, replacement)
	const char *string
	const char *delimiter
	const char *replacement

const char *
gaim_strcasestr(haystack, needle)
	const char *haystack
	const char *needle

gchar *
gaim_strdup_withhtml(src)
	const gchar *src

size_t
gaim_strftime(s, max, format, tm)
	char *s
	size_t max
	const char *format
	const struct tm *tm

gchar *
gaim_strreplace(string, delimiter, replacement)
	const char *string
	const char *delimiter
	const char *replacement

char *
gaim_text_strip_mnemonic(in)
	const char *in

time_t
gaim_time_build(year, month, day, hour, min, sec)
	int year
	int month
	int day
	int hour
	int min
	int sec

const char *
gaim_unescape_filename(str)
	const char *str

char *
gaim_unescape_html(html)
	const char *html

const char *
gaim_url_decode(str)
	const char *str

const char *
gaim_url_encode(str)
	const char *str

gboolean
gaim_url_parse(url, ret_host, ret_port, ret_path, ret_user, ret_passwd)
	const char *url
	char **ret_host
	int *ret_port
	char **ret_path
	char **ret_user
	char **ret_passwd

const char *
gaim_user_dir()

void
gaim_util_set_user_dir(dir)
	const char *dir

gboolean
gaim_util_write_data_to_file(filename, data, size)
	const char *filename
	const char *data
	size_t size
