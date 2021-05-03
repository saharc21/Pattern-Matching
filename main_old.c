#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <cuda_runtime.h>
#include "slist.c"
#include "pattern_matching_aho.c"


void print_pm(pm_t *pm);
void print_state(pm_state_t *state, int tabs, int *is_need, int is_get);
void print_tabs(int tabs, int *is_need);
unsigned char *shift(unsigned char *str, int len);
void search_and_destroy(pm_t *pm, char *s);
void _test1(pm_t *pm1);
void _test2(pm_t *pm1, pm_t *pm2, pm_t *pm3, pm_t *pm4, pm_t *pm5, pm_t *pm6, pm_t *pm7, pm_t *pm8, pm_t *pm9, pm_t *pm10);
void *myFunc(void *pm);
int pms_init(int NUM_OF_THREADS, pm_t **pms);

char s1[PM_CHARACTERS];

int main(int argc, char *argv[]) // ********************* Aho - parallel ***********************
{
    FILE *fp;
    fp = fopen("/home/jceproject/Desktop/Final Project/Pattern-Matching/chars_stream.txt", "r");
    fgets(s1, PM_CHARACTERS, (FILE *)fp);
    int numOfChars = strlen(strtok(s1, "\0"));

    int NUM_OF_THREADS = numOfChars - 113;

    pthread_t *tids = (pthread_t *)malloc(sizeof(pthread_t) * NUM_OF_THREADS);

    pm_t *pm = (pm_t *)malloc(sizeof(pm_t));

    if (!pm)
    {
        return -1;
    }

    if (pm_init(pm) == -1)
    {
        free(pm);
        return -1;
    }

    pm_addstring(pm, (unsigned char *)"DR5HY@yLcG*6GD7sXf1paMwfLiD&Gdub98QW0FzqpWjEHxMQZ0kJoZo#hJVJ9QGD3VdtIadTp4me*sfAzT3NjL0$4L2FC6NzmDd6mhtyW04o@*qO#le31L^MO$X#CpFYxHSegmPDjiCf8R^cmjtARILA*Z^e!n&pPEC&WKKNHO7I4P6XPA0&#75RJ7ghWsHXz^r3vSh@OzJK#jqaZBvz9x4EF*$hlfcrykrKdLgio7AaxLiuDD805i@4&s4WWusG7fsf5iNllmMrVkb7RtzSU@fZo1bLVaceLA#CkuWSpvaD0b#ZGwkcTesehS5UwKUObzILks#sdUJDBdadWpgP&lanjk^CbqjbumPvz2chvCa&Sc120z5p60TEDBc9esY3vbtq8w5j^M0K$$UYZB2NMAUjJ*kK^1SoX5f03yxAh0#!R$92Ui&88zhwxsnq2evaygzksj##w1Sx^jvknA&Vyba^Dt1j8CM", 479);
    pm_addstring(pm, (unsigned char *)"#^1wds5KDaplo&!mEFt7Xak&SFpa0fNP8xZwdmRXS7tZ2!B9P4na&yHXT*JAsuGUmeYypacsvkxnkUDgC*GWmUBf5WKT8E2k!UF@pK3g86ansmrmOfspzeSGjJmjfmg9#*pOYs@b^cGZLycrtv@*wrxCgeVvb&4v3mZwRwDgdEPZMQX0lyANM23HL3oWm#dm2nS6A!OAGq$QjoA9*DK&*ffQ2&wZxJf06Vxck1om!ccKe7qpqZk5iJECurt!3s6EpTPdpgxeKAZLlR4Bt1V#5ZBfX4tW0^3I0zlJvomrNOiOKi!vK^8lX36JkuJCzhrvAKk#aC^RtQAtS$qK^A*30I^$Ed3KZ82rgwG7z4mswABIzOH*3bm!6C@dCx", 378);
    pm_addstring(pm, (unsigned char *)"lKEX*VA$uFqJ2FfJE5*EI!MU5vMg!KGq9xP$fAshHrIVXgQfL45&1tT&TOUcTcKUYGoAgTEIrl6NiL8n&DPIvmomUpBp2sHM#*g1V2&xsYFOOznYQwzoqnBp2w*P", 124);
    pm_addstring(pm, (unsigned char *)"dA1sI0NGxYzGPWQreWrptknA9SMf44Bfl9hXDAV!TvWSGB!1e66$2wG6tx&Re*LoEBwJ!dUL@qOJuJ2kffeazN1Mhcvuc4$J7Xjox$1fAwSW6ziIvlv7PstENidAXl7DnlR4W7*yCjz$XhPvmWLflmvohg!XxTI*c#Ra5QRL2HKmc&PCzjQM*wzjSbbxLqC*G$Qe^nFIM8nEzfV4keXikucg$Z1GCW04eBYTWp*HCwrHHGqtC*XmRsPx@iGMQg5yGkeXQMiiQJUyLnp1f3PC4evR@^zijZ$H&XkZ7XuTUTcVV8vkIUpzoTRjYw$C1arvd&Y!&rFY35*RFDj!bv&&yl*sN4W3ALkDGXudUKRlPFYy!D89kXQmwkyHjO*&RAv#eJ2vBN7#*NMN*eY0bD$xQ0Rn7ywTUcch^fiU@3s$$2oGH2zjVLKXhHFVeRb7X9al9T^vwldPKFJDgqJgVky!QgWQ5g#*^lsEMykUV6o#h2UPhoy3@rqscqeBJ!GJwEJWj$Li^bynEWhtTsojwCiaB29gU&thp0O5$PI1Lq9UzpLTc8ZZ1$OgJSN11xJzpjC1&mNh7T@wD8j&M&Rjk^TXZckBHT9fVmS1!F0GHslpaBTZZ#DD6p@!CkDk", 632);
    pm_addstring(pm, (unsigned char *)"gGoojjNNZnj09JmxraEvWyaO9raPqdABd0iZSqoMVyPtyIfpLolELRVtyAjYyIE4x64spFYW6jXCE5CIRMdxaO0YfzlvkFYHwM1gSUbOqxuXkIYg9WFuQ6MG!VFoyoJ1xk0Y96mSP7eUhOmZeU9rzItkRzRVfvlKSvmxYwX7yoOIPVcxhL46ufZnn199V225aeKi8qXrCoUxiGELsNpV", 212);
    pm_addstring(pm, (unsigned char *)"E4yE1Zj7vkE2mNzp8Ikimo2lIEAFFyKZEsz7N7Sue629yw8RJ3zvHxWleH9KJqt0t9GTfL75eZqayVQOzHagHwLdacJHjaZ2ul4zHCb1af7CPpsSp6wstt8xBhqAaSydUCJcoFyetcxEh0zM1MiMDKEyDHmq4OhvSMFbI3SbgGoZ8SiMvCcWX2b0YsQVgF65LoEEKIL71xye0pBNxD4bNoQgvFTcNxK33mGB37oNekERK5qUOvO9xpKlWIaTDtkOTTnXL6WjK0WKbONOCw8x1kCKwk12aVUgM27L3kLTIdo0s6VUVAAGeqYFNRYzyn2F7qJSpEiygYaNdLkiJdLcf0QguBhhIOfTKh4henpss2Z2wBStst1lZvrZL6R5OjwMEFsUEsUjS8z0nOoFEAl1TOY4P9elaAPQ7ornOFAUybAVOZEmlu2atvLJVvv1VCsIEmwnezgyULFG886hbVRO5Z9uxWti2G8FmcI09JQkuXZ3vAyyEUVmLSJhc1tKMVpv8dRKq61ROSfs6f1qhepGxoJZQTkMRMaSii71JW6Fv0XzSSrsYEylWgkRNS1JUkB3Qe8YeryJ9ubX5I5VCd0JoStAY704klkXDUE2bKNMFALcFNjVDUdjpeu7q1TslRLFbpSrdfionpmmFyIzopE4CAYkz2jlablPHLUeBMjGgiBgbYOulSIKSuwx7rlvFCbhyTsibHtZxEY6TQEOkTWSPMXqp", 729);
    pm_addstring(pm, (unsigned char *)"99SYZnuFsHE2GyHLrMpkHhgLSsHbu669yJ8Qd1GgeYQmHl7L1CQ9o7WK5lU395rk7HF9eSCqMlS92tiS8XeDXD0dvsp5qF0GB215Fngv4fTJCOJU8J2dkB8V7SYy7Oc3SCVPosh6ttX35bteoOZxJC8t2pQ3jNeeqkwPyi0hL7jcVbPO4PTs9Fp8wKzg15XkgWHI5LG9cz3Rvh9cESHNxicnYapAf3rMvQr6IfXmzbBuMJcq2OkAujQxmBNmEFtN5qB2o5zIOeU49BBCmWvrlWXuodMBKOJB7P8soAnLZinDaYg7Xm4LtwPe0q0rNX6BBCW7SwvKjmFvvwApukgaV4J5RhzwfQ3MfGmiW4Gp5LyGuelGZDRnO2v19pG86G4P1fvTpyWWagPrpMedLrBhs6fw05n0XjbkBVm6G7vrFTwutOGDIKLZwZUnSar7e6D2rLwJXT0LydOZzflGwxjKHg9yim7n2iAExTaj4fNBQWMt4uzwttPnFu", 502);
    pm_addstring(pm, (unsigned char *)"lC14n8wwW12TxLorJKoO5PLRqSuafVm5MFC2OEzhvY5XD07nHYwGdzfDQ8XmUzUmyKNPjBp62AxJAJ97V9WPYAKHJDmKZ4AvSH1n2Ho651F9WgqbXzj3Abdr3m5wtDY8cb3kS3lRYcmNepUlOSuHw2Gy3EtPFLHVLyF2Fjls0mK6N2kxOR023jQTWwpqdHUN6MnTLn61Mx2c9sDrwSmvxaPcsGXhA6pnnPczdJhts6XvdIXmKaQzYpaC3niN82tSjwtR0VSsOeAE3WHZvjTxAduYagif23clk23QIdoenFqpxnqBLhFD9ShEfSuot28Ta0hcaIyxKuMi9KjD1f8HRM7mZ7zwQ3KaeSLoi7ovgWcUXMFkkCQsZ6rFLZlGRgBEFz2rfwXOmU0rGA94DIejZ6zy8VtI6D2nY13fsyPPGTpx80OwcCQhDEoJ83bEd1zlzwK9F15RDupRAmpy6eMz7BRosGy3rqgtauypdqbQ7vxWJ62X3dALkAwWgUu1WQNPRQpax2cusZ12igIJxRqd2MIMJw93C2BAU4eXphcFimgGLWlS3mUAtDP8ZUHqGjkACC4AQ6Fy3CigMKpLWAn7wPytFDkTXmVb0EeO0Mc6SEOOSWXbXukBT37kPMJlDcFL0pQBDqsuUp5Tny6EU", 657);
    pm_addstring(pm, (unsigned char *)"mjXnO5lUQKGB1RnpYMXq72alSUcZLthkcvCYuwmeTfThqjVbe0PzN6hId9a6u9DGxL37pbPcyBycDYWZmhwPF6CdQzyNxlAhLyjU0FG9jYICzxsGt", 113);
    pm_addstring(pm, (unsigned char *)"kDR1XpjpFrgtED8y24YD76zgJz5hIAEfRJylpD7wO8oHE0xyjPUMTcqEabuZtvnUNgfZZjclcPlwOzJALTQQ1KzlGzfrGxKnbxRYiCw0IXgfdgAINS9gaNTVVT2AYIfxG5oiTqSFGNe8mx2inlzPHJSdZLx7Gh0Rmncf5MESgPxdkVKHxS0dGHEx9WIbKiimQMy5LHl2RwRTMWybSy8X1PYiXcAGw1x9HQ7QjavVkjBBLt8GnbeulhV0dN2wTO9gnoIjhulLzRltiHx57vafQM4fOalysOUqfhUppN494uD482cWkuNq08bAaNjaNq19gL9hI3AJNXzFl7KH1f6h3xd0qGuunOQv5N25U9X676qRm3hVlkgra8EVKRMW2vbCO9SE55py9k7Ehg6lrnPy0WayZfJly2ZlgwtFqVFR7dBXYklRZcru3RQdVDFOvObNJmJPx0A82LC1Q0bqpAzuSQQ2mOWKhg6bEF0q2tWSB3d0I9m8I97MoCKDWDfIHBrpHLW1GbDJJdZCePakRSn6ityD0reUreEv3Nk3oKGY3mhVRqvwq1K374D80HBudTsYNP6xa1yJtANEus3YnqI38AQ9eu46q1xfmYfY28V9mvOviAnj3G9nGRkdIzO6CkNIPWZ7T0TTlEmftZKe1MIXZYW2AnIXaSn5YCmzT6rYqIpT0lO4TGTX8Nu2Aj8GDbhAj0Zs0cnxS5b3FSylzp8MHuJsdyNxgMGTeNtGloWQwvfhHdCsj9rEbchclaBriYZtx7rzaxbIB", 777);
    pm_makeFSM(pm);

    args_t **argus = (args_t **)malloc(NUM_OF_THREADS * sizeof(args_t *));

    clock_t begin;
    clock_t end;
    begin = clock();

    for (int i = 0; i < NUM_OF_THREADS; i++)
    {
        argus[i] = (args_t *)malloc(sizeof(args_t));
        argus[i]->pm = pm;
        argus[i]->data = (unsigned char *)&s1[i];

        pthread_create(&tids[i], NULL, myFunc, (void *)argus[i]);
    }
    for (int i = 0; i < NUM_OF_THREADS; i++)
    {
        pthread_join(tids[i], NULL);
    }
        free(tids);


    end = clock();
    fclose(fp);
    printf("\nExecuted time is: %f ms. \n\n", ((double)(end - begin) / CLOCKS_PER_SEC) * 1000);
}

void *myFunc(void *arguments)
{
    args_t *argus = (args_t *)arguments;
    search_and_destroy(argus->pm, (char *)argus->data);
    // pm_destroy(argus->pm);
    argus->data = NULL;
    pthread_exit(NULL);
}

// int main(int argc, char *argv[])   ********************* DFA - parallel ***********************
// {
//     int NUM_OF_THREADS = 10;
//     pthread_t tids[NUM_OF_THREADS];

//     pm_t **pms = (pm_t **)malloc(NUM_OF_THREADS * sizeof(pm_t *));

//     if (!pms)
//     {
//         return -1;
//     }

//     int returnVal = pms_init(NUM_OF_THREADS, &(*pms));

//     if (returnVal == -1)
//     {
//         free(pms);
//         return -1;
//     }

//     FILE *fp;
//     fp = fopen("/home/student/Final Project/Pattern-Matching/chars_stream.txt", "r");
//     fgets(s1, PM_CHARACTERS, (FILE *)fp);
//     fclose(fp);

//     clock_t begin;
//     clock_t end;
//     begin = clock();

//     for (int i = 0; i < NUM_OF_THREADS; i++)
//     {
//         pthread_create(&tids[i], NULL, myFunc, (void *)&(pms[i][0]));
//     }
//     for (int i = 0; i < NUM_OF_THREADS; i++)
//     {
//         pthread_join(tids[i], NULL);
//     }

//     end = clock();
//     printf("\nExecuted time is: %f ms. \n\n", ((double)(end - begin) / CLOCKS_PER_SEC) * 1000);
// }

// void *myFunc(void *pm)
// {
//     pm_t *cur_pm = (pm_t *)pm;
//     search_and_destroy(cur_pm, s1);
//     pm_destroy(cur_pm);
// }

// int pms_init(int NUM_OF_THREADS, pm_t **pms)
// {
//     int i, failureIndication = 0;
//     for (i = 0; i < NUM_OF_THREADS; i++)
//     {
//         pms[i] = (pm_t *)malloc(sizeof(pm_t));
//         if (!pms[i] || pm_init(pms[i]) == -1)
//         {
//             failureIndication = 1;
//             break;
//         }
//     }

//     if (failureIndication == 1)
//     {
//         for (; i >= 0; i--)
//         {
//             free(pms[i]);
//         }
//         return -1;
//     }
//     pm_addstring(pms[0], (unsigned char *)"DR5HY@yLcG*6GD7sXf1paMwfLiD&Gdub98QW0FzqpWjEHxMQZ0kJoZo#hJVJ9QGD3VdtIadTp4me*sfAzT3NjL0$4L2FC6NzmDd6mhtyW04o@*qO#le31L^MO$X#CpFYxHSegmPDjiCf8R^cmjtARILA*Z^e!n&pPEC&WKKNHO7I4P6XPA0&#75RJ7ghWsHXz^r3vSh@OzJK#jqaZBvz9x4EF*$hlfcrykrKdLgio7AaxLiuDD805i@4&s4WWusG7fsf5iNllmMrVkb7RtzSU@fZo1bLVaceLA#CkuWSpvaD0b#ZGwkcTesehS5UwKUObzILks#sdUJDBdadWpgP&lanjk^CbqjbumPvz2chvCa&Sc120z5p60TEDBc9esY3vbtq8w5j^M0K$$UYZB2NMAUjJ*kK^1SoX5f03yxAh0#!R$92Ui&88zhwxsnq2evaygzksj##w1Sx^jvknA&Vyba^Dt1j8CM", 479);
//     pm_addstring(pms[1], (unsigned char *)"#^1wds5KDaplo&!mEFt7Xak&SFpa0fNP8xZwdmRXS7tZ2!B9P4na&yHXT*JAsuGUmeYypacsvkxnkUDgC*GWmUBf5WKT8E2k!UF@pK3g86ansmrmOfspzeSGjJmjfmg9#*pOYs@b^cGZLycrtv@*wrxCgeVvb&4v3mZwRwDgdEPZMQX0lyANM23HL3oWm#dm2nS6A!OAGq$QjoA9*DK&*ffQ2&wZxJf06Vxck1om!ccKe7qpqZk5iJECurt!3s6EpTPdpgxeKAZLlR4Bt1V#5ZBfX4tW0^3I0zlJvomrNOiOKi!vK^8lX36JkuJCzhrvAKk#aC^RtQAtS$qK^A*30I^$Ed3KZ82rgwG7z4mswABIzOH*3bm!6C@dCx", 378);
//     pm_addstring(pms[2], (unsigned char *)"lKEX*VA$uFqJ2FfJE5*EI!MU5vMg!KGq9xP$fAshHrIVXgQfL45&1tT&TOUcTcKUYGoAgTEIrl6NiL8n&DPIvmomUpBp2sHM#*g1V2&xsYFOOznYQwzoqnBp2w*P", 124);
//     pm_addstring(pms[3], (unsigned char *)"dA1sI0NGxYzGPWQreWrptknA9SMf44Bfl9hXDAV!TvWSGB!1e66$2wG6tx&Re*LoEBwJ!dUL@qOJuJ2kffeazN1Mhcvuc4$J7Xjox$1fAwSW6ziIvlv7PstENidAXl7DnlR4W7*yCjz$XhPvmWLflmvohg!XxTI*c#Ra5QRL2HKmc&PCzjQM*wzjSbbxLqC*G$Qe^nFIM8nEzfV4keXikucg$Z1GCW04eBYTWp*HCwrHHGqtC*XmRsPx@iGMQg5yGkeXQMiiQJUyLnp1f3PC4evR@^zijZ$H&XkZ7XuTUTcVV8vkIUpzoTRjYw$C1arvd&Y!&rFY35*RFDj!bv&&yl*sN4W3ALkDGXudUKRlPFYy!D89kXQmwkyHjO*&RAv#eJ2vBN7#*NMN*eY0bD$xQ0Rn7ywTUcch^fiU@3s$$2oGH2zjVLKXhHFVeRb7X9al9T^vwldPKFJDgqJgVky!QgWQ5g#*^lsEMykUV6o#h2UPhoy3@rqscqeBJ!GJwEJWj$Li^bynEWhtTsojwCiaB29gU&thp0O5$PI1Lq9UzpLTc8ZZ1$OgJSN11xJzpjC1&mNh7T@wD8j&M&Rjk^TXZckBHT9fVmS1!F0GHslpaBTZZ#DD6p@!CkDk", 632);
//     pm_addstring(pms[4], (unsigned char *)"gGoojjNNZnj09JmxraEvWyaO9raPqdABd0iZSqoMVyPtyIfpLolELRVtyAjYyIE4x64spFYW6jXCE5CIRMdxaO0YfzlvkFYHwM1gSUbOqxuXkIYg9WFuQ6MG!VFoyoJ1xk0Y96mSP7eUhOmZeU9rzItkRzRVfvlKSvmxYwX7yoOIPVcxhL46ufZnn199V225aeKi8qXrCoUxiGELsNpV", 212);
//     pm_addstring(pms[5], (unsigned char *)"E4yE1Zj7vkE2mNzp8Ikimo2lIEAFFyKZEsz7N7Sue629yw8RJ3zvHxWleH9KJqt0t9GTfL75eZqayVQOzHagHwLdacJHjaZ2ul4zHCb1af7CPpsSp6wstt8xBhqAaSydUCJcoFyetcxEh0zM1MiMDKEyDHmq4OhvSMFbI3SbgGoZ8SiMvCcWX2b0YsQVgF65LoEEKIL71xye0pBNxD4bNoQgvFTcNxK33mGB37oNekERK5qUOvO9xpKlWIaTDtkOTTnXL6WjK0WKbONOCw8x1kCKwk12aVUgM27L3kLTIdo0s6VUVAAGeqYFNRYzyn2F7qJSpEiygYaNdLkiJdLcf0QguBhhIOfTKh4henpss2Z2wBStst1lZvrZL6R5OjwMEFsUEsUjS8z0nOoFEAl1TOY4P9elaAPQ7ornOFAUybAVOZEmlu2atvLJVvv1VCsIEmwnezgyULFG886hbVRO5Z9uxWti2G8FmcI09JQkuXZ3vAyyEUVmLSJhc1tKMVpv8dRKq61ROSfs6f1qhepGxoJZQTkMRMaSii71JW6Fv0XzSSrsYEylWgkRNS1JUkB3Qe8YeryJ9ubX5I5VCd0JoStAY704klkXDUE2bKNMFALcFNjVDUdjpeu7q1TslRLFbpSrdfionpmmFyIzopE4CAYkz2jlablPHLUeBMjGgiBgbYOulSIKSuwx7rlvFCbhyTsibHtZxEY6TQEOkTWSPMXqp", 729);
//     pm_addstring(pms[6], (unsigned char *)"99SYZnuFsHE2GyHLrMpkHhgLSsHbu669yJ8Qd1GgeYQmHl7L1CQ9o7WK5lU395rk7HF9eSCqMlS92tiS8XeDXD0dvsp5qF0GB215Fngv4fTJCOJU8J2dkB8V7SYy7Oc3SCVPosh6ttX35bteoOZxJC8t2pQ3jNeeqkwPyi0hL7jcVbPO4PTs9Fp8wKzg15XkgWHI5LG9cz3Rvh9cESHNxicnYapAf3rMvQr6IfXmzbBuMJcq2OkAujQxmBNmEFtN5qB2o5zIOeU49BBCmWvrlWXuodMBKOJB7P8soAnLZinDaYg7Xm4LtwPe0q0rNX6BBCW7SwvKjmFvvwApukgaV4J5RhzwfQ3MfGmiW4Gp5LyGuelGZDRnO2v19pG86G4P1fvTpyWWagPrpMedLrBhs6fw05n0XjbkBVm6G7vrFTwutOGDIKLZwZUnSar7e6D2rLwJXT0LydOZzflGwxjKHg9yim7n2iAExTaj4fNBQWMt4uzwttPnFu", 502);
//     pm_addstring(pms[7], (unsigned char *)"lC14n8wwW12TxLorJKoO5PLRqSuafVm5MFC2OEzhvY5XD07nHYwGdzfDQ8XmUzUmyKNPjBp62AxJAJ97V9WPYAKHJDmKZ4AvSH1n2Ho651F9WgqbXzj3Abdr3m5wtDY8cb3kS3lRYcmNepUlOSuHw2Gy3EtPFLHVLyF2Fjls0mK6N2kxOR023jQTWwpqdHUN6MnTLn61Mx2c9sDrwSmvxaPcsGXhA6pnnPczdJhts6XvdIXmKaQzYpaC3niN82tSjwtR0VSsOeAE3WHZvjTxAduYagif23clk23QIdoenFqpxnqBLhFD9ShEfSuot28Ta0hcaIyxKuMi9KjD1f8HRM7mZ7zwQ3KaeSLoi7ovgWcUXMFkkCQsZ6rFLZlGRgBEFz2rfwXOmU0rGA94DIejZ6zy8VtI6D2nY13fsyPPGTpx80OwcCQhDEoJ83bEd1zlzwK9F15RDupRAmpy6eMz7BRosGy3rqgtauypdqbQ7vxWJ62X3dALkAwWgUu1WQNPRQpax2cusZ12igIJxRqd2MIMJw93C2BAU4eXphcFimgGLWlS3mUAtDP8ZUHqGjkACC4AQ6Fy3CigMKpLWAn7wPytFDkTXmVb0EeO0Mc6SEOOSWXbXukBT37kPMJlDcFL0pQBDqsuUp5Tny6EU", 657);
//     pm_addstring(pms[8], (unsigned char *)"mjXnO5lUQKGB1RnpYMXq72alSUcZLthkcvCYuwmeTfThqjVbe0PzN6hId9a6u9DGxL37pbPcyBycDYWZmhwPF6CdQzyNxlAhLyjU0FG9jYICzxsGt", 113);
//     pm_addstring(pms[9], (unsigned char *)"kDR1XpjpFrgtED8y24YD76zgJz5hIAEfRJylpD7wO8oHE0xyjPUMTcqEabuZtvnUNgfZZjclcPlwOzJALTQQ1KzlGzfrGxKnbxRYiCw0IXgfdgAINS9gaNTVVT2AYIfxG5oiTqSFGNe8mx2inlzPHJSdZLx7Gh0Rmncf5MESgPxdkVKHxS0dGHEx9WIbKiimQMy5LHl2RwRTMWybSy8X1PYiXcAGw1x9HQ7QjavVkjBBLt8GnbeulhV0dN2wTO9gnoIjhulLzRltiHx57vafQM4fOalysOUqfhUppN494uD482cWkuNq08bAaNjaNq19gL9hI3AJNXzFl7KH1f6h3xd0qGuunOQv5N25U9X676qRm3hVlkgra8EVKRMW2vbCO9SE55py9k7Ehg6lrnPy0WayZfJly2ZlgwtFqVFR7dBXYklRZcru3RQdVDFOvObNJmJPx0A82LC1Q0bqpAzuSQQ2mOWKhg6bEF0q2tWSB3d0I9m8I97MoCKDWDfIHBrpHLW1GbDJJdZCePakRSn6ityD0reUreEv3Nk3oKGY3mhVRqvwq1K374D80HBudTsYNP6xa1yJtANEus3YnqI38AQ9eu46q1xfmYfY28V9mvOviAnj3G9nGRkdIzO6CkNIPWZ7T0TTlEmftZKe1MIXZYW2AnIXaSn5YCmzT6rYqIpT0lO4TGTX8Nu2Aj8GDbhAj0Zs0cnxS5b3FSylzp8MHuJsdyNxgMGTeNtGloWQwvfhHdCsj9rEbchclaBriYZtx7rzaxbIB", 777);

//     pm_makeFSM(pms[0]);
//     pm_makeFSM(pms[1]);
//     pm_makeFSM(pms[2]);
//     pm_makeFSM(pms[3]);
//     pm_makeFSM(pms[4]);
//     pm_makeFSM(pms[5]);
//     pm_makeFSM(pms[6]);
//     pm_makeFSM(pms[7]);
//     pm_makeFSM(pms[8]);
//     pm_makeFSM(pms[9]);

//     printf("\n**********************************  Initilaize PMs passed succesfully  ********************************\n");
//     return 0;
// }

// int main(int argc, char *argv[])
// {
//     clock_t begin;
//     clock_t end;

//     begin = clock();
//     pm_t *pm = (pm_t *)malloc(sizeof(pm_t));

//     if (!pm)
//     {
//         return -1;
//     }
//     _test1(pm);
//     free(pm);
//     end = clock();

//     printf("\n***************************************************************************\n");
//     printf("****************************** Aho - Corasick *****************************\n");
//     printf("***************************************************************************\n\n");
//     printf("\nExecuted time is: %f ms. \n\n", ((double)(end - begin) / CLOCKS_PER_SEC) * 1000);

//     begin = clock();

// pm_t *pm1 = (pm_t *)malloc(sizeof(pm_t));
// pm_t *pm2 = (pm_t *)malloc(sizeof(pm_t));
// pm_t *pm3 = (pm_t *)malloc(sizeof(pm_t));
// pm_t *pm4 = (pm_t *)malloc(sizeof(pm_t));
// pm_t *pm5 = (pm_t *)malloc(sizeof(pm_t));
// pm_t *pm6 = (pm_t *)malloc(sizeof(pm_t));
// pm_t *pm7 = (pm_t *)malloc(sizeof(pm_t));
// pm_t *pm8 = (pm_t *)malloc(sizeof(pm_t));
// pm_t *pm9 = (pm_t *)malloc(sizeof(pm_t));
// pm_t *pm10 = (pm_t *)malloc(sizeof(pm_t));

// if (!pm1 || !pm2 || !pm3 || !pm4 || !pm5 || !pm6 || !pm7 || !pm8 || !pm9 || !pm10)
// {
//     return -1;
// }

// FILE *fp;
// fp = fopen("log.txt", "w+");
// dup2(fileno(fp), fileno(stdout));

// _test2(pm1, pm2, pm3, pm4, pm5, pm6, pm7, pm8, pm9, pm10);
// free(pm1);
// free(pm2);
// free(pm3);
// free(pm4);
// free(pm5);
// free(pm6);
// free(pm7);
// free(pm8);
// free(pm9);
// free(pm10);

// // fclose(fp);
// end = clock();

//     printf("\n***************************************************************************\n");
//     printf("************************** String Searching with DFA. *********************\n");
//     printf("***************************************************************************\n\n");
//     printf("\nExecuted time is: %f ms. \n\n", ((double)(end - begin) / CLOCKS_PER_SEC) * 1000);

//     return 0;
// }

void _test1(pm_t *pm1)
{
    /* ------------------------------ Aho - Corasick ---------------------------------*/
    if (pm_init(pm1) == -1)
    {
        printf("error init pm");
        exit(-1);
    }

    // pm_addstring(pm, (unsigned char*)"ace", 3);
    // pm_addstring(pm, (unsigned char*)"abcdefghijklmnopqrstuvwxyz", 26);
    // pm_addstring(pm, (unsigned char*)"cbdefghijklmnopqrstuvwxyza", 26);
    // pm_addstring(pm, (unsigned char*)"cdefghijklmnopqrstuvwxyzab", 26);
    // pm_addstring(pm, (unsigned char*)"defghijklmnopqrstuvwxyzabc", 26);
    // pm_addstring(pm, (unsigned char*)"efghijklmnopqrstuvwxyzabcd", 26);
    // pm_addstring(pm, (unsigned char*)"fghijklmnopqrstuvwxyzabcde", 26);
    // pm_addstring(pm, (unsigned char*)"ghijklmnopqrstuvwxyzabcdef", 26);
    // pm_addstring(pm, (unsigned char*)"hijklmnopqrstuvwxyzabcdefg", 26);
    // pm_addstring(pm, (unsigned char*)"ijklmnopqrstuvwxyzabcdefgh", 26);
    // pm_addstring(pm, (unsigned char*)"jklmnopqrstuvwxyzabcdefghi", 26);
    // pm_addstring(pm, (unsigned char*)"klmnopqrstuvwxyzabcdefghij", 26);
    // pm_addstring(pm, (unsigned char*)"lmnopqrstuvwxyzabcdefghijk", 26);
    // pm_addstring(pm, (unsigned char*)"mnopqrstuvwxyzabcdefghijkl", 26);
    // pm_addstring(pm, (unsigned char*)"nopqrstuvwxyzabcdefghijklm", 26);
    // pm_addstring(pm, (unsigned char*)"opqrstuvwxyzabcdefghijklmn", 26);
    // pm_addstring(pm, (unsigned char*)"pqrstuvwxyzabcdefghijklmno", 26);

    pm_addstring(pm1, (unsigned char *)"KX6M", 4);
    pm_addstring(pm1, (unsigned char *)"fbxA", 4);
    // pm_addstring(pm1, (unsigned char *)"#^1wds5KDaplo&!mEFt7Xak&SFpa0fNP8xZwdmRXS7tZ2!B9P4na&yHXT*JAsuGUmeYypacsvkxnkUDgC*GWmUBf5WKT8E2k!UF@pK3g86ansmrmOfspzeSGjJmjfmg9#*pOYs@b^cGZLycrtv@*wrxCgeVvb&4v3mZwRwDgdEPZMQX0lyANM23HL3oWm#dm2nS6A!OAGq$QjoA9*DK&*ffQ2&wZxJf06Vxck1om!ccKe7qpqZk5iJECurt!3s6EpTPdpgxeKAZLlR4Bt1V#5ZBfX4tW0^3I0zlJvomrNOiOKi!vK^8lX36JkuJCzhrvAKk#aC^RtQAtS$qK^A*30I^$Ed3KZ82rgwG7z4mswABIzOH*3bm!6C@dCx", 378);
    pm_addstring(pm1, (unsigned char *)"lKEX*VA$uFqJ2FfJE5*EI!MU5vMg!KGq9xP$fAshHrIVXgQfL45&1tT&TOUcTcKUYGoAgTEIrl6NiL8n&DPIvmomUpBp2sHM#*g1V2&xsYFOOznYQwzoqnBp2w*P", 124);
    pm_addstring(pm1, (unsigned char *)"dA1sI0NGxYzGPWQreWrptknA9SMf44Bfl9hXDAV!TvWSGB!1e66$2wG6tx&Re*LoEBwJ!dUL@qOJuJ2kffeazN1Mhcvuc4$J7Xjox$1fAwSW6ziIvlv7PstENidAXl7DnlR4W7*yCjz$XhPvmWLflmvohg!XxTI*c#Ra5QRL2HKmc&PCzjQM*wzjSbbxLqC*G$Qe^nFIM8nEzfV4keXikucg$Z1GCW04eBYTWp*HCwrHHGqtC*XmRsPx@iGMQg5yGkeXQMiiQJUyLnp1f3PC4evR@^zijZ$H&XkZ7XuTUTcVV8vkIUpzoTRjYw$C1arvd&Y!&rFY35*RFDj!bv&&yl*sN4W3ALkDGXudUKRlPFYy!D89kXQmwkyHjO*&RAv#eJ2vBN7#*NMN*eY0bD$xQ0Rn7ywTUcch^fiU@3s$$2oGH2zjVLKXhHFVeRb7X9al9T^vwldPKFJDgqJgVky!QgWQ5g#*^lsEMykUV6o#h2UPhoy3@rqscqeBJ!GJwEJWj$Li^bynEWhtTsojwCiaB29gU&thp0O5$PI1Lq9UzpLTc8ZZ1$OgJSN11xJzpjC1&mNh7T@wD8j&M&Rjk^TXZckBHT9fVmS1!F0GHslpaBTZZ#DD6p@!CkDk", 632);
    pm_addstring(pm1, (unsigned char *)"gGoojjNNZnj09JmxraEvWyaO9raPqdABd0iZSqoMVyPtyIfpLolELRVtyAjYyIE4x64spFYW6jXCE5CIRMdxaO0YfzlvkFYHwM1gSUbOqxuXkIYg9WFuQ6MG!VFoyoJ1xk0Y96mSP7eUhOmZeU9rzItkRzRVfvlKSvmxYwX7yoOIPVcxhL46ufZnn199V225aeKi8qXrCoUxiGELsNpV", 212);
    pm_addstring(pm1, (unsigned char *)"E4yE1Zj7vkE2mNzp8Ikimo2lIEAFFyKZEsz7N7Sue629yw8RJ3zvHxWleH9KJqt0t9GTfL75eZqayVQOzHagHwLdacJHjaZ2ul4zHCb1af7CPpsSp6wstt8xBhqAaSydUCJcoFyetcxEh0zM1MiMDKEyDHmq4OhvSMFbI3SbgGoZ8SiMvCcWX2b0YsQVgF65LoEEKIL71xye0pBNxD4bNoQgvFTcNxK33mGB37oNekERK5qUOvO9xpKlWIaTDtkOTTnXL6WjK0WKbONOCw8x1kCKwk12aVUgM27L3kLTIdo0s6VUVAAGeqYFNRYzyn2F7qJSpEiygYaNdLkiJdLcf0QguBhhIOfTKh4henpss2Z2wBStst1lZvrZL6R5OjwMEFsUEsUjS8z0nOoFEAl1TOY4P9elaAPQ7ornOFAUybAVOZEmlu2atvLJVvv1VCsIEmwnezgyULFG886hbVRO5Z9uxWti2G8FmcI09JQkuXZ3vAyyEUVmLSJhc1tKMVpv8dRKq61ROSfs6f1qhepGxoJZQTkMRMaSii71JW6Fv0XzSSrsYEylWgkRNS1JUkB3Qe8YeryJ9ubX5I5VCd0JoStAY704klkXDUE2bKNMFALcFNjVDUdjpeu7q1TslRLFbpSrdfionpmmFyIzopE4CAYkz2jlablPHLUeBMjGgiBgbYOulSIKSuwx7rlvFCbhyTsibHtZxEY6TQEOkTWSPMXqp", 729);
    pm_addstring(pm1, (unsigned char *)"99SYZnuFsHE2GyHLrMpkHhgLSsHbu669yJ8Qd1GgeYQmHl7L1CQ9o7WK5lU395rk7HF9eSCqMlS92tiS8XeDXD0dvsp5qF0GB215Fngv4fTJCOJU8J2dkB8V7SYy7Oc3SCVPosh6ttX35bteoOZxJC8t2pQ3jNeeqkwPyi0hL7jcVbPO4PTs9Fp8wKzg15XkgWHI5LG9cz3Rvh9cESHNxicnYapAf3rMvQr6IfXmzbBuMJcq2OkAujQxmBNmEFtN5qB2o5zIOeU49BBCmWvrlWXuodMBKOJB7P8soAnLZinDaYg7Xm4LtwPe0q0rNX6BBCW7SwvKjmFvvwApukgaV4J5RhzwfQ3MfGmiW4Gp5LyGuelGZDRnO2v19pG86G4P1fvTpyWWagPrpMedLrBhs6fw05n0XjbkBVm6G7vrFTwutOGDIKLZwZUnSar7e6D2rLwJXT0LydOZzflGwxjKHg9yim7n2iAExTaj4fNBQWMt4uzwttPnFu", 502);
    pm_addstring(pm1, (unsigned char *)"lC14n8wwW12TxLorJKoO5PLRqSuafVm5MFC2OEzhvY5XD07nHYwGdzfDQ8XmUzUmyKNPjBp62AxJAJ97V9WPYAKHJDmKZ4AvSH1n2Ho651F9WgqbXzj3Abdr3m5wtDY8cb3kS3lRYcmNepUlOSuHw2Gy3EtPFLHVLyF2Fjls0mK6N2kxOR023jQTWwpqdHUN6MnTLn61Mx2c9sDrwSmvxaPcsGXhA6pnnPczdJhts6XvdIXmKaQzYpaC3niN82tSjwtR0VSsOeAE3WHZvjTxAduYagif23clk23QIdoenFqpxnqBLhFD9ShEfSuot28Ta0hcaIyxKuMi9KjD1f8HRM7mZ7zwQ3KaeSLoi7ovgWcUXMFkkCQsZ6rFLZlGRgBEFz2rfwXOmU0rGA94DIejZ6zy8VtI6D2nY13fsyPPGTpx80OwcCQhDEoJ83bEd1zlzwK9F15RDupRAmpy6eMz7BRosGy3rqgtauypdqbQ7vxWJ62X3dALkAwWgUu1WQNPRQpax2cusZ12igIJxRqd2MIMJw93C2BAU4eXphcFimgGLWlS3mUAtDP8ZUHqGjkACC4AQ6Fy3CigMKpLWAn7wPytFDkTXmVb0EeO0Mc6SEOOSWXbXukBT37kPMJlDcFL0pQBDqsuUp5Tny6EU", 657);
    pm_addstring(pm1, (unsigned char *)"mjXnO5lUQKGB1RnpYMXq72alSUcZLthkcvCYuwmeTfThqjVbe0PzN6hId9a6u9DGxL37pbPcyBycDYWZmhwPF6CdQzyNxlAhLyjU0FG9jYICzxsGt", 113);
    pm_addstring(pm1, (unsigned char *)"kDR1XpjpFrgtED8y24YD76zgJz5hIAEfRJylpD7wO8oHE0xyjPUMTcqEabuZtvnUNgfZZjclcPlwOzJALTQQ1KzlGzfrGxKnbxRYiCw0IXgfdgAINS9gaNTVVT2AYIfxG5oiTqSFGNe8mx2inlzPHJSdZLx7Gh0Rmncf5MESgPxdkVKHxS0dGHEx9WIbKiimQMy5LHl2RwRTMWybSy8X1PYiXcAGw1x9HQ7QjavVkjBBLt8GnbeulhV0dN2wTO9gnoIjhulLzRltiHx57vafQM4fOalysOUqfhUppN494uD482cWkuNq08bAaNjaNq19gL9hI3AJNXzFl7KH1f6h3xd0qGuunOQv5N25U9X676qRm3hVlkgra8EVKRMW2vbCO9SE55py9k7Ehg6lrnPy0WayZfJly2ZlgwtFqVFR7dBXYklRZcru3RQdVDFOvObNJmJPx0A82LC1Q0bqpAzuSQQ2mOWKhg6bEF0q2tWSB3d0I9m8I97MoCKDWDfIHBrpHLW1GbDJJdZCePakRSn6ityD0reUreEv3Nk3oKGY3mhVRqvwq1K374D80HBudTsYNP6xa1yJtANEus3YnqI38AQ9eu46q1xfmYfY28V9mvOviAnj3G9nGRkdIzO6CkNIPWZ7T0TTlEmftZKe1MIXZYW2AnIXaSn5YCmzT6rYqIpT0lO4TGTX8Nu2Aj8GDbhAj0Zs0cnxS5b3FSylzp8MHuJsdyNxgMGTeNtGloWQwvfhHdCsj9rEbchclaBriYZtx7rzaxbIB", 777);
    pm_makeFSM(pm1);

    FILE *fp;
    char s1[PM_CHARACTERS];
    fp = fopen("/home/student/Final Project/Pattern-Matching/chars_stream.txt", "r");
    fgets(s1, PM_CHARACTERS, (FILE *)fp);
    fclose(fp);

    search_and_destroy(pm1, s1);
    pm_destroy(pm1);
}

void _test2(pm_t *pm1, pm_t *pm2, pm_t *pm3, pm_t *pm4, pm_t *pm5, pm_t *pm6, pm_t *pm7, pm_t *pm8, pm_t *pm9, pm_t *pm10)
{
    /* ------------------------------ DFA ---------------------------------*/

    if (pm_init(pm1) == -1 || pm_init(pm2) == -1 || pm_init(pm3) == -1 || pm_init(pm4) == -1 || pm_init(pm5) == -1 || pm_init(pm6) == -1 || pm_init(pm7) == -1 || pm_init(pm8) == -1 || pm_init(pm9) == -1 || pm_init(pm10) == -1)
    {
        printf("error init pm");
        exit(-1);
    }

    // 1-3 non pattern , 4-6 half patterns, 7-10 full pattern
    pm_addstring(pm1, (unsigned char *)"DR5HY@yLcG*6GD7sXf1paMwfLiD&Gdub98QW0FzqpWjEHxMQZ0kJoZo#hJVJ9QGD3VdtIadTp4me*sfAzT3NjL0$4L2FC6NzmDd6mhtyW04o@*qO#le31L^MO$X#CpFYxHSegmPDjiCf8R^cmjtARILA*Z^e!n&pPEC&WKKNHO7I4P6XPA0&#75RJ7ghWsHXz^r3vSh@OzJK#jqaZBvz9x4EF*$hlfcrykrKdLgio7AaxLiuDD805i@4&s4WWusG7fsf5iNllmMrVkb7RtzSU@fZo1bLVaceLA#CkuWSpvaD0b#ZGwkcTesehS5UwKUObzILks#sdUJDBdadWpgP&lanjk^CbqjbumPvz2chvCa&Sc120z5p60TEDBc9esY3vbtq8w5j^M0K$$UYZB2NMAUjJ*kK^1SoX5f03yxAh0#!R$92Ui&88zhwxsnq2evaygzksj##w1Sx^jvknA&Vyba^Dt1j8CM", 479);
    pm_makeFSM(pm1);
    pm_addstring(pm2, (unsigned char *)"#^1wds5KDaplo&!mEFt7Xak&SFpa0fNP8xZwdmRXS7tZ2!B9P4na&yHXT*JAsuGUmeYypacsvkxnkUDgC*GWmUBf5WKT8E2k!UF@pK3g86ansmrmOfspzeSGjJmjfmg9#*pOYs@b^cGZLycrtv@*wrxCgeVvb&4v3mZwRwDgdEPZMQX0lyANM23HL3oWm#dm2nS6A!OAGq$QjoA9*DK&*ffQ2&wZxJf06Vxck1om!ccKe7qpqZk5iJECurt!3s6EpTPdpgxeKAZLlR4Bt1V#5ZBfX4tW0^3I0zlJvomrNOiOKi!vK^8lX36JkuJCzhrvAKk#aC^RtQAtS$qK^A*30I^$Ed3KZ82rgwG7z4mswABIzOH*3bm!6C@dCx", 378);
    pm_makeFSM(pm2);
    pm_addstring(pm3, (unsigned char *)"lKEX*VA$uFqJ2FfJE5*EI!MU5vMg!KGq9xP$fAshHrIVXgQfL45&1tT&TOUcTcKUYGoAgTEIrl6NiL8n&DPIvmomUpBp2sHM#*g1V2&xsYFOOznYQwzoqnBp2w*P", 124);
    pm_makeFSM(pm3);
    pm_addstring(pm4, (unsigned char *)"dA1sI0NGxYzGPWQreWrptknA9SMf44Bfl9hXDAV!TvWSGB!1e66$2wG6tx&Re*LoEBwJ!dUL@qOJuJ2kffeazN1Mhcvuc4$J7Xjox$1fAwSW6ziIvlv7PstENidAXl7DnlR4W7*yCjz$XhPvmWLflmvohg!XxTI*c#Ra5QRL2HKmc&PCzjQM*wzjSbbxLqC*G$Qe^nFIM8nEzfV4keXikucg$Z1GCW04eBYTWp*HCwrHHGqtC*XmRsPx@iGMQg5yGkeXQMiiQJUyLnp1f3PC4evR@^zijZ$H&XkZ7XuTUTcVV8vkIUpzoTRjYw$C1arvd&Y!&rFY35*RFDj!bv&&yl*sN4W3ALkDGXudUKRlPFYy!D89kXQmwkyHjO*&RAv#eJ2vBN7#*NMN*eY0bD$xQ0Rn7ywTUcch^fiU@3s$$2oGH2zjVLKXhHFVeRb7X9al9T^vwldPKFJDgqJgVky!QgWQ5g#*^lsEMykUV6o#h2UPhoy3@rqscqeBJ!GJwEJWj$Li^bynEWhtTsojwCiaB29gU&thp0O5$PI1Lq9UzpLTc8ZZ1$OgJSN11xJzpjC1&mNh7T@wD8j&M&Rjk^TXZckBHT9fVmS1!F0GHslpaBTZZ#DD6p@!CkDk", 632);
    pm_makeFSM(pm4);
    pm_addstring(pm5, (unsigned char *)"gGoojjNNZnj09JmxraEvWyaO9raPqdABd0iZSqoMVyPtyIfpLolELRVtyAjYyIE4x64spFYW6jXCE5CIRMdxaO0YfzlvkFYHwM1gSUbOqxuXkIYg9WFuQ6MG!VFoyoJ1xk0Y96mSP7eUhOmZeU9rzItkRzRVfvlKSvmxYwX7yoOIPVcxhL46ufZnn199V225aeKi8qXrCoUxiGELsNpV", 212);
    pm_makeFSM(pm5);
    pm_addstring(pm6, (unsigned char *)"E4yE1Zj7vkE2mNzp8Ikimo2lIEAFFyKZEsz7N7Sue629yw8RJ3zvHxWleH9KJqt0t9GTfL75eZqayVQOzHagHwLdacJHjaZ2ul4zHCb1af7CPpsSp6wstt8xBhqAaSydUCJcoFyetcxEh0zM1MiMDKEyDHmq4OhvSMFbI3SbgGoZ8SiMvCcWX2b0YsQVgF65LoEEKIL71xye0pBNxD4bNoQgvFTcNxK33mGB37oNekERK5qUOvO9xpKlWIaTDtkOTTnXL6WjK0WKbONOCw8x1kCKwk12aVUgM27L3kLTIdo0s6VUVAAGeqYFNRYzyn2F7qJSpEiygYaNdLkiJdLcf0QguBhhIOfTKh4henpss2Z2wBStst1lZvrZL6R5OjwMEFsUEsUjS8z0nOoFEAl1TOY4P9elaAPQ7ornOFAUybAVOZEmlu2atvLJVvv1VCsIEmwnezgyULFG886hbVRO5Z9uxWti2G8FmcI09JQkuXZ3vAyyEUVmLSJhc1tKMVpv8dRKq61ROSfs6f1qhepGxoJZQTkMRMaSii71JW6Fv0XzSSrsYEylWgkRNS1JUkB3Qe8YeryJ9ubX5I5VCd0JoStAY704klkXDUE2bKNMFALcFNjVDUdjpeu7q1TslRLFbpSrdfionpmmFyIzopE4CAYkz2jlablPHLUeBMjGgiBgbYOulSIKSuwx7rlvFCbhyTsibHtZxEY6TQEOkTWSPMXqp", 729);
    pm_makeFSM(pm6);
    pm_addstring(pm7, (unsigned char *)"99SYZnuFsHE2GyHLrMpkHhgLSsHbu669yJ8Qd1GgeYQmHl7L1CQ9o7WK5lU395rk7HF9eSCqMlS92tiS8XeDXD0dvsp5qF0GB215Fngv4fTJCOJU8J2dkB8V7SYy7Oc3SCVPosh6ttX35bteoOZxJC8t2pQ3jNeeqkwPyi0hL7jcVbPO4PTs9Fp8wKzg15XkgWHI5LG9cz3Rvh9cESHNxicnYapAf3rMvQr6IfXmzbBuMJcq2OkAujQxmBNmEFtN5qB2o5zIOeU49BBCmWvrlWXuodMBKOJB7P8soAnLZinDaYg7Xm4LtwPe0q0rNX6BBCW7SwvKjmFvvwApukgaV4J5RhzwfQ3MfGmiW4Gp5LyGuelGZDRnO2v19pG86G4P1fvTpyWWagPrpMedLrBhs6fw05n0XjbkBVm6G7vrFTwutOGDIKLZwZUnSar7e6D2rLwJXT0LydOZzflGwxjKHg9yim7n2iAExTaj4fNBQWMt4uzwttPnFu", 502);
    pm_makeFSM(pm7);
    pm_addstring(pm8, (unsigned char *)"lC14n8wwW12TxLorJKoO5PLRqSuafVm5MFC2OEzhvY5XD07nHYwGdzfDQ8XmUzUmyKNPjBp62AxJAJ97V9WPYAKHJDmKZ4AvSH1n2Ho651F9WgqbXzj3Abdr3m5wtDY8cb3kS3lRYcmNepUlOSuHw2Gy3EtPFLHVLyF2Fjls0mK6N2kxOR023jQTWwpqdHUN6MnTLn61Mx2c9sDrwSmvxaPcsGXhA6pnnPczdJhts6XvdIXmKaQzYpaC3niN82tSjwtR0VSsOeAE3WHZvjTxAduYagif23clk23QIdoenFqpxnqBLhFD9ShEfSuot28Ta0hcaIyxKuMi9KjD1f8HRM7mZ7zwQ3KaeSLoi7ovgWcUXMFkkCQsZ6rFLZlGRgBEFz2rfwXOmU0rGA94DIejZ6zy8VtI6D2nY13fsyPPGTpx80OwcCQhDEoJ83bEd1zlzwK9F15RDupRAmpy6eMz7BRosGy3rqgtauypdqbQ7vxWJ62X3dALkAwWgUu1WQNPRQpax2cusZ12igIJxRqd2MIMJw93C2BAU4eXphcFimgGLWlS3mUAtDP8ZUHqGjkACC4AQ6Fy3CigMKpLWAn7wPytFDkTXmVb0EeO0Mc6SEOOSWXbXukBT37kPMJlDcFL0pQBDqsuUp5Tny6EU", 657);
    pm_makeFSM(pm8);
    pm_addstring(pm9, (unsigned char *)"mjXnO5lUQKGB1RnpYMXq72alSUcZLthkcvCYuwmeTfThqjVbe0PzN6hId9a6u9DGxL37pbPcyBycDYWZmhwPF6CdQzyNxlAhLyjU0FG9jYICzxsGt", 113);
    pm_makeFSM(pm9);
    pm_addstring(pm10, (unsigned char *)"kDR1XpjpFrgtED8y24YD76zgJz5hIAEfRJylpD7wO8oHE0xyjPUMTcqEabuZtvnUNgfZZjclcPlwOzJALTQQ1KzlGzfrGxKnbxRYiCw0IXgfdgAINS9gaNTVVT2AYIfxG5oiTqSFGNe8mx2inlzPHJSdZLx7Gh0Rmncf5MESgPxdkVKHxS0dGHEx9WIbKiimQMy5LHl2RwRTMWybSy8X1PYiXcAGw1x9HQ7QjavVkjBBLt8GnbeulhV0dN2wTO9gnoIjhulLzRltiHx57vafQM4fOalysOUqfhUppN494uD482cWkuNq08bAaNjaNq19gL9hI3AJNXzFl7KH1f6h3xd0qGuunOQv5N25U9X676qRm3hVlkgra8EVKRMW2vbCO9SE55py9k7Ehg6lrnPy0WayZfJly2ZlgwtFqVFR7dBXYklRZcru3RQdVDFOvObNJmJPx0A82LC1Q0bqpAzuSQQ2mOWKhg6bEF0q2tWSB3d0I9m8I97MoCKDWDfIHBrpHLW1GbDJJdZCePakRSn6ityD0reUreEv3Nk3oKGY3mhVRqvwq1K374D80HBudTsYNP6xa1yJtANEus3YnqI38AQ9eu46q1xfmYfY28V9mvOviAnj3G9nGRkdIzO6CkNIPWZ7T0TTlEmftZKe1MIXZYW2AnIXaSn5YCmzT6rYqIpT0lO4TGTX8Nu2Aj8GDbhAj0Zs0cnxS5b3FSylzp8MHuJsdyNxgMGTeNtGloWQwvfhHdCsj9rEbchclaBriYZtx7rzaxbIB", 777);
    pm_makeFSM(pm10);

    FILE *fp;
    char s1[PM_CHARACTERS];
    fp = fopen("/home/student/Final Project/Pattern-Matching/chars_stream.txt", "r");
    fgets(s1, PM_CHARACTERS, (FILE *)fp);
    fclose(fp);

    // char *s1 = "abcdjklmnopqrstuvwabcdefghijefghiklmnopqrstuvwxzabcdjklmnopqrstuefghivw";

    search_and_destroy(pm1, s1);
    pm_destroy(pm1);
    search_and_destroy(pm2, s1);
    pm_destroy(pm2);
    search_and_destroy(pm3, s1);
    pm_destroy(pm3);
    search_and_destroy(pm4, s1);
    pm_destroy(pm4);
    search_and_destroy(pm5, s1);
    pm_destroy(pm5);
    search_and_destroy(pm6, s1);
    pm_destroy(pm6);
    search_and_destroy(pm7, s1);
    pm_destroy(pm7);
    search_and_destroy(pm8, s1);
    pm_destroy(pm8);
    search_and_destroy(pm9, s1);
    pm_destroy(pm9);
    search_and_destroy(pm10, s1);
    pm_destroy(pm10);
}

void search_and_destroy(pm_t *pm, char *s)
{
    slist_t *list = pm_fsm_search(pm->zerostate, (unsigned char *)s, strlen(s));
    slist_destroy(list, SLIST_FREE_DATA);
    free(list);
    // pm_destroy(pm);
}

unsigned char *shift(unsigned char *str, int len)
{
    int i;
    unsigned char *temp = (unsigned char *)malloc(sizeof(unsigned char) * (len));
    for (i = 0; i < len - 1; i++)
    {
        temp[i] = str[i + 1];
    }
    temp[len - 1] = str[0];
    return temp;
}

void print_match(slist_t *list)
{
    slist_node_t *node = slist_head(list);
    while (node)
    {
        pm_match_t *match = (pm_match_t *)slist_data(node);
        printf("the \"%s\" start at %d, end at %d at %d state\n", match->pattern, match->start_pos, match->end_pos, match->fstate->id);
        node = slist_next(node);
    }
    slist_destroy(list, SLIST_FREE_DATA);
    free(list);
}

void print_pm(pm_t *pm)
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
    free(b);
}

void print_state(pm_state_t *state, int tabs, int *is_need, int is_get)
{
    slist_node_t *node = slist_head(state->_transitions);
    if (!node)
    {
        printf("\n");
        print_tabs(tabs, is_need);
        printf("\n");
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
    print_tabs(tabs, is_need);
    printf("\n");
}

void print_tabs(int tabs, int *is_need)
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