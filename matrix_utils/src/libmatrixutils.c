#include <assert.h>
#include <dbg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libmatrixutils.h"

struct Matrix {
    int m; // row demension
    int n; // col demension
    // int **A_p;
};

struct Matrix *Matrix_create(int m, int n, int s)
{
    struct Matrix *matrix = malloc(sizeof(struct Matrix));
    assert(matrix != NULL);

    (*matrix).m = m;
    (*matrix).n = n;
    // (*matrix).A_p[m][n] = (int *[]) {(int[]) {0}};

    return matrix;
}

void Matrix_destroy(struct Matrix *matrix)
{
    assert(matrix != NULL);
    free(matrix);
}

void Matrix_print(struct Matrix *matrix)
{
    assert(matrix != NULL);

    printf("m: %d\n", (*matrix).m);
    printf("n: %d\n", (*matrix).n);
    // log_info("A_p: %p\n", (*matrix).A_p);

    // unsigned int m, n;
    // for (m=0; m<matrix->m; m++)
    // {
    //     for(n=0; n<matrix->n; n++)
    //     {
    //         printf("%d ", &matrix->A_p[m][n]);
    //         printf("\n");
    //     }
    //  }
}

// int main (int argc, char *argv[])
// {
//     struct Matrix *matrix = Matrix_create(2, 2, 3);
//     Matrix_print(matrix);
//     Matrix_destroy(matrix);
// }
