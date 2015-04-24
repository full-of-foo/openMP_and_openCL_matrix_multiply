#ifndef _matrixutils_h
#define _matrixutils_h

struct MATRIX
{
    unsigned int rows;
    unsigned int cols;
    float **A;
};

struct MATRIX *matrix_create(float **A_p);

struct MATRIX *matrix_create_scalar(unsigned int m, unsigned int n, float s);

void matrix_destroy(struct MATRIX *matrix);

void matrix_print(struct MATRIX *matrix);

float **get_empty_C_p(int aRows, int aCols);

#endif
