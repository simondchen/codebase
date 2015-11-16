#!/bin/bash
#
#2015年 11月 09日 星期一 14:22:07 CST
#<nicksimon109445@gmail.com>
#match one regur or another
#
sed -n -e '/^([0-9]\{3\}) [0-9]\{3\}-[0-9]\{4\}$/p' -n -e '/^[0-9]\{3\}-[0-9]\{3\}-[0-9]\{4\}$/p' file.txt
