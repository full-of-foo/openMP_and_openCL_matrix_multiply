#include <miniunit.h>
#include "matrixutils.h"

char *test_create()
{
    float A[2][2] = { {3, 3}, {3, 3} };
    int i, *A_p[2];
    for (i=0; i < 2; i++) A_p[i] = A[i];
    struct MATRIX *matrix = matrix_create(A_p);

    mu_assert(&matrix_create, "matrix_create_scalar should be present");
    mu_assert(matrix != NULL, "matrix instance should not be NULL");

    mu_assert(matrix->rows == 2, "matrix rows should be 2");
    mu_assert(sizeof(matrix->A)/sizeof(matrix->A[0][0]) == 2, "matrix rows should be 2");
    mu_assert(matrix->cols == 2, "matrix cols should be 2");
    mu_assert(sizeof(matrix->A[0])/sizeof(matrix->A[0][0]) == 2, "matrix cols should be 2");

    matrix_destroy(matrix);
    return NULL;
}

char *test_create_scalar()
{
    struct MATRIX *matrix = matrix_create_scalar(2, 2, 3);

    mu_assert(&matrix_create_scalar, "matrix_create_scalar should be present");
    mu_assert(matrix != NULL, "matrix instance should not be NULL");

    mu_assert(matrix->rows == 2, "matrix rows should be 2");
    mu_assert(sizeof(matrix->A)/sizeof(matrix->A[0][0]) == 2, "matrix rows should be 2");
    mu_assert(matrix->cols == 2, "matrix cols should be 2");
    mu_assert(sizeof(matrix->A)/sizeof(matrix->A[0][0]) == 2, "matrix cols should be 2");

    matrix_destroy(matrix);
    return NULL;
}

char *test_cell_at()
{
    float A[2][2] = { {3, 3}, {3, 3} };
    int i, *A_p[2];
    for (i=0; i < 2; i++) A_p[i] = A[i];
    struct MATRIX *matrix = matrix_create(A_p);

    mu_assert(matrix->A[0][0] == 3, "[0][0] should be 3");
    mu_assert(matrix->A[0][1] == 3, "[0][1] should be 3");
    mu_assert(matrix->A[1][0] == 3, "[1][0] should be 3");
    mu_assert(matrix->A[1][1] == 3, "[1][1] should be 3");

    matrix_destroy(matrix);
    return NULL;
}

char *test_cell_at_scalar()
{
    struct MATRIX *matrix = matrix_create_scalar(2, 2, 4);

    mu_assert(matrix->A[0][0] == 4, "[0][0] should be 4");
    mu_assert(matrix->A[0][1] == 4, "[0][1] should be 4");
    mu_assert(matrix->A[1][0] == 4, "[1][0] should be 4");
    mu_assert(matrix->A[1][1] == 4, "[1][1] should be 4");

    matrix_destroy(matrix);
    return NULL;
}

char *test_matrix_destroy()
{
    struct MATRIX *matrix = matrix_create_scalar(2, 2, 4);
    mu_assert(matrix != NULL, "matrix instance should not be NULL");
    mu_assert(matrix->A != NULL, "A instance should not be NULL");

    matrix_destroy(matrix);
    mu_assert(matrix->A == NULL, "A instance should be NULL");
    mu_assert(matrix != NULL, "matrix instance should not be NULL (cannot really 'free' structs)");
    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_create_scalar);
    mu_run_test(test_cell_at);
    mu_run_test(test_cell_at_scalar);
    mu_run_test(test_matrix_destroy);

    return NULL;
}

RUN_TESTS(all_tests);
