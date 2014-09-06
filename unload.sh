#!/bin/bash

RMMOD=`which rmmod`
MODULE=multiclip

echo "Removing module .."
$RMMOD "$MODULE".ko
rm -rf /dev/multiclip
rm -rf /usr/sbin/multiclipd

echo "module removed succesfully"
exit 0