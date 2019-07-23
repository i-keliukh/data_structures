#define POOL_SIZE       100000
#define HASHMAP_SIZE    500009

struct Node
{
    char value[64];
    int next;
};

int size;
Node pool[POOL_SIZE];
int free_list_head;
int head[HASHMAP_SIZE];

void init()
{
    size = 0;
    for (int i = 0; i < HASHMAP_SIZE; i++)
    {
        head[i] = -1;
    }
    free_list_head = -1;
}

unsigned long hash(register const char* str)
{
    register unsigned long hash = 5381;
    register int c;

    while (c = *str++)
    {
        hash = (((hash << 5) + hash) + c) % HASHMAP_SIZE;
    }

    return hash % HASHMAP_SIZE;
}

inline int is_same(register char *a, register char *b)
{
    while (*a != 0 && *b == *a)
    {
        a++;
        b++;
    }
    return *b == *a;
}

inline void mycpy(register char* dst, register char* src)
{
    while (*src != 0)
    {
        *dst = *src;
        src++;
        dst++;
    }
    *dst = 0;
}

inline bool find(int index, char* word)
{
    for (; index != -1; index = pool[index].next)
    {
        if (is_same(pool[index].value, word)) return true;
    }
    return false;
}

void add(char* word)
{
    unsigned int hash_value = hash(word);
    if (find(head[hash_value], word))
    {
        return;
    }

    int new_element;
    if (size < POOL_SIZE)
    {
        new_element = size;
        size++;
    }
    else
    {
        new_element = free_list_head;
        free_list_head = pool[free_list_head].next;
    }
    pool[new_element].next = head[hash_value];
    mycpy(pool[new_element].value, word);
    head[hash_value] = new_element;
}

void del(char* word)
{
    unsigned int hash_value = hash(word);
    register int index = head[hash_value];
    register int prev;
    int first = index;

    for (; index != -1; prev = index, index = pool[index].next)
    {
        if (!is_same(pool[index].value, word))
        {
            continue;
        }

        if (index == first)
        {
            head[hash_value] = pool[index].next;
        }
        else
        {
            pool[prev].next = pool[index].next;
        }
        pool[index].next = free_list_head;
        free_list_head = index;
        return;
    }
}

bool check(char* word)
{
    unsigned int index = hash(word);
    return find(head[index], word);
}
