from funcoes import *
from random import randrange

def primoaleatorio(b):
    eprimo = 0 
    exp = 2**b
    while eprimo==0:
        num = randrange(2,exp)
        eprimo = provavelmenteprimo(num,20)
    return num

b = int (input("b="))
resultado = primoaleatorio(b)
print (resultado)