#include <stdio.h>
#include <string.h>
int main(){
	char buf[1024];
	char a[1024] = {'a','b','c','\n'};
	sprintf(buf,"sprintf:%d",1);
	printf("%d\n",(int)strlen(buf));
	printf("%d\n",(int)strlen(a));
	return 0;
}
