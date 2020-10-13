#include "library.h"
#include <stdlib.h>

int local_handler(int* x) {
	return x[0]*x[1]*x[2];
}

__attribute__((noinline))
void do_it(int mode) {
	int(*funcptr)(int*) = 0;
	if (mode == 1) funcptr = local_handler;
	if (mode == 2) funcptr = library_function;

	int* memory = (int*) malloc(sizeof(int)*4);
	pid_printf("mode %d function %p argument %p", mode, funcptr, memory);
	memory[0] = 2;
	memory[1] = 3;
	memory[2] = 4;
	int result = funcptr(memory);
	pid_printf("result = %d", result);
}

int main() {
	pid_printf("started - should output 24 / 9");
	do_it(1);
	do_it(2);
	pid_printf("finished.");
	return 0;
}
