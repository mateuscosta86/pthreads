#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

struct param_t {
    int nst;
    char *name;
};

pthread_t t1;
pthread_t t2;

void worker_code_1(struct param_t *params) {
    for (int ns=0; ns < params->nst; ++ns) {
        sleep(1);
        printf("%s: %d seconds have passed\n", params->name, ns + 1);
    }
}

void worker_code_2(int *nst) {
    for (int ns=0; ns < *nst; ++ns) {
        sleep(1);
        printf("Worker 2: %d seconds have passed\n", ns + 1);
    }
}

int main(void) {
    struct param_t params = {
        .name = "Worker 1",
        .nst = 10
    };
    
    int param2 = 15;

    printf("Beginning\n");

    pthread_create(&t1, NULL, (void *)worker_code_1, (void *) &params);
    pthread_create(&t2, NULL, (void *)worker_code_2, (void *) &param2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Done!\n");

    return(0);
}
