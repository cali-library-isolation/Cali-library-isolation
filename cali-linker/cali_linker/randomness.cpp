#include <random>
#include "randomness.h"

namespace {
	std::random_device dev;
	std::mt19937_64 engine(dev());
}

std::string getRandomString(int length) {
	const char* chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::uniform_int_distribution<std::mt19937_64::result_type> dst(0, 61);
	std::string result;
	for (int i = 0; i < length; i++)
		result += chars[dst(engine)];
	return result;
}
