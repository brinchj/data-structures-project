#!/bin/sh


for i in *.c++; do
	cp ${i} cphstl/;
	g++ -O3 cphstl/${i} -o ${i}.o || exit 1;
done