/*
 * Mono Plugin Loader
 *
 * -- Thanks to the perl plugin loader for all the great tips ;-)
 *
 * Eoin Coffey
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib.h>
#include <string.h>
#include "mono-helper.h"
#include "value.h"
#include "debug.h"

static gboolean _runtime_active = FALSE;

gboolean ml_init()
{
	MonoDomain *d = NULL;
	
	if (_runtime_active == TRUE)
		return TRUE;

	d = mono_jit_init_version("purple", "v2.0.50727");
	
	if (!d) {
		ml_set_domain(NULL);
		return FALSE;
	}
	
	ml_set_domain(d);
	
	ml_init_internal_calls();
	
	_runtime_active = TRUE;
	
	return TRUE;
}

void ml_uninit()
{
	g_return_if_fail(_runtime_active == TRUE);
	
	mono_jit_cleanup(ml_get_domain());
	
	ml_set_domain(NULL);
	
	_runtime_active = FALSE;
}

MonoObject* ml_plugin_get_handle(MonoObject *plugin)
{
	void* handle = (void*)mono_object_get_class(plugin);

	return mono_value_box(ml_get_domain(), mono_get_intptr_class(), &handle);
}

MonoObject* ml_invoke(MonoMethod *method, void *obj, void **params)
{
	MonoObject *ret = NULL, *exception = NULL;
	
	ret = mono_runtime_invoke(method, obj, params, &exception);
	if (exception) {
		purple_debug(PURPLE_DEBUG_ERROR, "mono", "invoke caught exception: %s\n", mono_class_get_name(mono_object_get_class(exception)));
	}
	
	return ret;
}

MonoClass* ml_find_plugin_class(MonoImage *image)
{
	MonoClass *klass = NULL, *pklass = NULL;
	int i, total;

	total = mono_image_get_table_rows (image, MONO_TABLE_TYPEDEF);
	for (i = 1; i <= total; ++i) {
		klass = mono_class_get (image, MONO_TOKEN_TYPE_DEF | i);
		
		pklass = mono_class_get_parent(klass);
		if (pklass) {
		
			if (strcmp("Plugin", mono_class_get_name(pklass)) == 0)
				return klass;
		}
	}
	
	return NULL;
}

void ml_set_prop_string(MonoObject *obj, char *field, char *data)
{
	MonoClass *klass = NULL;
	MonoProperty *prop = NULL;
	MonoString *str = NULL;
	gpointer args[1];
	
	klass = mono_object_get_class(obj);
	
	prop = mono_class_get_property_from_name(klass, field);
	
	str = mono_string_new(ml_get_domain(), data);
	
	args[0] = str;
	
	mono_property_set_value(prop, obj, args, NULL);
}

gchar* ml_get_prop_string(MonoObject *obj, char *field)
{
	MonoClass *klass = NULL;
	MonoProperty *prop = NULL;
	MonoString *str = NULL;
	
	klass = mono_object_get_class(obj);
	
	prop = mono_class_get_property_from_name(klass, field);
	
	str = (MonoString*)mono_property_get_value(prop, obj, NULL, NULL);
	
	return mono_string_to_utf8(str);
}

MonoObject* ml_get_info_prop(MonoObject *obj)
{
	MonoClass *klass = NULL;
	MonoProperty *prop = NULL;
	
	klass = mono_class_get_parent(mono_object_get_class(obj));
	
	prop = mono_class_get_property_from_name(klass, "Info");
	
	return mono_property_get_value(prop, obj, NULL, NULL);
}

gboolean ml_is_api_dll(MonoImage *image)
{	
	MonoClass *klass = NULL;
	int i, total;

	total = mono_image_get_table_rows (image, MONO_TABLE_TYPEDEF);
	for (i = 1; i <= total; ++i) {
		klass = mono_class_get (image, MONO_TOKEN_TYPE_DEF | i);
		if (strcmp(mono_class_get_name(klass), "Debug") == 0)
			if (strcmp(mono_class_get_namespace(klass), "Purple") == 0) {
				ml_set_api_image(image);
				return TRUE;
			}
	}
	
	return FALSE;
}

static MonoDomain *_domain = NULL;

MonoDomain* ml_get_domain(void)
{
	return _domain;
}

void ml_set_domain(MonoDomain *d)
{
	_domain = d;
}

static MonoImage *_api_image = NULL;

void ml_set_api_image(MonoImage *image)
{
	_api_image = image;
}

MonoImage* ml_get_api_image()
{
	return _api_image;
}

void ml_init_internal_calls(void)
{
	mono_add_internal_call("Purple.Plugin::_plugin_get_handle", ml_plugin_get_handle);
}

