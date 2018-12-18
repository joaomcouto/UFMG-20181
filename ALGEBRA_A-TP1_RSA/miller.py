from funcoes import exprapida

def miller(a,n,n1,t,q): #
    if n%2==0 :
        if n==2:
            return 1
        return 0
    while q%2==0:
        q //=2
        t +=1
    r = exprapida(a,q,n)
    if r==1:
        return 1
    while i<t:
        if r ==n1:
            return 1
        r = (r**2)%n
        i +=1
    return 0

a = int (input("a="))
n = int (input("n="))
resultado = miller(a,n,n-1,0,n-1)
print (resultado)
    
    