// sorting.c
// Drew Bies
// username: abies2
// CPSC 346 02
// 10-29-2019

// this program demonstrates multi-threaded sorting of an array.
// the array is pre-defined. the array is cut in half and sorted with two threads using bubble sort.
// after the two threads are done, a third thread merges the two halves into a fully sorted array
//	and prints to the screen.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE				10
#define NUMBER_OF_THREADS	3

void printArray(int arr[]);
void *sorter(void *params);	/* thread that performs basic sorting algorithm */
void *merger(void *params);	/* thread that performs merging of results */

int list[SIZE] = {7,12,19,3,18,4,2,6,15,8};

int result[SIZE];

typedef struct parameters
{
	int from_index;
	int to_index;
} parameters;

int main (int argc, const char * argv[]) 
{
	// print unsorted array first
	printf("Unsorted Array:\n");
	printArray(list);
	
	int i;
    
	pthread_t workers[NUMBER_OF_THREADS];
	
	/* establish the first sorting thread */
	//1. call malloc to allocate a “parameters”
	parameters* paramsT1 = (parameters*) malloc(sizeof(parameters));
	
	//2. use “parameters” to specify the first half of the array
	paramsT1 -> from_index = 0;
	paramsT1 -> to_index = SIZE / 2 - 1;
	
	//3. create the first thread
	pthread_create(&workers[0], 0, sorter, (void *) paramsT1);

	/* establish the second sorting thread */
	//1. call malloc to allocate a “parameters”
	parameters* paramsT2 = (parameters*) malloc(sizeof(parameters));
	
	//2. use “parameters” to specify the first half of the array
	paramsT2 -> from_index = SIZE / 2;
	paramsT2 -> to_index = SIZE - 1;
	
	//3. create the second thread
	pthread_create(&workers[1], 0, sorter, (void *) paramsT2);
	
	/* now wait for the 2 sorting threads to finish */
	// use pthread_join; wait for 2 sorting threads to finish 
	pthread_join(workers[0], NULL);
	pthread_join(workers[1], NULL);
	

	/* establish the merge thread */
	//reuse “parameters” to hold the beginning index in each half
	paramsT1 -> from_index = 0;
	paramsT1 -> to_index = SIZE / 2;
	
	//create the third thread: merge
	pthread_create(&workers[2], 0, merger, (void *) paramsT1);
	
	/* wait for the merge thread to finish */
	pthread_join(workers[2], NULL);
	 


	/* output the sorted array */
	
	printf("Sorted Array:\n");
	printArray(result);
	
	// free memory
	free(paramsT1);
	free(paramsT2);
	
    return 0;
}

/**
 * Sorting thread.
 *
 * This thread can essentially use any algorithm for sorting
 */

void *sorter(void *params)
{
	parameters* p = (parameters *)params;
	
	// Sorting algorithm here. It can be any sorting algorithm.
	int j,
	i,
	temp;
	
	// bubble sort
	for(j = 0; j < (p -> to_index - p -> from_index); j++)
	{
		for(i = p -> from_index; i < p -> to_index - j; i++)
		{
			if(list[i] > list[i + 1])
			{
				temp = list[i];
				list[i] = list[i + 1];
				list[i + 1] = temp;
			}
		}
	}

	pthread_exit(0);
}

/**
 * Merge thread
 *
 * Uses simple merge sort for merging two sublists
 */

void *merger(void *params)
{
	parameters* p = (parameters *)params;
	
	//reuse “parameters” to hold the first index in each half
	//merge two sorted sublist to the arry result
	int i, first = p -> from_index, second = p -> to_index;
	for(i = 0; i < SIZE; i++)
	{
		if((list[first] < list[second] && first < p -> to_index) || second > SIZE - 1)
			result[i] = list[first++];
		else
			result[i] = list[second++];
	}
	
	
	pthread_exit(0);
}

// print array arr to the screen
// the size of the array must be SIZE
void printArray(int arr[])
{
	int i;
	for(i = 0; i < SIZE; i++)
	{
		printf("%d\t", arr[i]);
	}
	printf("\n\n");
}

