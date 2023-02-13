#include <omp.h>
#include "gtmp.h"
#include <stdio.h>

//Counting barrier
static int sense = 0;
static unsigned int threads;
static unsigned int count;

void gtmp_init(int num_threads){
    threads = num_threads;
    count = num_threads;
}

void gtmp_barrier(){
    int  sense_var = sense;
    
    #pragma omp critical
    {
    	count = count - 1;
    }
    if(count == 0){
        sense = !(sense_var);
        count = threads;
    } else {
        while(sense_var == sense);
    }
}

void gtmp_finalize(){
    //Pass
}

