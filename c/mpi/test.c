#include <stdio.h>
int main(){
	int a[10][10];
	int *p;
	p=a[1]+2;
	*p=1;
	printf("%d\n",a[1][2]);
	return 0;
}
