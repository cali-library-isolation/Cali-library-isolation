#include <alloca.h>
int
__libc_alloca_cutoff (size_t size)
{
	return size <= 4096;
}
libc_hidden_def (__libc_alloca_cutoff)
