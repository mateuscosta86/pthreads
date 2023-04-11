#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define N_THREADS           5

pthread_t my_threads[N_THREADS];

int TH_ERROR = 111;
int TH_SUCCESS = 222;

void* worker_code_1(char* first_letter) {
    for (int ns=0; ns < 10; ++ns) {
        sleep(1);
        printf("%s: %d seconds have passed\n", first_letter, ns + 1);
    }

    if (*first_letter >= 97 && *first_letter <= 122) {
        pthread_exit(&TH_ERROR);
    } else {
        pthread_exit(&TH_SUCCESS);
    }
}

int main(void) {
    char* threads_params[N_THREADS];

    int* threads_responses[N_THREADS];

    printf("Beginning\n");

    for (int i = 0; i < N_THREADS; ++i) {
        if (i % 2 == 0) {
            threads_params[i] = "Worker";
        } else {
            threads_params[i] = "worker";
        }

    }

    for (int i = 0; i < N_THREADS; ++i) {
        printf("%s", threads_params[i]);

    }
        

    for (int i = 0; i < N_THREADS; ++i)
        pthread_create(&my_threads[i], NULL, (void *)worker_code_1, (void *) threads_params[i]);

    for (int i = 0; i < N_THREADS; ++i)
        pthread_join(my_threads[i], (void **) &threads_responses[i]);

    for (int i = 0; i < N_THREADS; ++i)
        printf("Response from thread #%d with name: %s => %d\n", i, threads_params[i], *threads_responses[i]);

    printf("Done!");

    return(0);
}
