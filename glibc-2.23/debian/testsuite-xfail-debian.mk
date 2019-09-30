######################################################################
# All architectures
######################################################################
# These can fail when running with other loads or in a virtual machine
test-xfail-tst-cpuclock2 = yes
test-xfail-tst-cputimer1 = yes
test-xfail-tst-timer = yes

# This is an upstream regression on multiple architectures
# see https://sourceware.org/bugzilla/show_bug.cgi?id=19303
test-xfail-tst-cancel24-static = yes


######################################################################
# alpha (including optimized flavours)
######################################################################
ifneq (,$(filter $(config-machine)-$(config-os), alpha-linux-gnu alphaev67-linux-gnu))
test-xfail-tst-backtrace5 = yes
test-xfail-tst-backtrace6 = yes
test-xfail-tst-cancel19 = yes
test-xfail-check-localplt = yes
test-xfail-test-double = yes
test-xfail-test-float = yes
test-xfail-test-fenv-return = yes
test-xfail-test-snan = yes
test-xfail-tst-eintr1 = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-prelink-cmp = yes
test-xfail-tst-signal3 = yes
test-xfail-tst-timer4 = yes
test-xfail-tst-waitid = yes
test-xfail-tst-writev = yes
test-xfail-XOPEN2K8/netdb.h/conform = yes
test-xfail-XOPEN2K/netdb.h/conform = yes
endif


######################################################################
# amd64
######################################################################
ifeq ($(config-machine)-$(config-os),x86_64-linux-gnu)
test-xfail-tst-backtrace6 = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-waitid = yes
endif


######################################################################
# arm64
######################################################################
ifeq ($(config-machine)-$(config-os),aarch64-linux-gnu)
test-xfail-test-fenv = yes
test-xfail-test-fenv-tls = yes
test-xfail-tst-backtrace5 = yes
test-xfail-tst-backtrace6 = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-stack4 = yes

# This can be removed once bug#750834 is fixed
test-xfail-basic-test = yes
endif


######################################################################
# armel
######################################################################
ifeq ($(config-machine)-$(config-os),arm-linux-gnueabi)
test-xfail-test-misc = yes
test-xfail-tst-backtrace6 = yes
test-xfail-tst-makecontext = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-signal6 = yes
test-xfail-tst-stack4 = yes
test-xfail-tst-waitid = yes

# This can be removed once bug#748616 is fixed
test-xfail-basic-test = yes
endif


######################################################################
# armhf
######################################################################
ifeq ($(config-machine)-$(config-os),arm-linux-gnueabihf)
test-xfail-test-fenv = yes
test-xfail-tst-backtrace6 = yes
test-xfail-tst-makecontext = yes
test-xfail-tst-signal6 = yes
test-xfail-tst-stack4 = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-waitid = yes
endif


######################################################################
# hppa
######################################################################
ifeq ($(config-machine)-$(config-os),hppa-linux-gnu)
test-xfail-POSIX/aio.h/conform = yes
test-xfail-POSIX/mqueue.h/conform = yes
test-xfail-POSIX/pthread.h/conform = yes
test-xfail-POSIX/regex.h/conform = yes
test-xfail-POSIX/semaphore.h/conform = yes
test-xfail-POSIX/signal.h/conform = yes
test-xfail-POSIX/sys/mman.h/conform = yes
test-xfail-POSIX/sys/types.h/conform = yes
test-xfail-POSIX2008/aio.h/conform = yes
test-xfail-POSIX2008/mqueue.h/conform = yes
test-xfail-POSIX2008/pthread.h/conform = yes
test-xfail-POSIX2008/regex.h/conform = yes
test-xfail-POSIX2008/semaphore.h/conform = yes
test-xfail-POSIX2008/spawn.h/conform = yes
test-xfail-POSIX2008/sys/mman.h/conform = yes
test-xfail-POSIX2008/sys/types.h/conform = yes
test-xfail-UNIX98/aio.h/conform = yes
test-xfail-UNIX98/ftw.h/conform = yes
test-xfail-UNIX98/mqueue.h/conform = yes
test-xfail-UNIX98/pthread.h/conform = yes
test-xfail-UNIX98/regex.h/conform = yes
test-xfail-UNIX98/semaphore.h/conform = yes
test-xfail-UNIX98/sys/mman.h/conform = yes
test-xfail-UNIX98/sys/types.h/conform = yes
test-xfail-UNIX98/sys/uio.h/conform = yes
test-xfail-XOPEN2K/ftw.h/conform = yes
test-xfail-XOPEN2K/pthread.h/conform = yes
test-xfail-XOPEN2K/regex.h/conform = yes
test-xfail-XOPEN2K/spawn.h/conform = yes
test-xfail-XOPEN2K/stdlib.h/conform = yes
test-xfail-XOPEN2K/sys/mman.h/conform = yes
test-xfail-XOPEN2K/sys/types.h/conform = yes
test-xfail-XOPEN2K/sys/uio.h/conform = yes
test-xfail-XOPEN2K8/aio.h/conform = yes
test-xfail-XOPEN2K8/ftw.h/conform = yes
test-xfail-XOPEN2K8/mqueue.h/conform = yes
test-xfail-XOPEN2K8/pthread.h/conform = yes
test-xfail-XOPEN2K8/regex.h/conform = yes
test-xfail-XOPEN2K8/semaphore.h/conform = yes
test-xfail-XOPEN2K8/spawn.h/conform = yes
test-xfail-XOPEN2K8/stdlib.h/conform = yes
test-xfail-XOPEN2K8/sys/mman.h/conform = yes
test-xfail-XOPEN2K8/sys/types.h/conform = yes
test-xfail-XOPEN2K8/sys/uio.h/conform = yes
test-xfail-XPG4/sys/mman.h/conform = yes
test-xfail-check-execstack = yes
test-xfail-check-localplt = yes
test-xfail-check-textrel = yes
test-xfail-malloc/tst-trim1 = yes
test-xfail-test-double = yes
test-xfail-test-float = yes
test-xfail-test-idouble = yes
test-xfail-test-ifloat = yes
test-xfail-tst-addr1 = yes
test-xfail-tst-attr3 = yes
test-xfail-tst-audit2 = yes
test-xfail-tst-backtrace2 = yes
test-xfail-tst-backtrace3 = yes
test-xfail-tst-backtrace4 = yes
test-xfail-tst-backtrace5 = yes
test-xfail-tst-backtrace6 = yes
test-xfail-tst-cancel3 = yes
test-xfail-tst-cancel4 = yes
test-xfail-tst-cancel5 = yes
test-xfail-tst-cancelx20 = yes
test-xfail-tst-cancelx21 = yes
test-xfail-tst-cancelx4 = yes
test-xfail-tst-cancelx5 = yes
test-xfail-tst-cleanupx4 = yes
test-xfail-tst-cputimer2 = yes
test-xfail-tst-cputimer3 = yes
test-xfail-tst-longjmp_chk2 = yes
test-xfail-tst-makecontext = yes
test-xfail-tst-mqueue8 = yes
test-xfail-tst-mqueue8x = yes
test-xfail-tst-protected1a = yes
test-xfail-tst-protected1b = yes
test-xfail-tst-setcontext2 = yes
test-xfail-tst-setcontext3 = yes
test-xfail-tst-stack1 = yes
test-xfail-tst-stack4 = yes
test-xfail-tst-trim1 = yes
endif


######################################################################
# hurd-i386 (including optimized flavours)
######################################################################
ifneq (,$(filter $(config-machine)-$(config-os), i586-gnu-gnu-gnu i686-gnu-gnu-gnu))
# sysdeps/mach/hurd/dl-sysdep.c's open_file does not support the linker
# creating files.
test-xfail-tst-null-argv = yes

# Tests failing to build
tests-unsupported += test-as-const-signal-defines
tests-unsupported += tst-cputimer1
tests-unsupported += tst-cputimer2
tests-unsupported += tst-cputimer3
tests-unsupported += tst-cputimer4
tests-unsupported += tst-timer4
tests-unsupported += tst-timer5
tests-unsupported += tst-malloc-backtrace

# bounding memory allocation is not supported yet
tests-unsupported += tst-malloc-thread-fail

# Need actual porting
test-xfail-exe = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-pselect = yes
test-xfail-tst-ptrguard1-static = yes
test-xfail-tst-ptrguard1 = yes
test-xfail-tst-secure-getenv = yes
test-xfail-tst-timer2 = yes

# We don't have auxv yet
test-xfail-tst-auxv = yes

# check-c++-types.sh support will be fixed in hurd (>> 1:0.7.git20160316-1)
test-xfail-c++-types-check = yes

# We don't provide ABI reference yet
test-xfail-check-abi-libcrypt = yes
test-xfail-check-abi-libdl = yes
test-xfail-check-abi-ld = yes
test-xfail-check-abi-libc = yes
test-xfail-check-abi-libhurduser = yes
test-xfail-check-abi-libpthread = yes
test-xfail-check-abi-libBrokenLocale = yes
test-xfail-check-abi-libutil = yes
test-xfail-check-abi-libmachuser = yes
test-xfail-check-abi-libm = yes
test-xfail-check-abi-libnsl = yes
test-xfail-check-abi-libanl = yes
test-xfail-check-abi-libresolv = yes
test-xfail-check-abi-librt = yes

# Overzealous test
test-xfail-tst-pathconf = yes

# Need investigation
test-xfail-bug-regex31-mem = yes
test-xfail-check-execstack = yes
test-xfail-check-localplt = yes
test-xfail-check-textrel = yes
test-xfail-opendir-tst1 = yes
test-xfail-test-idouble = yes
test-xfail-test-ifloat = yes
test-xfail-tst-aio10 = yes
test-xfail-tst-aio9 = yes
test-xfail-tst-audit1 = yes
test-xfail-tst-audit2 = yes
test-xfail-tst-audit8 = yes
test-xfail-tst-backtrace4 = yes
test-xfail-tst-backtrace5 = yes
test-xfail-tst-chk2 = yes
test-xfail-tst-chk3 = yes
test-xfail-tst-chk4 = yes
test-xfail-tst-chk5 = yes
test-xfail-tst-chk6 = yes
test-xfail-tst-error1-mem = yes
test-xfail-tst-fdopendir2 = yes
test-xfail-tst-fdopendir = yes
test-xfail-tst-fnmatch-mem = yes
test-xfail-tst-futimesat = yes
test-xfail-tst-getconf = yes
test-xfail-tst-grantpt = yes
test-xfail-tst-lfschk2 = yes
test-xfail-tst-lfschk3 = yes
test-xfail-tst-lfschk4 = yes
test-xfail-tst-lfschk5 = yes
test-xfail-tst-lfschk6 = yes
test-xfail-tst-longjmp_chk2 = yes
test-xfail-tst-sprofil = yes
test-xfail-tst-stackguard1-static = yes
test-xfail-tst-stackguard1 = yes
test-xfail-tststatic2 = yes
test-xfail-tststatic3 = yes
test-xfail-tststatic4 = yes
test-xfail-tststatic5 = yes
test-xfail-tststatic = yes
test-xfail-tst-strtod-round = yes
test-xfail-tst-sysconf = yes
test-xfail-tst-tls9-static = yes
test-xfail-tst-vfork3-mem = yes

# This generates GiBs of data instead of sparse files, putting build box on its knees
tests-unsupported += test-lfs
#test-xfail-test-lfs = yes

# happens seldomly
test-xfail-tst-clock_nanosleep = yes
test-xfail-tst-nanosleep = yes

# new in 2.21
test-xfail-bug-atexit3 = yes
test-xfail-test-misc = yes
test-xfail-tst-ptsname = yes
test-xfail-tst-audit9 = yes

test-xfail-POSIX2008/errno.h/conform = yes
test-xfail-POSIX2008/limits.h/conform = yes
test-xfail-POSIX2008/mqueue.h/conform = yes
test-xfail-POSIX2008/pthread.h/conform = yes
test-xfail-POSIX2008/semaphore.h/conform = yes
test-xfail-POSIX2008/sys/stat.h/conform = yes
test-xfail-POSIX2008/sys/statvfs.h/conform = yes
test-xfail-POSIX2008/sys/un.h/conform = yes
test-xfail-POSIX2008/termios.h/conform = yes
test-xfail-POSIX/fcntl.h/conform = yes
test-xfail-POSIX/limits.h/conform = yes
test-xfail-POSIX/mqueue.h/conform = yes
test-xfail-POSIX/pthread.h/conform = yes
test-xfail-POSIX/sched.h/conform = yes
test-xfail-POSIX/semaphore.h/conform = yes
test-xfail-POSIX/sys/stat.h/conform = yes
test-xfail-POSIX/time.h/conform = yes
test-xfail-UNIX98/limits.h/conform = yes
test-xfail-UNIX98/mqueue.h/conform = yes
test-xfail-UNIX98/pthread.h/conform = yes
test-xfail-UNIX98/sched.h/conform = yes
test-xfail-UNIX98/semaphore.h/conform = yes
test-xfail-UNIX98/sys/stat.h/conform = yes
test-xfail-UNIX98/termios.h/conform = yes
test-xfail-UNIX98/time.h/conform = yes
test-xfail-XOPEN2K8/errno.h/conform = yes
test-xfail-XOPEN2K8/mqueue.h/conform = yes
test-xfail-XOPEN2K8/pthread.h/conform = yes
test-xfail-XOPEN2K8/semaphore.h/conform = yes
test-xfail-XOPEN2K8/sys/stat.h/conform = yes
test-xfail-XOPEN2K/limits.h/conform = yes
test-xfail-XOPEN2K/pthread.h/conform = yes
test-xfail-XOPEN2K/sys/stat.h/conform = yes
test-xfail-XPG4/limits.h/conform = yes
test-xfail-XPG4/sys/resource.h/conform = yes
test-xfail-XPG4/sys/statvfs.h/conform = yes
test-xfail-XPG4/sys/un.h/conform = yes
test-xfail-XPG4/time.h/conform = yes

test-xfail-ISO11/assert.h/linknamespace = yes
test-xfail-ISO11/complex.h/linknamespace = yes
test-xfail-ISO11/fenv.h/linknamespace = yes
test-xfail-ISO11/locale.h/linknamespace = yes
test-xfail-ISO11/math.h/linknamespace = yes
test-xfail-ISO11/setjmp.h/linknamespace = yes
test-xfail-ISO11/signal.h/linknamespace = yes
test-xfail-ISO11/stdio.h/linknamespace = yes
test-xfail-ISO11/stdlib.h/linknamespace = yes
test-xfail-ISO11/string.h/linknamespace = yes
test-xfail-ISO11/tgmath.h/linknamespace = yes
test-xfail-ISO11/time.h/linknamespace = yes
test-xfail-ISO11/uchar.h/linknamespace = yes
test-xfail-ISO11/wchar.h/linknamespace = yes
test-xfail-ISO99/assert.h/linknamespace = yes
test-xfail-ISO99/complex.h/linknamespace = yes
test-xfail-ISO99/fenv.h/linknamespace = yes
test-xfail-ISO99/locale.h/linknamespace = yes
test-xfail-ISO99/math.h/linknamespace = yes
test-xfail-ISO99/setjmp.h/linknamespace = yes
test-xfail-ISO99/signal.h/linknamespace = yes
test-xfail-ISO99/stdio.h/linknamespace = yes
test-xfail-ISO99/stdlib.h/linknamespace = yes
test-xfail-ISO99/string.h/linknamespace = yes
test-xfail-ISO99/tgmath.h/linknamespace = yes
test-xfail-ISO99/time.h/linknamespace = yes
test-xfail-ISO99/wchar.h/linknamespace = yes
test-xfail-ISO/assert.h/linknamespace = yes
test-xfail-ISO/locale.h/linknamespace = yes
test-xfail-ISO/math.h/linknamespace = yes
test-xfail-ISO/setjmp.h/linknamespace = yes
test-xfail-ISO/signal.h/linknamespace = yes
test-xfail-ISO/stdio.h/linknamespace = yes
test-xfail-ISO/stdlib.h/linknamespace = yes
test-xfail-ISO/string.h/linknamespace = yes
test-xfail-ISO/time.h/linknamespace = yes
test-xfail-POSIX2008/aio.h/linknamespace = yes
test-xfail-POSIX2008/arpa/inet.h/linknamespace = yes
test-xfail-POSIX2008/assert.h/linknamespace = yes
test-xfail-POSIX2008/complex.h/linknamespace = yes
test-xfail-POSIX2008/fcntl.h/linknamespace = yes
test-xfail-POSIX2008/fenv.h/linknamespace = yes
test-xfail-POSIX2008/fnmatch.h/linknamespace = yes
test-xfail-POSIX2008/glob.h/linknamespace = yes
test-xfail-POSIX2008/iconv.h/linknamespace = yes
test-xfail-POSIX2008/langinfo.h/linknamespace = yes
test-xfail-POSIX2008/locale.h/linknamespace = yes
test-xfail-POSIX2008/math.h/linknamespace = yes
test-xfail-POSIX2008/monetary.h/linknamespace = yes
test-xfail-POSIX2008/mqueue.h/linknamespace = yes
test-xfail-POSIX2008/net/if.h/linknamespace = yes
test-xfail-POSIX2008/netinet/in.h/linknamespace = yes
test-xfail-POSIX2008/nl_types.h/linknamespace = yes
test-xfail-POSIX2008/poll.h/linknamespace = yes
test-xfail-POSIX2008/pthread.h/linknamespace = yes
test-xfail-POSIX2008/pwd.h/linknamespace = yes
test-xfail-POSIX2008/sched.h/linknamespace = yes
test-xfail-POSIX2008/setjmp.h/linknamespace = yes
test-xfail-POSIX2008/signal.h/linknamespace = yes
test-xfail-POSIX2008/stdio.h/linknamespace = yes
test-xfail-POSIX2008/stdlib.h/linknamespace = yes
test-xfail-POSIX2008/string.h/linknamespace = yes
test-xfail-POSIX2008/stropts.h/linknamespace = yes
test-xfail-POSIX2008/sys/mman.h/linknamespace = yes
test-xfail-POSIX2008/sys/select.h/linknamespace = yes
test-xfail-POSIX2008/sys/socket.h/linknamespace = yes
test-xfail-POSIX2008/sys/stat.h/linknamespace = yes
test-xfail-POSIX2008/sys/statvfs.h/linknamespace = yes
test-xfail-POSIX2008/sys/times.h/linknamespace = yes
test-xfail-POSIX2008/sys/utsname.h/linknamespace = yes
test-xfail-POSIX2008/sys/wait.h/linknamespace = yes
test-xfail-POSIX2008/termios.h/linknamespace = yes
test-xfail-POSIX2008/tgmath.h/linknamespace = yes
test-xfail-POSIX2008/time.h/linknamespace = yes
test-xfail-POSIX2008/utime.h/linknamespace = yes
test-xfail-POSIX2008/wchar.h/linknamespace = yes
test-xfail-POSIX2008/wordexp.h/linknamespace = yes
test-xfail-POSIX/assert.h/linknamespace = yes
test-xfail-POSIX/dirent.h/linknamespace = yes
test-xfail-POSIX/fcntl.h/linknamespace = yes
test-xfail-POSIX/grp.h/linknamespace = yes
test-xfail-POSIX/locale.h/linknamespace = yes
test-xfail-POSIX/math.h/linknamespace = yes
test-xfail-POSIX/pthread.h/linknamespace = yes
test-xfail-POSIX/pwd.h/linknamespace = yes
test-xfail-POSIX/sched.h/linknamespace = yes
test-xfail-POSIX/setjmp.h/linknamespace = yes
test-xfail-POSIX/signal.h/linknamespace = yes
test-xfail-POSIX/stdio.h/linknamespace = yes
test-xfail-POSIX/stdlib.h/linknamespace = yes
test-xfail-POSIX/string.h/linknamespace = yes
test-xfail-POSIX/sys/mman.h/linknamespace = yes
test-xfail-POSIX/sys/stat.h/linknamespace = yes
test-xfail-POSIX/sys/times.h/linknamespace = yes
test-xfail-POSIX/sys/utsname.h/linknamespace = yes
test-xfail-POSIX/sys/wait.h/linknamespace = yes
test-xfail-POSIX/termios.h/linknamespace = yes
test-xfail-POSIX/time.h/linknamespace = yes
test-xfail-POSIX/utime.h/linknamespace = yes
test-xfail-UNIX98/aio.h/linknamespace = yes
test-xfail-UNIX98/arpa/inet.h/linknamespace = yes
test-xfail-UNIX98/assert.h/linknamespace = yes
test-xfail-UNIX98/dirent.h/linknamespace = yes
test-xfail-UNIX98/fcntl.h/linknamespace = yes
test-xfail-UNIX98/ftw.h/linknamespace = yes
test-xfail-UNIX98/grp.h/linknamespace = yes
test-xfail-UNIX98/iconv.h/linknamespace = yes
test-xfail-UNIX98/langinfo.h/linknamespace = yes
test-xfail-UNIX98/locale.h/linknamespace = yes
test-xfail-UNIX98/math.h/linknamespace = yes
test-xfail-UNIX98/monetary.h/linknamespace = yes
test-xfail-UNIX98/netinet/in.h/linknamespace = yes
test-xfail-UNIX98/nl_types.h/linknamespace = yes
test-xfail-UNIX98/poll.h/linknamespace = yes
test-xfail-UNIX98/pthread.h/linknamespace = yes
test-xfail-UNIX98/pwd.h/linknamespace = yes
test-xfail-UNIX98/sched.h/linknamespace = yes
test-xfail-UNIX98/semaphore.h/linknamespace = yes
test-xfail-UNIX98/setjmp.h/linknamespace = yes
test-xfail-UNIX98/signal.h/linknamespace = yes
test-xfail-UNIX98/string.h/linknamespace = yes
test-xfail-UNIX98/stropts.h/linknamespace = yes
test-xfail-UNIX98/sys/ipc.h/linknamespace = yes
test-xfail-UNIX98/sys/mman.h/linknamespace = yes
test-xfail-UNIX98/sys/msg.h/linknamespace = yes
test-xfail-UNIX98/sys/resource.h/linknamespace = yes
test-xfail-UNIX98/sys/sem.h/linknamespace = yes
test-xfail-UNIX98/sys/shm.h/linknamespace = yes
test-xfail-UNIX98/sys/socket.h/linknamespace = yes
test-xfail-UNIX98/sys/stat.h/linknamespace = yes
test-xfail-UNIX98/sys/statvfs.h/linknamespace = yes
test-xfail-UNIX98/sys/time.h/linknamespace = yes
test-xfail-UNIX98/sys/times.h/linknamespace = yes
test-xfail-UNIX98/sys/uio.h/linknamespace = yes
test-xfail-UNIX98/sys/utsname.h/linknamespace = yes
test-xfail-UNIX98/sys/wait.h/linknamespace = yes
test-xfail-UNIX98/termios.h/linknamespace = yes
test-xfail-UNIX98/time.h/linknamespace = yes
test-xfail-UNIX98/ucontext.h/linknamespace = yes
test-xfail-UNIX98/ulimit.h/linknamespace = yes
test-xfail-UNIX98/utime.h/linknamespace = yes
test-xfail-UNIX98/utmpx.h/linknamespace = yes
test-xfail-XOPEN2K8/aio.h/linknamespace = yes
test-xfail-XOPEN2K8/arpa/inet.h/linknamespace = yes
test-xfail-XOPEN2K8/assert.h/linknamespace = yes
test-xfail-XOPEN2K8/complex.h/linknamespace = yes
test-xfail-XOPEN2K8/fcntl.h/linknamespace = yes
test-xfail-XOPEN2K8/fenv.h/linknamespace = yes
test-xfail-XOPEN2K8/fnmatch.h/linknamespace = yes
test-xfail-XOPEN2K8/ftw.h/linknamespace = yes
test-xfail-XOPEN2K8/glob.h/linknamespace = yes
test-xfail-XOPEN2K8/iconv.h/linknamespace = yes
test-xfail-XOPEN2K8/langinfo.h/linknamespace = yes
test-xfail-XOPEN2K8/locale.h/linknamespace = yes
test-xfail-XOPEN2K8/math.h/linknamespace = yes
test-xfail-XOPEN2K8/monetary.h/linknamespace = yes
test-xfail-XOPEN2K8/mqueue.h/linknamespace = yes
test-xfail-XOPEN2K8/net/if.h/linknamespace = yes
test-xfail-XOPEN2K8/netinet/in.h/linknamespace = yes
test-xfail-XOPEN2K8/nl_types.h/linknamespace = yes
test-xfail-XOPEN2K8/poll.h/linknamespace = yes
test-xfail-XOPEN2K8/pthread.h/linknamespace = yes
test-xfail-XOPEN2K8/sched.h/linknamespace = yes
test-xfail-XOPEN2K8/semaphore.h/linknamespace = yes
test-xfail-XOPEN2K8/setjmp.h/linknamespace = yes
test-xfail-XOPEN2K8/signal.h/linknamespace = yes
test-xfail-XOPEN2K8/stdio.h/linknamespace = yes
test-xfail-XOPEN2K8/string.h/linknamespace = yes
test-xfail-XOPEN2K8/stropts.h/linknamespace = yes
test-xfail-XOPEN2K8/sys/ipc.h/linknamespace = yes
test-xfail-XOPEN2K8/sys/mman.h/linknamespace = yes
test-xfail-XOPEN2K8/sys/msg.h/linknamespace = yes
test-xfail-XOPEN2K8/sys/resource.h/linknamespace = yes
test-xfail-XOPEN2K8/sys/select.h/linknamespace = yes
test-xfail-XOPEN2K8/sys/sem.h/linknamespace = yes
test-xfail-XOPEN2K8/sys/shm.h/linknamespace = yes
test-xfail-XOPEN2K8/sys/socket.h/linknamespace = yes
test-xfail-XOPEN2K8/sys/stat.h/linknamespace = yes
test-xfail-XOPEN2K8/sys/statvfs.h/linknamespace = yes
test-xfail-XOPEN2K8/sys/time.h/linknamespace = yes
test-xfail-XOPEN2K8/sys/times.h/linknamespace = yes
test-xfail-XOPEN2K8/sys/uio.h/linknamespace = yes
test-xfail-XOPEN2K8/sys/utsname.h/linknamespace = yes
test-xfail-XOPEN2K8/sys/wait.h/linknamespace = yes
test-xfail-XOPEN2K8/termios.h/linknamespace = yes
test-xfail-XOPEN2K8/tgmath.h/linknamespace = yes
test-xfail-XOPEN2K8/time.h/linknamespace = yes
test-xfail-XOPEN2K8/ulimit.h/linknamespace = yes
test-xfail-XOPEN2K8/utime.h/linknamespace = yes
test-xfail-XOPEN2K8/utmpx.h/linknamespace = yes
test-xfail-XOPEN2K8/wchar.h/linknamespace = yes
test-xfail-XOPEN2K8/wordexp.h/linknamespace = yes
test-xfail-XOPEN2K/aio.h/linknamespace = yes
test-xfail-XOPEN2K/arpa/inet.h/linknamespace = yes
test-xfail-XOPEN2K/assert.h/linknamespace = yes
test-xfail-XOPEN2K/complex.h/linknamespace = yes
test-xfail-XOPEN2K/dirent.h/linknamespace = yes
test-xfail-XOPEN2K/fcntl.h/linknamespace = yes
test-xfail-XOPEN2K/fenv.h/linknamespace = yes
test-xfail-XOPEN2K/ftw.h/linknamespace = yes
test-xfail-XOPEN2K/grp.h/linknamespace = yes
test-xfail-XOPEN2K/iconv.h/linknamespace = yes
test-xfail-XOPEN2K/langinfo.h/linknamespace = yes
test-xfail-XOPEN2K/locale.h/linknamespace = yes
test-xfail-XOPEN2K/math.h/linknamespace = yes
test-xfail-XOPEN2K/monetary.h/linknamespace = yes
test-xfail-XOPEN2K/mqueue.h/linknamespace = yes
test-xfail-XOPEN2K/net/if.h/linknamespace = yes
test-xfail-XOPEN2K/netinet/in.h/linknamespace = yes
test-xfail-XOPEN2K/nl_types.h/linknamespace = yes
test-xfail-XOPEN2K/poll.h/linknamespace = yes
test-xfail-XOPEN2K/pthread.h/linknamespace = yes
test-xfail-XOPEN2K/pwd.h/linknamespace = yes
test-xfail-XOPEN2K/sched.h/linknamespace = yes
test-xfail-XOPEN2K/semaphore.h/linknamespace = yes
test-xfail-XOPEN2K/setjmp.h/linknamespace = yes
test-xfail-XOPEN2K/stdio.h/linknamespace = yes
test-xfail-XOPEN2K/string.h/linknamespace = yes
test-xfail-XOPEN2K/stropts.h/linknamespace = yes
test-xfail-XOPEN2K/sys/ipc.h/linknamespace = yes
test-xfail-XOPEN2K/sys/mman.h/linknamespace = yes
test-xfail-XOPEN2K/sys/msg.h/linknamespace = yes
test-xfail-XOPEN2K/sys/resource.h/linknamespace = yes
test-xfail-XOPEN2K/sys/select.h/linknamespace = yes
test-xfail-XOPEN2K/sys/sem.h/linknamespace = yes
test-xfail-XOPEN2K/sys/shm.h/linknamespace = yes
test-xfail-XOPEN2K/sys/socket.h/linknamespace = yes
test-xfail-XOPEN2K/sys/stat.h/linknamespace = yes
test-xfail-XOPEN2K/sys/statvfs.h/linknamespace = yes
test-xfail-XOPEN2K/sys/time.h/linknamespace = yes
test-xfail-XOPEN2K/sys/times.h/linknamespace = yes
test-xfail-XOPEN2K/sys/uio.h/linknamespace = yes
test-xfail-XOPEN2K/sys/utsname.h/linknamespace = yes
test-xfail-XOPEN2K/termios.h/linknamespace = yes
test-xfail-XOPEN2K/tgmath.h/linknamespace = yes
test-xfail-XOPEN2K/time.h/linknamespace = yes
test-xfail-XOPEN2K/ulimit.h/linknamespace = yes
test-xfail-XOPEN2K/utime.h/linknamespace = yes
test-xfail-XOPEN2K/utmpx.h/linknamespace = yes
test-xfail-XOPEN2K/wchar.h/linknamespace = yes
test-xfail-XPG3/assert.h/linknamespace = yes
test-xfail-XPG3/dirent.h/linknamespace = yes
test-xfail-XPG3/fcntl.h/linknamespace = yes
test-xfail-XPG3/ftw.h/linknamespace = yes
test-xfail-XPG3/grp.h/linknamespace = yes
test-xfail-XPG3/iconv.h/linknamespace = yes
test-xfail-XPG3/langinfo.h/linknamespace = yes
test-xfail-XPG3/locale.h/linknamespace = yes
test-xfail-XPG3/math.h/linknamespace = yes
test-xfail-XPG3/monetary.h/linknamespace = yes
test-xfail-XPG3/nl_types.h/linknamespace = yes
test-xfail-XPG3/pwd.h/linknamespace = yes
test-xfail-XPG3/setjmp.h/linknamespace = yes
test-xfail-XPG3/signal.h/linknamespace = yes
test-xfail-XPG3/string.h/linknamespace = yes
test-xfail-XPG3/sys/ipc.h/linknamespace = yes
test-xfail-XPG3/sys/msg.h/linknamespace = yes
test-xfail-XPG3/sys/sem.h/linknamespace = yes
test-xfail-XPG3/sys/shm.h/linknamespace = yes
test-xfail-XPG3/sys/stat.h/linknamespace = yes
test-xfail-XPG3/sys/times.h/linknamespace = yes
test-xfail-XPG3/sys/utsname.h/linknamespace = yes
test-xfail-XPG3/sys/wait.h/linknamespace = yes
test-xfail-XPG3/termios.h/linknamespace = yes
test-xfail-XPG3/time.h/linknamespace = yes
test-xfail-XPG3/ulimit.h/linknamespace = yes
test-xfail-XPG3/utime.h/linknamespace = yes
test-xfail-XPG4/arpa/inet.h/linknamespace = yes
test-xfail-XPG4/assert.h/linknamespace = yes
test-xfail-XPG4/dirent.h/linknamespace = yes
test-xfail-XPG4/fcntl.h/linknamespace = yes
test-xfail-XPG4/ftw.h/linknamespace = yes
test-xfail-XPG4/grp.h/linknamespace = yes
test-xfail-XPG4/iconv.h/linknamespace = yes
test-xfail-XPG4/langinfo.h/linknamespace = yes
test-xfail-XPG4/locale.h/linknamespace = yes
test-xfail-XPG4/math.h/linknamespace = yes
test-xfail-XPG4/monetary.h/linknamespace = yes
test-xfail-XPG4/netinet/in.h/linknamespace = yes
test-xfail-XPG4/nl_types.h/linknamespace = yes
test-xfail-XPG4/poll.h/linknamespace = yes
test-xfail-XPG4/pwd.h/linknamespace = yes
test-xfail-XPG4/setjmp.h/linknamespace = yes
test-xfail-XPG4/signal.h/linknamespace = yes
test-xfail-XPG4/string.h/linknamespace = yes
test-xfail-XPG4/stropts.h/linknamespace = yes
test-xfail-XPG4/sys/ipc.h/linknamespace = yes
test-xfail-XPG4/sys/mman.h/linknamespace = yes
test-xfail-XPG4/sys/msg.h/linknamespace = yes
test-xfail-XPG4/sys/resource.h/linknamespace = yes
test-xfail-XPG4/sys/sem.h/linknamespace = yes
test-xfail-XPG4/sys/shm.h/linknamespace = yes
test-xfail-XPG4/sys/socket.h/linknamespace = yes
test-xfail-XPG4/sys/stat.h/linknamespace = yes
test-xfail-XPG4/sys/statvfs.h/linknamespace = yes
test-xfail-XPG4/sys/time.h/linknamespace = yes
test-xfail-XPG4/sys/times.h/linknamespace = yes
test-xfail-XPG4/sys/uio.h/linknamespace = yes
test-xfail-XPG4/sys/utsname.h/linknamespace = yes
test-xfail-XPG4/sys/wait.h/linknamespace = yes
test-xfail-XPG4/termios.h/linknamespace = yes
test-xfail-XPG4/time.h/linknamespace = yes
test-xfail-XPG4/ucontext.h/linknamespace = yes
test-xfail-XPG4/ulimit.h/linknamespace = yes
test-xfail-XPG4/utime.h/linknamespace = yes
test-xfail-XPG4/utmpx.h/linknamespace = yes

# new in 2.22
test-xfail-tst-audit3 = yes
test-xfail-tst-dlmodcount = yes
test-xfail-tst-nodelete2 = yes
test-xfail-tst-prelink = yes
test-xfail-tst-tls-atexit = yes

test-xfail-POSIX2008/fcntl.h/conform = yes
test-xfail-UNIX98/sys/ipc.h/conform = yes
test-xfail-UNIX98/sys/msg.h/conform = yes
test-xfail-UNIX98/sys/resource.h/conform = yes
test-xfail-UNIX98/sys/shm.h/conform = yes
test-xfail-UNIX98/sys/socket.h/conform = yes
test-xfail-UNIX98/sys/statvfs.h/conform = yes
test-xfail-UNIX98/sys/un.h/conform = yes
test-xfail-XOPEN2K8/arpa/inet.h/conform = yes
test-xfail-XOPEN2K8/fcntl.h/conform = yes
test-xfail-XOPEN2K8/netdb.h/conform = yes
test-xfail-XOPEN2K8/netinet/in.h/conform = yes
test-xfail-XOPEN2K8/sys/ipc.h/conform = yes
test-xfail-XOPEN2K8/sys/msg.h/conform = yes
test-xfail-XOPEN2K8/sys/resource.h/conform = yes
test-xfail-XOPEN2K8/sys/shm.h/conform = yes
test-xfail-XOPEN2K8/sys/socket.h/conform = yes
test-xfail-XOPEN2K8/sys/statvfs.h/conform = yes
test-xfail-XOPEN2K8/sys/un.h/conform = yes
test-xfail-XOPEN2K/arpa/inet.h/conform = yes
test-xfail-XOPEN2K/netdb.h/conform = yes
test-xfail-XOPEN2K/netinet/in.h/conform = yes
test-xfail-XOPEN2K/sys/ipc.h/conform = yes
test-xfail-XOPEN2K/sys/msg.h/conform = yes
test-xfail-XOPEN2K/sys/resource.h/conform = yes
test-xfail-XOPEN2K/sys/shm.h/conform = yes
test-xfail-XOPEN2K/sys/socket.h/conform = yes
test-xfail-XOPEN2K/sys/statvfs.h/conform = yes
test-xfail-XOPEN2K/sys/un.h/conform = yes
test-xfail-XPG3/sys/ipc.h/conform = yes
test-xfail-XPG3/sys/msg.h/conform = yes
test-xfail-XPG3/sys/shm.h/conform = yes
test-xfail-XPG4/sys/ipc.h/conform = yes
test-xfail-XPG4/sys/msg.h/conform = yes
test-xfail-XPG4/sys/shm.h/conform = yes
test-xfail-XPG4/sys/socket.h/conform = yes

test-xfail-POSIX2008/dirent.h/linknamespace = yes
test-xfail-POSIX2008/grp.h/linknamespace = yes
test-xfail-POSIX2008/netdb.h/linknamespace = yes
test-xfail-POSIX2008/regex.h/linknamespace = yes
test-xfail-POSIX2008/semaphore.h/linknamespace = yes
test-xfail-POSIX2008/spawn.h/linknamespace = yes
test-xfail-POSIX2008/unistd.h/linknamespace = yes
test-xfail-POSIX/aio.h/linknamespace = yes
test-xfail-POSIX/fnmatch.h/linknamespace = yes
test-xfail-POSIX/glob.h/linknamespace = yes
test-xfail-POSIX/mqueue.h/linknamespace = yes
test-xfail-POSIX/regex.h/linknamespace = yes
test-xfail-POSIX/semaphore.h/linknamespace = yes
test-xfail-POSIX/unistd.h/linknamespace = yes
test-xfail-POSIX/wordexp.h/linknamespace = yes
test-xfail-UNIX98/fmtmsg.h/linknamespace = yes
test-xfail-UNIX98/fnmatch.h/linknamespace = yes
test-xfail-UNIX98/glob.h/linknamespace = yes
test-xfail-UNIX98/mqueue.h/linknamespace = yes
test-xfail-UNIX98/netdb.h/linknamespace = yes
test-xfail-UNIX98/regex.h/linknamespace = yes
test-xfail-UNIX98/search.h/linknamespace = yes
test-xfail-UNIX98/stdio.h/linknamespace = yes
test-xfail-UNIX98/stdlib.h/linknamespace = yes
test-xfail-UNIX98/syslog.h/linknamespace = yes
test-xfail-UNIX98/sys/timeb.h/linknamespace = yes
test-xfail-UNIX98/unistd.h/linknamespace = yes
test-xfail-UNIX98/wchar.h/linknamespace = yes
test-xfail-UNIX98/wordexp.h/linknamespace = yes
test-xfail-XOPEN2K8/dirent.h/linknamespace = yes
test-xfail-XOPEN2K8/fmtmsg.h/linknamespace = yes
test-xfail-XOPEN2K8/grp.h/linknamespace = yes
test-xfail-XOPEN2K8/netdb.h/linknamespace = yes
test-xfail-XOPEN2K8/pwd.h/linknamespace = yes
test-xfail-XOPEN2K8/regex.h/linknamespace = yes
test-xfail-XOPEN2K8/search.h/linknamespace = yes
test-xfail-XOPEN2K8/spawn.h/linknamespace = yes
test-xfail-XOPEN2K8/stdlib.h/linknamespace = yes
test-xfail-XOPEN2K8/syslog.h/linknamespace = yes
test-xfail-XOPEN2K8/unistd.h/linknamespace = yes
test-xfail-XOPEN2K/fmtmsg.h/linknamespace = yes
test-xfail-XOPEN2K/fnmatch.h/linknamespace = yes
test-xfail-XOPEN2K/glob.h/linknamespace = yes
test-xfail-XOPEN2K/netdb.h/linknamespace = yes
test-xfail-XOPEN2K/regex.h/linknamespace = yes
test-xfail-XOPEN2K/search.h/linknamespace = yes
test-xfail-XOPEN2K/signal.h/linknamespace = yes
test-xfail-XOPEN2K/spawn.h/linknamespace = yes
test-xfail-XOPEN2K/stdlib.h/linknamespace = yes
test-xfail-XOPEN2K/syslog.h/linknamespace = yes
test-xfail-XOPEN2K/sys/timeb.h/linknamespace = yes
test-xfail-XOPEN2K/sys/wait.h/linknamespace = yes
test-xfail-XOPEN2K/ucontext.h/linknamespace = yes
test-xfail-XOPEN2K/unistd.h/linknamespace = yes
test-xfail-XOPEN2K/wordexp.h/linknamespace = yes
test-xfail-XPG3/fnmatch.h/linknamespace = yes
test-xfail-XPG3/glob.h/linknamespace = yes
test-xfail-XPG3/regex.h/linknamespace = yes
test-xfail-XPG3/search.h/linknamespace = yes
test-xfail-XPG3/stdio.h/linknamespace = yes
test-xfail-XPG3/stdlib.h/linknamespace = yes
test-xfail-XPG3/unistd.h/linknamespace = yes
test-xfail-XPG3/wordexp.h/linknamespace = yes
test-xfail-XPG4/fmtmsg.h/linknamespace = yes
test-xfail-XPG4/fnmatch.h/linknamespace = yes
test-xfail-XPG4/glob.h/linknamespace = yes
test-xfail-XPG4/netdb.h/linknamespace = yes
test-xfail-XPG4/regex.h/linknamespace = yes
test-xfail-XPG4/search.h/linknamespace = yes
test-xfail-XPG4/stdio.h/linknamespace = yes
test-xfail-XPG4/stdlib.h/linknamespace = yes
test-xfail-XPG4/syslog.h/linknamespace = yes
test-xfail-XPG4/sys/timeb.h/linknamespace = yes
test-xfail-XPG4/unistd.h/linknamespace = yes
test-xfail-XPG4/wordexp.h/linknamespace = yes

# changed in 2.22, tests were run directly, now using threads
# TODO: should be succeeding again with gnumach >= 2016-03-06
test-xfail-test-fpucw = yes
test-xfail-test-fpucw-ieee = yes
test-xfail-test-fpucw-ieee-static = yes
test-xfail-test-fpucw-static = yes
test-xfail-test-static = yes

# new in 2.23
test-xfail-test-fenv-sse-2 = yes
test-xfail-test-fenv-x87 = yes
test-xfail-tst-audit11 = yes
test-xfail-tst-audit12 = yes
test-xfail-tst-get-cpu-features = yes

# newly failing in 2.21, real regression
test-xfail-tst-backtrace2 = yes
test-xfail-tst-backtrace3 = yes

# fixed upstream
test-xfail-POSIX/signal.h/conform = yes
test-xfail-POSIX/termios.h/conform = yes

# happens on linux-i386 too
test-xfail-annexc = yes
test-xfail-tst-backtrace6 = yes
test-xfail-tst-waitid = yes

# don't seem to happen any more with 2.19 already
test-xfail-bug22 = yes
test-xfail-bug-getcontext = yes
test-xfail-test-assert-perr = yes
test-xfail-test-assert = yes
test-xfail-test-float = yes
test-xfail-tst-aio8 = yes
test-xfail-tst-chk1 = yes
test-xfail-tst-dlmopen1 = yes
test-xfail-tst-gettext4 = yes
test-xfail-tst-gettext5 = yes
test-xfail-tst-gettext6 = yes
test-xfail-tst-lfschk1 = yes
test-xfail-tst-tls18 = yes
endif


######################################################################
# i386 (including optimized flavours)
######################################################################
ifneq (,$(filter $(config-machine)-$(config-os), i586-linux-gnu i686-linux-gnu))
test-xfail-tst-backtrace6 = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-waitid = yes

# Probably a GCC bug: https://sourceware.org/ml/libc-alpha/2015-11/msg00533.html
test-xfail-tst-cleanupx4 = yes
endif


######################################################################
# kfreebsd-*
######################################################################
ifeq ($(config-os),kfreebsd-gnu)
test-xfail-ISO/ctype.h/conform = yes
test-xfail-ISO/signal.h/conform = yes
test-xfail-ISO/stdio.h/conform = yes
test-xfail-ISO/time.h/conform = yes
test-xfail-ISO11/ctype.h/conform = yes
test-xfail-ISO11/signal.h/conform = yes
test-xfail-ISO11/stdio.h/conform = yes
test-xfail-ISO11/time.h/conform = yes
test-xfail-ISO11/wctype.h/conform = yes
test-xfail-ISO99/ctype.h/conform = yes
test-xfail-ISO99/signal.h/conform = yes
test-xfail-ISO99/stdio.h/conform = yes
test-xfail-ISO99/time.h/conform = yes
test-xfail-ISO99/wctype.h/conform = yes
test-xfail-POSIX/aio.h/conform = yes
test-xfail-POSIX/ctype.h/conform = yes
test-xfail-POSIX/dirent.h/conform = yes
test-xfail-POSIX/fcntl.h/conform = yes
test-xfail-POSIX/grp.h/conform = yes
test-xfail-POSIX/mqueue.h/conform = yes
test-xfail-POSIX/pthread.h/conform = yes
test-xfail-POSIX/pwd.h/conform = yes
test-xfail-POSIX/regex.h/conform = yes
test-xfail-POSIX/sched.h/conform = yes
test-xfail-POSIX/semaphore.h/conform = yes
test-xfail-POSIX/signal.h/conform = yes
test-xfail-POSIX/stdio.h/conform = yes
test-xfail-POSIX/sys/mman.h/conform = yes
test-xfail-POSIX/sys/stat.h/conform = yes
test-xfail-POSIX/sys/times.h/conform = yes
test-xfail-POSIX/sys/types.h/conform = yes
test-xfail-POSIX/time.h/conform = yes
test-xfail-POSIX/unistd.h/conform = yes
test-xfail-POSIX/utime.h/conform = yes
test-xfail-POSIX2008/aio.h/conform = yes
test-xfail-POSIX2008/ctype.h/conform = yes
test-xfail-POSIX2008/dirent.h/conform = yes
test-xfail-POSIX2008/errno.h/conform = yes
test-xfail-POSIX2008/fcntl.h/conform = yes
test-xfail-POSIX2008/grp.h/conform = yes
test-xfail-POSIX2008/monetary.h/conform = yes
test-xfail-POSIX2008/mqueue.h/conform = yes
test-xfail-POSIX2008/net/if.h/conform = yes
test-xfail-POSIX2008/netdb.h/linknamespace = yes
test-xfail-POSIX2008/netinet/tcp.h/conform = yes
test-xfail-POSIX2008/pthread.h/conform = yes
test-xfail-POSIX2008/pwd.h/conform = yes
test-xfail-POSIX2008/regex.h/conform = yes
test-xfail-POSIX2008/sched.h/conform = yes
test-xfail-POSIX2008/semaphore.h/conform = yes
test-xfail-POSIX2008/spawn.h/conform = yes
test-xfail-POSIX2008/stdio.h/conform = yes
test-xfail-POSIX2008/stropts.h/conform = yes
test-xfail-POSIX2008/sys/mman.h/conform = yes
test-xfail-POSIX2008/sys/select.h/conform = yes
test-xfail-POSIX2008/sys/stat.h/conform = yes
test-xfail-POSIX2008/sys/statvfs.h/conform = yes
test-xfail-POSIX2008/sys/times.h/conform = yes
test-xfail-POSIX2008/sys/types.h/conform = yes
test-xfail-POSIX2008/sys/un.h/conform = yes
test-xfail-POSIX2008/termios.h/conform = yes
test-xfail-POSIX2008/time.h/conform = yes
test-xfail-POSIX2008/unistd.h/conform = yes
test-xfail-POSIX2008/utime.h/conform = yes
test-xfail-POSIX2008/wctype.h/conform = yes
test-xfail-UNIX98/aio.h/conform = yes
test-xfail-UNIX98/ctype.h/conform = yes
test-xfail-UNIX98/dirent.h/conform = yes
test-xfail-UNIX98/errno.h/conform = yes
test-xfail-UNIX98/ftw.h/conform = yes
test-xfail-UNIX98/grp.h/conform = yes
test-xfail-UNIX98/monetary.h/conform = yes
test-xfail-UNIX98/mqueue.h/conform = yes
test-xfail-UNIX98/pthread.h/conform = yes
test-xfail-UNIX98/pwd.h/conform = yes
test-xfail-UNIX98/regex.h/conform = yes
test-xfail-UNIX98/sched.h/conform = yes
test-xfail-UNIX98/semaphore.h/conform = yes
test-xfail-UNIX98/stropts.h/conform = yes
test-xfail-UNIX98/sys/ipc.h/conform = yes
test-xfail-UNIX98/sys/mman.h/conform = yes
test-xfail-UNIX98/sys/msg.h/conform = yes
test-xfail-UNIX98/sys/resource.h/conform = yes
test-xfail-UNIX98/sys/sem.h/conform = yes
test-xfail-UNIX98/sys/shm.h/conform = yes
test-xfail-UNIX98/sys/socket.h/conform = yes
test-xfail-UNIX98/sys/stat.h/conform = yes
test-xfail-UNIX98/sys/statvfs.h/conform = yes
test-xfail-UNIX98/sys/timeb.h/conform = yes
test-xfail-UNIX98/sys/times.h/conform = yes
test-xfail-UNIX98/sys/types.h/conform = yes
test-xfail-UNIX98/sys/uio.h/conform = yes
test-xfail-UNIX98/sys/un.h/conform = yes
test-xfail-UNIX98/termios.h/conform = yes
test-xfail-UNIX98/time.h/conform = yes
test-xfail-UNIX98/utime.h/conform = yes
test-xfail-UNIX98/wchar.h/conform = yes
test-xfail-UNIX98/wctype.h/conform = yes
test-xfail-XOPEN2K/arpa/inet.h/conform = yes
test-xfail-XOPEN2K/ctype.h/conform = yes
test-xfail-XOPEN2K/dirent.h/conform = yes
test-xfail-XOPEN2K/errno.h/conform = yes
test-xfail-XOPEN2K/ftw.h/conform = yes
test-xfail-XOPEN2K/grp.h/conform = yes
test-xfail-XOPEN2K/monetary.h/conform = yes
test-xfail-XOPEN2K/net/if.h/conform = yes
test-xfail-XOPEN2K/netdb.h/conform = yes
test-xfail-XOPEN2K/netdb.h/linknamespace = yes
test-xfail-XOPEN2K/netinet/in.h/conform = yes
test-xfail-XOPEN2K/netinet/tcp.h/conform = yes
test-xfail-XOPEN2K/pthread.h/conform = yes
test-xfail-XOPEN2K/pwd.h/conform = yes
test-xfail-XOPEN2K/regex.h/conform = yes
test-xfail-XOPEN2K/sched.h/conform = yes
test-xfail-XOPEN2K/spawn.h/conform = yes
test-xfail-XOPEN2K/stdlib.h/conform = yes
test-xfail-XOPEN2K/stropts.h/conform = yes
test-xfail-XOPEN2K/sys/ipc.h/conform = yes
test-xfail-XOPEN2K/sys/mman.h/conform = yes
test-xfail-XOPEN2K/sys/msg.h/conform = yes
test-xfail-XOPEN2K/sys/resource.h/conform = yes
test-xfail-XOPEN2K/sys/select.h/conform = yes
test-xfail-XOPEN2K/sys/sem.h/conform = yes
test-xfail-XOPEN2K/sys/shm.h/conform = yes
test-xfail-XOPEN2K/sys/socket.h/conform = yes
test-xfail-XOPEN2K/sys/stat.h/conform = yes
test-xfail-XOPEN2K/sys/statvfs.h/conform = yes
test-xfail-XOPEN2K/sys/time.h/conform = yes
test-xfail-XOPEN2K/sys/timeb.h/conform = yes
test-xfail-XOPEN2K/sys/times.h/conform = yes
test-xfail-XOPEN2K/sys/types.h/conform = yes
test-xfail-XOPEN2K/sys/uio.h/conform = yes
test-xfail-XOPEN2K/sys/un.h/conform = yes
test-xfail-XOPEN2K/time.h/conform = yes
test-xfail-XOPEN2K/unistd.h/conform = yes
test-xfail-XOPEN2K/utime.h/conform = yes
test-xfail-XOPEN2K/utmpx.h/conform = yes
test-xfail-XOPEN2K/wchar.h/conform = yes
test-xfail-XOPEN2K/wctype.h/conform = yes
test-xfail-XOPEN2K8/aio.h/conform = yes
test-xfail-XOPEN2K8/arpa/inet.h/conform = yes
test-xfail-XOPEN2K8/ctype.h/conform = yes
test-xfail-XOPEN2K8/dirent.h/conform = yes
test-xfail-XOPEN2K8/errno.h/conform = yes
test-xfail-XOPEN2K8/fcntl.h/conform = yes
test-xfail-XOPEN2K8/ftw.h/conform = yes
test-xfail-XOPEN2K8/grp.h/conform = yes
test-xfail-XOPEN2K8/monetary.h/conform = yes
test-xfail-XOPEN2K8/mqueue.h/conform = yes
test-xfail-XOPEN2K8/net/if.h/conform = yes
test-xfail-XOPEN2K8/netdb.h/conform = yes
test-xfail-XOPEN2K8/netdb.h/linknamespace = yes
test-xfail-XOPEN2K8/netinet/in.h/conform = yes
test-xfail-XOPEN2K8/netinet/tcp.h/conform = yes
test-xfail-XOPEN2K8/pthread.h/conform = yes
test-xfail-XOPEN2K8/pwd.h/conform = yes
test-xfail-XOPEN2K8/regex.h/conform = yes
test-xfail-XOPEN2K8/sched.h/conform = yes
test-xfail-XOPEN2K8/semaphore.h/conform = yes
test-xfail-XOPEN2K8/spawn.h/conform = yes
test-xfail-XOPEN2K8/stdlib.h/conform = yes
test-xfail-XOPEN2K8/stropts.h/conform = yes
test-xfail-XOPEN2K8/sys/ipc.h/conform = yes
test-xfail-XOPEN2K8/sys/mman.h/conform = yes
test-xfail-XOPEN2K8/sys/msg.h/conform = yes
test-xfail-XOPEN2K8/sys/resource.h/conform = yes
test-xfail-XOPEN2K8/sys/select.h/conform = yes
test-xfail-XOPEN2K8/sys/sem.h/conform = yes
test-xfail-XOPEN2K8/sys/shm.h/conform = yes
test-xfail-XOPEN2K8/sys/socket.h/conform = yes
test-xfail-XOPEN2K8/sys/stat.h/conform = yes
test-xfail-XOPEN2K8/sys/statvfs.h/conform = yes
test-xfail-XOPEN2K8/sys/time.h/conform = yes
test-xfail-XOPEN2K8/sys/times.h/conform = yes
test-xfail-XOPEN2K8/sys/types.h/conform = yes
test-xfail-XOPEN2K8/sys/uio.h/conform = yes
test-xfail-XOPEN2K8/sys/un.h/conform = yes
test-xfail-XOPEN2K8/time.h/conform = yes
test-xfail-XOPEN2K8/unistd.h/conform = yes
test-xfail-XOPEN2K8/utime.h/conform = yes
test-xfail-XOPEN2K8/utmpx.h/conform = yes
test-xfail-XOPEN2K8/wchar.h/conform = yes
test-xfail-XOPEN2K8/wctype.h/conform = yes
test-xfail-XPG3/ctype.h/conform = yes
test-xfail-XPG3/dirent.h/conform = yes
test-xfail-XPG3/errno.h/conform = yes
test-xfail-XPG3/grp.h/conform = yes
test-xfail-XPG3/monetary.h/conform = yes
test-xfail-XPG3/regex.h/conform = yes
test-xfail-XPG3/sys/ipc.h/conform = yes
test-xfail-XPG3/sys/msg.h/conform = yes
test-xfail-XPG3/sys/sem.h/conform = yes
test-xfail-XPG3/sys/shm.h/conform = yes
test-xfail-XPG3/sys/times.h/conform = yes
test-xfail-XPG3/utime.h/conform = yes
test-xfail-XPG4/ctype.h/conform = yes
test-xfail-XPG4/dirent.h/conform = yes
test-xfail-XPG4/errno.h/conform = yes
test-xfail-XPG4/ftw.h/conform = yes
test-xfail-XPG4/grp.h/conform = yes
test-xfail-XPG4/monetary.h/conform = yes
test-xfail-XPG4/pwd.h/conform = yes
test-xfail-XPG4/regex.h/conform = yes
test-xfail-XPG4/stropts.h/conform = yes
test-xfail-XPG4/sys/ipc.h/conform = yes
test-xfail-XPG4/sys/mman.h/conform = yes
test-xfail-XPG4/sys/msg.h/conform = yes
test-xfail-XPG4/sys/resource.h/conform = yes
test-xfail-XPG4/sys/sem.h/conform = yes
test-xfail-XPG4/sys/shm.h/conform = yes
test-xfail-XPG4/sys/socket.h/conform = yes
test-xfail-XPG4/sys/statvfs.h/conform = yes
test-xfail-XPG4/sys/timeb.h/conform = yes
test-xfail-XPG4/sys/times.h/conform = yes
test-xfail-XPG4/sys/uio.h/conform = yes
test-xfail-XPG4/sys/un.h/conform = yes
test-xfail-XPG4/time.h/conform = yes
test-xfail-XPG4/utime.h/conform = yes

# will expectedly SIGSEGV on kfreebsd 10.0 and later, due to having
# nxstack=1 by default (bug #762404)
test-xfail-tst-execstack = yes
test-xfail-tst-execstack-needed = yes
test-xfail-tst-execstack-prog = =yes
endif


######################################################################
# kfreebsd-amd64
######################################################################
ifeq ($(config-machine)-$(config-os),x86_64-kfreebsd-gnu)
test-xfail-check-local-headers = yes
test-xfail-tst-aio10 = yes
test-xfail-tst-aio9 = yes
test-xfail-tst-attr2 = yes
test-xfail-tst-attr3 = yes
test-xfail-tst-cancel7 = yes
test-xfail-tst-cancelx7 = yes
test-xfail-tst-default-attr = yes
test-xfail-tst-getpid1 = yes
test-xfail-tst-getpid2 = yes
test-xfail-tst-longjmp_chk = yes
test-xfail-tst-renameat = yes
test-xfail-tst-setcontext2 = yes
test-xfail-tst-shm = yes
test-xfail-tst-waitid = yes
test-xfail-tst-writev = yes
endif


######################################################################
# kfreebsd-i386 (including optimized flavours)
######################################################################
ifneq (,$(filter $(config-machine)-$(config-os), i586-kfreebsd-gnu i686-kfreebsd-gnu))
test-xfail-check-local-headers = yes
test-xfail-tst-aio10 = yes
test-xfail-tst-aio9 = yes
test-xfail-tst-attr2 = yes
test-xfail-tst-attr3 = yes
test-xfail-tst-cancel7 = yes
test-xfail-tst-cancelx7 = yes
test-xfail-tst-cleanupx4 = yes
test-xfail-tst-default-attr = yes
test-xfail-tst-getpid1 = yes
test-xfail-tst-getpid2 = yes
test-xfail-tst-longjmp_chk = yes
test-xfail-tst-renameat = yes
test-xfail-tst-setcontext2 = yes
test-xfail-tst-shm = yes
test-xfail-tst-waitid = yes
endif


######################################################################
# m68k
######################################################################
ifeq ($(config-machine)-$(config-os),m68k-linux-gnu)
test-xfail-annexc = yes
test-xfail-bug-nextafter = yes
test-xfail-bug-nexttoward = yes
test-xfail-bug-regex20 = yes
test-xfail-check-localplt = yes
test-xfail-localedata/sort-test = yes
test-xfail-test-double = yes
test-xfail-test-fenv = yes
test-xfail-test-float = yes
test-xfail-test-idouble = yes
test-xfail-test-ifloat = yes
test-xfail-test-misc = yes
test-xfail-tst-atomic-long = yes
test-xfail-tst-atomic = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-mqueue6 = yes
test-xfail-tst-mutexpi2 = yes
test-xfail-tst-mutexpi4 = yes
test-xfail-tst-mutexpi5a = yes
test-xfail-tst-mutexpi5 = yes
test-xfail-tst-mutexpi6 = yes
test-xfail-tst-mutexpi7a = yes
test-xfail-tst-mutexpi7 = yes
test-xfail-tst-mutexpi9 = yes
test-xfail-tst-robust1 = yes
test-xfail-tst-robust2 = yes
test-xfail-tst-robust3 = yes
test-xfail-tst-robust4 = yes
test-xfail-tst-robust5 = yes
test-xfail-tst-robust6 = yes
test-xfail-tst-robust7 = yes
test-xfail-tst-robust8 = yes
test-xfail-tst-robust9 = yes
test-xfail-tst-robustpi1 = yes
test-xfail-tst-robustpi2 = yes
test-xfail-tst-robustpi3 = yes
test-xfail-tst-robustpi4 = yes
test-xfail-tst-robustpi5 = yes
test-xfail-tst-robustpi6 = yes
test-xfail-tst-robustpi7 = yes
test-xfail-tst-robustpi8 = yes
test-xfail-tst-rxspencer = yes
endif


######################################################################
# mips*
######################################################################
ifneq (,$(filter $(config-machine)-$(config-os), mips-linux-gnu mipsel-linux-gnu mips64-linux-gnuabi64 mips64el-linux-gnuabi64 mips64-linux-gnuabin32 mips64el-linux-gnuabin32))
# MIPS GCC does not use PT_GNU_STACK markers (this is a GCC issue)
test-xfail-check-execstack = yes

# Peculiarities of MIPS ELF
test-xfail-check-localplt = yes

# These tests require at least a kernel 4.0
test-xfail-tst-mode-switch-1 = yes
test-xfail-tst-mode-switch-2 = yes
test-xfail-tst-mode-switch-3 = yes

# This is a namespace pollution from kernel header <asm/sockios.h>
# which includes <asm/ioctl.h>
test-xfail-UNIX98/sys/socket.h/conform = yes
test-xfail-XOPEN2K/arpa/inet.h/conform = yes
test-xfail-XOPEN2K/netdb.h/conform = yes
test-xfail-XOPEN2K/netinet/in.h/conform = yes
test-xfail-XOPEN2K/sys/socket.h/conform = yes
test-xfail-XOPEN2K8/arpa/inet.h/conform = yes
test-xfail-XOPEN2K8/netdb.h/conform = yes
test-xfail-XOPEN2K8/netinet/in.h/conform = yes
test-xfail-XOPEN2K8/sys/socket.h/conform = yes
test-xfail-XPG4/sys/socket.h/conform = yes
endif


######################################################################
# mips
######################################################################
ifeq ($(config-machine)-$(config-os),mips-linux-gnu)
test-xfail-POSIX2008/sys/stat.h/conform = yes
test-xfail-POSIX/sys/stat.h/conform = yes
test-xfail-UNIX98/sys/stat.h/conform = yes
test-xfail-XOPEN2K8/sys/stat.h/conform = yes
test-xfail-XOPEN2K/sys/stat.h/conform = yes
test-xfail-tst-audit1 = yes
test-xfail-tst-audit2 = yes
test-xfail-tst-audit8 = yes
test-xfail-tst-audit9 = yes
test-xfail-tst-cond16 = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-stack4 = yes
test-xfail-tst-waitid = yes
endif


######################################################################
# mipsel
######################################################################
ifeq ($(config-machine)-$(config-os),mipsel-linux-gnu)
test-xfail-POSIX2008/sys/stat.h/conform = yes
test-xfail-POSIX/sys/stat.h/conform = yes
test-xfail-UNIX98/sys/stat.h/conform = yes
test-xfail-XOPEN2K8/sys/stat.h/conform = yes
test-xfail-XOPEN2K/sys/stat.h/conform = yes
test-xfail-test-fenv = yes
test-xfail-tst-audit1 = yes
test-xfail-tst-audit2 = yes
test-xfail-tst-audit8 = yes
test-xfail-tst-audit9 = yes
test-xfail-tst-cond16 = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-stack4 = yes
test-xfail-tst-waitid = yes

# These failures are due to a bug in the Loongson 3A FPU
test-xfail-test-double = yes
test-xfail-test-double-finite = yes
test-xfail-test-idouble = yes
endif


######################################################################
# mips64el
######################################################################
ifeq ($(config-machine)-$(config-os),mips64el-linux-gnuabi64)
test-xfail-tst-audit1 = yes
test-xfail-tst-audit2 = yes
test-xfail-tst-audit8 = yes
test-xfail-tst-audit9 = yes
test-xfail-tst-cond16 = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-stack4 = yes
test-xfail-tst-waitid = yes
endif


######################################################################
# mipsn32el
######################################################################
ifeq ($(config-machine)-$(config-os),mips64el-linux-gnuabin32)
test-xfail-test-fenv = yes
test-xfail-tst-audit1 = yes
test-xfail-tst-audit2 = yes
test-xfail-tst-audit8 = yes
test-xfail-tst-audit9 = yes
test-xfail-tst-cond16 = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-stack4 = yes
test-xfail-tst-waitid = yes
endif


######################################################################
# mips64
######################################################################
ifeq ($(config-machine)-$(config-os),mips64-linux-gnuabi64)
test-xfail-tst-audit1 = yes
test-xfail-tst-audit2 = yes
test-xfail-tst-audit8 = yes
test-xfail-tst-audit9 = yes
test-xfail-tst-cond16 = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-stack4 = yes
test-xfail-tst-waitid = yes
endif


######################################################################
# mipsn32
######################################################################
ifeq ($(config-machine)-$(config-os),mips64-linux-gnuabin32)
test-xfail-tst-audit1 = yes
test-xfail-tst-audit2 = yes
test-xfail-tst-audit8 = yes
test-xfail-tst-audit9 = yes
test-xfail-tst-cond16 = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-stack4 = yes
test-xfail-tst-waitid = yes
endif


######################################################################
# ppc64el
######################################################################
ifeq ($(config-machine)-$(config-os),powerpc64le-linux-gnu)
test-xfail-tst-cancel17 = yes
test-xfail-tst-cancelx17 = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-waitid = yes
endif

######################################################################
# ppc64
######################################################################
ifeq ($(config-machine)-$(config-os),powerpc64-linux-gnu)
test-xfail-tst-backtrace5 = yes
test-xfail-tst-backtrace6 = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-waitid = yes

# Known failure not a regression, see https://sourceware.org/bugzilla/show_bug.cgi?id=6527
test-xfail-tst-malloc-thread-exit = yes
test-xfail-tst-malloc-thread-fail = yes
endif


######################################################################
# powerpc
######################################################################
ifeq ($(config-machine)-$(config-os),powerpc-linux-gnu)
test-xfail-tst-backtrace5 = yes
test-xfail-tst-backtrace6 = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-waitid = yes

# Known failure not a regression, see https://sourceware.org/bugzilla/show_bug.cgi?id=6527
test-xfail-tst-malloc-thread-exit = yes
test-xfail-tst-malloc-thread-fail = yes

# Failures due to a GCC bug, see http://gcc.gnu.org/bugzilla/show_bug.cgi?id=59412
#                            and http://gcc.gnu.org/bugzilla/show_bug.cgi?id=64811
test-xfail-test-double = yes
test-xfail-test-float = yes
test-xfail-test-idouble = yes
test-xfail-test-ifloat = yes
endif


######################################################################
# powerpcspe
######################################################################
ifeq ($(config-machine)-$(config-os),powerpc-linux-gnuspe)
test-xfail-annexc = yes
test-xfail-bug-nextafter = yes
test-xfail-bug-nexttoward = yes
test-xfail-check-localplt = yes
test-xfail-iconv-test = yes
test-xfail-isomac = yes
test-xfail-test-double = yes
test-xfail-test-fenv = yes
test-xfail-test-float = yes
test-xfail-test-idouble = yes
test-xfail-test-ifloat = yes
test-xfail-test-misc = yes
test-xfail-tst-backtrace5 = yes
test-xfail-tst-backtrace6 = yes
test-xfail-tst-cancel4 = yes
test-xfail-tst-cancel5 = yes
test-xfail-tst-cancelx4 = yes
test-xfail-tst-cancelx5 = yes
test-xfail-tst-key1 = yes
test-xfail-tst-key4 = yes
test-xfail-tst-setcontext-fpscr = yes
endif


######################################################################
# s390
######################################################################
ifeq ($(config-machine)-$(config-os),s390-linux-gnu)
test-xfail-tst-cancel17 = yes
test-xfail-tst-cancelx17 = yes
test-xfail-tst-waitid = yes
endif


######################################################################
# s390x
######################################################################
ifeq ($(config-machine)-$(config-os),s390x-linux-gnu)
test-xfail-tst-cancel17 = yes
test-xfail-tst-cancelx17 = yes
test-xfail-tst-cond10 = yes
test-xfail-tst-cond25 = yes
test-xfail-tst-protected1a = yes
test-xfail-tst-protected1b = yes
test-xfail-tst-waitid = yes

# Known failure not a regression, see https://sourceware.org/bugzilla/show_bug.cgi?id=6527
test-xfail-tst-malloc-thread-exit = yes
test-xfail-tst-malloc-thread-fail = yes
endif


######################################################################
# sparc
######################################################################
ifeq ($(config-machine)-$(config-os),sparc-linux-gnu)
test-xfail-annexc = yes
test-xfail-check-localplt = yes
test-xfail-tst-backtrace2 = yes
test-xfail-tst-backtrace3 = yes
test-xfail-tst-backtrace4 = yes
test-xfail-tst-backtrace5 = yes
test-xfail-tst-backtrace6 = yes
test-xfail-tst-waitid = yes

# Even if configured using --with-long-double-128, the biarch32 compiler
# on sparc64 defaults to 64-bit doubles, causing the failure below. This
# should be fixed by the following gcc patch:
# http://gcc.gnu.org/ml/gcc-patches/2013-12/msg00318.html
test-xfail-stdlib/isomac = yes
endif


######################################################################
# sparc64
######################################################################
ifeq ($(config-machine)-$(config-os),sparc64-linux-gnu)
test-xfail-ISO/setjmp.h/conform = yes
test-xfail-ISO11/setjmp.h/conform = yes
test-xfail-ISO99/setjmp.h/conform = yes
test-xfail-POSIX/pthread.h/conform = yes
test-xfail-POSIX/setjmp.h/conform = yes
test-xfail-POSIX2008/pthread.h/conform = yes
test-xfail-POSIX2008/setjmp.h/conform = yes
test-xfail-UNIX98/pthread.h/conform = yes
test-xfail-UNIX98/setjmp.h/conform = yes
test-xfail-XOPEN2K/pthread.h/conform = yes
test-xfail-XOPEN2K/setjmp.h/conform = yes
test-xfail-XOPEN2K8/pthread.h/conform = yes
test-xfail-XOPEN2K8/setjmp.h/conform = yes
test-xfail-XPG3/setjmp.h/conform = yes
test-xfail-XPG4/setjmp.h/conform = yes
test-xfail-test-wcsncmp = yes
test-xfail-tst-protected1a = yes
test-xfail-tst-protected1b = yes
test-xfail-tst-waitid = yes
endif


######################################################################
# x32
######################################################################
ifeq ($(config-machine)-$(config-os),x86_64-linux-gnux32)
test-xfail-ISO11/time.h/conform = yes
test-xfail-POSIX2008/mqueue.h/conform = yes
test-xfail-POSIX2008/sys/select.h/conform = yes
test-xfail-POSIX2008/sys/stat.h/conform = yes
test-xfail-POSIX2008/time.h/conform = yes
test-xfail-POSIX/mqueue.h/conform = yes
test-xfail-POSIX/sched.h/conform = yes
test-xfail-POSIX/time.h/conform = yes
test-xfail-UNIX98/mqueue.h/conform = yes
test-xfail-UNIX98/sched.h/conform = yes
test-xfail-UNIX98/time.h/conform = yes
test-xfail-XOPEN2K8/mqueue.h/conform = yes
test-xfail-XOPEN2K8/sys/select.h/conform = yes
test-xfail-XOPEN2K8/sys/stat.h/conform = yes
test-xfail-XOPEN2K8/time.h/conform = yes
test-xfail-XOPEN2K/sys/select.h/conform = yes
test-xfail-XOPEN2K/time.h/conform = yes
test-xfail-tst-backtrace6 = yes
test-xfail-tst-mqueue5 = yes
test-xfail-tst-waitid = yes
endif
