#ifndef CALI_SHARED_MEMORY_LOCKING_H
#define CALI_SHARED_MEMORY_LOCKING_H

/*
 * We use a "status lock" on the shm file to determine when we are allowed to terminate.
 */

void statuslock_install();

void statuslock_install_forkhandler();

bool statuslock_is_sole_survivor();

#endif //CALI_SHARED_MEMORY_LOCKING_H
