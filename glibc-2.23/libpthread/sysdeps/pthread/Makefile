ifeq ($(subdir),posix)
  # FIXME: this is not getting $(pthread-version) from libpthread/Makefile!
CFLAGS-confstr.c += -DLIBPTHREAD_VERSION='"libpthread $(pthread-version)"'
endif
