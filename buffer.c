// buffer.c
// Drew Bies
// username: abies2

// description: this program demonstrates the producer-consumer problem.
// usage: <sleep time> <producer threads> <consumer threads>
// main thread creates producer and consumer threads based on user input and 
// sleeps for a desired time (also user input) before terminating the producer
// and consumer threads. a mutex and two semaphores are used to prevent multiple
// threads accessing the sensitive data which is the buffer array.


#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define SLEEP 5

buffer_item buffer[BUFFER_SIZE];

//define semaphores and mutex
pthread_mutex_t buffer_mutex;

sem_t empty;
sem_t occupied;


int running = 1;
int insertPointer = 0, removePointer = 0;

void *producer(void *param);
void *consumer(void *param);

// insert buffer_item item into buffer
int insert_item(buffer_item item)
{
    /* Acquire Empty Semaphore */
	sem_wait(&empty);
	
	/* Acquire mutex lock to protect buffer */
	pthread_mutex_lock(&buffer_mutex);
	
	buffer[insertPointer] = item;
	if(++insertPointer >= BUFFER_SIZE)
		insertPointer = 0;

	/* Release mutex lock and full semaphore */	
	pthread_mutex_unlock(&buffer_mutex);
	sem_post(&occupied);

	return 0;
}

// remove item from buffer and store in *item
int remove_item(buffer_item *item)
{
	/* Acquire Full Semaphore */
	sem_wait(&occupied);

	/* Acquire mutex lock to protect buffer */
	pthread_mutex_lock(&buffer_mutex);
	
	*item = buffer[removePointer];
	if(++removePointer >= BUFFER_SIZE)
		removePointer = 0;

	/* Release mutex lock and empty semaphore */
	pthread_mutex_unlock(&buffer_mutex);
	sem_post(&empty);

	return 0;
}

// main
int main(int argc, char *argv[])
{
	int sleepTime, producerThreads, consumerThreads;
	int i, j;

	if(argc != 4)
	{
		fprintf(stderr, "Usage: <sleep time> <producer threads> <consumer threads>\n");
		return -1;
	}

	/*call atoi to get arguments */
	sleepTime = atoi(argv[1]);
	producerThreads = atoi(argv[2]);
	consumerThreads = atoi(argv[3]);
 

	/* Create the producer and consumer threads */
	pthread_t producers[producerThreads];
	pthread_t consumers[consumerThreads];

	// init mutex
	if(pthread_mutex_init(&buffer_mutex, 0) != 0)
	{
		fprintf(stderr, "mutex initialization failed\n");
		return -1;
	}

	if(sem_init(&empty, 0, BUFFER_SIZE) != 0 || sem_init(&occupied, 0, 0) != 0)
	{
		fprintf(stderr, "semaphore initialization failed\n");
		return -1;
	}

	for(i = 0; i < producerThreads; i++)
	{
		pthread_create(&producers[i], 0, producer, NULL);
	}
 

	for(j = 0; j < consumerThreads; j++)
	{
		pthread_create(&consumers[j], 0, consumer, NULL);
	}


	/* Sleep for user specified time */
	sleep(sleepTime);

	running = 0;

	// wait for threads
	while(producerThreads > 0)
		pthread_join(producers[--producerThreads], NULL);

	while(consumerThreads > 0)
		pthread_join(consumers[--consumerThreads], NULL);	

	// destroy mutex
	pthread_mutex_destroy(&buffer_mutex);

	// destroy semaphore
	sem_destroy(&empty);
	sem_destroy(&occupied);

	return 0;
}

// function for producer threads
void *producer(void *param)
{
	buffer_item item; 

	while(running)
	{
		// sleep for a random about of time
		sleep(rand() % SLEEP);
		// generate a random number for item
		item = rand() % 100;
		if(insert_item(item))
			fprintf(stderr, "report error condition, Producer\n");
		else
			printf("producer produced %d\n", item);
	}
	pthread_exit(0);
}

// function for consumer threads
void *consumer(void *param)
{
	buffer_item item;

	while(running)
	{
		// sleep for a random period of time
		sleep(rand() % SLEEP);
		if(remove_item(&item))
			fprintf(stderr, "report error condition, Consumer\n");
		else
			printf("consumer consumed %d\n", item);
	}
	pthread_exit(0);
}