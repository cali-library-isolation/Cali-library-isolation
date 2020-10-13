#include "library.h"

static void signal_handler(int sig) {
	pid_printf("handled signal %d", sig);
}

void prepare() {
	pid_printf("handles SIGUSR1=%d and SIGALRM=%d", SIGUSR1, SIGALRM);
	signal(SIGUSR1, signal_handler);
	signal(SIGALRM, SIG_IGN);
}

void trigger_sigill() {
	pid_printf("Trigger signal %d", SIGILL);
	// asm("ud2");
	kill(getpid(), SIGILL);
}

void trigger_sigusr2() {
	pid_printf("Trigger signal %d", SIGUSR2);
	kill(getpid(), SIGUSR2);
}
