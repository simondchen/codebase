#include <stdio.h>
#include <stdlib.h>


void lcs_length(int *c,int *b,int m,int n,char x[],char y[]){
	int (*cc)[n+1] = (int (*)[n+1])c;
	int (*bb)[n+1] = (int (*)[n+1])b;
	int i,j;
	cc[0][0]=0;
	for(i=1;i<=m;i++)
		cc[i][0]=0;
	for(j=1;j<=n;j++)
		cc[0][j]=0;
	for(i=1;i<=m;i++)
	{
		for(j=1;j<=n;j++) {
			if(x[i-1]==y[j-1])
			{
				cc[i][j]=cc[i-1][j-1]+1;
				//0 represents ==
				bb[i][j]=0;
			}else if(cc[i][j-1]>=cc[i-1][j]){ 
				cc[i][j]=cc[i][j-1]; 
				//1 represents <--- 
				bb[i][j]=2; 
			}else{
				cc[i][j]=cc[i-1][j];
				//2 represents up
				bb[i][j]=1;
			}
		}
	}
}

void print_lcs(int *b,int n,char x[],int i,int j){
	int (*bb)[n+1] = (int (*)[n+1])b;
	if(i==0 || j==0){
		return;
	}
	if(bb[i][j]==0){
		print_lcs(b,n,x,i-1,j-1);
		printf("%c ",x[i-1]);
	}else if(bb[i][j]==1){
		print_lcs(b,n,x,i-1,j);
	}else{
		print_lcs(b,n,x,i,j-1);
	}
}

int main(void){
	char x[7]="abcbdab";
	//char x[7]="acbdb";
	char y[6]="bdcaba";
	int m=sizeof(x);
	int n=sizeof(y);
	int *c=malloc((m+1)*(n+1)*sizeof(int));
	int *b=malloc((m+1)*(n+1)*sizeof(int));
	lcs_length(c,b,m,n,x,y);	
	int i;
	for(i=0;i<(m+1)*(n+1);i++)
	{
		printf("%d",c[i]);
		if((i+1)%(n+1)==0)
			printf("\n");
	}
	printf("\n");
	for(i=0;i<(m+1)*(n+1);i++)
	{
		printf("%d",b[i]);
		if((i+1)%(n+1)==0)
			printf("\n");
	}
	printf("\n");
	print_lcs(b,n,x,m,n);
	printf("\n");
	return 0;
}
