#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include "gtmpi.h"
#include <math.h>

int num_processes, rank;
int num_rounds;

void gtmpi_init(int num_processes){
   num_rounds = (int)ceil(log2(num_processes));
}

void gtmpi_barrier(){
    for (int i = 0; i < num_rounds; i++) {
        int dest_rank = (rank+(int)pow(2,i)) % num_processes;
		MPI_Send(NULL, 0, MPI_INT, dest_rank, 0, MPI_COMM_WORLD);
        int src_rank = (rank-(int)pow(2,i) < 0) ? rank-(int)pow(2,i) + num_processes : rank-(int)pow(2,i) ;
		MPI_Recv(NULL, 0, MPI_INT, src_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
}

void gtmpi_finalize(){
    //Pass
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  
  gtmpi_init(num_processes);

  double t1, t2;
  t1 = MPI_Wtime();
  for (int i = 0; i < 10000; i++) gtmpi_barrier();
  t2 = MPI_Wtime();

  printf("Average time used in micro second %f\n", (t2 - t1) * 100);
  gtmpi_finalize();
   MPI_Finalize();
   return(0);
}
