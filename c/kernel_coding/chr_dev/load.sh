#!/bin/sh

module="my_chr"
device="my_chr"
mode="664"

#将模块装载到内核
/sbin/insmod ./$module.ko || exit 1

#在/proc/device文件中过滤出主设备号
major=$(cat /proc/devices | grep $module | sed 's/^ *//g ' | cut -f1 -d ' ')

echo $major
mknod /dev/${device}0 c $major 0
