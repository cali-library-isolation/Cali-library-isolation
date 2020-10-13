#ifndef CALI_LIB_GZIP_H
#define CALI_LIB_GZIP_H

#include <zlib.h>
#include "utils.h"

void instrument_before_gzdopen(int fd, const char *mode) {
	if (GZ_VERBOSE | check_ptr(mode)) debug_printf("gzdopen(%d, %p)  mode=%s", fd, mode, mode);
}

void instrument_after_gzdopen(gzFile file) {
	if (GZ_VERBOSE) debug_printf(" => %p", file);
}

void instrument_before_gzread(gzFile file, void *buf, unsigned int len) {
	if (GZ_VERBOSE | check_ptr(file) | check_ptr(buf)) debug_printf("gzread(%p, %p, %d)", file, buf, len);
}

void instrument_after_gzread(int result) {
	if (GZ_VERBOSE) debug_printf(" => %d", result);
}

void instrument_before_gzclose(gzFile file) {
	if (GZ_VERBOSE | check_ptr(file)) debug_printf("gzclose(%p)", file);
}


#endif //CALI_LIB_GZIP_H
