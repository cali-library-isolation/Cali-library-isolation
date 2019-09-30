#include <python2.7/Python.h>

#include "test_initializer.h"


void print_hello_using_python(const char* name){
	register_shared_memory_as_default();

	char* buffer = (char*) malloc(strlen(name) + 128);
	sprintf(buffer, "print \"Hello %s!\"\n", name);
	printf("Python code: %s\n", buffer);

	Py_Initialize();
	PyRun_SimpleString(buffer);
	Py_Finalize();
}



int main(int argc, char *argv[]) {
	init();

	if (argc <= 1){
		fprintf(stderr, "USAGE: %s <username>\n", argv[0]);
		return 1;
	}

	//shm_malloc(200000);

	// Py_SetProgramName(argv[0]);/* optional but recommended */
	puts("=== C ===");
	system("id && ls /");
	puts("");

	print_hello_using_python(argv[1]);

	puts("");
	puts("=== Back in C ===");
	system("id && ls /");

	return 0;
}

