#include <stddef.h>
#include <unistd.h>

extern "C" {
extern size_t _dl_pagesize;
}


#include "shm_malloc.h"

void shared_memory_ipccomm_init();
void semaphore_init();
void fork_event_loop();
void semaphore_free();


class StaticResources {
public:
	StaticResources() {
		_dl_pagesize = (size_t) getpagesize();
		shm_register_fork_handler();
		shared_memory_ipccomm_init();
		semaphore_init();
		fork_event_loop();
	}
	~StaticResources(){
		semaphore_free();
	}
};

StaticResources resources;
