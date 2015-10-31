#!/bin/sh
#chmod.sh
for script in $(ls *.sh)
do
	if [ ! -x "$script" ]
	then
		chmod u+x $script	
		echo "chmod of $script success"
	fi
done
