
#include <ctype.h>
#include <string.h>
#include <glib.h>

#include "gtkwebview.h"

void
gtk_webview_append_text (GtkWebView *message_view)
{

}

GtkWidget*
gtk_webview_new ()
{
	return GTK_WIDGET(g_object_new(gtk_webview_get_type(), NULL));
}

/*
 * Replace all <img id=""> tags with <img src="">. I hoped to never
 * write any HTML parsing code, but I'm forced to do this, until 
 * purple changes the way it works.
 */
static char*
replace_img_id_with_src (const char* html)
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

		g_string_append_printf (buffer, " src='%s' ", purple_imgstore_get_filename (
						purple_imgstore_find_by_id (nid)));
	}
	return g_string_free (buffer, FALSE);
}
static void
gtk_webview_class_init (GtkWebViewClass *klass, gpointer userdata)
{
	/* nothing to do really */
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
gtk_webview_load_html_string_with_imgstore (GtkWebView* view, const char* html, const char *loc)
{
	char* html_imged = replace_img_id_with_src (html);
	printf ("%s\n", html_imged);
	webkit_web_view_load_html_string (WEBKIT_WEB_VIEW (view), html_imged, loc);
	g_free (html_imged);
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
