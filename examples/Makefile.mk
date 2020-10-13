CALI=$(shell pwd)/../../cmake-build-relwithdebinfo
CC="clang-7"
CXX="clang++-7"
CFLAGS=-flto -O2 -fPIC -g
LDFLAGS=-L. -Wl,-rpath,. -g
LDFLAGS_CALI=-fuse-ld=${CALI}/cali-linker/cali
LDFLAGS_REF=-fuse-ld=lld
CALI_DEPENDENCIES=${CALI}/cali-linker/cali ${CALI}/libipc/libipc.a ${CALI}/libipc/libipc-mprotect.a ${CALI}/libnsjail/libnsjail.a

all: ${NAME}-cali ${NAME}-ref

clean:
	rm -rf *.o ${NAME}-cali ${NAME}-ref *.bc *.so

libtest.so: ${SRC_LIBRARY}
	${CC} ${CFLAGS} -shared ${SRC_LIBRARY} -o libtest.so

${NAME}-cali: libtest.so ${SRC_PROGRAM} config.yaml ${CALI_DEPENDENCIES}
	${CC} ${CFLAGS} ${LDFLAGS} ${LDFLAGS_CALI} ${SRC_PROGRAM} -o ${NAME}-cali -ltest

${NAME}-ref: libtest.so ${SRC_PROGRAM}
	${CC} ${CFLAGS} ${LDFLAGS} ${LDFLAGS_REF} ${SRC_PROGRAM} -o ${NAME}-ref -ltest

run: ${NAME}-cali
	./${NAME}-cali

run-ref: ${NAME}-ref
	./${NAME}-ref