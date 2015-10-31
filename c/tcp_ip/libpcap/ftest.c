#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(){
	FILE *file;
	char buf[512];
	int num;
	int ch;
	char *pos;
	int split=':';
	file=fopen("./sample","r");
	while(fgets(buf,sizeof(buf),file))
	{
		//printf("%s",buf);
		pos=strrchr(buf,split);
		buf[strlen(buf)-3]='\0';
		printf("%d\n",atoi(pos+2));
	}
	return 0;
}
