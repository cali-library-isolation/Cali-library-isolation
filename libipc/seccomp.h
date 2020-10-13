#ifndef CALI_SECCOMP_H
#define CALI_SECCOMP_H

#include <cstdint>

class SeccompFilterBuilder {
	struct sock_filter* filter;
	unsigned short filter_count = 0;
public:
	SeccompFilterBuilder();
	void install_filter();
};

#endif //CALI_SECCOMP_H
