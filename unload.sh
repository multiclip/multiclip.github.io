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