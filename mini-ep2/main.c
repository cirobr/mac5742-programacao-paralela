/*
 * miniEP2 - Versão Turbo
 * Ciro B Rosa, No USP 2320769
 * comando para execução do programa: cmake . | make | ./mini-ep2
*/

#include <stdio.h>
#include <stdlib.h>
//#include <stdbool.h>

#define NN 26

int main(void)
{
    int
            i, j, ii, ji,
            N, Ni,
            primos,
            primosEspeciais;

    N = 1 << NN;
    Ni = N / 2;

    //int crivo[Ni];
    int *crivo;
        crivo = malloc(Ni * sizeof(int));

    primos = 1;
    primosEspeciais = 1;

    for (i=0; i <= Ni; i++)
    {
        crivo[i] = 1;
    }

    for (i=3; i <= N; i=i+2)
    {
        ii = (i - 1) / 2;
        if (crivo[ii] == 1)
        {
            primos += 1;

            if (i % 4 != 3)
            {
                    primosEspeciais += 1;
            }

            for (ji=ii; ji <= Ni; ji = ji+i)
            {
                    crivo[ji] = 0;
            }
        }
    }

    printf("%d %d \n", primos, primosEspeciais);
    free(crivo);

    return(0);
}
