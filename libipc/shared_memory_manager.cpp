#include <time.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <cstdio>
#include <unistd.h>

#include <malloc.h>
#include <bitset>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <mutex>
#include <syscall.h>
#include <csignal>

#include "shared_memory_manger.h"
#include "byte_bitset.h"
#include "malloctest/malloc_tester.h"
#include "shared_lock.h"
#include "libipc/annotations.h"
#include "debug.h"
#include "file_descriptor_manager.h"
#include "program_params.h"
#include "shared_memory_locking.h"
#include "gdb.h"

extern "C" {
static void *shm_malloc_hook(size_t size, const void *caller);
static void *shm_realloc_hook(void *ptr, size_t size, const void *caller);
static void shm_free_hook(void *ptr, const void *caller);
static void *shm_memalign_hook(size_t alignment, size_t size, const void *caller);
}

std::mutex memory_mutex;

#define void_ptr_add(p, x) ((void*) (((char*) p) + x))
#define void_ptr_sub(p, x) ((void*) (((char*) p) - x))
#define ALIGN_DOWN(base, size)    ((base) & -((__typeof__ (base)) (size)))
#define ALIGN_UP(base, size)    ALIGN_DOWN ((base) + (size) - 1, (size))


real_function(free, void(void * ));
real_function(realloc, void* (
		void*, size_t));
real_function(mmap, void* (
		void *, size_t, int, int, int, off_t ));
real_function(mmap64, void* (
		void *, size_t, int, int, int, off64_t ));
real_function(mremap, void* (
		void*, size_t, size_t, int));

static bool use_shared_memory_only = false;

static void shared_memory_remapped_to_size(void* addr, size_t len);

void free(void *ptr) {
	if (shm_is_shared_memory_pointer(ptr)) {
		std::lock_guard<std::mutex> guard(memory_mutex);
		unregister_shared_memory_as_default();
		// shm_debug("shm_free(%p);\n", ptr);
#ifdef MALLOC_TESTER
		free_shm_pointer_hook(ptr);
#endif
		shm_free(ptr);
		reregister_shared_memory_as_default();
	} else {
		if (real(free)) real(free)(ptr);
	}
}

static inline pid_t gettid() {
	pid_t tid = syscall(SYS_gettid);
	return tid;
}


void *realloc(void *ptr, size_t size) {
	if ((!ptr && use_shared_memory_only) || shm_is_shared_memory_pointer(ptr)) {
		std::lock_guard<std::mutex> guard(memory_mutex);
		unregister_shared_memory_as_default();
		// shm_debug("shm_realloc(%p, %ld);", ptr, size);
		void *new_ptr = shm_realloc(ptr, size);
		// shm_debug("  // => %p\n", new_ptr);
#ifdef MALLOC_TESTER
		realloc_shm_pointer_hook(ptr, new_ptr, size);
#endif
		reregister_shared_memory_as_default();
		return new_ptr;
	} else if (ptr == nullptr) {
		return malloc(size);
	} else if (!real(realloc)) {
		// realloc not ready(!)
		uintptr_t current_size = ((uintptr_t *) ptr)[-1] & -8u;
		if (current_size > size)
			return ptr;
		// hack - give out new memory, leave the old one dangling
		auto ptr2 = malloc(size);
		memcpy(ptr2, ptr, current_size);
		free(ptr);
		return ptr2;
	} else {
		return real(realloc)(ptr, size);
	}
}

void *realloc_to_shm(void *ptr, size_t size) {
	if (!ptr || shm_is_shared_memory_pointer(ptr)) {
#ifdef MALLOC_TESTER
		unregister_shared_memory_as_default();
		void* new_ptr = shm_realloc(ptr, size);
		realloc_shm_pointer_hook(ptr, new_ptr, size);
		reregister_shared_memory_as_default();
		return new_ptr;
#else
		return shm_realloc(ptr, size);
#endif
	}
	auto result = shm_malloc_2(size);
	memmove(result, ptr, size);
	return result;
}


/*
int posix_memalign(void **memptr, size_t alignment, size_t size) {
	std::lock_guard guard(mem_mutex);
	fprintf(stderr, "[%d] posix_memalign(%ld, %ld)", gettid(), size, alignment);
	*memptr = malloc(size + alignment*2);
	fprintf(stderr, " => %p\n", *memptr);
	return 0;
	/*int result = real(posix_memalign)(memptr, alignment, size);
	fprintf(stderr, "[%d] posix_memalign(%ld, %ld) => %d + %p\n", gettid(), size, alignment, result, *memptr);
	return result;*/
// }

void *ipc_free(void *ptr) {
	if (shm_is_shared_memory_pointer(ptr)) {
		free(ptr);
		return 0;
	}
	return ptr;
}

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
	if (use_shared_memory_only) {
		void *result = shm_mmap(addr, length, prot, flags, fd, offset);
		shm_debug("Caught mmap replacement: mmap(%p, %lu, %d, %d, %d, %ld); // %p [%d]\n", addr, length, prot, flags, fd, offset, result, gettid());
		return result;
	}
	void *result = real(mmap)(addr, length, prot, flags, fd, offset);
	//fprintf(stderr, "[CALL] mmap(%p, %zu, %d, %d, %d, %zd) => %p\n", addr, length, prot, flags, fd, offset, result);
	return result;
}

void *mmap64(void *addr, size_t length, int prot, int flags, int fd, off64_t offset) {
	if (use_shared_memory_only) {
		void *result = shm_mmap(addr, length, prot, flags, fd, offset);
		shm_debug("Caught mmap replacement: mmap(%p, %lu, %d, %d, %d, %ld); // %p [%d]\n", addr, length, prot, flags, fd, offset, result, gettid());
		return result;
	}
	void *result = real(mmap64)(addr, length, prot, flags, fd, offset);
	//fprintf(stderr, "[CALL] mmap64(%p, %zu, %d, %d, %d, %zd) => %p\n", addr, length, prot, flags, fd, offset, result);
	return result;
}

void *mremap(void *old_address, size_t old_size, size_t new_size, int flags, ...) {
	if (shm_is_shared_memory_pointer(old_address))
		return shm_mremap(old_address, old_size, new_size, flags);
	return real(mremap)(old_address, old_size, new_size, flags);
}

int mlock(const void *addr, size_t len) {
	//TODO real function for non-shared memory?
	return 0;
}

int mlock2(const void *addr, size_t len, unsigned int flags) __THROW {
	//TODO real function for non-shared memory?
	return 0;
}

int munlock(const void *addr, size_t len) {
	//TODO real function for non-shared memory?
	return 0;
}


static void *shm_malloc_hook(size_t size, const void *) {
	std::lock_guard<std::mutex> guard(memory_mutex);
	//printf("[SHM] Hook malloc(%zu)\n", size);
	//void *result = shm_malloc(size);
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	__malloc_hook = nullptr; //TODO hack and not multithread-safe
	void *result = shm_malloc_2(size); //TODO
	__malloc_hook = nullptr; //TODO hack and not multithread-safe
	// shm_debug("shm_malloc(%zu); // %p\n", size, result);
	__malloc_hook = shm_malloc_hook;
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
	return result;
}

/*
static void *shm_realloc_hook(void *ptr, size_t size, const void *) {
	if (!shm_is_shared_memory_pointer(ptr) && ptr != 0) {
		shm_debug("[ERR] Invalid realloc() ptr: %p\n", ptr);
	}
	return shm_realloc(ptr, size);
}

static void shm_free_hook(void *ptr, const void *) {
	shm_debug("shm_free(%p); \n", ptr);
	if (!shm_is_shared_memory_pointer(ptr) && ptr != 0) {
		fprintf(stderr, "[ERR] Invalid free() ptr: %p\n", ptr);
	}
	shm_free(ptr);
}
 */

static void *shm_memalign_hook(size_t alignment, size_t size, const void *) {
	shm_debug("[SHM] memalign hook (%zu, %zu)\n", alignment, size);
	return shm_memalign(alignment, size);
}


void register_shared_memory_as_default() {
	use_shared_memory_only = true;
	debug_printf("Malloc hook installed");
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
	__malloc_hook = shm_malloc_hook;
	//__realloc_hook = shm_realloc_hook;
	//__free_hook = shm_free_hook;
	__memalign_hook = shm_memalign_hook;
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
	shm_debug("%s()\n", __func__);
}

void unregister_shared_memory_as_default() {
	if (use_shared_memory_only) {
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
		__malloc_hook = nullptr;
		//__realloc_hook = nullptr;
		//__free_hook = nullptr;
		__memalign_hook = nullptr;
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
	}
}

void reregister_shared_memory_as_default() {
	if (use_shared_memory_only) {
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
		__malloc_hook = shm_malloc_hook;
		//__realloc_hook = shm_realloc_hook;
		//__free_hook = shm_free_hook;
		__memalign_hook = shm_memalign_hook;
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
	}
}

inline int inline_mprotect(void *ptr, size_t size, unsigned long prot) {
	int result;
	asm volatile ("syscall" : "=a"(result) : "a"(SYS_mprotect), "D"(ptr), "S"(size), "d"(prot));
	return result;
}




// 64 different characters - 6 bit randomness per char
static const char *random_characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_-";

static int create_new_shared_memory_segment(size_t initial_size) {
	srand((unsigned) time(nullptr) - 1337 * getpid());
	char random[37]; // 32 * 6 = 192bit randomness
	for (int i = 4; i < 36; i++) {
		random[i] = random_characters[rand() & 0x3f];
	}
	random[0] = 'i';
	random[1] = 'p';
	random[2] = 'c';
	random[3] = '_';
	random[36] = 0;
	int fd = shm_open(random, O_RDWR | O_CREAT | O_EXCL, 0666);
	if (fd < 0) perror("shm_open() failed");
	if (shm_unlink(random) < 0) perror("shm_unlink() failed");
	if (ftruncate(fd, initial_size))
		perror("ftruncate");
	shmman_debug("[SHM] Created segment \"%s\", open as %d\n", random, fd);
	// return fd;
	// Change the file descriptor to a higher number (to avoid collisions with seccomp filter)
	if (dup3(fd, SHARED_MEMORY_FD, O_CLOEXEC) < 0) {
		perror("dup3");
		return fd;
	}
	close(fd);
	return SHARED_MEMORY_FD;
}


static bool shared_memory_has_forked_complete;


struct SharedMemoryPrivateInformation {
	size_t current_mapped_size = 0;
};


class SharedMemoryInformation {
public:
	void *base = (void *) shm_shared_memory_base;
	bool has_forked = false;
	bool has_unintenden_fork = false; // true if the program (not Cali) does a fork - i.e. do not free shared stuff
	size_t shared_size = 0;
	FileDescriptorMapping *fileDescriptors = nullptr;
	SharedMemoryPrivateInformation *unshared_infos = new SharedMemoryPrivateInformation();
private:
	int fd = -1;

	//std::bitset<8> *pages_used = nullptr;
	ByteBitset *pages_used = nullptr;
	size_t bitset_size = 0; // bytes in pages_used
	size_t first_unused_page_index = 0; // page index of the first page that has never been used / mapped (e.g. is zero'd).

	//std::mutex mtx;
	SharedMutex mtx;

	uint64_t mmap_next_free_address = 0x3e0000000000;

	char shared_memory_for_malloc[2400];  // should be the last slot, allocate memory for malloc (main_arena etc). current impl requires 2232 bytes

public:
	~SharedMemoryInformation() {
		mtx.has_forked = has_unintenden_fork;
	}

	void init(int fd, size_t size, void *pages_initial_memory, size_t bitset_size, int used_pages) {
		this->shared_size = this->unshared_infos->current_mapped_size = size;
		this->fd = fd;
		this->base = this;
		this->has_forked = false;
		this->pages_used = (ByteBitset *) pages_initial_memory;
		this->bitset_size = bitset_size;
		// memset(this->pages_used, 0, this->bitset_size); // memory is already 0-initialized (either empty file or fresh mmap call)
		for (int i = 0; i < used_pages; i++) {
			pages_used[i / 8].set(i % 8, true);
		}
		first_unused_page_index = used_pages;
		init_shm_malloc(&this->shared_memory_for_malloc);
		// fprintf(stderr, "Maximal memory page: %ld (%p)\n", bitset_size * 8L, pageIndexToAddress(bitset_size*8L));
	}

	void terminate() {
		if (program_params.is_forking ? statuslock_is_sole_survivor() : !has_unintenden_fork) {
			mtx.terminate();
		}
	}

	inline void *pageIndexToAddress(long page) {
		return (char *) base + page * _dl_pagesize;
	}

	inline size_t addressToPageIndex(void *address) {
		return (((char *) address) - ((char *) base)) / _dl_pagesize;
	}

	/**
	 * Find and block <count> pages. If there is not enough shared memory, it will be enlarged.
	 * @param count
	 * @return page index to start with
	 */
	long requestContinousFreePages(size_t count) {
		SharedLockGuard guard(mtx);
		remap_shared_memory();
		return requestContinousFreePagesInternal(count);
	}

private:
	long findContinousFreePages(size_t count) {
		long i = 1;
		long c = 0;
		long last_start = 0;
		while (i / 8 < bitset_size) {
			if (pages_used[i / 8].all()) {
				i += 8;
				c = 0;
			} else if (pages_used[i / 8].none()) {
				if (c == 0)
					last_start = i;
				i += 8;
				c += 8;
				if (c >= count)
					return last_start;
			} else {
				for (int j = 0; j < 8; j++) {
					if (pages_used[i / 8][j]) {
						c = 0;
					} else {
						if (c == 0)
							last_start = i + j;
						c += 1;
						if (c >= count)
							return last_start;
					}
				}
				i += 8;
			}
		}

		std::cout << bitset_size * 8L * _dl_pagesize / 1024 / 1024 << " MB in " << bitset_size * 8 << " pages possible" << std::endl;
		long freepages = 0;
		for (long j = 0; j < bitset_size; j++) {
			freepages += 8 - pages_used[j].count();
			if (freepages < 0)
				std::cout << j << " " << pages_used[j].count() << " / " << pages_used[j].size() << " = " << pages_used[j].data << " " << freepages << std::endl;
		}
		std::cout << freepages * _dl_pagesize / 1024L / 1024L << " MB in " << freepages << " pages used" << std::endl;

		throw std::runtime_error("Too much memory used! (NOT IMPLEMENTED)");
	}

	long requestContinousFreePagesInternal(size_t count) {
		long pos = findContinousFreePages(count);

		if (shared_size < (pos + count) * _dl_pagesize) {
			resizeSharedMemory(pos + count);
		}
		if (shared_size > this->unshared_infos->current_mapped_size)
			remapSharedMemory(this->unshared_infos->current_mapped_size, false);

		// Only re-initialize if that page had been used before
		if (pos < first_unused_page_index)
			bzero(pageIndexToAddress(pos), count * _dl_pagesize);

		// Block pages
		for (long i = pos; i < pos + count; i++) {
			pages_used[i / 8].set(i % 8, true);
		}
		first_unused_page_index = std::max(first_unused_page_index, pos + count);

		return pos;
	}


public:
	long requestContinousFreePagesForMove(size_t count, size_t preferred_pos) {
		SharedLockGuard guard(mtx);
		//TODO
		return requestContinousFreePagesInternal(count);

		for (long i = preferred_pos; i < preferred_pos + count; i++) {
			if (pages_used[i / 8][i % 8]) {
				return requestContinousFreePagesInternal(count);
			}
		}
		// Block pages
		for (long i = preferred_pos; i < preferred_pos + count; i++) {
			pages_used[i / 8].set(i % 8, true);
		}
		return preferred_pos;
	}

	void freePages(size_t pos, size_t count) {
		SharedLockGuard guard(mtx);
		for (long i = pos; i < pos + count; i++) {
			pages_used[i / 8].set(i % 8, false);
		}
	}

	/**
	 * Resize the shared memory segment
	 * @param pages
	 */
	void resizeSharedMemory(size_t pages) {
		if (pages * _dl_pagesize == shared_size)
			return;
		shared_size = _dl_pagesize * pages;
		//TODO ugly hack
		//if (use_shared_memory_only) {
		//	__ipc_replacement_internal_setSharedFDSize();
		//} else {
		setFDsize();
		//}
		remapSharedMemory(unshared_infos->current_mapped_size);
		first_unused_page_index = std::min(first_unused_page_index, pages);
	}

	void setFDsize() {
#ifndef NO_SHM_MODE
		if (ftruncate(fd, shared_size) != 0) {
			perror("ftruncate() failed");
			fprintf(stderr, "fd=%d size=%ld pages=%ld\n", fd, shared_size, shared_size / _dl_pagesize);
			throw std::runtime_error("ftruncate failed");
		}
		debug_printf("%d  ftruncate(%d, %ld)", use_shared_memory_only, fd, shared_size);
#endif
	}

	void remapSharedMemory(size_t old_shared_size, bool may_propagate=true) {
		// fprintf(stderr, "[%d] remap %p: 0x%lx => 0x%lx  (has_forked=%d)\n", getpid(), base, old_shared_size, shared_size, has_forked);
#ifndef NO_SHM_MODE
		void *addr = real(mremap)(base, old_shared_size, shared_size, MREMAP_MAYMOVE);
		if (addr == MAP_FAILED) {
			unregister_shared_memory_as_default();
			perror("mremap on fd");
			throw std::runtime_error("Could not remap shared memory!");
		}
		if (addr != base) {
			if (write(2, "addr != base\n", 13)){}
			unregister_shared_memory_as_default();
			fprintf(stderr, "Error: mremap moved shared memory!\n");
			throw std::runtime_error("Remap failed (memory moved)");
		}
#else
		void *addr = real(mmap)(((char *) base) + old_shared_size, shared_size - old_shared_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (addr == MAP_FAILED) {
			perror("mremap on fd");
			throw std::runtime_error("Could not remap shared memory!");
		}
#endif
		this->unshared_infos->current_mapped_size = this->shared_size;
		if (has_forked && may_propagate) {
#ifndef NO_FORK_MODE
			// fprintf(stderr, "trigger __ipc_replacement_internal_mremap(%p) from %d\n", base, getpid());
			// __ipc_replacement_internal_remap_shm(); //TODO release lock
#endif
		} else {
			// printf("-> No remote remap needed!\n");
		}
	}

	void debugPrintAllocation() {
		long maxIndex = bitset_size;
		while (maxIndex > 0 && pages_used[maxIndex - 1].none())
			maxIndex--;

		shm_debug("Page allocation: ");
		for (long i = 0; i < maxIndex; i++) {
			shm_debug("%02x ", pages_used[i].data);
		}
		shm_debug("\n");
	}

	void debugPrintAllocationLarge() {
		long maxIndex = bitset_size;
		while (maxIndex > 0 && pages_used[maxIndex - 1].none())
			maxIndex--;

		shm_debug("Page allocation: ");
		for (long i = 0; i < 8 * maxIndex; i++) {
			if (i % 100 == 0)
					shm_debug("\n%05ld:   ", i);
			shm_debug(pages_used[i / 8][i % 8] ? "b" : "_");
		}
		shm_debug("\n");
	}

	void *synchronizedFileMap(size_t length, int prot, int flags, int fd, off64_t offset) {
		auto remoteFD = fileDescriptors->mapToOtherSide(fd);
		void *addr = (void *) mmap_next_free_address;
		mmap_next_free_address += ALIGN_UP(length, _dl_pagesize);
		auto result = real(mmap)(addr, length, prot, flags, fd, offset);
		auto result2 = __ipc_replacement_internal_mmap(addr, length, prot, flags, remoteFD, offset);
		debug_printf("synchronized file mmap: (%d@%p) == (%d@%p)", fd, result, remoteFD, result2);
		return result;
	}
};

SharedMemoryInformation *shared_memory_info;
int shared_memory_fd = -1;

void shared_memory_init(FileDescriptorMapping *fileDescriptors) {
	if (!shared_memory_info) {
		size_t ps = _dl_pagesize = (unsigned) getpagesize();
		int info_pages = 1;
		const int bitset_pages = 512;
		while (ps * info_pages < sizeof *shared_memory_info)
			info_pages *= 2;
#ifdef NO_SHM_MODE
		int fd = -1;
		void *ptr = real(mmap)((void *) shm_shared_memory_base, ps * (info_pages + bitset_pages), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, fd, 0);
#else
#ifdef MPROTECT_MODE
		int fd = create_new_shared_memory_segment(ps);
		void *ptr = real(mmap)((void *) shm_shared_memory_base, ps, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		void *initial_memory = real(mmap)(nullptr, ps * (info_pages + bitset_pages), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
		int used_pages = 0;
		size_t mapped_size = ps;
#else
		int fd = create_new_shared_memory_segment(ps * (info_pages + bitset_pages));
		void *ptr = real(mmap)((void *) shm_shared_memory_base, ps * (info_pages + bitset_pages), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		void *initial_memory = (void *) (shm_shared_memory_base + (ps * info_pages));
		int used_pages = info_pages + bitset_pages;
		size_t mapped_size = ps * (info_pages + bitset_pages);
#endif
#endif
		shared_memory_info = new(ptr) SharedMemoryInformation();
		debug_printf("shared_memory_info = %p", shared_memory_info);
		shared_memory_info->init(fd, mapped_size, initial_memory, bitset_pages * ps, used_pages);
		shared_memory_fd = fd;
		shared_memory_info->fileDescriptors = fileDescriptors;

		shm_register_fork_handler();
	}
}

void shared_memory_terminate() {
	shared_memory_info->terminate();
}

void shared_memory_forking() {
	shared_memory_info->has_forked = true;
}

void shared_memory_forking_complete() {
	shared_memory_has_forked_complete = true;
}

void shared_memory_unfork() {
	shared_memory_has_forked_complete = false;
	if (shared_memory_info) shared_memory_info->has_unintenden_fork = true;
	// fprintf(stderr, "Unintended fork here (to %d).\n", getpid());
}

int shared_memory_has_forked(void) {
	return shared_memory_has_forked_complete;
}

void internal_setSharedFDSize() {
	shared_memory_info->setFDsize();
}

void shared_memory_set_rw() {
	inline_mprotect(shared_memory_info->base, shared_memory_info->shared_size, PROT_READ | PROT_WRITE);
}

void shared_memory_set_ro() {
	inline_mprotect(shared_memory_info->base, shared_memory_info->shared_size, PROT_READ);
}

static void shared_memory_remapped_to_size(void* addr, size_t len) {
	if (shared_memory_info && shared_memory_info->base == addr) {
		// fprintf(stderr, "[%d] Follow remap: %ld\n", getpid(), len);
		shared_memory_info->unshared_infos->current_mapped_size = len;
	}
}

static void print_flags(int flags) {
	printf("Flags: ");
#define MAP_HUGE_2MB    (21 << MAP_HUGE_SHIFT)
#define MAP_HUGE_1GB    (30 << MAP_HUGE_SHIFT)
	if (flags & MAP_32BIT) printf("MAP_32BIT ");
	if (flags & MAP_ANONYMOUS) printf("MAP_ANONYMOUS ");
	if (flags & MAP_DENYWRITE) printf("MAP_DENYWRITE ");
	if (flags & MAP_EXECUTABLE) printf("MAP_EXECUTABLE ");
	if (flags & MAP_FILE) printf("MAP_FILE ");
	if (flags & MAP_FIXED) printf("MAP_FIXED ");
	//if (flags & MAP_FIXED_NOREPLACE) printf("MAP_FIXED_NOREPLACE ");
	if (flags & MAP_GROWSDOWN) printf("MAP_GROWSDOWN ");
	if (flags & MAP_HUGE_1GB) printf("MAP_HUGE_1GB ");
	if (flags & MAP_HUGE_2MB) printf("MAP_HUGE_2MB ");
	if (flags & MAP_HUGETLB) printf("MAP_HUGETLB ");
	if (flags & MAP_LOCKED) printf("MAP_LOCKED ");
	if (flags & MAP_NONBLOCK) printf("MAP_NONBLOCK ");
	if (flags & MAP_NORESERVE) printf("MAP_NORESERVE ");
	if (flags & MAP_POPULATE) printf("MAP_POPULATE ");
	if (flags & MAP_PRIVATE) printf("MAP_PRIVATE ");
	if (flags & MAP_SHARED) printf("MAP_SHARED ");
	if (flags & MAP_STACK) printf("MAP_STACK ");
	//if (flags & MAP_SYNC) printf("MAP_SYNC ");
	//if (flags & MAP_UNINITIALIZED) printf("MAP_UNINITIALIZED ");
	printf("\n");
}

/**
 * mmap calls from other processes, coming over IPC. Handle with care...
 * @param addr
 * @param length
 * @param prot
 * @param flags
 * @param fd
 * @param offset
 * @return
 */
void *internal_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
	if (addr == nullptr || (!shm_is_shared_memory_pointer(addr) && !shm_is_shared_file_pointer(addr))) {
		fprintf(stderr, "[SHM] [ERR] Invalid address through IPC mmap: %p!\n", addr);
		return MAP_FAILED;
	}
	shmman_debug("[SHM] internal_mmap(%p, %zu (=%zuKB), %d, %d, %d, %zd)\n", addr, length, length / 1024, prot, flags, fd, offset);
	return real(mmap)(addr, length, prot, flags, fd, offset);
}

/*
void *internal_mremap(void *start, size_t old_len, size_t len, int flags) {
	debug_printf("%d  (sym) remap %p: %ld => %ld", use_shared_memory_only, start, old_len, len);

	if (!shm_is_shared_memory_pointer(start) || !shm_is_shared_memory_pointer(void_ptr_add(start, len))) {
		fprintf(stderr, "[SHM] [ERR] Invalid address through IPC remap: %p!\n", start);
		return MAP_FAILED;
	}
	if (shared_memory_info && start == shared_memory_info->base)
		old_len = shared_memory_info->unshared_infos->current_mapped_size;
	void *result = real(mremap)(start, old_len, len, flags);
	debug_printf("%p result", result);
	if (result != MAP_FAILED)
		shared_memory_remapped_to_size(start, len);
	return result;
}
 */

void internal_remap_shm() {
	if (shared_memory_info->shared_size > shared_memory_info->unshared_infos->current_mapped_size) {
		// fprintf(stderr, "[%d] mremap(%p, 0x%lx, 0x%lx)\n", getpid(), shared_memory_info->base, shared_memory_info->unshared_infos->current_mapped_size, shared_memory_info->shared_size);
		void *result = real(mremap)(shared_memory_info->base, shared_memory_info->unshared_infos->current_mapped_size, shared_memory_info->shared_size, MREMAP_MAYMOVE);
		if (result == MAP_FAILED) {
			unregister_shared_memory_as_default();
			perror("remap failed");
			pmap_me_shm();
			exit(1);
		}
		if (result != shared_memory_info->base) {
			unregister_shared_memory_as_default();
			fputs("remap moved the shared memory!\n", stderr);
			exit(1);
		}
		shared_memory_remapped_to_size(shared_memory_info->base, shared_memory_info->shared_size);
	}
}

void remap_shared_memory() {
	internal_remap_shm();
}

void *shm_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
	if (fd >= 0 || offset > 0 || addr != nullptr) {
		// Delegate to original mmap
		void *result = real(mmap)(addr, length, prot, flags, fd, offset);
		if ((program_params.silent & 1) == 0) {
			fprintf(stderr, "[SHM] [WARN] Unsupported shm_mmap call from pid %d!\n", getpid());
			fprintf(stderr, "[SHM] fallback mmap(%p, %zu (=%zuKB), %d, %d, fd=%d, %zd) => %p\n", addr, length, length / 1024, prot, flags, fd, offset, result);
		}
		return result;
	}
	// print_flags(flags);

	length = ALIGN_UP(length, _dl_pagesize);
	// Force addr in our special area
	//addr = void_ptr_add(shared_memory_info->base, shared_memory_info->shared_size);
	//flags = (flags & ~MAP_PRIVATE) | MAP_SHARED;
	shmman_debug("[SHM] mmap(%p, %zu (=%zuKB), %d, %d, %d, %zd)\n", addr, length, length / 1024, prot, flags, fd, offset);
	// Map already existing fd
	long pageStart = shared_memory_info->requestContinousFreePages(length / _dl_pagesize);
	return shared_memory_info->pageIndexToAddress(pageStart);
}

void *shm_mmap_share(void *addr, size_t length, int prot, int flags, int fd, off64_t offset) {
	if (fd >= 0 && shared_memory_info) {
		return shared_memory_info->synchronizedFileMap(length, prot, flags, fd, offset);
	}
	return shm_mmap(addr, length, prot, flags, fd, offset);
}

void *shm_mremap(void *old_address, size_t old_size, size_t new_size, int flags) {
	// puts("shm_remap");
	old_size = ALIGN_UP(old_size, _dl_pagesize);
	new_size = ALIGN_UP(new_size, _dl_pagesize);
	size_t old_page_index = shared_memory_info->addressToPageIndex(old_address);
	long new_page_index = shared_memory_info->requestContinousFreePagesForMove(new_size / _dl_pagesize, old_page_index);
	if (new_page_index == old_page_index)
		return old_address;

	// Copy content and free old pages
	void *new_address = shared_memory_info->pageIndexToAddress(new_page_index);
	memcpy(new_address, old_address, old_size);
	shared_memory_info->freePages(old_page_index, old_size / _dl_pagesize);
	return new_address;
}

int shm_munmap(void *addr, size_t length) {
	length = ALIGN_UP(length, _dl_pagesize);
	shared_memory_info->freePages(shared_memory_info->addressToPageIndex(addr), length / _dl_pagesize);
}

int shm_mprotect(void *addr, size_t len, int prot) {
	if (shm_is_shared_memory_pointer(addr) && (prot == (PROT_READ | PROT_WRITE) || prot == PROT_READ)) {
		return 0;
	}
	return mprotect(addr, len, prot);
}

void shm_msync() {
	//fprintf(stderr, "Invalidate...\n");
	//if (msync(shared_memory_info->base, shared_memory_info->size, MS_SYNC) != 0)
	//	perror("[SHM] msync()");
}

void shm_print_debug_info() {
	shared_memory_info->debugPrintAllocation();
}

int shm_vasprintf(char **strp, const char *fmt, va_list ap) {
	char *result;
	int len = vasprintf(&result, fmt, ap);
	*strp = (char *) shm_malloc_2(len + 1);
	memcpy(*strp, result, len + 1);
	free(result);
	return len;
}

int shm_asprintf(char **strp, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	int len = shm_vasprintf(strp, fmt, va);
	va_end(va);
	return len;
}

char *shm_getenv(const char *name) {
	char *result = getenv(name);
	if (result) {
		int len = strlen(result);
		char *r2 = (char *) shm_malloc_2(len + 1);
		memmove(r2, result, len + 1);
		return r2;
	}
	return result;
}

char *shm_strdup(const char *s) {
	size_t len = strlen(s) + 1;
	void *newString = shm_malloc_2(len);
	if (newString == nullptr)
		return nullptr;
	memmove(newString, s, len);
	return (char *) newString;
}

void shm_free_wrapper(void *ptr, void *originalFree) {
	if (shm_is_shared_memory_pointer(ptr)) {
		shm_free(ptr);
	} else {
		auto f = (void (*)(void *)) originalFree;
		f(ptr);
	}
}
