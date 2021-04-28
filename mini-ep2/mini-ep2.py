# miniEP2 - Versão Turbo
# Ciro B Rosa, No USP 2320769
# comando para execução do programa: time python3 mini-ep2.py
# ep1 original
# vetor crivo de impares

N = 1 << 26
Ni = int(N / 2)                         # quantidade de numeros impares entre 0 e N
primos = 1                              # numero 2 já é contado como primo
primosEspeciais = 1                     # numero 2 já é contado como primo especial
crivo = [True for i in range(Ni)]       # vetor crivo contempla apenas os impares

for i in range(3, N+1, 2):              # visita apenas os numeros impares
    ii = int((i - 1) / 2)
    if crivo[ii] is True:
        primos += 1
        if i % 4 != 3:
            primosEspeciais += 1
        for ji in range(ii, Ni, i):
            crivo[ji] = False

print(primos, primosEspeciais)
