def exprapida(b,exp,n):
    resp = 1
    pot = b
    while exp>0:
        if exp%2!=0:
            resp = (resp*pot)%n
        exp //= 2
        pot = (pot*pot)%n
    return resp

b = int (input())
e = int (input())
n = int (input())

resultado = exprapida(b,e,n)
print (resultado)
        
        