#include <stdio.h>
#include <string.h>
int main(){
	FILE *file;
	char s[100];
	if((file=fopen("./seq","r"))==NULL)
	{
		fprintf(stderr,"open error\n");
	}
	//while(fgets(s,26,file))
	if(fgets(s,27,file))
	{
		fprintf(stdout,"%s",s);
		printf("%d\n",(int)strlen(s));
	}
	return 0;
}
