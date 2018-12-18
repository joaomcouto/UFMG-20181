from funcoes import *

def descriptografa(C,n,d):
    M = exprapida(C,d,n)
    M = decodifica(M)
    return M

C = int ( input("C="))
n = int (input("n="))
d = int (input("d="))
    
M = descriptografa(C,n,d)
print("A mensagem descriptografa eh:", M)   
    