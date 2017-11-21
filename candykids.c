/*
 * candykids.c
 *
 *  Created on: 
 *      Author: Ali Sharifi
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdint.h>
#include "bbuff.h"
#include "stats.h"

_Bool stop_thread = false;

typedef struct
{
	int factory_number;
	double time_stamp_in_ms;
} candy_t;

// gets current number of milliseconds
double current_time_in_ms(void)
{
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	return now.tv_sec * 1000.0 + now.tv_nsec/1000000.0;
}

void* candyfactoryThread(void *arg)
{
	int factoryNo = *((int*)arg);
	while(!stop_thread)
	{
		int randomWaitTime = (rand() % 4);
	    printf("\tFactory %d ships candy & waits %ds\n", factoryNo, randomWaitTime);
		candy_t* candy = malloc(sizeof(candy_t));
		candy->factory_number = factoryNo;
		candy->time_stamp_in_ms = current_time_in_ms();

		bbuff_blocking_insert(candy);
		stats_record_produced(factoryNo);
		//free(candy); must be freed by consumer!
		sleep(randomWaitTime);

	}
	printf("Candy-factory %i done.\n", factoryNo);
	pthread_exit(NULL);
}

void* kidThread(void *arg)
{
	int kidNo=*((int*)arg);
	while(1)
	{
		int randomWaitTime=(rand() % 2);
		candy_t* candy = (candy_t*)bbuff_blocking_extract();
		printf("\tKid %d eats candy & waits %ds\n",kidNo,randomWaitTime);
		stats_record_consumed(candy->factory_number,  current_time_in_ms() - candy->time_stamp_in_ms);
		free(candy);
		sleep(randomWaitTime);
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

	if (argc < 4)
	{
		printf("Please enter 3 positive numbers, # Factories, # Kids, # Seconds\n");
		exit(1);
	}

	for(int x = 1; x < argc; x++)
	{
		int arguments = atoi(argv[x]);
		if (arguments <= 0)
		{
			printf("Please enter a positive number larger than 0\n");
			exit(1);
		}
	}

	bbuff_init();
	int factoryNo = atoi(argv[1]);
	int kidNo = atoi(argv[2]);
	int waitingTime = atoi(argv[3]);
	stats_init(factoryNo);

	pthread_t kid_threads[kidNo];
	for(int i = 0; i < kidNo; i++)
	{
		pthread_create(&kid_threads[i], NULL, kidThread, &i);
	}

	pthread_t factory_threads[factoryNo];
	int list[factoryNo];
	for(int j = 0; j < factoryNo; j++)
	{
		list[j] = j;
		pthread_create(&factory_threads[j], NULL, candyfactoryThread, &list[j]);
	}

	for(int i =0; i < waitingTime; i++)
	{
		sleep(1);
		printf("Time %ds\n", i);
	}

	printf("Stopping factories....\n");
	stop_thread = true;

	for(int j = 0; j < factoryNo; j++)
	{
		pthread_join(factory_threads[j], NULL);
	}

	while(bbuff_is_empty()==false)
	{
		printf("Waiting for kids to finish candies....\n");
		sleep(1);
	}

	for(int i = 0; i < kidNo; i++)
	{
		pthread_cancel(kid_threads[i]);
		pthread_join(kid_threads[i], NULL);
	}

	printf("Stopping kids....\n");

	stats_display();
	stats_cleanup();
}
