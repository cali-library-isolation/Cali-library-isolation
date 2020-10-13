#ifndef CALI_PROGRAM_PARAMS_H
#define CALI_PROGRAM_PARAMS_H

struct program_params {
	int is_forking;
	int silent; // 1 = shared memory warning, 2 = dup stderr to pipe, 4+ = ...
	int concurrentLibraryCommunication; // 1 = lock sending direction
};

extern struct program_params program_params;

#endif //CALI_PROGRAM_PARAMS_H
