#include "library.h"

int library_function(void* things) {
	unsigned int* x = (unsigned int*) things;
	pid_printf("%p: %08x %08x", x+0, x[0], x[1]);
	pid_printf("%p: %08x %08x", x+2, x[2], x[3]);
	pid_printf("%p: %08x %08x", x+4, x[4], x[5]);
	pid_printf("%p: %08x %08x", x+6, x[6], x[7]);
	return 0;
}
