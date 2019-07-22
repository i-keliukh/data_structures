#define POOL_SIZE       900000
#define HASHMAP_SIZE    500009

typedef unsigned long long ulong64;

struct Node
{
    ulong64 value;
    int next;
};

int size;
Node pool[POOL_SIZE];
int head[HASHMAP_SIZE];

void init()
{
    size = 0;
    for (int i = 0; i < HASHMAP_SIZE; i++)
    {
        head[i] = -1;
    }
}

inline unsigned int hash(ulong64 word)
{
    return word % HASHMAP_SIZE;
}

inline ulong64 to_ulong64(char* word)
{
    return (*(ulong64*)word) & 0xFFFFFFFFFFFF;
}

inline bool find(int index, ulong64 word)
{
    for (; index != -1; index = pool[index].next)
    {
        if (pool[index].value == word) return true;
    }
    return false;
}

void add(char* word)
{
    ulong64 string = to_ulong64(word);
    unsigned int hash_value = hash(string);
    if (find(head[hash_value], string))
    {
        return;
    }
    pool[size].next = head[hash_value];
    pool[size].value = string;
    head[hash_value] = size;
    size++;
}

void del(char* word)
{
    ulong64 string = to_ulong64(word);
    unsigned int hash_value = hash(string);
    int index = head[hash_value];
    int prev = index;
    int first = index;

    for (; index != -1; index = pool[index].next)
    {
        if (pool[index].value == string)
        {
            if (index == first)
            {
                head[hash_value] = pool[index].next;
            }
            else
            {
                pool[prev].next = pool[index].next;
            }
            return;
        }
        prev = index;
    }
}

bool check(char* word)
{
    ulong64 string = to_ulong64(word);
    unsigned int index = hash(string);
    return find(head[index], string);
}
