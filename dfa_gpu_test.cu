#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include <cuda_runtime.h>
#include "slist.c"
#include "pattern_matching_dfa.c"


__device__ void print_pm(pm_t *pm);
__device__ void print_state(pm_state_t *state, int tabs, int *is_need, int is_get);
__device__ void print_tabs(int tabs, int *is_need);
__global__ void search_patterns(pm_t **pm, unsigned char *s, int lenOfS);
__global__ void initilaizeTrees_dfa(pm_t ** cu_pms, pattern_details** patternArray, int numOfTrees);
__global__ void allocateStaticPacket(char *d, int n);
__global__ void f ();
void cleanup_destroy(slist_t *list);

char s1[PM_CHARACTERS+1];
int MAX_THEADS_FOR_EPOCH;
int NUM_OF_THREADS;

unsigned char * cu_s1;

int main(int argc, char *argv[]) // ********************* DFA - parallel ***********************
{
    FILE *fp;
    fp = fopen("/home/jceproject/Desktop/Final Project/Pattern-Matching/chars_stream.txt", "r");
    fgets(s1, PM_CHARACTERS+1, (FILE *)fp);
    int numOfChars = strlen(strtok(s1, "\0"));
    int numOfTrees = 112;


    pm_t** cu_pms;
    cudaMallocManaged(&cu_pms, numOfTrees*sizeof(pm_t*));//cu_pms
    
    pattern_details** patternArray;
    cudaMallocManaged(&patternArray, numOfTrees*sizeof(pattern_details*));//patternArray
    
    int i;
    for(i = 0; i< numOfTrees; i++){
        cudaMallocManaged(&(patternArray[i]), sizeof(pattern_details));//patternArray[i]
    }
    
    
    patternArray[0]->len = 479;
    cudaMallocManaged(&(patternArray[0]->string), patternArray[0]->len);
    cudaMemcpy(patternArray[0]->string,"DR5HY@yLcG*6GD7sXf1paMwfLiD&Gdub98QW0FzqpWjEHxMQZ0kJoZo#hJVJ9QGD3VdtIadTp4me*sfAzT3NjL0$4L2FC6NzmDd6mhtyW04o@*qO#le31L^MO$X#CpFYxHSegmPDjiCf8R^cmjtARILA*Z^e!n&pPEC&WKKNHO7I4P6XPA0&#75RJ7ghWsHXz^r3vSh@OzJK#jqaZBvz9x4EF*$hlfcrykrKdLgio7AaxLiuDD805i@4&s4WWusG7fsf5iNllmMrVkb7RtzSU@fZo1bLVaceLA#CkuWSpvaD0b#ZGwkcTesehS5UwKUObzILks#sdUJDBdadWpgP&lanjk^CbqjbumPvz2chvCa&Sc120z5p60TEDBc9esY3vbtq8w5j^M0K$$UYZB2NMAUjJ*kK^1SoX5f03yxAh0#!R$92Ui&88zhwxsnq2evaygzksj##w1Sx^jvknA&Vyba^Dt1j8CM", patternArray[0]->len, cudaMemcpyHostToDevice);
    patternArray[1]->len = 378;
    cudaMallocManaged(&(patternArray[1]->string), patternArray[1]->len);
    cudaMemcpy(patternArray[1]->string,"#^1wds5KDaplo&!mEFt7Xak&SFpa0fNP8xZwdmRXS7tZ2!B9P4na&yHXT*JAsuGUmeYypacsvkxnkUDgC*GWmUBf5WKT8E2k!UF@pK3g86ansmrmOfspzeSGjJmjfmg9#*pOYs@b^cGZLycrtv@*wrxCgeVvb&4v3mZwRwDgdEPZMQX0lyANM23HL3oWm#dm2nS6A!OAGq$QjoA9*DK&*ffQ2&wZxJf06Vxck1om!ccKe7qpqZk5iJECurt!3s6EpTPdpgxeKAZLlR4Bt1V#5ZBfX4tW0^3I0zlJvomrNOiOKi!vK^8lX36JkuJCzhrvAKk#aC^RtQAtS$qK^A*30I^$Ed3KZ82rgwG7z4mswABIzOH*3bm!6C@dCx", patternArray[1]->len, cudaMemcpyHostToDevice);
    patternArray[2]->len = 124;
    cudaMallocManaged(&(patternArray[2]->string), patternArray[2]->len);
    cudaMemcpy(patternArray[2]->string,"lKEX*VA$uFqJ2FfJE5*EI!MU5vMg!KGq9xP$fAshHrIVXgQfL45&1tT&TOUcTcKUYGoAgTEIrl6NiL8n&DPIvmomUpBp2sHM#*g1V2&xsYFOOznYQwzoqnBp2w*P", patternArray[2]->len, cudaMemcpyHostToDevice);
    patternArray[3]->len = 632;
    cudaMallocManaged(&(patternArray[3]->string), patternArray[3]->len);
    cudaMemcpy(patternArray[3]->string,"dA1sI0NGxYzGPWQreWrptknA9SMf44Bfl9hXDAV!TvWSGB!1e66$2wG6tx&Re*LoEBwJ!dUL@qOJuJ2kffeazN1Mhcvuc4$J7Xjox$1fAwSW6ziIvlv7PstENidAXl7DnlR4W7*yCjz$XhPvmWLflmvohg!XxTI*c#Ra5QRL2HKmc&PCzjQM*wzjSbbxLqC*G$Qe^nFIM8nEzfV4keXikucg$Z1GCW04eBYTWp*HCwrHHGqtC*XmRsPx@iGMQg5yGkeXQMiiQJUyLnp1f3PC4evR@^zijZ$H&XkZ7XuTUTcVV8vkIUpzoTRjYw$C1arvd&Y!&rFY35*RFDj!bv&&yl*sN4W3ALkDGXudUKRlPFYy!D89kXQmwkyHjO*&RAv#eJ2vBN7#*NMN*eY0bD$xQ0Rn7ywTUcch^fiU@3s$$2oGH2zjVLKXhHFVeRb7X9al9T^vwldPKFJDgqJgVky!QgWQ5g#*^lsEMykUV6o#h2UPhoy3@rqscqeBJ!GJwEJWj$Li^bynEWhtTsojwCiaB29gU&thp0O5$PI1Lq9UzpLTc8ZZ1$OgJSN11xJzpjC1&mNh7T@wD8j&M&Rjk^TXZckBHT9fVmS1!F0GHslpaBTZZ#DD6p@!CkDk", patternArray[3]->len, cudaMemcpyHostToDevice);
    patternArray[4]->len = 212;
    cudaMallocManaged(&(patternArray[4]->string), patternArray[4]->len);
    cudaMemcpy(patternArray[4]->string,"gGoojjNNZnj09JmxraEvWyaO9raPqdABd0iZSqoMVyPtyIfpLolELRVtyAjYyIE4x64spFYW6jXCE5CIRMdxaO0YfzlvkFYHwM1gSUbOqxuXkIYg9WFuQ6MG!VFoyoJ1xk0Y96mSP7eUhOmZeU9rzItkRzRVfvlKSvmxYwX7yoOIPVcxhL46ufZnn199V225aeKi8qXrCoUxiGELsNpV", patternArray[4]->len, cudaMemcpyHostToDevice);
    patternArray[5]->len = 729;
    cudaMallocManaged(&(patternArray[5]->string), patternArray[5]->len);
    cudaMemcpy(patternArray[5]->string,"E4yE1Zj7vkE2mNzp8Ikimo2lIEAFFyKZEsz7N7Sue629yw8RJ3zvHxWleH9KJqt0t9GTfL75eZqayVQOzHagHwLdacJHjaZ2ul4zHCb1af7CPpsSp6wstt8xBhqAaSydUCJcoFyetcxEh0zM1MiMDKEyDHmq4OhvSMFbI3SbgGoZ8SiMvCcWX2b0YsQVgF65LoEEKIL71xye0pBNxD4bNoQgvFTcNxK33mGB37oNekERK5qUOvO9xpKlWIaTDtkOTTnXL6WjK0WKbONOCw8x1kCKwk12aVUgM27L3kLTIdo0s6VUVAAGeqYFNRYzyn2F7qJSpEiygYaNdLkiJdLcf0QguBhhIOfTKh4henpss2Z2wBStst1lZvrZL6R5OjwMEFsUEsUjS8z0nOoFEAl1TOY4P9elaAPQ7ornOFAUybAVOZEmlu2atvLJVvv1VCsIEmwnezgyULFG886hbVRO5Z9uxWti2G8FmcI09JQkuXZ3vAyyEUVmLSJhc1tKMVpv8dRKq61ROSfs6f1qhepGxoJZQTkMRMaSii71JW6Fv0XzSSrsYEylWgkRNS1JUkB3Qe8YeryJ9ubX5I5VCd0JoStAY704klkXDUE2bKNMFALcFNjVDUdjpeu7q1TslRLFbpSrdfionpmmFyIzopE4CAYkz2jlablPHLUeBMjGgiBgbYOulSIKSuwx7rlvFCbhyTsibHtZxEY6TQEOkTWSPMXqp", patternArray[5]->len, cudaMemcpyHostToDevice);
    patternArray[6]->len = 311;
    cudaMallocManaged(&(patternArray[6]->string), patternArray[6]->len);
    cudaMemcpy(patternArray[6]->string,"YBx1TflInCLoTvSMvcgYv2Xd916G1nwiVqdpgmFdvFykLJPJRtSfVD2oiGccXNeSuTq1Pqt25hfFtIKGJDO8SVQdI5W24qLsUUJIgo31tAjbg4D6HFngMcApiewrd5CAWW41an8N5h6ZXtYM5pLytzkWddE4esqvEKHM9eRBdVpkbC0pXmVYeNCoSSjR7pwXxfSw4nk7Q0LSAdbbMgOIijc8z7wvfRLdbzuIgjVbR9USyiNm4fSYizzqc4fbQGdCx7nuNk1hxmFhS3Wpp70rP3Q7hrD4VJe3p2HlneNnpoLbJOWhJwVt7ze", patternArray[6]->len, cudaMemcpyHostToDevice);
    patternArray[7]->len = 657;
    cudaMallocManaged(&(patternArray[7]->string), patternArray[7]->len);
    cudaMemcpy(patternArray[7]->string,"lC14n8wwW12TxLorJKoO5PLRqSuafVm5MFC2OEzhvY5XD07nHYwGdzfDQ8XmUzUmyKNPjBp62AxJAJ97V9WPYAKHJDmKZ4AvSH1n2Ho651F9WgqbXzj3Abdr3m5wtDY8cb3kS3lRYcmNepUlOSuHw2Gy3EtPFLHVLyF2Fjls0mK6N2kxOR023jQTWwpqdHUN6MnTLn61Mx2c9sDrwSmvxaPcsGXhA6pnnPczdJhts6XvdIXmKaQzYpaC3niN82tSjwtR0VSsOeAE3WHZvjTxAduYagif23clk23QIdoenFqpxnqBLhFD9ShEfSuot28Ta0hcaIyxKuMi9KjD1f8HRM7mZ7zwQ3KaeSLoi7ovgWcUXMFkkCQsZ6rFLZlGRgBEFz2rfwXOmU0rGA94DIejZ6zy8VtI6D2nY13fsyPPGTpx80OwcCQhDEoJ83bEd1zlzwK9F15RDupRAmpy6eMz7BRosGy3rqgtauypdqbQ7vxWJ62X3dALkAwWgUu1WQNPRQpax2cusZ12igIJxRqd2MIMJw93C2BAU4eXphcFimgGLWlS3mUAtDP8ZUHqGjkACC4AQ6Fy3CigMKpLWAn7wPytFDkTXmVb0EeO0Mc6SEOOSWXbXukBT37kPMJlDcFL0pQBDqsuUp5Tny6EU", patternArray[7]->len, cudaMemcpyHostToDevice);
    patternArray[8]->len = 113;
    cudaMallocManaged(&(patternArray[8]->string), patternArray[8]->len);
    cudaMemcpy(patternArray[8]->string,"mjXnO5lUQKGB1RnpYMXq72alSUcZLthkcvCYuwmeTfThqjVbe0PzN6hId9a6u9DGxL37pbPcyBycDYWZmhwPF6CdQzyNxlAhLyjU0FG9jYICzxsGt", patternArray[8]->len, cudaMemcpyHostToDevice);
    patternArray[9]->len = 777;
    cudaMallocManaged(&(patternArray[9]->string), patternArray[9]->len);
    cudaMemcpy(patternArray[9]->string,"kDR1XpjpFrgtED8y24YD76zgJz5hIAEfRJylpD7wO8oHE0xyjPUMTcqEabuZtvnUNgfZZjclcPlwOzJALTQQ1KzlGzfrGxKnbxRYiCw0IXgfdgAINS9gaNTVVT2AYIfxG5oiTqSFGNe8mx2inlzPHJSdZLx7Gh0Rmncf5MESgPxdkVKHxS0dGHEx9WIbKiimQMy5LHl2RwRTMWybSy8X1PYiXcAGw1x9HQ7QjavVkjBBLt8GnbeulhV0dN2wTO9gnoIjhulLzRltiHx57vafQM4fOalysOUqfhUppN494uD482cWkuNq08bAaNjaNq19gL9hI3AJNXzFl7KH1f6h3xd0qGuunOQv5N25U9X676qRm3hVlkgra8EVKRMW2vbCO9SE55py9k7Ehg6lrnPy0WayZfJly2ZlgwtFqVFR7dBXYklRZcru3RQdVDFOvObNJmJPx0A82LC1Q0bqpAzuSQQ2mOWKhg6bEF0q2tWSB3d0I9m8I97MoCKDWDfIHBrpHLW1GbDJJdZCePakRSn6ityD0reUreEv3Nk3oKGY3mhVRqvwq1K374D80HBudTsYNP6xa1yJtANEus3YnqI38AQ9eu46q1xfmYfY28V9mvOviAnj3G9nGRkdIzO6CkNIPWZ7T0TTlEmftZKe1MIXZYW2AnIXaSn5YCmzT6rYqIpT0lO4TGTX8Nu2Aj8GDbhAj0Zs0cnxS5b3FSylzp8MHuJsdyNxgMGTeNtGloWQwvfhHdCsj9rEbchclaBriYZtx7rzaxbIB", patternArray[9]->len, cudaMemcpyHostToDevice);


    patternArray[10]->len = 17;
    cudaMallocManaged(&(patternArray[10]->string), patternArray[10]->len);
    cudaMemcpy(patternArray[10]->string,"check1=1234567890", patternArray[10]->len, cudaMemcpyHostToDevice);

    patternArray[11]->len = 17;
    cudaMallocManaged(&(patternArray[11]->string), patternArray[11]->len);
    cudaMemcpy(patternArray[11]->string,"check2=1234567890", patternArray[11]->len, cudaMemcpyHostToDevice);

    patternArray[12]->len = 17;
    cudaMallocManaged(&(patternArray[12]->string), patternArray[12]->len);
    cudaMemcpy(patternArray[12]->string,"check3=1234567890", patternArray[12]->len, cudaMemcpyHostToDevice);

    patternArray[13]->len = 17;
    cudaMallocManaged(&(patternArray[13]->string), patternArray[13]->len);
    cudaMemcpy(patternArray[13]->string,"check4=1234567890", patternArray[13]->len, cudaMemcpyHostToDevice);

    patternArray[14]->len = 17;
    cudaMallocManaged(&(patternArray[14]->string), patternArray[14]->len);
    cudaMemcpy(patternArray[14]->string,"check5=1234567890", patternArray[14]->len, cudaMemcpyHostToDevice);

    patternArray[15]->len = 17;
    cudaMallocManaged(&(patternArray[15]->string), patternArray[15]->len);
    cudaMemcpy(patternArray[15]->string,"check6=1234567890", patternArray[15]->len, cudaMemcpyHostToDevice);

    patternArray[16]->len = 17;
    cudaMallocManaged(&(patternArray[16]->string), patternArray[16]->len);
    cudaMemcpy(patternArray[16]->string,"check7=1234567890", patternArray[16]->len, cudaMemcpyHostToDevice);

    patternArray[17]->len = 17;
    cudaMallocManaged(&(patternArray[17]->string), patternArray[17]->len);
    cudaMemcpy(patternArray[17]->string,"check8=1234567890", patternArray[17]->len, cudaMemcpyHostToDevice);

    patternArray[18]->len = 17;
    cudaMallocManaged(&(patternArray[18]->string), patternArray[18]->len);
    cudaMemcpy(patternArray[18]->string,"check9=1234567890", patternArray[18]->len, cudaMemcpyHostToDevice);

    patternArray[19]->len = 18;
    cudaMallocManaged(&(patternArray[19]->string), patternArray[19]->len);
    cudaMemcpy(patternArray[19]->string,"check10=1234567890", patternArray[19]->len, cudaMemcpyHostToDevice);

    patternArray[20]->len = 18;
    cudaMallocManaged(&(patternArray[20]->string), patternArray[20]->len);
    cudaMemcpy(patternArray[20]->string,"check11=1234567890", patternArray[20]->len, cudaMemcpyHostToDevice);

    patternArray[21]->len = 18;
    cudaMallocManaged(&(patternArray[21]->string), patternArray[21]->len);
    cudaMemcpy(patternArray[21]->string,"check12=1234567890", patternArray[21]->len, cudaMemcpyHostToDevice);

    patternArray[22]->len = 18;
    cudaMallocManaged(&(patternArray[22]->string), patternArray[22]->len);
    cudaMemcpy(patternArray[22]->string,"check13=1234567890", patternArray[22]->len, cudaMemcpyHostToDevice);

    patternArray[23]->len = 18;
    cudaMallocManaged(&(patternArray[23]->string), patternArray[23]->len);
    cudaMemcpy(patternArray[23]->string,"check14=1234567890", patternArray[23]->len, cudaMemcpyHostToDevice);

    patternArray[24]->len = 18;
    cudaMallocManaged(&(patternArray[24]->string), patternArray[24]->len);
    cudaMemcpy(patternArray[24]->string,"check15=1234567890", patternArray[24]->len, cudaMemcpyHostToDevice);

    patternArray[25]->len = 18;
    cudaMallocManaged(&(patternArray[25]->string), patternArray[25]->len);
    cudaMemcpy(patternArray[25]->string,"check16=1234567890", patternArray[25]->len, cudaMemcpyHostToDevice);

    patternArray[26]->len = 18;
    cudaMallocManaged(&(patternArray[26]->string), patternArray[26]->len);
    cudaMemcpy(patternArray[26]->string,"check17=1234567890", patternArray[26]->len, cudaMemcpyHostToDevice);

    patternArray[27]->len = 18;
    cudaMallocManaged(&(patternArray[27]->string), patternArray[27]->len);
    cudaMemcpy(patternArray[27]->string,"check18=1234567890", patternArray[27]->len, cudaMemcpyHostToDevice);

    patternArray[28]->len = 18;
    cudaMallocManaged(&(patternArray[28]->string), patternArray[28]->len);
    cudaMemcpy(patternArray[28]->string,"check19=1234567890", patternArray[28]->len, cudaMemcpyHostToDevice);

    patternArray[29]->len = 18;
    cudaMallocManaged(&(patternArray[29]->string), patternArray[29]->len);
    cudaMemcpy(patternArray[29]->string,"check20=1234567890", patternArray[29]->len, cudaMemcpyHostToDevice);
    
    patternArray[30]->len = 18;
    cudaMallocManaged(&(patternArray[30]->string), patternArray[30]->len);
    cudaMemcpy(patternArray[30]->string,"check21=1234567890", patternArray[30]->len, cudaMemcpyHostToDevice);

    patternArray[31]->len = 18;
    cudaMallocManaged(&(patternArray[31]->string), patternArray[31]->len);
    cudaMemcpy(patternArray[31]->string,"check22=1234567890", patternArray[31]->len, cudaMemcpyHostToDevice);

    patternArray[32]->len = 18;
    cudaMallocManaged(&(patternArray[32]->string), patternArray[32]->len);
    cudaMemcpy(patternArray[32]->string,"check23=1234567890", patternArray[32]->len, cudaMemcpyHostToDevice);

    patternArray[33]->len = 18;
    cudaMallocManaged(&(patternArray[33]->string), patternArray[33]->len);
    cudaMemcpy(patternArray[33]->string,"check24=1234567890", patternArray[33]->len, cudaMemcpyHostToDevice);

    patternArray[34]->len = 19;
    cudaMallocManaged(&(patternArray[34]->string), patternArray[34]->len);
    cudaMemcpy(patternArray[34]->string,"check103=1234567890", patternArray[34]->len, cudaMemcpyHostToDevice);

    patternArray[35]->len = 18;
    cudaMallocManaged(&(patternArray[35]->string), patternArray[35]->len);
    cudaMemcpy(patternArray[35]->string,"check25=1234567890", patternArray[35]->len, cudaMemcpyHostToDevice);

    patternArray[36]->len = 18;
    cudaMallocManaged(&(patternArray[36]->string), patternArray[36]->len);
    cudaMemcpy(patternArray[36]->string,"check26=1234567890", patternArray[36]->len, cudaMemcpyHostToDevice);

    patternArray[37]->len = 18;
    cudaMallocManaged(&(patternArray[37]->string), patternArray[37]->len);
    cudaMemcpy(patternArray[37]->string,"check27=1234567890", patternArray[37]->len, cudaMemcpyHostToDevice);

    patternArray[38]->len = 18;
    cudaMallocManaged(&(patternArray[38]->string), patternArray[38]->len);
    cudaMemcpy(patternArray[38]->string,"check28=1234567890", patternArray[38]->len, cudaMemcpyHostToDevice);

    patternArray[39]->len = 18;
    cudaMallocManaged(&(patternArray[39]->string), patternArray[39]->len);
    cudaMemcpy(patternArray[39]->string,"check29=1234567890", patternArray[39]->len, cudaMemcpyHostToDevice);

    patternArray[40]->len = 18;
    cudaMallocManaged(&(patternArray[40]->string), patternArray[40]->len);
    cudaMemcpy(patternArray[40]->string,"check30=1234567890", patternArray[40]->len, cudaMemcpyHostToDevice);

    patternArray[41]->len = 19;
    cudaMallocManaged(&(patternArray[41]->string), patternArray[41]->len);
    cudaMemcpy(patternArray[41]->string,"check101=1234567890", patternArray[41]->len, cudaMemcpyHostToDevice);

    patternArray[42]->len = 18;
    cudaMallocManaged(&(patternArray[42]->string), patternArray[42]->len);
    cudaMemcpy(patternArray[42]->string,"check31=1234567890", patternArray[42]->len, cudaMemcpyHostToDevice);

    patternArray[43]->len = 18;
    cudaMallocManaged(&(patternArray[43]->string), patternArray[43]->len);
    cudaMemcpy(patternArray[43]->string,"check32=1234567890", patternArray[43]->len, cudaMemcpyHostToDevice);

    patternArray[44]->len = 18;
    cudaMallocManaged(&(patternArray[44]->string), patternArray[44]->len);
    cudaMemcpy(patternArray[44]->string,"check33=1234567890", patternArray[44]->len, cudaMemcpyHostToDevice);

    patternArray[45]->len = 18;
    cudaMallocManaged(&(patternArray[45]->string), patternArray[45]->len);
    cudaMemcpy(patternArray[45]->string,"check34=1234567890", patternArray[45]->len, cudaMemcpyHostToDevice);

    patternArray[46]->len = 19;
    cudaMallocManaged(&(patternArray[46]->string), patternArray[46]->len);
    cudaMemcpy(patternArray[46]->string,"check102=1234567890", patternArray[46]->len, cudaMemcpyHostToDevice);

    patternArray[47]->len = 18;
    cudaMallocManaged(&(patternArray[47]->string), patternArray[47]->len);
    cudaMemcpy(patternArray[47]->string,"check35=1234567890", patternArray[47]->len, cudaMemcpyHostToDevice);

    patternArray[48]->len = 18;
    cudaMallocManaged(&(patternArray[48]->string), patternArray[48]->len);
    cudaMemcpy(patternArray[48]->string,"check36=1234567890", patternArray[48]->len, cudaMemcpyHostToDevice);

    patternArray[49]->len = 18;
    cudaMallocManaged(&(patternArray[49]->string), patternArray[49]->len);
    cudaMemcpy(patternArray[49]->string,"check37=1234567890", patternArray[49]->len, cudaMemcpyHostToDevice);

    patternArray[50]->len = 18;
    cudaMallocManaged(&(patternArray[50]->string), patternArray[50]->len);
    cudaMemcpy(patternArray[50]->string,"check38=1234567890", patternArray[50]->len, cudaMemcpyHostToDevice);

    patternArray[51]->len = 18;
    cudaMallocManaged(&(patternArray[51]->string), patternArray[51]->len);
    cudaMemcpy(patternArray[51]->string,"check39=1234567890", patternArray[51]->len, cudaMemcpyHostToDevice);

    patternArray[52]->len = 18;
    cudaMallocManaged(&(patternArray[52]->string), patternArray[52]->len);
    cudaMemcpy(patternArray[52]->string,"check40=1234567890", patternArray[52]->len, cudaMemcpyHostToDevice);

    patternArray[53]->len = 18;
    cudaMallocManaged(&(patternArray[53]->string), patternArray[53]->len);
    cudaMemcpy(patternArray[53]->string,"check41=1234567890", patternArray[53]->len, cudaMemcpyHostToDevice);

    patternArray[54]->len = 18;
    cudaMallocManaged(&(patternArray[54]->string), patternArray[54]->len);
    cudaMemcpy(patternArray[54]->string,"check42=1234567890", patternArray[54]->len, cudaMemcpyHostToDevice);

    patternArray[55]->len = 18;
    cudaMallocManaged(&(patternArray[55]->string), patternArray[55]->len);
    cudaMemcpy(patternArray[55]->string,"check43=1234567890", patternArray[55]->len, cudaMemcpyHostToDevice);

    patternArray[56]->len = 18;
    cudaMallocManaged(&(patternArray[56]->string), patternArray[56]->len);
    cudaMemcpy(patternArray[56]->string,"check44=1234567890", patternArray[56]->len, cudaMemcpyHostToDevice);

    patternArray[57]->len = 18;
    cudaMallocManaged(&(patternArray[57]->string), patternArray[57]->len);
    cudaMemcpy(patternArray[57]->string,"check45=1234567890", patternArray[57]->len, cudaMemcpyHostToDevice);

    patternArray[58]->len = 18;
    cudaMallocManaged(&(patternArray[58]->string), patternArray[58]->len);
    cudaMemcpy(patternArray[58]->string,"check46=1234567890", patternArray[58]->len, cudaMemcpyHostToDevice);

    patternArray[59]->len = 18;
    cudaMallocManaged(&(patternArray[59]->string), patternArray[59]->len);
    cudaMemcpy(patternArray[59]->string,"check47=1234567890", patternArray[59]->len, cudaMemcpyHostToDevice);

    patternArray[60]->len = 18;
    cudaMallocManaged(&(patternArray[60]->string), patternArray[60]->len);
    cudaMemcpy(patternArray[60]->string,"check48=1234567890", patternArray[60]->len, cudaMemcpyHostToDevice);

    patternArray[61]->len = 18;
    cudaMallocManaged(&(patternArray[61]->string), patternArray[61]->len);
    cudaMemcpy(patternArray[61]->string,"check49=1234567890", patternArray[61]->len, cudaMemcpyHostToDevice);

    patternArray[62]->len = 18;
    cudaMallocManaged(&(patternArray[62]->string), patternArray[62]->len);
    cudaMemcpy(patternArray[62]->string,"check50=1234567890", patternArray[62]->len, cudaMemcpyHostToDevice);

    patternArray[63]->len = 18;
    cudaMallocManaged(&(patternArray[63]->string), patternArray[63]->len);
    cudaMemcpy(patternArray[63]->string,"check51=1234567890", patternArray[63]->len, cudaMemcpyHostToDevice);

    patternArray[64]->len = 18;
    cudaMallocManaged(&(patternArray[64]->string), patternArray[64]->len);
    cudaMemcpy(patternArray[64]->string,"check52=1234567890", patternArray[64]->len, cudaMemcpyHostToDevice);

    patternArray[65]->len = 18;
    cudaMallocManaged(&(patternArray[65]->string), patternArray[65]->len);
    cudaMemcpy(patternArray[65]->string,"check53=1234567890", patternArray[65]->len, cudaMemcpyHostToDevice);

    patternArray[66]->len = 18;
    cudaMallocManaged(&(patternArray[66]->string), patternArray[66]->len);
    cudaMemcpy(patternArray[66]->string,"check54=1234567890", patternArray[66]->len, cudaMemcpyHostToDevice);

    patternArray[67]->len = 18;
    cudaMallocManaged(&(patternArray[67]->string), patternArray[67]->len);
    cudaMemcpy(patternArray[67]->string,"check55=1234567890", patternArray[67]->len, cudaMemcpyHostToDevice);

    patternArray[68]->len = 18;
    cudaMallocManaged(&(patternArray[68]->string), patternArray[68]->len);
    cudaMemcpy(patternArray[68]->string,"check56=1234567890", patternArray[68]->len, cudaMemcpyHostToDevice);

    patternArray[69]->len = 18;
    cudaMallocManaged(&(patternArray[69]->string), patternArray[69]->len);
    cudaMemcpy(patternArray[69]->string,"check57=1234567890", patternArray[69]->len, cudaMemcpyHostToDevice);

    patternArray[70]->len = 18;
    cudaMallocManaged(&(patternArray[70]->string), patternArray[70]->len);
    cudaMemcpy(patternArray[70]->string,"check58=1234567890", patternArray[70]->len, cudaMemcpyHostToDevice);

    patternArray[71]->len = 18;
    cudaMallocManaged(&(patternArray[71]->string), patternArray[71]->len);
    cudaMemcpy(patternArray[71]->string,"check59=1234567890", patternArray[71]->len, cudaMemcpyHostToDevice);

    patternArray[72]->len = 18;
    cudaMallocManaged(&(patternArray[72]->string), patternArray[72]->len);
    cudaMemcpy(patternArray[72]->string,"check60=1234567890", patternArray[72]->len, cudaMemcpyHostToDevice);

    patternArray[73]->len = 18;
    cudaMallocManaged(&(patternArray[73]->string), patternArray[73]->len);
    cudaMemcpy(patternArray[73]->string,"check61=1234567890", patternArray[73]->len, cudaMemcpyHostToDevice);

    patternArray[74]->len = 18;
    cudaMallocManaged(&(patternArray[74]->string), patternArray[74]->len);
    cudaMemcpy(patternArray[74]->string,"check62=1234567890", patternArray[74]->len, cudaMemcpyHostToDevice);

    patternArray[75]->len = 18;
    cudaMallocManaged(&(patternArray[75]->string), patternArray[75]->len);
    cudaMemcpy(patternArray[75]->string,"check63=1234567890", patternArray[75]->len, cudaMemcpyHostToDevice);

    patternArray[76]->len = 18;
    cudaMallocManaged(&(patternArray[76]->string), patternArray[76]->len);
    cudaMemcpy(patternArray[76]->string,"check64=1234567890", patternArray[76]->len, cudaMemcpyHostToDevice);

    patternArray[77]->len = 18;
    cudaMallocManaged(&(patternArray[77]->string), patternArray[77]->len);
    cudaMemcpy(patternArray[77]->string,"check65=1234567890", patternArray[77]->len, cudaMemcpyHostToDevice);

    patternArray[78]->len = 18;
    cudaMallocManaged(&(patternArray[78]->string), patternArray[78]->len);
    cudaMemcpy(patternArray[78]->string,"check66=1234567890", patternArray[78]->len, cudaMemcpyHostToDevice);

    patternArray[79]->len = 18;
    cudaMallocManaged(&(patternArray[79]->string), patternArray[79]->len);
    cudaMemcpy(patternArray[79]->string,"check67=1234567890", patternArray[79]->len, cudaMemcpyHostToDevice);

    patternArray[80]->len = 18;
    cudaMallocManaged(&(patternArray[80]->string), patternArray[80]->len);
    cudaMemcpy(patternArray[80]->string,"check68=1234567890", patternArray[80]->len, cudaMemcpyHostToDevice);

    patternArray[81]->len = 18;
    cudaMallocManaged(&(patternArray[81]->string), patternArray[81]->len);
    cudaMemcpy(patternArray[81]->string,"check69=1234567890", patternArray[81]->len, cudaMemcpyHostToDevice);

    patternArray[82]->len = 18;
    cudaMallocManaged(&(patternArray[82]->string), patternArray[82]->len);
    cudaMemcpy(patternArray[82]->string,"check70=1234567890", patternArray[82]->len, cudaMemcpyHostToDevice);

    patternArray[83]->len = 18;
    cudaMallocManaged(&(patternArray[83]->string), patternArray[83]->len);
    cudaMemcpy(patternArray[83]->string,"check71=1234567890", patternArray[83]->len, cudaMemcpyHostToDevice);

    patternArray[84]->len = 18;
    cudaMallocManaged(&(patternArray[84]->string), patternArray[84]->len);
    cudaMemcpy(patternArray[84]->string,"check72=1234567890", patternArray[84]->len, cudaMemcpyHostToDevice);

    patternArray[85]->len = 18;
    cudaMallocManaged(&(patternArray[85]->string), patternArray[85]->len);
    cudaMemcpy(patternArray[85]->string,"check73=1234567890", patternArray[85]->len, cudaMemcpyHostToDevice);

    patternArray[86]->len = 18;
    cudaMallocManaged(&(patternArray[86]->string), patternArray[86]->len);
    cudaMemcpy(patternArray[86]->string,"check74=1234567890", patternArray[86]->len, cudaMemcpyHostToDevice);

    patternArray[87]->len = 18;
    cudaMallocManaged(&(patternArray[87]->string), patternArray[87]->len);
    cudaMemcpy(patternArray[87]->string,"check75=1234567890", patternArray[87]->len, cudaMemcpyHostToDevice);

    patternArray[88]->len = 18;
    cudaMallocManaged(&(patternArray[88]->string), patternArray[88]->len);
    cudaMemcpy(patternArray[88]->string,"check76=1234567890", patternArray[88]->len, cudaMemcpyHostToDevice);

    patternArray[89]->len = 18;
    cudaMallocManaged(&(patternArray[89]->string), patternArray[89]->len);
    cudaMemcpy(patternArray[89]->string,"check77=1234567890", patternArray[89]->len, cudaMemcpyHostToDevice);

    patternArray[90]->len = 18;
    cudaMallocManaged(&(patternArray[90]->string), patternArray[90]->len);
    cudaMemcpy(patternArray[90]->string,"check78=1234567890", patternArray[90]->len, cudaMemcpyHostToDevice);

    patternArray[91]->len = 18;
    cudaMallocManaged(&(patternArray[91]->string), patternArray[91]->len);
    cudaMemcpy(patternArray[91]->string,"check79=1234567890", patternArray[91]->len, cudaMemcpyHostToDevice);

    patternArray[92]->len = 18;
    cudaMallocManaged(&(patternArray[92]->string), patternArray[92]->len);
    cudaMemcpy(patternArray[92]->string,"check80=1234567890", patternArray[92]->len, cudaMemcpyHostToDevice);

    patternArray[93]->len = 18;
    cudaMallocManaged(&(patternArray[93]->string), patternArray[93]->len);
    cudaMemcpy(patternArray[93]->string,"check81=1234567890", patternArray[93]->len, cudaMemcpyHostToDevice);

    patternArray[94]->len = 18;
    cudaMallocManaged(&(patternArray[94]->string), patternArray[94]->len);
    cudaMemcpy(patternArray[94]->string,"check82=1234567890", patternArray[94]->len, cudaMemcpyHostToDevice);

    patternArray[95]->len = 18;
    cudaMallocManaged(&(patternArray[95]->string), patternArray[95]->len);
    cudaMemcpy(patternArray[95]->string,"check83=1234567890", patternArray[95]->len, cudaMemcpyHostToDevice);

    patternArray[96]->len = 18;
    cudaMallocManaged(&(patternArray[96]->string), patternArray[96]->len);
    cudaMemcpy(patternArray[96]->string,"check84=1234567890", patternArray[96]->len, cudaMemcpyHostToDevice);

    patternArray[97]->len = 18;
    cudaMallocManaged(&(patternArray[97]->string), patternArray[97]->len);
    cudaMemcpy(patternArray[97]->string,"check85=1234567890", patternArray[97]->len, cudaMemcpyHostToDevice);

    patternArray[98]->len = 18;
    cudaMallocManaged(&(patternArray[98]->string), patternArray[98]->len);
    cudaMemcpy(patternArray[98]->string,"check86=1234567890", patternArray[98]->len, cudaMemcpyHostToDevice);

    patternArray[99]->len = 18;
    cudaMallocManaged(&(patternArray[99]->string), patternArray[99]->len);
    cudaMemcpy(patternArray[99]->string,"check87=1234567890", patternArray[99]->len, cudaMemcpyHostToDevice);

    patternArray[100]->len = 18;
    cudaMallocManaged(&(patternArray[100]->string), patternArray[100]->len);
    cudaMemcpy(patternArray[100]->string,"check88=1234567890", patternArray[100]->len, cudaMemcpyHostToDevice);

    patternArray[101]->len = 18;
    cudaMallocManaged(&(patternArray[101]->string), patternArray[101]->len);
    cudaMemcpy(patternArray[101]->string,"check89=1234567890", patternArray[101]->len, cudaMemcpyHostToDevice);

    patternArray[102]->len = 18;
    cudaMallocManaged(&(patternArray[102]->string), patternArray[102]->len);
    cudaMemcpy(patternArray[102]->string,"check90=1234567890", patternArray[102]->len, cudaMemcpyHostToDevice);

    patternArray[103]->len = 18;
    cudaMallocManaged(&(patternArray[103]->string), patternArray[103]->len);
    cudaMemcpy(patternArray[103]->string,"check91=1234567890", patternArray[103]->len, cudaMemcpyHostToDevice);

    patternArray[104]->len = 18;
    cudaMallocManaged(&(patternArray[104]->string), patternArray[104]->len);
    cudaMemcpy(patternArray[104]->string,"check92=1234567890", patternArray[104]->len, cudaMemcpyHostToDevice);

    patternArray[105]->len = 18;
    cudaMallocManaged(&(patternArray[105]->string), patternArray[105]->len);
    cudaMemcpy(patternArray[105]->string,"check93=1234567890", patternArray[105]->len, cudaMemcpyHostToDevice);

    patternArray[106]->len = 18;
    cudaMallocManaged(&(patternArray[106]->string), patternArray[106]->len);
    cudaMemcpy(patternArray[106]->string,"check94=1234567890", patternArray[106]->len, cudaMemcpyHostToDevice);

    patternArray[107]->len = 18;
    cudaMallocManaged(&(patternArray[107]->string), patternArray[107]->len);
    cudaMemcpy(patternArray[107]->string,"check95=1234567890", patternArray[107]->len, cudaMemcpyHostToDevice);

    patternArray[108]->len = 18;
    cudaMallocManaged(&(patternArray[108]->string), patternArray[108]->len);
    cudaMemcpy(patternArray[108]->string,"check96=1234567890", patternArray[108]->len, cudaMemcpyHostToDevice);

    patternArray[109]->len = 18;
    cudaMallocManaged(&(patternArray[109]->string), patternArray[109]->len);
    cudaMemcpy(patternArray[109]->string,"check97=1234567890", patternArray[109]->len, cudaMemcpyHostToDevice);

    patternArray[110]->len = 18;
    cudaMallocManaged(&(patternArray[110]->string), patternArray[110]->len);
    cudaMemcpy(patternArray[110]->string,"check98=1234567890", patternArray[110]->len, cudaMemcpyHostToDevice);

    patternArray[111]->len = 7;
    cudaMallocManaged(&(patternArray[111]->string), patternArray[111]->len);
    cudaMemcpy(patternArray[111]->string,"KX65fbx", patternArray[111]->len, cudaMemcpyHostToDevice);

    // patternArray[112]->len = 19;
    // cudaMallocManaged(&(patternArray[112]->string), patternArray[112]->len);
    // cudaMemcpy(patternArray[112]->string,"check100=1234567890", patternArray[112]->len, cudaMemcpyHostToDevice);

    

    if (!cu_pms)
    {
        return -1;
    }
    
    // if (pm_init(pm) == -1)
    // {
    //     free(pm);
    //     return -1;
    // }

    // for(int i=0; i<63; i++){
    //     allocateStaticPacket<<63,1024>>(s1,i);
    // }
    initilaizeTrees_dfa<<<1,1>>>(cu_pms, patternArray, numOfTrees);

    cudaMalloc(&cu_s1, (numOfChars+1)*sizeof(unsigned char));//cu_s1
    cudaMemcpy(cu_s1, s1,numOfChars,cudaMemcpyHostToDevice);
    
    int len = strlen(s1);
    
    
    
    clock_t begin;
    clock_t end;
    
    begin = clock();
    search_patterns<<<numOfTrees,1>>>(cu_pms, cu_s1, len);
    cudaDeviceSynchronize();
    end = clock();
    
    
    printf("\nExecuted time is: %f ms. \n\n", ((double)(end - begin) / CLOCKS_PER_SEC) * 1000);
    
    // cudaFree(cu_pm);
    // cudaFree(cu_s1);
    
    // f<<<1,1>>>();

    // cleanup_destroy(list);

    // cudaFree(cu_s1);
    // for(int j=0; j<numOfTrees;j++){
    //     cudaFree(patternArray[j]->string);
    // }
    // for(int j=0; j<numOfTrees;j++){
    //     cudaFree(patternArray[j]);
    // }
    // cudaFree(patternArray);
    // for(int j=0; j<numOfTrees;j++){
    //     cudaFree(cu_pms[j]);
    // }
    // cudaFree(cu_pms);
    // fclose(fp);
    return 0;
}
__global__ void f (){
    printf("in f");
}

__global__ void initilaizeTrees_dfa(pm_t ** cu_pms, pattern_details** patternArray, int numOfTrees)
{
    int i;
    for (i=0; i<numOfTrees; i++){
        cu_pms[i] = (pm_t*)malloc(sizeof(pm_t));
        if(pm_init_gpu(cu_pms[i]) == -1){
            return;
        }
        if (pm_addstring_gpu(cu_pms[i], patternArray[i]->string, patternArray[i]->len) == -1){
            return;
        }
        if (pm_makeFSM_gpu(cu_pms[i]) == -1){
            return;
        }
    }
}


__global__ void search_patterns(pm_t ** cu_pms, unsigned char *s, int lenOfS)
{   
    // __shared__ unsigned char cu_s1[40000+1];
    // int i;
    // if(threadIdx.x == 0){
    //     // cu_s1 = (unsigned char *)malloc(sizeof(unsigned char)*(lenOfS+1));
    //     for(i=0; i<40000; i++){
    //         cu_s1[i] = s[i]; 
    //     }
    // cu_s1[40000]='\0';
    // // printf("cu_s1: %s\n", cu_s1);
    // }
    // __syncthreads();
    pm_fsm_search_gpu((cu_pms[blockIdx.x])->zerostate, s, lenOfS);
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



// __global__ void allocateStaticPacket(char *d,int i)
// {
//   __shared__ char s[64000];
//     int t = blockIdx.x*i + threadIdx.x;
//     s[t] = d[t];
//   __syncthreads();
// }