#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>

const int kWordSize = 63;
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
        static const int kAlpahabetSize = ('Z' - 'A') + ('z' - 'a') + ('9' - '0') + 3;
        char alpahabet[kAlpahabetSize];

        Rnd(UInit64 seed) : seed(seed) {
            char* p = alpahabet;
            for (char c = 'A'; c <= 'Z'; ++c)* p++ = c;
            for (char c = 'a'; c <= 'z'; ++c)* p++ = c;
            for (char c = '0'; c <= '9'; ++c)* p++ = c;
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

        inline void rand_str(int length, char* ptr) {
            char* end = ptr + length;
            while (ptr != end)* ptr++ = rand_char();
            *ptr = 0;
        }
    };

    class Checker
    {
    public:
        int run(int seed, int words, int rounds) {
            Rnd rnd(seed);

            for (int i = 0; i < words; ++i) {

                rnd.rand_str(kWordSize, dict[i].word);
                dict[i].present = false;
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
                if (!check(rnd.rand(words))) {
                    ++fail;
                }
            }
            return fail;
        }
    private:
        void copyKey(int index) {
            for (int i = 0; i <= kWordSize; ++i) {
                key[i] = dict[index].word[i];
            }
        }
        void add(int index) {
            copyKey(index);
            ::add(dict[index].word);
            dict[index].present = true;
        }

        void del(int index) {
            copyKey(index);
            ::del(dict[index].word);
            dict[index].present = false;
        }

        bool check(int index) {
            copyKey(index);
            bool result = ::check(dict[index].word);
            return dict[index].present == result;
        }

        char key[kWordSize + 1];
        struct {
            char word[kWordSize + 1];
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

