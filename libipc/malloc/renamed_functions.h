#ifndef LIBSHMMALLOC_RENAMED_FUNCTIONS_H
#define LIBSHMMALLOC_RENAMED_FUNCTIONS_H

#define __mmap shm_mmap
#define __mremap shm_mremap
#define __munmap shm_munmap
#define __mprotect shm_mprotect
#define __get_nprocs get_nprocs

#include "shared_memory_manger.h"

#define NONDEFAULT_NAMESPACE


#endif //LIBSHMMALLOC_RENAMED_FUNCTIONS_H