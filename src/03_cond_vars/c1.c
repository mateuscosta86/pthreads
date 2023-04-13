#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_t t1;
pthread_t t2;

#define N_THREADS           4

pthread_t depositers[N_THREADS];
pthread_t withdrawers[N_THREADS];

double balance = 10000.0;
pthread_mutex_t balance_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t has_balance = PTHREAD_COND_INITIALIZER;


void worker_code_1(void) {
    for( int i=0; i < 20; ++i) {
		sleep(1);
		printf("Worker 1 depositing R$20, i=%d\n",i);
		pthread_mutex_lock(&balance_mutex);
		
		balance = balance + 20;
		pthread_cond_signal(&has_balance);

		pthread_mutex_unlock(&balance_mutex);
		printf("Worker 1 deposited R$20, i=%d\n",i);	
	}
}

void worker_code_2(void) {
    for( int i=0; i < 20; ++i) {
		sleep(1);
		printf("Worker 2 withdrawing R$20, i=%d\n",i);
		pthread_mutex_lock(&balance_mutex);
		
		while (balance < 20.0)
			pthread_cond_wait(&has_balance, &balance_mutex);
		
		balance -= 20.0;
		pthread_mutex_unlock(&balance_mutex);
		printf("Worker 2 withdrew R$20, i=%d\n",i);	
	}
}


int main(void) {
    printf("Beginning\n");

	for (int i = 0; i < N_THREADS; ++i)
        pthread_create(&depositers[i], NULL, (void *)worker_code_1, NULL);
    
	for (int i = 0; i < N_THREADS; ++i)
        pthread_create(&withdrawers[i], NULL, (void *)worker_code_2, NULL);
    
	for (int i = 0; i < N_THREADS; ++i)
        pthread_join(depositers[i], NULL);

	for (int i = 0; i < N_THREADS; ++i)
        pthread_join(withdrawers[i], NULL);

    printf("Done: new balance is %.2f\n", balance);

    return(0);
}
