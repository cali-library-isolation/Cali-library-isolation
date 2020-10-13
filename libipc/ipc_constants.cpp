
#include "ipc_constants.h"

int use_nsjail() {
#ifdef NO_NSJAIL_MODE
	return 0;
#else
	return 1;
#endif
}


void __warning_breakpoint() {}
