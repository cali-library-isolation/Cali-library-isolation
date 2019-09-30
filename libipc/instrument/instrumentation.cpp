#include <cstdint>
#include <iostream>
#include <map>
#include <fstream>
#include <sys/mman.h>
#include <unistd.h>
#include <cmath>
#include "IntervalTree.h"
#include "../shared_memory_manger.h"
#include "json.hpp"

using namespace std;

#define use_symbol(x) static volatile void* __use_##x = (void*) &(x)
#define byte unsigned char


extern "C" {
void insInit();
void insAddMemoryNode(uint64_t id, uint64_t type, uint64_t is_shared, char *def, char *function);
void insAlloc(uint64_t id, void *start, uint64_t size);
int insFree(void *addr) asm("insFree");
void insFinished();

__attribute__((noinline)) void insWriteReportUrl(char *buffer, const char *extension);
void insWriteReportUrl(char *buffer, const char *extension) {
	buffer[0] = 0;
}

// do not optimize away the init function
void __instrumentation_init(void);
use_symbol(__instrumentation_init);

extern char __start_shm_data;
extern char __last_shm_data_variable;
}

static inline bool shm_is_global_variable(void *addr) {
	return &__start_shm_data <= (char *) addr && ((char *) addr) < &__last_shm_data_variable;
}


class MemoryInfo {
public:
	MemoryInfo() {}

	MemoryInfo(uint64_t id, uint64_t type, bool is_shared, const string &definition, const string &function)
			: id(id), type(type), is_shared(is_shared), definition(definition), function(function) {}

	uint64_t id;
	uint64_t type;
	bool is_shared;
	string definition;
	string function;

	uint64_t allocations = 0;
	uint64_t deallocations = 0;
	uint64_t used_allocations = 0; // = used in library
	uint64_t not_deallocated = 0;
	uint64_t size = 0;

	enum Type {
		MI_ALLOCA = 1,
		MI_GLOBAL = 2,
		MI_HEAP = 3
	};
};

class MemoryRangeMarker {
	static constexpr int divisor = 4;

	uintptr_t memory_start;
	byte *data;
public:
	MemoryRangeMarker(uintptr_t memory_start, uintptr_t pin_start, size_t memory_size_in_mb) : memory_start(memory_start) {
		auto size = memory_size_in_mb * 1024L * 1024L / divisor / 8;
		data = (byte *) mmap((void *) pin_start, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON | MAP_FIXED, -1, 0);
	}

	void unset(void *addr, size_t size) {
		uintptr_t bitidx_begin = (((uintptr_t) addr) - memory_start) / divisor;
		uintptr_t bitidx_end = (((uintptr_t) addr) + size - memory_start + divisor - 1) / divisor;

		uintptr_t innerbytes_begin = (bitidx_begin + 7) / 8;
		uintptr_t innerbytes_end = bitidx_end / 8;

		for (uintptr_t i = innerbytes_begin; i < innerbytes_end; i++) {
			data[i] = 0;
		}
		for (uintptr_t i = bitidx_begin; i < innerbytes_begin * 8; i++) {
			data[i / 8] &= ~(1 << (i % 8));
			// if (size == 8) fprintf(stderr, "%lu ", i);
		}
		for (uintptr_t i = innerbytes_end * 8; i < bitidx_end; i++) {
			data[i / 8] &= ~(1 << (i % 8));
			// if (size == 8) fprintf(stderr, "%lu ", i);
		}
		// fprintf(stderr, "Clean %lu - %lu\n", bitidx_begin, bitidx_end);
	}

	bool check(void *addr, size_t size) {
		uintptr_t bitidx_begin = (((uintptr_t) addr) - memory_start) / divisor;
		uintptr_t bitidx_end = (((uintptr_t) addr) + size - memory_start + divisor - 1) / divisor;

		uintptr_t innerbytes_begin = (bitidx_begin + 7) / 8;
		uintptr_t innerbytes_end = bitidx_end / 8;

		for (uintptr_t i = bitidx_begin; i < innerbytes_begin * 8; i++) {
			if (data[i / 8] & (1 << (i % 8))) {
				/*if (addr == (void*) 0x3f0000205d00)
					fprintf(stderr, "check(%p, %ld) => [%ld, %ld), first hit: %ld\n", addr, size, bitidx_begin, bitidx_end, i);*/
				return true;
			}
		}
		for (uintptr_t i = innerbytes_begin; i < innerbytes_end; i++) {
			if (data[i] != 0) {
				/*if (addr == (void*) 0x3f0000205d00)
					fprintf(stderr, "check(%p, %ld) => [%ld, %ld), first hit (b): %ld\n", addr, size, bitidx_begin, bitidx_end, i*8);*/
				return true;
			}
		}
		for (uintptr_t i = innerbytes_end * 8; i < bitidx_end; i++) {
			if (data[i / 8] & (1 << (i % 8))) {
				/*if (addr == (void*) 0x3f0000205d00)
					fprintf(stderr, "check(%p, %ld) => [%ld, %ld), first hit: %ld\n", addr, size, bitidx_begin, bitidx_end, i);*/
				return true;
			}
		}

		// fprintf(stderr, "CHECK %lu - %lu => false\n", bitidx_begin, bitidx_end);

		return false;
	}
};

static map<uint64_t, MemoryInfo *> *nodes = nullptr;
static fub::IntervalMap<void *, MemoryInfo *> *intervals = nullptr;
static MemoryRangeMarker *heapMarker = nullptr;
static MemoryRangeMarker *globalMarker = nullptr;

void writeHtmlReport();

void writeJsonReport();


static inline uintptr_t page(void *ptr) {
	uintptr_t mask = ~(((uintptr_t) getpagesize()) - 1);
	return mask & (uintptr_t) ptr;
}


void insInit() {
	cout << "<Instrumentation Init>" << endl;
	uintptr_t shm_data_page = page(&__start_shm_data);
	heapMarker = new MemoryRangeMarker(shm_shared_memory_base, 0x2f0000000000, 4096);
	globalMarker = new MemoryRangeMarker(shm_data_page, 0x280000000000, 128);
	register_shared_memory_as_default(); // Our datastructure should be shared
	nodes = new map<uint64_t, MemoryInfo *>();
	intervals = new fub::IntervalMap<void *, MemoryInfo *>();
	cout << "</Instrumentation Init>" << endl;
}

void insAddMemoryNode(uint64_t id, uint64_t type, uint64_t is_shared, char *def, char *function) {
	nodes->emplace(std::make_pair(id, new MemoryInfo(id, type, is_shared != 0, def, function)));
}

void insAlloc(uint64_t id, void *start, uint64_t size) {
	std::lock_guard<std::mutex> guard(memory_mutex);
	unregister_shared_memory_as_default();
	// fprintf(stderr, "insAlloc(%ld, %p, %ld);\n", id, start, size);
	if (nodes == nullptr) {
		fprintf(stderr, "!!! insInit() not called !!! - insAlloc(%ld, %p, %ld)\n", id, start, size);
	} else {
		(*nodes)[id]->allocations++;
		(*nodes)[id]->size += size;

		// fprintf(stderr, "Alloc %p (%lu) for %lu\n", start, size, id);

		// register region
		void *end = ((char *) start) + size;
		if ((*intervals)[start]) {
			//TODO that check is shit
			auto interval = *(*intervals)[start];
			fprintf(stderr, "Collision: %lu @ %p (%s) still allocated\nNew one:   %lu @ %p (%s)!\n", interval->id, start, interval->definition.c_str(), id,
					start, (*nodes)[id]->definition.c_str());
			// exit(-1);
			// throw runtime_error("Intervals overlapping!");
		} else {
			intervals->insert(start, end, (*nodes)[id]);
		}
		// remove previous usages
		if (shm_is_shared_memory_pointer(start))
			heapMarker->unset(start, size);
		else if (shm_is_global_variable(start))
			globalMarker->unset(start, size);
	}
	reregister_shared_memory_as_default();
}

void checkAndReportUsage(void *addr) {
	auto it = intervals->interval_start(addr);
	if (it == intervals->end() || !it->second)
		return;

	void *start = it->first;
	MemoryInfo *info = *it->second;
	it++;
	void *end = it->first;

	if ((shm_is_shared_memory_pointer(start) && heapMarker->check(start, (uintptr_t) end - (uintptr_t) start)) ||
		(shm_is_global_variable(start) && globalMarker->check(start, (uintptr_t) end - (uintptr_t) start))) {
		// if (addr == (void *) 0x3f0000205d00)
		// 	cerr << "Used: " << info->id << endl;
		info->used_allocations++;
	}
}

int insFree(void *addr) {
	// fprintf(stderr, "insFree(%p)\n", addr);
	if (nodes == nullptr) {
		fprintf(stderr, "!!! insInit() not called !!! - insFree(%p)\n", addr);
		return 0;
	}
	auto info = intervals->interval_start(addr);
	if (info != intervals->end()) {
		// fprintf(stderr, "  = %p\n", info->first);
		checkAndReportUsage(info->first);
		(*info->second)->deallocations++;
		intervals->remove(info->first);
	}
	// fprintf(stderr, "  done(%p)\n", addr);
	return 0;
}

void insFinished() {
	cout.flush();
	fflush(stdout);
	cerr.flush();

	// check outstanding uses for everything in intervals / not freed
	for (const auto &it: intervals->std_map()) {
		if (it.second) {
			if ((*it.second)->type == MemoryInfo::MI_GLOBAL) {
				// cout << (*it.second)->definition << endl;
				(*it.second)->deallocations++;
			} else {
				(*it.second)->not_deallocated++;
			}
			checkAndReportUsage(it.first);
		}
	}

	cerr << "Finished, writing report..." << endl;
	writeHtmlReport();
	writeJsonReport();
}


static std::string replaceAll(std::string &str, const std::string &from, const std::string &to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

static string html_escape(string s) {
	s = replaceAll(s, "&", "&amp;");
	s = replaceAll(s, "<", "&lt;");
	s = replaceAll(s, ">", "&gt;");
	s = replaceAll(s, "\"", "&quot;");
	return s;
}

void writeHtmlReport() {
	uint64_t shared = 0;
	uint64_t shared_and_used = 0;
	uint64_t shared_and_alloced = 0;
	uint64_t allocations = 0;
	uint64_t deallocations = 0;
	uint64_t used_allocations = 0;
	uint64_t not_deallocated = 0;
	uint64_t shared_analyses = 0;
	for (const auto &it: *nodes) {
		if (it.second->is_shared) {
			shared++;
			shared_analyses += it.second->deallocations + it.second->not_deallocated;
			shared_and_alloced += it.second->allocations > 0;
			shared_and_used += it.second->used_allocations > 0;
		}
		allocations += it.second->allocations;
		deallocations += it.second->deallocations;
		used_allocations += it.second->used_allocations;
		not_deallocated += it.second->not_deallocated;
	}

	printf("[Stats] Shared %ld / %ld memory-allocating nodes (%.1f%%)\n", shared, nodes->size(), shared * 100.0 / nodes->size());
	cout << "[Stats] Shared " << shared << " / " << nodes->size() << " memory-allocating nodes" << endl;

	char buffer[1024];
	insWriteReportUrl(buffer, "html");
	ofstream f(buffer, ios::out | ios::trunc);
	f << "<!DOCTYPE html>\n"
		 "<html lang=\"en\">\n"
		 "  <head>\n"
		 "    <meta charset=\"utf-8\">\n"
		 "    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
		 "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
		 "    <title>Report</title>\n"
		 "    <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\" integrity=\"sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u\" crossorigin=\"anonymous\">\n"
		 "    <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css\" integrity=\"sha384-rHyoN1iRsVXV4nD0JutlnGaslCJuC7uwjduW9SVrLvRYooPp2bWYgmgJQIXwl/Sp\" crossorigin=\"anonymous\">\n"
		 "    <style>.hide-nonshared tbody > tr {display: none;}  .hide-nonshared tbody > tr.shared {display: table-row;} td:nth-child(2){word-break: break-all;}</style>"
		 "  </head>\n"
		 "  <body>\n"
		 "    <div class=\"container-fluid\">\n"
		 "      <h1>IPC Statistics Report</h1>\n"
		 "      <button class=\"btn btn-default\" onclick=\"document.body.classList.add('hide-nonshared')\">Hide nonshared</button>\n"
		 "      <button class=\"btn btn-default\" onclick=\"document.body.classList.remove('hide-nonshared')\">Show nonshared</button>\n<br/><br/>"
		 "      <table class=\"table table-bordered\">\n"
		 "        <thead>\n"
		 "          <tr>\n"
		 "            <th>ID</th>\n"
		 "            <th>Function</th>\n"
		 "            <th>Instruction</th>\n"
		 "            <th>~ Size</th>\n"
		 "            <th>Shared?</th>\n"
		 "            <th># alloc</th>\n"
		 "            <th># dealloc</th>\n"
		 "            <th># used allocs</th>\n"
		 "            <th># leaked</th>\n"
		 "          </tr>\n"
		 "        </thead>\n"
		 "        <tbody>\n";
	for (const auto &it: *nodes) {
		f << "          <tr" << (it.second->is_shared ? " class=\"shared\"" : "") << ">\n";
		f << "            <td>" << it.second->id << "</td>\n";
		f << "            <td>" << html_escape(it.second->function) << "</td>\n";
		f << "            <td>" << html_escape(it.second->definition) << "</td>\n";
		f << "            <td>" << (it.second->allocations ? round(it.second->size / it.second->allocations) : 0.0) << "</td>\n";
		if (it.second->is_shared && it.second->used_allocations)
			f << "            <td class=\"success\">" << (it.second->is_shared ? "X" : "-") << "</td>\n";
		else if (it.second->is_shared && it.second->allocations)
			f << "            <td class=\"warning\">" << (it.second->is_shared ? "X" : "-") << "</td>\n";
		else
			f << "            <td>" << (it.second->is_shared ? "X" : "-") << "</td>\n";
		f << "            <td>" << it.second->allocations << "</td>\n";
		f << "            <td>" << it.second->deallocations << "</td>\n";
		f << "            <td>" << it.second->used_allocations << "</td>\n";
		f << "            <td>" << it.second->not_deallocated << "</td>\n";
		f << "          </tr>\n";
	}
	f << "        </tbody>\n";
	f << "        <tfoot><tr>\n";
	f << "            <td>" << nodes->size() << "</td>\n";
	f << "            <td>&nbsp;</td>\n";
	f << "            <td>&nbsp;</td>\n";
	f << "            <td>&nbsp;</td>\n";
	f << "            <td>" << shared << " (" << shared_and_used << " / " << shared_and_alloced << " used)</td>\n";
	f << "            <td>" << allocations << "</td>\n";
	f << "            <td>" << deallocations << "</td>\n";
	auto percent = shared_analyses > 0 ? round(used_allocations * 1000000.0 / shared_analyses) / 10000.0 : 0.0;
	f << "            <td>" << used_allocations << " / " << shared_analyses << " (" << percent << "%)</td>\n";
	f << "            <td>" << not_deallocated << "</td>\n";
	f << "        </tr></tfoot>\n";
	f << "      </table>\n"
		 "    </div>\n"
		 "  </body>\n"
		 "</html>";

	cout << "Wrote \"" << buffer << "\"" << endl;
}


void writeJsonReport() {
	uint64_t shared = 0;
	uint64_t allocations = 0;
	uint64_t deallocations = 0;
	uint64_t used_allocations = 0;
	uint64_t not_deallocated = 0;
	uint64_t shared_analyses = 0;
	for (const auto &it: *nodes) {
		if (it.second->is_shared) {
			shared++;
			shared_analyses += it.second->deallocations + it.second->not_deallocated;
		}
		allocations += it.second->allocations;
		deallocations += it.second->deallocations;
		used_allocations += it.second->used_allocations;
		not_deallocated += it.second->not_deallocated;
	}

	nlohmann::json j = {
			{"node_count",        nodes->size()},
			{"shared_node_count", shared},
			{"allocations",       allocations},
			{"deallocations",     deallocations},
			{"used_allocations",  used_allocations},
			{"not_deallocated",   not_deallocated},
			{"nodes",             nlohmann::json::array()}
	};

	for (const auto &it: *nodes) {
		j["nodes"].push_back({
									 {"id",             it.second->id},
									 {"function",       it.second->function},
									 {"instruction",    it.second->definition},
									 {"avg_size",       it.second->allocations ? round(it.second->size / it.second->allocations) : 0.0},
									 {"shared",         it.second->is_shared},
									 {"num_alloc",      it.second->allocations},
									 {"num_dealloc",    it.second->deallocations},
									 {"num_used_alloc", it.second->used_allocations},
									 {"num_leaked",     it.second->not_deallocated}
							 });
	}


	char buffer[1024];
	insWriteReportUrl(buffer, "json");
	ofstream f(buffer, ios::out | ios::trunc);
	f << j;

	cout << "Wrote \"" << buffer << "\"" << endl;
}
