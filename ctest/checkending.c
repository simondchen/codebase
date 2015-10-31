#include <stdio.h>
union{
	int i;
	char c;
}u;
	
int main(){
	int i=1;
	u.i=i;
	printf("%02x\n",u.c);
}
