#!/usr/bin/env bash

set -e

tar -cJf cali.tar.xz \
cali-linker/cali \
cali-linker/ipc-communication-proto.bc \
cali-linker/libc-stubs.bc \
instrumentations/*.so \
libipc/libipc-instrument.a \
libipc/libipc-mprotect.a \
libipc/libipc-sequential.a \
libipc/libipc.a \
libnsjail/libkafel.a \
libnsjail/libnsjail.a \
../precompiled-libraries \
../patch-libtool.sh

echo "[OK] Created $(pwd)/cali.tar.xz"
