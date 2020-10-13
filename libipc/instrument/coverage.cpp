#include <vector>
#include <cstdio>
#include <cerrno>
#include <fstream>
#include <unistd.h>

extern "C" {
[[maybe_unused]] void __coverage_count(int index, int count);
}

std::vector<bool> recorded_coverage;

[[maybe_unused]] void __coverage_count(int index, int count) {
	if (recorded_coverage.size() < count)
		recorded_coverage.resize(count, false);
	recorded_coverage[index] = true;
}

void report_coverage() {
	if (recorded_coverage.empty()) {
		fprintf(stderr, "Coverage not initialized.\n");
		return;
	}
	fprintf(stderr, "Coverage: ");
	int count = 0;
	for (bool b: recorded_coverage) {
		fprintf(stderr, b ? "x" : ".");
		if (b) count++;
	}
	auto percent = 100.0 * count / recorded_coverage.size();
	fprintf(stderr, " (%d / %ld, %.2f%%)\n", count, recorded_coverage.size(), percent);

	int start = -1;
	bool comma = false;
	for (int i = 0; i < recorded_coverage.size(); i++) {
		if (recorded_coverage[i]) {
			if (start == -1) start = i;
		} else {
			if (start != -1) {
				if (comma) fprintf(stderr, ", ");
				else comma = true;
				if (start == i - 1) fprintf(stderr, "%d", start);
				else fprintf(stderr, "%d-%d", start, i - 1);
				start = -1;
			}
		}
	}
	if (start != -1) {
		if (comma) fprintf(stderr, ", ");
		if (start == recorded_coverage.size() - 1) fprintf(stderr, "%d", start);
		else fprintf(stderr, "%d-%ld", start, recorded_coverage.size() - 1);
	}
	fprintf(stderr, "\n");
}

void report_coverage_lite(int ipc_calls) {
	if (recorded_coverage.empty()) return;
	std::string pattern;
	for (bool b: recorded_coverage) {
		pattern += b ? 'x' : '.';
	}
	auto fname = "/dev/shm/coverage_" + std::string(program_invocation_short_name);
	std::ofstream ofs(fname, std::ofstream::out | std::ofstream::app);
	if (ofs.tellp() == 0) {
		std::vector<char> v(1024);
		int r = readlink("/proc/self/exe", v.data(), v.size());
		if (r > 0) {
			v[r] = 0;
			ofs << "BINARY: " << v.data() << std::endl;
		}
	}
	std::string ipc_calls_s = std::to_string(ipc_calls);
	ofs << ipc_calls;
	for (int i = ipc_calls_s.size(); i < 7; i++) ofs << " ";
	ofs << " " << pattern << std::endl;
	ofs.close();
	// fprintf(stderr, "[%d] Saved coverage report.\n", getpid());
}



// ================= Disable seccomp =================
#include "../hook.hpp"
#define PR_SET_NO_NEW_PRIVS	38
#define PR_GET_NO_NEW_PRIVS	39
#define PR_GET_SECCOMP	21
#define PR_SET_SECCOMP	22

/*

real_function(prctl, int(int, unsigned long, unsigned long, unsigned long, unsigned long));

extern "C" {
int prctl(int option, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
int seccomp(unsigned int operation, unsigned int flags, void *args);
int seccomp_load(void*);
}

int prctl(int option, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5) {
	if (option == PR_SET_NO_NEW_PRIVS) {
		fprintf(stderr, "Mocked prctl(PR_SET_NO_NEW_PRIVS)\n");
		errno = 0;
		return 0;
	}
	if (option == PR_SET_SECCOMP) {
		fprintf(stderr, "Mocked prctl(PR_SET_SECCOMP)\n");
		errno = 0;
		return 0;
	}
	if (option == PR_GET_NO_NEW_PRIVS || option == PR_GET_SECCOMP)
		fprintf(stderr, "Might need to mock prctl(%d)\n", option);
	return real(prctl)(option, arg2, arg3, arg4, arg5);
}

int seccomp(unsigned int operation, unsigned int flags, void *args) {
	fprintf(stderr, "Mocker seccomp(%d, %d, %p)\n", operation, flags, args);
	return 0;
}

int seccomp_load(void*) {
	fprintf(stderr, "Mocked seccomp_load(...)\n");
	return 0;
}

// */
