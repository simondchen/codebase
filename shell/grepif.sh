#!/bin/sh
#grepif.sh
if grep 'Office' quote.txt
then
	echo 'Office is in the file'
else
	echo 'Office is not in the file'
fi
