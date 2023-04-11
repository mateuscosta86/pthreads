#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define N_THREADS           10

pthread_t my_threads[N_THREADS];

void* worker_code_1(int* nst) {
    for (int ns=0; ns < *nst; ++ns) {
        sleep(1);
        printf("Worker 1: %d seconds have passed\n", ns + 1);
    }
}

int main(void) {
    int threads_params[N_THREADS];

    printf("Beginning\n");

    for (int i = 0; i < N_THREADS; ++i)
        threads_params[i] = 5 + i;

    for (int i = 0; i < N_THREADS; ++i)
        pthread_create(&my_threads[i], NULL, (void *)worker_code_1, (void *) &threads_params[i]);

    for (int i = 0; i < N_THREADS; ++i)
        pthread_join(my_threads[i], NULL);

    printf("Done!");

    return(0);
}
