from funcoes import *
from random import randrange

def provavelmenteprimo(n,itera):
    r = 1 
    while itera>0:
        base = randrange(2,n-1)
        resultado = miller(base,n,n-1,0,n-1)
        r = r and resultado
        if r==0:
            return 0
        itera-=1
    return 1

n = int (input("n="))
itera = int (input("numero de iteracoes="))
resultado = provavelmenteprimo(n,itera)
print (resultado)