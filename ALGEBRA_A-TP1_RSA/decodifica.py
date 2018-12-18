def decodifica(a):
    s = ''
    while a!=0:
        letra = chr(a%256)
        s = s + letra
        a= a//256
    return s
    
entrada = int(input())
decoded = decodifica(entrada)
print (decoded)