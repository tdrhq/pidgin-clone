
#include <ctype.h>
#include <string.h>
#include <glib.h>
#include <glib/gstdio.h>

#include "gtkwebview.h"
#include "util.h"

static WebKitWebViewClass *parent_class = NULL;

void
gtk_webview_append_text (GtkWebView *message_view)
{

}

GtkWidget*
gtk_webview_new ()
{
	return GTK_WIDGET(g_object_new(gtk_webview_get_type(), NULL));
}

static char*
get_img_filename_by_id (GtkWebView* view, int id)
{
	char *filename = NULL;
	FILE *file;
	PurpleStoredImage* img;

	if (!view->images)
		view->images = g_hash_table_new_full (g_direct_hash, g_direct_equal, NULL, g_free);
	
	filename = (char*) g_hash_table_lookup (view->images, GINT_TO_POINTER (id));
	if (filename) return filename;
			
	/* else get from img store */
	file = purple_mkstemp (&filename, TRUE);

	img = purple_imgstore_find_by_id (id);

	fwrite (purple_imgstore_get_data (img), purple_imgstore_get_size (img), 1, file);
	g_hash_table_insert (view->images, GINT_TO_POINTER (id), filename);
	fclose (file);
	return filename;
}

static void 
clear_single_image (gpointer key, gpointer value, gpointer userdata) 
{
	g_unlink ((char*) value);
}

static void
clear_images (GtkWebView* view)
{
	if (!view->images) return;
	g_hash_table_foreach (view->images, clear_single_image, NULL);
	g_hash_table_unref (view->images);
}

/*
 * Replace all <img id=""> tags with <img src="">. I hoped to never
 * write any HTML parsing code, but I'm forced to do this, until 
 * purple changes the way it works.
 */
static char*
replace_img_id_with_src (GtkWebView *view, const char* html)
{
	GString *buffer = g_string_sized_new (strlen (html));
	const char* cur = html;
	char *id;
	int nid;

	while (*cur) {
		const char* img = strstr (cur, "<img");
		if (!img) {
			g_string_append (buffer, cur);
			break;
		} else
			g_string_append_len (buffer, cur, img - cur);

		cur = strstr (img, "/>");
		if (!cur)
			cur = strstr (img, ">");

		if (!cur) { /*oops, invalid html */
			g_string_printf (buffer, "%s", html);
			break;
		}

		if (strstr (img, "src=") || !strstr (img, "id=")) {
			g_string_printf (buffer, "%s", html);
			break;
		}

		/* now I _kinda_ know that it has an id=, and does not have a src= */
		/* todo: take care of src= and id= appearing in strings? */
		id = strstr (img, "id=") + 3; 

		/* *id can't be \0, since a ">" appears after this */
		if (isdigit (*id)) 
			nid = atoi (id);
		else 
			nid = atoi (id+1);

		/* let's dump this, tag and then dump the src information */
		g_string_append_len (buffer, img, cur - img);

		g_string_append_printf (buffer, " src='file://%s' ", get_img_filename_by_id (view, nid));
	}
	return g_string_free (buffer, FALSE);
}

static void
gtk_webview_finalize (GObject *view)
{
	clear_images (GTK_WEBVIEW (view));
	G_OBJECT_CLASS (parent_class)->finalize (G_OBJECT(view));
}

static void
gtk_webview_class_init (GtkWebViewClass *klass, gpointer userdata)
{
	parent_class = g_type_class_ref (webkit_web_view_get_type ());
	G_OBJECT_CLASS (klass)->finalize = gtk_webview_finalize;
}

static gboolean
webview_link_clicked (WebKitWebView *view,
                     WebKitWebFrame *frame,
                     WebKitNetworkRequest *request,
                     WebKitWebNavigationAction *navigation_action,
                     WebKitWebPolicyDecision *policy_decision)
{
	const gchar *uri;
	
	uri = webkit_network_request_get_uri (request);

	/* the gtk imhtml way was to create an idle cb, not sure
	 * why, so right now just using purple_notify_uri directly */
	purple_notify_uri (NULL, uri);
	return TRUE;
}

static void
gtk_webview_init (GtkWebView *view, gpointer userdata)
{
	g_signal_connect (view, "navigation-policy-decision-requested",
			  G_CALLBACK (webview_link_clicked),
			  view);
}


void
gtk_webview_load_html_string_with_imgstore (GtkWebView* view, const char* html)
{
	char* html_imged;
	
	clear_images (view);
	html_imged = replace_img_id_with_src (view, html);
	printf ("%s\n", html_imged);
	webkit_web_view_load_html_string (WEBKIT_WEB_VIEW (view), html_imged, "file:///");
	g_free (html_imged);
}

/* taken from sean's webkit plugin */
static char *escape_message(const char *text)
{
        GString *str = g_string_new(NULL);
        const char *cur = text;

        while (cur && *cur) {
                switch (*cur) {
                case '\\':
                        g_string_append(str, "\\\\");   
                        break;
                case '\"':
                        g_string_append(str, "\\\"");
                        break;
                case '\r':
                        g_string_append(str, "<br/>");
                        break;
                case '\n':
                        break;
                default:
			g_string_append_c(str, *cur);
		}
		cur ++;
	}
	return g_string_free (str, FALSE);
}


/* this is a "hack", my plan is to eventually handle this 
 * correctly using a signals and a plugin: the plugin will have
 * the information as to what javascript function to call. It seems
 * wrong to hardcode that here.
 */
void
gtk_webview_append_html (GtkWebView* view, const char* html)
{
	char* escaped = escape_message (html);
	char* script = g_strdup_printf ("document.write(\"%s<br />\")", escaped);
	printf ("script: %s\n", script);
	webkit_web_view_execute_script (WEBKIT_WEB_VIEW (view), script);
	g_free (script);
	g_free (escaped);
}

GType gtk_webview_get_type ()
{
	static GType mview_type = 0;
	if (!mview_type) {
		static const GTypeInfo mview_info = {
			sizeof (GtkWebViewClass),
			NULL,
			NULL,
			(GClassInitFunc) gtk_webview_class_init,
			NULL,
			NULL,
			sizeof (GtkWebView),
			0,
			(GInstanceInitFunc) gtk_webview_init,
			NULL
		};
		mview_type = g_type_register_static(webkit_web_view_get_type (),
				"GtkWebView", &mview_info, 0);
	}
	return mview_type;
}