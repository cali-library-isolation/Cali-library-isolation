#ifndef LIBIPC_CONSTANTS_H
#define LIBIPC_CONSTANTS_H


//#define NO_FORK_MODE
//#define NO_SHM_MODE
//#define MALLOC_TESTER
//#define MALLOC_TESTER_INTERVALCHECK
//#define NO_NSJAIL_MODE

int use_nsjail(void) asm ("__use_nsjail");

__attribute__((noinline))
void __warning_breakpoint(void) asm("__warning_breakpoint");


#endif //LIBIPC_CONSTANTS_H
