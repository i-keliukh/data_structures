struct Node
{
    int value;
    unsigned short props;
    int next;
};

#define NUMBER_OF_BUCKETS 1000

static int list_heads[NUMBER_OF_BUCKETS];

static Node pool[250000];
static int size = 0;


void init()
{
    size = 0;
    for (int i = 0; i < NUMBER_OF_BUCKETS; i++)
    {
        list_heads[i] = -1;
    }
}


void add_number(int bucket, int value, unsigned short props)
{
    if (list_heads[bucket] < 0 || pool[list_heads[bucket]].value < value)
    {
        pool[size].value = value;
        pool[size].props = props;
        pool[size].next = list_heads[bucket];
        list_heads[bucket] = size;
        size++;
        return;
    }

    int current = list_heads[bucket];
    while (pool[current].next > -1)
    {
        if (pool[pool[current].next].value > value)
        {
            current = pool[current].next;
        }
        else
        {
            break;
        }
    }

    pool[size].value = value;
    pool[size].props = props;
    pool[size].next = pool[current].next;
    pool[current].next = size;
    size++;
    return;
}


void dump_array(int bucket, int array[10], unsigned short mandatory_props)
{
    int current = list_heads[bucket];
    int added = 0;
    while (current != -1)
    {
        if (pool[current].value == 16657355)
        {
            int i = 0;
        }

        if((pool[current].props & mandatory_props) != mandatory_props)
        {
            current = pool[current].next;
            continue;
        }

        array[added++] = pool[current].value;
        current = pool[current].next;

        if (added == 10) return;
    }
}
