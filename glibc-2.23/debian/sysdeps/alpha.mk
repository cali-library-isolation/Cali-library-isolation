# configuration options for all flavours
libc = libc6.1

# build an ev67 optimized library
GLIBC_PASSES += alphaev67
DEB_ARCH_REGULAR_PACKAGES += libc6.1-alphaev67
alphaev67_configure_target = alphaev67-linux-gnu
alphaev67_CC = $(CC) -mcpu=ev67 -mtune=ev67 
alphaev67_CXX = $(CXX) -mcpu=ev67 -mtune=ev67 
alphaev67_slibdir = /lib/$(DEB_HOST_MULTIARCH)/ev67
