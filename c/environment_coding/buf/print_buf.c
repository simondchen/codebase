#include <stdio.h>
int main(void){
	int i;
	for(i=0;i<2;i++){
		printf("-");
		fork();
	}
	printf("\n");
	return 0;
}
