#include <omp.h>
#include <stdio.h>
#include <dbg.h>

float **matrix_multiply(float **A_p, float **B_p, float **C_p, int aRows, int aCols, int bRows, int bCols)
{
    int isSquares = aRows == bRows && aCols == bCols;
    int isValidNonSquares = aCols == bRows;
    check(isSquares || isValidNonSquares, "Matrix demensions must agree");

    int i, j, k;
    for(i=0; i<aRows; i++)
        for(j=0; j<aCols; j++)
            for(k=0; k<bRows; k++)
                C_p[i][j] += (A_p[i][k] * B_p[k][j]);


    return C_p;
error:
    return NULL;
}
