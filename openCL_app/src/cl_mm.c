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


void malloc_matrices(cl_int aRows, cl_int aCols, cl_int bRows, cl_int bCols, int scalar)
{
    int i,j;
    a_data = malloc(sizeof(cl_float)*(aRows*aCols));
    b_data = malloc(sizeof(cl_float)*(bRows*bCols));
    c_data = malloc(sizeof(cl_float)*(aRows*aCols));

    for(i=0; i<aRows; i++) {
        for(j=0; j<aCols; j++) {
            a_data[i*aRows+j] = scalar;
            b_data[i*bRows+j] = scalar;
            c_data[i*aRows+j] = 0;
        }
    }
}

void print_matrices(cl_int aRows, cl_int bRows)
{
    int i,j;
    log_debug("Matrix A (10x10)");
    for (i=0; i<10; i++) {
        for (j=0; j<10; j++)
            printf("%.f ", a_data[i*aRows+j]);
        printf("\n");
    }
    log_debug("Matrix B (10x10)");
    for (i=0; i<10; i++) {
        for (j=0; j<10; j++)
            printf("%.f ", b_data[i*bRows+j]);
        printf("\n");
    }
    log_debug("Matrix C (10x10)");
    for (i=0; i<10; i++) {
        for (j=0; j<10; j++)
            printf("%.f ", c_data[i*aRows+j]);
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

void run_data_parallel_multiplication(cl_int aRows, cl_int bRows)
{
    log_debug("1: Opening kernel file: %s", kernel_src_name);
    fp = fopen(kernel_src_name,"r");
    fseek(fp,0L, SEEK_END);
    file_len = ftell(fp);
    rewind(fp);

    log_debug("2: Reading kernel_src into kernel source");
    kernel_src = malloc(sizeof(char)*(file_len+1));
    read_len = fread(kernel_src,1,file_len,fp);
    kernel_src[file_len+1]='\0';
    if(read_len != file_len) exit(1);

    log_debug("3: Getting platform IDs");
    res = clGetPlatformIDs(1, &platform_id, &num_platforms_returned);
    if (res != CL_SUCCESS) exit(1);

    log_debug("4: Getting device ID (CL_DEVICE_TYPE_GPU)");
    res = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &num_devs_returned);
    if (res != CL_SUCCESS) exit(1);

    properties[0] = CL_CONTEXT_PLATFORM;
    properties[1] = (cl_context_properties) platform_id;
    properties[2] = 0;
    log_debug("5: Creating cntx");
    context = clCreateContext(properties, 1, &device_id, NULL, NULL, &res);
    if (res != CL_SUCCESS) exit(1);

    log_debug("6: Creating command queue with profiling enabled");
    command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &res);
    if (res != CL_SUCCESS) exit(1);

    log_debug("7: Create program with kernel source");
    program = clCreateProgramWithSource(context, 1, (const char **) &kernel_src, NULL, &res);
    if (res != CL_SUCCESS) exit(1);

    log_debug("8: Building program");
    res = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    if (res != CL_SUCCESS) exit(1);

    log_debug("9: Creating kernel");
    kernel = clCreateKernel(program, "cl_mm", &res);
    if (res != CL_SUCCESS) exit(1);

    log_debug("10: Creating matrix buffers");
    A_mem  = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, sizeof(cl_float)*(aRows*bRows), a_data, NULL);
    B_mem  = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, sizeof(cl_float)*(aRows*bRows), b_data, NULL);
    C_mem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float)*(aRows*bRows), NULL, NULL);

    log_debug("11: Setting kernel arguments");
    res  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &A_mem);
    res |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &B_mem);
    res |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &C_mem);
    res |= clSetKernelArg(kernel, 3, sizeof(cl_int), &aRows);
    res |= clSetKernelArg(kernel, 4, sizeof(cl_int), &bRows);
    if (res != CL_SUCCESS) exit(1);

    global_sizes[0] = aRows;
    global_sizes[1] = bRows;
    local_sizes[0] = 8;
    local_sizes[1] = 8;
    log_debug("12: Execute kernel with event for profiling");
    cl_event event;
    res = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_sizes, local_sizes, 0, NULL, &event);
    if (res != CL_SUCCESS) exit(1);
    clWaitForEvents(1, &event); // wait on host thread for commands by event objects to complete

    log_debug("13: Getting profiling information");
    cl_ulong time_start = (cl_ulong) 0;
    cl_ulong time_end = (cl_ulong) 0;
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &time_start, NULL);
    clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &time_end, NULL);

    unsigned long runtimegpu = (unsigned long)((time_end - time_start)); // time in nsec
    log_info("Execution time in: %lu nsec", runtimegpu);

    log_debug("14: Transfering data to host and taking a little nap");
    res = clEnqueueReadBuffer(command_queue, C_mem, CL_TRUE, 0, sizeof(cl_float)*(aRows*bRows),c_data, 0, NULL, NULL);
    clFinish(command_queue);
    clFlush(command_queue);
    cleanup();
    usleep(1000*4000);
}

int main(int argc, char** argv)
{
    cl_int aRows, aCols, bRows, bCols;
    int scalar_value = 3;

    aRows = aCols = bRows = bCols = 200;
    malloc_matrices(aRows, aCols, bRows, bCols, scalar_value);
    log_info("Running (%dx%d).(%dx%d)", aRows, aCols, bRows, bCols);
    run_data_parallel_multiplication(aRows, bRows);

    aRows = bRows = 200;
    aCols = bCols = 300;
    malloc_matrices(aRows, aCols, bRows, bCols, scalar_value);
    log_info("Running (%dx%d).(%dx%d)", aRows, aCols, bRows, bCols);
    run_data_parallel_multiplication(aRows, bRows);

    aRows = bRows = 1000;
    aCols = bCols = 1000;
    malloc_matrices(aRows, aCols, bRows, bCols, scalar_value);
    log_info("Running (%dx%d).(%dx%d)", aRows, aCols, bRows, bCols);
    run_data_parallel_multiplication(aRows, bRows);

    aRows = bRows = 1000;
    aCols = bCols = 1200;
    malloc_matrices(aRows, aCols, bRows, bCols, scalar_value);
    log_info("Running (%dx%d).(%dx%d)", aRows, aCols, bRows, bCols);
    run_data_parallel_multiplication(aRows, bRows);

    return 0;
}
