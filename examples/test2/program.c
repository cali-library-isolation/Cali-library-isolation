#include "library.h"

int main() {
	volatile int buffer;

	buffer = 0;
	pid_printf("buffer = %d", buffer);
	library_function((int*) &buffer);
	pid_printf("buffer = %d", buffer);
	buffer = 2;
	pid_printf("buffer = %d", buffer);
	usleep(500000);
	pid_printf("buffer = %d", buffer);
	pid_printf("finished.");
	return 0;
}
