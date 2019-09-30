## We use -march=i686 and glibc's i686 routines use cmov, so require it.
## A Debian-local glibc patch adds cmov to the search path.
#GLIBC_PASSES += i686
#DEB_ARCH_REGULAR_PACKAGES += libc0.3-i686
#i686_configure_target=i686-gnu
#i686_CC = $(CC) -march=i686 -mtune=generic
#i686_CXX = $(CXX) -march=i686 -mtune=generic
#i686_slibdir = /lib/$(DEB_HOST_MULTIARCH)/i686/cmov
#i686_extra_config_options = $(extra_config_options) --disable-compatible-utmp
#
## We use -mno-tls-direct-seg-refs to not wrap-around segments, as it
## greatly increase the speed when running under the 32bit Xen hypervisor.
#GLIBC_PASSES += xen
#DEB_ARCH_REGULAR_PACKAGES += libc0.3-xen
#xen_configure_target=i686-gnu
#xen_CC = $(CC) -march=i686 -mtune=generic
#xen_CXX = $(CXX) -march=i686 -mtune=generic
#xen_extra_cflags = -mno-tls-direct-seg-refs
#xen_slibdir = /lib/$(DEB_HOST_MULTIARCH)/i686/nosegneg
#
#ifeq ($(filter stage1,$(DEB_BUILD_PROFILES)),)
#define libc0.3-dev_extra_pkg_install
#mkdir -p debian/libc0.3-dev/$(libdir)/xen
#cp -af debian/tmp-xen/$(libdir)/*.a \
#	debian/libc0.3-dev/$(libdir)/xen
#endef
#endif
