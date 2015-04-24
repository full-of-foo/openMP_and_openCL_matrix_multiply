#include <miniunit.h>
#include <dbg.h>
#include "matrixutils.h"
#include "mp_mm.h"

char *test_good_demensions_1()
{
    struct MATRIX *mA = matrix_create_scalar(2, 3, 3);
    struct MATRIX *mB = matrix_create_scalar(2, 3, 3);
    struct MATRIX *mC = matrix_create_scalar(2, 3, 0);
    float **C_p = matrix_multiply(mA->A, mB->A, mC->A, 2, 3, 2, 3);

    mu_assert(C_p != NULL, "C_p should not be NULL");
    mu_assert(C_p[0][0] == 18.0, "C_p[0][0] should be 18.0");
    mu_assert(C_p[0][1] == 18.0, "C_p[0][1] should be 18.0");
    mu_assert(C_p[1][0] == 18.0, "C_p[1][0] should be 18.0");
    mu_assert(C_p[1][1] == 18.0, "C_p[1][1] should be 18.0");

    matrix_destroy(mA);
    matrix_destroy(mB);
    matrix_destroy(mC);
    return NULL;
}

char *test_good_demensions_2()
{
    struct MATRIX *mA = matrix_create_scalar(2, 2, 4);
    struct MATRIX *mB = matrix_create_scalar(2, 4, 4);
    struct MATRIX *mC = matrix_create_scalar(2, 2, 0);
    float **C_p = matrix_multiply(mA->A, mB->A, mC->A, 2, 2, 2, 4);

    mu_assert(C_p != NULL, "C_p should not be NULL");
    mu_assert(C_p[0][0] == 32.0, "C_p[0][0] should be 32.0");
    mu_assert(C_p[0][1] == 32.0, "C_p[0][1] should be 32.0");
    mu_assert(C_p[1][0] == 32.0, "C_p[1][0] should be 32.0");
    mu_assert(C_p[1][1] == 32.0, "C_p[1][1] should be 32.0");

    matrix_destroy(mA);
    matrix_destroy(mB);
    matrix_destroy(mC);
    return NULL;
}

char *test_bad_demensions_1()
{
    struct MATRIX *mA = matrix_create_scalar(4, 4, 4);
    struct MATRIX *mB = matrix_create_scalar(8, 4, 4);
    struct MATRIX *mC = matrix_create_scalar(4, 4, 0);
    float **C_p = matrix_multiply(mA->A, mB->A, mC->A, 4, 4, 8, 4);

    mu_assert(C_p == NULL, "C_p should be NULL");

    matrix_destroy(mA);
    matrix_destroy(mB);
    matrix_destroy(mC);
    return NULL;
}

char *test_bad_demensions_2()
{
    struct MATRIX *mA = matrix_create_scalar(4, 8, 4);
    struct MATRIX *mB = matrix_create_scalar(4, 4, 4);
    struct MATRIX *mC = matrix_create_scalar(4, 8, 0);
    float **C_p = matrix_multiply(mA->A, mB->A, mC->A, 4, 4, 8, 4);

    mu_assert(C_p == NULL, "C_p should be NULL");

    matrix_destroy(mA);
    matrix_destroy(mB);
    matrix_destroy(mC);
    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_good_demensions_1);
    mu_run_test(test_good_demensions_2);
    mu_run_test(test_bad_demensions_1);
    mu_run_test(test_bad_demensions_2);

    return NULL;
}

RUN_TESTS(all_tests);
