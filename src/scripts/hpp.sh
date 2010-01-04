#!/bin/sh


for i in *.h++; do
	cp ${i} cphstl/;
	g++ -O3 cphstl/${i} || exit 1;
done