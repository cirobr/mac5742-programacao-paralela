/*
 * miniEP1
 * Ciro B Rosa, No USP 2320769
 * comando para execução do programa: xxx
*/

#include <stdio.h>
#include <stdlib.h>
//#include <stdbool.h>

#define NN 26

int main(void)
{
    int
            i, j,
            N,
            primos,
            primosEspeciais;

    N = 1 << NN;

    //int crivo[N+1];
    int *crivo;
        crivo = malloc(N * sizeof(int));

    primos = 0;
    primosEspeciais = 0;

    for (i=0; i <= N; i++)
    {
        crivo[i] = 1;
    }

    for (i=2; i <= N; i++)
    {
        if (crivo[i] == 1)
        {
            primos += 1;

            if (i % 4 != 3)
            {
                    primosEspeciais += 1;
            }

            for (j=i*2; j <= N; j = j+i)
            {
                    crivo[j] = 0;
            }
        }
    }

    printf("%d %d \n", primos, primosEspeciais);
    free(crivo);

    return(0);
}
