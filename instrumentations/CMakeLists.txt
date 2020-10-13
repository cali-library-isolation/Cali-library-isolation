cmake_minimum_required(VERSION 3.7)
project(cali-instrumentations)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(instrument-dbus-daemon SHARED instrument-dbus-daemon.c)
add_library(instrument-mandb SHARED instrument-mandb.c lib-gzip.h lib-internal.h)
add_library(instrument-xz SHARED instrument-xz.c utils.h lib-lzma.h)
add_library(instrument-gpg SHARED instrument-gpg.c utils.h lib-zlib.h lib-sqlite3.h lib-ksba.h)
add_library(instrument-imagemagick SHARED instrument-imagemagick.c utils.h)
add_library(instrument-filezilla SHARED instrument-filezilla.c utils.h lib-sqlite3.h)
add_library(instrument-socat SHARED instrument-socat.c utils.h lib-openssl.h)
