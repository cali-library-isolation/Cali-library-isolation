/* Since 'struct dirent64' == 'struct dirent', the functions 'getdirentries64'
   and 'getdirentries' differ only in the type of the BASEP argument.  */
#define GETDIRENTRIES getdirentries64
#define OFF_T off64_t
#include <getdirentries.c>
