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

void remove();

void cut(int count);
void paste();


namespace {
	typedef unsigned int uint;
	static uint seed;
	static uint mrand(uint num)
	{
		seed = seed * 1103515245 + 37209;
		return ((seed >> 8) % num);
	}

	std::list<int> numbers, cut_list;
	std::list<int>::iterator numbers_iterator;

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
				if (numbers.empty() || numbers_iterator == numbers.begin()) break;
				if (pop_front() != numbers.front()) return false;
				numbers.pop_front();
				break;
			}
			case 3:
			{
				if (numbers.empty()) break;
				init_iterator();
				numbers_iterator = numbers.begin();
				break;
			}
			case 4:
			{
				if (numbers_iterator == numbers.end()) break;
				inc_iterator();
				++numbers_iterator;
				break;
			}
			case 5:
			{
				if (numbers_iterator == numbers.end()) break;
				if (*numbers_iterator != get_value()) return false;
				break;
			}
			case 6:
			{
				if (numbers_iterator == numbers.end()) break;
				numbers_iterator++;
				int value = mrand(INT_MAX);
				numbers_iterator = numbers.insert(numbers_iterator, value);
				insert(value);
				break;
			}
			case 7:
			{
				if (numbers_iterator == numbers.end()) break;
				remove();
				auto next_element = std::next(numbers_iterator);
				if (next_element == numbers.end()) break;
				numbers.erase(next_element);
				break;
			}
			case 8:
			{
				if (numbers_iterator == numbers.end()) break;
				auto next_element = std::next(numbers_iterator);

				int count = mrand((uint)numbers.size() + 1) + 1;
				int real_count = 0;

				auto last_element = next_element;
				while (real_count < count && last_element != numbers.end())
				{
					++last_element;
					++real_count;
				}

				if (numbers.size() % 2 == 0 && real_count == numbers.size() / 2)
				{
					// The nasty bug in std library of VC 2019 leads to the incorrect
					// update of the debug pointer to owning container inside iterator
					// when the number of spliced elements equals the number of remaining
					// elements. It only reproduces when using debug configuration in
					// IDE, but is still annoying enough to require this workaround.
					break;
				}

				if (next_element != numbers.end())
				{
					cut_list.clear();
					cut_list.splice(cut_list.begin(), numbers, next_element, last_element);
				}
				cut(count);

				break;
			}
			case 9:
			{
				if (cut_list.empty()) break;
				if (numbers_iterator == numbers.end()) break;
				paste();
				numbers.splice(std::next(numbers_iterator), cut_list);
				break;
			}
			case 10:
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
		cut_list.clear();
		numbers_iterator = numbers.end();
		for (int i = 0; i < iterations; ++i)
		{
			int action = mrand(10);
			if (action != 8 || mrand(500) == 0)
			{
				if (!test_action(action)) return false;
			}

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
		cut_list.clear();
		numbers_iterator = numbers.end();

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
