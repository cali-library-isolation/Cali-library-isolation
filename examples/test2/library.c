#include "library.h"
#include <signal.h>

static void pmap_interesting() {
	char buffer[256];
	sprintf(buffer, "pmap %d | grep 00003f00", getpid());
	int x = system(buffer);
	if (x != 0)
		pid_printf("system() result = %x", x);
}

static void async_modification(void* x) {
	signal(SIGCHLD, SIG_IGN);
	usleep(100000);
	pmap_interesting();
	int* y = x;
	*y = 3;
	pid_printf("modification written.");
	//usleep(1000000);
	pthread_exit();
}

int library_function(int* buffer) {
	pid_printf("buffer at %p", buffer);
	*buffer = 1;
	pmap_interesting();

	pthread_t thread;
	pthread_create(&thread, NULL, async_modification, buffer);
	pthread_detach(thread);

	pid_printf("After thread creation:");
	pmap_interesting();

	return 0;
}
