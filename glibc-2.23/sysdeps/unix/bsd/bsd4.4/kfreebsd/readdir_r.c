/* 'readdir64_r' is the same as 'readdir_r', because
   struct dirent64 == struct dirent.  */

#define readdir64_r __no_readdir64_r_decl
#define __readdir64_r __no___readdir64_r_decl
#include <sysdeps/posix/readdir_r.c>
#undef __readdir64_r
#undef readdir64_r

strong_alias (__readdir_r, __readdir64_r)
weak_alias (__readdir64_r, readdir64_r)
