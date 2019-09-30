# configuration options for all flavours
extra_config_options = --enable-multi-arch

# main library
libc_mvec = yes
libc_rtlddir = /libx32

# build 64-bit (amd64) alternative library
GLIBC_MULTILIB_PASSES += amd64
DEB_ARCH_MULTILIB_PACKAGES += libc6-amd64 libc6-dev-amd64
libc6-amd64_shlib_dep = libc6-amd64 (>= $(shlib_dep_ver))
amd64_configure_target = x86_64-linux-gnu
amd64_CC = $(CC) -m64
amd64_CXX = $(CXX) -m64
amd64_mvec = yes
amd64_rtlddir = /lib64
amd64_slibdir = /lib64
amd64_libdir = /usr/lib64

define libc6-dev-amd64_extra_pkg_install

mkdir -p debian/libc6-dev-amd64/usr/include
ln -s x86_64-linux-gnux32/bits debian/libc6-dev-amd64/usr/include/
ln -s x86_64-linux-gnux32/gnu debian/libc6-dev-amd64/usr/include/
ln -s x86_64-linux-gnux32/fpu_control.h debian/libc6-dev-amd64/usr/include/

mkdir -p debian/libc6-dev-amd64/usr/include/x86_64-linux-gnux32/gnu
cp -a debian/tmp-amd64/usr/include/gnu/stubs-64.h \
	debian/libc6-dev-amd64/usr/include/x86_64-linux-gnux32/gnu

mkdir -p debian/libc6-dev-amd64/usr/include/sys
for i in `ls debian/tmp-libc/usr/include/x86_64-linux-gnux32/sys`; do \
	ln -s ../x86_64-linux-gnux32/sys/$$i debian/libc6-dev-amd64/usr/include/sys/$$i ; \
done

endef

# build 32-bit (i386) alternative library
GLIBC_MULTILIB_PASSES += i386
DEB_ARCH_MULTILIB_PACKAGES += libc6-i386 libc6-dev-i386
libc6-i386_shlib_dep = libc6-i386 (>= $(shlib_dep_ver))
i386_configure_target = i686-linux-gnu
i386_CC = $(CC) -m32 -march=pentium4 -mtune=generic
i386_CXX = $(CXX) -m32 -march=pentium4 -mtune=generic
i386_mvec = no
i386_slibdir = /lib32
i386_libdir = /usr/lib32

define libc6-dev-i386_extra_pkg_install

mkdir -p debian/libc6-dev-i386/usr/include/x86_64-linux-gnux32/gnu
cp -a debian/tmp-i386/usr/include/gnu/stubs-32.h \
	debian/libc6-dev-i386/usr/include/x86_64-linux-gnux32/gnu

endef

define libc6-i386_extra_pkg_install
mkdir -p debian/libc6-i386/lib
ln -sf /lib32/ld-linux.so.2 debian/libc6-i386/lib
endef
