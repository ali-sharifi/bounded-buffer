/*
 * bbuff.c
 *
 *  Created on: 
 *      Author: Ali Sharifi
 */

#include "bbuff.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t mutex;
sem_t full_buffer;
sem_t empty_buffer;
int counter;
void* buffer[BUFFER_SIZE];

void bbuff_init(void)
{
	sem_init(&full_buffer, 0, 0);
	sem_init(&empty_buffer, 0, BUFFER_SIZE);
	sem_init(&mutex,0,1);

	counter = 0;
}

void bbuff_blocking_insert(void* item)
{
	sem_wait(&empty_buffer);
	sem_wait(&mutex);

	buffer[counter] = item;
	counter++;

	sem_post(&mutex);
	sem_post(&full_buffer);

}

void* bbuff_blocking_extract(void)
{
	sem_wait(&full_buffer);
	sem_wait(&mutex);

	void* item = buffer[counter-1];
	counter--;

	sem_post(&mutex);
	sem_post(&empty_buffer);

	return item;
}

_Bool bbuff_is_empty(void)
{
	if (counter == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
