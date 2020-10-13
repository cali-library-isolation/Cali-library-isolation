#include "library.h"

static void signal_handler_prog(int sig) {
	pid_printf("prog handled signal %d", sig);
}

int main() {
	pid_printf("started");

	prepare();
	pid_printf("handles SIGILL=%d and SIGUSR2=%d", SIGILL, SIGUSR2);
	signal(SIGILL, signal_handler_prog);
	signal(SIGUSR2, signal_handler_prog);

	pid_printf("Trigger signal SIGUSR2=%d", SIGUSR2);
	kill(getpid(), SIGUSR2);
	pid_printf("Trigger signal SIGUSR1=%d", SIGUSR1);
	kill(getpid(), SIGUSR1);
	trigger_sigusr2();
	trigger_sigill();
	alarm(1);
	usleep(1300000);
	pid_printf("survived an alarm");

	pid_printf("finished.");
	return 0;
}
