#include <stdio.h>
#include <cuda.h>
#include <cuda_runtime.h>

__global__ void vecAdd(float *A, float *B, float *C, int numElements) {
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    if (i < numElements) {
        C[i] = A[i] + B[i];
    }
}

int main(int argc, char **argv) {
    //int maxBlockSize = 1024;
    int numElements = 50000;
    int size = numElements * sizeof(float);

    printf("Alocando vetores no host\n");
    float *h_A = (float *) malloc(size);
    float *h_B = (float *) malloc(size);
    float *h_C = (float *) malloc(size);

    if (h_A == NULL || h_B == NULL || h_C == NULL) {
        fprintf(stderr, "Falha em alocar vetores no host\n");
        exit(-1);
    }

    printf("Inicializando vetores no host\n");
    for (int i = 0; i < numElements; ++i) {
        h_A[i] = rand() / (float) (RAND_MAX);
        h_B[i] = rand() / (float) (RAND_MAX);
    }

    float *d_A;
    float *d_B;
    float *d_C;

    printf("Alocando vetores no device\n");

    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_C, size);

    printf("Copiando memória do host para o device\n");
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    // número de threads a serem lançadas na GPU
    int threadsPerBlock = 256;
    int blocksPerGrid = (numElements + threadsPerBlock - 1) / threadsPerBlock;

    printf("Lançando um kernel com %d blocks e %d threads por bloco\n",blocksPerGrid,threadsPerBlock);

    dim3 cudaBlockSize(threadsPerBlock,1,1);
    dim3 cudaGridSize(blocksPerGrid,1,1);

    cudaError_t err = cudaSuccess;
    vecAdd<<<cudaGridSize, cudaBlockSize>>>(d_A, d_B, d_C, numElements);
    err = cudaGetLastError();

    if (err !=cudaSuccess){
        fprintf(stderr,
        "Failed to lauch vectorAdd kernel (error code %s)!\n",
        cudaGetErrorString(err));
        exit(-1);
    }

    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    free(h_A);
    free(h_B);
    free(h_C);

    printf("Fim\n");

    return 0;
}
