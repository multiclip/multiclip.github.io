#!/usr/bin/python
"""
    MultiClip - more than one clipboard to cp/paste
    Copyright (C) 2014 Kadir CETINKAYA - Esref OZTURK

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    kadir@kadircetinkaya.me
    esref@esrefozturk.com
"""

import sys, os

try:
	f = open('/var/log/multiclip.log', 'a+')
	for arg in sys.argv:
		f.write(arg+" ")
	f.write("\n")

	#f.write(os)
	
	if len(sys.argv)>=2:
		if sys.argv[1] == "PASTE":
			f.write("PASTE")
			os.system("xclip -selection c -i /dev/multiclip/board30")
		elif sys.argv[1] == "COPY":
			f.write("COPY")
			os.system("xclip -selection c -o >> /var/log/multiclip.log")
	f.write("\n")
	f.close()
except Exception as e:
	f.close()
	
