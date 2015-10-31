#include <mpi.h>
#include <stdio.h>
int processorID;  
int numberOfProcessors;  


int main(int argc, char* argv[]){
MPI_Init(&argc, &argv);  
MPI_Comm_size(MPI_COMM_WORLD ,&numberOfProcessors);  
MPI_Comm_rank(MPI_COMM_WORLD ,&processorID);

int a, b, c;

if(!processorID){ // master
    scanf("%d", &a);
    MPI_Bcast(&a, 1, MPI_INT, 0, MPI_COMM_WORLD);

}
    //MPI_Barrier(MPI_COMM_WORLD);
else {
    //if(SLAVE){
            // blocks at Bcast waiting for a. Prints value read by MASTER
            MPI_Bcast(&a, 1, MPI_INT, 0, MPI_COMM_WORLD); 
            printf("%d\n" ,a);
            printf("%d\n" ,processorID);
    }
MPI_Finalize();  
return 0;  

}
