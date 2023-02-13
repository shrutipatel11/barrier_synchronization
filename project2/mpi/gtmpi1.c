#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include "gtmpi.h"

#define nchild_arrival 4
#define nchild_wakeup 2

struct node_t {
    //For arrival tree
    int p_arrival;
    int c_arrival[nchild_arrival];

    //For wake-up tree
    int p_wakeup;
    int c_wakeup[nchild_wakeup];

};

struct node_t *node;
int rank,num_processes;

void gtmpi_init(int num_processes){
    node = (struct node_t *)malloc(sizeof(struct node_t));
    for(int i = 0; i < nchild_arrival; i++) {
        if ((rank*nchild_arrival)+i+1 < num_processes) node->c_arrival[i] = (rank*nchild_arrival)+i+1;
        else node->c_arrival[i] = -1;
    }

    if(rank != 0) {
        node->p_arrival = (rank-1)/4;
        node->p_wakeup = (rank-1)/2;
    } else {
        node->p_arrival = -1;
        node->p_wakeup = -1;
    }

    if((rank*nchild_wakeup)+1 < num_processes) node->c_wakeup[0] = (rank*nchild_wakeup)+1; 
    else node->c_wakeup[0] = -1;

    if((rank*nchild_wakeup)+2 < num_processes) node->c_wakeup[1] = (rank*nchild_wakeup)+2; 
    else node->c_wakeup[1] = -1;
}

void gtmpi_barrier(){
    for (int i = 0; i < nchild_arrival; i++) {
        int crank = node->c_arrival[i];
        if (crank != -1) MPI_Recv(NULL, 0, MPI_INT, crank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    if (rank != 0) {
        MPI_Send(NULL, 0, MPI_INT, node->p_arrival, 0, MPI_COMM_WORLD);
        MPI_Recv(NULL, 0, MPI_INT, node->p_wakeup, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int i = 0; i < nchild_wakeup; i++) {
        int crank = node->c_wakeup[i];
        if (crank != -1) MPI_Send(NULL, 0, MPI_INT, crank, 1, MPI_COMM_WORLD);
    }
}

void gtmpi_finalize(){
    free(node);
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
