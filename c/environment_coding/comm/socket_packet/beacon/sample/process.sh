#!/bin/bash
#
#2015年 11月 12日 星期四 11:45:52 CST
#<nicksimon109445@gmail.com>
#process seq
#

if [ ! $# -eq 2 ];then
    echo "Usage:./process.sh sample 1/2"
    exit -1
fi
case "$2" in
    1)
        out="seq_initiator"
        ;;
    2)
        out="seq_responser"
        ;;
    *)
        echo "Usage:./process.sh sample 1/2"
        exit -1
        ;;
esac

if [ -e $out ];then
    rm $out
fi

awk -F : -v outfile=$out '{$2=-$2;print $2 > outfile}' "$1"
#awk -F : -f seq.awk "$1"
