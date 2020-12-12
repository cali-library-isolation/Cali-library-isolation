#include "library.h"
#include <stdlib.h>

int main() {
	pid_printf("started");
	
	char* cwd = getcwd(NULL, 0);
	pid_printf("outer cwd = %s", cwd);
	free(cwd);

	library_function();
	pid_printf("finished.");
	return 0;
}
