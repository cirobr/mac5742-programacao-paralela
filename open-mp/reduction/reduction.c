#include <stdio.h>
#include <omp.h>

#define n 1000000

int main(int argc, char *argv[])  {
    int   i, chunk;
    float a[n], b[n], result;

    chunk  = 10;
    result = 0.0;

    for(i = 0; i < n; i++){
        a[i] = i * 1.0;
        b[i] = i * 2.0;
    };

    #pragma omp parallel for        \
        default(shared) private(i)  \
        schedule(static, chunk)     \
        reduction(+:result)

        for(i = 0; i < n; i++){
            result = result + (a[i] * b[i]);
        };

    printf("Final result = %f\n",result);

    return 0;
};
