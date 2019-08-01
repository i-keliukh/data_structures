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
    int next_with_same_link;
    int prev_with_same_link;
    int first_incoming_link;
};

int hash_head[HASHMAP_SIZE];
int size;
int free_list_head;
Server pool[POOL_SIZE];

void init()
{
    size = 0;
	free_list_head = -1;
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

inline int find(unsigned int hash_value, char* name, int *previous)
{
    for (int index = hash_head[hash_value], prev = -1; index != -1; index = pool[index].next_with_same_hash)
    {
        if (is_same(pool[index].name, name))
        {
            if (previous) *previous = prev;
            return index;
        }
        prev = index;
    }
    return -1;
}


void create(char name[20], char value[20])
{
    unsigned int hash_value = hash(name);
    int index = find(hash_value, name, nullptr);
    if (index != -1)
    {
        mycpy(pool[index].value, value);
        return;
    }

	int new_item;
	if (size < POOL_SIZE)
	{
		new_item = size++;
	}
	else
	{
		new_item = free_list_head;
		free_list_head = pool[free_list_head].next_with_same_hash;
	}
	mycpy(pool[new_item].name, name);
	mycpy(pool[new_item].value, value);
    pool[new_item].link = -1;
	pool[new_item].next_with_same_hash = hash_head[hash_value];
    pool[new_item].next_with_same_link = -1;
    pool[new_item].prev_with_same_link = -1;
    pool[new_item].first_incoming_link = -1;
	hash_head[hash_value] = new_item;
}

void remove_from_children_list(int index)
{
    int current_target = pool[index].link;
    if (current_target == -1)
    {
        return;
    }

    int prev = pool[index].prev_with_same_link;
    int next = pool[index].next_with_same_link;
    if (prev == -1)
    {
        pool[current_target].first_incoming_link = next;
    }
    else
    {
        pool[prev].next_with_same_link = next;
    }

    if (next != -1)
    {
        pool[next].prev_with_same_link = prev;
    }
}

void link(char name[20], char target[20]) // target=="" means no link
{
    int source_index = find(hash(name), name, nullptr);
    remove_from_children_list(source_index);

    if (!target[0])
    {
        pool[source_index].link = -1;
        return;
    }

    // add to the list of linking to the new server
    int target_index = find(hash(target), target, nullptr);
    pool[source_index].link = target_index;

    int first_incoming_link = pool[target_index].first_incoming_link;
    if (first_incoming_link != -1)
    {
        pool[first_incoming_link].prev_with_same_link = source_index;
    }

    pool[source_index].next_with_same_link = first_incoming_link;
    pool[source_index].prev_with_same_link = -1;

    pool[target_index].first_incoming_link = source_index;
}

void get_value(char name[20], char result[96])
{
    unsigned int hash_value = hash(name);
    int index = find(hash_value, name, nullptr);
    char* p = result;
    for (int i = 0; i < 5 && index != -1; i++)
    {
        mycpy(p, pool[index].value);
        p = p + 19;
        index = pool[index].link;
    }
}

void destroy(char name[20])
{
	unsigned int hash_value = hash(name);
    int index, prev;
    index = find(hash_value, name, &prev);
    if (index == -1)
    {
        return;
    }
	
    // remove itself from the list of children in linked server
    remove_from_children_list(index);

    // rewrite link in all children
    int last_child = -1;
    for (int current = pool[index].first_incoming_link; current != -1; current = pool[current].next_with_same_link)
    {
        pool[current].link = pool[index].link;
        last_child = current;
    }

    // move the list of children to the list of children of link
    int link = pool[index].link;
    if (link != -1)
    {
        int next = pool[link].first_incoming_link;
        if (pool[index].first_incoming_link != -1)
        {
            pool[link].first_incoming_link = pool[index].first_incoming_link;
            if (next != -1)
            {
                pool[last_child].next_with_same_link = next;
                pool[next].prev_with_same_link = last_child;
            }
        }
    }

	if (prev == -1)
	{
		hash_head[hash_value] = pool[index].next_with_same_hash;
	}
	else
	{
		pool[prev].next_with_same_hash = pool[index].next_with_same_hash;
	}
	pool[index].next_with_same_hash = free_list_head;
	free_list_head = index;
	return;
}