#include "library.h"

void exec_program(const char* prog) {
	if (fork() == 0) {
		const char* argv[2] = {prog, 0};
		execv(prog, argv);
		fprintf(stderr, "Could not run: \"%s\".\n", prog);
		exit(2);
	}
	usleep(200000);
}

int main() {
	pid_printf("started");
	library_function(1);

	exec_program("/usr/bin/id");

	library_function(2);

	exec_program("/bin/does_not_exist");

	library_function(3);
	pid_printf("finished.");
	return 0;
}
