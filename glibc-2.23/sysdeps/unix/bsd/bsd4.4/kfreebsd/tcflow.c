/* just use internal functions */
#define tcgetattr __tcgetattr
#define write __write
#include <sysdeps/unix/bsd/tcflow.c>
