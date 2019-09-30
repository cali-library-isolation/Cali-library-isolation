#ifndef LIBIPC_MAPPINGS_H
#define LIBIPC_MAPPINGS_H

#include <cstddef>
#include <bits/mman.h>

void remap_shared(void *addr, size_t len, int permissions = PROT_READ | PROT_WRITE);

void remap_library_segments();

void *get_new_stack();

void unmap_stack(void *sp);

void print_mapping(void);

#endif //LIBIPC_MAPPINGS_H
