# miniEP1
# Ciro B Rosa, No USP 2320769
# comando para execução do programa: xxx
N = 24
primos = 0
primosEspeciais = 0
crivo = [True for i in range(N+1)]

for i in range(2, N+1):
    if crivo[i] is True:
        primos += 1
        if i % 4 != 3:
            primosEspeciais += 1
        for j in range(i*2, N+1, i):
            crivo[j] = False
print(primos, primosEspeciais)