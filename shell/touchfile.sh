#!/bin/sh
read -p "input the filename: " fileuser
filename=${fileuser:-"filename"}
echo "$filename"
echo $(( 3 + 3 ))

date1=$(date --date='2 days ago' +%Y%m%d)
date2=`date --date='1 day ago' +%Y%m%d`
date3=$(date +%Y%m%d)

filename1=${filename}${date1}
filename2=${filename}${date2}
filename3=${filename}${date3}

touch "$filename1"
touch "$filename2"
touch "$filename3"
