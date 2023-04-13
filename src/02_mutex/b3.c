#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_t t1;
pthread_t t2;

pthread_mutex_t balance_mutex = PTHREAD_MUTEX_INITIALIZER;

double balance = 10000.0;

double calculate_interest(double x) {
	double interest;
	pthread_mutex_lock(&balance_mutex);
	interest = balance * (x / 100.0);
	pthread_mutex_unlock(&balance_mutex);
	return interest;
}

void deposit_interest(double y) {
	double interest;
	pthread_mutex_lock(&balance_mutex);
	interest = calculate_interest(y);
	balance = balance + interest;
	pthread_mutex_unlock(&balance_mutex);
}

void worker_code_1(void) {
    for( int i=0; i < 20; ++i) {
		printf("Worker 1 depositing R$20, i=%d\n",i);
		pthread_mutex_lock(&balance_mutex);
		balance = balance + 20;
		pthread_mutex_unlock(&balance_mutex);

	}
}

void worker_code_2(void) {
    for( int i=0; i < 20; ++i) {
		printf("Worker 2: depositing 5%% interest\n");
		deposit_interest(5);
	}
}


int main(void) {
    printf("Beginning\n");

	pthread_mutexattr_t mat;		
	pthread_mutexattr_init(&mat);	
	pthread_mutexattr_settype(&mat, PTHREAD_MUTEX_RECURSIVE_NP);	
	pthread_mutex_init(&balance_mutex, &mat);

    pthread_create(&t1, NULL, (void *)worker_code_1, NULL);
    pthread_create(&t2, NULL, (void *)worker_code_2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Done: new balance is %.2f\n", balance);

    return(0);
}
