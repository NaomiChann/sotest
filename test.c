
#include <stdio.h>
#include <unistd.h> //Para utilizar sleep
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

void errorCheck( int ea );
void* Vet( void* rec );

int timey;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_cond  = PTHREAD_COND_INITIALIZER;

int main ( void )
{
	pthread_t lion[4];
	char mesa[4] = { '0', '1', '2', '3' };
	int ea;
	int starting = time( NULL ), current = time( NULL ), newt;

	pthread_mutex_init( &mutex, NULL );
	pthread_mutex_init( &condition_mutex, NULL );
	while ( timey < 2000 ) {
		newt = time( NULL );
		if ( newt != current ) {
			current = newt;
			timey = current - starting;
			printf( "Hours %d\n", timey );
		}
		for( int i = 0; i < 4; ++i ) {
			ea = pthread_create( &lion[i], NULL, Vet, (void *) &mesa[i] );
			errorCheck( ea );
		}

		for( int i = 0; i < 4; ++i ) {
			pthread_join( lion[i], NULL );
		}
	}

	return 0;
}

void* Vet( void* rec ){
	char* mess = ( char* ) rec;
/*
	pthread_mutex_lock( &condition_mutex );
	if( counter < 2000 ) {
		if ( counter % ( atoi( &mess[0] ) * 10 ) == 0 ) {
			pthread_cond_wait( &condition_cond, &condition_mutex );
		} else {
			pthread_cond_signal( &condition_cond );
		}
	}
	pthread_mutex_unlock( &condition_mutex );
	*/
	if ( timey % 3 == 0 ) {
		pthread_mutex_lock( &mutex );
		printf( "thread [%c]: babybaby \n", mess[0] );
		pthread_mutex_unlock( &mutex );
	}

	pthread_exit( NULL );
}

void errorCheck( int ea )
{
	if ( ea ){
		printf("ERRO; pthread_create() dos animais devolveu o erro %d\n", ea);
		exit( -1 );
	}
}