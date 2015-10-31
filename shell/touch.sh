#!/bin/bash
echo $#
if [ ! $# -eq 1 ]; then
	echo "Usage:touch.sh filename"
	exit -1
fi
filename=$1
#read -p "input the filename:" input
#if [ -z $input ]; then
#	echo "empty filename"
#	exit -1
#fi
date=`date`
echo $date
read -p "please description for the file:" input
desp=$input
mail="<nicksimon109445@gmail.com>"
cat <<EOF >> $filename
#
#$date
#$mail
#$desp
#
EOF
