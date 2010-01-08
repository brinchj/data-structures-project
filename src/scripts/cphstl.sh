#!/bin/sh
 
SOURCE="../cphstl/Source"
SOURCES="Algorithm Assert Common Iterator List Type
Priority-queue-framework Queue"
 
mkdir -p "cphstl"
 
for i in ${SOURCES}; do
path="${SOURCE}/${i}/Code/"
    for fn in `find ${path} -type "f" -iname "*++"`; do
cp "${fn}" "cphstl";
    done
done
