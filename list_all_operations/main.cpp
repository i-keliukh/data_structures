#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <list>


void init();
void push_front(int value);
int pop_front();
void dump_list(int copy[]);


namespace {
	typedef unsigned int uint;
	static uint seed;
	static uint mrand(uint num)
	{
		seed = seed * 1103515245 + 37209;
		return ((seed >> 8) % num);
	}

	std::list<int> numbers;
	int list_copy[1000000];

	bool test_dump() {
		dump_list(list_copy);
		int index = 0;
		for (auto it = numbers.begin(); it != numbers.end(); ++it, ++index) {
			if (*it != list_copy[index]) return false;
		}
		return true;
	}

	bool test_random()
	{
		int iterations;
		(void)scanf("%d%u", &iterations, &seed);
		init();
		numbers.clear();
		for (int i = 0; i < iterations; ++i) {
			int action = mrand(3);
			switch (action) {
			case 0:
			case 1:
				int value;
				value = mrand(INT_MAX);
				numbers.push_front(value);
				push_front(value);
				break;
			case 2:
				if (numbers.empty()) break;
				if (pop_front() != numbers.front()) return false;
				numbers.pop_front();
				break;
			}

			if (mrand(iterations/10) == 0) {
				if (!test_dump()) return false;
			}
		}
		return test_dump();
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
