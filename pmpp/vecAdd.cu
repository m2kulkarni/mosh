#include <stdio.h>


// Kernel function to add the elements of two arrays
__global__ void vecAddKernel(float* A, float* B, float* C, int n)
{
    int i = blockDim.x*blockIdx.x + threadIdx.x;
    if(i<n)  
    {
        C[i] = A[i] + B[i];
    }
}

void vecAdd(float* h_A, float*h_B, float* h_C, int n)
{
    int size = n*sizeof(float);
    float *d_A, *d_B, *d_C;

    cudaError_t err = cudaMalloc( (void**) &d_A, size);
    if(err != cudaSuccess)
    {
        printf("%s in %s at line %d\n", cudaGetErrorString(err), __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMalloc( (void**) &d_B, size);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);
    cudaMalloc( (void**) &d_C, size);

    dim3 dimGrid(32,1,1);
    dim3 dimBlock(128,1,1);

    vecAddKernel<<<dimGrid, dimBlock>>>(d_A, d_B, d_C, n);

    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
    for(int i=0; i<n; i++)
        printf("%f ", h_C[i]);
    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
}

int main()
{
    int n = 6;
    float h_A[] = {1,2,3,3,2,4};
    float h_B[] = {1,2,3,4,5,3};
    float h_C[] = {0,0,0,0,0,0};    
    vecAdd(h_A, h_B, h_C, n);
    cudaDeviceReset();
}

