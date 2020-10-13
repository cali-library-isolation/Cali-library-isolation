#ifndef CALI_SIGNALS_H
#define CALI_SIGNALS_H

#include "hook.hpp"
#include <signal.h>

real_function_extern(signal, sighandler_t(int, sighandler_t));

#endif //CALI_SIGNALS_H
