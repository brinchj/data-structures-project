all: test

CFLAGS=-O2
INC=cphstl


test: test.c++ priority-queue.h++ priority-queue-pair.c++ priority-queue-pair-lazy.c++
	g++ -I${INC} ${CFLAGS} -o test test.c++

clean:
	rm *.o
