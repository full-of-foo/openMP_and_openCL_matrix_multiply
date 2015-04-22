#include <stdio.h>
#include "libmatrixutils.h"

int main (int argc, char *argv[])
{
    struct Matrix *matrix = Matrix_create(2, 2, 3);
    Matrix_print(matrix);
    Matrix_destroy(matrix);
}
