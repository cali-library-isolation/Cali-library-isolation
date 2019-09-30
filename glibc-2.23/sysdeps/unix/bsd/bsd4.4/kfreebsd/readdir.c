/* 'readdir64' is the same as 'readdir', because
   struct dirent64 == struct dirent.  */

#define readdir64 __no_readdir64_decl
#define __readdir64 __no___readdir64_decl
#include <sysdeps/posix/readdir.c>
#undef __readdir64
#undef readdir64

strong_alias (__readdir, __readdir64)
weak_alias (__readdir64, readdir64)
