#!/bin/sh
#ifdirect.sh
DIRECTORY=$1
if [ -z "`ls -A $DIRECTORY`" ];then
	echo "$1 is empty"
else
	echo "$1 is not empty"
fi
