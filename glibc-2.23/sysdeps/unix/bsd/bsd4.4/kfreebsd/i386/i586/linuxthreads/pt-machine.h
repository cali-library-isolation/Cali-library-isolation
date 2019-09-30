
/* i486+ is almost same as plain i386 version */
#include_next <pt-machine.h>

/* but compare-and-swap is always available  */

#undef TEST_FOR_COMPARE_AND_SWAP

