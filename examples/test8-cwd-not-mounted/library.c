#include "library.h"

int library_function() {
	char* cwd = getcwd(NULL, 0);
	pid_printf("inner cwd = %s", cwd);
	free(cwd);
	return 0;
}
