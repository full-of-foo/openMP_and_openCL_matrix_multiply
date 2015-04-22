#include "matrixutils.h"

int main (int argc, char *argv[])
{
    struct MATRIX matrix;
    matrix = matrix_create(2, 2, 3);
    matrix_print(matrix);
    matrix_destroy(matrix);
}
