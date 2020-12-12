#ifndef LLVMREWRITER_CONFIGFILE_H
#define LLVMREWRITER_CONFIGFILE_H


#include <string>
#include <map>
#include <vector>
#include <memory>
#include <set>


class RlimitConfig {
public:
	std::string as = "";
	std::string core = "";
	std::string cpu = "";
	std::string fsize = "";
	std::string nofile = "";
	std::string nproc = "";
	std::string stack = "";
};

class PermissionConfig {
public:
	std::vector<std::string> readonly;
	std::vector<std::string> readwrite;
	std::vector<std::string> tmpfs;
	std::string user = "";
	std::string group = "";
	std::string network = "";

	std::string seccomp = "";
	std::vector<std::string> capabilities;

	int max_cpus = 0;
	bool silent = false;
	RlimitConfig rlimit;
	ssize_t cgroup_mem_max = 0;
	ssize_t cgroup_pids_max = 0;
	ssize_t cgroup_cpu_ms_per_sec = 0;
};

class ContextConfig {
public:
	std::string name;
	std::vector<std::string> selectors;
	std::vector<std::string> ignoredFunctions;
	PermissionConfig permissions;
	bool instrument = false;
	bool instrument_coverage = false;
	std::string instrument_user = "";
	bool warnings = false;

	bool mprotect_mode = false;
	bool sequential_mode = false;
	bool programIsForking = false;
	int silent = 0;
	bool concurrentLibraryCommunication = false;
	std::map<std::string, std::string> functionBehavior;

	bool matches(std::string filename) const;
};

class YamlConfig {
public:
	std::map<std::string, ContextConfig> contexts;

	static YamlConfig fromFile(const std::string& filename);
	static YamlConfig empty();

	bool writeGraphs = false;
	bool writeGraphsDot = false;
	bool linkerOverride = false;
	bool addLibstdcxx = false;

	std::map<std::string, std::vector<std::string>> replaceArguments;
	std::vector<std::string> ignoreOutputs;

	int limit_subnode_depth = 3;
	int limit_struct_parts = 32;

	std::map<std::string, std::vector<int>> filedescriptors;
	bool strongFDAnalysis = false;
	bool strongFPAnalysis = false;
};

#endif
