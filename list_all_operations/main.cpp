#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <algorithm>
#include <vector>
#include <list>


void init();
void push_front(int value);
int pop_front();
void push_back(int value);
void dump_list(int copy[]);

void init_iterator();
void inc_iterator();
int get_value();
void insert(int value);


namespace {
	typedef unsigned int uint;
	static uint seed;
	static uint mrand(uint num)
	{
		seed = seed * 1103515245 + 37209;
		return ((seed >> 8) % num);
	}

	std::list<int> numbers;
	std::list<int>::iterator iterator;
	int list_copy[1000000];

	bool test_dump()
	{
		dump_list(list_copy);
		int index = 0;
		for (auto it = numbers.begin(); it != numbers.end(); ++it, ++index)
		{
			if (*it != list_copy[index]) return false;
		}
		return true;
	}
	bool test_action(int action)
	{
		switch (action)
		{
			case 0:
			{
				int value = mrand(INT_MAX);
				numbers.push_back(value);
				push_back(value);
				break;
			}
			case 1:
			{
				int value = mrand(INT_MAX);
				numbers.push_front(value);
				push_front(value);
				break;
			}
			case 2:
			{
				if (numbers.empty() || iterator == numbers.begin()) break;
				if (pop_front() != numbers.front()) return false;
				numbers.pop_front();
				break;
			}
			case 3:
			{
				if (numbers.empty()) break;
				init_iterator();
				iterator = numbers.begin();
				break;
			}
			case 4:
			{
				if (iterator == numbers.end()) break;
				inc_iterator();
				++iterator;
				break;
			}
			case 5:
			{
				if (iterator == numbers.end()) break;
				if (*iterator != get_value()) return false;
				break;
			}
			case 6:
			{
				if (iterator == numbers.end()) break;
				iterator++;
				int value = mrand(INT_MAX);
				iterator = numbers.insert(iterator, value);
				insert(value);
				break;
			}
			case 7:
			{
				if (!test_dump()) return false;
				break;
			}

		}

		return true;
	}

	bool test_random()
	{
		int iterations = 0;
		(void)scanf("%d%u", &iterations, &seed);
		init();
		numbers.clear();
		iterator = numbers.end();
		for (int i = 0; i < iterations; ++i)
		{
			if (!test_action(mrand(7))) return false;

			if (mrand(iterations/10) == 0)
			{
				if (!test_dump()) return false;
			}
		}
		return test_dump();
	}

	bool test_manual()
	{
		int commands;
		bool result = true;
		(void)scanf("%d", &commands);
		init();
		numbers.clear();
		iterator = numbers.end();
		for (int i = 0; i < commands; ++i)
		{
			int action;
			(void)scanf("%d", &action);
			result = result && test_action(action);
		}
		return result && test_dump();
	}

	bool test()
	{
		int method;
		(void)scanf("%d", &method);
		return method == 1 ? test_random() : test_manual();
	}
}


int main()
{
	setbuf(stdout, NULL);
	int ntests = 0;
	(void)scanf("%d", &ntests);
	for (int t = 1; t <= ntests; ++t)
	{

		printf("#%d %d\n", t, test() ? 100 : 0);
	}
	return 0;
}
