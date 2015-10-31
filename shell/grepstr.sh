#!/bin/sh
#grepstr.sh
echo 'enter a list of names:'
read name
if echo $name | grep 'simon'
then
	echo 'simon is here'
else
	echo "simon is not here,but $name is here"
fi
