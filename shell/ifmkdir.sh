#!/bin/sh
#ifmkdir.sh
DIR=$1
if [ -z "$DIR" ];then
	echo "Usage:$0 directory"
	exit 1
fi
	if [ -d $DIR ]
	then :
	else
		echo -n "create it now?[y..n]:"
		read ANS
		if [ "$ANS"=="y" ] || [ "$ANS"=="Y" ];then
			echo "creating now"
			mkdir $DIR
			if [ $? != 0 ];then
				echo "Error creating"
				exit 1
			fi
		else :
		fi
	fi	
