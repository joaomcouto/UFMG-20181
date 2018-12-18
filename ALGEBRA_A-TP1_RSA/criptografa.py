from funcoes import *

def criptografa(n,e,M):
    C = codifica(M)
    C = exprapida(C,e,n)
    return C
    
n = int (input())
e = int (input())
M = input()
C = criptografa(n,e,M)
print (C)
    
    