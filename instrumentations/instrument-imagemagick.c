#include "utils.h"

#define VERBOSE 0


typedef struct gsapi_revision_s {
	const char *product;
	const char *copyright;
	long revision;
	long revisiondate;
} gsapi_revision_t;




void instrument_before_gsapi_delete_instance(void *instance) {
	if (VERBOSE | check_ptr(instance)) debug_printf("gsapi_delete_instance(...)");
}

void instrument_before_gsapi_exit(void *instance) {
	if (VERBOSE | check_ptr(instance)) debug_printf("gsapi_exit(...)");
}

void instrument_before_gsapi_init_with_args(void *instance, int argc, char **argv) {
	if (VERBOSE | check_ptr(instance) | check_ptr(argv)) debug_printf("gsapi_init_with_args(...)");
}

void instrument_before_gsapi_new_instance(void **pinstance, void *caller_handle) {
	if (VERBOSE | check_ptr(pinstance) | check_ptr(caller_handle)) debug_printf("gsapi_new_instance(...)");
}

void instrument_before_gsapi_revision(gsapi_revision_t *pr, int len) {
	if (VERBOSE | check_ptr(pr) | check_ptr(pr->copyright) | check_ptr(pr->product)) debug_printf("gsapi_revision(...)");
}

void instrument_before_gsapi_run_string(void *instance, const char *str, int user_errors, int *pexit_code) {
	if (VERBOSE | check_ptr(instance) | check_ptr(str) | check_ptr(pexit_code)) debug_printf("gsapi_run_string(...)");
}

void instrument_before_gsapi_set_stdio(void *instance, int(*stdin_fn)(void *caller_handle, char *buf, int len),
									   int(*stdout_fn)(void *caller_handle, const char *str, int len),
									   int(*stderr_fn)(void *caller_handle, const char *str, int len)) {
	if (VERBOSE | check_ptr(instance)) debug_printf("gsapi_set_stdio(...)");
}

