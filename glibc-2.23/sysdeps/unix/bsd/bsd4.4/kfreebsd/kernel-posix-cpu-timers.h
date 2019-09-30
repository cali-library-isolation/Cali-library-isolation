/* Parameters for the FreeBSD kernel ABI for CPU clocks.  */

#define CPUCLOCK_WHICH(clock)   ((clock) & (clockid_t) CPUCLOCK_CLOCK_MASK)
#define CPUCLOCK_CLOCK_MASK     15

