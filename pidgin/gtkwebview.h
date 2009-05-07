
#include <glib.h>
#include <gtk/gtk.h>
#include <webkit/webkitwebview.h>

struct _GtkWebView
{
	WebKitWebView webkit_web_view;
};

typedef struct _GtkWebView GtkWebView;

struct _GtkWebViewClass
{
	WebKitWebViewClass parent;
};

typedef struct _GtkWebViewClass GtkWebViewClass;


void
gtk_webview_append_text (GtkWebView *message_view);

GtkWidget* gtk_webview_new ();

GType gtk_webview_get_type ();




