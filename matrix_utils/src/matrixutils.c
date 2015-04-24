#include <omp.h>
#include <dbg.h>
#include <stdio.h>
#include "matrixutils.h"

struct MATRIX *matrix_create(float **A_p)
{
    struct MATRIX *matrix = malloc(sizeof(struct MATRIX));
    matrix->rows = (sizeof(A_p)/sizeof(A_p[0]))+1;
    matrix->cols = (sizeof(A_p[0])/sizeof(A_p[0][0]))+1;

    matrix->A = (float **) malloc(matrix->rows*sizeof(float *));

    int i,j;
    for(i=0; i<matrix->rows; i++){
        matrix->A[i] = (float *) malloc(matrix->cols*sizeof(float));
        for (j=0; j<matrix->cols; j++){
            matrix->A[i][j] = A_p[i][j];
        }
    }

    return matrix;
}

struct MATRIX *matrix_create_scalar(unsigned int rows, unsigned int cols, float s)
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
            matrix->A[i] = malloc(cols * sizeof(float));
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

    matrix->A = NULL;
    memset(&matrix, 0, sizeof(sizeof(struct MATRIX)));
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
