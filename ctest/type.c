#include <stdio.h>
typedef struct my{
	int i;
	char a;
}__attribute__((__packed__)) mm;

int main(){
	mm m;
	m.i=1;
	m.a='d';
	printf("%d--%c\n",m.i,m.a);
	return 0;
}
