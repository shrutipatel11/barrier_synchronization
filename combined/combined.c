#include <stdio.h>
#include <omp.h>
#include "mpi.h"
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>

static int sense = 0;
static unsigned int threads;
static unsigned int count;
static unsigned int num_rounds;

void combined_init(int num_threads, int num_processes){
  threads = num_threads;
  count = num_threads;
  num_rounds = (int)ceil(log2(num_processes));
}

void combined_barrier(){
  int num_processes, rank;

  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int  sense_var = !sense;
    if(__sync_fetch_and_sub(&count,1)==1){
      if(num_processes > 1){
        for (int i = 0; i < num_rounds; i++) {
          int dest_rank = (rank+(int)pow(2,i)) % num_processes;
          MPI_Request request;
		      MPI_Isend(NULL, 0, MPI_INT, dest_rank, 0, MPI_COMM_WORLD, &request);
          int src_rank = (rank-(int)pow(2,i) < 0) ? rank-(int)pow(2,i) + num_processes : rank-(int)pow(2,i) ;
		      MPI_Recv(NULL, 0, MPI_INT, src_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	      }
      }
      sense = sense_var;
      count = threads;
    } else {
      while(sense_var != sense);
    }
}

int main(int argc, char **argv){
  int num_processes, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  omp_set_dynamic(0);
  int num_threads = strtol(argv[2], NULL, 10);
  omp_set_num_threads(num_threads);

  combined_init(num_threads, num_processes);
  struct timeval startTime, endTime;
  long avg_diff;

  #pragma omp parallel
  {
    gettimeofday(&startTime, NULL);
    for (int i = 0; i < 1000; i++) {
      combined_barrier();
    }
    gettimeofday(&endTime, NULL);
    avg_diff = ((endTime.tv_sec*1000000 + endTime.tv_usec) -  (startTime.tv_sec*1000000 + startTime.tv_usec))/1000;
  }

  printf("Average time(us) for 1000 iterations : %ld \n",avg_diff);
  MPI_Finalize();
  return 0;
}