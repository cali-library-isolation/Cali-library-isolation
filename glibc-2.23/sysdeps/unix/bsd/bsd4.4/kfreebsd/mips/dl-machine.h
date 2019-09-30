/* Machine-dependent ELF dynamic relocation inline functions.  MIPS version.
   Copyright (C) 1996-2001, 2002, 2003, 2004, 2005, 2006, 2007, 2010
   Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Kazumoto Kojima <kkojima@info.kanagawa-u.ac.jp>.
   Port to kFreeBSD (kernel of FreeBSD) by Robert Millan.

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

/* kFreeBSD puts arguments in a different location (see start.S).  */
#undef RTLD_START
#define RTLD_START asm (\
	".text\n\
	" _RTLD_PROLOGUE(ENTRY_POINT) "\
	" STRINGXV(SETUP_GPX($25)) "\n\
	" STRINGXV(SETUP_GPX64($18,$25)) "\n\
	# i386 ABI book says that the first entry of GOT holds\n\
	# the address of the dynamic structure. Though MIPS ABI\n\
	# doesn't say nothing about this, I emulate this here.\n\
	" STRINGXP(PTR_LA) " $9, _DYNAMIC\n\
	# Subtract OFFSET_GP_GOT\n\
	" STRINGXP(PTR_S) " $9, -0x7ff0($28)\n\
	move $9, $29\n\
	" STRINGXP(PTR_SUBIU) " $29, 16\n\
	\n\
	" STRINGXP(PTR_LA) " $8, .Lcoff\n\
	bltzal $8, .Lcoff\n\
.Lcoff:	" STRINGXP(PTR_SUBU) " $8, $31, $8\n\
	\n\
	" STRINGXP(PTR_LA) " $25, _dl_start\n\
	" STRINGXP(PTR_ADDU) " $25, $8\n\
	jalr $25\n\
	\n\
	" STRINGXP(PTR_ADDIU) " $29, 16\n\
	# Get the value of label '_dl_start_user' in t9 ($25).\n\
	" STRINGXP(PTR_LA) " $25, _dl_start_user\n\
	" _RTLD_EPILOGUE(ENTRY_POINT) "\
	\n\
	\n\
	" _RTLD_PROLOGUE(_dl_start_user) "\
	" STRINGXP(SETUP_GP) "\n\
	" STRINGXV(SETUP_GP64($18,_dl_start_user)) "\n\
	move $16, $28\n\
	# Save the user entry point address in a saved register.\n\
	move $17, $2\n\
	# See if we were run as a command with the executable file\n\
	# name as an extra leading argument.\n\
	lw $2, _dl_skip_args\n\
	beq $2, $0, 1f\n\
	# Load the original argument count.\n\
	" STRINGXP(PTR_L) " $10, 0($4)\n\
	# Subtract _dl_skip_args from it.\n\
	subu $10, $2\n\
	# FIXME: unnecessary? Adjust the stack pointer to skip _dl_skip_args words.\n\
	sll $2, " STRINGXP (PTRLOG) "\n\
	" STRINGXP(PTR_ADDU) " $29, $2\n\
	# Save back the modified argument count.\n\
	" STRINGXP(PTR_S) " $10, 0($4)\n\
1:	# Call _dl_init (struct link_map *main_map, int argc, char **argv, char **env) \n\
	" STRINGXP(PTR_L) /* or lw???  fixme */ " $5, 0($4)\n\
	" STRINGXP(PTR_LA) " $6, " STRINGXP (PTRSIZE) "($4)\n\
	" STRINGXP(PTR_L) " $4, _rtld_local\n\
	sll $7, $5, " STRINGXP (PTRLOG) "\n\
	" STRINGXP(PTR_ADDU) " $7, $7, $6\n\
	" STRINGXP(PTR_ADDU) " $7, $7, " STRINGXP (PTRSIZE) " \n\
	# Make sure the stack pointer is aligned for _dl_init.\n\
	and $2, $29, -2 * " STRINGXP(SZREG) "\n\
	" STRINGXP(PTR_S) " $29, -" STRINGXP(SZREG) "($2)\n\
	" STRINGXP(PTR_SUBIU) " $29, $2, 32\n\
	" STRINGXP(SAVE_GP(16)) "\n\
	# Call the function to run the initializers.\n\
	jal _dl_init\n\
	# Restore the stack pointer for _start.\n\
	" STRINGXP(PTR_L)  " $29, 32-" STRINGXP(SZREG) "($29)\n\
	# Pass our finalizer function to the user in $2 as per ELF ABI.\n\
	" STRINGXP(PTR_LA) " $2, _dl_fini\n\
	# Jump to the user entry point.\n\
	move $25, $17\n\
	jr $25\n\t"\
	_RTLD_EPILOGUE(_dl_start_user)\
	".previous"\
);
