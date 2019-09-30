#include <unistd.h>
#include "ipc_constants.h"

#ifdef NO_FORK_MODE
struct IpcCommunication;
void setCommunicationInstance(struct IpcCommunication *inst) {}
#endif

void *__ipc_replacement_internal_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset){
	return 0;
}

void *__ipc_replacement_internal_mremap(void *addr, size_t old_length, size_t length, int flags){
	return 0;
}

void __ipc_replacement_internal_setSharedFDSize(){
}