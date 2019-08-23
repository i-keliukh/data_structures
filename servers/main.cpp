#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cctype>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////////////////////////
void init();
void create(char name[20], char value[20]);
void destroy(char name[20]);
void link(char source[20], char target[20]);
void unlink(char name[20]);
void get_value(char name[20], char result[96]);
//////////////////////////////////////////////////////////////////////////////////////////////////

const int command_length = 10;
const int name_length = 19;
const int value_length = 19;
const int result_length = 5 * value_length;

int test_run()
{
    char name[name_length + 1];
    int number_of_commands;
    int fail_count = 0;
    (void)scanf("%d", &number_of_commands);
    ::init();
    for (int i = 0; i < number_of_commands; i++)
    {
        char command[command_length + 1];
        (void)scanf("%s", command);
        switch (toupper(command[0]))
        {
            case 'C':
            {
                char value[value_length + 1];
                (void)scanf("%s %s", name, value);
                ::create(name, value);
                break;
            }
            case 'D':
            {
                (void)scanf("%s", name);
                ::destroy(name);
                break;
            }
            case 'G':
            {
                char expected_result[result_length + 1];
                char actual_result[result_length + 1];
                (void)scanf("%s %s", name, expected_result);
                ::get_value(name, actual_result);
                if (strcmp(expected_result, actual_result) != 0) 
                    fail_count++;
                break;
            }
            case 'L':
            {
                char source[name_length + 1];
                char target[name_length + 1];
                (void)scanf("%s %s", source, target);
                ::link(source, target);
                break;
            }
            case 'U':
            {
                (void)scanf("%s", name);
                ::unlink(name);
                break;
            }
        }
    }
    return fail_count == 0;
}


struct ServerEntry
{
    char value[20];
    int link;
    int next_with_same_link;
    int prev_with_same_link;
    int first_incoming_link;
};

struct HashEntry
{
    char name[20];
    int next;
    ServerEntry data;
};

int main()
{
    int tests;
    //freopen("input.txt", "r", stdin);
    (void)scanf("%d", &tests);

    for (int test = 0; test < tests; ++test) {
        printf("#%d %d\n", test + 1, test_run() ? 100 : 0);
    }

    return 0;
}
