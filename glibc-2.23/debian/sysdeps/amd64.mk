# configuration options for all flavours
extra_config_options = --enable-multi-arch
MIN_KERNEL_SUPPORTED := 2.6.32

# main library
libc_mvec = yes
libc_extra_cflags = -O3
libc_rtlddir = /lib64

# build 32-bit (i386) alternative library
GLIBC_MULTILIB_PASSES += i386
DEB_ARCH_MULTILIB_PACKAGES += libc6-i386 libc6-dev-i386
libc6-i386_shlib_dep = libc6-i386 (>= $(shlib_dep_ver))
i386_configure_target = i686-linux-gnu
i386_CC = $(CC) -m32 -march=pentium4 -mtune=generic -fno-regmove
i386_CXX = $(CXX) -m32 -march=pentium4 -mtune=generic -fno-regmove
i386_slibdir = /lib32
i386_libdir = /usr/lib32

define libc6-dev-i386_extra_pkg_install

mkdir -p debian/libc6-dev-i386/usr/include
ln -sf x86_64-linux-gnu/bits debian/libc6-dev-i386/usr/include/
ln -sf x86_64-linux-gnu/gnu debian/libc6-dev-i386/usr/include/
ln -sf x86_64-linux-gnu/fpu_control.h debian/libc6-dev-i386/usr/include/

mkdir -p debian/libc6-dev-i386/usr/include/x86_64-linux-gnu/gnu
cp -a debian/tmp-i386/usr/include/gnu/stubs-32.h \
        debian/libc6-dev-i386/usr/include/x86_64-linux-gnu/gnu

mkdir -p debian/libc6-dev-i386/usr/include/sys
for i in `ls debian/tmp-libc/usr/include/x86_64-linux-gnu/sys` ; do \
	ln -sf ../x86_64-linux-gnu/sys/$$i debian/libc6-dev-i386/usr/include/sys/$$i ; \
done

endef

define libc6-i386_extra_pkg_install
mkdir -p debian/libc6-i386/lib
ln -sf /lib32/ld-linux.so.2 debian/libc6-i386/lib
endef

# build x32 ABI alternative library
GLIBC_MULTILIB_PASSES += x32
DEB_ARCH_MULTILIB_PACKAGES += libc6-x32 libc6-dev-x32
libc6-x32_shlib_dep = libc6-x32 (>= $(shlib_dep_ver))
x32_configure_target = x86_64-linux-gnux32
x32_CC = $(CC) -mx32
x32_CXX = $(CXX) -mx32
x32_mvec = yes
x32_rtlddir = /libx32
x32_slibdir = /libx32
x32_libdir = /usr/libx32

define libc6-dev-x32_extra_pkg_install

mkdir -p debian/libc6-dev-x32/usr/include/x86_64-linux-gnu/gnu
cp -a debian/tmp-x32/usr/include/gnu/stubs-x32.h \
	debian/libc6-dev-x32/usr/include/x86_64-linux-gnu/gnu/

endef
