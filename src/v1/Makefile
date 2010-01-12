all: test


CFLAGS=-ggdb -O2 
#-Wfatal-errors
INC=cphstl

DEPS=pairing-heap-framework.h++ pairing-heap-framework.i++ \
	pairing-heap-node.c++ \
	pairing-heap-policy-strict.c++ \
	pairing-heap-policy-lazy-insert.c++ \
	pairing-heap-policy-lazy-increase.c++ Makefile

test: ${DEPS} test.c++
	g++ ${CFLAGS} -I${INC} -o test test.c++

test-realizator: ${DEPS} test-realizator.c++
	g++ ${CFLAGS} -I${INC} -o test-realizator test-realizator.c++

test-erase: ${DEPS} test-erase.c++
	g++ ${CFLAGS} -I${INC} -o test-erase test-erase.c++

clean:
	rm test
