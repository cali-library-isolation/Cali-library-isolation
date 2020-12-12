#!/usr/bin/env bash

set -e

if [ -f "build-aux/ltmain.sh" ]; then
  sed -i 's/-fuse-linker-plugin|-fstack-protector/-fuse-linker-plugin|-fuse-ld=*|-fstack-protector/' build-aux/ltmain.sh
  echo "[OK] patched build-aux/ltmain.sh"
fi

if [ -f "./libtool" ]; then
  sed -i 's/-fuse-linker-plugin|-fstack-protector/-fuse-linker-plugin|-fuse-ld=*|-fstack-protector/' ./libtool
  echo "[OK] patched ./libtool"
fi

