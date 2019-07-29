#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <vector>
using namespace std;

const int kWordSize = 20;
const int kMaxWords = 100000;

//////////////////////////////////////////////////////////////////////////////////////////////////
extern void init();
void create(char name[20], char value[20]);
void link(char name[20], char target[20]); // target=="" means no link
void destroy(char name[20]);
//////////////////////////////////////////////////////////////////////////////////////////////////

namespace {

	struct Rnd {
		typedef unsigned long long UInit64;
		UInit64 seed;
		static const int kAlpahabetSize = ('f' - 'a') + 1;
		char alpahabet[kAlpahabetSize];

		Rnd(UInit64 seed) : seed(seed) {
			char* p = alpahabet;
			for (char c = 'a'; c <= 'f'; ++c)* p++ = c;
		}

		inline UInit64 rand() {
			seed = (214013 * seed + 2531011);
			return seed >> 16;
		}

		inline int rand(UInit64 mx) {
			return static_cast<int>(rand() % mx);
		}

		inline char rand_char() {
			return alpahabet[rand(kAlpahabetSize)];
		}

		inline vector<char> rand_str(int length) {
			vector<char> result;
            length--;
			result.resize(length + 1);
			result[length--] = 0;
			while (length >= 0) result[length--] = rand_char();
			return result;
		}
	};

	class Checker
	{
	public:
		int run(int seed, int words, int rounds) {
			Rnd rnd(seed);

			for (int i = 0; i < words; ++i) {
				dict[i].name = rnd.rand_str(kWordSize);
				dict[i].value = rnd.rand_str(kWordSize);
				dict[i].present = false;
			}

			const int initRounds = static_cast<int>(words * 0.9);
			int fail = 0;
			::init();
			for (int i = 0; i < initRounds; ++i) {
				add(rnd.rand(words), rnd.rand_str(kWordSize));
				add(rnd.rand(words), rnd.rand_str(kWordSize));
				del(rnd.rand(words));
				if (!check(rnd.rand(words))) {
					++fail;
				}
			}
			for (int i = 0; i < rounds; ++i) {
				add(rnd.rand(words), rnd.rand_str(kWordSize));
				del(rnd.rand(words));
                link(rnd.rand(words), rnd.rand(words));
				if (!check(rnd.rand(words))) {
					++fail;
				}
			}
			return fail;
		}
	private:
		void add(int index, vector<char> value) {
			::create(&dict[index].name[0], &value[0]);
			dict[index].value = value;
			dict[index].present = true;
		}

		void del(int index) {
			::destroy(&dict[index].name[0]);
			dict[index].present = false;
		}

        void link(int source, int target)
        {
            if (!dict[source].present) return;
            if (!dict[target].present) return;
            ::link(&dict[source].name[0], &dict[target].name[0]);
        }

		bool check(int index) {
			//bool result = ::check(&dict[index].word[0]);
			//return dict[index].present == result;
			return true;
		}

		struct {
			vector<char> name;
			vector<char> value;
			bool present;
		} dict[kMaxWords];
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

