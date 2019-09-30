/* config.h.  Generated from config.h.in by configure.  */
#if !defined IS_IN_build && !defined __ASSEMBLER__ && !defined _ISOMAC \
    && !defined __OPTIMIZE__
# error "glibc cannot be compiled without optimization"
#endif

/* Another evil option when it comes to compiling the C library is
   --ffast-math since it alters the ABI.  */
#if defined __FAST_MATH__ && !defined TEST_FAST_MATH
# error "glibc must not be compiled with -ffast-math"
#endif

/* Define if building with SELinux support.  Set by --with-selinux.  */
#define HAVE_SELINUX 1

/* Defined if building with SELinux support & audit libs are detected. */
#define HAVE_LIBAUDIT 1

/* Defined if building with SELinux support & libcap libs are detected.  */
#define HAVE_LIBCAP 1

/* Define to the assembler line separator character for multiple
   assembler instructions per line.  Default is `;'  */
/* #undef ASM_LINE_SEP */

/* Define if __attribute__((section("foo"))) puts quotes around foo.  */
/* #undef HAVE_SECTION_QUOTES */

/* Define if the assembler supports the `.set' directive.  */
#define HAVE_ASM_SET_DIRECTIVE 1

/* On powerpc64, use overlapping .opd entries.  */
/* #undef USE_PPC64_OVERLAPPING_OPD */

/* Define if _Unwind_Find_FDE should be exported from glibc.  */
/* #undef EXPORT_UNWIND_FIND_FDE */

/* Define if static NSS modules are wanted.  */
/* #undef DO_STATIC_NSS */

/* Define if the compiler supports __builtin_memset.  */
#define HAVE_BUILTIN_MEMSET 1

/* Define if compiler accepts -ftree-loop-distribute-patterns.  */
#define HAVE_CC_INHIBIT_LOOP_TO_LIBCALL 1

/* Define if the regparm attribute shall be used for local functions
   (gcc on ix86 only).  */
/* #undef USE_REGPARMS */

/* Defined on SPARC if GCC emits GOTDATA relocations.  */
/* #undef HAVE_GCC_GOTDATA */

/* Define on SPARC if AS supports VIS3 instructions.  */
/* #undef HAVE_AS_VIS3_SUPPORT */

/* Define if the linker supports the -z combreloc option.  */
#define HAVE_Z_COMBRELOC 1

/* Define if _rtld_local structure should be forced into .sdata section.  */
/* #undef HAVE_SDATA_SECTION */

/* Define if compiler supports AVX512.  */
#define HAVE_AVX512_SUPPORT 1

/* Define if assembler supports AVX512.  */
#define HAVE_AVX512_ASM_SUPPORT 1

/* Define if assembler supports vector instructions on S390.  */
/* #undef HAVE_S390_VX_ASM_SUPPORT */

/* Define if assembler supports Intel MPX.  */
#define HAVE_MPX_SUPPORT 1

/* Define if the compiler\'s exception support is based on libunwind.  */
/* #undef HAVE_CC_WITH_LIBUNWIND */

/* Define if the access to static and hidden variables is position independent
   and does not need relocations.  */
#define PI_STATIC_AND_HIDDEN 1

/* Define this to disable the 'hidden_proto' et al macros in
   include/libc-symbols.h that avoid PLT slots in the shared objects.  */
/* #undef NO_HIDDEN */

/* AArch64 big endian ABI */
/* #undef HAVE_AARCH64_BE */


/* Defined to some form of __attribute__ ((...)) if the compiler supports
   a different, more efficient calling convention.  */
#if defined USE_REGPARMS && !defined PROF
# define internal_function __attribute__ ((regparm (3), stdcall))
#endif

/* Linux specific: minimum supported kernel version.  */
#define __LINUX_KERNEL_VERSION (2 * 65536 + 6 * 256 + 32)

/* kFreeBSD specific: minimum supported kernel version.  */
/* #undef __KFREEBSD_KERNEL_VERSION */

/* Override abi-tags ABI version if necessary.  */
#define __ABI_TAG_VERSION 2,6,32

/* bash 2.0 introduced the _XXX_GNU_nonoption_argv_flags_ variable to help
   getopt determine whether a parameter is a flag or not.  This features
   was disabled later since it caused trouble.  We are by default therefore
   disabling the support as well.  */
/* #undef USE_NONOPTION_FLAGS */

/* Mach/Hurd specific: define if mig supports the `retcode' keyword.  */
/* #undef HAVE_MIG_RETCODE */

/* Mach specific: define if the `host_page_size' RPC is available.  */
/* #undef HAVE_HOST_PAGE_SIZE */

/* Mach/i386 specific: define if the `i386_io_perm_*' RPCs are available.  */
/* #undef HAVE_I386_IO_PERM_MODIFY */

/* Mach/i386 specific: define if the `i386_set_gdt' RPC is available.  */
/* #undef HAVE_I386_SET_GDT */

/* Defined of libidn is available.  */
#define HAVE_LIBIDN 1

/* Define if inlined system calls are available.  */
#define HAVE_INLINED_SYSCALLS 1

/* Define if your compiler defaults to -msecure-plt mode on ppc.  */
/* #undef HAVE_PPC_SECURE_PLT */

/* Define if __stack_chk_guard canary should be randomized at program startup.  */
#define ENABLE_STACKGUARD_RANDOMIZE 1

/* Define if lock elision should be enabled by default.  */
/* #undef ENABLE_LOCK_ELISION */

/* Package description.  */
#define PKGVERSION "(Ubuntu GLIBC 2.23-0ubuntu7) "

/* Bug reporting address.  */
#define REPORT_BUGS_TO "<https://bugs.launchpad.net/ubuntu/+source/glibc/+bugs>"

/* Define if multi-arch DSOs should be generated.  */
#define USE_MULTIARCH 1

/* Define if `.ctors' and `.dtors' sections shouldn't be used.  */
#define NO_CTORS_DTORS_SECTIONS 1

/* Define if obsolete RPC code should be made available for user-level code
   to link against.  */
#define LINK_OBSOLETE_RPC 1

/* Define if Systemtap <sys/sdt.h> probes should be defined.  */
#define USE_STAP_PROBE 1

/* Define if library functions should try to contact the nscd daemon.  */
#define USE_NSCD 1

/* Define if the dynamic linker should consult an ld.so.cache file.  */
#define USE_LDCONFIG 1

/* Define to 1 if STT_GNU_IFUNC support actually works.  */
#define HAVE_IFUNC 1

/* Define if linux/fanotify.h is available.  */
#define HAVE_LINUX_FANOTIFY_H 1

/* Define if the linker defines __ehdr_start.  */
#define HAVE_EHDR_START 1

/*
 */

#ifndef	_LIBC

/* These symbols might be defined by some sysdeps configures.
   They are used only in miscellaneous generator programs, not
   in compiling libc itself.   */

/* sysdeps/generic/configure.ac */
/* #undef HAVE_PSIGNAL */

/* sysdeps/unix/configure.ac */
/* #undef HAVE_STRERROR */

/* sysdeps/unix/common/configure.ac */
/* #undef HAVE_SYS_SIGLIST */
/* #undef HAVE__SYS_SIGLIST */
/* #undef HAVE__CTYPE_ */
/* #undef HAVE___CTYPE_ */
/* #undef HAVE___CTYPE */
/* #undef HAVE__CTYPE__ */
/* #undef HAVE__CTYPE */
/* #undef HAVE__LOCP */

#endif

/*
 */

#ifdef	_LIBC

/* The zic and zdump programs need these definitions.  */

#define	HAVE_STRERROR	1

/* The locale code needs these definitions.  */

#define HAVE_REGEX 1

/* The ARM hard-float ABI is being used.  */
/* #undef HAVE_ARM_PCS_VFP */

/* The ARM movw/movt instructions using PC-relative relocs work right.  */
#define ARM_PCREL_MOVW_OK 0

/* The pt_chown binary is being built and used by grantpt.  */
#define HAVE_PT_CHOWN 0

/* Define if the compiler supports __builtin_trap without
   any external dependencies such as making a function call.  */
#define HAVE_BUILTIN_TRAP 1

/* ports/sysdeps/mips/configure.in  */
/* Define if using the IEEE 754-2008 NaN encoding on the MIPS target.  */
/* #undef HAVE_MIPS_NAN2008 */

/* The PowerPC64 ELFv2 ABI is being used.  */
/* #undef HAVE_ELFV2_ABI */

/* PowerPC32 uses fcfid for integer to floating point conversions.  */
#define HAVE_PPC_FCFID 0

/* PowerPC32 uses fctidz for floating point to long long conversions.  */
#define HAVE_PPC_FCTIDZ 0

#endif
