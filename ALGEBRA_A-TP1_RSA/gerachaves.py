from funcoes import *


p = primoaleatorio(2048)
q = primoaleatorio(2048)
n = p*q 
phin = (p-1)*(q-1)
e = 65537
d = modularinverse(e,phin)
while d==None:
    e+=2
    d = modularinverse(e,phin)

print("n=",n)
print("p=",p)
print("q=",q)
print("e=",e)
print("d=",d)
