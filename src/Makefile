all: test

CFLAGS=-O2
INC=cphstl


test: test.c++
	g++ -I${INC} ${CFLAGS} -o test test.c++

clean:
	rm *.o
