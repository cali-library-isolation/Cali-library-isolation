/* Machine-dependent ELF dynamic relocation inline functions.  FreeBSD/amd64 version.
   Copyright (C) 2006 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Petr Salinger, 2006.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include_next <dl-machine.h>

#undef RTLD_START

/* Initial entry point code for the dynamic linker.
   The C function `_dl_start' is the real entry point;
   its return value is the user program's entry point.  */
#define RTLD_START asm ("\n\
.text\n\
	.align 16\n\
.globl _start\n\
# we dont use it: .globl _dl_start_user\n\
_start:\n\
	# align stack.\n\
	andq $-16, %rsp\n\
	# save argument pointer.\n\
	movq %rdi, %r13\n\
	call _dl_start\n\
# we dont use it: _dl_start_user:\n\
	# Save the user entry point address in %r12.\n\
	movq %rax, %r12\n\
	# See if we were run as a command with the executable file\n\
	# name as an extra leading argument.\n\
	movl _dl_skip_args(%rip), %eax\n\
	# get the original argument count.\n\
	movq 0(%r13), %rdx\n\
	# Adjust the pointer to skip _dl_skip_args words.\n\
	leaq (%r13,%rax,8), %r13\n\
	# Subtract _dl_skip_args from argc.\n\
	subl %eax, %edx\n\
	# Put argc on adjusted place\n\
	movq %rdx, 0(%r13)\n\
	# Call _dl_init (struct link_map *main_map, int argc, char **argv, char **env)\n\
	# argc -> rsi\n\
	movq %rdx, %rsi\n\
	# _dl_loaded -> rdi\n\
	movq _rtld_local(%rip), %rdi\n\
	# env -> rcx\n\
	leaq 16(%r13,%rdx,8), %rcx\n\
	# argv -> rdx\n\
	leaq 8(%r13), %rdx\n\
	# Clear %rbp to mark outermost frame obviously even for constructors.\n\
	xorl %ebp, %ebp\n\
	# Call the function to run the initializers.\n\
	call _dl_init@PLT\n\
	# Pass our finalizer function to the user in %rdx, as per ELF ABI draft.\n\
	leaq _dl_fini(%rip), %rdx\n\
	# And make sure %rdi points to argc stored on the stack.\n\
	movq %r13, %rdi\n\
	# Pass finalizer function also in %rsi, as per C calling convention.\n\
	movq %rdx, %rsi\n\
	# Jump to the user's entry point.\n\
	jmp *%r12\n\
.previous\n\
");

#if 0
Under FreeBSD:
#define AT_EXECPATH     15      /* Path to the executable. */

Under Linux:
#define AT_PLATFORM     15      /* String identifying platform.  */

Filled entries from kernel:

        if (args->execfd != -1)
                AUXARGS_ENTRY(pos, AT_EXECFD, args->execfd);
        AUXARGS_ENTRY(pos, AT_PHDR, args->phdr);    
        AUXARGS_ENTRY(pos, AT_PHENT, args->phent);  
        AUXARGS_ENTRY(pos, AT_PHNUM, args->phnum);  
        AUXARGS_ENTRY(pos, AT_PAGESZ, args->pagesz);
        AUXARGS_ENTRY(pos, AT_FLAGS, args->flags);
        AUXARGS_ENTRY(pos, AT_ENTRY, args->entry);
        AUXARGS_ENTRY(pos, AT_BASE, args->base);
        if (imgp->execpathp != 0)
                AUXARGS_ENTRY(pos, AT_EXECPATH, imgp->execpathp);
        AUXARGS_ENTRY(pos, AT_NULL, 0);
                                                                                                                
#endif

#ifndef _DL_MACHINE_KFREEBSD
#define _DL_MACHINE_KFREEBSD

static inline void __attribute__ ((unused))
dl_platform_kfreebsd_x86_64_init (void)
{
	/* This calls cpuid and and fills dl_x86_cpu_features */
	DL_PLATFORM_INIT;

	/* we don't have reasonable AT_PLATFORM from kernel
	   use cpuid to guess AT_HWCAP */
	GLRO(dl_hwcap) = GLRO(dl_x86_cpu_features).cpuid[COMMON_CPUID_INDEX_1].edx;
	GLRO(dl_platform) = ELF_MACHINE_NAME;
}

#undef  DL_PLATFORM_INIT
#define DL_PLATFORM_INIT dl_platform_kfreebsd_x86_64_init ()

#endif
