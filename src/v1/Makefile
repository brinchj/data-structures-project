all: test


CFLAGS=-ggdb -O2
INC=cphstl

DEPS=pairing-heap-framework.h++ pairing-heap-framework.i++ \
	pairing-heap-policy-strict.c++ \
	pairing-heap-policy-lazy-insert.c++ \
	pairing-heap-policy-lazy-increase.c++ Makefile

test: ${DEPS} test.c++
	g++ ${CFLAGS} -I${INC} -o test test.c++

clean:
	rm test
