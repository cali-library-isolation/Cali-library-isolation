#ifndef CALI_LIB_INTERNAL_H
#define CALI_LIB_INTERNAL_H

void instrument_before_internal_remap_shm() {
	if (INTERNAL_VERBOSE) debug_printf("internal_remap_shm()");
}

void instrument_before_internal_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
	if (INTERNAL_VERBOSE) debug_printf("internal_mmap(%p, %ld, %d, flags=%d, fd=%d, offset=%ld)", addr, length, prot, flags, fd, offset);
}

void instrument_after_internal_mmap(void* result) {
	if (INTERNAL_VERBOSE) debug_printf(" => %p", result);
}

void instrument_before_internal_setSharedFDSize() {
	if (INTERNAL_VERBOSE) debug_printf("internal_setSharedFDSize()");
}

void instrument_before_internal_receive_fd(void* mapping, int fd, int pid) {
	if (INTERNAL_VERBOSE) debug_printf("internal_receive_fd(%p, fd=%d, pid=%d)", mapping, fd, pid);
}

void instrument_before_internal_close_fd(int fd, int pid) {
	if (INTERNAL_VERBOSE) debug_printf("internal_close_fd(%d, %d)", fd, pid);
}

void instrument_after_internal_close_fd() {
	if (INTERNAL_VERBOSE) debug_printf("closed.");
}

void instrument_before_internal_signal(int signal, void* handler) {
	if (INTERNAL_VERBOSE) debug_printf("internal_signal(%d, %p)", signal, handler);
}


#endif //CALI_LIB_INTERNAL_H
