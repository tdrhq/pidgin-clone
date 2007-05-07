#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <glib.h>

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
