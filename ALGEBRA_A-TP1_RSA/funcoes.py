from random import randrange

def mdcext(a, b):
    resto = a%b 
    q = a//b
    if resto==0:
        return (0,1,b)
    x,y,mdc = mdcext(b,resto)
    return (y,x-q*y,mdc)
    
def exprapida(b,exp,n):
    resp = 1
    pot = b
    while exp>0:
        if exp%2!=0:
            resp = (resp*pot)%n
        exp //= 2
        pot = (pot*pot)%n
    return resp

def modularinverse(a,b):
    x,y,mdc = mdcext(a,b)
    if mdc==1:
        return x%b
    return None

def miller(a,n,n1,t,q): 
    
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
    i = 0
    while i<t:
        if r ==n1:
            return 1
        r = (r**2)%n
        i +=1
    return 0

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

def primoaleatorio(b):
    eprimo = 0 
    exp = 2**b
    while eprimo==0:
        num = randrange(2,exp)
        eprimo = provavelmenteprimo(num,20)
    return num

def codifica(s):
    resultado = 0
    i = 0 
    for letra in s:
        resultado += (256**i)*ord(letra)
        i+=1
    return resultado

def decodifica(a):
    s = ''
    while a!=0:
        letra = chr(a%256)
        s = s + letra
        a= a//256
    return s

def criptografa(n,e,M):
    C = codifica(M)
    C = exprapida(C,e,n)
    return C

def descriptografa(C,n,d):
    M = exprapida(C,d,n)
    M = decodifica(M)
    return M
