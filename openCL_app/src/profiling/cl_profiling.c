#include <stdio.h>
#include <stdlib.h>

static const int RUNS_PER_BENCHMARK = 5;

char *get_benchmark(int a_rows, int a_cols, int b_rows, int b_cols)
{
    char output[250], cmd[250];

    sprintf(cmd, "openCL_app/src/cl_mm %d %d %d %d", a_rows, a_cols, b_rows, b_cols);
    FILE* fp = popen(cmd, "r");
    fgets(output, 250, fp);

    return output;
}

int main(int argc, char *argv[])
{
    unsigned int i,j,data_row_count;
    char row_title[250];
    float demensions[4][2] = { { 200, 200 }, { 200, 300 },{ 1000, 1000 }, { 1000, 1200 } };
    data_row_count = (sizeof(demensions)/sizeof(demensions[0]))+1;
    char data[data_row_count][RUNS_PER_BENCHMARK+2][250];

    get_benchmark(200, 200, 200, 200);
    // collate data
    sprintf(data[0][0], "");
    for (i=0; i<data_row_count-1; i++){
        sprintf(row_title, "%.fx%.f", demensions[i][0],demensions[i][1]);
        sprintf(data[i+1][0], row_title);
        for (j=0; j<RUNS_PER_BENCHMARK+1; j++){
            sprintf(data[0][j+1], "%d", j+1);
            sprintf(data[i+1][j+1], "%s", get_benchmark(demensions[i][0],demensions[i][1],
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
