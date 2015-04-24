#ifndef _matrixutils_h
#define _matrixutils_h

struct MATRIX
{
    int rows;
    int cols;
    int initialized;
    float **A;
};

struct MATRIX *matrix_create(float **A_p);

struct MATRIX *matrix_create_scalar(int m, int n, float s);

void matrix_destroy(struct MATRIX *matrix);

void matrix_print(struct MATRIX *matrix);

#endif
