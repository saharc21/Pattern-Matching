#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <cuda_runtime.h>
#include "slist.c"
#include "pattern_matching_aho.c"


__device__ void print_pm(pm_t *pm);
__device__ void print_state(pm_state_t *state, int tabs, int *is_need, int is_get);
__device__ void print_tabs(int tabs, int *is_need);
__global__ void search_patterns(pm_t *pm,  char *s,int lenOfS, slist_t *list);
__global__ void initilaizeTree(pm_t * cu_pm);
void cleanup_destroy(slist_t *list);

char s1[PM_CHARACTERS];
__device__ int NUM_OF_THREADS;

int main(int argc, char *argv[]) // ********************* Aho - parallel ***********************
{
    FILE *fp;
    fp = fopen("/home/jceproject/Desktop/Final Project/Pattern-Matching/chars_test.txt", "r");
    fgets(s1, PM_CHARACTERS, (FILE *)fp);
    int numOfChars = strlen(strtok(s1, "\0"));
    
    // NUM_OF_THREADS = numOfChars - 113;
    NUM_OF_THREADS = 10;
    
    // pthread_t *tids = (pthread_t *)malloc(sizeof(pthread_t) * NUM_OF_THREADS);
    
    // pm_t *pm = (pm_t *)malloc(sizeof(pm_t));
    pm_t * cu_pm;
    cudaMallocManaged(&cu_pm, sizeof(pm_t));
    
    if (!cu_pm)
    {
        return -1;
    }
    
    // if (pm_init(pm) == -1)
    // {
    //     free(pm);
    //     return -1;
    // }

    initilaizeTree<<<1,1>>>(cu_pm);
    char * cu_s1;
    cudaMallocManaged(&cu_s1, (numOfChars+1)*sizeof(char));
    cudaMemcpy(cu_s1, s1,numOfChars,cudaMemcpyHostToDevice);

    
    clock_t begin;
    clock_t end;
    begin = clock();
    
    // cudaMemcpy(cu_pm, pm,sizeof(*pm),cudaMemcpyHostToDevice);

    // cudaMallocManaged(&(cu_pm->zerostate), sizeof(pm->zerostate));
    // cudaMemcpy(cu_pm->zerostate, pm->zerostate,sizeof(pm->zerostate),cudaMemcpyHostToDevice);
    
    // cudaMallocManaged(&(cu_pm->zerostate->_transitions), sizeof(pm->zerostate->_transitions));
    // cudaMemcpy(cu_pm->zerostate->_transitions, pm->zerostate->_transitions,sizeof(pm->zerostate->_transitions),cudaMemcpyHostToDevice);



    slist_t *list = NULL;
    int len = strlen(s1);


    // printf("gpu tree: \n");
    // print_pm(cu_pm);
    search_patterns<<< 1, NUM_OF_THREADS>>>(&(*cu_pm), cu_s1, len, list);

    printf("in main\n");
    cudaDeviceSynchronize();


    // cudaFree(cu_pm);
    // cudaFree(cu_s1);
    
    cleanup_destroy(list);

    end = clock();
    // fclose(fp);
    printf("\nExecuted time is: %f ms. \n\n", ((double)(end - begin) / CLOCKS_PER_SEC) * 1000);
    return 0;
}


__global__ void initilaizeTree(pm_t * cu_pm)
{
    if(pm_init_gpu(cu_pm) == -1)
    {
        return;
    }
    if (pm_addstring_gpu(cu_pm, (unsigned char *)"hello", 5) == -1){
        return;
    }    
    if (pm_addstring_gpu(cu_pm, (unsigned char *)"GPU", 3) == -1){
        return;
    }
    if (pm_makeFSM_gpu(cu_pm) == -1){
        return;
    }
    print_pm(cu_pm);    
}

__global__ void search_patterns(pm_t *pm,  char *s, int lenOfS, slist_t *list)
{
    pm_fsm_search<<< 1,1>>>(pm->zerostate, (unsigned char *)(&s[threadIdx.x]), lenOfS-threadIdx.x, threadIdx.x);
}

void cleanup_destroy( slist_t *list ){
    slist_destroy(list, SLIST_FREE_DATA);
    free(list);
    // pm_destroy(pm);
}

__device__ void print_pm(pm_t *pm)
{
    if (!pm)
    {
        return;
    }
    printf("state(id, fail state id)\n");
    printf("(root)--------");
    int *b = (int *)malloc(sizeof(int) * 100);
    int i;
    for (i = 0; i < 100; i++)
    {
        b[i] = 0;
    }
    print_state(pm->zerostate, 1, b, 0);
    printf("\n");
    free(b);
}

__device__ void print_state(pm_state_t *state, int tabs, int *is_need, int is_get)
{
    slist_node_t *node = slist_head(state->_transitions);
    if (!node)
    {
        printf("\n");
        print_tabs(tabs, is_need);
        return;
    }
    int use_tabs = 0;
    while (node)
    {
        pm_labeled_edge_t *edge = (pm_labeled_edge_t *)slist_data(node);
        int is_out_state = slist_head(edge->state->output) ? 0 : -1;
        if (use_tabs != 0)
        {
            print_tabs(tabs, is_need);
            printf("%c---", edge->label);
        }

        else
        {
            printf("--|%c---", edge->label);
            use_tabs++;
        }

        int id = edge->state->id;
        int fail_id = edge->state->fail ? edge->state->fail->id : 0;
        is_out_state == 0 ? printf("(") : printf("-");
        printf("(");
        if (id < 10)
            printf(" ");
        printf("%d,", id);
        if (fail_id < 10)
            printf(" ");
        printf("%d)", fail_id);
        is_out_state == 0 ? printf(")") : printf("-");
        node = slist_next(node);
        if (node)
        {
            is_need[state->depth] = 1;
        }

        else
        {
            is_need[state->depth] = 0;
        }
        print_state(edge->state, tabs + 1, is_need, is_get);
    }
    // print_tabs(tabs, is_need);
    cudaDeviceSynchronize();
}

__device__ void print_tabs(int tabs, int *is_need)
{
    int i;
    for (i = 0; i < tabs; i++)
    {
        printf("\t\t\t\t");
        if (is_need[i] == 1)
        {
            printf("|");
        }
    }
}