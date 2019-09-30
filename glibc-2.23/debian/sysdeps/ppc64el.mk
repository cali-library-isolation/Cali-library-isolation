# configuration options for all flavours
extra_config_options = --enable-multi-arch --with-cpu=power8

# main library
libc_rtlddir = /lib64
libc_extra_cflags = -O3 -fno-tree-vectorize
