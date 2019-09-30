/* The sigreturn syscall cannot be explicitly called on FreeBSD, only
   implicitly by returning from a signal handler.  */
#include <signal/sigreturn.c>
