#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "gtmp.h"
#include <sys/time.h>

int main(int argc, char** argv)
{
  int num_threads, num_iter=10000;

  if (argc < 2){
    fprintf(stderr, "Usage: ./harness [NUM_THREADS]\n");
    exit(EXIT_FAILURE);
  }
  num_threads = strtol(argv[1], NULL, 10);

  omp_set_dynamic(0);
  if (omp_get_dynamic())
    printf("Warning: dynamic adjustment of threads has been set\n");

  omp_set_num_threads(num_threads);
  
  gtmp_init(num_threads);
  struct timeval startTime, endTime;
  long avg_diff;
  
#pragma omp parallel shared(num_threads)
   {
     int i;
     gettimeofday(&startTime, NULL);
     for(i = 0; i < num_iter; i++){
       gtmp_barrier();
     }
     gettimeofday(&endTime, NULL);
     avg_diff = ((endTime.tv_sec*1000000 + endTime.tv_usec) -  (startTime.tv_sec*1000000 + startTime.tv_usec))/10000;
   }

   printf("Average time(us) for 10000 iterations : %ld \n",avg_diff);
   //printf("Barrier run finished\n");
   gtmp_finalize();

   return 0;
}
