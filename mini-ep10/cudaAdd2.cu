/*
mini EP 10

NOME: Ciro B Rosa
NUSP: 2320769
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long getMS() {
	struct timespec s;
	clock_gettime(CLOCK_REALTIME, &s);
	return s.tv_sec*1000 + s.tv_nsec/1000000;
}

// number of tests
#define NTESTS 10
#define SEED 123456
#define SIZE 1024

long seqSum(int *, int *);
long cudaSum(int *, int *);

int main() {
	srand(SEED);
	long seqTime = 0;
	long cudaTime = 0;

	int * reference = (int *)malloc(sizeof(int)*SIZE*SIZE);
	int result;

	for(int i = 0; i < NTESTS; i++) {
		for(int j = 0; j < SIZE*SIZE; j++) reference[j] = rand()%1024;
		seqTime += seqSum(reference, &result);
		cudaTime += cudaSum(reference, &result);
	}

	free(reference);

	printf("Average seqTime %ldms\nAvarage cudaTime %ldms\n", seqTime/NTESTS, cudaTime/NTESTS);
}

long seqSum(int *refs, int *res) {
	long t0 = getMS();

	int sum = 0;
	for(int i = 0; i < SIZE*SIZE; i++)
		sum += refs[i];
	*res = sum;

	long tf = getMS();
	return tf-t0;
}

// good tutorial link https://cuda-tutorial.readthedocs.io/en/latest/tutorials/tutorial01/

__global__ void cudaSumGPU(int *ints) {

	// you code goes here
	int numElements = SIZE * SIZE;
	int thread = blockDim.x * blockIdx.x + threadIdx.x;

	int sum = 0;
	int len = numElements / blockDim.x;
	for (int i = 0; i < len; i++){
		int ind = thread * len + i;
		sum += ints[ind];
		}

	int k = numElements + threadIdx.x;
	ints[k] = sum;

	return;
}

long cudaSum(int *refs, int *res) {
	int *cudaRefs;
	int results[SIZE];

	// usem os ulimos 1024 ints da memoria para guardar o resultado
	cudaMalloc(&cudaRefs, sizeof(int)*SIZE*(SIZE+1));
	cudaMemcpy(cudaRefs, refs, sizeof(int)*SIZE*SIZE, cudaMemcpyHostToDevice);

	long t0 = getMS();

	// Experiment here
	int numBlocks = 1;
	int blockSize = SIZE / numBlocks;
	cudaSumGPU<<< numBlocks, blockSize >>>(cudaRefs);

	cudaMemcpy(results, cudaRefs+(SIZE*SIZE), sizeof(int)*SIZE, cudaMemcpyDeviceToHost);

	int sum = 0;
	for(int i = 0; i < SIZE; i++) {
		sum += results[i];
	}
	long tf = getMS();

	cudaFree(cudaRefs);

	printf("res %d   soma %d\n", *res, sum);
	puts((*res == sum)?"OK":"Err, sum mismatch");

	return tf-t0;
}
