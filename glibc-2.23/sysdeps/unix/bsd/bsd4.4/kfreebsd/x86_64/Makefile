# The default ABI is 64.
default-abi := 64

32bit-predefine = __i386__
64bit-predefine = __x86_64__

# Additional header files to be installed in $prefix/include:

ifeq ($(subdir),misc)
sysdep_headers += \
 sys/io.h \
 sys/perm.h
endif

# Additional functions, and particular system calls:

ifeq ($(subdir),misc)
# For <sys/io.h> and <sys/perm.h>.
sysdep_routines += iopl ioperm
endif
