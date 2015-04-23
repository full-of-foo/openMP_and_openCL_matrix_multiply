#include <miniunit.h>
#include <dbg.h>
#include "matrixutils.h"
#include "openmp_app.h"

char *test_good_demensions_1()
{
    struct MATRIX *mA = matrix_create(2, 3, 3);
    struct MATRIX *mB = matrix_create(2, 3, 3);
    mu_assert(mA != NULL, "mA instance should not be NULL");
    mu_assert(mB != NULL, "mB instance should not be NULL");

    struct MATRIX *mC = matrix_multiply(mA, mB);
    mu_assert(mC != NULL, "mC instance should not be NULL");

    mu_assert(mC->A[0][0] == 18.0, "mC[0][0] should be 18.0");
    mu_assert(mC->A[0][1] == 18.0, "mC[0][1] should be 18.0");
    mu_assert(mC->A[1][0] == 18.0, "mC[1][0] should be 18.0");
    mu_assert(mC->A[1][1] == 18.0, "mC[1][1] should be 18.0");

    matrix_destroy(mA);
    matrix_destroy(mB);
    matrix_destroy(mC);
    return NULL;
}

char *test_good_demensions_2()
{
    struct MATRIX *mA = matrix_create(2, 2, 2);
    struct MATRIX *mB = matrix_create(2, 4, 2);
    mu_assert(mA != NULL, "mA instance should not be NULL");
    mu_assert(mB != NULL, "mB instance should not be NULL");

    struct MATRIX *mC = matrix_multiply(mA, mB);
    mu_assert(mC != NULL, "mC instance should not be NULL");

    mu_assert(mC->A[0][0] == 8.0, "mC[0][0] should be 8.0");
    mu_assert(mC->A[0][1] == 8.0, "mC[0][1] should be 8.0");
    mu_assert(mC->A[1][0] == 8.0, "mC[1][0] should be 8.0");
    mu_assert(mC->A[1][1] == 8.0, "mC[1][1] should be 8.0");

    matrix_destroy(mA);
    matrix_destroy(mB);
    matrix_destroy(mC);
    return NULL;
}

char *test_bad_demensions_1()
{
    struct MATRIX *mA = matrix_create(4, 4, 3);
    struct MATRIX *mB = matrix_create(8, 4, 3);
    mu_assert(mA != NULL, "mA instance should not be NULL");
    mu_assert(mB != NULL, "mB instance should not be NULL");

    struct MATRIX *mC = matrix_multiply(mA, mB);
    mu_assert(mC == NULL, "mC instance should be NULL");

    matrix_destroy(mA);
    matrix_destroy(mB);
    return NULL;
}

char *test_bad_demensions_2()
{
    struct MATRIX *mA = matrix_create(4, 8, 3);
    struct MATRIX *mB = matrix_create(4, 4, 3);
    mu_assert(mA != NULL, "mA instance should not be NULL");
    mu_assert(mB != NULL, "mB instance should not be NULL");

    struct MATRIX *mC = matrix_multiply(mA, mB);
    mu_assert(mC == NULL, "mC instance should be NULL");

    matrix_destroy(mA);
    matrix_destroy(mB);
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
