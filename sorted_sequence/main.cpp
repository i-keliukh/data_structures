#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <set>


void init();
void add_number(int value, unsigned short props);
void dump_array(int array[10], unsigned short mandatory_props);

namespace {
    typedef unsigned int uint;
    static uint seed = 123;
    static uint mrand(uint num)
    {
        seed = seed * 1103515245 + 37209;
        return ((seed >> 8) % num);
    }

    struct StoredNode
    {
        int value;
        unsigned short props;
        StoredNode(int value, unsigned short props) : value(value), props(props) {}
        bool operator<(const StoredNode& right) const
        {
            return value < right.value;
        }
    };

    std::multiset<StoredNode> numbers;

    bool test_dump()
    {
        int array[10];
        for (int attempts = 0; attempts < 16; attempts++)
        {
            unsigned short mandatory_props = mrand(USHRT_MAX + 1);
            dump_array(array, mandatory_props);

            int i = 0;
            for (auto it = numbers.rbegin(); it != numbers.rend() && i < 10; ++it)
            {
                if ((it->props & mandatory_props) != mandatory_props) continue;
                if (it->value != array[i])
                    return false;
                i++;
            }
        }
        return true;
    }

    bool test_random()
    {
        int iterations = 0;
        (void)scanf("%d", &iterations);
        init();
        numbers.clear();

        for (int i = 0; i < iterations; ++i)
        {
            int value = mrand(INT_MAX);
            unsigned short props = mrand(USHRT_MAX + 1);
            numbers.insert(StoredNode(value, props));

            add_number(value, props);

            if (mrand(1 + iterations / 10) == 0)
            {
                if (!test_dump()) return false;
            }
        }

        if (!test_dump()) return false;

        return true;
    }
}

int main()
{
    setbuf(stdout, NULL);
    int ntests = 0;
    (void)scanf("%d", &ntests);
    for (int t = 1; t <= ntests; ++t) {
        printf("#%d %d\n", t, test_random() ? 100 : 0);
    }
    return 0;
}