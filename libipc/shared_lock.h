#ifndef LIBIPC_SHARED_LOCK_H
#define LIBIPC_SHARED_LOCK_H

#include <sys/prctl.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <cstdio>


static struct sembuf sema_inc = {0, 1, 0};
static struct sembuf sema_dec = {0, -1, 0};

class SharedMutex {
	key_t semaphore;

public:
	SharedMutex() : semaphore(semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | IPC_RMID | 0600)) {
		unlock();
	}

	~SharedMutex() {
		semctl(semaphore, 0, IPC_RMID);
	}

	void lock() {
		if (semop(semaphore, &sema_dec, 1) < 0)
			perror("SharedMutex::lock() semaphore");
	}

	void unlock() {
		if (semop(semaphore, &sema_inc, 1) < 0)
			perror("SharedMutex::lock() semaphore");
	}
};


class SharedLockGuard {
	SharedMutex &mtx;
public:
	explicit SharedLockGuard(SharedMutex &mtx) : mtx(mtx) {
		mtx.lock();
	}

	~SharedLockGuard() {
		mtx.unlock();
	}
};


#endif //LIBIPC_SHARED_LOCK_H
