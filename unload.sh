#!/bin/bash

RMMOD=`which rmmod`
MODULE=multiclip

export DISPLAY=:0
echo "Removing module .."
$RMMOD "$MODULE".ko
rm -rf /dev/multiclip
rm -rf /usr/sbin/multiclipd
update-rc.d -f multiclipd remove
xhost -

echo "module removed succesfully"

exit 0
