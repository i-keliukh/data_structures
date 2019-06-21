#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <algorithm>
#include <vector>
#include <list>


void init();
void add_number(int);
void dump_array(int array[10]);

namespace {
    typedef unsigned int uint;
    static uint seed;
    uint mrand(uint num) {
        seed = ((seed * 1103515245) + 12345) & 0x7FFFFFFF;
        return seed % num;
    }

    bool test_random()
    {
		int iterations = 0;
		(void)scanf("%d%u", &iterations, &seed);
		init();
		for (int i = 0; i < iterations; ++i)
		{
			add_number(mrand(INT_MAX));
		}

		int array[10];
		dump_array(array);
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