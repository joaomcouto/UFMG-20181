def codifica(s):
    resultado = 0
    i = 0 
    for letra in s:
        resultado += (256**i)*ord(letra)
        i+=1
    return resultado

s = (input("digite sua mensagem:"))
resultado = codifica(s)
print(resultado)