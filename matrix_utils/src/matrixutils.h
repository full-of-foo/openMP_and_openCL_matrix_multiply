#ifndef _matrixutils_h
#define _matrixutils_h

struct MATRIX
{
    int rows;
    int cols;
    int *A;
};

struct MATRIX matrix_create(int m, int n, int s);

void matrix_destroy(struct MATRIX matrix);

int cell_at(struct MATRIX matrix, int i, int j);

void matrix_print(struct MATRIX matrix);

#endif
