#!/bin/sh
#ifpwd.sh
DIRECTORY=$(pwd)
if  test "$DIRECTORY" != '/home/simon/test/shell' 
then
	echo 'you need to run this shell under /'
	exit 1
fi
