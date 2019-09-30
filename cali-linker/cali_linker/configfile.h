#ifndef LLVMREWRITER_CONFIGFILE_H
#define LLVMREWRITER_CONFIGFILE_H


#include <string>
#include <map>
#include <vector>
#include <memory>


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
	PermissionConfig permissions;
	bool instrument = false;

	bool matches(std::string filename) const;
};

class YamlConfig {
public:
	std::map<std::string, ContextConfig> contexts;

	static YamlConfig fromFile(std::string filename);

	bool writeGraphs = false;
	bool writeGraphsDot = false;
	bool linkerOverride = false;
	bool addLibstdcxx = false;

	int limit_subnode_depth = 3;
	int limit_struct_parts = 32;

	std::map<std::string, int> filedescriptors;
	bool strongFDAnalysis = false;
};

#endif
