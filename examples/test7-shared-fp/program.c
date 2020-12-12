#include "library.h"
#include <stdlib.h>

struct X {
	int a, b, c, d;
	void (*callback)(int, int);
	void* data;
};

void cb(int c, int d) {
	pid_printf("callback(%d, %d)", c, d);
}


void* get_memory() {
	struct X* x = malloc(sizeof(struct X));
	x->a = 1;
	x->b = 2;
	x->c = 3;
	x->d = 4;
	x->callback = cb;
	x->data = 0;
	return x;
}

int main() {
	pid_printf("started");
	library_function(get_memory());
	pid_printf("finished.");
	return 0;
}
