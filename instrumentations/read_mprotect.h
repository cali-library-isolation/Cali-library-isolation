#ifndef CALI_READ_MPROTECT_H
#define CALI_READ_MPROTECT_H

// TAKEN FROM https://github.com/18446744073709551615/reDroid/blob/master/jni/reDroid/re_mprot.c

#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/mman.h>

#define PROT_PRIVATE 0x8
#define PROT_SHARED 0x16

struct buffer {
	int pos;
	int size;
	char *mem;
};

char *_buf_reset(struct buffer *b) {
	b->mem[b->pos] = 0;
	b->pos = 0;
	return b->mem;
}

struct buffer *_new_buffer(int length) {
	struct buffer *res = malloc(sizeof(struct buffer) + length + 4);
	res->pos = 0;
	res->size = length;
	res->mem = (void *) (res + 1);
	return res;
}

int _buf_putchar(struct buffer *b, int c) {
	b->mem[b->pos++] = c;
	return b->pos >= b->size;
}

unsigned int read_mprotection(const void *addr) {
	int a;
	unsigned int res = 0;
	int found = 0;
	FILE *f = fopen("/proc/self/maps", "r");
	struct buffer *b = _new_buffer(1024);
	while ((a = fgetc(f)) >= 0) {
		if (_buf_putchar(b, a) || a == '\n') {
			char *end0 = (void *) 0;
			unsigned long addr0 = strtoul(b->mem, &end0, 0x10);
			char *end1 = (void *) 0;
			unsigned long addr1 = strtoul(end0 + 1, &end1, 0x10);
			if ((void *) addr0 <= addr && addr < (void *) addr1) {
				res |= (end1 + 1)[0] == 'r' ? PROT_READ : 0;
				res |= (end1 + 1)[1] == 'w' ? PROT_WRITE : 0;
				res |= (end1 + 1)[2] == 'x' ? PROT_EXEC : 0;
				res |= (end1 + 1)[3] == 'p' ? PROT_PRIVATE : (end1 + 1)[3] == 's' ? PROT_SHARED : 0;
				found = 1;
				break;
			}
			_buf_reset(b);
		}
	}
	free(b);
	fclose(f);
	return found ? res : -1;
}

int has_mprotection(const void *addr, unsigned int prot, unsigned int prot_mask) {
	unsigned prot1 = read_mprotection(addr);
	// fprintf(stderr, "MPROTECT CHECK OF %p => 0x%x\n", addr, prot1);
	return (prot1 & prot_mask) == prot;
}

#endif //CALI_READ_MPROTECT_H
