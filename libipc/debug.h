#ifndef LIBIPC_DEBUG_H
#define LIBIPC_DEBUG_H

#ifndef NDEBUG
#define debug_printf(msg, ...) fprintf(stderr, msg "\n", ##__VA_ARGS__)
#else
#define debug_printf(msg, ...) do {} while(0)
#endif


/*
#ifdef __cplusplus
extern "C" {
#endif

void timing_debug_ext(const char *file, int line, const char *func, const char *msg);
#define timing_debug(msg) timing_debug_ext(__FILE__, __LINE__, __func__, msg)

#ifdef __cplusplus
};
#endif
// */

#endif //LIBIPC_DEBUG_H
