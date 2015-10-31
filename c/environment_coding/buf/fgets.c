#include <stdio.h>
int main(void){
	FILE *file=NULL;
	char buf[10];
	file = fopen("a","r");
	if(file==NULL){
		perror("error\n");
		return -1;
	}
	fgets(buf,10,file);	
	fputs(buf,stdout);
	return 0;
}
