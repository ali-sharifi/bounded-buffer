/*
 * stats.c
 *
 *  Created on: 
 *      Author: Ali Sharifi
 */

#include "stats.h"

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

sem_t mutex;
int lastFactNo;
typedef struct
{
	int factoryNo;
	int producedNo;
	int consumedNo;
	double min_Delay;
	double avg_Delay;
	double max_Delay;
	double total_Delay;
} stats;

stats* stats_list;

void stats_init(int num_producers)
{
	lastFactNo = num_producers;
	stats_list = malloc(num_producers*sizeof(stats));
	for (int i = 0; i < num_producers; i++)
	{
		stats_list[i].factoryNo = i;
		stats_list[i].producedNo = 0;
		stats_list[i].consumedNo = 0;
		stats_list[i].min_Delay = -1;
		stats_list[i].avg_Delay = -1;
		stats_list[i].max_Delay = -1;
		stats_list[i].total_Delay = 0;
	}
	sem_init(&mutex,0,1);
}

void stats_record_produced(int factory_number)
{
	sem_wait(&mutex);
	stats_list[factory_number].producedNo++;
	sem_post(&mutex);
}


void stats_record_consumed(int factory_number, double delay_in_ms)
{
	sem_wait(&mutex);
	stats_list[factory_number].consumedNo++;
	if(stats_list[factory_number].min_Delay == -1) {
		stats_list[factory_number].min_Delay = delay_in_ms;
		stats_list[factory_number].avg_Delay = delay_in_ms;
		stats_list[factory_number].max_Delay = delay_in_ms;
		stats_list[factory_number].total_Delay = delay_in_ms;
	}
	else {
		if(stats_list[factory_number].min_Delay > delay_in_ms) {
			stats_list[factory_number].min_Delay = delay_in_ms;
		}
		if(stats_list[factory_number].max_Delay < delay_in_ms) {
			stats_list[factory_number].max_Delay = delay_in_ms;
		}
		stats_list[factory_number].total_Delay += delay_in_ms;
		stats_list[factory_number].avg_Delay =( stats_list[factory_number].total_Delay/(double)stats_list[factory_number].consumedNo);
	}
	sem_post(&mutex);
}

void stats_display()
{
	printf("\n");
	printf("Statistics:\n");
	printf("-------------------------------------------------------------------------------\n");
	printf("%10s%10s%10s%16s%16s%16s\n", "Factory#", "#Made", "#Eaten", "Min_Delay[ms]", "Avg_Delay[ms]", "Max_Delay[ms]");
	for(int i = 0; i < lastFactNo; i++)
	{
		printf("%8d%10d%10d%16f%16f%16f\n", stats_list[i].factoryNo,stats_list[i].producedNo,stats_list[i].consumedNo,stats_list[i].min_Delay,stats_list[i].avg_Delay,stats_list[i].max_Delay);
		if (stats_list[i].producedNo != stats_list[i].consumedNo)
		{
			printf("Not all candies are eaten");
			exit(1);
		}
	}
}

void stats_cleanup()
{
	free(stats_list);
	stats_list = NULL;
}


