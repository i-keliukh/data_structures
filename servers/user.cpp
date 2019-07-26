/*
1. Server identified by unique name, keeps value; second create overwrites the value
2. There is a link to upstream server
3. On getting value, server returns its own value and all values from upstream server
4. Links can be updated
5. Server are created and deleted dynamically
5. Upon delete, all servers that link to current server relink to its parent (if any)
*/


#define POOL_SIZE       100000
#define HASHMAP_SIZE    500009


struct Server
{
    char name[20];
    char value[20];
    int link;
    int next_with_same_hash;
};

int hash_head[HASHMAP_SIZE];
int size;
Server pool[POOL_SIZE];

void init()
{
    size = 0;
    for (int i = 0; i < HASHMAP_SIZE; i++)
    {
        hash_head[i] = -1;
    }
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


inline int is_same(register char* a, register char* b)
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
    for (; index != -1; index = pool[index].next_with_same_hash)
    {
        if (is_same(pool[index].name, word)) return true;
    }
    return false;
}


void create(char name[20], char value[20])
{
    unsigned int hash_value = hash(name);
    for (int index = hash_head[hash_value]; index != -1; index = pool[index].next_with_same_hash)
    {
        if (is_same(pool[index].name, name))
        {
            mycpy(pool[index].value, value);
            return;
        }
    }



}

void link(char name[20], char target[20]) // target=="" means no link
{
}

void get_value(char name[20], char result[100])
{
}

void destroy(char name[20])
{
}