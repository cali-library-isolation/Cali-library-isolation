/* Jump to the position specified by ENV, causing the
   setjmp call there to return VAL, or 1 if VAL is 0.
   void __longjmp (__jmp_buf env, int val).  */
      
#warning longjmp_chk unimplemented
#define __longjmp ____longjmp_chk
#include <__longjmp.c>
