#!/bin/bash
args=$#
dir=${!args}
exclude=($@)
files=($(ls))
for file in ${exclude[@]}
do
	files=(${files[@]/$file})
done
echo ${files[@]}
for file in ${files[@]}
do
	if [ -d $file ]; then
		echo "is a directory"
		cp -rf $file $dir
	elif [ -f $file ]; then
		echo "is a file"
		cp $file $dir
	else
		echo "neither a file nor a directory"
	fi
done
