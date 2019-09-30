#include <utility>

#ifndef LLVMREWRITER_TIMING_UTILS_H
#define LLVMREWRITER_TIMING_UTILS_H

#include <chrono>
#include <string>
#include <iostream>

struct TimingTester {
	std::string name;
	std::chrono::time_point<std::chrono::system_clock> starttime;

	explicit inline TimingTester(std::string name) : name(std::move(name)) {
		starttime = std::chrono::high_resolution_clock::now();
	}

	inline ~TimingTester() {
		auto dt = std::chrono::high_resolution_clock::now() - starttime;
		std::cout << "[TIME]  " << name << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(dt).count() << " ms" << std::endl;
		std::flush(std::cout);
	}


	static std::map<std::string, std::chrono::time_point<std::chrono::system_clock>> start_times;

	static inline void start(const std::string &name) {
		start_times[name] = std::chrono::high_resolution_clock::now();
	}

	static inline void stop(const std::string &name) {
		auto it = start_times.find(name);
		if (it == start_times.end()) {
			std::cout << "[TIME]  " << name << " ???" << std::endl;
		} else {
			auto dt = std::chrono::high_resolution_clock::now() - it->second;
			std::cout << "[TIME]  " << name << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(dt).count() << " ms" << std::endl;
		}
		std::flush(std::cout);
	}
};

#endif //LLVMREWRITER_TIMING_UTILS_H
