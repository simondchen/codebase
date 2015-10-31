#!/bin/sh
#ifpath.sh
if [ -z $PATH ];then
	echo '$PATH is no set'
else
	echo "\$PATH is $PATH"
fi
