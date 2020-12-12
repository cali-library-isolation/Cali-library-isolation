#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define pid_printf(format, ...) fprintf(stderr, "[%d] " format "\n", getpid(), ##__VA_ARGS__)

int library_function(void);
