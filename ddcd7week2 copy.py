a = 1

def myFunc1(x):
    a = 2
    return a + x    #local a

def myFunc2(x):
    return a + x    #global a

print(myFunc1(1))
print(myFunc2(1))