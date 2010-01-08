all: test


CFLAGS=-ggdb -O2
INC=cphstl

DEPS=pairing-heap-framework.h++ pairing-heap-framework.i++ \
	pairing-heap-policy-pair.c++ Makefile

test: ${DEPS} test.c++
	g++ ${CFLAGS} -I${INC} -o test test.c++

clean:
	rm test test-rnd
