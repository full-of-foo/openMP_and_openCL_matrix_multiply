#include <miniunit.h>
#include "matrixutils.h"

char *test_create()
{
    float **A_p = (float *[]) { (float[]) {3,3,3}, (float[]) {3,3,3} };
    struct MATRIX *matrix = matrix_create(A_p);

    mu_assert(matrix != NULL, "matrix instance should not be NULL");
    mu_assert(matrix->rows == 2, "matrix rows should be 2");
    mu_assert(matrix->cols == 3, "matrix cols should be 3");

    matrix_destroy(matrix);
    return NULL;
}

char *test_create_scalar()
{
    struct MATRIX *matrix = matrix_create_scalar(2, 2, 3);

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
    float **A_p = (float *[]) { (float[]) {4,4,4}, (float[]) {4,4,4} };
    struct MATRIX *matrix = matrix_create(A_p);

    mu_assert(matrix->A[0][0] == 4, "[0][0] should be 4");
    mu_assert(matrix->A[0][1] == 4, "[0][1] should be 4");
    mu_assert(matrix->A[1][0] == 4, "[1][0] should be 4");
    mu_assert(matrix->A[1][1] == 4, "[1][1] should be 4");

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
