#include <miniunit.h>

char *test_matrix()
{
    struct Matrix *matrix = Matrix_create(2, 2, 3);
    mu_assert(matrix != NULL, "matrix should not be NULL");

    Matrix_destroy(matrix);

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    test_matrix();

    return NULL;
}

RUN_TESTS(all_tests);
