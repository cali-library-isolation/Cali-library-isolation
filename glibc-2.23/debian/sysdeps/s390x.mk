# configuration options for all flavours
extra_config_options = --enable-multi-arch

# main library
libc_rtlddir = /lib
libc_CC = $(CC) -fno-pie -no-pie
libc_CXX = $(CXX) -fno-pie -no-pie

# build 32-bit (s390) alternative library
GLIBC_MULTILIB_PASSES += s390
DEB_ARCH_MULTILIB_PACKAGES += libc6-s390 libc6-dev-s390
libc6-s390_shlib_dep = libc6-s390 (>= $(shlib_dep_ver))
s390_configure_target = s390-linux-gnu
s390_CC = $(CC) -fno-pie -no-pie -m31
s390_CXX = $(CXX) -fno-pie -no-pie -m31
s390_slibdir = /lib32
s390_libdir = /usr/lib32

define libc6-dev-s390_extra_pkg_install

mkdir -p debian/libc6-dev-s390/usr/include
ln -s s390x-linux-gnu/bits debian/libc6-dev-s390/usr/include/
ln -s s390x-linux-gnu/gnu debian/libc6-dev-s390/usr/include/
ln -s s390x-linux-gnu/fpu_control.h debian/libc6-dev-s390/usr/include/

mkdir -p debian/libc6-dev-s390/usr/include/s390x-linux-gnu/gnu
cp -a debian/tmp-s390/usr/include/gnu/stubs-32.h \
        debian/libc6-dev-s390/usr/include/s390x-linux-gnu/gnu

mkdir -p debian/libc6-dev-s390/usr/include/sys
for i in `ls debian/tmp-libc/usr/include/s390x-linux-gnu/sys` ; do \
        ln -s ../s390x-linux-gnu/sys/$$i debian/libc6-dev-s390/usr/include/sys/$$i ; \
done

endef

define libc6-s390_extra_pkg_install
mkdir -p debian/$(curpass)/lib
ln -s /lib32/ld.so.1 debian/$(curpass)/lib
endef
