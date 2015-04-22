#include <omp.h>
#include <assert.h>
#include <dbg.h>
#include <stdio.h>
#include "matrixutils.h"

struct MATRIX matrix_create(int rows, int cols, int s)
{
    struct MATRIX matrix;
    matrix.rows = rows;
    matrix.cols = cols;
    matrix.A = (int *) malloc(sizeof(int)*rows*cols);

    unsigned int i,j;
#pragma omp parallel private(i,j)
{
#pragma omp for schedule(static)
    for(i=0; i<rows; i++) {
        for(j=0; j<cols; j++)
            matrix.A[i + j * matrix.cols] = s;
    }
}
    return matrix;
}

void matrix_destroy(struct MATRIX matrix)
{
    free(matrix.A);
}

int cell_at(struct MATRIX matrix, int i, int j)
{
    return matrix.A[i + j * matrix.cols];
}

void matrix_print(struct MATRIX matrix)
{
    assert(matrix != NULL);

    log_info("rows: %d", matrix.rows);
    log_info("cols: %d", matrix.cols);

    unsigned int i,j;
    for(i=0; i<matrix.rows; i++) {
        printf("| ");
        for(j=0; j<matrix.cols; j++)
          printf("%d ", cell_at(matrix,j,i));
        printf("|\n");
    }
}
