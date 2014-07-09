#!/bin/bash

INSMOD=`which insmod`
MODULE=multiclip
MLTCLP_MAJOR=66

make clean
make

gcc multi.c -omulticlip
ln -s `pwd`/multiclip /bin/multiclip
rm -rf /dev/multiclip
mkdir -p /dev/multiclip
for i in $(seq 0 127)
do
	mknod /dev/multiclip/board$i c $MLTCLP_MAJOR $i
	chmod a+r+w /dev/multiclip/board$i
done

mknod /dev/multiclip/board c $MLTCLP_MAJOR 128
chmod a+r+w /dev/multiclip/board

echo "Loading module .."
$INSMOD "$MODULE".ko || exit 1

echo "module installed succesfully"
