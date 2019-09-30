# configuration options for all flavours
extra_cflags = -mno-plt

# main library
libc_rtlddir = /lib32

# build 64-bit alternative library
GLIBC_MULTILIB_PASSES += mips64
DEB_ARCH_MULTILIB_PACKAGES += libc6-mips64 libc6-dev-mips64
libc6-mips64_shlib_dep = libc6-mips64 (>= $(shlib_dep_ver))
mips64_configure_target = mips64el-linux-gnuabi64
mips64_CC = $(CC) -mabi=64
mips64_CXX = $(CXX) -mabi=64
mips64_rtlddir = /lib64
mips64_slibdir = /lib64
mips64_libdir = /usr/lib64

# build 32-bit (o32) alternative library
GLIBC_MULTILIB_PASSES += mips32
DEB_ARCH_MULTILIB_PACKAGES += libc6-mips32 libc6-dev-mips32
libc6-mips32_shlib_dep = libc6-mips32 (>= $(shlib_dep_ver))
mips32_configure_target = mipsel-linux-gnu
mips32_CC = $(CC) -mabi=32
mips32_CXX = $(CXX) -mabi=32
mips32_rtlddir = /lib
mips32_slibdir = /libo32
mips32_libdir = /usr/libo32

define libc6-dev-mips64_extra_pkg_install

mkdir -p debian/libc6-dev-mips64/usr/include
ln -sf mips64el-linux-gnuabin32/bits debian/libc6-dev-mips64/usr/include/
ln -sf mips64el-linux-gnuabin32/gnu debian/libc6-dev-mips64/usr/include/
ln -sf mips64el-linux-gnuabin32/fpu_control.h debian/libc6-dev-mips64/usr/include/

mkdir -p debian/libc6-dev-mips64/usr/include/mips64el-linux-gnuabin32/gnu
cp -a debian/tmp-mips64/usr/include/gnu/stubs-n64_hard.h \
        debian/libc6-dev-mips64/usr/include/mips64el-linux-gnuabin32/gnu

mkdir -p debian/libc6-dev-mips64/usr/include/sys
for i in `ls debian/tmp-libc/usr/include/mips64el-linux-gnuabin32/sys` ; do \
        ln -sf ../mips64el-linux-gnuabin32/sys/$$i debian/libc6-dev-mips64/usr/include/sys/$$i ; \
done

endef

define libc6-dev-mips32_extra_pkg_install

mkdir -p debian/libc6-dev-mips32/usr/include/mips64el-linux-gnuabin32/gnu
cp -a debian/tmp-mips32/usr/include/gnu/stubs-o32_hard.h \
        debian/libc6-dev-mips32/usr/include/mips64el-linux-gnuabin32/gnu

endef

# create a symlink for the 32 bit dynamic linker in /lib
define libc6-mips32_extra_pkg_install
mkdir -p debian/libc6-mips32/lib
ln -sf /libo32/ld.so.1 debian/libc6-mips32/lib
endef
