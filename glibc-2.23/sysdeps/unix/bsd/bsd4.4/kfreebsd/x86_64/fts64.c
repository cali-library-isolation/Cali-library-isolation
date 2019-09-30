/* On GNU/kFreeBSD x86_64, ino_t and ino64_t have different sizes, so
   fts and fts64 have to be different functions.  */
#include <io/fts64.c>
