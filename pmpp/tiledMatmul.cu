#include <cuda_runtime.h>
#include <stdlib.h>
#include <stdio.h>
#define TILE_WIDTH 2

void tiledMatmul(float* h_A, float* h_B, float* h_C, int k, int l, int m);

__global__ void tiledMatmulKernel(float* d_A, float* d_B, float* d_C, int k, int l, int m)
{
    __shared__ float* Mds;
    __shared__ float* Nds;

    Mds = (float*)malloc(sizeof(float)*TILE_WIDTH*TILE_WIDTH);
    Nds = (float*)malloc(sizeof(float)*TILE_WIDTH*TILE_WIDTH);

    int bx = blockIdx.x;
    int by = blockIdx.y;
    int tx = threadIdx.x;
    int ty = threadIdx.y;

    int row = by*TILE_WIDTH + ty;  // 0 to k
    int col = bx*TILE_WIDTH + tx; // 0 to m

    float Pval = 0.0f;
    for(int ph=0; ph < ceil(l/TILE_WIDTH); ph++)
    {
        if(row < k && (ph*TILE_WIDTH+tx) < l)
            Mds[ty*TILE_WIDTH + tx] = d_A[row*l + ph*TILE_WIDTH + tx];
        else
            Mds[ty*TILE_WIDTH + tx] = 0.0f;
        
        if(col < m && (ph*TILE_WIDTH+ty) < l)
            Nds[ty*TILE_WIDTH + tx] = d_B[(ph*TILE_WIDTH + ty)*m + col];
        else
            Nds[ty*TILE_WIDTH + tx] = 0.0f;

        __syncthreads();

        for(int i = 0; i < TILE_WIDTH; i++)
        {
            Pval += Mds[ty*TILE_WIDTH + i]*Nds[i*TILE_WIDTH + tx];
        }

    }

    if(row < k && col < m)
        d_C[row*m + col] = Pval;

}

void tiledMatmul(float* h_A, float* h_B, float* h_C, int k, int l, int m)
{
    float *d_A, *d_B, *d_C;
    cudaMalloc((void**)&d_A, sizeof(float)*k*l);
    cudaMemcpy(d_A, h_A, sizeof(float)*k*l, cudaMemcpyHostToDevice);

    cudaMalloc((void**)&d_B, sizeof(float)*l*m);
    cudaMemcpy(d_B, h_B, sizeof(float)*l*m, cudaMemcpyHostToDevice);

    cudaMalloc((void**)&d_C, sizeof(float)*k*l);

    int t = 4;
    dim3 dimGrid(t, t, 1);
    dim3 dimBlock(ceil(m/t), ceil(k/t), 1);
    tiledMatmulKernel<<<dimGrid, dimBlock>>>(d_A, d_B, d_C, k, l, m);
    cudaMemcpy(h_C, d_C, sizeof(float)*k*m, cudaMemcpyDeviceToHost);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);
}

void initOnes(float *M, int a, int b)
{
    for(int i=0; i<a; i++)
    {
        for(int j=0; j<b; j++)
        {
            M[i*b + j] = i;
        }
    }

}

void initIdentity(float *M, int a, int b)
{
    for(int i=0; i<a; i++)
    {
        for(int j=0; j<b; j++)
        {
            if(i==j)
                M[i*b + j] = 2;
        }
    }

}

void printMatrix(float* M, int a, int b)
{
    for(int i=0; i<a; i++)
    {
        for(int j=0; j<b; j++)
        {
            printf("%.2f ", M[i*b + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    int k = 10;
    int l = 10;
    int m = 10;
    float *h_A, *h_B, *h_C;
    h_A = (float*)malloc(sizeof(float)*k*l); // A = kxl matrix
    h_B = (float*)malloc(sizeof(float)*l*m); // B = lxm matrix
    h_C = (float*)malloc(sizeof(float)*k*m); // C = kxm matrix
    initIdentity(h_A, k, l);
    printMatrix(h_A, k, l);
    initOnes(h_B, l, m);
    printMatrix(h_B, l, m);

    tiledMatmul(h_A, h_B, h_C, k, l, m);
    printMatrix(h_C, k, m);

    return 0;
}
