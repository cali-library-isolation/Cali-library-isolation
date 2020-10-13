#ifndef LIBIPC_MALLOC_TESTER_H
#define LIBIPC_MALLOC_TESTER_H

#include <cstddef>
#include "../ipc_constants.h"

extern "C" {
void *shm_malloc_2(size_t size);

void *shm_calloc_2(size_t size1, size_t size2);

void free_shm_pointer_hook(void *ptr);

void shm_free_2(void *ptr);

void realloc_shm_pointer_hook(void *ptr_old, void *ptr_new, size_t size_new);
};

#endif //LIBIPC_MALLOC_TESTER_H
