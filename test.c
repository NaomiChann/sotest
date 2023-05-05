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
	int total;;
} animal_t;

typedef struct
{
	int type;
	int id;
} message_t;

// GLOBALS
int timer_g = 0, timeStarting_g = 0, timeCurrent_g = 0, timeNew_g = 0;

pthread_mutex_t mutControl_g = PTHREAD_MUTEX_INITIALIZER;

animal_t lion_g[4], meerkat_g[10], ostrich_g[7];

// PROTOTYPES
void Hello();
void* Routine( void* received );
void Setter( animal_t *animal );
void Initializer();
void ErrorCheck( int error );
void TimeUpdater();
void Joiner();
void Act( message_t animal );

int main()
{
	Hello();
	Initializer();
	Joiner();

	

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
	message_t message = *( message_t* ) received;

	while ( timer_g < HOURS_DAY )
	{
		timeNew_g = time( NULL );
		pthread_mutex_lock( &mutControl_g );
		if ( timeNew_g != timeCurrent_g )
		{
			TimeUpdater();

		} else {
			Act( message );
		}
		pthread_mutex_unlock( &mutControl_g );
	}

	pthread_exit( NULL );
}

void Setter( animal_t *animal )
{
	animal->isSleeping = false;
	animal->sleepTimer = 4;
	animal->total = 0;
}

void Initializer()
{
	int errorCheck;

	timeStarting_g = time( NULL );
	timeCurrent_g = time( NULL );

	pthread_mutex_init( &mutControl_g, NULL );

	for( int i = 0; i < POP_LIONS; ++i )
	{
		message_t message = { .id = i, .type = LION };
		Setter( &lion_g[i] );
		errorCheck = pthread_create( &lion_g[i].aThread, NULL, Routine, ( void* ) &message );
		ErrorCheck( errorCheck );
	}

	for( int i = 0; i < POP_MEERKATS; ++i )
	{
		message_t message = { .id = i, .type = MEERKAT };
		Setter( &meerkat_g[i] );
		errorCheck = pthread_create( &meerkat_g[i].aThread, NULL, Routine, ( void* ) &message );
		ErrorCheck( errorCheck );
	}

	for( int i = 0; i < POP_OSTRICHES; ++i )
	{
		message_t message = { .id = i, .type = OSTRICH };
		Setter( &ostrich_g[i] );
		errorCheck = pthread_create( &ostrich_g[i].aThread, NULL, Routine, ( void* ) &message );
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

void TimeUpdater()
{
	timeCurrent_g = timeNew_g;
	timer_g = timeCurrent_g - timeStarting_g;
	printf( "Hour %d\n\n", timer_g );
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

void Act( message_t animal )
{
	switch ( animal.type )
	{
		case LION:
			printf( "EAT MEAT \n" );
			printf( "Lion [%d]: RAWR :3 \n", animal.id );
			printf( "EAT FLESH \n" );
			break;
		
		case MEERKAT:
			printf( "EAT LARVAE \n" );
			printf( "Meerkat [%d]: EEEEE >:( \n", animal.id );
			printf( "EAT INSECT \n" );
			break;
		
		case OSTRICH:
			printf( "EAT WEED \n" );
			printf( "Ostrich [%d]: IWIWIWIWI >//< \n", animal.id );
			printf( "EAT FLOWER \n" );
			break;
		
		default:
			break;
	}
}