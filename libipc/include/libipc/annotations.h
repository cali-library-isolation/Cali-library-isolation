#ifndef LIBIPC_ANNOTATIONS_H
#define LIBIPC_ANNOTATIONS_H

#ifdef __cplusplus
extern "C" {
#endif


#define ipc_replacement_function(name) __ipc_specialized_##name

/**
 * Frees a pointer if it comes from shared memory. Should be used before every custom free() implementation
 * @param pointer
 * @return 0 if the pointer has been free'd, the pointer itself otherwise.
 */
void *ipc_free(void *pointer);


#ifdef __cplusplus
}
#endif

#endif