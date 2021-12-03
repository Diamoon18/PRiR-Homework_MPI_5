#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mpi.h"

int main (int argc, char** argv) {

    int nr_proc, size;
    MPI_Datatype diag;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &nr_proc);

    int matrix[size][size];

    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if (i==j) 
                matrix[i][j] = nr_proc;
 	        else 
                matrix[i][j] = 0;
        }
     }
	
    if (nr_proc == 0) {
       printf("Process nr-0 -> matrix before communication:\n");
       for (int i = 0; i < size; i++) {
           for (int j = 0; j < size; j++){
               printf("%d ", matrix[i][j]);
           }
           printf("\n");
       }
       printf("\n"); 
    }

  
    MPI_Type_vector(size, 1, size+1, MPI_INT,&diag);
    MPI_Type_commit(&diag);
	
	
    MPI_Gather(matrix, 1, diag, matrix, size, MPI_INT, 0, MPI_COMM_WORLD);

    
    if (nr_proc == 0) {
       printf("Process nr-0 -> matrix after communication:\n");
       for (int i = 0; i < size; i++) {
           for (int j = 0; j < size; j++){
                printf("%d ", matrix[i][j]);
           }
           printf("\n");
       }
       printf("\n");
    }

    MPI_Type_free(&diag);
    MPI_Finalize();
    return 0;
}

