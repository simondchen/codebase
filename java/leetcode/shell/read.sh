#!/bin/bash
#
#2015年 11月 09日 星期一 13:59:30 CST
#<nicksimon109445@gmail.com>
#return the 10th line
#

cat file.txt | sed -n -e '10p'
