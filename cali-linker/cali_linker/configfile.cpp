#include <stdexcept>
#include <fstream>
#include <sstream>
#include "configfile.h"
#include <llvm/ObjectYAML/YAML.h>
#include <llvm/Support/YAMLParser.h>
#include <iostream>
#include "llvm/Support/YAMLTraits.h"


using llvm::yaml::MappingTraits;
using llvm::yaml::IO;

namespace llvm {
	namespace yaml {

		template<>
		struct MappingTraits<YamlConfig> {
			static void mapping(IO &io, YamlConfig &config) {
				io.mapRequired("contexts", config.contexts);
				io.mapOptional("writeGraphs", config.writeGraphs);
				io.mapOptional("writeGraphsDot", config.writeGraphsDot);
				io.mapOptional("linkerOverride", config.linkerOverride);
				io.mapOptional("addLibstdc++", config.addLibstdcxx);
				io.mapOptional("replaceArguments", config.replaceArguments);
				io.mapOptional("ignoreOutputs", config.ignoreOutputs);
				io.mapOptional("filedescriptors", config.filedescriptors);
				io.mapOptional("strongFDAnalysis", config.strongFDAnalysis);
				io.mapOptional("strongFPAnalysis", config.strongFPAnalysis);
				io.mapOptional("limit_struct_parts", config.limit_struct_parts);
				io.mapOptional("limit_subnode_depth", config.limit_subnode_depth);
			}
		};


		template<>
		struct MappingTraits<ContextConfig> {
			static void mapping(IO &io, ContextConfig &config) {
				io.mapRequired("selectors", config.selectors);
				io.mapOptional("ignored_functions", config.ignoredFunctions);
				io.mapOptional("permissions", config.permissions);
				io.mapOptional("instrument", config.instrument);
				io.mapOptional("instrumentCoverage", config.instrument_coverage);
				io.mapOptional("instrumentUser", config.instrument_user);
				io.mapOptional("warnings", config.warnings);
				io.mapOptional("mprotect_mode", config.mprotect_mode);
				io.mapOptional("sequential_mode", config.sequential_mode);
				io.mapOptional("programIsForking", config.programIsForking);
				io.mapOptional("silent", config.silent);
				io.mapOptional("concurrentLibraryCommunication", config.concurrentLibraryCommunication);
				io.mapOptional("function_behavior", config.functionBehavior);
			}
		};


		template<>
		struct MappingTraits<PermissionConfig> {
			static void mapping(IO &io, PermissionConfig &config) {
				io.mapOptional("readonly", config.readonly);
				io.mapOptional("readwrite", config.readwrite);
				io.mapOptional("tmpfs", config.tmpfs);
				io.mapOptional("user", config.user);
				io.mapOptional("group", config.group);
				io.mapOptional("network", config.network);
				io.mapOptional("seccomp", config.seccomp);
				io.mapOptional("capabilities", config.capabilities);
				io.mapOptional("max_cpus", config.max_cpus);
				io.mapOptional("silent", config.silent);
				io.mapOptional("rlimit", config.rlimit);
				io.mapOptional("cgroup_mem_max", config.cgroup_mem_max);
				io.mapOptional("cgroup_pids_max", config.cgroup_pids_max);
				io.mapOptional("cgroup_cpu_ms_per_sec", config.cgroup_cpu_ms_per_sec);
			}
		};


		template<>
		struct MappingTraits<RlimitConfig> {
			static void mapping(IO &io, RlimitConfig &config) {
				io.mapOptional("as", config.as);
				io.mapOptional("core", config.core);
				io.mapOptional("cpu", config.cpu);
				io.mapOptional("fsize", config.fsize);
				io.mapOptional("nofile", config.nofile);
				io.mapOptional("nproc", config.nproc);
				io.mapOptional("stack", config.stack);
			}
		};


		template<>
		struct CustomMappingTraits<std::map<std::string, ContextConfig>> : public StdMapStringCustomMappingTraitsImpl<ContextConfig> {
			static void inputOne(IO &io, StringRef key, map_type &v) {
				io.mapRequired(key.str().c_str(), v[key]);
				v[key].name = key.str();
			}
		};

		template<>
		struct CustomMappingTraits<std::map<std::string, int>> : public StdMapStringCustomMappingTraitsImpl<int> {};

		template<>
		struct CustomMappingTraits<std::map<std::string, std::string>> : public StdMapStringCustomMappingTraitsImpl<std::string> {};

		template<>
		struct CustomMappingTraits<std::map<std::string, std::vector<int>>> : public StdMapStringCustomMappingTraitsImpl<std::vector<int>> {};

		template<>
		struct CustomMappingTraits<std::map<std::string, std::vector<std::string>>> : public StdMapStringCustomMappingTraitsImpl<std::vector<std::string>> {};

	}
}


// LLVM_YAML_IS_STRING_MAP(ContextConfig)


YamlConfig YamlConfig::fromFile(const std::string& filename) {
	std::ifstream t(filename);
	std::stringstream buffer;
	buffer << t.rdbuf();
	if (!t.is_open())
		throw std::runtime_error("Could not find config file");
	std::string buf = buffer.str();

	YamlConfig config;
	llvm::yaml::Input yaml_in(buf);
	yaml_in >> config;
	if (yaml_in.error())
		throw std::runtime_error("Parse error in config file");

	std::cout << "Config file: " << filename << std::endl;
	/*for (auto c: config.contexts) {
		std::cout << "Name = " << c.second.name << std::endl;
	}*/

	return config;
}


YamlConfig YamlConfig::empty() {
	YamlConfig result;
	result.contexts["main"] = ContextConfig();
	result.contexts["main"].name = "main";
	result.contexts["main"].selectors.emplace_back("*.o");
	result.contexts["library"] = ContextConfig();
	result.contexts["library"].name = "library";
	return result;
}



// following https://www.geeksforgeeks.org/wildcard-pattern-matching/
bool strmatch(std::string str, std::string pattern) {
	auto n = str.length();
	auto m = pattern.length();
	// empty pattern can only match with empty string
	if (m == 0)
		return (n == 0);

	// lookup table for storing results of subproblems
	bool lookup[n + 1][m + 1];

	// initailze lookup table to false
	memset(lookup, false, sizeof(lookup));

	// empty pattern can match with empty string
	lookup[0][0] = true;

	// Only '*' can match with empty string
	for (int j = 1; j <= m; j++)
		if (pattern[j - 1] == '*')
			lookup[0][j] = lookup[0][j - 1];

	// fill the table in bottom-up fashion
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			// Two cases if we see a '*'
			// a) We ignore ‘*’ character and move to next  character in the pattern, i.e., ‘*’ indicates an empty sequence.
			// b) '*' character matches with ith character in input
			if (pattern[j - 1] == '*')
				lookup[i][j] = lookup[i][j - 1] || lookup[i - 1][j];

				// Current characters are considered as matching in two cases
				// (a) current character of pattern is '?'
				// (b) characters actually match
			else if (pattern[j - 1] == '?' || str[i - 1] == pattern[j - 1])
				lookup[i][j] = lookup[i - 1][j - 1];

				// If characters don't match
			else lookup[i][j] = false;
		}
	}

	return lookup[n][m];
}


bool ContextConfig::matches(const std::string filename) const {
	for (const auto &pattern: selectors) {
		// std::cout << "M \"" << filename << "\" \"" << "*" + pattern << "\" => " << strmatch(filename, "*" + pattern) << std::endl;
		if (!pattern.empty() && pattern[0] == '^') {
			if (strmatch(filename, pattern.substr(1)))
				return true;
		} else {
			if (strmatch(filename, "*" + pattern))
				return true;
		}
	}
	return false;
}
