Section: misc
Priority: optional
Standards-Version: 3.9.2
Package: multiclip
Maintainer: Kadir CETINKAYA <kadircetinkaya.06.tr@gmail.com>
Description: multiple clipboards
Files: COPYING /usr/local/multiclip/
	Makefile /usr/local/multiclip/
	HEADER /usr/local/multiclip/
	HELP /usr/local/multiclip/
	load.sh /usr/local/multiclip/
	multiclipd.c /usr/local/multiclip/
	multiclip.c /usr/local/multiclip/
	multiclip.h /usr/local/multiclip/
	README /usr/local/multiclip/
	unload.sh /usr/local/multiclip/
	multiclip /bin/
	multiclipd /etc/init.d/

File: postinst
	#!/bin/sh
	/usr/local/multiclip/load.sh

File: prerm
	#!/bin/sh
	/usr/local/multiclip/unload.sh