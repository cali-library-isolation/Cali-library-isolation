#ifndef LIBSHMMALLOC_SHARED_MEMORY_MANGER_H
#define LIBSHMMALLOC_SHARED_MEMORY_MANGER_H

#include <stdlib.h>
#include <stdarg.h>
#include "ipc_constants.h"

#define SHARED_MEMORY_FD 1022

#include "shm_malloc.h"

#ifdef __cplusplus
extern "C" {
struct FileDescriptorMapping;
#else
// Forward declaration in C
typedef struct FileDescriptorMapping FileDescriptorMapping;
#endif


void *shm_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
void *shm_mmap64(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
void *shm_mmap_share(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
void *shm_mremap(void *old_address, size_t old_size, size_t new_size, int flags);
//void *shm_mremap(void *old_address, size_t old_size, size_t new_size, int flags, void *new_address);
int shm_munmap(void *addr, size_t length);
int shm_mprotect(void *addr, size_t len, int prot);



void free(void *ptr);
void *realloc(void *ptr, size_t size);
void *realloc_to_shm(void *ptr, size_t size);
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
void *mmap64(void *addr, size_t len, int prot, int flags, int fildes, off64_t off);
void *mremap(void *old_address, size_t old_size, size_t new_size, int flags, ...);
int mlock(const void *addr, size_t len);
int mlock2(const void *addr, size_t len, unsigned int flags) __THROW;
int munlock(const void *addr, size_t len);

void *internal_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
// void *internal_mremap(void *addr, size_t old_length, size_t length, int flags);
void internal_remap_shm(void);
void remap_shared_memory(void);
void internal_setSharedFDSize(void);
void *__ipc_replacement_internal_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
// void *__ipc_replacement_internal_mremap(void *addr, size_t old_length, size_t length, int flags);
void __ipc_replacement_internal_remap_shm(void);
void __ipc_replacement_internal_setSharedFDSize(void);

void register_shared_memory_as_default(void);
void unregister_shared_memory_as_default(void);
void reregister_shared_memory_as_default(void);
void shared_memory_init(FileDescriptorMapping* fileDescriptors);
void shared_memory_terminate(void);
void shared_memory_forking(void);
void shared_memory_forking_complete(void);
void shared_memory_unfork(void);
void __attribute__((section("text_mprotect"))) shared_memory_set_rw(void);
void shared_memory_set_ro(void);
int shared_memory_has_forked(void);
void shm_msync(void);

void shm_print_debug_info(void);

int shm_vasprintf(char **strp, const char *fmt, va_list ap);
int shm_asprintf(char **strp, const char *fmt, ...);
char* shm_getenv(const char* name);
char *shm_strdup(const char *s);
void shm_free_wrapper(void* ptr, void* originalFree);

extern size_t _dl_pagesize;


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include "hook.hpp"

real_function_extern(free, void(void * ));
real_function_extern(realloc, void* (
		void*, size_t));
real_function_extern(mmap, void* (
		void *addr, size_t
		length, int
		prot, int
		flags, int
		fd, off_t
		offset));
real_function_extern(mmap64, void* (
		void *addr, size_t
		length, int
		prot, int
		flags, int
		fd, off64_t
		offset));
real_function_extern(mremap, void* (
		void*, size_t, size_t, int));

extern int shared_memory_fd;

#include <mutex>
extern std::mutex memory_mutex;

#endif



//#define shmman_debug(...) fprintf(stderr, __VA_ARGS__)
#define shmman_debug(...)

#endif //LIBSHMMALLOC_SHARED_MEMORY_MANGER_H
