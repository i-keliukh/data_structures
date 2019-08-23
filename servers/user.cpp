#define POOL_SIZE       100000
#define HASHMAP_SIZE    500009

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


inline int mystrcmp(register char* a, register char* b)
{
    while (*a != 0 && *b == *a)
    {
        a++;
        b++;
    }
    return *b != *a;
}

inline char* mystrcpy(register char* dst, register char* src)
{
    while (*src != 0)
    {
        *dst = *src;
        src++;
        dst++;
    }
    *dst = 0;
    return dst;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ServerEntry
{
    char value[20];
    ServerEntry* link;
    ServerEntry* next_with_same_link;
    ServerEntry* prev_with_same_link;
    ServerEntry* first_incoming_link;
};

struct HashEntry
{
    char name[20];
    HashEntry* next;
    ServerEntry data;
};



int allocator_size;
HashEntry* allocator_free_list_head;
HashEntry allocator_pool[POOL_SIZE];

void allocator_init()
{
    allocator_size = 0;
    allocator_free_list_head = nullptr;
}

HashEntry* allocator_alloc()
{
    if (allocator_size < POOL_SIZE)
    {
        return &allocator_pool[allocator_size++];
    }
    HashEntry* result = allocator_free_list_head;
    allocator_free_list_head = allocator_free_list_head->next;
    return result;
}

void allocator_free(HashEntry* hash_entry)
{
    hash_entry->next = allocator_free_list_head;
    allocator_free_list_head = hash_entry;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

HashEntry* hash_head[HASHMAP_SIZE];

inline HashEntry* hash_find(unsigned int index_in_hash_heads, char* name, HashEntry** previous)
{
    for (HashEntry *current = hash_head[index_in_hash_heads], *prev = nullptr; current != nullptr; current = current->next)
    {
        if (mystrcmp(current->name, name) == 0)
        {
            if (previous) *previous = prev;
            return current;
        }
        prev = current;
    }
    return nullptr;
}

void hash_init()
{
    allocator_init();
    for (int i = 0; i < HASHMAP_SIZE; i++)
    {
        hash_head[i] = nullptr;
    }
}

ServerEntry* hash_get_or_create(char* name, bool* created)
{
    int index_in_hash_heads = hash(name);
    HashEntry* hash_entry = hash_find(index_in_hash_heads, name, nullptr);

    if (!hash_entry)
    {
        hash_entry = allocator_alloc();
        mystrcpy(hash_entry->name, name);
        hash_entry->next = hash_head[index_in_hash_heads];
        hash_head[index_in_hash_heads] = hash_entry;
        *created = true;
    }
    else
    {
        *created = false;
    }
    return &hash_entry->data;
}

ServerEntry* hash_get(char* name)
{
    int index_in_hash_heads = hash(name);
    HashEntry* hash_entry = hash_find(index_in_hash_heads, name, nullptr);
    if (!hash_entry)
    {
        return nullptr;
    }
    else
    {
        return &hash_entry->data;
    }
}

void hash_remove(char* name)
{
    int index_in_hash_heads = hash(name);
    HashEntry* previous_entry;
    HashEntry* hash_entry = hash_find(index_in_hash_heads, name, &previous_entry);

    if (!hash_entry) return;

    if (previous_entry)
    {
        previous_entry->next = hash_entry->next;
    }
    else
    {
        hash_head[index_in_hash_heads] = hash_entry->next;
    }
    allocator_free(hash_entry);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void init()
{
    hash_init();
}

void create(char name[20], char value[20])
{
    bool created;
    ServerEntry* server = hash_get_or_create(name, &created);

    mystrcpy(server->value, value);
    if (created)
    {
        server->next_with_same_link = nullptr;
        server->prev_with_same_link = nullptr;
        server->link = nullptr;
        server->first_incoming_link = nullptr;
    }
}

void remove_from_backlist(ServerEntry* server)
{
    if (!server->link) return;

    ServerEntry* next = server->next_with_same_link;
    ServerEntry* prev = server->prev_with_same_link;

    if (prev)
    {
        prev->next_with_same_link = next;
    }
    else
    {
        server->link->first_incoming_link = next;
    }

    if (next)
    {
        next->prev_with_same_link = prev;
    }

}

void destroy(char name[20])
{
    ServerEntry* server = hash_get(name);
    if (!server) return;

    remove_from_backlist(server);

    ServerEntry* last_child = nullptr;
    for (ServerEntry* current = server->first_incoming_link; current != nullptr; current = current->next_with_same_link)
    {
        current->link = server->link;
        last_child = current;
    }

    if (server->link)
    {
        ServerEntry* first_current_link = server->link->first_incoming_link;
        if (server->first_incoming_link)
        {
            server->link->first_incoming_link = server->first_incoming_link;
            if (first_current_link)
            {
                last_child->next_with_same_link = first_current_link;
                first_current_link->prev_with_same_link = last_child;
            }
        }
    }

    hash_remove(name);
}

void link(char source[20], char target[20])
{
    ServerEntry* source_server = hash_get(source);
    ServerEntry* target_server = hash_get(target);

    remove_from_backlist(source_server);
    source_server->link = target_server;

    if (target_server->first_incoming_link)
    {
        target_server->first_incoming_link->prev_with_same_link = source_server;
    }

    source_server->prev_with_same_link = nullptr;
    source_server->next_with_same_link = target_server->first_incoming_link;
    target_server->first_incoming_link = source_server;
}

void unlink(char name[20])
{
    ServerEntry* source_server = hash_get(name);

    remove_from_backlist(source_server);
    source_server->link = nullptr;
}

void get_value(char name[20], char result[96])
{
    ServerEntry* server = hash_get(name);
    char* p = result;

    for (int i = 0; i < 5 && server != nullptr; i++)
    {
        p = mystrcpy(p, server->value);
        server = server->link;
    }
}
