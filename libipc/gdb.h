#ifndef CALI_GDB_H
#define CALI_GDB_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

extern char* program_invocation_name;

static void attach_gdb(pid_t pid, int cont) {
	fprintf(stderr, "[%d] Attaching gdb to %d...\n", getpid(), pid);
	char buffer[1024];
	if (cont) {
		sprintf(buffer, "gnome-terminal -- sudo gdb %s %d -ex 'set confirm on' -ex continue", program_invocation_name, pid);
	} else {
		sprintf(buffer, "gnome-terminal -- sudo gdb ./%s %d", program_invocation_name, pid);
	}
	fprintf(stderr, "[%d] > %s\n", getpid(), buffer);
	if (system(buffer))
		perror("system gdb");
}

static void gdb_me(int cont) {
	attach_gdb(getpid(), cont);
	sleep(2);
}


static inline void pmap_me_shm() {
	char buffer[128];
	sprintf(buffer, "pmap %d | grep 003f >&2", getpid());
	fprintf(stderr, ">>> %s\n", buffer);
	if (system(buffer)){}
	fprintf(stderr, "<<< %s\n", buffer);
}

static inline void pmap_me() {
	char buffer[128];
	sprintf(buffer, "pmap %d >&2", getpid());
	fprintf(stderr, ">>> %s\n", buffer);
	if (system(buffer)){}
	fprintf(stderr, "<<< %s\n", buffer);
}

static inline void sigtrap() {
	raise(SIGTRAP);
}

#endif //CALI_GDB_H
