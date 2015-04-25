#include <omp.h>
#include <stdio.h>
#include <dbg.h>

float **matrix_multiply(float **A_p, float **B_p, float **C_p, int aRows, int aCols, int bRows, int bCols)
{
    int isSquares = aRows == bRows && aCols == bCols;
    int isValidNonSquares = aCols == bRows;
    check(isSquares || isValidNonSquares, "Matrix demensions must agree");

    int i, j, k, i2, k2, j2, s, s2, s3;
    s = (int) ceil((aRows * 1.0)/30);
    s2 = (int) ceil((aCols * 1.0)/30);
    s3 = (int) ceil((bRows * 1.0)/30);

    #pragma omp parallel for shared(A_p,B_p,C_p,aRows,aCols,bRows,bCols) private(i,j,k,i2,k2,j2)
    for(i=0; i<aRows/s; i+=s){
        for(j=0; j<aCols/s2; j+=s2){
            for(k=0; k<bRows/s3; k+=s3){
                for(i2=i; i2<(i+s) && i2<aRows; i2++){
                    for(j2=j; j2<(j+s2) && j2<aCols; j2++){
                        for(k2=k; k2<(k+s3) && k2<bRows; k2++){
                            C_p[i2][j2] += (A_p[i2][k2] * B_p[k2][j2]);
                        }
                    }
                }
            }
        }
    }

    return C_p;
error:
    return NULL;
}
