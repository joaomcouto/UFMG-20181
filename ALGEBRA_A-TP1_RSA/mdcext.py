a = int (input())
b = int (input())

def mdcext(a, b):
    resto = a%b 
    q = a//b
    if resto==0:
        return (0,1,b)
    x,y,mdc = mdcext(b,resto)
    return (y,x-q*y,mdc)
    
x,y,mdc = mdcext(a,b)
print (mdc,x,y)