#include <string.h>
#include "account.h"
#include "blist.h"
#include "mono-helper.h"
#include "mono-glue.h"

MonoObject* purple_account_build_account_object(void* data)
{
	MonoObject *obj = NULL;
			
	PurpleAccount *account = (PurpleAccount*)data;
	
	obj = ml_create_api_object("Account", data);
	g_return_val_if_fail(obj != NULL, NULL);
		
	return obj;
}

MonoString* purple_account_get_username_glue(MonoObject *handle)
{
	PurpleAccount *account = (PurpleAccount*)mono_object_unbox(handle);

	return mono_string_new_wrapper(purple_account_get_username(account));
}

MonoString* purple_account_get_protocol_id_glue(MonoObject *handle)
{
	PurpleAccount *account = (PurpleAccount*)mono_object_unbox(handle);

	return mono_string_new_wrapper(purple_account_get_protocol_id(account));
}

MonoObject* purple_account_new_glue(MonoObject* account_handle, MonoString *name, MonoString *alias)
{
	PurpleAccount *account;
	char *cname = mono_string_to_utf8(name);
	char *calias = mono_string_to_utf8(alias);

	account = purple_account_new(cname, calias);
	
	return mono_value_box(ml_get_domain(), mono_get_intptr_class(), &account);
}