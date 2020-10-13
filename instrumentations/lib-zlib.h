#ifndef CALI_LIB_ZLIB_H
#define CALI_LIB_ZLIB_H

#include <zlib.h>
#include "utils.h"

void instrument_before_inflateInit2_(z_streamp stream, int windowBits, char *version, int stream_size) {
	if (ZLIB_VERBOSE | check_ptr(stream) | check_ptr(version)) debug_printf("inflateInit2_(%p, %d, %p, %d)", stream, windowBits, version, stream_size);
}

void instrument_before_inflateInit_(z_streamp stream, const char *version, int stream_size) {
	if (ZLIB_VERBOSE | check_ptr(stream) | check_ptr(version)) debug_printf("inflateInit_(%p, %p, %d)", stream, version, stream_size);
}

void instrument_before_inflate(z_streamp stream, int flush) {
	if (ZLIB_VERBOSE | check_ptr(stream) | check_ptr(stream->next_in) | check_ptr(stream->next_out))
		debug_printf("inflate(%p, %d)  {%p, %p}", stream, flush, stream->next_in, stream->next_out);
}

void instrument_before_deflateInit2_(z_streamp stream, int level, int method, int windowBits, int memLevel, int strategy, char *version, int stream_size) {
	if (ZLIB_VERBOSE | check_ptr(stream) | check_ptr(version))
		debug_printf("deflateInit2_(%p, %d, %d, %d, %d, %d, %p, %d)", stream, level, method, windowBits, memLevel, strategy, version, stream_size);
}

void instrument_before_deflateInit_(z_streamp stream, int level, const char *version, int stream_size) {
	if (ZLIB_VERBOSE | check_ptr(stream)) debug_printf("deflateInit_(%p, %d, %p, %d)", stream, level, version, stream_size);
}

void instrument_before_inflateEnd(z_streamp stream) {
	if (ZLIB_VERBOSE | check_ptr(stream)) debug_printf("inflateEnd(%p)", stream);
}

void instrument_before_deflateEnd(z_streamp stream) {
	if (ZLIB_VERBOSE | check_ptr(stream)) debug_printf("deflateEnd(%p)", stream);
}

void instrument_before_deflate(z_streamp stream, int flush) {
	if (ZLIB_VERBOSE | check_ptr(stream) | check_ptr(stream->next_in) | check_ptr(stream->next_out))
		debug_printf("deflate(%p, %d)  {%p, %p}", stream, flush, stream->next_in, stream->next_out);
}

void instrument_before_crc32(uLong crc, const Bytef *buf, uInt len) {
	if (ZLIB_VERBOSE | check_ptr(buf)) debug_printf("crc32(%ld, %p, %d)", crc, buf, len);
}

void instrument_before_deflateBound(z_streamp stream, uLong sourceLen) {
	if (ZLIB_VERBOSE | check_ptr(stream)) debug_printf("deflateBound(%p, %ld)", stream, sourceLen);
}


#endif //CALI_LIB_ZLIB_H
