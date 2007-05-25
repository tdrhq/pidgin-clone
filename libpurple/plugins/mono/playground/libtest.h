#ifndef LIBTEST_H_
#define LIBTEST_H_

typedef struct {
	char *data;
} TestStruct;

char* say_hello();
void set_hello(char *str);
TestStruct* create_struct(char *s);
char* print_struct_value(TestStruct* s);

#endif
