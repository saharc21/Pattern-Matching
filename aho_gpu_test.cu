#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include <cuda_runtime.h>
#include "slist.c"
#include "pattern_matching_aho.c"


__device__ void print_pm(pm_t *pm);
__device__ void print_state(pm_state_t *state, int tabs, int *is_need, int is_get);
__device__ void print_tabs(int tabs, int *is_need);
__global__ void search_patterns(pm_t *pm,  char *s,int lenOfS, int offset);
__global__ void initilaizeTree_aho(pm_t * cu_pm);
void cleanup_destroy(slist_t *list);

char s1[PM_CHARACTERS];
__device__  char * cu_s1;
__device__  pm_t * cu_pm; 
int MAX_THEADS_FOR_EPOCH;
int NUM_OF_THREADS;


int main(int argc, char *argv[]) // ********************* Aho - parallel ***********************
{
    FILE *fp;
    fp = fopen("/home/jceproject/Desktop/Final Project/Pattern-Matching/chars_stream.txt", "r");
    fgets(s1, PM_CHARACTERS+1, (FILE *)fp);
    int numOfChars = strlen(strtok(s1, "\0"));
    
    MAX_THEADS_FOR_EPOCH = 1024;
    
    if(numOfChars <= MAX_THEADS_FOR_EPOCH){    
        NUM_OF_THREADS = numOfChars;
    }    

    else{
        NUM_OF_THREADS = MAX_THEADS_FOR_EPOCH;
    }
    

    // pm_t * cu_pm;
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

    initilaizeTree_aho<<<1,1>>>(cu_pm);
    // char * cu_s1;
    cudaMallocManaged(&cu_s1, (numOfChars+1)*sizeof(char));
    cudaMemcpy(cu_s1, s1,numOfChars,cudaMemcpyHostToDevice);

    
    clock_t begin;
    clock_t end;
    begin = clock();

    int len = strlen(s1);


    // printf("gpu tree: \n");
    // print_pm(cu_pm);

    int loop_limit = numOfChars/NUM_OF_THREADS;
    if(numOfChars%NUM_OF_THREADS > 0)
    {
        loop_limit++;
    }    
    int offset;
    int threads = NUM_OF_THREADS;
    for (offset = 0; offset <= loop_limit; offset++){
        if(offset == loop_limit){
            threads = numOfChars%NUM_OF_THREADS;
        }
        search_patterns<<<threads,1>>>(&(*cu_pm), cu_s1, len, MAX_THEADS_FOR_EPOCH*offset);
    }
    cudaDeviceSynchronize();


    // cudaFree(cu_pm);
    // cudaFree(cu_s1);
    end = clock();
    printf("\nExecuted time is: %f ms. \n\n", ((double)(end - begin) / CLOCKS_PER_SEC) * 1000);
    
    // cleanup_destroy(list);

    end = clock();
    // fclose(fp);
    return 0;
}


__global__ void initilaizeTree_aho(pm_t * cu_pm)
{
    if(pm_init_gpu(cu_pm) == -1)
    {
        return;
    }
    if (pm_addstring_gpu(cu_pm, (unsigned char *)"DR5HY@yLcG*6GD7sXf1paMwfLiD&Gdub98QW0FzqpWjEHxMQZ0kJoZo#hJVJ9QGD3VdtIadTp4me*sfAzT3NjL0$4L2FC6NzmDd6mhtyW04o@*qO#le31L^MO$X#CpFYxHSegmPDjiCf8R^cmjtARILA*Z^e!n&pPEC&WKKNHO7I4P6XPA0&#75RJ7ghWsHXz^r3vSh@OzJK#jqaZBvz9x4EF*$hlfcrykrKdLgio7AaxLiuDD805i@4&s4WWusG7fsf5iNllmMrVkb7RtzSU@fZo1bLVaceLA#CkuWSpvaD0b#ZGwkcTesehS5UwKUObzILks#sdUJDBdadWpgP&lanjk^CbqjbumPvz2chvCa&Sc120z5p60TEDBc9esY3vbtq8w5j^M0K$$UYZB2NMAUjJ*kK^1SoX5f03yxAh0#!R$92Ui&88zhwxsnq2evaygzksj##w1Sx^jvknA&Vyba^Dt1j8CM", 479) == -1){
        return;
    }
    if (pm_addstring_gpu(cu_pm, (unsigned char *)"#^1wds5KDaplo&!mEFt7Xak&SFpa0fNP8xZwdmRXS7tZ2!B9P4na&yHXT*JAsuGUmeYypacsvkxnkUDgC*GWmUBf5WKT8E2k!UF@pK3g86ansmrmOfspzeSGjJmjfmg9#*pOYs@b^cGZLycrtv@*wrxCgeVvb&4v3mZwRwDgdEPZMQX0lyANM23HL3oWm#dm2nS6A!OAGq$QjoA9*DK&*ffQ2&wZxJf06Vxck1om!ccKe7qpqZk5iJECurt!3s6EpTPdpgxeKAZLlR4Bt1V#5ZBfX4tW0^3I0zlJvomrNOiOKi!vK^8lX36JkuJCzhrvAKk#aC^RtQAtS$qK^A*30I^$Ed3KZ82rgwG7z4mswABIzOH*3bm!6C@dCx", 378) == -1){
        return;
    }
    if (pm_addstring_gpu(cu_pm, (unsigned char *)"lKEX*VA$uFqJ2FfJE5*EI!MU5vMg!KGq9xP$fAshHrIVXgQfL45&1tT&TOUcTcKUYGoAgTEIrl6NiL8n&DPIvmomUpBp2sHM#*g1V2&xsYFOOznYQwzoqnBp2w*P", 124) == -1){
        return;
    }    
    if (pm_addstring_gpu(cu_pm, (unsigned char *)"dA1sI0NGxYzGPWQreWrptknA9SMf44Bfl9hXDAV!TvWSGB!1e66$2wG6tx&Re*LoEBwJ!dUL@qOJuJ2kffeazN1Mhcvuc4$J7Xjox$1fAwSW6ziIvlv7PstENidAXl7DnlR4W7*yCjz$XhPvmWLflmvohg!XxTI*c#Ra5QRL2HKmc&PCzjQM*wzjSbbxLqC*G$Qe^nFIM8nEzfV4keXikucg$Z1GCW04eBYTWp*HCwrHHGqtC*XmRsPx@iGMQg5yGkeXQMiiQJUyLnp1f3PC4evR@^zijZ$H&XkZ7XuTUTcVV8vkIUpzoTRjYw$C1arvd&Y!&rFY35*RFDj!bv&&yl*sN4W3ALkDGXudUKRlPFYy!D89kXQmwkyHjO*&RAv#eJ2vBN7#*NMN*eY0bD$xQ0Rn7ywTUcch^fiU@3s$$2oGH2zjVLKXhHFVeRb7X9al9T^vwldPKFJDgqJgVky!QgWQ5g#*^lsEMykUV6o#h2UPhoy3@rqscqeBJ!GJwEJWj$Li^bynEWhtTsojwCiaB29gU&thp0O5$PI1Lq9UzpLTc8ZZ1$OgJSN11xJzpjC1&mNh7T@wD8j&M&Rjk^TXZckBHT9fVmS1!F0GHslpaBTZZ#DD6p@!CkDk", 632) == -1){
        return;
    }
    if (pm_addstring_gpu(cu_pm, (unsigned char *)"gGoojjNNZnj09JmxraEvWyaO9raPqdABd0iZSqoMVyPtyIfpLolELRVtyAjYyIE4x64spFYW6jXCE5CIRMdxaO0YfzlvkFYHwM1gSUbOqxuXkIYg9WFuQ6MG!VFoyoJ1xk0Y96mSP7eUhOmZeU9rzItkRzRVfvlKSvmxYwX7yoOIPVcxhL46ufZnn199V225aeKi8qXrCoUxiGELsNpV", 212) == -1){
        return;
    }
    if (pm_addstring_gpu(cu_pm, (unsigned char *)"E4yE1Zj7vkE2mNzp8Ikimo2lIEAFFyKZEsz7N7Sue629yw8RJ3zvHxWleH9KJqt0t9GTfL75eZqayVQOzHagHwLdacJHjaZ2ul4zHCb1af7CPpsSp6wstt8xBhqAaSydUCJcoFyetcxEh0zM1MiMDKEyDHmq4OhvSMFbI3SbgGoZ8SiMvCcWX2b0YsQVgF65LoEEKIL71xye0pBNxD4bNoQgvFTcNxK33mGB37oNekERK5qUOvO9xpKlWIaTDtkOTTnXL6WjK0WKbONOCw8x1kCKwk12aVUgM27L3kLTIdo0s6VUVAAGeqYFNRYzyn2F7qJSpEiygYaNdLkiJdLcf0QguBhhIOfTKh4henpss2Z2wBStst1lZvrZL6R5OjwMEFsUEsUjS8z0nOoFEAl1TOY4P9elaAPQ7ornOFAUybAVOZEmlu2atvLJVvv1VCsIEmwnezgyULFG886hbVRO5Z9uxWti2G8FmcI09JQkuXZ3vAyyEUVmLSJhc1tKMVpv8dRKq61ROSfs6f1qhepGxoJZQTkMRMaSii71JW6Fv0XzSSrsYEylWgkRNS1JUkB3Qe8YeryJ9ubX5I5VCd0JoStAY704klkXDUE2bKNMFALcFNjVDUdjpeu7q1TslRLFbpSrdfionpmmFyIzopE4CAYkz2jlablPHLUeBMjGgiBgbYOulSIKSuwx7rlvFCbhyTsibHtZxEY6TQEOkTWSPMXqp", 729) == -1){
        return;
    }
    if (pm_addstring_gpu(cu_pm, (unsigned char *)"YBx1TflInCLoTvSMvcgYv2Xd916G1nwiVqdpgmFdvFykLJPJRtSfVD2oiGccXNeSuTq1Pqt25hfFtIKGJDO8SVQdI5W24qLsUUJIgo31tAjbg4D6HFngMcApiewrd5CAWW41an8N5h6ZXtYM5pLytzkWddE4esqvEKHM9eRBdVpkbC0pXmVYeNCoSSjR7pwXxfSw4nk7Q0LSAdbbMgOIijc8z7wvfRLdbzuIgjVbR9USyiNm4fSYizzqc4fbQGdCx7nuNk1hxmFhS3Wpp70rP3Q7hrD4VJe3p2HlneNnpoLbJOWhJwVt7ze", 311) == -1){
        return;
    }    
    if (pm_addstring_gpu(cu_pm, (unsigned char *)"lC14n8wwW12TxLorJKoO5PLRqSuafVm5MFC2OEzhvY5XD07nHYwGdzfDQ8XmUzUmyKNPjBp62AxJAJ97V9WPYAKHJDmKZ4AvSH1n2Ho651F9WgqbXzj3Abdr3m5wtDY8cb3kS3lRYcmNepUlOSuHw2Gy3EtPFLHVLyF2Fjls0mK6N2kxOR023jQTWwpqdHUN6MnTLn61Mx2c9sDrwSmvxaPcsGXhA6pnnPczdJhts6XvdIXmKaQzYpaC3niN82tSjwtR0VSsOeAE3WHZvjTxAduYagif23clk23QIdoenFqpxnqBLhFD9ShEfSuot28Ta0hcaIyxKuMi9KjD1f8HRM7mZ7zwQ3KaeSLoi7ovgWcUXMFkkCQsZ6rFLZlGRgBEFz2rfwXOmU0rGA94DIejZ6zy8VtI6D2nY13fsyPPGTpx80OwcCQhDEoJ83bEd1zlzwK9F15RDupRAmpy6eMz7BRosGy3rqgtauypdqbQ7vxWJ62X3dALkAwWgUu1WQNPRQpax2cusZ12igIJxRqd2MIMJw93C2BAU4eXphcFimgGLWlS3mUAtDP8ZUHqGjkACC4AQ6Fy3CigMKpLWAn7wPytFDkTXmVb0EeO0Mc6SEOOSWXbXukBT37kPMJlDcFL0pQBDqsuUp5Tny6EU", 657) == -1){
        return;
    }    
    if (pm_addstring_gpu(cu_pm, (unsigned char *)"mjXnO5lUQKGB1RnpYMXq72alSUcZLthkcvCYuwmeTfThqjVbe0PzN6hId9a6u9DGxL37pbPcyBycDYWZmhwPF6CdQzyNxlAhLyjU0FG9jYICzxsGt", 113) == -1){
        return;
    }                
    if (pm_addstring_gpu(cu_pm, (unsigned char *)"kDR1XpjpFrgtED8y24YD76zgJz5hIAEfRJylpD7wO8oHE0xyjPUMTcqEabuZtvnUNgfZZjclcPlwOzJALTQQ1KzlGzfrGxKnbxRYiCw0IXgfdgAINS9gaNTVVT2AYIfxG5oiTqSFGNe8mx2inlzPHJSdZLx7Gh0Rmncf5MESgPxdkVKHxS0dGHEx9WIbKiimQMy5LHl2RwRTMWybSy8X1PYiXcAGw1x9HQ7QjavVkjBBLt8GnbeulhV0dN2wTO9gnoIjhulLzRltiHx57vafQM4fOalysOUqfhUppN494uD482cWkuNq08bAaNjaNq19gL9hI3AJNXzFl7KH1f6h3xd0qGuunOQv5N25U9X676qRm3hVlkgra8EVKRMW2vbCO9SE55py9k7Ehg6lrnPy0WayZfJly2ZlgwtFqVFR7dBXYklRZcru3RQdVDFOvObNJmJPx0A82LC1Q0bqpAzuSQQ2mOWKhg6bEF0q2tWSB3d0I9m8I97MoCKDWDfIHBrpHLW1GbDJJdZCePakRSn6ityD0reUreEv3Nk3oKGY3mhVRqvwq1K374D80HBudTsYNP6xa1yJtANEus3YnqI38AQ9eu46q1xfmYfY28V9mvOviAnj3G9nGRkdIzO6CkNIPWZ7T0TTlEmftZKe1MIXZYW2AnIXaSn5YCmzT6rYqIpT0lO4TGTX8Nu2Aj8GDbhAj0Zs0cnxS5b3FSylzp8MHuJsdyNxgMGTeNtGloWQwvfhHdCsj9rEbchclaBriYZtx7rzaxbIB", 777) == -1){
        return;
    }      
 
 
    if (pm_makeFSM_gpu(cu_pm) == -1){
        return;
    }
}

__global__ void search_patterns(pm_t *pm,  char *s, int lenOfS, int offset)
{
    pm_fsm_search(pm->zerostate, (unsigned char *)(&s[offset + blockIdx.x]), lenOfS-blockIdx.x, offset + blockIdx.x);
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