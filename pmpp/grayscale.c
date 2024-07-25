#define STB_IMAGE_IMPLEMENTATION
#include <stdlib.h>
#include "stb_image.h"

__global__
void colorToGrayscale(unsigned char* Pout, unsigned char* Pin, int width, int height, int channels)
{
    int size = width*height*channels;

    int col = blockIdx.x*blockDim.x + threadIdx.x;
    int row = blockIdx.y*blockDim.y + threadIdx.y;

    if(col < width & row < height)
    {
        int grayoffset = row*width + col;
        int rgboffset = grayoffset*channels;
        unsigned char r = Pin[rgboffset];
        unsigned char g = Pin[rgboffset+1];
        unsigned char b = Pin[rgboffset+2];

        Pout[grayoffset] = 0.21f*r + 0.71f*g + 0.07f*b;
    }
}

int main()
{
    int width, height, channels;

    unsigned char* img = stbi_load("/home/mohit/Pictures/lock2.jpg", &width, &height, &channels, 0);
    if(img==NULL)
    {
        printf("Error in loading image\n");
        return 1;
    }
    int size = width*height*channels;
    printf("%d ", size);

    unsigned char* h_Pin = (unsigned char*)malloc(size*sizeof(unsigned char));
    unsigned char* h_Pout = (unsigned char*)malloc(size*sizeof(unsigned char));
    if(h_Pin==NULL)
    {
        printf("Error in allocating memory\n");
        return 1;
    }
    for(int i=0; i<size; i++)
        h_Pin[i] = img[i];
    stbi_image_free(img);

    unsigned char *d_Pin, *d_Pout;
    cudaMalloc( (void**) &d_Pin, size*sizeof(unsigned char));
    cudaMemcpy(d_Pin, h_Pin, size*sizeof(unsigned char), cudaMemcpyHostToDevice);
    cudaMalloc( (void**) &d_Pout, size*sizeof(unsigned char));

    dim3 dimGrid(ceil(size/256.0),ceil(size/256.0),1);
    dim3 dimBlock(256,256,1);
    
    colorToGrayscale<<<dimGrid, dimBlock>>>(d_Pout, d_Pin, width, height, channels);
    cudaMemcpy(h_Pout, d_Pout, size*sizeof(unsigned char), cudaMemcpyDeviceToHost);


    return 0;

}

