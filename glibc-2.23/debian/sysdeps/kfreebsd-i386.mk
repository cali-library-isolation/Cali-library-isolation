# Build a 32-bit optimized library
GLIBC_PASSES += i686
DEB_ARCH_REGULAR_PACKAGES += libc0.1-i686

# We use -march=i686 and glibc's i686 routines use cmov, so require it.
# A Debian-local glibc patch adds cmov to the search path.
i686_add-ons = $(libc_add-ons)
i686_configure_target=i686-kfreebsd-gnu
i686_CC = $(CC) -march=i686 -mtune=generic
i686_CXX = $(CXX) -march=i686 -mtune=generic
i686_slibdir = /lib/$(DEB_HOST_MULTIARCH)/i686/cmov
