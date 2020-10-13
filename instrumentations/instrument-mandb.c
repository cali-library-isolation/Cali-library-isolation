#include "utils.h"
#include <gdbm.h>
#include <stdlib.h>

#define VERBOSE 12u

#define GZ_VERBOSE (VERBOSE & 4u)

#include "lib-gzip.h"

#define INTERNAL_VERBOSE (VERBOSE & 8u)

#include "lib-internal.h"


inline const char *format_datum(datum d) {
	if (!d.dptr) return "{-}";
	char *buffer = malloc(128);
	sprintf(buffer, "{\"%s\",%d}", d.dptr, d.dsize);
	return buffer;
}


void instrument_before_gdbm_fetch(GDBM_FILE f, datum d) {
	if ((VERBOSE & 1) | check_ptr(f) | check_ptr(d.dptr))
		debug_printf("gdbm_fetch(%p, {%p, %d})", f, d.dptr, d.dsize);
	if (VERBOSE & 2) debug_printf("FETCH %s ...", format_datum(d));
}

void instrument_after_gdbm_fetch(datum result) {
	if ((VERBOSE & 1) | check_ptr(result.dptr))
		debug_printf(" => {%p, %d}", result.dptr, result.dsize);
	if (VERBOSE & 2) debug_printf(" => %s", format_datum(result));
}

void instrument_before_gdbm_delete(GDBM_FILE f, datum d) {
	if ((VERBOSE & 1) | check_ptr(f) | check_ptr(d.dptr))
		debug_printf("gdbm_delete(%p, {%p, %d})", f, d.dptr, d.dsize);
}

void instrument_before_gdbm_reorganize(GDBM_FILE f) {
	if ((VERBOSE & 1) | check_ptr(f))
		debug_printf("gdbm_reorganize(%p)", f);
}

void instrument_before_gdbm_exists(GDBM_FILE f, datum d) {
	if ((VERBOSE & 1) | check_ptr(f) | check_ptr(d.dptr))
		debug_printf("gdbm_exists(%p, {%p, %d})", f, d.dptr, d.dsize);
}

void instrument_before_gdbm_store(GDBM_FILE f, datum d1, datum d2, int i) {
	if ((VERBOSE & 1) | check_ptr(f) | check_ptr(d1.dptr) | check_ptr(d2.dptr))
		debug_printf("gdbm_store(%p, {%p, %d}, {%p, %d}, %d)", f, d1.dptr, d1.dsize, d2.dptr, d2.dsize, i);
	if (VERBOSE & 2) debug_printf("STORE %s => %s ...", format_datum(d1), format_datum(d2));
}

void instrument_after_gdbm_store(int result) {
	if (VERBOSE & 2) {
		if (result == 0) debug_printf(" => OK");
		else
			debug_printf(" => ERROR %d", result);
	}
}

void instrument_before_gdbm_open(const char *s, int p2, int p3, int p4, void (*cb)(const char *)) {
	if ((VERBOSE & 1) | check_ptr(s))
		debug_printf("gdbm_open(%p, %d, %d, %d, %p)  \"%s\"", s, p2, p3, p4, cb, s);
}

void instrument_after_gdbm_open(GDBM_FILE f) {
	if ((VERBOSE & 1) | check_ptr(f))
		debug_printf(" => %p", f);
}

void instrument_before_gdbm_firstkey(GDBM_FILE f) {
	if ((VERBOSE & 1) | check_ptr(f))
		debug_printf("gdbm_firstkey(%p)", f);
}

void instrument_after_gdbm_firstkey(datum result) {
	if ((VERBOSE & 1) | check_ptr(result.dptr))
		debug_printf(" => {%p, %d}", result.dptr, result.dsize);
	if (VERBOSE & 2) debug_printf("first key => %s", format_datum(result));
}

void instrument_before_gdbm_nextkey(GDBM_FILE f, datum d) {
	if ((VERBOSE & 1) | check_ptr(f) | check_ptr(d.dptr))
		debug_printf("gdbm_nextkey(%p, {%p, %d})", f, d.dptr, d.dsize);
	if (VERBOSE & 2) debug_printf("next key of %s ...", format_datum(d));
}

void instrument_after_gdbm_nextkey(datum result) {
	if ((VERBOSE & 1) | check_ptr(result.dptr))
		debug_printf(" => {%p, %d}", result.dptr, result.dsize);
	if (VERBOSE & 2) debug_printf(" => %s", format_datum(result));
}

void instrument_before_gdbm_fdesc(GDBM_FILE f) {
	if ((VERBOSE & 1) | check_ptr(f))
		debug_printf("gdbm_fdesc(%p)", f);
}

void instrument_before_gdbm_close(GDBM_FILE f) {
	if ((VERBOSE & 1) | check_ptr(f))
		debug_printf("gdbm_close(%p)", f);
}
