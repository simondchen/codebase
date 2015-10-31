#include <stdio.h>
int main(void){
	char buf[10];
	while(fgets(buf,10,stdin)){
		fputs(buf,stdout);
	}
	return 0;
}
