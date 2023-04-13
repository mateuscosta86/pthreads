#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define	NTHREADS	5

pthread_t th_smelting[NTHREADS];
pthread_t th_polishment[NTHREADS];
pthread_t th_painting[NTHREADS];	

pthread_mutex_t mutex_smelted = PTHREAD_MUTEX_INITIALIZER;
int smelted_pieces = 0;

pthread_mutex_t mutex_polished = PTHREAD_MUTEX_INITIALIZER;
int polished_pieces = 0;

pthread_mutex_t mutex_painted = PTHREAD_MUTEX_INITIALIZER;
int painted_pieces = 0;

pthread_cond_t has_smelted_pieaces = PTHREAD_COND_INITIALIZER;
pthread_cond_t has_polished_pieces = PTHREAD_COND_INITIALIZER;

void smelting_phase_worker(void){
	while(1) {
		sleep(1);
		pthread_mutex_lock(&mutex_smelted);	
		++smelted_pieces;
		printf("Pieace has been smelted\n");
		pthread_cond_signal(&has_smelted_pieaces);
		pthread_mutex_unlock(&mutex_smelted);	
	}
}

void polishment_phase_worker(void){
	while(1) {
		pthread_mutex_lock(&mutex_smelted);

		while(smelted_pieces == 0)
			pthread_cond_wait( &has_smelted_pieaces, &mutex_smelted);

		--smelted_pieces;
		printf("Smelted pieace's been picked.\n");
		pthread_mutex_unlock(&mutex_smelted);	

		sleep(1);

		pthread_mutex_lock(&mutex_polished);	
		++polished_pieces;
		printf("Pieace's been polished.\n");
		pthread_cond_signal(&has_polished_pieces);
		pthread_mutex_unlock(&mutex_polished);	
	}
}

void painting_phase_worker(void){
	while(1) {
		pthread_mutex_lock(&mutex_polished);

		while(polished_pieces == 0)
			pthread_cond_wait( &has_polished_pieces, &mutex_polished);

		--polished_pieces;
		printf("Polished pieace's been picked.\n");
		pthread_mutex_unlock(&mutex_polished);	

		sleep(1);

		pthread_mutex_lock(&mutex_painted);	
		++painted_pieces;
		printf("Pieaces's been painte. Total pieces processed: %d\n", painted_pieces);

		if( painted_pieces == 50 )
			exit(1);	

		pthread_mutex_unlock(&mutex_painted);	
	}
}

int main(void) {
    printf("Beginning\n");
    
	for( int i=0; i<NTHREADS; ++i)
		pthread_create(&th_smelting[i], NULL, (void *) smelting_phase_worker, NULL);

	for( int i=0; i<NTHREADS; ++i)
		pthread_create(&th_polishment[i], NULL, (void *) polishment_phase_worker, NULL);

	for( int i=0; i<NTHREADS; ++i)
		pthread_create(&th_painting[i], NULL, (void *) painting_phase_worker, NULL);

	for( int i=0; i<NTHREADS; ++i)
		pthread_join(th_smelting[i], NULL);

	for( int i=0; i<NTHREADS; ++i)
		pthread_join(th_polishment[i], NULL);

	for( int i=0; i<NTHREADS; ++i)
		pthread_join(th_painting[i], NULL);

	printf("Done!\n");
    return(0);
}
