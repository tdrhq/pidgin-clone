#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <glib.h>

char* say_hello()
{
	return g_strdup("hello!");
}

