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
	if (addr == nullptr || !shm_is_shared_memory_pointer(addr)) {
		fprintf(stderr, "[SHM] [ERR] Invalid address through IPC mmap: %p!\n", addr);
		return MAP_FAILED;
	}
	shmman_debug("[SHM] internal_mmap(%p, %zu (=%zuKB), %d, %d, %d, %zd)\n", addr, length, length / 1024, prot, flags, fd, offset);
	return real(mmap)(addr, length, prot, flags, fd, offset);
}

void *internal_mremap(void *start, size_t old_len, size_t len, int flags) {
	debug_printf("%d  (sym) remap %p: %ld => %ld\n", use_shared_memory_only, start, old_len, len);

	if (!shm_is_shared_memory_pointer(start) || !shm_is_shared_memory_pointer(void_ptr_add(start, len))) {
		fprintf(stderr, "[SHM] [ERR] Invalid address through IPC remap: %p!\n", start);
		return MAP_FAILED;
	}
	void *result = real(mremap)(start, old_len, len, flags);
	debug_printf("%p result\n", result);
	return result;
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
	debug_printf("Malloc hook installed\n");
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




// 64 different characters - 6 bit randomness per char
static const char *random_characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_-";

static int create_new_shared_memory_segment(size_t initial_size) {
	srand((unsigned) time(0));
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
	ftruncate(fd, initial_size);
	shmman_debug("[SHM] Created segment \"%s\", open as %d\n", random, fd);
	return fd;
}




class SharedMemoryInformation {
public:
	void *base = (void *) shm_shared_memory_base;
	bool has_forked = false;
private:
	size_t shared_size = 0;
	int fd = -1;

	//std::bitset<8> *pages_used = nullptr;
	ByteBitset *pages_used = nullptr;
	size_t bitset_size = 0; // bytes in pages_used
	size_t first_unused_page_index = 0; // page index of the first page that has never been used / mapped (e.g. is zero'd).

	//std::mutex mtx;
	SharedMutex mtx;

public:
	void init(int fd, size_t size, void *pages_initial_memory, size_t bitset_size, int used_pages) {
		this->shared_size = size;
		this->fd = fd;
		this->base = this;
		this->has_forked = false;
		this->pages_used = (ByteBitset *) pages_initial_memory;
		this->bitset_size = bitset_size;
		memset(this->pages_used, 0, this->bitset_size);
		for (int i = 0; i < used_pages; i++) {
			pages_used[i / 8].set(i % 8, true);
		}
		first_unused_page_index = used_pages;
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
		size_t old_shared_size = shared_size;
		shared_size = _dl_pagesize * pages;
		//TODO ugly hack
		//if (use_shared_memory_only) {
		//	__ipc_replacement_internal_setSharedFDSize();
		//} else {
		setFDsize();
		//}
		remapSharedMemory(old_shared_size);
		first_unused_page_index = std::min(first_unused_page_index, pages);
	}

	void setFDsize() {
#ifndef NO_SHM_MODE
		if (ftruncate(fd, shared_size) != 0) {
			perror("ftruncate() failed");
			fprintf(stderr, "fd=%d size=%ld pages=%ld\n", fd, shared_size, shared_size / _dl_pagesize);
			throw std::runtime_error("ftruncate failed");
		}
		debug_printf("%d  ftruncate(%d, %ld)\n", use_shared_memory_only, fd, shared_size);
#endif
	}

	void remapSharedMemory(size_t old_shared_size) {
		// printf("%d  remap %p: %ld => %ld\n", use_shared_memory_only, base, old_shared_size, shared_size);
#ifndef NO_SHM_MODE
		void *addr = real(mremap)(base, old_shared_size, shared_size, MREMAP_MAYMOVE);
		if (addr == MAP_FAILED) {
			perror("mremap on fd");
			throw std::runtime_error("Could not remap shared memory!");
		}
		if (addr != base) {
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
		if (has_forked) {
#ifndef NO_FORK_MODE
			addr = __ipc_replacement_internal_mremap(base, old_shared_size, shared_size, MREMAP_MAYMOVE);
			if (addr == MAP_FAILED) {
				fprintf(stderr, "mremap failed on remote\n");
				throw std::runtime_error("Could not remap shared memory!");
			}
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
};

SharedMemoryInformation *shared_memory_info;
int shared_memory_fd = -1;

void shared_memory_init() {
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
		int fd = create_new_shared_memory_segment(ps * (info_pages + bitset_pages));
		void *ptr = real(mmap)((void *) shm_shared_memory_base, ps * (info_pages + bitset_pages), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
#endif
		shared_memory_info = new(ptr) SharedMemoryInformation();
		debug_printf("%p\n", shared_memory_info);
		shared_memory_info->init(fd, ps * (info_pages + bitset_pages), (void *) (shm_shared_memory_base + (ps * info_pages)), bitset_pages * ps,
								 info_pages + bitset_pages);
		shared_memory_fd = fd;

		shm_register_fork_handler();
	}
}

void shared_memory_forking() {
	shared_memory_info->has_forked = true;
}

void internal_setSharedFDSize() {
	shared_memory_info->setFDsize();
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

void *shm_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
	if (fd >= 0 || offset > 0 || addr != nullptr) {
		// Delegate to original mmap
		void *result = real(mmap)(addr, length, prot, flags, fd, offset);
		fprintf(stderr, "[SHM] [WARN] Unsupported shm_mmap call!\n");
		fprintf(stderr, "[SHM] fallback mmap(%p, %zu (=%zuKB), %d, %d, %d, %zd) => %p\n", addr, length, length / 1024, prot, flags, fd, offset, result);
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

void shm_msync() {
	//fprintf(stderr, "Invalidate...\n");
	//if (msync(shared_memory_info->base, shared_memory_info->size, MS_SYNC) != 0)
	//	perror("[SHM] msync()");
}

void shm_print_debug_info() {
	shared_memory_info->debugPrintAllocation();
}