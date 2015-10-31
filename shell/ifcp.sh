#!/bin/sh
#ifcp.sh
if cp quote.txt quote.bak; then
	echo 'copy success'
else
	echo "basename $0:cp file error"
fi
