#ifndef LLVMREWRITER_STDLIB_IS_SHIT_H
#define LLVMREWRITER_STDLIB_IS_SHIT_H

#include <string>
#include <sys/stat.h>
#include <boost/filesystem.hpp>

namespace {

	inline bool startsWith(const std::string &check, const std::string &prefix) {
		return std::equal(prefix.begin(), prefix.end(), check.begin());
	}

	inline bool endsWith(std::string const &fullString, std::string const &ending) {
		if (fullString.length() >= ending.length()) {
			return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
		} else {
			return false;
		}
	}

	inline std::string replaceAll(std::string const &original, std::string const &from, std::string const &to) {
		std::string results;
		std::string::const_iterator end = original.end();
		std::string::const_iterator current = original.begin();
		std::string::const_iterator next = std::search(current, end, from.begin(), from.end());
		while (next != end) {
			results.append(current, next);
			results.append(to);
			current = next + from.size();
			next = std::search(current, end, from.begin(), from.end());
		}
		results.append(current, next);
		return results;
	}

	inline bool exists(const std::string &name) {
		struct stat buffer;
		return (stat(name.c_str(), &buffer) == 0);
	}

	inline std::string absolute(const std::string &path) {
		return boost::filesystem::absolute(path).string();
	}

	inline std::string read_file_limit(const std::string &filename, int limit) {
		std::fstream f(filename);
		std::string result;
		char c;
		while (limit && f.get(c)) {
			result += c;
			limit--;
		}
		f.close();
		return result;
	}
}

#endif
