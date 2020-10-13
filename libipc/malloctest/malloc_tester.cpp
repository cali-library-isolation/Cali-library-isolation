#include <mutex>
#include "malloc_tester.h"
#include "order_statistic_tree.h"
#include "../shm_malloc.h"
#include "../shared_memory_manger.h"
#include "../gdb.h"
#include <sys/types.h>
#include <sys/syscall.h>
#include <zconf.h>
#include <cstring>


static inline pid_t gettid() {
	pid_t tid = syscall(SYS_gettid);
	return tid;
}



class MallocInterval {
public:
	void *start;
	void *end;

	MallocInterval(void *start, void *end) : start(start), end(end) {}

	MallocInterval(void *start, size_t len) : start(start), end(((char *) start) + len) {}

	bool operator<(const MallocInterval &r) const {
		return start < r.start;
	}

	bool operator>(const MallocInterval &r) const {
		return start > r.start;
	}

	bool operator<(const void *ptr) const {
		return start < ptr;
	}

	bool operator>(const void *ptr) const {
		return start > ptr;
	}

	long &operator[](int i) {
		return static_cast<long *>(start)[i];
	}

	const long &operator[](int i) const {
		return static_cast<long *>(start)[i];
	}

	inline size_t len() const {
		return ((char *) end) - ((char *) start);
	}

};

bool operator<(const void *ptr, const MallocInterval &r) {
	return ptr < r.start;
}

static order_statistic_tree_t<MallocInterval> intervals;
static const int prefixSize = 8;
static std::mutex intervalsLock;


static bool isCollisionFree(MallocInterval &interval) {
	auto possibleCollision = std::max_element(std::begin(intervals),
											  std::find_if(std::begin(intervals), std::end(intervals),
														   [&](MallocInterval i) { return interval < i; }));
	bool collisionFree =
			possibleCollision->end <= ((char *) interval.start) - prefixSize ||
			interval.end <= ((char *) possibleCollision->start) - prefixSize;

	possibleCollision = std::lower_bound(std::begin(intervals), std::end(intervals), interval.start);
	collisionFree &= possibleCollision->end <= ((char *) interval.start) - prefixSize ||
					 interval.end <= ((char *) possibleCollision->start) - prefixSize;

	possibleCollision = std::upper_bound(std::begin(intervals), std::end(intervals), interval.start);
	collisionFree &= possibleCollision->end <= ((char *) interval.start) - prefixSize ||
					 interval.end <= ((char *) possibleCollision->start) - prefixSize;

	return collisionFree;
}

static bool findAndRemoveInterval(void *start) {
	auto it = std::lower_bound(intervals.begin(), intervals.end(), start);
	if (it != intervals.end() && it->start == start) {
		intervals.erase(it);
		return true;
	} else {
		return false;
	}
}



void *shm_malloc_2(size_t size) {
#ifndef MALLOC_TESTER
	return shm_malloc(size);
#endif
	std::lock_guard<std::mutex> lock(intervalsLock);
	unregister_shared_memory_as_default();
	void *ptr = shm_malloc(size);

	shm_debug("shm_malloc_2(%zu); // %p [%d]\n", size, ptr, gettid());

#ifdef MALLOC_TESTER_INTERVALCHECK
	if (intervals.empty()) {
		fprintf(stderr, "[MallocTest] First interval ever: %p [%ld]\n", ptr, size);
		shm_print_debug_info();
	}

	MallocInterval interval(ptr, size);
	if (!isCollisionFree(interval)) {
		fprintf(stderr, "[ERROR?]  Interval collision in shm_malloc: %p (len: %ld)\n", ptr, size);
		throw runtime_error("shm_malloc: Interval collision");
	}
	intervals.insert(interval);
#endif

	reregister_shared_memory_as_default();
	return ptr;
}

void *shm_calloc_2(size_t size1, size_t size2) {
#ifndef MALLOC_TESTER
	return shm_calloc(size1, size2);
#endif

	std::lock_guard<std::mutex> lock(intervalsLock);
	unregister_shared_memory_as_default();
	void *ptr = shm_calloc(size1, size2);

	shm_debug("shm_calloc_2(%zu, %zu); // %p [%d]\n", size1, size2, ptr, gettid());

#ifdef MALLOC_TESTER_INTERVALCHECK
	if (intervals.empty()) {
		fprintf(stderr, "[MallocTest] First interval ever: %p [%ld]\n", ptr, size1*size2);
		shm_print_debug_info();
	}

	MallocInterval interval(ptr, size1*size2);
	if (!isCollisionFree(interval)) {
		fprintf(stderr, "[ERROR?]  Interval collision in shm_calloc: %p (len: %ld)\n", ptr, size1*size2);
		throw runtime_error("shm_calloc: Interval collision");
	}
	intervals.insert(interval);
#endif

	reregister_shared_memory_as_default();
	return ptr;
}

static long c1 = 0;
static long c2 = 0;

void free_shm_pointer_hook(void *ptr) {
#ifndef MALLOC_TESTER
	return;
#endif

	std::lock_guard<std::mutex> lock(intervalsLock);
	shm_debug("shm_free_2(%p); [%d/%d]\n", ptr, gettid(), getpid());

	c1++;
#ifdef MALLOC_TESTER_INTERVALCHECK
	if (!findAndRemoveInterval(ptr)) {
		c2++;
		fprintf(stderr, "[ERROR?]  [free]: Interval has not been issued before! %p (%ld / %ld)\n", ptr, c2, c1);
		for (const auto &i: intervals) {
			fprintf(stderr, " - %p\n", i.start);
		}
		throw runtime_error("free: Interval has not been issued before!");
	}
#endif
}

void shm_free_2(void *ptr) {
	std::lock_guard<std::mutex> guard(memory_mutex);
	unregister_shared_memory_as_default();
#ifdef MALLOC_TESTER
	free_shm_pointer_hook(ptr);
#endif
	shm_free(ptr);
	reregister_shared_memory_as_default();
}

void realloc_shm_pointer_hook(void *ptr_old, void *ptr_new, size_t size_new) {
#ifndef MALLOC_TESTER
	return;
#endif

	std::lock_guard<std::mutex> lock(intervalsLock);
	shm_debug("shm_realloc_2(%p, %ld); // %p [%d]\n", ptr_old, size_new, ptr_new, gettid());

#ifdef MALLOC_TESTER_INTERVALCHECK
	if (ptr_old && !findAndRemoveInterval(ptr_old)) {
		fprintf(stderr, "[ERROR?]  [realloc]: Interval has not been issued before! %p\n", ptr_old);
		// throw runtime_error("realloc: Interval has not been issued before!");
	}
	MallocInterval interval(ptr_new, size_new);
	if (!isCollisionFree(interval)) {
		fprintf(stderr, "[ERROR?]  Interval collision in shm_realloc: %p (len: %ld)\n", ptr_new, size_new);
		throw runtime_error("shm_realloc: Interval collision");
	}
	intervals.insert(interval);
#endif
}
