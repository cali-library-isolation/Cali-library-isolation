#include <stdlib.h>

#include "test_initializer.h"


#pragma GCC diagnostic ignored "-Wunused-result"

int main(int argc, char* argv[]){
	init();
	printf("Pagesize = %zu\n", _dl_pagesize);
	shm_malloc(132);
	register_shared_memory_as_default();

	puts("Hello world");

	pid_t pid = fork();
	if (pid < 0) perror("fork()");
	if (pid > 0){
		//parent
		sleep(1);
		return 0;
	}else {
		usleep(100*1000);
		for (int i = 0; i < 20; i++) {
			malloc(80);
			malloc(32);
			//realloc(0, 80);
			//realloc(0, 12);
			malloc(2048);
		}

		return 0;
	}
}