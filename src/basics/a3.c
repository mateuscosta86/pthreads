#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

struct param_t {
    int nst;
    char *name;
};

pthread_t t1;
pthread_t t2;

int ERROR_TH = 111;
int SUCCESS_TH = 222;

void* worker_code_1(void) {
    for (int ns=0; ns < 15; ++ns) {
        sleep(1);
        printf("Worker 1: %d seconds have passed\n", ns + 1);
    }

    return((void *)&SUCCESS_TH);
}

void worker_code_2(void) {
    for (int ns=0; ns < 10; ++ns) {
        sleep(1);
        printf("Worker 2: %d seconds have passed\n", ns + 1);
    }

    pthread_exit(&ERROR_TH);
}

int main(void) {
    int* response1;
    int* response2;

    printf("Beginning\n");

    pthread_create(&t1, NULL, (void *)worker_code_1, NULL);
    pthread_create(&t2, NULL, (void *)worker_code_2, NULL);

    pthread_join(t1, (void **)&response1);
    pthread_join(t2, (void **)&response2);

    printf("Done: t1 returned %d, t2 returned %d\n", *response1, *response2);

    return(0);
}
