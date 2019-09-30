# When changing this, make sure to update debian/debhelper.in/libc.preinst!
MIN_KERNEL_SUPPORTED := 8.3.0
libc = libc0.1

# Build and expect pt_chown on this platform
pt_chown = yes
# Expect pldd on this platform
pldd = no

# NPTL Config
threads = yes
extra_config_options = --disable-compatible-utmp --disable-multi-arch --disable-werror
libc_add-ons = fbtl $(add-ons)

ifndef KFREEBSD_SOURCE
  ifeq ($(DEB_HOST_GNU_TYPE),$(DEB_BUILD_GNU_TYPE))
    KFREEBSD_HEADERS := /usr/include
  else
    KFREEBSD_HEADERS := /usr/$(DEB_HOST_GNU_TYPE)/include
  endif
  KFREEBSD_ARCH_HEADERS := /usr/include/$(DEB_HOST_MULTIARCH)
else
  KFREEBSD_HEADERS := $(KFREEBSD_SOURCE)/sys
endif

# Minimum Kernel supported
with_headers = --with-headers=$(shell pwd)/debian/include --enable-kernel=$(call xx,MIN_KERNEL_SUPPORTED)

KERNEL_HEADER_DIR = $(stamp)mkincludedir
$(stamp)mkincludedir:
	rm -rf debian/include
	mkdir debian/include

	# Link to any headers found at the new multiarch location,
	# otherwise look for them in the old locations
	for file in bsm machine machine-amd64 machine-i386 net netatalk netipx nfs osreldate.h x86 vm ; do \
	    if test -e $(KFREEBSD_ARCH_HEADERS)/$$file ; then \
	        ln -s $(KFREEBSD_ARCH_HEADERS)/$$file debian/include ; \
	    elif test -e $(KFREEBSD_HEADERS)/$$file ; then \
	        ln -s $(KFREEBSD_HEADERS)/$$file debian/include ; \
	    fi ; \
	done

	mkdir -p debian/include/sys
	# Link to any headers found in the old locations first
	if test -d $(KFREEBSD_HEADERS)/sys ; then \
	    find $(KFREEBSD_HEADERS)/sys -mindepth 1 \
		-exec ln -sf '{}' debian/include/sys ';' ; \
	fi
	# Link to any headers found at the new multiarch location,
	# replacing any existing links
	if test -d $(KFREEBSD_ARCH_HEADERS)/sys ; then \
	    find $(KFREEBSD_ARCH_HEADERS)/sys -mindepth 1 \
		-exec ln -sf '{}' debian/include/sys ';' ; \
	fi

	# To make configure happy if libc0.1-dev is not installed.
	touch debian/include/assert.h

	touch $@

# Also to make configure happy.
export CPPFLAGS = -isystem $(shell pwd)/debian/include

# This round of ugliness decomposes the FreeBSD kernel version number
# into an integer so it can be easily compared and then does so.
CURRENT_KERNEL_VERSION=$(shell uname -r)
define kernel_check
(minimum=$$((`echo $(1) | sed 's/\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\)/\1 \* 10000 + \2 \* 100 + \3/'`)); \
current=$$((`echo $(CURRENT_KERNEL_VERSION) | sed 's/\([0-9]*\)\.\([0-9]*\).*/\1 \* 10000 + \2 \* 100/'`)); \
if [ $$current -lt $$minimum ]; then \
  false; \
fi)
endef

