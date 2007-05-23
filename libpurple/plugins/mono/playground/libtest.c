#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <glib.h>

#include "libtest.h"

char* say_hello()
{
	return "Hello";
}

void set_hello(char* str)
{
	printf("%s\n", str);
}

int length(char* s)
{
	return strlen(s);
}

TestStruct* create_struct(int v)
{
	TestStruct* ret = (TestStruct*)malloc(sizeof(TestStruct));
	
	ret->value = v;
	
	return ret;
}

void print_struct_value(TestStruct *s)
{
	printf("%d\n", s->value);
}
