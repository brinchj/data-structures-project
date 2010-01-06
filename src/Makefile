all: test

CFLAGS=-O2
INC=cphstl


test: test.c++ priority-queue.h++ priority-queue-pair.c++ priority-queue-pair-lazy.c++ \
	priority-queue-costless-meld.c++
	g++ -I${INC} ${CFLAGS} -o test test.c++

clean:
	rm test
