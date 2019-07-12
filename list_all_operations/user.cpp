#define MAX_SIZE            1100000
#define NUMBER_OF_BUCKETS   1000

struct Node
{
    int value;
    int next;
};

Node array[MAX_SIZE];
unsigned int size;

struct ListIterators {
    int first;
    int last;
    int current;
};

ListIterators list_iterators[NUMBER_OF_BUCKETS];
int cut_begin;
int cut_end;


void init() 
{
    size = 0;
    for (int i = 0; i < NUMBER_OF_BUCKETS; i++)
    {
        list_iterators[i].first = -1;
        list_iterators[i].last = -1;
    }
}

void push_front(int bucket, int value)
{
    array[size].value = value;
    array[size].next = list_iterators[bucket].first;
    if (list_iterators[bucket].first == -1)
    {
        list_iterators[bucket].last = size;
    }
    list_iterators[bucket].first = size;
    size++;
}

int pop_front(int bucket)
{
    int result = array[list_iterators[bucket].first].value;
    list_iterators[bucket].first = array[list_iterators[bucket].first].next;
    if (list_iterators[bucket].first == -1)
    {
        list_iterators[bucket].last = -1;
    }
    return result;
}

void push_back(int bucket, int value)
{
    array[size].value = value;
    array[size].next = -1;
    if (list_iterators[bucket].last == -1)
    {
        list_iterators[bucket].first = size;
    }
    else
    {
        array[list_iterators[bucket].last].next = size;
    }
    list_iterators[bucket].last = size;
    size++;
}

void dump_list(int bucket, int copy[])
{
    int iterator = list_iterators[bucket].first;
    int index = 0;
    while (iterator != -1)
    {
        copy[index] = array[iterator].value;
        index++;
        iterator = array[iterator].next;
    }
}


void init_iterator(int bucket)
{
    list_iterators[bucket].current = list_iterators[bucket].first;
}

void inc_iterator(int bucket)
{
    list_iterators[bucket].current = array[list_iterators[bucket].current].next;
}

int get_value(int bucket)
{
    return array[list_iterators[bucket].current].value;
}

void insert(int bucket, int value)
{
    int next = array[list_iterators[bucket].current].next;

    array[size].value = value;
    array[size].next = next;

    array[list_iterators[bucket].current].next = size;
    if (list_iterators[bucket].last == list_iterators[bucket].current)
    {
        list_iterators[bucket].last = size;
    }
    list_iterators[bucket].current = size;

    size++;
}

void remove(int bucket)
{
    int next = array[list_iterators[bucket].current].next;
    if (next == -1) return;

    array[list_iterators[bucket].current].next = array[next].next;
    if (list_iterators[bucket].last == next)
    {
        list_iterators[bucket].last = list_iterators[bucket].current;
    }
}


void cut_tail(int bucket)
{
    if (array[list_iterators[bucket].current].next == -1) return;
    cut_begin = array[list_iterators[bucket].current].next;
    cut_end = list_iterators[bucket].last;
    list_iterators[bucket].last = list_iterators[bucket].current;
    array[list_iterators[bucket].current].next = -1;
}

void paste(int bucket)
{
    int next = array[list_iterators[bucket].current].next;
    array[list_iterators[bucket].current].next = cut_begin;
    array[cut_end].next = next;
    if (list_iterators[bucket].last == list_iterators[bucket].current)
    {
        list_iterators[bucket].last = cut_end;
    }
}