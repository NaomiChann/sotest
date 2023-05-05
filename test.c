#include <stdio.h>
#include <unistd.h> //Para utilizar sleep
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define HOURS_DAY			24
/*
#define HOURS_WEEK			168
#define HOURS_MONTH			720
#define HOURS_SEMESTER		4320
*/
#define POP_LIONS			4
#define POP_MEERKATS		10
#define POP_OSTRICHES		7
#define AMOUNT_FEEDERS		3

enum aType_t{
	LION,
	MEERKAT,
	OSTRICH
};

typedef struct
{
	pthread_t aThread;
	bool isSleeping;
	int sleepTimer;
	int total;
} animal_t;

// GLOBALS
int timer_g = 0, timeStarting_g = 0, timeCurrent_g = 0, timeNew_g = 0;

pthread_mutex_t mutStorage_g = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutFeeder_g[AMOUNT_FEEDERS] = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutClock_g = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condClock_g  = PTHREAD_COND_INITIALIZER;

animal_t lion_g[4], meerkat_g[10], ostrich_g[7];

// PROTOTYPES
void Hello();
void* Routine( void* received );
void Initializer();
void ErrorCheck( int error );
void Joiner();

int main()
{
	Hello();
	Initializer();

	while ( timer_g < HOURS_DAY ) // loops until simulation time is due
	{
		timeNew_g = time( NULL );

		pthread_mutex_lock( &mutClock_g );
		if ( timeNew_g != timeCurrent_g )
		{
			pthread_cond_signal( &condClock_g );
			timeCurrent_g = timeNew_g;
			timer_g = timeCurrent_g - timeStarting_g;
			printf( "Hour %d\n\n", timer_g );
		}
		pthread_mutex_unlock( &mutClock_g );

		Joiner();
	}

	return 0;
}

void Hello()
{
    printf( "\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n" );
    printf( "                  HELLO                     \n" );
    printf( " ٩(｡•́‿•̀｡)۶  WELCOME TO THE ZOO  ٩(｡•́‿•̀｡)۶ " );
    printf( "\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n\n" );
}


void* Routine( void* received )
{
	int* message = ( int* ) received;

	pthread_mutex_lock( &mutClock_g );
	while( timeNew_g == timeCurrent_g )
	{
		pthread_cond_wait( &condClock_g, &mutClock_g );
	}
	pthread_mutex_unlock( &mutClock_g );

	if ( timer_g % 3 == 1 )
	{
		printf( "thread [%d]: babybaby \n", *message );
	}

	pthread_exit( NULL );
}

void Initializer()
{
	int errorCheck;

	timeStarting_g = time( NULL );
	timeCurrent_g = time( NULL );

	pthread_mutex_init( &mutStorage_g, NULL );
	pthread_mutex_init( &mutClock_g, NULL );
	for ( int i = 0; i < AMOUNT_FEEDERS; ++i )
	{
		pthread_mutex_init( &mutFeeder_g[i], NULL );
	}

	for( int i = 0; i < POP_LIONS; ++i )
	{
		errorCheck = pthread_create( &lion_g[i].aThread, NULL, Routine, ( void* ) LION );
		ErrorCheck( errorCheck );
	}

	for( int i = 0; i < POP_MEERKATS; ++i )
	{
		errorCheck = pthread_create( &meerkat_g[i].aThread, NULL, Routine, ( void* ) LION );
		ErrorCheck( errorCheck );
	}

	for( int i = 0; i < POP_OSTRICHES; ++i )
	{
		errorCheck = pthread_create( &ostrich_g[i].aThread, NULL, Routine, ( void* ) LION );
		ErrorCheck( errorCheck );
	}
}

void ErrorCheck( int error )
{
	if ( error )
	{
		fputs( "\n=!=!=!=!=!=!= \n\npthread_create() RETURNED AN ERROR \n\n=!=!=!=!=!=!= \n", stderr );
		printf( "ERROR( %d ) \n", error );
		exit( -1 );
	}
}

void Joiner()
{
	for ( int i = 0; i < POP_LIONS; ++i )
	{
		pthread_join( lion_g[i].aThread, NULL );
	}

	for ( int i = 0; i < POP_MEERKATS; ++i )
	{
		pthread_join( meerkat_g[i].aThread, NULL );
	}

	for ( int i = 0; i < POP_OSTRICHES; ++i )
	{
		pthread_join( ostrich_g[i].aThread, NULL );
	}
}
