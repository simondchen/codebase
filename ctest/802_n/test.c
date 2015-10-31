#include <stdio.h> 
#include "md5.h"
#include <string.h>
int main(){
	char *s="The quick brown fox jumps over the lazy dog";
	unsigned char b[43];
	unsigned char *r;
	memcpy(b,s,43);
	r=ComputeHash(b,43);
	//printf("%02x\n",r[0]);
	printf("%c\n",b[42]);
	return 0;
}
