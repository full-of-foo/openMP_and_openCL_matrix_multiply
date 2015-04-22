#include <miniunit.h>
#include "matrixutils.h"

char *test_create()
{
    struct MATRIX matrix;
    matrix = matrix_create(2, 2, 3);

    mu_assert(&matrix_create, "matrix_create should be true");
    mu_assert(&matrix, "matrix instance should not be NULL");

    mu_assert(matrix.rows == 2, "matrix rows should be 2");
    mu_assert(sizeof(matrix.A)/sizeof(int) == 2, "matrix rows should be 2");
    mu_assert(matrix.cols == 2, "matrix cols should be 2");
    mu_assert(sizeof(matrix.A[0])/sizeof(int) == 2, "matrix cols should be 2");

    matrix_destroy(matrix);
    return NULL;
}

char *test_cell_at()
{
    struct MATRIX matrix;
    matrix = matrix_create(2, 2, 4);

    mu_assert(&cell_at, "cell_at should be true");
    mu_assert(cell_at(matrix,0,0) == 4, "[0][0] should be 4");
    mu_assert(cell_at(matrix,0,1) == 4, "[0][1] should be 4");
    mu_assert(cell_at(matrix,1,0) == 4, "[1][0] should be 4");
    mu_assert(cell_at(matrix,1,1) == 4, "[1][1] should be 4");

    matrix_destroy(matrix);
    return NULL;
}


char *all_tests() {
    mu_suite_start();

    test_create();
    test_cell_at();

    return NULL;
}

RUN_TESTS(all_tests);
