#include <stdio.h>
#include <sys/uio.h>
#include <string.h>
/*
 *2015年 10月 28日 星期三 10:39:24 CST
 *<nicksimon109445@gmail.com>
 *write_v read_v test
 */
int main(int argc,char *argv[]){
	char buf1[]="abc\n",buf2[]="cde\n",buf3[]="def\n";
	struct iovec vector[3];
	vector[0].iov_base=buf1;
	vector[0].iov_len=strlen(buf1)+1;
	vector[1].iov_base=buf2;
	vector[1].iov_len=strlen(buf2)+1;
	vector[2].iov_base=buf3;
	vector[2].iov_len=strlen(buf3)+1;
	writev(1,vector,3);
	return 0;
}
