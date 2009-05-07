
#include <glib.h>
#include <gtk/gtk.h>
#include <webkit/webkit.h>

#include "notify.h"

#define GTK_TYPE_WEBVIEW            (gtk_webview_get_type())
#define GTK_WEBVIEW(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_TYPE_WEBVIEW, GtkWebView))

struct _GtkWebView
{
	WebKitWebView webkit_web_view;

	/*< private >*/
	GHashTable *images; /**< a map from id to temporary file for the image */
};

typedef struct _GtkWebView GtkWebView;

struct _GtkWebViewClass
{
	WebKitWebViewClass parent;
};

typedef struct _GtkWebViewClass GtkWebViewClass;


void
gtk_webview_append_text (GtkWebView *message_view);

void
gtk_webview_load_html_string_with_imgstore (GtkWebView* view, const char* html);

GtkWidget* gtk_webview_new ();

GType gtk_webview_get_type ();




