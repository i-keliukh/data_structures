#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <list>


void init();
void push_back(int value);
int pop_back();
int get_value(int index);


namespace {
	typedef unsigned int uint;
	static uint seed;
	uint mrand(uint num) {
		seed = ((seed * 1103515245) + 12345) & 0x7FFFFFFF;
		return seed % num;
	}

	bool test_random()
	{
		int iterations;
		scanf("%d%u", &iterations, &seed);
		init();
		std::vector<int> stack;
		for (int i = 0; i < iterations; ++i) {
			int action = mrand(3);
			switch (action) {
			case 0:
				uint value;
				value = mrand(UINT_MAX);
				stack.push_back(value);
				push_back(value);
				break;
			case 1:
				if (stack.empty()) break;
				if (stack.back() != pop_back()) return false;
				stack.pop_back();
				break;
			default:
				if (stack.empty()) break;
				int index = mrand(stack.size());
				if (stack[index] != get_value(index)) return false;
				break;
			}
		}
		return true;
	}
}

int main()
{
	setbuf(stdout, NULL);
	int ntests = 0;
	scanf("%d", &ntests);
	for (int t = 1; t <= ntests; ++t) {
		printf("#%d %d\n", t, test_random() ? 100 : 0);
	}
	return 0;
}