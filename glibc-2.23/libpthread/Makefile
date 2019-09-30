#
#   Copyright (C) 1994, 1995, 1996, 1997, 2000, 2002, 2004, 2005, 2006, 2007,
#     2008, 2011 Free Software Foundation, Inc.
#
#   This program is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public License as
#   published by the Free Software Foundation; either version 2, or (at
#   your option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#   General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

ifeq ($(..),)
# non-glibc build
IN_GLIBC = no
else
# glibc build
IN_GLIBC = yes
# set elf=yes, to retain compatibility with glibc < 2.16
ifeq ($(elf),)
elf = yes
endif
endif

ifeq ($(IN_GLIBC),no)
dir := libpthread
makemode := library
else
subdir := libpthread

pthread-version := 0.3

srcdir = .
endif

MICROKERNEL := mach
SYSDEPS := lockfile

LCLHDRS :=

libpthread-routines := pt-attr pt-attr-destroy pt-attr-getdetachstate	    \
	pt-attr-getguardsize pt-attr-getinheritsched			    \
	pt-attr-getschedparam pt-attr-getschedpolicy pt-attr-getscope	    \
	pt-attr-getstack pt-attr-getstackaddr pt-attr-getstacksize	    \
	pt-attr-init pt-attr-setdetachstate pt-attr-setguardsize	    \
	pt-attr-setinheritsched pt-attr-setschedparam			    \
	pt-attr-setschedpolicy pt-attr-setscope pt-attr-setstack	    \
	pt-attr-setstackaddr pt-attr-setstacksize			    \
									    \
	pt-barrier-destroy pt-barrier-init pt-barrier-wait		    \
	pt-barrier pt-barrierattr-destroy pt-barrierattr-init		    \
	pt-barrierattr-getpshared pt-barrierattr-setpshared		    \
									    \
	pt-destroy-specific pt-init-specific				    \
	pt-key-create pt-key-delete					    \
	pt-getspecific pt-setspecific					    \
									    \
	pt-once								    \
									    \
	pt-alloc							    \
	pt-create							    \
	pt-getattr							    \
	pt-equal							    \
	pt-dealloc							    \
	pt-detach							    \
	pt-exit								    \
	pt-initialize							    \
	pt-join								    \
	pt-self								    \
	pt-sigmask							    \
	pt-spin-inlines							    \
	pt-cleanup							    \
	pt-setcancelstate						    \
	pt-setcanceltype						    \
	pt-testcancel							    \
	pt-cancel							    \
									    \
	pt-mutexattr							    \
	pt-mutexattr-destroy pt-mutexattr-init				    \
	pt-mutexattr-getprioceiling pt-mutexattr-getprotocol		    \
	pt-mutexattr-getpshared pt-mutexattr-gettype			    \
	pt-mutexattr-setprioceiling pt-mutexattr-setprotocol		    \
	pt-mutexattr-setpshared pt-mutexattr-settype			    \
									    \
	pt-mutex-init pt-mutex-destroy					    \
	pt-mutex-lock pt-mutex-trylock pt-mutex-timedlock		    \
	pt-mutex-unlock							    \
	pt-mutex-transfer-np						    \
	pt-mutex-getprioceiling pt-mutex-setprioceiling			    \
									    \
	pt-rwlock-attr							    \
	pt-rwlockattr-init pt-rwlockattr-destroy			    \
	pt-rwlockattr-getpshared pt-rwlockattr-setpshared		    \
									    \
	pt-rwlock-init pt-rwlock-destroy				    \
	pt-rwlock-rdlock pt-rwlock-tryrdlock				    \
	pt-rwlock-trywrlock pt-rwlock-wrlock				    \
	pt-rwlock-timedrdlock pt-rwlock-timedwrlock			    \
	pt-rwlock-unlock						    \
									    \
	pt-cond								    \
	pt-condattr-init pt-condattr-destroy				    \
	pt-condattr-getclock pt-condattr-getpshared			    \
	pt-condattr-setclock pt-condattr-setpshared			    \
									    \
	pt-cond-destroy pt-cond-init					    \
	pt-cond-brdcast							    \
	pt-cond-signal							    \
	pt-cond-wait							    \
	pt-cond-timedwait						    \
	pt-hurd-cond-wait						    \
	pt-hurd-cond-timedwait						    \
									    \
	pt-stack-alloc							    \
	pt-thread-alloc							    \
	pt-thread-start							    \
	pt-thread-terminate						    \
	pt-startup							    \
									    \
	pt-getconcurrency pt-setconcurrency				    \
									    \
	pt-block							    \
	pt-timedblock							    \
	pt-wakeup							    \
	pt-docancel							    \
	pt-sysdep							    \
	pt-setup							    \
	pt-machdep							    \
	pt-spin								    \
									    \
	pt-sigstate-init						    \
	pt-sigstate-destroy						    \
	pt-sigstate							    \
									    \
	pt-atfork							    \
	pt-kill								    \
	pt-getcpuclockid						    \
									    \
	pt-getschedparam pt-setschedparam pt-setschedprio		    \
	pt-yield							    \
									    \
	sem-close sem-destroy sem-getvalue sem-init sem-open		    \
	sem-post sem-timedwait sem-trywait sem-unlink			    \
	sem-wait							    \
									    \
	shm-directory							    \
									    \
	cthreads-compat							    \
	$(SYSDEPS)

ifeq ($(IN_GLIBC),no)
SRCS := $(addsuffix .c,$(libpthread-routines))
OBJS = $(addsuffix .o,$(basename $(notdir $(SRCS))))

OTHERTAGS = 

libname = libpthread
endif

headers :=				\
              pthread.h				\
              pthread/pthread.h			\
              pthread/pthreadtypes.h		\
	      semaphore.h			\
						\
              bits/pthread.h			\
              bits/pthread-np.h			\
              bits/pthreadtypes.h		\
              bits/mutex.h			\
              bits/condition.h			\
              bits/condition-attr.h		\
              bits/spin-lock.h			\
              bits/spin-lock-inline.h		\
              bits/cancelation.h		\
              bits/thread-attr.h		\
              bits/barrier-attr.h		\
              bits/barrier.h			\
              bits/thread-specific.h		\
              bits/once.h			\
              bits/mutex-attr.h			\
              bits/rwlock.h			\
              bits/rwlock-attr.h		\
	      bits/semaphore.h

ifeq ($(IN_GLIBC),yes)
distribute :=

routines := forward libc_pthread_init alloca_cutoff
shared-only-routines = forward

vpath %.c

extra-libs := libpthread
extra-libs-others := $(extra-libs)
install-lib-ldscripts := libpthread.so

include ../Makeconfig
endif

SYSDEP_PATH = $(srcdir)/sysdeps/$(MICROKERNEL)/hurd/i386	\
	 $(srcdir)/sysdeps/$(MICROKERNEL)/i386			\
	 $(srcdir)/sysdeps/i386					\
	 $(srcdir)/sysdeps/$(MICROKERNEL)/hurd			\
	 $(srcdir)/sysdeps/$(MICROKERNEL)			\
	 $(srcdir)/sysdeps/hurd					\
	 $(srcdir)/sysdeps/generic				\
	 $(srcdir)/sysdeps/posix				\
	 $(srcdir)/pthread					\
	 $(srcdir)/include

VPATH += $(SYSDEP_PATH)

ifeq ($(IN_GLIBC),no)
HURDLIBS = ihash
else
LDLIBS-pthread.so = -lihash
endif

ifeq ($(IN_GLIBC),no)
installhdrs :=
installhdrsubdir := .

include ../Makeconf
endif

CPPFLAGS += \
	  -DENABLE_TLS					\
	  $(addprefix -I, $(SYSDEP_PATH))		\
	  -imacros $(srcdir)/not-in-libc.h

ifeq ($(IN_GLIBC),no)
CPPFLAGS += \
	  -imacros $(srcdir)/include/libc-symbols.h
endif

ifeq ($(IN_GLIBC),yes)
CFLAGS-lockfile.c = -D_IO_MTSAFE_IO

all: # Make this the default target; it will be defined in Rules.
endif

ifeq ($(IN_GLIBC),no)
inst_libdir = $(libdir)
endif

ifeq ($(IN_GLIBC),no)
install: install-headers
install-headers: $(addprefix $(includedir)/, $(headers))

install: $(inst_libdir)/libpthread2.a $(inst_libdir)/libpthread2_pic.a
else
subdir_install: $(inst_libdir)/libpthread2.a
endif

# XXX: If $(inst_libdir)/libpthread2.a is installed and
# $(inst_libdir)/libpthread is not, we can have some issues.
.PHONY: $(inst_libdir)/libpthread.a $(inst_libdir)/libpthread_pic.a

# XXX: These rules are a hack.  But it is better than messing with
# ../Makeconf at the moment.  Note that the linker scripts
# $(srcdir)/libpthread.a and $(srcdir)/libpthread_pic.a get overwritten
# when building in $(srcdir) and not a seperate build directory.
$(inst_libdir)/libpthread2.a: $(inst_libdir)/libpthread.a
	mv $< $@
	$(INSTALL_DATA) $(srcdir)/libpthread.a $<

$(inst_libdir)/libpthread2_pic.a: $(inst_libdir)/libpthread_pic.a
	mv $< $@
	$(INSTALL_DATA) $(srcdir)/libpthread_pic.a $<

ifeq ($(IN_GLIBC),yes)
$(inst_libdir)/libpthread.so: $(objpfx)libpthread.so$(libpthread.so-version) \
			      $(+force)
	ln -sf $(slibdir)/libpthread.so$(libpthread.so-version) $@

libc-link.so = $(common-objpfx)libc.so

extra-B-pthread.so = -B$(common-objpfx)libpthread/

include ../Rules

# Depend on libc.so so a DT_NEEDED is generated in the shared objects.
# This ensures they will load libc.so for needed symbols if loaded by
# a statically-linked program that hasn't already loaded it.
# Depend on ld.so too to get proper versions of ld.so symbols.
$(objpfx)libpthread.so: $(libc-link.so) $(common-objpfx)libc_nonshared.a \
			$(if $(filter yes,$(elf)), $(elf-objpfx)/ld.so) \
			$(common-objpfx)/mach/libmachuser.so \
			$(common-objpfx)/hurd/libhurduser.so
endif

ifeq ($(IN_GLIBC),no)
.PHONY: $(addprefix $(includedir)/, $(headers))

$(addprefix $(includedir)/, $(headers)):
	@set -e;							\
	t="$@";								\
	t=$${t#$(includedir)/};						\
	header_ok=0;							\
	for dir in $(SYSDEP_PATH);					\
	do								\
	  if test -e "$$dir/$$t";					\
	  then								\
	    tdir=`dirname "$@"`;					\
	    if test ! -e $$tdir;					\
	    then							\
	      mkdir $$tdir;						\
	    fi;								\
	    echo $(INSTALL_DATA) "$$dir/$$t" "$@";			\
	    $(INSTALL_DATA) "$$dir/$$t" "$@";				\
	    header_ok=1;						\
	    break;							\
	  fi;								\
	done;								\
	if test "$${header_ok}" -ne 1;					\
	then								\
	  echo;								\
	  echo '*** 'The header file \`$@\' is required, but not	\
provided, by;								\
	  echo '*** 'this configuration.  Please report this to the	\
maintainer.;								\
	  echo;								\
	  false;							\
	fi

#  ifeq ($(VERSIONING),yes)
#  
#  # Adding this dependency gets it included in the command line,
#  # where ld will read it as a linker script.
#  $(libname).so.$(hurd-version): $(srcdir)/$(libname).map
#  
#  endif
endif
