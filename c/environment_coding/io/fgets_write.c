#include <stdio.h>

int main(void){
	struct _IO_FILE *file,*out;
	char buf[10];
	if((file=fopen("a","r"))==NULL){
		perror("open error");		
		return -1;
	}
	if((out=fopen("b","a+"))==NULL){
		perror("open for write error");		
		return -1;
	}
	while(fgets(buf,10,file)!=NULL){	
		fwrite(buf,10,1,out);
		//fputs(buf,out);
	}
	return 0;
}
