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
