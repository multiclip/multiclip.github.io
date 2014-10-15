#!/bin/bash

#    MultiClip - more than one clipboard to cp/paste
#    Copyright (C) 2014 Kadir CETINKAYA - Esref OZTURK

#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.

#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
#    kadir@kadircetinkaya.me
#    esref@esrefozturk.com

INSMOD=`which insmod`
MODULE=multiclip
MLTCLP_MAJOR=66

cd /usr/local/multiclip/

make -C /usr/local/multiclip/ clean
make -C /usr/local/multiclip/

gcc /usr/local/multiclip/multiclipd.c -o /usr/sbin/multiclipd
rm -rf /dev/multiclip
mkdir -p /dev/multiclip
for i in $(seq 0 127)
do
	mknod /dev/multiclip/board$i c $MLTCLP_MAJOR $i
	chmod a+r+w /dev/multiclip/board$i
done

mknod /dev/multiclip/board c $MLTCLP_MAJOR 128
chmod 777 /dev/multiclip/board

echo "Loading module .."
$INSMOD "$MODULE".ko

echo "module installed succesfully"

export DISPLAY=:0

xhost +
update-rc.d multiclipd defaults
service multiclipd start
