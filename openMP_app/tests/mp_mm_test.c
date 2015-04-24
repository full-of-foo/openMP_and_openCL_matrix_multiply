#include <miniunit.h>
#include <dbg.h>
#include "matrixutils.h"
#include "mp_mm.h"

char *test_good_demensions_1()
{
    float **A_p = (float *[]) { (float[]) {3,3,3}, (float[]) {3,3,3} };
    float **B_p = (float *[]) { (float[]) {3,3,3}, (float[]) {3,3,3} };
    float **C_p = matrix_multiply(A_p, B_p, 2, 3, 2, 3);
    mu_assert(C_p != NULL, "C_p should not be NULL");
    mu_assert(C_p[0][0] == 18.0, "C_p[0][0] should be 18.0");
    mu_assert(C_p[0][1] == 18.0, "C_p[0][1] should be 18.0");
    mu_assert(C_p[1][0] == 18.0, "C_p[1][0] should be 18.0");
    mu_assert(C_p[1][1] == 18.0, "C_p[1][1] should be 18.0");

    return NULL;
}

char *test_good_demensions_2()
{
    float **A_p = (float *[]) {(float[]) {2,2},
                               (float[]) {2,2}};
    float **B_p = (float *[]) {(float[]) {2,2},
                               (float[]) {2,2,2,2}};
    float **C_p = matrix_multiply(A_p, B_p, 2, 2, 2, 4);
    mu_assert(C_p != NULL, "C_p should not be NULL");
    mu_assert(C_p[0][0] == 8.0, "C_p[0][0] should be 8.0");
    mu_assert(C_p[0][1] == 8.0, "C_p[0][1] should be 8.0");
    mu_assert(C_p[1][0] == 8.0, "C_p[1][0] should be 8.0");
    mu_assert(C_p[1][1] == 8.0, "C_p[1][1] should be 8.0");

    return NULL;
}

char *test_bad_demensions_1()
{
    float **A_p = (float *[]) {(float[]) {4,4,4,4},
                               (float[]) {4,4,4,4}};
    float **B_p = (float *[]) {(float[]) {4,4,4,4,4,4,4,4},
                               (float[]) {4,4,4,4}};
    float **C_p = matrix_multiply(A_p, B_p, 4, 4, 8, 4);
    mu_assert(C_p == NULL, "C_p should be NULL");

    return NULL;
}

char *test_bad_demensions_2()
{
    float **A_p = (float *[]) {(float[]) {4,4,4,4},
                               (float[]) {4,4,4,4,4,4,4,4}};
    float **B_p = (float *[]) {(float[]) {4,4,4,4},
                               (float[]) {4,4,4,4}};
    float **C_p = matrix_multiply(A_p, B_p, 4, 8, 4, 4);
    mu_assert(C_p == NULL, "C_p should be NULL");

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
