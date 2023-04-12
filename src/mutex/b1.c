#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_t t1;
pthread_t t2;

pthread_mutex_t mutex_saldo = PTHREAD_MUTEX_INITIALIZER;

double balance = 10000.0;

void worker_code_1(void) {
    double old_balance, new_balance;
    
    sleep(1);

    for (int n=0; n < 100; ++n) {
        pthread_mutex_lock(&mutex_saldo);
        old_balance = balance;
        new_balance = old_balance - 10;
        printf("Subtractor: Balance went from: %.2f to %.2f\n", old_balance, new_balance);

        balance = new_balance;
        pthread_mutex_unlock(&mutex_saldo);
    }
}

void worker_code_2(void) {
    double old_balance, new_balance;
    
    sleep(1);

    for (int n=0; n < 100; ++n) {
        pthread_mutex_lock(&mutex_saldo);
        old_balance = balance;
        new_balance = old_balance + 10;
        printf("Adder: Balance went from: %.2f to %.2f\n", old_balance, new_balance);

        balance = new_balance;
        pthread_mutex_unlock(&mutex_saldo);
    }
}

int main(void) {
    printf("Beginning\n");

    pthread_create(&t1, NULL, (void *)worker_code_1, NULL);
    pthread_create(&t2, NULL, (void *)worker_code_2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Done: new balance is %.2f\n", balance);

    return(0);
}
