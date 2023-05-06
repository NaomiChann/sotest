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
#define POP_VETS			2
#define AMOUNT_FEEDERS		3
#define MAX_STORAGE			30
#define MAX_FOOD			10
#define SIZE_ALL			( POP_LIONS + POP_MEERKATS + POP_OSTRICHES + POP_VETS + 1 )

enum aType_t{
	LION,
	MEERKAT,
	OSTRICH,
	VET,
	SUPPLIER
};

typedef struct
{
	pthread_t aThread;
	int actions;
	int sleepTimer;
	int total;
} animal_t;

typedef struct
{
	int type;
	int id;
} message_t;

// GLOBALS
int timer_g = 0, timeStarting_g = 0, timeCurrent_g = 0, timeNew_g = 0;
int storage_g[AMOUNT_FEEDERS], feeder_g[AMOUNT_FEEDERS];

pthread_mutex_t mutControl_g = PTHREAD_MUTEX_INITIALIZER;

animal_t lion_g[POP_LIONS], meerkat_g[POP_MEERKATS], ostrich_g[POP_OSTRICHES];
pthread_t vet_g[POP_VETS], supplier_g;

// PROTOTYPES
void Hello();
void* Routine( void* received );
void Setter( animal_t *animal );
void Initializer( message_t* message );
void ErrorCheck( int error );
void TimeUpdater();
void Joiner();
void Act( message_t animal );
void Eat( int* food, animal_t* animal, message_t message );
void Counter();

int main()
{
	message_t* messages = ( message_t* ) malloc( sizeof( message_t ) * SIZE_ALL );
	Hello();
	Initializer( messages );
	Joiner();
	Counter();

	free( messages );

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
	bool restock = false;

	while ( timer_g <= HOURS_DAY )
	{
		timeNew_g = time( NULL );

		pthread_mutex_lock( &mutControl_g );
		if ( timeNew_g != timeCurrent_g )
		{
			TimeUpdater();

		}

		if ( message.type == SUPPLIER && restock )
		{
			storage_g[LION] = MAX_STORAGE;
			storage_g[MEERKAT] = MAX_STORAGE;
			storage_g[OSTRICH] = MAX_STORAGE;
			restock = false;
			printf( "Storage restocked \n\n" );
		}

		switch ( message.type )
		{
			case LION:
				if ( lion_g[message.id].sleepTimer > 0 )
				{
					--lion_g[message.id].sleepTimer;
				} else if ( lion_g[message.id].actions > 0 ) {
					Act( message );
					--lion_g[message.id].actions;
				}
				break;
			
			case MEERKAT:
				if ( meerkat_g[message.id].sleepTimer > 0 )
				{
					--meerkat_g[message.id].sleepTimer;
				} else if ( lion_g[message.id].actions > 0 ) {
					Act( message );
					--meerkat_g[message.id].actions;
				}
				break;

			case OSTRICH:
				if ( ostrich_g[message.id].sleepTimer > 0 )
				{
					--ostrich_g[message.id].sleepTimer;
				} else if ( lion_g[message.id].actions > 0 ) {
					Act( message );
					--ostrich_g[message.id].actions;
				}
				break;

			case VET:
				for ( int i = 0; i < AMOUNT_FEEDERS; ++i )
				{
					if ( feeder_g[i] < MAX_FOOD && storage_g[i] > 0 )
					{
						--storage_g[i];
						++feeder_g[i];
						
						printf( "Vet [%d] gave food to ", message.id );

						switch ( i )
						{
							case LION:
								printf( "the lions " );
								break;

							case MEERKAT:
								printf( "the meerkats " );
								break;

							case OSTRICH:
								printf( "the ostriches " );
								break;
							
							default:
								break;
						}
						
						printf( "( currently at %d units ) \n\n", feeder_g[i] );
					}

					if ( storage_g[i] == 0 && !restock )
					{
						printf( "Vet [%d] asked for restocking", message.id );
						restock = true;
					}
				}
				break;
		}
		
		pthread_mutex_unlock( &mutControl_g );
	}

	pthread_exit( NULL );
}

void Setter( animal_t *animal )
{
	animal->actions = 2;
	animal->sleepTimer = 0;
	animal->total = 0;
}

void Initializer( message_t* message )
{
	int errorCheck, tracker = 0;

	timeStarting_g = time( NULL );
	timeCurrent_g = time( NULL );

	for( int i = 0; i < AMOUNT_FEEDERS; ++i )
	{
		storage_g[i] = 30;
	}

	pthread_mutex_init( &mutControl_g, NULL );

	for( int i = 0; i < POP_LIONS; ++i )
	{
		message[tracker].id = i;
		message[tracker].type = LION;
		Setter( &lion_g[i] );
		errorCheck = pthread_create( &lion_g[i].aThread, NULL, Routine, ( void* ) &message[tracker] );
		ErrorCheck( errorCheck );
		++tracker;
	}

	for( int i = 0; i < POP_MEERKATS; ++i )
	{
		message[tracker].id = i;
		message[tracker].type = MEERKAT;
		Setter( &meerkat_g[i] );
		errorCheck = pthread_create( &meerkat_g[i].aThread, NULL, Routine, ( void* ) &message[tracker] );
		ErrorCheck( errorCheck );
		++tracker;
	}

	for( int i = 0; i < POP_OSTRICHES; ++i )
	{
		message[tracker].id = i;
		message[tracker].type = OSTRICH;
		Setter( &ostrich_g[i] );
		errorCheck = pthread_create( &ostrich_g[i].aThread, NULL, Routine, ( void* ) &message[tracker] );
		ErrorCheck( errorCheck );
		++tracker;
	}

	for( int i = 0; i < POP_VETS; ++i )
	{
		message[tracker].id = i;
		message[tracker].type = VET;
		errorCheck = pthread_create( &vet_g[i], NULL, Routine, ( void* ) &message[tracker] );
		ErrorCheck( errorCheck );
		++tracker;
	}

	message[tracker].id = 0;
	message[tracker].type = SUPPLIER;
	errorCheck = pthread_create( &supplier_g, NULL, Routine, ( void* ) &message[tracker] );
	ErrorCheck( errorCheck );
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

	for ( int i = 0; i < POP_VETS; ++i )
	{
		pthread_join( vet_g[i], NULL );
	}

	pthread_join( supplier_g, NULL );
}

void Act( message_t animal )
{
	int food = 0;
	switch ( animal.type )
	{
		case LION:
			food = ( rand() % 4 ) + 2;
			Eat( &food, &lion_g[animal.id], animal );
			printf( "Lion [%d]: EAT MEAT ( %d units ) \n", animal.id , food );

			printf( "Lion [%d]: RAWR :3 \n", animal.id );

			food = ( rand() % 4 ) + 2;
			Eat( &food, &lion_g[animal.id], animal );
			printf( "Lion [%d]: EAT FLESH ( %d units ) \n", animal.id , food );

			lion_g[animal.id].sleepTimer = ( rand() % 5 ) + 8;
			printf( "Lion [%d]: sleepy time uwu ( %d hours ) \n\n", animal.id, lion_g[animal.id].sleepTimer );
			break;
		
		case MEERKAT:
			food = ( rand() % 2 ) + 1;
			Eat( &food, &meerkat_g[animal.id], animal );
			printf( "Meerkat [%d]: EAT LARVAE ( %d units ) \n", animal.id , food );

			printf( "Meerkat [%d]: EEEEE >:( \n", animal.id );

			food = ( rand() % 2 ) + 1;
			Eat( &food, &meerkat_g[animal.id], animal );
			printf( "Meerkat [%d]: EAT INSECT ( %d units ) \n", animal.id , food );

			meerkat_g[animal.id].sleepTimer = ( rand() % 5 ) + 6;
			printf( "Meerkat [%d]: sleepy time uwu ( %d hours ) \n\n", animal.id, meerkat_g[animal.id].sleepTimer );
			break;
		
		case OSTRICH:
			food = ( rand() % 3 ) + 2;
			Eat( &food, &ostrich_g[animal.id], animal );
			printf( "Ostrich [%d]: EAT WEED ( %d units ) \n", animal.id , food );

			printf( "Ostrich [%d]: IWIWIWIWI >//< \n", animal.id );

			food = ( rand() % 3 ) + 2;
			Eat( &food, &ostrich_g[animal.id], animal );
			printf( "Ostrich [%d]: EAT FLOWER ( %d units ) \n", animal.id , food );

			ostrich_g[animal.id].sleepTimer = ( rand() % 5 ) + 4;
			printf( "Ostrich [%d]: sleepy time uwu ( %d hours ) \n\n", animal.id, ostrich_g[animal.id].sleepTimer );
			break;
		
		default:
			break;
	}
}

void Eat( int* food, animal_t* animal, message_t message )
{
	if ( *food > storage_g[message.type] ) {
		printf( "i wanted more :( ( %d/%d units ) \n", storage_g[message.type], *food );
		*food = storage_g[message.type];
		animal->total += *food;
		storage_g[message.type] = 0;
	} else {
		animal->total += *food;
		storage_g[message.type] -= *food;
	}
}

void Counter()
{
	for ( int i = 0; i < POP_LIONS; ++i )
	{
		printf( "Lion [%d]: consumed %d units \n", i, lion_g[i].total );
	}

	for ( int i = 0; i < POP_MEERKATS; ++i )
	{
		printf( "Meerkat [%d]: consumed %d units \n", i, meerkat_g[i].total );
	}

	for ( int i = 0; i < POP_OSTRICHES; ++i )
	{
		printf( "Ostrich [%d]: consumed %d units \n", i, ostrich_g[i].total );
	}
}