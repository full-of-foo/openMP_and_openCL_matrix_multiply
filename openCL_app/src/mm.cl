__kernel void cl_mm(__global float *A_mem, __global float *B_mem, __global float *C_mem, int a_rows, int b_rows)
{
    int i = get_global_id(0);
    int j = get_global_id(1);
    int k;

    float value = 0.0;
    for (k=0; k<a_rows; k++)
        value += A_mem[j*a_rows+k] * B_mem[k*b_rows+i];

    C_mem[j*a_rows+i] = value; // write to device mem
}
