#include "shared_memory_locking.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "shared_memory_manger.h"


void statuslock_install() {
	struct flock lock{
			.l_type = F_RDLCK,
			.l_whence = SEEK_SET,
			.l_start  = 0,
			.l_len    = 1,
			.l_pid    = getpid(),
	};
	if (fcntl(shared_memory_fd, F_SETLK, &lock))
		perror("fcntl in statuslock_install");
}

void statuslock_install_forkhandler() {
	pthread_atfork(nullptr, []() {
		// parent
		usleep(10000); // 10 ms quirky workaround
	}, []() {
		// child
		statuslock_install();
	});
}

bool statuslock_is_sole_survivor() {
	struct flock lock{
			.l_type = F_WRLCK,
			.l_whence = SEEK_SET,
			.l_start  = 0,
			.l_len    = 1,
			.l_pid    = getpid(),
	};
	if (fcntl(shared_memory_fd, F_GETLK, &lock))
		perror("fcntl in statuslock_is_sole_survivor");
	return lock.l_type == F_UNLCK;
}
