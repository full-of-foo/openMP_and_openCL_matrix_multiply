#include <omp.h>
#include <dbg.h>
#include "matrixutils.h"


struct MATRIX *matrix_multiply(struct MATRIX *mA, struct MATRIX *mB)
{
    int isSquares = mA->rows == mB->rows && mA->cols == mB->cols;
    int isValidNonSquares = mA->cols == mB->rows;
    check(isSquares || isValidNonSquares, "Matrix demensions must agree");

    struct MATRIX *mC = matrix_create_scalar(mA->rows, mA->cols, 0);
    int i, j, k;

    // #pragma omp parallel for default(none) private(i,j,k) shared(mC->A,mB->A,mC->A,mA->rows,mA->cols,mB->rows)
    for(i=0; i<mA->rows; i++)
        for(j=0; j<mA->cols; j++)
            for(k=0; k<mB->rows; k++)
                mC->A[i][j] += (mA->A[i][k] * mB->A[k][j]);

    return mC;
error:
    return NULL;
}
