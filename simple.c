// simple.c
// Drew Bies
// project 2
// CPSC 346 02


#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>


typedef struct person
 {
	int month;
	int day;
	int year;
	struct person *next;
} node;

node* pptr;

// makes node with head ptr
void makeNode(node** ptr)
{
	node* newNode = (node*) kmalloc(sizeof(node),GFP_USER);
	newNode -> month = 10;
	newNode -> day = 8;
	newNode -> year = 2019;
	newNode -> next = NULL;
	*ptr = newNode;
}

// prints node from ptr
void printNode(node* ptr)
{
	if(ptr != NULL)
	{
		printk(KERN_INFO "month: %d\n", ptr -> month);
		printk(KERN_INFO "day: %d\n", ptr -> day);
		printk(KERN_INFO "year: %d\n", ptr -> year);
		printk(KERN_INFO "next: NULL\n");
	}else{
		printk(KERN_INFO "ptr = NULL\n");
	}
}

/* This function is called when the module is loaded. */
int simple_init(void)
{
    printk(KERN_INFO "Loading Module\n");
	makeNode(&pptr);
	printNode(pptr);
    return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void)
 {
	printk(KERN_INFO "Removing Module\n");
	pptr = NULL;
	printNode(pptr);
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");