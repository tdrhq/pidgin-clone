
int pidgin_crash_minidump_print(const char *);

#include <stdio.h>

int main (int argc, char **argv) {

	printf("dumping a file!\n");

	if (argc <= 0) {
		printf("no crash file");
		return 1;
	}

	const char *minidump_file = argv[1];


	printf("file name is %s\n", minidump_file);

	return pidgin_crash_minidump_print(minidump_file);
}

