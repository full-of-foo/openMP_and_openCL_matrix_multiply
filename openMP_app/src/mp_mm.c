#include <omp.h>
#include <stdio.h>
#include <dbg.h>

float **matrix_multiply(float **A_p, float **B_p, float **C_p, int aRows, int aCols, int bRows, int bCols)
{
    int isSquares = aRows == bRows && aCols == bCols;
    int isValidNonSquares = aCols == bRows;
    check(isSquares || isValidNonSquares, "Matrix demensions must agree");

    int i, j, k;
    float sum;

    #pragma omp parallel for shared(A_p,B_p,C_p,aRows,aCols,bRows) private(i,j,k, sum)
    for(i=0; i<aRows; i++){
        for(j=0; j<aCols; j++){
            sum = 0;
            #pragma omp parallel for reduction(+:sum)
            for(k=0; k<bRows; k++){
                sum += (A_p[i][k] * B_p[k][j]);
            }
            C_p[i][j] = sum;
        }
    }


    return C_p;
error:
    return NULL;
}
