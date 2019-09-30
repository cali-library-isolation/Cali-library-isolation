#include <map>
#include "timing-utils.h"

std::map<std::string, std::chrono::time_point<std::chrono::system_clock>> TimingTester::start_times;
