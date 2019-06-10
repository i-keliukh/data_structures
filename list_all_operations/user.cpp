#define MAX_SIZE	1100000

#define PROPER_IMPLEMENTATION
#ifdef PROPER_IMPLEMENTATION
struct Node {
	int value;
	int next;
};

Node array[MAX_SIZE];
unsigned int size;

int first;

void init() 
{
	size = 0;
	first = -1;
}

void push_front(int value)
{
	array[size].value = value;
	array[size].next = first;
	first = size;
	size++;
}

int pop_front()
{
	int result = array[first].value;
	first = array[first].next;
	return result;
}

void dump_list(int copy[])
{
	int iterator = first;
	int index = 0;
	while (iterator != -1)
	{
		copy[index] = array[iterator].value;
		index++;
		iterator = array[iterator].next;
	}
}
#else

#include <memory.h>

int array[MAX_SIZE];
unsigned int size;


void init()
{
	size = 0;
}

void push_front(int value)
{
	memmove(array + 1, array, size * sizeof(int));
	array[0] = value;
	size++;
}

int pop_front()
{
	int result = array[0];
	memmove(array, array + 1, size * sizeof(int));
	size--;
	return result;
}

void dump_list(int copy[])
{
	memcpy(copy, array, size * sizeof(int));
}
#endif