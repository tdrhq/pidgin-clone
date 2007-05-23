#ifndef LIBTEST_H_
#define LIBTEST_H_

typedef struct {
	int value;
} TestStruct;

char* say_hello();
void set_hello(char *str);
TestStruct* create_struct(int v);
void print_struct_value(TestStruct* s);

#endif
