#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <glib.h>

#include "libtest.h"

int get_strlen(char* str)
{
	return strlen(str);
}

char* say_hello()
{
	return "Hello";
}

void set_hello(char* str)
{
	printf("%s\n", str);
}



TestStruct* create_struct(char *s)
{
	TestStruct* ret = (TestStruct*)malloc(sizeof(TestStruct));
	
	ret->data = strdup(s);
	
	return ret;
}

char* print_struct_value(TestStruct *s)
{
	return s->data;
}
