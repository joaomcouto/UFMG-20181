from funcoes import mdcext

def modularinverse(a,b):
    x,y,mdc = mdcext(a,b)
    if mdc==1:
        return x%b
    return None

a = int (input())
b = int (input())
inverso = modularinverse(a,b)
print (inverso)