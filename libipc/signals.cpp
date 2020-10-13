#include "signals.h"
#include <unistd.h>
#include "shared_memory_manger.h"
#include "ipc_communication.h"

#ifdef __cplusplus
extern "C" {
#endif

sighandler_t internal_signal(int signum, sighandler_t handler);
sighandler_t __ipc_replacement_internal_signal(int signum, sighandler_t handler);

#ifdef __cplusplus
}
#endif




// sighandler_t signal(int signum, sighandler_t handler);
real_function(signal, sighandler_t(int, sighandler_t));

sighandler_t signal(int signum, sighandler_t handler) {
	auto result = real(signal)(signum, handler);
	if (shared_memory_has_forked()) {
		if (IpcCommunication::last_instance->sending_is_blocked) {
			fprintf(stderr, "[WARN] signal used while sending is blocked!\n");
		} else {
			__ipc_replacement_internal_signal(signum, handler);
		}
	}
	return result;
}

sighandler_t internal_signal(int signum, sighandler_t handler) {
	// printf("[%d] Set signal handler %p for signal %d\n", getpid(), handler, signum);
	return real(signal)(signum, handler);
}

