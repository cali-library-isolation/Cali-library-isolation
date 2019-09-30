#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <ipc_communication.h>
#include <ipc_constants.h>
#include <shm_malloc.h>
#include <cstring>

typedef struct __ipc_data {
	unsigned int code;
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
void getArguments(int argc, const char* argv[]) {
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




static void process_ipc_call_1(__ipc_data *receiving) {
	__ipc_data *sending = (__ipc_data *) communication.sending_data;
	switch (receiving->code) {
		case 2:
			sending->data = nullptr;
			break;
		case 3:
			break;
		default:
			fprintf(stderr, "[IPC ERROR] Invalid function (%d)\n", receiving->code);
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
			fprintf(stderr, "[IPC ERROR] Invalid function (%d)\n", receiving->code);
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



void *wrap_pointer_from_callback(void *ptr) {
	char c;
	size_t diff = (char *) ptr - &c;
	if (diff < 4096u) {
		void *ptr2 = shm_malloc(1024);
		memcpy(ptr2, ptr, 1024);
		return ptr2;
	}
	return ptr;
}



void debug_i64(uint64_t) asm("debug_i64");
void debug_i64(uint64_t x) {
	fprintf(stderr, "[debug] i64: %ld  (= 0x%lx)\n", x, x);
}

}
