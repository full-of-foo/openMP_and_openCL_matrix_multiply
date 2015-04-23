#include <omp.h>
#include <dbg.h>
#include <assert.h>
#include <stdlib.h>
#include "matrixutils.h"


struct MATRIX *matrix_multiply(struct MATRIX *mA, struct MATRIX *mB)
{
    int isSquares = mA->rows == mB->rows && mA->cols == mB->cols;
    int isValidNonSquares = mA->cols == mB->rows;
    check(isSquares || isValidNonSquares, "Matrix demensions must agree");

    struct MATRIX *mC = matrix_create(mA->rows, mA->cols, 0);
    int i, j, k;
    for(i=0; i<mA->rows; i++)
        for(j=0; j<mA->cols; j++)
            for(k=0; k<mB->rows; k++)
                mC->A[i][j] += (mA->A[i][k] * mB->A[k][j]);


    return mC;
error:
    return NULL;
}

int main (int argc, char *argv[])
{
    struct MATRIX *mA = matrix_create(2, 3, 3);
    struct MATRIX *mB = matrix_create(2, 3, 3);

    struct MATRIX *mC = matrix_multiply(mA, mB);
    check(mC != NULL, "...exiting");

    matrix_print(mC);
    matrix_destroy(mA);
    matrix_destroy(mB);
    matrix_destroy(mC);

    return 0;
error:
    return 1;
}
