#include <stdio.h>
#include<stdlib.h>

#define MAXSIZE 500                             // definition needed in this version

void vecAdd(float A[MAXSIZE], float B[MAXSIZE], float C[MAXSIZE], int i){
    C[i] = A[i] + B[i];
}

int main(int argc, char **argv){
    int n = 256;                                // vector size (elements of a vector)
    printf("n: %d \n", n);

    int size = n * sizeof(float);               // memory size for a vector
    printf("size: %d \n", size);
    
    float A[n], B[n], C[n];                     // declar vectors (and reserve memory space)
    int i;

    for(i = 0; i < n; i++){                     // fill up with values
        A[i] = rand() / (float) RAND_MAX;
        B[i] = rand() / (float) RAND_MAX;
        vecAdd(A, B, C, i);                     // execute calculation
    }

    for(i = 0; i < n; i++){
        printf("%d  %.3f  %.3f  %.3f \n", i, A[i], B[i], C[i]);
    }

    return 0;
}