#include <miniunit.h>

char *test_a_test()
{
    mu_assert(1 == 1, "Should not fail");

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    test_a_test();

    return NULL;
}

RUN_TESTS(all_tests);
