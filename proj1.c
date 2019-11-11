// proj1.c
// Drew Bies
// CPSC 346 02

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node
{
	int data;
	struct node * next;
} node;



// prints the nodes
void printNode(node* head)
{
	node* temp = head;
	while(temp != NULL)
	{
		printf("%d -> ", temp -> data);
		if(temp -> data < 10)
			printf(" ");
		temp = temp -> next; 
	}
	printf("NULL\n\n");
}

// move forward n times and return the head
node* moveForward(node* head, int n)
{
	int i;
	for(i = 0; i < n; i++)
	{
		head = head -> next;
		if(head == NULL)
			return head;
	}
	return head;
}

// pushes data at the front of head
void push(node** head, int newData)
{
	node* newNode = (node*) malloc(sizeof(node));
	newNode -> data = newData;
	newNode -> next = *head;
	*head = newNode;
}

// merges head1 and head2 and returns head
node* merge(node* head1, node* head2)
{
	node* temp = NULL;
	
	if(head1 == NULL)
		return head2;
	else if(head2 == NULL)
		return head1;
	
	if(head1 -> data < head2 -> data)
	{
		temp = head1;
		temp -> next = merge(head1 -> next, head2);
	}
	else
	{
		temp = head2;
		temp -> next = merge(head1, head2 -> next);
	}
	
	return temp;
}

// returns the list size of head
int getListSize(node* head)
{
	int size = 0;
	while(head != NULL)
	{
		size++;
		head = head -> next;
	}
	
	return size;
}

// merge sorts head with length size and returns sorted head
node* mergeSort(node* head, int size)
{
	if(size == 1)
		return head;
	
	node* head2 = NULL;
	head2 = moveForward(head, size / 2);
	
	node* temp = NULL;
	temp = moveForward(head, size / 2 - 1);	
	temp -> next = NULL;
	
	merge(mergeSort(head, getListSize(head)), mergeSort(head2, getListSize(head2)));
}

// makes node with length size and returns head
node* makeNode(int size)
{
	int i = 0;
	
	srand(time(0));
	
	node* headptr = NULL;
	
	for(i; i < size; i++)
	{
		push(&headptr, rand() % 50);
	}
	
	return headptr;
}

int main()
{
	node* a = makeNode(7);
	
	printf("\nnode a unsorted: ");
	printNode(a);
	
	printf("node a   sorted: ");
	printNode(mergeSort(a, getListSize(a)));
	
	return 0;
}