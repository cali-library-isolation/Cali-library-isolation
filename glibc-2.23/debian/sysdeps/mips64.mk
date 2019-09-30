# configuration options for all flavours
extra_cflags = -mno-plt

# main library
libc_rtlddir = /lib64

# build 32-bit (n32) alternative library
GLIBC_MULTILIB_PASSES += mipsn32
DEB_ARCH_MULTILIB_PACKAGES += libc6-mipsn32 libc6-dev-mipsn32
libc6-mipsn32_shlib_dep = libc6-mipsn32 (>= $(shlib_dep_ver))
mipsn32_configure_target = mips64-linux-gnuabin32
mipsn32_CC = $(CC) -mabi=n32
mipsn32_CXX = $(CXX) -mabi=n32
mipsn32_rtlddir = /lib32
mipsn32_slibdir = /lib32
mipsn32_libdir = /usr/lib32

# build 32-bit (o32) alternative library
GLIBC_MULTILIB_PASSES += mips32
DEB_ARCH_MULTILIB_PACKAGES += libc6-mips32 libc6-dev-mips32
libc6-mips32_shlib_dep = libc6-mips32 (>= $(shlib_dep_ver))
mips32_configure_target = mips-linux-gnu
mips32_CC = $(CC) -mabi=32
mips32_CXX = $(CXX) -mabi=32
mips32_rtlddir = /lib
mips32_slibdir = /libo32
mips32_libdir = /usr/libo32

define libc6-dev-mipsn32_extra_pkg_install

mkdir -p debian/libc6-dev-mipsn32/usr/include
ln -sf mips64-linux-gnuabi64/bits debian/libc6-dev-mipsn32/usr/include/
ln -sf mips64-linux-gnuabi64/gnu debian/libc6-dev-mipsn32/usr/include/
ln -sf mips64-linux-gnuabi64/fpu_control.h debian/libc6-dev-mipsn32/usr/include/

mkdir -p debian/libc6-dev-mipsn32/usr/include/mips64-linux-gnuabi64/gnu
cp -a debian/tmp-mipsn32/usr/include/gnu/stubs-n32_hard.h \
        debian/libc6-dev-mipsn32/usr/include/mips64-linux-gnuabi64/gnu

mkdir -p debian/libc6-dev-mipsn32/usr/include/sys
for i in `ls debian/tmp-libc/usr/include/mips64-linux-gnuabi64/sys` ; do \
        ln -sf ../mips64-linux-gnuabi64/sys/$$i debian/libc6-dev-mipsn32/usr/include/sys/$$i ; \
done

endef

define libc6-dev-mips32_extra_pkg_install

mkdir -p debian/libc6-dev-mips32/usr/include/mips64-linux-gnuabi64/gnu
cp -a debian/tmp-mips32/usr/include/gnu/stubs-o32_hard.h \
        debian/libc6-dev-mips32/usr/include/mips64-linux-gnuabi64/gnu

endef

# create a symlink for the 32 bit dynamic linker in /lib
define libc6-mips32_extra_pkg_install
mkdir -p debian/libc6-mips32/lib
ln -sf /libo32/ld.so.1 debian/libc6-mips32/lib
endef
