#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <algorithm>
#include <vector>
#include <list>

#define NUMBER_OF_BUCKETS 1000

void init();
void push_front(int bucket, int value);
int pop_front(int bucket);
void push_back(int bucket, int value);
void dump_list(int bucket, int copy[]);

void init_iterator(int bucket);
void inc_iterator(int bucket);
int get_value(int bucket);
void insert(int bucket, int value);

void remove(int bucket);

void cut_tail(int bucket);
void paste(int bucket);


namespace {
    typedef unsigned int uint;
    static uint seed;
    static uint mrand(uint num)
    {
        seed = seed * 1103515245 + 37209;
        return ((seed >> 8) % num);
    }

    std::list<int> numbers[NUMBER_OF_BUCKETS], cut_list;
    std::list<int>::iterator numbers_iterator[NUMBER_OF_BUCKETS];
    int numbers_iterator_index[NUMBER_OF_BUCKETS];

    int list_copy[1000000];

    bool test_dump()
    {
        for (int i = 0; i < NUMBER_OF_BUCKETS; i++)
        {
            dump_list(i, list_copy);
            int index = 0;
            for (auto it = numbers[i].begin(); it != numbers[i].end(); ++it, ++index)
            {
                if (*it != list_copy[index]) return false;
            }
        }
        return true;
    }

    bool test_action(int action)
    {
        int bucket = mrand(NUMBER_OF_BUCKETS);

        switch (action)
        {
            case 0:
            {
                int value = mrand(INT_MAX);
                numbers[bucket].push_back(value);
                push_back(bucket, value);
                break;
            }
            case 1:
            {
                int value = mrand(INT_MAX);
                numbers[bucket].push_front(value);
                push_front(bucket, value);
                ++numbers_iterator_index[bucket];
                break;
            }
            case 2:
            {
                if (numbers[bucket].empty() || numbers_iterator[bucket] == numbers[bucket].begin()) break;
                if (pop_front(bucket) != numbers[bucket].front()) return false;
                numbers[bucket].pop_front();
                --numbers_iterator_index[bucket];
                break;
            }
            case 3:
            {
                if (numbers[bucket].empty()) break;
                init_iterator(bucket);
                numbers_iterator[bucket] = numbers[bucket].begin();
                numbers_iterator_index[bucket] = 0;
                break;
            }
            case 4:
            {
                if (numbers_iterator[bucket] == numbers[bucket].end()) break;
                inc_iterator(bucket);
                ++numbers_iterator[bucket];
                ++numbers_iterator_index[bucket];
                break;
            }
            case 5:
            {
                if (numbers_iterator[bucket] == numbers[bucket].end()) break;
                if (*numbers_iterator[bucket] != get_value(bucket)) return false;
                break;
            }
            case 6:
            {
                if (numbers_iterator[bucket] == numbers[bucket].end()) break;
                ++numbers_iterator[bucket];
                ++numbers_iterator_index[bucket];
                int value = mrand(INT_MAX);
                numbers_iterator[bucket] = numbers[bucket].insert(numbers_iterator[bucket], value);
                insert(bucket, value);
                break;
            }
            case 7:
            {
                if (numbers_iterator[bucket] == numbers[bucket].end()) break;
                remove(bucket);
                auto next_element = std::next(numbers_iterator[bucket]);
                if (next_element == numbers[bucket].end()) break;
                numbers[bucket].erase(next_element);
                break;
            }
            case 8:
            {
                if (numbers_iterator[bucket] == numbers[bucket].end()) break;
                if (!cut_list.empty() && mrand(10000) != 0) break; // the cut list gets overwritten with low probability
                
                auto next_element = std::next(numbers_iterator[bucket]);

                if (numbers[bucket].size() % 2 == 0 && numbers_iterator_index[bucket] == numbers[bucket].size() / 2 - 1)
                {
                    // The nasty bug in std library of VC 2019 leads to the incorrect
                    // update of the debug pointer to owning container inside iterator
                    // when the number of spliced elements equals the number of remaining
                    // elements. It only reproduces when using debug configuration in
                    // IDE, but is still annoying enough to require this workaround.
                    break;
                }

                if (next_element != numbers[bucket].end())
                {
                    cut_list.clear();
                    cut_list.splice(cut_list.begin(), numbers[bucket], next_element, numbers[bucket].end());
                }
                cut_tail(bucket);

                break;
            }
            case 9:
            {
                if (cut_list.empty()) break;
                if (numbers_iterator[bucket] == numbers[bucket].end()) break;
                paste(bucket);
                numbers[bucket].splice(std::next(numbers_iterator[bucket]), cut_list);
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
    
    void test_init()
    {
        init();
        cut_list.clear();

        for (int i = 0; i < NUMBER_OF_BUCKETS; i++)
        {
            numbers[i].clear();
            numbers_iterator[i] = numbers[i].end();
            numbers_iterator_index[i] = -1;
        }
    }

    bool test_random()
    {
        int iterations = 0;
        (void)scanf("%d%u", &iterations, &seed);

        test_init();

        for (int i = 0; i < iterations; ++i)
        {
            int action = mrand(10);

            if (!test_action(action)) return false;
                
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

        test_init();

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
