#include <stdio.h>

struct matrix{
	//定义数组的维数
	int pl;
	int pr;
};

int m[4][4];
int s[4][4];

void matrix_chain(struct matrix *p,int size){
	int i,l,j,k;
	int q=0;
	for(i=0;i<=size-1;i++){
		//对角线都置为零
		m[i][i]=0;
	}
	//变量l用来表示每次的间隔	
	for(l=1;l<=size-1;l++){
		for(i=0;i<=size-1-l;i++){
			j=i+l;
			//赋一个很大的数
			m[i][j]=1000000;	
			for(k=i;k<j;k++){
				q = m[i][k] + m[k+1][j] + (p[i].pl) * (p[k].pr) * (p[j].pr);			
				if(q<m[i][j]){
					m[i][j] = q;
					s[i][j] = k;
				}
			}
		}
	}
}

void print_chain(i,j){
	if(i==j){
		printf("A");
	}else{
		printf("(");
		print_chain(i,s[i][j]);
		print_chain(s[i][j]+1,j);
		printf(")");
	}
}

int main(void){
	struct matrix p[3];
	int i,j;
	p[0].pl=10;
	p[0].pr=20;
	p[1].pl=20;
	p[1].pr=50;
	p[2].pl=50;
	p[2].pr=1;
	p[3].pl=1;
	p[3].pr=100;
	matrix_chain(p,4);
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			printf("%d ",m[i][j]);	
		}
		printf("\n");
	}
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			printf("%d ",s[i][j]);	
		}
		printf("\n");
	}
	print_chain(0,3);
	printf("\n");
	return 0;
}
