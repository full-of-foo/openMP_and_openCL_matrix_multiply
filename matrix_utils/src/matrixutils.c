#include <omp.h>
#include <dbg.h>
#include <stdio.h>
#include "matrixutils.h"

struct MATRIX *matrix_create(int rows, int cols, float s)
{
    struct MATRIX *matrix = malloc(sizeof(struct MATRIX));;
    matrix->rows = rows;
    matrix->cols = cols;
    matrix->A = malloc(rows * sizeof(float *));

    int i, j;
    #pragma omp parallel private(i, j)
    {
        #pragma omp for schedule(static)
        for(i=0; i<rows; i++) {
            matrix->A[i] = malloc(cols * sizeof(int));
            for(j=0; j<cols; j++)
                matrix->A[i][j] = s;
        }
    }

    return matrix;
}

void matrix_destroy(struct MATRIX *matrix)
{
    free(matrix->A);
    free(matrix);
}

void matrix_print(struct MATRIX *matrix)
{
    log_info("rows: %d", matrix->rows);
    log_info("cols: %d", matrix->cols);

    int i,j;
    for(i=0; i<matrix->rows; i++) {
        printf("| ");
        for(j=0; j<matrix->cols; j++)
          printf("%f ", matrix->A[i][j]);
        printf("|\n");
    }
}
