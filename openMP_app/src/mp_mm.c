#include <omp.h>
#include <stdio.h>
#include <dbg.h>

float **matrix_multiply(float **A_p, float **B_p, int aRows, int aCols, int bRows, int bCols)
{
    int isSquares = aRows == bRows && aCols == bCols;
    int isValidNonSquares = aCols == bRows;
    check(isSquares || isValidNonSquares, "Matrix demensions must agree");

    float **C_p = (float **) malloc(aRows*sizeof(float *));
    int i, j, k;
    for(i=0; i<aRows; i++){
        C_p[i] = (float *) malloc(aCols*sizeof(float));
        for(j=0; j<aCols; j++){
            float s = 0.0;
            for(k=0; k<bRows; k++){
                s += (A_p[i][k] * B_p[k][j]);
            }
            C_p[i][j] = s;
        }
    }

    return C_p;
error:
    return NULL;
}
