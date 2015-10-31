#include <stdio.h>
int main(void){
	char *buf="-";
	setvbuf(stdout,NULL,_IONBF,0);
	fwrite(buf,1,1,stdout);
	while(1);
	return 0;
}
