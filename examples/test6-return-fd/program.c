#include "library.h"
#include <stdlib.h>

int main() {
	pid_printf("started");
	int fd = library_function(1);
	pid_printf("program fd = %d", fd);
	char buffer[12];
	buffer[0] = 'A';
	int result = read(fd, buffer, 12);
	if (result < 0) perror("read");
	pid_printf("(%d, %d) == (12, 0)", result, buffer[0]);
	pid_printf("finished.");

	pid_printf("Now list all my file descriptors:");
	char systembuffer[256];
	sprintf(systembuffer, "ls -l /proc/%d/fd", getpid());
	if (system(systembuffer)) perror("system");

	return 0;
}
