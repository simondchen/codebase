#!/bin/sh
#ifparam.sh
if [ $# -lt 3 ];then  #what is -lt ???
	echo "usage:$0 parm1 parm2 parm3"
	exit 1
fi
echo "parm1:$1"
echo "parm2:$2"
echo "parm3:$3"
