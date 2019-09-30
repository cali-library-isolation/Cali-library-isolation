#ifndef LIBSHMMALLOC_SHM_MALLOC_H_H
#define LIBSHMMALLOC_SHM_MALLOC_H_H

#ifdef __cplusplus
extern "C"{
#endif


#include <stddef.h>
#include <stdint.h>

void shm_register_fork_handler(void);

void * shm_malloc (size_t bytes);
void * shm_malloc_2 (size_t bytes);

// int shm_malloc_info(int options, FILE *fp);

void   shm_free(void*);
void*  shm_calloc(size_t, size_t);
void*  shm_realloc(void*, size_t);
void*  shm_memalign(size_t, size_t);
void*  shm_valloc(size_t);
int    shm_mallopt(int, int);
void*  shm_pvalloc(size_t);



#ifdef __cplusplus
};
#endif


#define shm_shared_memory_base (    0x3f0000000000)
#define shm_shared_memory_mask (0xffffff0000000000)
#define shm_is_shared_memory_pointer(p) (((uintptr_t) (p) & shm_shared_memory_mask) == shm_shared_memory_base)

//#define shm_debug(...) fprintf(stderr, __VA_ARGS__)
#define shm_debug(...)

#endif //LIBSHMMALLOC_SHM_MALLOC_H_H
