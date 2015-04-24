#include <sys/time.h>
#include <time.h>
#include "matrixutils.h"
#include "mp_mm.h"

static const int RUNS_PER_BENCHMARK = 5;

typedef unsigned long long timestamp_t;

static timestamp_t get_timestamp()
{
  struct timeval now;
  gettimeofday(&now, NULL);

  return now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

float get_benchmark_time(unsigned int aRows, unsigned int aCols, unsigned int bRows, unsigned int bCols)
{
    struct MATRIX *mA = matrix_create_scalar(aRows, aCols, 3);
    struct MATRIX *mB = matrix_create_scalar(bRows, bCols, 3);

    timestamp_t t0 = get_timestamp();
    matrix_multiply(mA->A, mB->A, aRows, aCols, bRows, bCols);
    timestamp_t t1 = get_timestamp();

    matrix_destroy(mA);
    matrix_destroy(mB);
    return ((t1 - t0) / 1000000.0L);
}

int main(int argc, char *argv[])
{
    unsigned int i,j,data_row_count;
    char row_title[5];
    float demensions[4][2] = { { 200, 200 }, { 200, 300 },{ 1000, 1000 }, { 1000, 1200 } };
    data_row_count = (sizeof(demensions)/sizeof(demensions[0]))+1;
    char data[data_row_count][RUNS_PER_BENCHMARK+2][250];

    // collate data
    sprintf(data[0][0], "");
    for (i=0; i<data_row_count-1; i++){
        sprintf(row_title, "%.fx%.f", demensions[i][0],demensions[i][1]);
        sprintf(data[i+1][0], row_title);
        for (j=0; j<RUNS_PER_BENCHMARK+1; j++){
            sprintf(data[0][j+1], "%d", j+1);
            sprintf(data[i+1][j+1], "%g", get_benchmark_time(demensions[i][0],demensions[i][1],
                                                             demensions[i][0],demensions[i][1]));
        }
    }
    // print CSV data
    for(i=0; i<data_row_count; i++) {
        for(j=0; j<RUNS_PER_BENCHMARK+1; j++){
          printf("%s", data[i][j]);
          if(j<RUNS_PER_BENCHMARK) printf(",");
        }
        printf("\n");
    }

    return 0;
}
