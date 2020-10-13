#include "library.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int library_function(int code) {
	int fd = open("/dev/zero", O_RDONLY);
	pid_printf("library fd %d", fd);
	return fd;
}
