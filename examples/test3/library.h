#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define pid_printf(format, ...) fprintf(stderr, "[%d] " format "\n", getpid(), ##__VA_ARGS__)

void prepare();
void trigger_sigill();
void trigger_sigusr2();
