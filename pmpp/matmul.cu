#include <stdlib.h>

__global__
void matMulKernel(float* out, float* M, float* N, int n)
{
    int row = blockIdx.x*blockDim.x + threadIdx.x;
    int col = blockIdx.y*blockDim.y + threadIdx.y;

    for(int j=0; j<=n; j++)
    {
        out[row*n+col] += M[row*n + col]*N[k*n+col];
    }
}

void matMul(float *h_out, float* h_M, float* h_N, int n)
{

    float* d_M, d_N, d_out;
    cudaMalloc((void**)&d_M, n*n*sizeof(float));
    cudaMemcpy(d_M, h_M, n*n*sizeof(float), cudaMemcpyHostToDevice)
    cudaMalloc((void**)&d_N, n*n*sizeof(float));
    cudaMemcpy(d_N, h_N, n*n*sizeof(float), cudaMemcpyHostToDevice)

    cudaMalloc((void**)&d_out, n*n*sizeof(float));
    dim3 dimGrid(ceil(n/16.0), ceil(n/16.0));
    dim3 dimBlock(16,16,1);
    matMulKernel<<<dimGrid, dimBlock>>>(d_out, d_M, d_N, n);
    cudaMemcpy(h_out, d_out, n*n*sizeof(float), cudaMemcpyDeviceToHost);

    cudaFree(d_M);
    cudaFree(d_N);
    cudaFree(d_out);
}

int main() 
{
    int n;
    float *h_M, *h_N;
    matMul(h_M, h_N, n);
    cudaDeviceReset();
}
