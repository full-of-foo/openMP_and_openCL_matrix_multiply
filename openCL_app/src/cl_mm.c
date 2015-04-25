#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dbg.h>
#include <OpenCL/opencl.h>


const char *kernel_src_name = "openCL_app/src/mm.cl";

cl_float *a_data;
cl_float *b_data;
cl_float *c_data;

cl_uint num_devs_returned;
cl_context_properties properties[3];
cl_device_id device_id;
cl_int res;
cl_platform_id platform_id;
cl_uint num_platforms_returned;
cl_context context;
cl_command_queue command_queue;
cl_program program;
cl_kernel kernel;
cl_mem A_mem, B_mem, C_mem;
size_t global_sizes[2];
size_t local_sizes[2];

FILE *fp;
long file_len;
long read_len;
char *kernel_src;


void malloc_matrices(cl_int a_rows, cl_int a_cols, cl_int b_rows, cl_int b_cols, int scalar)
{
    int i,j;
    a_data = malloc(sizeof(cl_float)*(a_rows*a_cols));
    b_data = malloc(sizeof(cl_float)*(b_rows*b_cols));
    c_data = malloc(sizeof(cl_float)*(a_rows*a_cols));

    for(i=0; i<a_rows; i++) {
        for(j=0; j<a_cols; j++) {
            a_data[i*a_rows+j] = scalar;
            b_data[i*b_rows+j] = scalar;
            c_data[i*a_rows+j] = 0;
        }
    }
}

void print_matrices(cl_int a_rows, cl_int b_rows)
{
    int i,j;
    debug("Matrix A (10x10)");
    for (i=0; i<10; i++) {
        for (j=0; j<10; j++)
            printf("%.f ", a_data[i*a_rows+j]);
        printf("\n");
    }
    debug("Matrix B (10x10)");
    for (i=0; i<10; i++) {
        for (j=0; j<10; j++)
            printf("%.f ", b_data[i*b_rows+j]);
        printf("\n");
    }
    debug("Matrix C (10x10)");
    for (i=0; i<10; i++) {
        for (j=0; j<10; j++)
            printf("%.f ", c_data[i*a_rows+j]);
        printf("\n");
    }
}

void cleanup()
{
    clReleaseMemObject(A_mem);
    clReleaseMemObject(B_mem);
    clReleaseMemObject(C_mem);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);
    free(kernel_src);
    free(a_data);
    free(b_data);
    free(c_data);
}

void run_data_parallel_multiplication(cl_int a_rows, cl_int b_rows)
{
    debug("1: Opening kernel file: %s", kernel_src_name);
    fp = fopen(kernel_src_name,"r");
    fseek(fp,0L, SEEK_END);
    file_len = ftell(fp);
    rewind(fp);

    debug("2: Reading kernel_src into kernel source");
    kernel_src = malloc(sizeof(char)*(file_len+1));
    read_len = fread(kernel_src,1,file_len,fp);
    kernel_src[file_len+1]='\0';
    if(read_len != file_len) exit(1);

    debug("3: Getting platform IDs");
    res = clGetPlatformIDs(1, &platform_id, &num_platforms_returned);
    if (res != CL_SUCCESS) exit(1);

    debug("4: Getting device ID (CL_DEVICE_TYPE_GPU)");
    res = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &num_devs_returned);
    if (res != CL_SUCCESS) exit(1);

    properties[0] = CL_CONTEXT_PLATFORM;
    properties[1] = (cl_context_properties) platform_id;
    properties[2] = 0;
    debug("5: Creating cntx");
    context = clCreateContext(properties, 1, &device_id, NULL, NULL, &res);
    if (res != CL_SUCCESS) exit(1);

    debug("6: Creating command queue with profiling enabled");
    command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &res);
    if (res != CL_SUCCESS) exit(1);

    debug("7: Create program with kernel source");
    program = clCreateProgramWithSource(context, 1, (const char **) &kernel_src, NULL, &res);
    if (res != CL_SUCCESS) exit(1);

    debug("8: Building program");
    res = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    if (res != CL_SUCCESS) exit(1);

    debug("9: Creating kernel");
    kernel = clCreateKernel(program, "cl_mm", &res);
    if (res != CL_SUCCESS) exit(1);

    debug("10: Creating matrix buffers");
    A_mem  = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, sizeof(cl_float)*(a_rows*b_rows), a_data, NULL);
    B_mem  = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, sizeof(cl_float)*(a_rows*b_rows), b_data, NULL);
    C_mem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float)*(a_rows*b_rows), NULL, NULL);

    debug("11: Setting kernel arguments");
    res  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &A_mem);
    res |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &B_mem);
    res |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &C_mem);
    res |= clSetKernelArg(kernel, 3, sizeof(cl_int), &a_rows);
    res |= clSetKernelArg(kernel, 4, sizeof(cl_int), &b_rows);
    if (res != CL_SUCCESS) exit(1);

    global_sizes[0] = a_rows;
    global_sizes[1] = b_rows;
    local_sizes[0] = 8;
    local_sizes[1] = 8;
    debug("12: Execute kernel with event for profiling");
    cl_event event;
    res = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_sizes, local_sizes, 0, NULL, &event);
    if (res != CL_SUCCESS) exit(1);
    clWaitForEvents(1, &event); // wait on host thread for commands by event objects to complete

    debug("13: Getting profiling information");
    cl_ulong time_start = (cl_ulong) 0;
    cl_ulong time_end = (cl_ulong) 0;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &time_start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &time_end, NULL);

    double nseconds = (time_end - time_start);
    debug("Execution time in: %.6f seconds", nseconds/1000000000.0f);
    printf("%.6f", nseconds/1000000000.0f);

    debug("14: Transfering data to host");
    res = clEnqueueReadBuffer(command_queue, C_mem, CL_TRUE, 0, sizeof(cl_float)*(a_rows*b_rows),c_data, 0, NULL, NULL);
    clFinish(command_queue);
    clFlush(command_queue);
    cleanup();
}

int main(int argc, char** argv)
{
    if(argc != 5) return 1;

    cl_int a_rows = (cl_int)atoi(argv[1]), a_cols = (cl_int)atoi(argv[2]);
    cl_int b_rows = (cl_int)atoi(argv[3]), b_cols = (cl_int)atoi(argv[4]);
    int scalar_value = 3;

    malloc_matrices(a_rows, a_cols, b_rows, b_cols, scalar_value);
    debug("Running (%dx%d).(%dx%d)", a_rows, a_cols, b_rows, b_cols);
    run_data_parallel_multiplication(a_rows, b_rows);

    return 0;
}
