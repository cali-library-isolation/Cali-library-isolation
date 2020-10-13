#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ipc_communication.h>
#include <ipc_constants.h>
#include <shm_malloc.h>
#include <cstring>
#include "../../libipc/ipc_communication.h"

typedef struct __ipc_data {
	unsigned int code;
	int libc_errno;
	void *callbacknumber;
	void *data;
} __ipc_data;

extern "C" {
#include <nsjail/api.h>

static void process_ipc_call_1(__ipc_data *receiving) asm ("process_ipc_call_1");
static void process_ipc_call_2(__ipc_data *receiving) asm ("process_ipc_call_2");

static void nsjail_configure(struct nsjconf_t *conf) asm ("nsjail_configure");
void *__unused_stuff_for_nsjail_configure();

__attribute__((constructor))
void getArguments(int argc, const char *argv[]) {
	nsjail_argc = argc;
	nsjail_argv = argv;
}

#ifndef NO_FORK_MODE
__attribute__ ((section (".data")))
static IpcCommunication communication(2048, 2048, (void (*)(void *)) &process_ipc_call_1, (void (*)(void *)) &process_ipc_call_2, nsjail_configure, true);
#else

static IpcCommunication communicationDefaultInstance(2048, 2048, (void (*)(void *)) &process_ipc_call_1, (void (*)(void *)) &process_ipc_call_2, true);
thread_local static IpcCommunication* communicationInstance = &communicationDefaultInstance;
extern "C" {
	void setCommunicationInstance(IpcCommunication *inst) {
		communicationInstance = inst;
	}
}
#define communication (*communicationInstance)

#endif


void trigger_ipc_call() asm ("trigger_ipc_call");
void trigger_ipc_call() {
	communication.trigger_ipc_call();
}

__ipc_data *ipc_get_sending_struct() asm ("ipc_get_sending_struct");
__ipc_data *ipc_get_sending_struct() {
	return (__ipc_data *) communication.sending_data;
}

__ipc_data *ipc_wait_for_return() asm ("ipc_wait_for_return");
__ipc_data *ipc_wait_for_return() {
	return (__ipc_data *) communication.wait_for_return();
}

int ipc_share_fd(int) asm ("ipc_share_fd");
int ipc_share_fd(int fd) {
	return communication.share_fd(fd);
}

__attribute__((always_inline))
void ipc_lock_sending(void) asm("ipc_lock_sending");
void ipc_lock_sending(void) {
	if (communication.sending_lock)
		communication.sending_lock_lock();
}




static void process_ipc_call_1(__ipc_data *receiving) {
	__ipc_data *sending = (__ipc_data *) communication.sending_data;
	switch (receiving->code) {
		case 2:
			sending->data = nullptr;
			break;
		case 3:
			break;
		default:
			fprintf(stderr, "[IPC ERROR] Invalid function (%d) in process %d\n", receiving->code, getpid());
	}
	sending->code = 1;
}

static void process_ipc_call_2(__ipc_data *receiving) {
	__ipc_data *sending = (__ipc_data *) communication.sending_data;
	switch (receiving->code) {
		case 2:
			sending->data = nullptr;
			break;
		case 3:
			break;
		default:
			fprintf(stderr, "[IPC ERROR] Invalid function (%d) in process %d\n", receiving->code, getpid());
	}
	sending->code = 1;
}

static void nsjail_configure(struct nsjconf_t *conf) {}
void *__unused_stuff_for_nsjail_configure() {
	// All nsjail API functions must be used once, so that prototypes will end up in the compiled .bc module.
	return new void *[5]{
			(void *) nsjail_addMountRO,
			(void *) nsjail_addMountRW,
			(void *) nsjail_setUID,
			(void *) nsjail_setGID,
			(void *) nsjail_setNetwork
	};
}


void *wrap_outgoing_callback(void *callback, void (*wrapper)(void *, void *, void *)) asm("wrap_outgoing_callback");
void *wrap_outgoing_callback(void *callback, void (*wrapper)(void *, void *, void *)) {
	return global_callback_manager.wrap_outgoing_callback(callback, wrapper);
}

void *wrap_incoming_callback(void *callback, void *replacement) asm("wrap_incoming_callback");
void *wrap_incoming_callback(void *callback, void *replacement) {
	return global_callback_manager.wrap_incoming_callback(callback, replacement);
}


// https://stackoverflow.com/questions/4958384/what-is-the-format-of-the-x86-64-va-list-structure
struct va_arguments {
	unsigned int gp_offset;
	unsigned int fp_offset;
	char *overflow_arg_area;
	char *reg_save_area;
};

void *wrap_outgoing_valist(void *list) asm("wrap_outgoing_valist");
void *wrap_outgoing_valist(void *list) {
	auto *arguments = (va_arguments *) list;
	auto register_size = (arguments->overflow_arg_area - arguments->reg_save_area); // all registers
	auto stack_size = 160;
	auto *result = shm_malloc_2(sizeof(va_arguments) + register_size + stack_size);
	auto result_arguments = (va_arguments *) result;
	result_arguments->gp_offset = arguments->gp_offset;
	result_arguments->fp_offset = arguments->fp_offset;
	result_arguments->reg_save_area = ((char *) result) + sizeof(va_arguments);
	result_arguments->overflow_arg_area = result_arguments->reg_save_area + register_size;
	memcpy(result_arguments->reg_save_area, arguments->reg_save_area, register_size + stack_size);
	return result;
}



//#define MAX_STACKVAR_SIZE 1536
#define MAX_STACKVAR_SIZE 4096
#define MAX_STACKVAR_OFFSET 8192u

void *wrap_pointer_from_callback(void *ptr) {
	char c;
	size_t diff = (char *) ptr - &c;
	if (diff < MAX_STACKVAR_OFFSET) {
		void *newptr = ((char *) shm_malloc(MAX_STACKVAR_SIZE + 256)) + 128;
		// fprintf(stderr, "malloc: %p\n", newptr);
		memset((char *) newptr - 128, 0, 128);
		memset((char *) newptr + MAX_STACKVAR_SIZE, 0, 128);
		memmove(newptr, ptr, MAX_STACKVAR_SIZE);
		return newptr;
	}
	return ptr;
}

static int iszero(void *memory, unsigned int size) {
	unsigned char *mm = (unsigned char *) memory;
	for (int i = 0; i < size; i++)
		if (mm[i] != 0) return 0;
	return 1;
}

void unwrap_pointer_from_callback(void *oldptr, void *newptr, int other_old, ...) {
	if (oldptr != newptr && shm_is_shared_memory_pointer(newptr)) {
		/*
		if (!iszero((char*)newptr - 128, 128)) {
			fprintf(stderr, "modification in [-128,0[ detected %p\n", newptr);
		}
		if (!iszero((char*)newptr + MAX_STACKVAR_SIZE, 128)) {
			fprintf(stderr, "modification in [1024,+128[ detected %p\n", newptr);
		}
		 */
		size_t size = MAX_STACKVAR_SIZE;
		// fprintf(stderr, "other_old=%d\n", other_old);
		if (other_old > 0) {
			va_list valist;
			va_start(valist, other_old);
			for (int i = 0; i < other_old; i++) {
				char *ptr = va_arg(valist, char * );
				size_t diff = (char *) ptr - (char *) &size;
				// fprintf(stderr, "ptr=%p diff=%lu ptr-oldptr=%ld\n", ptr, diff, ptr-(char*)oldptr);
				if (diff < MAX_STACKVAR_OFFSET && ptr > oldptr) { // stack pointer & above the current one
					if (ptr - (char *) oldptr < size) size = ptr - (char *) oldptr;
					// fprintf(stderr, "size=%lu\n", size);
				}
				// fflush(stderr);
			}
			va_end(valist);
		}

		//fprintf(stderr, "memmove(%p => %p, %d)\n", newptr, oldptr, 1024);
		unsigned char *o = (unsigned char *) oldptr;
		unsigned char *n = (unsigned char *) newptr;
		// fprintf(stderr, "[%d] 1 memmove(%p => %p, %ld)  (%02x, %02x, %02x, %02x) => (%02x, %02x, %02x, %02x)\n", getpid(), newptr, oldptr, size, n[0], n[1], n[2], n[3], o[0], o[1], o[2], o[3]);
		memmove(oldptr, newptr, size);
		// fprintf(stderr, "[%d] 2 memmove(%p => %p, %ld)  (%02x, %02x, %02x, %02x) => (%02x, %02x, %02x, %02x)\n", getpid(), newptr, oldptr, size, n[0], n[1], n[2], n[3], o[0], o[1], o[2], o[3]);
		// fprintf(stderr, "restore: old=%p new=%p size=%ld\n", oldptr, newptr, size);
		shm_free(((char *) newptr) - 128);
	}
}



void debug_i64(uint64_t) asm("debug_i64");
void debug_i64(uint64_t x) {
	fprintf(stderr, "[debug] i64: %ld  (= 0x%lx)\n", x, x);
}

}
