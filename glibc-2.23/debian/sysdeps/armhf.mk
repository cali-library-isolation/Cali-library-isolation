# configuration options for all flavours
extra_config_options = --enable-multi-arch

# Install a compat symlink so old binaries keep working:
define libc6_extra_pkg_install
mkdir -p debian/libc6/lib/arm-linux-gnueabihf
ln -sf ld-linux-armhf.so.3 debian/libc6/lib/arm-linux-gnueabihf/ld-linux.so.3
endef

define libc6-udeb_extra_pkg_install
mkdir -p debian/libc6-udeb/lib/arm-linux-gnueabihf
ln -sf /lib/ld-linux-armhf.so.3 debian/libc6-udeb/lib/arm-linux-gnueabihf/ld-linux.so.3
endef

# build soft-float (armel) alternative library
GLIBC_MULTILIB_PASSES += armel
DEB_ARCH_MULTILIB_PACKAGES += libc6-armel libc6-dev-armel
armel_configure_target = arm-linux-gnueabi
armel_CC = $(CC) -mfloat-abi=soft
armel_CXX = $(CXX) -mfloat-abi=soft
armel_slibdir = /lib/arm-linux-gnueabi
armel_libdir = /usr/lib/arm-linux-gnueabi

define libc6-dev-armel_extra_pkg_install

mkdir -p debian/libc6-dev-armel/usr/include
ln -sf arm-linux-gnueabihf/bits debian/libc6-dev-armel/usr/include/
ln -sf arm-linux-gnueabihf/gnu debian/libc6-dev-armel/usr/include/
ln -sf arm-linux-gnueabihf/fpu_control.h debian/libc6-dev-armel/usr/include/

mkdir -p debian/libc6-dev-armel/usr/include/arm-linux-gnueabihf/gnu
cp -a debian/tmp-armel/usr/include/gnu/stubs-soft.h \
	debian/libc6-dev-armel/usr/include/arm-linux-gnueabihf/gnu

mkdir -p debian/libc6-dev-armel/usr/include/sys
for i in `ls debian/tmp-libc/usr/include/arm-linux-gnueabihf/sys` ; do \
	ln -sf ../arm-linux-gnueabihf/sys/$$i debian/libc6-dev-armel/usr/include/sys/$$i ; \
done

endef

define libc6-armel_extra_pkg_install
mkdir -p debian/libc6-armel/lib
ln -sf $(armel_slibdir)/ld-linux.so.3 debian/libc6-armel/lib
endef
