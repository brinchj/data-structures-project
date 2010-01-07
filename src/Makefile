all: test test-rnd


CFLAGS=-O2 -g -pg
INC=cphstl

DEPS=priority-queue.h++ priority-queue-pair.c++ priority-queue-pair-lazy.c++ \
	priority-queue-costless-meld.c++

test: test.c++ ${DEPS}
	g++ -I${INC} ${CFLAGS} -o test test.c++

test-rnd: test-rnd.c++ ${DEPS}
	g++ -I${INC} ${CFLAGS} -o test-rnd test-rnd.c++

clean:
	rm test test-rnd
