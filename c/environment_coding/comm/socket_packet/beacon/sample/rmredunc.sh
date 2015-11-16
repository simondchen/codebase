#!/bin/bash
#
#2015年 11月 14日 星期六 16:19:52 CST
#<nicksimon109445@gmail.com>
#remove the redunctary seq in responser
#
#两个文件同时处理
if [ ! $# -eq 2 ];then
    echo "Usage:./rmredunc.sh init_seq resp_seq"
    exit -1
fi
#i=0
#while [ $i -lt 10 ]
#do
#    a[i]=$i
#    let i+=1
#done
#echo ${a[@]}
#exit 0
i=0
seq_init=(`cat "$1" | cut -d: -f1`)
seq_resp=(`cat "$2" | cut -d: -f1`)
for seq in ${seq_resp[@]}
do
    s=`echo ${seq_init[@]} | grep $seq`
    if [ "$s"x = ""x ];then
        #将这些多出来的seq放于数组中,更好的做法是先判断resp比init多几个,然后数目够后就退出
        seq_dup[i]=$seq
        let i+=1
        #echo ${seq_dup[$i-1]}
    fi
done

echo ${seq_dup[@]}

for seq in ${seq_dup[@]}
do
    #若使用shell变量,使用双引号,它会解析变量,单引号的话会原封不动的当做字符串处理
    sed -i "/$seq/d" "$2"
done

outinit=seq_initiator
outresp=seq_responser
awk -F : -v out=$outinit '{ $2=-$2; print $2 > out}' "$1"
awk -F : -v out=$outresp '{ $2=-$2; print $2 > out}' "$2"
