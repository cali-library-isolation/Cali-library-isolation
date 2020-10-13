#include "library.h"

int main() {
	pid_printf("started");
	library_function(1);
	pid_printf("finished.");
	return 0;
}
