#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <vector>
using namespace std;

const int kWordSize = 31;
const int kMaxWords = 100000;

//////////////////////////////////////////////////////////////////////////////////////////////////
extern void init();
extern void add(char* word);
extern void del(char* word);
extern bool check(char* word);
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
            length = rand(length - 16) + 16;
            result.resize(length + 1);
            result[length--] = 0;
            while (length>=0) result[length--] = rand_char();
            return result;
        }
    };

    class Checker
    {
    public:
        int run(int seed, int words, int rounds) {
            Rnd rnd(seed);

            for (int i = 0; i < words; ++i) {
                dict[i].word = rnd.rand_str(kWordSize);
                dict[i].present = false;
            }

            const int initRounds = static_cast<int>(words * 0.9);
            int fail = 0;
            ::init();
            for (int i = 0; i < initRounds ; ++i) {
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
                if (!check(rnd.rand(words))) {
                    ++fail;
                }
            }
            return fail;
        }
    private:
        void add(int index) {
            ::add(&dict[index].word[0]);
            dict[index].present = true;
        }

        void del(int index) {
            ::del(&dict[index].word[0]);
            dict[index].present = false;
        }

        bool check(int index) {
            bool result = ::check(&dict[index].word[0]);
            return dict[index].present == result;
        }

        struct {
            vector<char> word;
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

