#!/bin/bash

INSMOD=`which insmod`
MODULE=multiclip
MLTCLP_MAJOR=66

make clean
make

rm -rf /dev/multiclip
mkdir -p /dev/multiclip
for i in $(seq 0 128)
do
	mknod /dev/multiclip/board$i c $MLTCLP_MAJOR $i
	chmod a+r+w /dev/multiclip/board$i
done

echo "Loading module .."
$INSMOD "$MODULE".ko || exit 1

echo "module installed succesfully"
