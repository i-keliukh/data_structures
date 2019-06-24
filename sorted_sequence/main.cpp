#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <set>


void init();
void add_number(int);
void dump_array(int array[10]);

namespace {
    typedef unsigned int uint;
    static uint seed = 123;
    uint mrand(uint num) {
        seed = ((seed * 1103515245) + 12345) & 0x7FFFFFFF;
        return seed % num;
    }

    std::set<int> numbers;

    bool test_dump()
    {
        int array[10];
        dump_array(array);

        int i = 0;
        for (auto it = numbers.rbegin(); it != numbers.rend(); ++it, ++i)
        {
            if (*it != array[i]) return false;
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
            numbers.insert(value);
            if (numbers.size() > 10)
            {
                numbers.erase(numbers.begin());
            }

            add_number(value);

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