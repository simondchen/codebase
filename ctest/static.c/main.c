#include <stdio.h>
int fun(){
	static int count = 10;
	return count--;
}
int main(){
	int i;
	for(i=0;i<11;i++)
		printf("%d,%d\n",i,fun());
	return 0; 
}
