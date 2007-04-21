#include <string.h>
#include "account.h"
#include "blist.h"
#include "mono-helper.h"
#include "mono-glue.h"

MonoObject* purple_blist_get_handle_glue(void)
{
	void *handle = purple_blist_get_handle();
	
	return mono_value_box(ml_get_domain(), mono_get_intptr_class(), &handle);
}

MonoObject* purple_blist_build_buddy_object(void* data)
{
	MonoObject *obj = NULL;
			
	PurpleBuddy *buddy = (PurpleBuddy*)data;
	
	obj = ml_create_api_object("Buddy", data);
	g_return_val_if_fail(obj != NULL, NULL);
		
	/*ml_set_prop_string(obj, "Name", (char*)purple_buddy_get_name(buddy));
	ml_set_prop_string(obj, "Alias", (char*)purple_buddy_get_alias(buddy));*/
	
	return obj;
}

MonoString* purple_buddy_get_name_glue(MonoObject *obj, MonoObject *handle)
{
	PurpleBuddy *buddy;
	
	purple_debug_info("mono", "obj class name : %s\n", mono_class_get_name(mono_object_get_class(obj)));
	purple_debug_info("mono", "class name: %s\n", mono_class_get_name(mono_object_get_class(handle)));
	
	/*buddy = (PurpleBuddy*)mono_object_unbox(handle);*/

	return mono_string_new_wrapper("yarg");
}

MonoString* purple_buddy_get_alias_glue(MonoObject *handle)
{
	/*PurpleBuddy *buddy = (PurpleBuddy*)mono_object_unbox(handle);*/

	return mono_string_new_wrapper("blah");
}

MonoObject* purple_buddy_new_glue(MonoObject* account_handle, MonoString *name, MonoString *alias)
{
	PurpleBuddy *buddy;
	char *cname = mono_string_to_utf8(name);
	char *calias = mono_string_to_utf8(alias);

	buddy = purple_buddy_new((PurpleAccount*)mono_object_unbox(account_handle), cname, calias);
	
	return mono_value_box(ml_get_domain(), mono_get_intptr_class(), &buddy);
}
