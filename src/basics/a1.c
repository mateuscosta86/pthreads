#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_t t1;
pthread_t t2;

void worker_code_1(void) {
    for (int ns=0; ns < 10; ++ns) {
        sleep(1);
        printf("Worker 1: %d seconds have passed\n", ns + 1);
    }
}

void worker_code_2(void) {
    for (int ns=0; ns < 15; ++ns) {
        sleep(1);
        printf("Worker 1: %d seconds have passed\n", ns + 1);
    }
}

int main(void) {
    printf("Beginning\n");

    pthread_create(&t1, NULL, (void *)worker_code_1, NULL);
    pthread_create(&t2, NULL, (void *)worker_code_2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Done!\n");

    return(0);
}
