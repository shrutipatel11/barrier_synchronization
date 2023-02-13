#include <omp.h>
#include "gtmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
    int has_child[4];
    int child_not_ready[4];
    int parentsense;
    int *parentpointer;
    int *childpointers[2];
    int dummy;
} node_tree;

int global_sense = 1;
int nchild = 4 ;
node_tree **nodes;

void gtmp_init(int num_threads){
    int num_nodes = num_threads;
    //printf("Num threads %d\n",num_nodes);
    nodes = (node_tree **) malloc(num_nodes * sizeof(node_tree *));
    //sleep(2);
    //printf("Allocated array\n");
    for(int i = 0; i < num_nodes; i++){
        nodes[i] = (node_tree*) malloc(sizeof(node_tree));
    }

    //printf("Allocated space for nodes\n");
    for(int i=0;i<num_nodes;i++){
    	//printf("Inside for loop %d\n",i);
        for(int j=0;j<nchild;j++){
            if((nchild*i)+j+1<num_nodes) nodes[i]->has_child[j]=1;
            else nodes[i]->has_child[j]=0;
        }
        //printf("Updated has_child\n");
        if(i!=0) nodes[i]->parentpointer = &(nodes[(i-1)/nchild]->child_not_ready[(i-1)%nchild]);
        else nodes[i]->parentpointer = &nodes[i]->dummy;

        if((2*i)+1<num_threads) nodes[i]->childpointers[0] = &nodes[(2*i)+1]->parentsense;
        else nodes[i]->childpointers[0] = &nodes[i]->dummy;

        if((2*i)+2<num_threads) nodes[i]->childpointers[1] = &nodes[(2*i)+2]->parentsense;
        else nodes[i]->childpointers[1] = &nodes[i]->dummy;
	
	//printf("Updated child pointers\n");
        for(int j=0;j<nchild;j++) nodes[i]->child_not_ready[j] = nodes[i]->has_child[j];
        nodes[i]->parentsense = 0;
        //printf("Initialisation done\n");

    }
}

void gtmp_barrier(){
    //printf("Inside barrier\n");
    int vpid = omp_get_thread_num();
    int sense = global_sense;

    //printf("Done setting sense and vpid %d and %d \n",sense,vpid);
    while(nodes[vpid]->child_not_ready[0] == 1 || nodes[vpid]->child_not_ready[1] == 1|| nodes[vpid]->child_not_ready[2] == 1 || nodes[vpid]->child_not_ready[3]==1);
    //printf("Spin over\n");
    for(int i=0;i<nchild;i++) nodes[vpid]->child_not_ready[i] = nodes[vpid]->has_child[i];
    //printf("Has child set\n");
    *(nodes[vpid]->parentpointer) = 0;
    //printf("parent pointer set \n");
    if(vpid!=0) while(nodes[vpid]->parentsense != sense);
    *nodes[vpid]->childpointers[0] = sense;
    *nodes[vpid]->childpointers[1] = sense;

    if(sense==0) global_sense = 1;
    else global_sense = 0;

}

void gtmp_finalize(){
    free(nodes);
}

