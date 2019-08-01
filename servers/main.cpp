#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

const int kWordSize = 20;
const int kMaxWords = 100000;

//////////////////////////////////////////////////////////////////////////////////////////////////
extern void init();
void create(char name[20], char value[20]);
void link(char name[20], char target[20]); // target=="" means no link
void get_value(char name[20], char result[96]);
void destroy(char name[20]);
//////////////////////////////////////////////////////////////////////////////////////////////////

namespace {

    typedef unsigned long long UInt64;

	struct Rnd {
		UInt64 seed;
		static const int kAlpahabetSize = ('f' - 'a') + 1;
		char alpahabet[kAlpahabetSize];

		Rnd()
        {
			char* p = alpahabet;
			for (char c = 'a'; c <= 'f'; ++c)* p++ = c;
		}

        void init(UInt64 new_seed)
        {
            seed = new_seed;
        }

		inline UInt64 rand() {
			seed = (214013 * seed + 2531011);
			return seed >> 16;
		}

		inline int rand(UInt64 mx) {
			return static_cast<int>(rand() % mx);
		}

		inline char rand_char() {
			return alpahabet[rand(kAlpahabetSize)];
		}

        inline void rand_str(int length, char* ptr) {
            length--;
            char* end = ptr + length;
            while (ptr != end)* ptr++ = rand_char();
            *ptr = 0;
        }
    };

	class Checker
	{
	public:
		int run(int seed, int words, int rounds) {
            rnd.init(seed);

			for (int i = 0; i < words; ++i) {
                rnd.rand_str(kWordSize, dict[i].name);
                rnd.rand_str(kWordSize, dict[i].value);
                dict[i].present = false;
                dict[i].link = -1;
			}

			const int initRounds = static_cast<int>(words * 0.9);
			int fail = 0;
			::init();
			for (int i = 0; i < initRounds; ++i) {
				add(rnd.rand(words));
				add(rnd.rand(words));
				del(rnd.rand(words));
				if (!check(rnd.rand(words))) {
					++fail;
				}
			}
			for (int i = 0; i < rounds; ++i) {
				add(rnd.rand(words));
                del(rnd.rand(words));
                link(rnd.rand(words), rnd.rand((UInt64)words + 1) - 1);
				if (!check(rnd.rand(words))) {
					++fail;
				}
			}
			return fail;
		}
	private:
		void add(int index) {
            char value[kWordSize];
            rnd.rand_str(kWordSize, value);
			::create(dict[index].name, value);
			strcpy(dict[index].value, value);
            if (!dict[index].present)
            {
                dict[index].present = true;
                dict[index].link = -1;
                dict[index].children.clear();
            }
		}

        void remove_from_parents_children(int child)
        {
            int parent = dict[child].link;
            if (parent == -1) return;
            dict[parent].children.erase(std::remove(dict[parent].children.begin(), dict[parent].children.end(), child), dict[parent].children.end());
        }

		void del(int index) {
			::destroy(dict[index].name);
            if (!dict[index].present) return;

            dict[index].present = false;
            remove_from_parents_children(index);
            int parent = dict[index].link;
            for (auto it = dict[index].children.begin(); it != dict[index].children.end(); ++it)
            {
                dict[*it].link = parent;
                if (parent != -1)
                {
                    dict[parent].children.push_back(*it);
                }
            }
		}

        void link(int source, int target)
        {
            if (!dict[source].present) return;
            if (target != -1 && !dict[target].present) return;
            int current = target;
            while (current != -1)
            {
                if (current == source) return;
                current = dict[current].link;
            }

            remove_from_parents_children(source);
            if (target != -1)
            {
                dict[source].link = target;
                if (std::find(dict[target].children.begin(), dict[target].children.end(), source) == dict[target].children.end())
                {
                    dict[target].children.push_back(source);
                }
                ::link(dict[source].name, dict[target].name);
            }
            else
            {
                static char empty[20] = "";
                dict[source].link = -1;
                ::link(dict[source].name, empty);
            }
        }

		bool check(int index) {
            if (!dict[index].present) return true;

            char actual[96], expected[96];
            ::get_value(dict[index].name, actual);

            char* p = expected;
            for (int i = 0; i < 5 && index != -1 && dict[index].present; i++)
            {
                strcpy(p, dict[index].value);
                p = p + 19;
                index = dict[index].link;
            }

			bool result = strcmp(actual, expected) == 0;
            return result;
		}

		struct {
			char name[kWordSize];
			char value[kWordSize];
			bool present;
            int link;
            vector<int> children;
		} dict[kMaxWords];

        Rnd rnd;
    };

	Checker checker;
}

int main()
{
	int tests;
	(void)scanf("%d", &tests);

	for (int test = 0; test < tests; ++test) {
		int seed, words, rounds;
		(void)scanf("%d%d%d", &seed, &words, &rounds);
		printf("#%d %d\n", test + 1, checker.run(seed, words, rounds));
	}

	return 0;
}

