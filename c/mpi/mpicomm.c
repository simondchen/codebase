#include <mpi.h>
#include <stdio.h>
#define MASTERPE 0
int main(int argc,char **argv){
	int rank;
	int nproc;
	int data[9][9];
	int err=0;
	int i;
	int j;
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nproc);	
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(nproc!=4){
		printf("4 processes needed\n");
		MPI_Abort(MPI_COMM_WORLD,err);
	}
	if(rank==MASTERPE){
		for(i=1;i<nproc;i++){
			MPI_Recv(data[(i-1)*3],3*9,MPI_INT,i,0,MPI_COMM_WORLD,&status);
			printf("received message from proc %d\n",i);
		}
		for(j=0;j<9;j++){
		for(i=0;i<9;i++){
			printf("%d\n",data[j][i]);
		}
		}
	}else{
		int ldata[3][9];
		int i,j;
		for(j=0;j<3;j++){
			int k=(rank-1)*3+j;
			for(i=0;i<9;i++)
			ldata[j][i]=k*9+i;
		}
		MPI_Send(ldata,3*9,MPI_INT,MASTERPE,0,MPI_COMM_WORLD);
	}
	MPI_Finalize();
}
